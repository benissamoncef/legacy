/*E*/
/*  Fichier : $Id: xzae62sp.prc,v 1.9 2020/11/03 16:54:24 pc2dpdy Exp $      Release : $Revision: 1.9 $        Date : $Date: 2020/11/03 16:54:24 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae62sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	03/10/94	: Creation	(V 1.1)
* C.T.	10/11/94	: Modif bugs applix (V 1.2)
* B.G.	21/12/94	: Modif test enrichisseur (1.4) 
* C.T.	09/02/95        : Modif update : zz value (V 1.5)
* C.T.	23/09/96        : Retour de XZAEC_FMC_PAS_ENR si pas les droits d'enrichir (V1.6) (CONF 4)
* JMG	08/01/98	: gestion multi-sites (dem/1536) 1.7
* VR	06/10/11	: Ajout vitesse et vitesse_opposee 1.8 (DEM/1015)
* LCL   22/04/20        : MOVIS Ajout site local pilotage DEM-SAE93
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Modifier une fiche de basculement relative a un evenement existant
* 
* Sequence d'appel
* SP	XZAE62_Ecrire_Fiche_Basculement
* 
* Arguments en entree
* XDY_Entier		va_NumEvenement_in
* XDY_Octet		va_CleEvenement_in
* XDY_PR		va_PRfin_in
* XDY_PR		va_PR_in
* XDY_Entier		va_Longueur_in
* XDY_Voie		va_VR_in
* XDY_Voie		va_VM2_in
* XDY_Voie		va_VM1_in
* XDY_Voie		va_VL_in
* XDY_Voie		va_BAU_in
* XDY_Voie		va_VR_I_in
* XDY_Voie		va_VM2_I_in
* XDY_Voie		va_VM1_I_in
* XDY_Voie		va_VL_I_in
* XDY_Voie		va_BAU_I_in
* XDY_Eqt		va_PosteOperateur_in
* Vitesse		vitesse
* Vitesse_opposee	vitesse_opposee
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: evenement non trouve, update impossible,
*                 poste enrichisseur different
* XDC_ARG_INV	: parametres d'entree null
* 
* Conditions d'utilisation
* 
* Fonction
* Modifier dans la table BASCULEMENT
--------------------------------------------------------*/

use PRC
go


drop procedure XZAE62
go


create procedure XZAE62
	@va_NumEvenement_in	int	= null,
	@va_CleEvenement_in	tinyint	= null,
	@va_PRfin_in		T_PR	= null,
	@va_VR_in		T_VOIE	= null,
	@va_VM2_in		T_VOIE	= null,
	@va_VM1_in		T_VOIE	= null,
	@va_VL_in		T_VOIE	= null,
	@va_BAU_in		T_VOIE	= null,
	@va_VR_I_in		T_VOIE	= null,
	@va_VM2_I_in		T_VOIE	= null,
	@va_VM1_I_in		T_VOIE	= null,
	@va_VL_I_in		T_VOIE	= null,
	@va_BAU_I_in		T_VOIE	= null,
	@va_PosteOperateur_in	T_EQUIPEMENT = null,
	@vitesse		tinyint	= null,
	@vitesse_opposee	tinyint = null,
	@va_NomSiteLocal_in             char(2)         = null
as
	declare @vl_Site T_SITE, @vl_typeSite tinyint

	/*! transfo en param…tres nulls bug applix */
	if @va_NumEvenement_in = 0 select @va_NumEvenement_in = null
	if @va_CleEvenement_in = 0 select @va_CleEvenement_in = null
	if @va_PRfin_in = XDC_PR_INCONNU select @va_PRfin_in = null
	if @va_PosteOperateur_in = 0 select @va_PosteOperateur_in = null

	/*A controle parametres en entree */
	if @va_NumEvenement_in  = null or @va_CleEvenement_in = null or
	   @va_PosteOperateur_in = null
		return XDC_ARG_INV

	/*A recherche du site local */
        /* MOVIS */
        if @va_NomSiteLocal_in = null
                select @vl_Site = numero, @vl_typeSite = type
                from CFG..RES_DIS
                where serveur = @@servername
        else
                select @vl_Site = numero, @vl_typeSite = type
                from CFG..RES_DIS
                where code = @va_NomSiteLocal_in

	if @vl_typeSite = XDC_TYPEM_PCS
		return XDC_NOK

	/*A verifier l'existance de l'evenement et la concordance */
	/*A du poste enrichisseur                                 */
	if not exists ( select * from EXP..FMC_GEN 
			where	numero = @va_NumEvenement_in and 
				cle = @va_CleEvenement_in and
				(poste_enrichisseur = @va_PosteOperateur_in or
				 poste_enrichisseur = null ) and
				sit = @vl_Site)
		return XZAEC_FMC_PAS_ENR


	/*A modification de la fiche basculement */
	update EXP..FMC_BAS set 
		localisation_PR_fin = @va_PRfin_in,
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
		zz = zz + 1,
		vitesse = @vitesse,
		vitesse_opposee = @vitesse_opposee
	where	numero = @va_NumEvenement_in and 
		cle = @va_CleEvenement_in and
		sit = @vl_Site

	if @@rowcount != 1
		return XZAEC_FMC_PAS_ENR

	return XDC_OK

go
