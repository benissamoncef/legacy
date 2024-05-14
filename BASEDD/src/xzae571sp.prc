/*e*/
/* Fichier : %W%      Release : %I%        Date : %G%
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae571sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG		06/06/18 : Creation GABEB (DEM/2831
* LCL	10/09/2019 Envoi automatique fin de diffusion RVA pour FMC GABEB DEM1349 DEMSAE-151 1.2
* LCL   22/04/20        : MOVIS Ajout site local pilotage DEM-SAE93
* ABK   26/09/23 	: Ajout district CA (DEM-483)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzae571sp.h" */

/* static char *version = "@(#)xzae571.c	1.1 23/18/94 : xzae571" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE571' and type = 'P')
	drop procedure XZAE571
go

create procedure XZAE571
   @va_Numero_in		int,
   @va_Cle_in		tinyint,
   @va_NomSiteLocal_in             char(2)         = null
   as 
   
/* 
*
* SP    XZAE571SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd        
*					va_Numero_in
*					va_Cle_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf       va_Resultat_out
*
* CODE RETOUR :
*
* 0    XDC_OK
* 1    XDC_NOK 
*
*
* FONCTION :
*
* Mise a jour des donnees de la table FMC_REG
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* VR    13 Jan 2012    : Creation (DEM/1016)
* LCL	10/09/2019 Envoi automatique fin de diffusion RVA pour FMC GABEB DEM1349 1.2
------------------------------------------------------*/
declare @vl_Site T_SITE, @vl_typeSite tinyint,
	@va_ConfirmeSignale_in bit, @va_Validation_in datetime,
	@va_NumAutoroute_in T_AUTOROUTE, @va_PR_in int,
	@va_Sens_in tinyint, @va_TypePtCaracteristique_in tinyint, @va_PtCaracteristique_in tinyint,
	@vl_Position tinyint, @va_VR_in tinyint, @va_VM2_in tinyint, @va_VM1_in tinyint, @va_VL_in tinyint,
	@va_BAU_in tinyint, @va_VR_I_in tinyint, @va_VM2_I_in tinyint, @va_VM1_I_in tinyint, @va_VL_I_in tinyint,
	@va_BAU_I_in tinyint, @va_BAU_Etroite_in tinyint,  @va_BAU_I_Etroite_in tinyint, 
	@va_TypeAnterieur_in smallint, @va_Type_in smallint, @va_Longueur_in int,
	@vl_horodate datetime,
	@vl_last_horo datetime,
	@vl_NumEvt int,
	@vl_CleEvt int,
        @vl_Cmd_TFM tinyint


	/*! transfo en parametres nulls bug applix */
	if @va_Numero_in = 0 select @va_Numero_in = null
	if @va_Cle_in = 0 select @va_Cle_in = null

	select @vl_horodate = getdate()

	/*A controle parametres en entree */
	if @va_Numero_in  = null or @va_Cle_in = null
		return XDC_ARG_INV

	/*A recherche du site local */
        /* MOVIS */
        if @va_NomSiteLocal_in = null
                select  @vl_Site = numero
                from CFG..RES_DIS
                where serveur = @@servername
        else
                select  @vl_Site = numero
                from CFG..RES_DIS
                where code = @va_NomSiteLocal_in


	if @vl_Site is null
		return XDC_NOK

	/*A verifier l'existance de l'evenement */
		if not exists ( select * from EXP..FMC_GEN 
			where	numero = @va_Numero_in and 
				cle = @va_Cle_in)
			return XDC_NOK

	/*mise a jour de FMC_GEN*/
	update EXP..FMC_GEN
	set zz=zz+1,
		fin = @vl_horodate
	where numero = @va_Numero_in and cle = @va_Cle_in

	/* DEM1349 fin de diffusion RVA pour les FMC GABEB */
 	select @vl_NumEvt=evenement,
        	@vl_CleEvt=cle
   	from EXP..ACT_GEN
  	where numero=@va_Numero_in
     	and sit=@va_Cle_in
	and type=XDC_ACT_TFM
	and heure_fin=null
	and heure_lancement_fin=null

	if @@rowcount>0
	begin
		update EXP..ACT_GEN
		set heure_lancement_fin=@vl_horodate
		where  numero=@va_Numero_in
        	and sit=@va_Cle_in
        	and type=XDC_ACT_TFM
        	and heure_fin=null

    		/*A prevenir l'animateur radio de la modification de l'action */
                select @vl_Cmd_TFM = XDC_TERMINE
                if @va_NomSiteLocal_in = XDC_CI
                        exec TASRV_CI...ACRP19 @vl_Cmd_TFM, @va_Numero_in, @vl_Site
                else if @va_NomSiteLocal_in = XDC_DP
                        exec TASRV_DP...ACRP19 @vl_Cmd_TFM, @va_Numero_in, @vl_Site
                else if @va_NomSiteLocal_in = XDC_VC
                        exec TASRV_VC...ACRP19 @vl_Cmd_TFM, @va_Numero_in, @vl_Site
				else if @va_NomSiteLocal_in = XDC_CA
                        exec TASRV_CA...ACRP19 @vl_Cmd_TFM, @va_Numero_in, @vl_Site

	end

	/*recup du dernier enregistrement dans FMC_HIS*/
	select @va_ConfirmeSignale_in = confirme_ou_signale, 
		@va_NumAutoroute_in = autoroute,  @va_PR_in = PR,
		@va_Sens_in =sens, @va_TypePtCaracteristique_in = point_caracteristique, @va_PtCaracteristique_in = numero_point_caracteristique,
		@vl_Position = position_sur_aire_ou_echangeur, @va_VR_in = VR, @va_VM2_in = VM2, @va_VM1_in = VM1, @va_VL_in=VL,
		@va_BAU_in=BAU, @va_VR_I_in=VR_I, @va_VM2_I_in=VM2_I, @va_VM1_I_in=VM1_I, @va_VL_I_in=VL_I,
		@va_BAU_I_in=BAU_I, @va_BAU_Etroite_in=BAU_etroite,  @va_BAU_I_Etroite_in = BAU_I_etroite,
		@va_TypeAnterieur_in=type_anterieur, @va_Type_in=type_FMC_valide, @va_Longueur_in=longueur,
		@vl_horodate = horodate_validation
	from EXP..FMC_HIS
	where numero = @va_Numero_in and cle = @va_Cle_in
	and horodate_validation in (select max(horodate_validation) from EXP..FMC_HIS
		where numero = @va_Numero_in and cle = @va_Cle_in)

	select @vl_horodate = getdate()
	insert EXP..FMC_HIS ( numero, cle, confirme_ou_signale,
			horodate_validation, autoroute, PR, sens, point_caracteristique,
			numero_point_caracteristique,position_sur_aire_ou_echangeur,
			VR, VM2, VM1, VL, BAU, VR_I, VM2_I, VM1_I, VL_I, BAU_I,
			BAU_etroite, BAU_I_etroite, sit,
			type_anterieur, type_FMC_valide, longueur)
	values( @va_Numero_in, @va_Cle_in,
			XDC_FMC_ETAT_TRAI, @vl_horodate,
			@va_NumAutoroute_in, @va_PR_in,
			@va_Sens_in, @va_TypePtCaracteristique_in, @va_PtCaracteristique_in,
			@vl_Position, isnull(@va_VR_in, XDC_VOIE_INCONNUE), isnull(@va_VM2_in,XDC_VOIE_INCONNUE), 
			isnull(@va_VM1_in,XDC_VOIE_INCONNUE), isnull(@va_VL_in,XDC_VOIE_INCONNUE),
			isnull(@va_BAU_in, XDC_VOIE_INCONNUE), isnull(@va_VR_I_in, XDC_VOIE_INCONNUE), 
			isnull(@va_VM2_I_in, XDC_VOIE_INCONNUE), 
			isnull(@va_VM1_I_in, XDC_VOIE_INCONNUE),
			isnull(@va_VL_I_in, XDC_VOIE_INCONNUE),
			isnull(@va_BAU_I_in, XDC_VOIE_INCONNUE), @va_BAU_Etroite_in,  @va_BAU_I_Etroite_in, @vl_Site,
			@va_TypeAnterieur_in, @va_TypeAnterieur_in, @va_Longueur_in)


go
