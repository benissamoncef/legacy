/*E*/
/*  Fichier : $Id: xzae61sp.prc,v 1.18 2020/11/03 16:54:06 pc2dpdy Exp $      $Revision: 1.18 $        $Date: 2020/11/03 16:54:06 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae61sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	04/10/94	: Creation	(V 1.1)
* C.T.	10/11/94	: Modif bugs applix (V 1.2)
* C.T.	23/11/94	: Modif bugs test type evenement 
*                         non teste (V 1.4)
* B.G.	21/12/94	: Modif test enrichisseur (1.5) 
* C.T.	09/02/95        : Modif update : zz value (V 1.6)
* JMG	05/09/96	: Ajout PC simplifies (RADT), (V1.7)
*                         si la fmc passe a traiter alors retour erreur
* C.T.	23/09/96        : Retour de XZAEC_FMC_PAS_ENR si pas les droits d'enrichir (V1.8) (CONF 4)
* JMG	11/02/97	: neutralisations de voies (DEM/1394) ANA64	1.9
* JMG	08/01/98	: gestion multi-sites (dem/1536) 1.10
* C.T.	20/04/98        : Correction en mode fmc close (1612) 1.11
* ESCOTA  2/2/00        : 1.12 update FMC_TRF (voies) uniquement si aucune est neutralisee
* JPL	06/06/07	: Renommage; Prise en compte FMC (classe) Basculement pour travaux 1.13
* VR	05/10/11	: Ajout vitesse et vitesse_opposee 1.14 (DEM/1015)
* JMG	15/15/16	: horo debut et fin continues a null 1.15 (SAGA)
* JMG	08/03/18 	: ajout personnel 1.16
* JPL	06/09/19	: Ajout de l'indicateur de bouchon (mobile) (DEM 1346)  1.17
* LCL   22/04/20        : MOVIS Ajout site local pilotage DEM-SAE93
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Modifier une fiche de travaux relative a un evenement existant
* 
* Sequence d'appel
* SP	XZAE61_Ecrire_Fiche_Travaux
* 
* Arguments en entree
* XDY_Entier		va_NumEvenement_in
* XDY_Octet		va_CleEvenement_in
* XDY_Booleen		va_Nature_in
* XDY_Octet		va_Type_in
* XDY_Booleen		va_Continu_in
* XDY_Horodate		va_HoroDebC_in
* XDY_Horodate		va_HoroFinC_in
* XDY_Booleen		va_Mobile_in
* XDY_Autoroute		va_Autoroute_out
* XDY_PR		va_PRqueueM_in
* XDY_PR		va_PRteteM_in
* XDY_Sens		va_Sens_in
* XDY_PR		va_PRtete_in
* XDY_Booleen		va_VoieRet_in
* XDY_Horodate		va_Horodate_in
* XDY_Eqt		va_PosteOperateur_in
* XDY_Octet		va_Previson_Traiter_Clos_in
* XDY_Voie              va_VR_in
* XDY_Voie              va_VM2_in
* XDY_Voie              va_VM1_in
* XDY_Voie              va_VL_in
* XDY_Voie              va_BAU_in
* XDY_Voie              va_VR_I_in
* XDY_Voie              va_VM2_I_in
* XDY_Voie              va_VM1_I_in
* XDY_Voie              va_VL_I_in
* XDY_Voie              va_BAU_I_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: evenement non trouve, insert ou update impossible,
*                 poste enrichisseur different
* XZAEC_FMC_PAS_ENR : pas enrichissable
* XDC_ARG_INV	: parametres d'entree null
* 
* Conditions d'utilisation
* la date passee en parametre d'entree doit etre ensuite passee
* a la procedure de validation de la fiche main courante (XZAE11)
* 
* Fonction
* Modifier dans la table TRAVAUX
* Selon le cas correspondant a la valeur de Prevision_Traiter_Clos :
* 	Prevision :	Inserer dans la table TRAVAUX_HISTORIQUE
* 	Traiter :	Inserer ou modifier dans la table TRAVAUX_HISTORIQUE
* 	Clos :		Modifier dans la table TRAVAUX_HISTORIQUE
-------------------------------------------------------*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAE61' and type = 'P')
	drop procedure XZAE61
go


create procedure XZAE61
	@va_NumEvenement_in	int	= null,
	@va_CleEvenement_in	tinyint	= null,
	@va_Nature_in		bit 	,
	@va_Type_in		tinyint ,
	@va_Continu_in		bit 	,
	@va_HoroDebC_in		datetime  = null ,
	@va_HoroFinC_in		datetime  = null ,
	@va_Mobile_in		bit 	,
	@va_Autoroute_in	T_AUTOROUTE ,
	@va_PRqueueM_in		T_PR 	,
	@va_PRteteM_in		T_PR 	,
	@va_SensM_in		T_SENS 	,
	@va_PRtete_in		T_PR 	,
	@va_VoiesRet_in		bit 	,
	@va_Horodate_in		datetime = null,
	@va_PosteOperateur_in	T_EQUIPEMENT,
	@va_Prevision_Traiter_Clos_in tinyint = null,
	@va_VR_in               T_VOIE  = null,
	@va_VM2_in              T_VOIE  = null,
	@va_VM1_in              T_VOIE  = null,
	@va_VL_in               T_VOIE  = null,
	@va_BAU_in              T_VOIE  = null,
	@va_VR_I_in             T_VOIE  = null,
	@va_VM2_I_in            T_VOIE  = null,
	@va_VM1_I_in            T_VOIE  = null,
	@va_VL_I_in             T_VOIE  = null,
	@va_BAU_I_in            T_VOIE  = null,
	@va_PRdebut_VR_in       T_PR    = null,
	@va_PRfin_VR_in         T_PR    = null,
	@va_debut_VR_in         datetime = null,
	@va_fin_VR_in           datetime = null,
	@va_PRdebut_VM2_in      T_PR    = null,
	@va_PRfin_VM2_in        T_PR    = null,
	@va_debut_VM2_in        datetime = null,
	@va_fin_VM2_in          datetime = null,
	@va_PRdebut_VM1_in      T_PR    = null,
	@va_PRfin_VM1_in        T_PR    = null,
	@va_debut_VM1_in        datetime = null,
	@va_fin_VM1_in          datetime = null,
	@va_PRdebut_VL_in       T_PR    = null,
	@va_PRfin_VL_in         T_PR    = null,
	@va_debut_VL_in         datetime = null,
	@va_fin_VL_in           datetime = null,
	@va_PRdebut_BAU_in      T_PR    = null,
	@va_PRfin_BAU_in        T_PR    = null,
	@va_debut_BAU_in        datetime = null,
	@va_fin_BAU_in          datetime = null,
	@va_PRdebut_VR_I_in     T_PR    = null,
	@va_PRfin_VR_I_in       T_PR    = null,
	@va_debut_VR_I_in       datetime = null,
	@va_fin_VR_I_in         datetime = null,
	@va_PRdebut_VM2_I_in    T_PR    = null,
	@va_PRfin_VM2_I_in      T_PR    = null,
	@va_debut_VM2_I_in      datetime = null,
	@va_fin_VM2_I_in        datetime = null,
	@va_PRdebut_VM1_I_in    T_PR    = null,
	@va_PRfin_VM1_I_in      T_PR    = null,
	@va_debut_VM1_I_in      datetime = null,
	@va_fin_VM1_I_in        datetime = null,
	@va_PRdebut_VL_I_in     T_PR    = null,
	@va_PRfin_VL_I_in       T_PR    = null,
	@va_debut_VL_I_in       datetime = null,
	@va_fin_VL_I_in         datetime = null,
	@va_PRdebut_BAU_I_in    T_PR    = null,
	@va_PRfin_BAU_I_in      T_PR    = null,
	@va_debut_BAU_I_in      datetime = null,
	@va_fin_BAU_I_in        datetime = null,
	@vitesse		tinyint = null,
	@personnel		tinyint = null,
	@va_bouchon_in		tinyint = null,
        @va_NomSiteLocal_in             char(2)         = null

as

	declare @vl_Site T_SITE,@vl_Erreur int,@VR T_VOIE,@VM2 T_VOIE,@VM1 T_VOIE,@VL  T_VOIE,@BAU T_VOIE,@VR_I T_VOIE,@VM2_I T_VOIE,@VM1_I T_VOIE,@VL_I T_VOIE,@BAU_I T_VOIE  

	/*! transfo en param…tres nulls bug applix */
	if @va_NumEvenement_in = 0 select @va_NumEvenement_in = null
	if @va_CleEvenement_in = 0 select @va_CleEvenement_in = null
	if @va_Type_in = 0 select @va_Type_in = null
	if @va_PosteOperateur_in = 0 select @va_PosteOperateur_in = null
	if @va_Prevision_Traiter_Clos_in = 0 select @va_Prevision_Traiter_Clos_in = null
	if @va_Horodate_in = XDC_DATE_NULLE select @va_Horodate_in = null
	if @va_HoroDebC_in = XDC_DATE_NULLE select @va_HoroDebC_in = null
	if @va_HoroFinC_in = XDC_DATE_NULLE select @va_HoroFinC_in = null
	if @va_Autoroute_in = XDC_AUT_INCONNUE select @va_Autoroute_in = null
	if @va_PRqueueM_in = XDC_PR_INCONNU select @va_PRqueueM_in = null
	if @va_PRteteM_in = XDC_PR_INCONNU select @va_PRteteM_in = null
	if @va_SensM_in = XDC_SENS_INCONNU select @va_SensM_in = null
	if @va_PRtete_in = XDC_PR_INCONNU select @va_PRtete_in = null
	if @va_PRdebut_VR_in = XDC_PR_INCONNU select @va_PRdebut_VR_in = null
	if @va_PRfin_VR_in = XDC_PR_INCONNU select @va_PRfin_VR_in = null
	if @va_debut_VR_in = XDC_DATE_NULLE select @va_debut_VR_in = null
	if @va_fin_VR_in = XDC_DATE_NULLE select @va_fin_VR_in = null
	if @va_PRdebut_VM2_in = XDC_PR_INCONNU select @va_PRdebut_VM2_in = null
	if @va_PRfin_VM2_in = XDC_PR_INCONNU select @va_PRfin_VM2_in = null
	if @va_debut_VM2_in = XDC_DATE_NULLE select @va_debut_VM2_in = null
	if @va_fin_VM2_in = XDC_DATE_NULLE select @va_fin_VM2_in = null
	if @va_PRdebut_VM1_in = XDC_PR_INCONNU select @va_PRdebut_VM1_in = null
	if @va_PRfin_VM1_in = XDC_PR_INCONNU select @va_PRfin_VM1_in = null
	if @va_debut_VM1_in = XDC_DATE_NULLE select @va_debut_VM1_in = null
	if @va_fin_VM1_in = XDC_DATE_NULLE select @va_fin_VM1_in = null
	if @va_PRdebut_VL_in = XDC_PR_INCONNU select @va_PRdebut_VL_in = null
	if @va_PRfin_VL_in = XDC_PR_INCONNU select @va_PRfin_VL_in = null
	if @va_debut_VL_in = XDC_DATE_NULLE select @va_debut_VL_in = null
	if @va_fin_VL_in = XDC_DATE_NULLE select @va_fin_VL_in = null
	if @va_PRdebut_BAU_in = XDC_PR_INCONNU select @va_PRdebut_BAU_in = null
	if @va_PRfin_BAU_in = XDC_PR_INCONNU select @va_PRfin_BAU_in = null
	if @va_debut_BAU_in = XDC_DATE_NULLE select @va_debut_BAU_in = null
	if @va_fin_BAU_in = XDC_DATE_NULLE select @va_fin_BAU_in = null
	if @va_PRdebut_VR_I_in = XDC_PR_INCONNU select @va_PRdebut_VR_I_in = null
	if @va_PRfin_VR_I_in = XDC_PR_INCONNU select @va_PRfin_VR_I_in = null
	if @va_debut_VR_I_in = XDC_DATE_NULLE select @va_debut_VR_I_in = null
	if @va_fin_VR_I_in = XDC_DATE_NULLE select @va_fin_VR_I_in = null
	if @va_PRdebut_VM2_I_in = XDC_PR_INCONNU select @va_PRdebut_VM2_I_in = null
	if @va_PRfin_VM2_I_in = XDC_PR_INCONNU select @va_PRfin_VM2_I_in = null
	if @va_debut_VM2_I_in = XDC_DATE_NULLE select @va_debut_VM2_I_in = null
	if @va_fin_VM2_I_in = XDC_DATE_NULLE select @va_fin_VM2_I_in = null
	if @va_PRdebut_VM1_I_in = XDC_PR_INCONNU select @va_PRdebut_VM1_I_in = null
	if @va_PRfin_VM1_I_in = XDC_PR_INCONNU select @va_PRfin_VM1_I_in = null
	if @va_debut_VM1_I_in = XDC_DATE_NULLE select @va_debut_VM1_I_in = null
	if @va_fin_VM1_I_in = XDC_DATE_NULLE select @va_fin_VM1_I_in = null
	if @va_PRdebut_VL_I_in = XDC_PR_INCONNU select @va_PRdebut_VL_I_in = null
	if @va_PRfin_VL_I_in = XDC_PR_INCONNU select @va_PRfin_VL_I_in = null
	if @va_debut_VL_I_in = XDC_DATE_NULLE select @va_debut_VL_I_in = null
	if @va_fin_VL_I_in = XDC_DATE_NULLE select @va_fin_VL_I_in = null
	if @va_PRdebut_BAU_I_in = XDC_PR_INCONNU select @va_PRdebut_BAU_I_in = null
	if @va_PRfin_BAU_I_in = XDC_PR_INCONNU select @va_PRfin_BAU_I_in = null
	if @va_debut_BAU_I_in = XDC_DATE_NULLE select @va_debut_BAU_I_in = null
	if @va_fin_BAU_I_in = XDC_DATE_NULLE select @va_fin_BAU_I_in = null

	/*A initialisation de la variable locale */
	select @vl_Erreur = XDC_OK

	/*A controle parametres en entree */
	if @va_NumEvenement_in  = null or @va_CleEvenement_in = null or
	   @va_Horodate_in = null or
	   @va_PosteOperateur_in = null or @va_Prevision_Traiter_Clos_in = null
		return XDC_ARG_INV

	if @va_bouchon_in is null  select @va_bouchon_in = XDC_NON
	else if @va_bouchon_in not in (XDC_OUI, XDC_NON)
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

	/*A verifier l'existance de l'evenement et la concordance */
	/*A du poste enrichisseur                                 */
#ifdef PC_SIMPL
	if exists ( select * from EXP..FMC_GEN
			where	EXP..FMC_GEN.numero = @va_NumEvenement_in and 
				EXP..FMC_GEN.cle = @va_CleEvenement_in and
				EXP..FMC_GEN.sit != @vl_Site)
		return XZAEC_FMC_PAS_ENR
#endif
	if not exists ( select * from EXP..FMC_GEN, CFG..TYP_FMC 
			where	EXP..FMC_GEN.numero = @va_NumEvenement_in and 
				EXP..FMC_GEN.cle = @va_CleEvenement_in and
				( EXP..FMC_GEN.poste_enrichisseur = @va_PosteOperateur_in or
				  EXP..FMC_GEN.poste_enrichisseur =null ) and
				CFG..TYP_FMC.numero = EXP..FMC_GEN.type and
				CFG..TYP_FMC.classe in (XZAEC_ClasseTravaux, XZAEC_ClasseBasculTrav) and
				EXP..FMC_GEN.sit = @vl_Site) and 
	   @va_Prevision_Traiter_Clos_in != XDC_FMC_ETAT_CLOS
		return XZAEC_FMC_PAS_ENR

	/* on est en mode clos et le site n'est pas le local : on sort */
	if not exists ( select sit
			from EXP..FMC_TRH 
			where	numero = @va_NumEvenement_in and
				cle = @va_CleEvenement_in and
				horodate = @va_Horodate_in and
				sit = @vl_Site) and
	   @va_Prevision_Traiter_Clos_in = XDC_FMC_ETAT_CLOS
		return XZAEC_FMC_CLOSE_PAS_ENR


	begin tran
                select @VR=VR,@VR_I=VR_I,@VM1=VM1,@VM1_I=VM1_I,@VM2=VM2,@VM2_I=VM2_I,@VL=VL,@VL_I=VL_I,
		       @BAU=BAU,@BAU_I=BAU_I from EXP..FMC_TRF
                where  numero = @va_NumEvenement_in and cle = @va_CleEvenement_in and
					sit = @vl_Site
		if ( @VR=XDC_VOIE_SENS_INVERSE or @VM1=XDC_VOIE_SENS_INVERSE or @VM2=XDC_VOIE_SENS_INVERSE or
		     @VL=XDC_VOIE_SENS_INVERSE or @BAU=XDC_VOIE_SENS_INVERSE or @VR_I=XDC_VOIE_SENS_INVERSE or
		     @VM1_I=XDC_VOIE_SENS_INVERSE or @VM2_I=XDC_VOIE_SENS_INVERSE or @VL_I=XDC_VOIE_SENS_INVERSE or
		     @BAU_I=XDC_VOIE_SENS_INVERSE )
                begin

		  /*A mise a jour de la partie fixe sans les voies */
		  update EXP..FMC_TRF set 
			nature = @va_Nature_in,
			type = @va_Type_in, continu = @va_Continu_in, 
			horodate_debut_planifiee = @va_HoroDebC_in, 
			horodate_fin_planifiee = @va_HoroFinC_in, 
			mobile = @va_Mobile_in,
			autoroute_planifiee = @va_Autoroute_in, 
			PR_queue_planifie = @va_PRqueueM_in, 
			PR_tete_planifie = @va_PRteteM_in, 
			sens_planifie = @va_SensM_in,
			zz = zz + 1,
			PR_debut_VR = @va_PRdebut_VR_in,
			PR_fin_VR = @va_PRfin_VR_in,
			debut_VR = @va_debut_VR_in,
			fin_VR = @va_fin_VR_in,
			PR_debut_VM2 = @va_PRdebut_VM2_in,
			PR_fin_VM2 = @va_PRfin_VM2_in,
			debut_VM2 = @va_debut_VM2_in,
			fin_VM2 = @va_fin_VM2_in,
			PR_debut_VM1 = @va_PRdebut_VM1_in,
			PR_fin_VM1 = @va_PRfin_VM1_in,
			debut_VM1 = @va_debut_VM1_in,
			fin_VM1 = @va_fin_VM1_in,
			PR_debut_VL = @va_PRdebut_VL_in,
			PR_fin_VL = @va_PRfin_VL_in,
			debut_VL = @va_debut_VL_in,
			fin_VL = @va_fin_VL_in,
			PR_debut_BAU= @va_PRdebut_BAU_in,
			PR_fin_BAU = @va_PRfin_BAU_in,
			debut_BAU = @va_debut_BAU_in,
			fin_BAU = @va_fin_BAU_in,
			PR_debut_VR_I = @va_PRdebut_VR_I_in,
			PR_fin_VR_I = @va_PRfin_VR_I_in,
			debut_VR_I = @va_debut_VR_I_in,
			fin_VR_I = @va_fin_VR_I_in,
			PR_debut_VM2_I = @va_PRdebut_VM2_I_in,
			PR_fin_VM2_I = @va_PRfin_VM2_I_in,
			debut_VM2_I = @va_debut_VM2_I_in,
			fin_VM2_I = @va_fin_VM2_I_in,
			PR_debut_VM1_I = @va_PRdebut_VM1_I_in,
			PR_fin_VM1_I = @va_PRfin_VM1_I_in,
			debut_VM1_I = @va_debut_VM1_I_in,
			fin_VM1_I = @va_fin_VM1_I_in,
			PR_debut_VL_I = @va_PRdebut_VL_I_in,
			PR_fin_VL_I = @va_PRfin_VL_I_in,
			debut_VL_I = @va_debut_VL_I_in,
			fin_VL_I = @va_fin_VL_I_in,
			PR_debut_BAU_I= @va_PRdebut_BAU_I_in,
			PR_fin_BAU_I = @va_PRfin_BAU_I_in,
			debut_BAU_I = @va_debut_BAU_I_in,
			fin_BAU_I = @va_fin_BAU_I_in
		  where	numero = @va_NumEvenement_in and cle = @va_CleEvenement_in and
			sit = @vl_Site
                end

		else
		begin
		  /*A mise a jour de la partie fixe avec les voies dans le cas ou on n'y a jamais touche  */
		  update EXP..FMC_TRF set 
			nature = @va_Nature_in,
			type = @va_Type_in, continu = @va_Continu_in, 
			horodate_debut_planifiee = @va_HoroDebC_in, 
			horodate_fin_planifiee = @va_HoroFinC_in, 
			mobile = @va_Mobile_in,
			autoroute_planifiee = @va_Autoroute_in, 
			PR_queue_planifie = @va_PRqueueM_in, 
			PR_tete_planifie = @va_PRteteM_in, 
			sens_planifie = @va_SensM_in,
			zz = zz + 1,
			VR = @va_VR_in,
			VM2 = @va_VM2_in,
			VM1 = @va_VM1_in,
			VL = @va_VL_in,
			BAU = @va_BAU_in,
			VR_I = @va_VR_I_in,
			VM2_I = @va_VM2_I_in,
			VM1_I = @va_VM1_I_in,
			VL_I = @va_VL_I_in,
			BAU_I = @va_BAU_I_in,
			PR_debut_VR = @va_PRdebut_VR_in,
			PR_fin_VR = @va_PRfin_VR_in,
			debut_VR = @va_debut_VR_in,
			fin_VR = @va_fin_VR_in,
			PR_debut_VM2 = @va_PRdebut_VM2_in,
			PR_fin_VM2 = @va_PRfin_VM2_in,
			debut_VM2 = @va_debut_VM2_in,
			fin_VM2 = @va_fin_VM2_in,
			PR_debut_VM1 = @va_PRdebut_VM1_in,
			PR_fin_VM1 = @va_PRfin_VM1_in,
			debut_VM1 = @va_debut_VM1_in,
			fin_VM1 = @va_fin_VM1_in,
			PR_debut_VL = @va_PRdebut_VL_in,
			PR_fin_VL = @va_PRfin_VL_in,
			debut_VL = @va_debut_VL_in,
			fin_VL = @va_fin_VL_in,
			PR_debut_BAU= @va_PRdebut_BAU_in,
			PR_fin_BAU = @va_PRfin_BAU_in,
			debut_BAU = @va_debut_BAU_in,
			fin_BAU = @va_fin_BAU_in,
			PR_debut_VR_I = @va_PRdebut_VR_I_in,
			PR_fin_VR_I = @va_PRfin_VR_I_in,
			debut_VR_I = @va_debut_VR_I_in,
			fin_VR_I = @va_fin_VR_I_in,
			PR_debut_VM2_I = @va_PRdebut_VM2_I_in,
			PR_fin_VM2_I = @va_PRfin_VM2_I_in,
			debut_VM2_I = @va_debut_VM2_I_in,
			fin_VM2_I = @va_fin_VM2_I_in,
			PR_debut_VM1_I = @va_PRdebut_VM1_I_in,
			PR_fin_VM1_I = @va_PRfin_VM1_I_in,
			debut_VM1_I = @va_debut_VM1_I_in,
			fin_VM1_I = @va_fin_VM1_I_in,
			PR_debut_VL_I = @va_PRdebut_VL_I_in,
			PR_fin_VL_I = @va_PRfin_VL_I_in,
			debut_VL_I = @va_debut_VL_I_in,
			fin_VL_I = @va_fin_VL_I_in,
			PR_debut_BAU_I= @va_PRdebut_BAU_I_in,
			PR_fin_BAU_I = @va_PRfin_BAU_I_in,
			debut_BAU_I = @va_debut_BAU_I_in,
			fin_BAU_I = @va_fin_BAU_I_in
		  where	numero = @va_NumEvenement_in and cle = @va_CleEvenement_in and
			sit = @vl_Site
                end

		/*A mise a jour de la partie historisee */
		if @@rowcount != 1
			select @vl_Erreur = XDC_NOK

		else if @va_Prevision_Traiter_Clos_in = XDC_FMC_ETAT_PREV
		begin
			/*A fiche en prevision a inserer si non existante sinon */
			/*A modifier les valeurs                                */
			if exists ( select * from EXP..FMC_TRH
					where 	numero = @va_NumEvenement_in and
						cle = @va_CleEvenement_in and
						sit = @vl_Site)
				update EXP..FMC_TRH set
					horodate = @va_Horodate_in,
					localisation_tete = @va_PRtete_in,
					voies_retrecies = @va_VoiesRet_in,
					vitesse = @vitesse,
					personnel = @personnel,
					bouchon = @va_bouchon_in
				where	numero = @va_NumEvenement_in and
					cle = @va_CleEvenement_in and
					sit = @vl_Site
			else
				insert EXP..FMC_TRH (numero, cle, horodate, localisation_tete,
				voies_retrecies, sit, vitesse, personnel, bouchon)
				values (@va_NumEvenement_in, @va_CleEvenement_in,
				@va_Horodate_in, @va_PRtete_in,  @va_VoiesRet_in, @vl_Site, @vitesse, @personnel, @va_bouchon_in)
		end
		else if @va_Prevision_Traiter_Clos_in = XDC_FMC_ETAT_TRAI
			if (not exists (select * from EXP..FMC_TRH
					where   numero = @va_NumEvenement_in and
					 cle = @va_CleEvenement_in and
					 sit = @vl_Site and
					 horodate = @va_Horodate_in))
			begin
			/*A insertion d'une fiche a traiter */
			insert EXP..FMC_TRH (numero, cle, horodate, localisation_tete,
			voies_retrecies, sit, vitesse, personnel, bouchon)
			values (@va_NumEvenement_in, @va_CleEvenement_in,
			@va_Horodate_in, @va_PRtete_in,  @va_VoiesRet_in, @vl_Site, @vitesse, @personnel, @va_bouchon_in)
			end

		else if @va_Prevision_Traiter_Clos_in = XDC_FMC_ETAT_CLOS
		begin
			/*A modification de la fiche pour cloturer */
			if not exists (select * from EXP..FMC_TRH
				where   numero = @va_NumEvenement_in and
						cle = @va_CleEvenement_in and
						sit = @vl_Site and
					horodate = @va_Horodate_in)
			begin
			update EXP..FMC_TRH set
				localisation_tete = @va_PRtete_in,
				voies_retrecies = @va_VoiesRet_in,
				vitesse = @vitesse,
				personnel = @personnel,
				bouchon = @va_bouchon_in
			where	numero = @va_NumEvenement_in and
				cle = @va_CleEvenement_in and
				sit = @vl_Site and
				horodate = @va_Horodate_in
			end	
		end
		else select @vl_Erreur =  XDC_ARG_INV

	commit tran

	if @vl_Erreur != XDC_OK
		return @vl_Erreur

	return XDC_OK

go
