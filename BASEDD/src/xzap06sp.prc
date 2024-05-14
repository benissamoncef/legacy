/*E*/
/*Fichier : $Id: xzap06sp.prc,v 1.10 1998/09/22 13:37:16 gaborit Exp $      Release : $Revision: 1.10 $        Date : $Date: 1998/09/22 13:37:16 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzap06sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Plan d'Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	02 Sep 1994	: Creation
* gaborit	21 Sep 1994	: Rien (v1.2)
* gaborit	21 Sep 1994	: Rien (v1.3)
* volcic	13 Oct 1994	: Modification (v1.4)
* volcic	28 Oct 1994	: Modification reception chaine nulle (v1.5)
* gaborit	28 dec 1994	: transfert fmc_tfm->sad_txt_tfm (v1.6)
* C.T.   	17 Mar 1995	: Modif insert dans SAD_FTM suite a
*                                 l'ajout d'une colonne dans la table (v1.7)
*                                 remarque : reste a faire mise a jour colonne
* C.T.   	11 Avr 1995	: faire mise a jour colonne confidentiel ds SAD_TFM (v1.8)
* P.N.   	18 Oct 1996	: ajout du niveau (dem/1248) v1.9
* Niepceron	07 Jul 1998	: modif separation EXP/HIS (1696) V1.10
* GGY	26/09/23 : Ajout district CA (DEM483)
------------------------------------------------------*/

use PRC
go

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajoute une proposition d'envoi de message a TFM a un plan d'actions
*  (C.I uniquement)
* 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

create procedure XZAP;06

	@NumPA_In		smallint,
	@Ordre_In		smallint,
	@District_In		tinyint,
	@Horodate_In		datetime,
	@Libelle_In		char(25),
	@Remarque_In		char(50)

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Mot 		NumPA_In
* XDY_Mot 		Ordre_In
* XDY_District		District_In
* XDY_Horodate		Horodate_In
* XDY_Nom		Libelle_In
* XDY_Remarque		Remarque_In
*
* ARGUMENTS EN SORTIE :
*
* CODE RETOUR : 
*
*  XDC_OK 
*  XDC_NOK
*
* CONDITION D'UTILISATION :
*
* FONCTION :
*
* Ajoute dans les tables PA_TFM et TEXTE_PA_TFM
*
* MECANISMES :
*
------------------------------------------------------*/


as 
   
   declare @vl_evt int,@vl_cle tinyint, @vl_Confidentiel bit,@vl_Niveau tinyint,
   @vl_Status int, @vl_Site tinyint

#ifdef HIST

	/* Verification du site */
	select  @vl_Site = numero
		from CFG..RES_DIS
		where serveur = @@servername and type = XDC_TYPEM_SD

	if @vl_Site = XDC_CI
		exec @vl_Status = SQL_CI.PRC..XZAP;06
		@NumPA_In		,
		@Ordre_In		,
		@District_In		,
		@Horodate_In		,
		@Libelle_In		,
		@Remarque_In		

	else if @vl_Site = XDC_CA
		exec @vl_Status = SQL_CA.PRC..XZAP;06
		@NumPA_In		,
		@Ordre_In		,
		@District_In		,
		@Horodate_In		,
		@Libelle_In		,
		@Remarque_In		

	if @vl_Status = null
		return XDC_PRC_INC
	else
		return @vl_Status
#else
   /*A 
   ** Test des parametres d'entree :
   **    Si un des parametres d'entree est egal a null alors XDC_NOK
   */

   if      @NumPA_In = null 
   	or @Ordre_In = null 
   	or @District_In = null
	or @Horodate_In = null
      
      return XDC_NOK
   else   
      begin
      /*A arrondi de l'horodate */
      
      select @Horodate_In=dateadd(ms,500,@Horodate_In)
      select @Horodate_In=convert(datetime,
                           convert(char(8),@Horodate_In,3)+' '+
                           convert(char(8),@Horodate_In,8))
   
      /*A recuperation du numero d'evt */
      
      select @vl_evt=evenement, @vl_cle=cle
      from EXP..SAD_PLN
      where numero=@NumPA_In

      if @@rowcount=0 return XDC_NOK
      
      /*A transfert du texte du message */
      
      insert EXP..SAD_TXT_TFM (numero,ordre,morceau,texte)
      select @NumPA_In, @Ordre_In, morceau,texte
      from EXP..FMC_TFM
      where numero=@vl_evt
        and cle=@vl_cle
        and horodate=@Horodate_In

      if @@rowcount=0 return XDC_NOK

      /*A
      ** Ajout dans la table SAD_TFM (PA_TFM) d'une proposition de
      ** message TFM a un plan d'actions
      */
   
      if @Remarque_In = null
         select @Remarque_In = ' '
      
      if @Libelle_In = null
         select @Libelle_In = ' '
      
      select @vl_Niveau=niveau,@vl_Confidentiel=confidentiel from EXP..FMC_TFM
		 where numero=@vl_evt and cle=@vl_cle and
			horodate=@Horodate_In 
      if @@rowcount=0 	
      begin 
      	select @vl_Confidentiel = XDC_NON
      	select @vl_Niveau = null
      end

      insert into EXP..SAD_TFM (numero, ordre, district, remarque, libelle, confidentiel, niveau )
      values ( @NumPA_In, @Ordre_In, @District_In, @Remarque_In, @Libelle_In, @vl_Confidentiel, @vl_Niveau )
  
      return XDC_OK
      
   end
#endif
go
