/*E*/
/*Fichier : $Id: xzai06sp.prc,v 1.8 2019/01/29 11:12:00 pc2dpdy Exp $      Release : $Revision: 1.8 $        Date : $Date: 2019/01/29 11:12:00 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAI * FICHIER xzai06sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Info TFM >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	02 Nov 1994	: Creation
* gaborit	29 dec 1994	: refonte (1.2)
* Niepceron	30 Nov 1998	: intergestionnaire (dem/1724) 1.3
* PNI		11/10/12	: Mise a jour de la date d'echec si date succes null 1.4 DEM1054
* LCL		04/07/17	: Modif PRA
* JMG		12/02/18	: mdif PRA / CI 1.6
* JMG		12/02/18	: correction PRA / CI 1.7
* GGY		26/09/23	: Ajout district CA (DEM483)
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Suppression d'un message TFM de la liste des messages a diffuser
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAI;06
   @va_NumAct_in		int = null,
   @va_CleAct_in		tinyint = null,
   @va_Horodate_in		datetime = null
   as 

/* 
*
* SP	XZAI06SP
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
* Suppression d'un message TFM de la liste des messages a diffuser
*
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK
*
* CONDITIONS D'UTILISATION :
*
* Uniquement si la fin de message a ete decidee par le CI (horodate_lancement_fin)
*
* FONCTION :
*
* MECANISMES :
*
------------------------------------------------------*/

declare	@vl_NumEvt int,
		@vl_operateur  T_OPERATEUR,
		@vl_CleEvt int,
		@vl_Site tinyint, 
		@vl_SiteMaitre T_SITE

	/* Verification du site CI ou CA */
	select  @vl_SiteMaitre = numero
		from CFG..RES_DIS
			where type = XDC_TYPEM_SD
     
      /*B
      ** Le site est le CI
      */
      
      select  @vl_Site = numero
      from CFG..RES_DIS
      where serveur = @@servername

      /* PRA
      if @@servername = XDC_SQLCI and @va_CleAct_in=XDC_CI
      if @vl_Site = XDC_CI and @va_CleAct_in=XDC_CI
      begin
     */ 

	if (@vl_Site = @vl_SiteMaitre) and (@va_CleAct_in != @vl_SiteMaitre)
   begin
        if (@va_CleAct_in = XDC_DP)
                begin
                exec SQL_DS.PRC..XZAI;06  @va_NumAct_in,  @va_CleAct_in,    @va_Horodate_in
        end
        else if (@va_CleAct_in = XDC_VC)
                begin
                exec SQL_DM.PRC..XZAI;06  @va_NumAct_in,  @va_CleAct_in,    @va_Horodate_in
         end 
	return XDC_OK
end
         /*B
         ** L'horodate de fin de lancement est non nulle
         ** => update de l'heure de fin de la table ACT_GEN
         */
      
            update EXP..ACT_GEN
            set heure_fin = @va_Horodate_in
            where numero = @va_NumAct_in
              and sit = @va_CleAct_in
              and type=XDC_ACT_TFM
              and heure_fin = null

            update EXP..ACT_GEN
            set heure_echec = @va_Horodate_in
            where numero = @va_NumAct_in
              and sit = @va_CleAct_in
              and type=XDC_ACT_TFM
              and heure_succes = null
      /*
      end
      
      else
      
         return ( XDC_NOK )
  */

   /*A insert l'action dans TFM_ADA s'il existe une action XDC_ACT_EXPTFM */
   select @vl_NumEvt=evenement,
   	@vl_CleEvt=cle,
   	@vl_operateur=operateur
   from EXP..ACT_GEN
   where numero=@va_NumAct_in
     and sit=@va_CleAct_in
    if exists ( select * from EXP..ACT_GEN,EXP..ACT_TFM_EXP where type=XDC_ACT_EXPTFM and heure_fin=null and numero=actio and actio_tfm=@va_NumAct_in)
   and not exists ( select * from EXP..TFM_ADA where actio=@va_NumAct_in and sit=@va_CleAct_in)
		insert EXP..TFM_ADA (actio,sit) 
		values (@va_NumAct_in,@va_CleAct_in)
	
   
   
   return ( XDC_OK )
         
go  
