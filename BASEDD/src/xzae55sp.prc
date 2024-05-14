/*E*/
/*  Fichier : $Id: xzae55sp.prc,v 1.9 2020/11/03 16:46:31 pc2dpdy Exp $      Release : $Revision: 1.9 $        Date : $Date: 2020/11/03 16:46:31 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae55sp.prc
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
* B.G.	21/12/94	: Modif test enrichisseur (1.4) 
* C.T.	23/09/96        : Retour de XZAEC_FMC_PAS_ENR si pas les droits d'enrichir (V1.5) (CONF 4)
* C.T.	27/09/96	: Modif interface pour ajout de colonne
*                         largeur,hauteur,poids,contenu (FMC6) (V1.6)
* JMG	01/08/98	: gestion multi-sites (dem/1536) 1.7
* C.T.	17/04/98	: Correction test dans le cas fmc close (1612) 1.8
* LCL   22/04/20        : MOVIS Ajout site local pilotage DEM-SAE93
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Mettre en base une fiche de vehicule lent relative a un 
* evenement existant
* 
* Sequence d'appel
* SP	XZAE55_Ecrire_Fiche_Veh_Lent
* 
* Arguments en entree
* XDY_Entier		va_NumEvenement_in
* XDY_Octet		va_CleEvenement_in
* XDY_Horodate		va_Horodate_in
* XDY_Mot		va_Longueur_in
* XDY_Octet		va_Vitesse_in
* XDY_Mot 		va_Hauteur_in
* XDY_Mot 		va_Largeur_in
* XDY_Mot		va_Poids_in
* char(15)		va_Contenu_in
* XDY_Eqt		va_PosteOperateur_in
* XDY_Octet		va_Previson_Traiter_Clos_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: evenement non trouve, insert ou update impossible,
*                 poste enrichisseur different
* XDC_ARG_INV	: parametres d'entree null
* 
* Conditions d'utilisation
* la date passee en parametre d'entree doit etre ensuite passee
* a la procedure de validation de la fiche main courante (XZAE11)
* 
* Fonction
* Selon la valeur de Prevision_Traiter_Clos :
* 	Prevision :	Inserer ou modifier dans la table VEHICULES_LENTS_HISTORIQUE
* 	Traiter :	Inserer dans la table VEHICULES_LENTS_HISTORIQUE
* 	Clos :		Modifier dans la table VEHICULES_LENTS_HISTORIQUE
---------------------------------------------------------*/

use PRC
go
create procedure XZAE;55
	@va_NumEvenement_in	int	= null,
	@va_CleEvenement_in	tinyint	= null,
	@va_Horodate_in		datetime = null,
	@va_Longueur_in		smallint = null,
	@va_Vitesse_in		tinyint = null,
	@va_Hauteur_in		smallint = null,
	@va_Largeur_in		smallint = null,
	@va_Poids_in		smallint = null,
	@va_Contenu_in		char(25) = null,
	@va_PosteOperateur_in	T_EQUIPEMENT,
	@va_Prevision_Traiter_Clos_in tinyint = null,
	@va_NomSiteLocal_in             char(2)         = null
as
	declare @vl_Site T_SITE, @vl_typeSite tinyint, @vl_SiteFMC T_SITE

	/*! transfo en paramétres nulls bug applix */
	if @va_CleEvenement_in = 0 select @va_CleEvenement_in = null
	if @va_NumEvenement_in = 0 select @va_NumEvenement_in = null
	if @va_Horodate_in = XDC_DATE_NULLE select @va_Horodate_in = null
	if @va_Prevision_Traiter_Clos_in = 0 select @va_Prevision_Traiter_Clos_in = null
	if @va_PosteOperateur_in = 0 select @va_PosteOperateur_in = null
	if @va_Hauteur_in = XDC_INFO_NON_VALUEE select @va_Hauteur_in = null
	if @va_Largeur_in = XDC_INFO_NON_VALUEE select @va_Largeur_in = null
	if @va_Poids_in = XDC_INFO_NON_VALUEE select @va_Poids_in = null
	if @va_Contenu_in = null select @va_Contenu_in = XDC_CHAINE_VIDE

	/*A controle parametres en entree */
	if @va_NumEvenement_in  = null or @va_CleEvenement_in = null or
	   @va_Horodate_in = null or
	   @va_PosteOperateur_in = null or @va_Prevision_Traiter_Clos_in = null
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
	if @va_Prevision_Traiter_Clos_in != XDC_FMC_ETAT_CLOS and
	   not exists ( select * from EXP..FMC_GEN 
			where	numero = @va_NumEvenement_in and 
				cle = @va_CleEvenement_in and
				( poste_enrichisseur = @va_PosteOperateur_in or
				  poste_enrichisseur = null ) and
				sit = @vl_Site)
		return XZAEC_FMC_PAS_ENR

	if @va_Prevision_Traiter_Clos_in = XDC_FMC_ETAT_CLOS and
	   not exists(	select sit
			from EXP..FMC_VLN
			where	numero = @va_NumEvenement_in and
				cle = @va_CleEvenement_in and
				horodate = @va_Horodate_in and
				sit = @vl_Site)
		return XZAEC_FMC_CLOSE_PAS_ENR


	if @va_Prevision_Traiter_Clos_in = XDC_FMC_ETAT_PREV
	begin
		/*A fiche en prevision a inserer si non existante sinon */
		/*A modifier les valeurs                                */
		if exists ( select * from EXP..FMC_VLN
				where 	numero = @va_NumEvenement_in and
					cle = @va_CleEvenement_in and
					sit = @vl_Site)
			update EXP..FMC_VLN set
				horodate = @va_Horodate_in,
				longueur = @va_Longueur_in,
				vitesse = @va_Vitesse_in,
				hauteur = @va_Hauteur_in,
				largeur = @va_Largeur_in,
				poids = @va_Poids_in,
				contenu = @va_Contenu_in
			where	numero = @va_NumEvenement_in and
				cle = @va_CleEvenement_in and
				sit = @vl_Site
		else
			insert EXP..FMC_VLN (numero, cle, horodate, longueur,
			vitesse, sit, hauteur, largeur, poids, contenu)
			values (@va_NumEvenement_in, @va_CleEvenement_in,
			@va_Horodate_in, @va_Longueur_in, @va_Vitesse_in,
			@vl_Site, @va_Hauteur_in, @va_Largeur_in, @va_Poids_in, @va_Contenu_in)
	end
	else if @va_Prevision_Traiter_Clos_in = XDC_FMC_ETAT_TRAI
		/*A insertion d'une fiche a traiter */
		insert EXP..FMC_VLN (numero, cle, horodate, longueur,
		vitesse, sit, hauteur, largeur, poids, contenu)
		values (@va_NumEvenement_in, @va_CleEvenement_in,
		@va_Horodate_in, @va_Longueur_in, @va_Vitesse_in,
		@vl_Site, @va_Hauteur_in, @va_Largeur_in, @va_Poids_in, @va_Contenu_in)
	else if @va_Prevision_Traiter_Clos_in = XDC_FMC_ETAT_CLOS
	begin

		/*A modification de la fiche pour cloturer */
		update EXP..FMC_VLN set
			longueur = @va_Longueur_in,
			vitesse = @va_Vitesse_in,
			hauteur = @va_Hauteur_in,
			largeur = @va_Largeur_in,
			poids = @va_Poids_in,
			contenu = @va_Contenu_in
		where	numero = @va_NumEvenement_in and
			cle = @va_CleEvenement_in and
			sit = @vl_Site and
			horodate = @va_Horodate_in
	end
	else return XDC_ARG_INV

	if @@rowcount != 1
		return XDC_NOK

	return XDC_OK

go
