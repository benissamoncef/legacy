/*E*/
/*  Fichier : $Id: xzae63sp.prc,v 1.10 2020/11/03 16:54:42 pc2dpdy Exp $      Release : $Revision: 1.10 $        Date : $Date: 2020/11/03 16:54:42 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae63sp.prc
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
* C.T.	24/11/94	: Ajout du test d'existence du plan (V 1.4)
* B.G.	21/12/94	: Modif test enrichisseur (1.4) 
* C.T.	09/02/95        : Modif update : zz value (V 1.6)
* C.T.	23/09/96        : Retour de XZAEC_FMC_PAS_ENR si pas les droits d'enrichir (V1.7) (CONF 4)
* JMG	08/01/98	: gestion multi-sites (dem/1536) 1.8
* LCL   22/04/20        : MOVIS Ajout site local pilotage DEM-SAE93
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Modifier une fiche de delestage relative a un evenement existant
* 
* Sequence d'appel
* SP	XZAE63_Ecrire_Fiche_Delestage
* 
* Arguments en entree
* XDY_Entier		va_NumEvenement_in
* XDY_Octet		va_CleEvenement_in
* XDY_Mot		va_NumPlan_in
* XDY_Nom		va_Hierarchie_in
* XDY_Eqt		va_PosteOperateur_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK		: evenement non trouve, update impossible,
*                 	  poste enrichisseur different
* XDC_ARG_INV		: parametres d'entree null
* XZAEC_PLN_SCR_INC	: plan de secours inconnu
* 
* Conditions d'utilisation
* 
* Fonction
* Modifier dans la table DELESTAGE
-------------------------------------------------------*/

use PRC
go
create procedure XZAE;63
	@va_NumEvenement_in	int	= null,
	@va_CleEvenement_in	tinyint	= null,
	@va_NumPlan_in		smallint= null,
	@va_Hierarchie_in	T_NOM	= null,
	@va_PosteOperateur_in	T_EQUIPEMENT,
	@va_NomSiteLocal_in             char(2)         = null
as
	declare @vl_Site T_SITE, @vl_Operateur T_OPERATEUR,
		@vl_typeSite tinyint,
		@vl_com_fmc_init char(50)

	/*! transfo en param…tres nulls bug applix */
	if @va_NumEvenement_in = 0 select @va_NumEvenement_in = null
	if @va_CleEvenement_in = 0 select @va_CleEvenement_in = null
	if @va_NumPlan_in = 0 select @va_NumPlan_in = null
	if @va_PosteOperateur_in = 0 select @va_PosteOperateur_in = null

	/*A controle parametres en entree */
	if @va_NumEvenement_in  = null or @va_CleEvenement_in = null or
	   @va_PosteOperateur_in = null
		return XDC_ARG_INV

	/*A controle les chaines null */
	if @va_Hierarchie_in = null
		select @va_Hierarchie_in = XDC_CHAINE_VIDE

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

	select @vl_com_fmc_init = com_fmc_init
	from EXP..FMC_GEN
	where   numero = @va_NumEvenement_in and
	cle = @va_CleEvenement_in

	/*A verifier l'existance du plan de secours */
	if @vl_com_fmc_init = null
	begin
		if not exists ( select * from CFG..PLN_SCR
			where numero = @va_NumPlan_in)
			return XZAEC_PLN_SCR_INC
	end

	/*A modification de la fiche delestage */
	update EXP..FMC_DEL set 
		plan_associe = @va_NumPlan_in,
		responsable = @va_Hierarchie_in,
		zz = zz + 1
	where	numero = @va_NumEvenement_in and 
		cle = @va_CleEvenement_in and
		sit = @vl_Site

	if @@rowcount != 1
		return XZAEC_FMC_PAS_ENR

	return XDC_OK

go
