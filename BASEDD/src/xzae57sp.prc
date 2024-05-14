/*E*/
/*  Fichier : $Id: xzae57sp.prc,v 1.8 2020/11/03 16:52:00 pc2dpdy Exp $      Release : $Revision: 1.8 $        Date : $Date: 2020/11/03 16:52:00 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae57sp.prc
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
* JMG	01/08/98	: gestion multi-sites (dem/1536) 1.7
* LCL   22/04/20        : MOVIS Ajout site local pilotage DEM-SAE93
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Modifier une fiche de fermeture echangeur relative a un evenement existant
* 
* Sequence d'appel
* SP	XZAE57_Ecrire_Fiche_Fermeture_Echangeur
* 
* Arguments en entree
* XDY_Entier		va_NumEvenement_in
* XDY_Octet		va_CleEvenement_in
* XDY_Booleen		va_Entree1_in
* XDY_Booleen		va_Entree2_in
* XDY_Booleen		va_Sortie1_in
* XDY_Booleen		va_Sortie2_in
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
* 
* Fonction
* Modifier dans la table FERMETURE_ECHANGEUR
-------------------------------------------------------*/

use PRC
go
create procedure XZAE;57
	@va_NumEvenement_in	int	= null,
	@va_CleEvenement_in	tinyint	= null,
	@va_Entree1_in		bit,
	@va_Entree2_in		bit,
	@va_Sortie1_in		bit,
	@va_Sortie2_in		bit,
	@va_PosteOperateur_in	T_EQUIPEMENT,
	@va_NomSiteLocal_in             char(2)         = null
as
	declare @vl_Site T_SITE, @vl_typeSite tinyint

	/*! transfo en param…tres nulls bug applix */
	if @va_NumEvenement_in = 0 select @va_NumEvenement_in = null
	if @va_CleEvenement_in = 0 select @va_CleEvenement_in = null
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
				 poste_enrichisseur =null) and
				sit = @vl_Site)
		return XZAEC_FMC_PAS_ENR

	/*A modification de la fiche fermeture echangeur */
	update EXP..FMC_ECH set 
		entree1 = @va_Entree1_in,
		sortie1 = @va_Sortie1_in,
		entree2 = @va_Entree2_in,
		sortie2 = @va_Sortie2_in,
		zz = zz + 1
	where	numero = @va_NumEvenement_in and 
		cle = @va_CleEvenement_in and
		sit = @vl_Site

	if @@rowcount != 1
		return XZAEC_FMC_PAS_ENR

	return XDC_OK

go
