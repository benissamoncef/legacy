/*E*/
/*  Fichier : $Id: xzae58sp.prc,v 1.9 2020/11/03 16:52:24 pc2dpdy Exp $      Release : $Revision: 1.9 $        Date : $Date: 2020/11/03 16:52:24 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae58sp.prc
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
* JMG	01/08/98	: gestion multi-sites (dem/1536) 1.8
* LCL   22/04/20        : MOVIS Ajout site local pilotage DEM-SAE93
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Modifier une fiche sous concessionnaire relative a un evenement existant
* 
* Sequence d'appel
* SP	XZAE58_Ecrire_Fiche_Sous_Concess
* 
* Arguments en entree
* XZAET_SousConcess	Description
* XDY_Entier		va_NumEvenement_in
* XDY_Octet		va_CleEvenement_in
* XDY_Nom		va_Nom_in
* XDY_Booleen		va_Ferme_in
* XDY_Booleen		va_Penurie_in
* XDY_Octet		va_Carburant_in
* XDY_Eqt		va_PosteOperateur_in
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
* Preciser le carburant uniquement si penurie
* 
* Fonction
* Modifier dans la table SS_CONCESSIONNAIRE
------------------------------------------------------*/

use PRC
go
create procedure XZAE;58
	@va_NumEvenement_in	int	= null,
	@va_CleEvenement_in	tinyint	= null,
	@va_Nom_in		T_NOM	= null,
	@va_Ferme_in		bit,
	@va_Penurie_in		bit,
	@va_Carburant_in	tinyint = null,
	@va_PosteOperateur_in	T_EQUIPEMENT = null,
	@va_NomSiteLocal_in             char(2)         = null
as
	declare @vl_Site T_SITE, @vl_typeSite tinyint

	/*! transfo en param…tres nulls bug applix */
	if @va_NumEvenement_in = 0 select @va_NumEvenement_in = null
	if @va_CleEvenement_in = 0 select @va_CleEvenement_in = null
	if @va_Carburant_in = 0 select @va_Carburant_in = null
	if @va_PosteOperateur_in = 0 select @va_PosteOperateur_in = null

	/*A controle parametres en entree */
	if @va_NumEvenement_in  = null or @va_CleEvenement_in = null or
	   @va_PosteOperateur_in = null or (@va_Penurie_in = XDC_OUI and
	   @va_Carburant_in = null) or (@va_Penurie_in = XDC_NON and
	   @va_Carburant_in != null)
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

	/*A remplacer chaine null par chaine vide */
	if @va_Nom_in = null
		select @va_Nom_in = XDC_CHAINE_VIDE

	/*A modification de la fiche manifestation */
	update EXP..FMC_CNC set 
		nom_ss_concessionnaire = @va_Nom_in,
		ferme = @va_Ferme_in,
		penurie = @va_Penurie_in,
		carburant = @va_Carburant_in,
		zz = zz + 1
	where	numero = @va_NumEvenement_in and 
		cle = @va_CleEvenement_in and
		sit = @vl_Site
	

	if @@rowcount != 1
		return XZAEC_FMC_PAS_ENR

	return XDC_OK
go
