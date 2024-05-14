/*E*/
/*X Fichier : $Id: xzae011sp.prc,v 1.65 2020/11/27 09:07:02 pc2dpdy Exp $        $Revision: 1.65 $        $Date: 2020/11/27 09:07:02 $
------------------------------------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------------------------------------
* FICHIER xzae011sp.prc
------------------------------------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* valide une FMC
*
------------------------------------------------------------------------------------
* HISTORIQUE :
*
* GGY	26/05/23	: Creation => Correction Calcul Plan d'Action Consignation Bouchons (DEM-475)
* ABK	26/09/23 : Ajout district CA (DEM-483)
-----------------------------------------------------*/

/*X*/
/*----------------------------------------------------------------------------------
* Sequence d'appel
* SP	XZAE011_Valider_Fiche_MC
* 
* Arguments en entree
* XDY_Entier	va_NumEvenement_in
* XDY_Octet	va_CleEvenement_in
* XDY_Machine	va_PosteOperateur_in		: numero de la machine de l'operateur
* XDY_Octet	va_Prevision_Traiter_Clos_in	: etat de l'evenement
* XDY_Mot	va_Type_in
* XDY_Horodate	va_DebutPrevu_in
* XDY_Horodate	va_Debut_in
* XDY_Horodate	va_FinPrevu_in
* XDY_Horodate	va_Fin_in
* XDY_Horodate	va_Validation_in
* XDY_Entier	va_NumCause_in
* XDY_Octet	va_CleCause_in
* XDY_Alerte	va_NumAlerte_in
* XDY_Nom	va_Origine_in
* XDY_Eqt	va_PosteEnrich_in		: numero de la machine de l'enrichisseur
* XDY_Booleen	va_ConfirmeSignale_in
* XDY_Autoroute	va_Autoroute_in
* XDY_PR	va_PR_in
* XDY_Sens	va_Sens_in
* XDY_Octet	va_PtCaracteristique_in		: numero du point caracteristique
* XDY_Octet	va_TypePtCaracteristique_in	: type du point caracteristique
* XDY_Octet	va_Position_in			: position sur aire ou echangeur ou lit
* XDY_Voie	va_VR_in
* XDY_Voie	va_VM2_in
* XDY_Voie	va_VM1_in
* XDY_Voie	va_VL_in
* XDY_Voie	va_BAU_in
* XDY_Voie	va_VR_I_in
* XDY_Voie	va_VM2_I_in
* XDY_Voie	va_VM1_I_in
* XDY_Voie	va_VL_I_in
* XDY_Voie	va_BAU_I_in
* XDY_Voie	va_BAU_Etroite_in
* XDY_Voie	va_BAU_I_Etroite_in
* XDY_Octet	va_Degats_in
* XDY_Mot	va_TypeAnterieur_in
* XDY_Octet	va_FausseAlerte_in
* XDY_Entier	va_Longueur_in
* XDY_Entier	va_ComEvInit_in
* XDY_Octet	va_SiteAlerte_in
* XDY_Octet	va_Majeur_in
* XDY_Octet	va_Ecoulement_in
* 
* Arguments en sortie
* 
* Service rendu
* Met en base la fiche correspondante 
* (doit etre appele apres Ecrire_Fiche_xxx)
*

* Code retour
* XDC_OK
* XDC_NOK		: alerte inconnue, cause de l'evenement inconnue, insertion 
*                         impossible, declencher d'un reveil impossible
* XDC_ARG_INV		: argument d'entree null, ou valeur incorrecte
* XDC_PRC_INC		: procedure stockee n'existe pas en base
* XZAEC_FMC_PAS_ENR	: pas le droit d'enrichir
* XZAEC_FMC_ETAT_INC	: l'evenement n'est pas dans l'etat precise en entree
* XZAEC_FMC_DEB_PRV_INC	: date de debut des travaux en prevision doit etre superieur
*                         a la date courante + 10h
* XZAEC_FMC_VOIE_INC	: la description des voies est incorrecte
* XZAEC_FMC_LOC_INC	: la localisation de l'evenement est incorrecte
* XZAEC_FMC_PAS_RETYPAGE : retypage non autorise
* XZAEC_FMC_INC_RETYPAGE : nouveau type pas de meme classe que l'actuel
* < 0          		: erreur sybase
* 
* Conditions d'utilisation
* 
* Fonction
* Controler si le poste operateur a le droit d'enrichir la fiche MC.
* Dans le cas de fiche "a traiter", tester si le poste 
* d'enrichissement est le poste operateur passe en parametre 
* dans le cas ou le poste d'enrichissement est value.
* Update la table EVENEMENT 
* Suivant le cas de fiche :
* 	"A traiter" :	Insert dans la table EVENEMENT_HISTORIQUE
* 	"Prevision" :	Update ou insert dans la table EVENEMENT_HISTORIQUE
* 	"Clos" :	Update dans la table EVENEMENT_HISTORIQUE
* Envoyer un reveil aux operateurs qui traitent l'evenement avec XZAR03
-------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE011' and type = 'P')
	drop procedure XZAE011
go


create procedure XZAE011
	@va_NumEvenement_in		        int	= null,
	@va_CleEvenement_in		        tinyint	= null,
	@va_PosteOperateur_in		      smallint = null,
	@va_Prevision_Traiter_Clos_in	tinyint	= null,
	@va_Type_in			            smallint= null,
	@va_DebutPrevu_in		        datetime = null,
	@va_Debut_in			        datetime = null,
	@va_FinPrevu_in			        datetime = null,
	@va_Fin_in			            datetime = null,
	@va_Validation_in		        datetime = null,
	@va_NumCause_in			        int	= null,
	@va_CleCause_in			        tinyint	= null,
	@va_NumAlerte_in		        int	= null,
	@va_Origine_in			        T_NOM	= null,
	@va_PosteEnrich_in		        smallint = null,
	@va_ConfirmeSignale_in		    bit,
	@va_NumAutoroute_in		        T_AUTOROUTE = null,
	@va_PR_in			            T_PR	= null,
	@va_Sens_in			            T_SENS	= null,
	@va_PtCaracteristique_in	    tinyint	= null,
	@va_TypePtCaracteristique_in	tinyint = null,
	@va_Position_in			        tinyint = null,
	@va_VR_in			            tinyint	= null,
	@va_VM2_in			            tinyint	= null,
	@va_VM1_in			            tinyint	= null,
	@va_VL_in			            tinyint	= null,
	@va_BAU_in			            tinyint	= null,
	@va_VR_I_in			            tinyint	= null,
	@va_VM2_I_in		           	tinyint	= null,
	@va_VM1_I_in		           	tinyint	= null,
	@va_VL_I_in			            tinyint	= null,
	@va_BAU_I_in			        tinyint	= null,
	@va_BAU_Etroite_in		        tinyint	= null,
	@va_BAU_I_Etroite_in	        tinyint	= null,
	@va_Degats_in			        tinyint	= null,
	@va_TypeAnterieur_in	        smallint= null,
	@va_FausseAlerte_in		        tinyint = null,
	@va_Longueur_in			        int	= null,
	@va_ComEvtInit_in		        T_NOM	= null,
	@va_SiteAlerte_in		        tinyint	= null,
	@va_Majeur_in			        tinyint = null,
	@va_Ecoulement_in		        tinyint = null,
	@va_NomSiteLocal_in             char(2) = null
as
	declare @vl_sqlserver char(6), 
          @vl_Status int

	/* Transformation des parametres d'entree nulls */
	if @va_NumEvenement_in = 0 select @va_NumEvenement_in = null
	if @va_CleEvenement_in = 0 select @va_CleEvenement_in = null
	if @va_PosteOperateur_in = 0 select @va_PosteOperateur_in = null
	if @va_Prevision_Traiter_Clos_in = 0 select @va_Prevision_Traiter_Clos_in = null
	if @va_Type_in = 0 select @va_Type_in = null
	if @va_DebutPrevu_in = XDC_DATE_NULLE select @va_DebutPrevu_in = null
	if @va_Debut_in = XDC_DATE_NULLE select @va_Debut_in = null
	if @va_FinPrevu_in = XDC_DATE_NULLE select @va_FinPrevu_in = null
	if @va_Fin_in = XDC_DATE_NULLE select @va_Fin_in = null
	if @va_Validation_in = XDC_DATE_NULLE select @va_Validation_in = null
	if @va_NumCause_in = 0 select @va_NumCause_in = null
	if @va_CleCause_in = 0 select @va_CleCause_in = null
	if @va_NumAlerte_in = 0 select @va_NumAlerte_in = null
	if @va_SiteAlerte_in = 0 select @va_SiteAlerte_in = null
	if @va_PosteEnrich_in = 0 select @va_PosteEnrich_in = null
	if @va_PtCaracteristique_in = 0 select @va_PtCaracteristique_in = null
	if @va_TypePtCaracteristique_in = 0 select @va_TypePtCaracteristique_in = null
	if @va_Position_in = 0 select @va_Position_in = null
	if @va_TypeAnterieur_in = 0 select @va_TypeAnterieur_in = @va_Type_in
	if @va_FausseAlerte_in = null select @va_FausseAlerte_in = XDC_FAUX

	/* Controle des parametres obligatoires */
    if @va_NumEvenement_in = null or @va_CleEvenement_in = null or @va_Validation_in = null or @va_Prevision_Traiter_Clos_in = null or
	   (@va_NumCause_in = null and @va_CleCause_in != null) or (@va_NumCause_in != null and @va_CleCause_in = null)
	    return XDC_ARG_INV

	/*A recherche du site local servant de cle a l'evenement */
    select @vl_sqlserver = serveur_pra from CFG..RES_PRA where numero=@va_CleEvenement_in

	if @vl_sqlserver is null
		return XDC_NOK


    if @vl_sqlserver = XDC_SQLVC
		exec @vl_Status = SQL_DM.PRC..XZAE11   @va_NumEvenement_in,
                                               @va_CleEvenement_in,
                                               @va_PosteOperateur_in,
                                               @va_Prevision_Traiter_Clos_in,
                                               @va_Type_in,
                                               @va_DebutPrevu_in,
                                               @va_Debut_in,
                                               @va_FinPrevu_in,
                                               @va_Fin_in,
                                               @va_Validation_in,
                                               @va_NumCause_in,
                                               @va_CleCause_in,
                                               @va_NumAlerte_in,
                                               @va_Origine_in,
                                               @va_PosteEnrich_in,
                                               @va_ConfirmeSignale_in,
                                               @va_NumAutoroute_in,
                                               @va_PR_in,
                                               @va_Sens_in,
                                               @va_PtCaracteristique_in,
                                               @va_TypePtCaracteristique_in,
                                               @va_Position_in,
                                               @va_VR_in,
                                               @va_VM2_in,
                                               @va_VM1_in,
                                               @va_VL_in,
                                               @va_BAU_in,
                                               @va_VR_I_in,
                                               @va_VM2_I_in,
                                               @va_VM1_I_in,
                                               @va_VL_I_in,
                                               @va_BAU_I_in,
                                               @va_BAU_Etroite_in,
                                               @va_BAU_I_Etroite_in,
                                               @va_Degats_in,
                                               @va_TypeAnterieur_in,
                                               @va_FausseAlerte_in,
                                               @va_Longueur_in,
                                               @va_ComEvtInit_in,
                                               @va_SiteAlerte_in,
                                               @va_Majeur_in,
                                               @va_Ecoulement_in,
                                               @va_NomSiteLocal_in
	else if @vl_sqlserver = XDC_SQLDP
		exec @vl_Status = SQL_DS.PRC..XZAE11   @va_NumEvenement_in,
                                               @va_CleEvenement_in,
                                               @va_PosteOperateur_in,
                                               @va_Prevision_Traiter_Clos_in,
                                               @va_Type_in,
                                               @va_DebutPrevu_in,
                                               @va_Debut_in,
                                               @va_FinPrevu_in,
                                               @va_Fin_in,
                                               @va_Validation_in,
                                               @va_NumCause_in,
                                               @va_CleCause_in,
                                               @va_NumAlerte_in,
                                               @va_Origine_in,
                                               @va_PosteEnrich_in,
                                               @va_ConfirmeSignale_in,
                                               @va_NumAutoroute_in,
                                               @va_PR_in,
                                               @va_Sens_in,
                                               @va_PtCaracteristique_in,
                                               @va_TypePtCaracteristique_in,
                                               @va_Position_in,
                                               @va_VR_in,
                                               @va_VM2_in,
                                               @va_VM1_in,
                                               @va_VL_in,
                                               @va_BAU_in,
                                               @va_VR_I_in,
                                               @va_VM2_I_in,
                                               @va_VM1_I_in,
                                               @va_VL_I_in,
                                               @va_BAU_I_in,
                                               @va_BAU_Etroite_in,
                                               @va_BAU_I_Etroite_in,
                                               @va_Degats_in,
                                               @va_TypeAnterieur_in,
                                               @va_FausseAlerte_in,
                                               @va_Longueur_in,
                                               @va_ComEvtInit_in,
                                               @va_SiteAlerte_in,
                                               @va_Majeur_in,
                                               @va_Ecoulement_in,
                                               @va_NomSiteLocal_in
	else if @vl_sqlserver = XDC_SQLCI
		exec @vl_Status = SQL_CI.PRC..XZAE11   @va_NumEvenement_in,
                                               @va_CleEvenement_in,
                                               @va_PosteOperateur_in,
                                               @va_Prevision_Traiter_Clos_in,
                                               @va_Type_in,
                                               @va_DebutPrevu_in,
                                               @va_Debut_in,
                                               @va_FinPrevu_in,
                                               @va_Fin_in,
                                               @va_Validation_in,
                                               @va_NumCause_in,
                                               @va_CleCause_in,
                                               @va_NumAlerte_in,
                                               @va_Origine_in,
                                               @va_PosteEnrich_in,
                                               @va_ConfirmeSignale_in,
                                               @va_NumAutoroute_in,
                                               @va_PR_in,
                                               @va_Sens_in,
                                               @va_PtCaracteristique_in,
                                               @va_TypePtCaracteristique_in,
                                               @va_Position_in,
                                               @va_VR_in,
                                               @va_VM2_in,
                                               @va_VM1_in,
                                               @va_VL_in,
                                               @va_BAU_in,
                                               @va_VR_I_in,
                                               @va_VM2_I_in,
                                               @va_VM1_I_in,
                                               @va_VL_I_in,
                                               @va_BAU_I_in,
                                               @va_BAU_Etroite_in,
                                               @va_BAU_I_Etroite_in,
                                               @va_Degats_in,
                                               @va_TypeAnterieur_in,
                                               @va_FausseAlerte_in,
                                               @va_Longueur_in,
                                               @va_ComEvtInit_in,
                                               @va_SiteAlerte_in,
                                               @va_Majeur_in,
                                               @va_Ecoulement_in,
                                               @va_NomSiteLocal_in
  	else if @vl_sqlserver = XDC_SQLCA
		exec @vl_Status = SQL_CA.PRC..XZAE11   @va_NumEvenement_in,
                                               @va_CleEvenement_in,
                                               @va_PosteOperateur_in,
                                               @va_Prevision_Traiter_Clos_in,
                                               @va_Type_in,
                                               @va_DebutPrevu_in,
                                               @va_Debut_in,
                                               @va_FinPrevu_in,
                                               @va_Fin_in,
                                               @va_Validation_in,
                                               @va_NumCause_in,
                                               @va_CleCause_in,
                                               @va_NumAlerte_in,
                                               @va_Origine_in,
                                               @va_PosteEnrich_in,
                                               @va_ConfirmeSignale_in,
                                               @va_NumAutoroute_in,
                                               @va_PR_in,
                                               @va_Sens_in,
                                               @va_PtCaracteristique_in,
                                               @va_TypePtCaracteristique_in,
                                               @va_Position_in,
                                               @va_VR_in,
                                               @va_VM2_in,
                                               @va_VM1_in,
                                               @va_VL_in,
                                               @va_BAU_in,
                                               @va_VR_I_in,
                                               @va_VM2_I_in,
                                               @va_VM1_I_in,
                                               @va_VL_I_in,
                                               @va_BAU_I_in,
                                               @va_BAU_Etroite_in,
                                               @va_BAU_I_Etroite_in,
                                               @va_Degats_in,
                                               @va_TypeAnterieur_in,
                                               @va_FausseAlerte_in,
                                               @va_Longueur_in,
                                               @va_ComEvtInit_in,
                                               @va_SiteAlerte_in,
                                               @va_Majeur_in,
                                               @va_Ecoulement_in,
                                               @va_NomSiteLocal_in
	else 
        return XDC_NOK

	return @vl_Status
go
