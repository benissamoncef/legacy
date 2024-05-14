/*E*/
/*Fichier : $Id: xzai05sp.prc,v 1.10 2019/01/29 11:15:25 pc2dpdy Exp $      Release : $Revision: 1.10 $        Date : $Date: 2019/01/29 11:15:25 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAI * FICHIER xzai05sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Info TFM >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	27 Oct 1994	: Creation
* gaborit	28 dec 1994	: refonte
* Niepceron	30 Nov 1998	: intergestionnaire (dem/1724) 1.3
* Niepceron	03 Mar 1999	: correction test existance action EXP_TFM  V.14
* PNI		05/11/2012	: Prise en compte du paraitre diffuser pour gerer la diffusion implice v1.5 DEM1054
* PNI		27/11/12	: retour a la version 1.4 1.6
* LCL		04/07/17	: Modif PRA
* JMG		12/02/18	: modif PRA acces pas qu'au CI 1.8
* JMG		12/02/18	: correction PRA acces pas qu'au CI 1.9
* GGY		26/09/23	: Ajout district CA (DEM483)
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Acquittement d'un message TFM
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAI;05
   @va_NumAct_in		int = null,
   @va_CleAct_in		tinyint = null,
   @va_Horodate_in		datetime = null
   as 

/* 
*
* SP	XZAI05SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Evt		va_NumEvt_in
* XDY_Horodate		va_Horodate_in
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Incremente le nombre de diffusions du message
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
* MECANISMES :
*
------------------------------------------------------*/

declare 
	@vl_operateur  T_OPERATEUR,
	@vl_NumeroAction int,
	@vl_NumEvt int,
	@vl_CleEvt int,
	@vl_Site tinyint,
	@vl_status int, 
	@vl_SiteMaitre T_SITE

	/* Verification du site CI ou CA */
	select  @vl_SiteMaitre = numero
		from CFG..RES_DIS
			where type = XDC_TYPEM_SD


	select  @vl_Site = numero
		from CFG..RES_DIS
			where serveur = @@servername

   /* PRA
   if @va_CleAct_in<>XDC_CI or @@servername<>"SQL_CI"
   if @va_CleAct_in<>XDC_CI or @vl_Site<>XDC_CI
   return XDC_NOK -- l'acquittement ne se fait qu'au CI
  */

   if (@vl_Site = @vl_SiteMaitre) and (@va_CleAct_in != @vl_SiteMaitre)
   begin
	if (@va_CleAct_in = XDC_DP)
		begin
		exec @vl_status = SQL_DS.PRC..XZAI;05  @va_NumAct_in,  @va_CleAct_in,    @va_Horodate_in
	end
   	else if (@va_CleAct_in = XDC_VC)
		begin
		exec @vl_status = SQL_DM.PRC..XZAI;05  @va_NumAct_in,  @va_CleAct_in,    @va_Horodate_in
		end
   	return @vl_status
   end
   select @vl_NumEvt=evenement,
   	@vl_CleEvt=cle,
   	@vl_operateur=operateur
   from EXP..ACT_GEN
  where numero=@va_NumAct_in
     and sit=@va_CleAct_in
     
   update EXP..ACT_TFM
   set nombre_diffusions=nombre_diffusions+1
   where actio=@va_NumAct_in
     and sit=@va_CleAct_in
   
   if @@rowcount=0
   return XDC_NOK -- l'action n'existe pas
   
   update EXP..ACT_GEN
   set heure_succes=@va_Horodate_in
   where numero=@va_NumAct_in
     and sit=@va_CleAct_in
     and heure_succes=null
     and type=XDC_ACT_TFM
     
     
   /*A insert l'action dans TFM_ADA s'il existe une action XDC_ACT_EXP */
   if exists ( select * from EXP..ACT_GEN where evenement=@vl_NumEvt and cle= @vl_CleEvt and type=XDC_ACT_EXP and heure_fin=null)
   begin
	
	/* A s'il n'existe pas d'action XDC_ACT_EXP_TFM alors la creer */

	if not exists ( select * from EXP..ACT_GEN, EXP..ACT_TFM_EXP where evenement=@vl_NumEvt and cle= @vl_CleEvt and type=XDC_ACT_EXPTFM and heure_fin=null and numero=actio and actio_tfm=@va_NumAct_in)
	begin
		
		/*A recherche du numero d'actio suivant */
		select @vl_NumeroAction = isnull(max(numero), 0) + 1 
		from EXP..ACT_GEN
		where sit = @va_CleAct_in
		
		insert EXP..ACT_GEN (numero, evenement, cle, type, heure_lancement,
			heure_succes, heure_echec, prioritaire, heure_lancement_fin,
			heure_fin, operateur, sit, distance_evt, sit_origine)
		values( @vl_NumeroAction, @vl_NumEvt, @vl_CleEvt, 
			XDC_ACT_EXPTFM, @va_Horodate_in,
			null, null, XDC_NON, null,
			null, @vl_operateur, @va_CleAct_in, null, @va_CleAct_in)
		insert EXP..ACT_TFM_EXP (actio, sit, actio_tfm)
		values( @vl_NumeroAction, @va_CleAct_in, @va_NumAct_in)
	end
	if not exists ( select * from EXP..TFM_ADA where actio=@va_NumAct_in and sit=@va_CleAct_in)
		insert EXP..TFM_ADA (actio,sit) 
		values (@va_NumAct_in,@va_CleAct_in)
   end
   
   return ( XDC_OK )
         
go  
