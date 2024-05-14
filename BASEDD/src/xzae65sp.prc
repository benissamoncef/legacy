/*E*/
/*  Fichier : $Id: xzae65sp.prc,v 1.9 2020/11/03 16:55:09 pc2dpdy Exp $        $Revision: 1.9 $        $Date: 2020/11/03 16:55:09 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae65sp.prc
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
* XXX	xx/01/98	: Type site en base RES_DIS ; filtrage PC simplifies (V 1.7)
* JPL	19/03/19	: Dégroupée; historisée : ajout horodate de validation et état FMC (DEM 1327)  1.8
* LCL   22/04/20        : MOVIS Ajout site local pilotage DEM-SAE93
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Modifier une fiche de manifestation relative a un evenement existant
* 
* Sequence d'appel
* SP	XZAE65_Ecrire_Fiche_Manif
* 
* Arguments en entree
* XDY_Entier		va_NumEvenement_in
* XDY_Octet		va_CleEvenement_in
* XDY_Horodate		va_Horodate_in
* XDY_Mot		va_Nombre_in
* XDY_Booleen		va_Perception_in
* XDY_Booleen		va_Violence_in
* XDY_Eqt		va_PosteOperateur_in
* XDY_Octet		va_Previson_Traiter_Clos_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: evenement non trouve, update impossible,
* XZAEC_FMC_PAS_ENR :	poste enrichisseur different
* XDC_ARG_INV	: parametres d'entree null
* 
* Conditions d'utilisation
* 
* Fonction
* Modifier dans la table MANIFESTATION
-------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE65' and type = 'P')
	drop procedure XZAE65
go


create procedure XZAE65
	@va_NumEvenement_in	int	= null,
	@va_CleEvenement_in	tinyint	= null,
	@va_Horodate_in		datetime = null,
	@va_Nombre_in		smallint= null,
	@va_Perception_in	bit,
	@va_Violence_in		bit,
	@va_PosteOperateur_in	T_EQUIPEMENT = null,
	@va_Prevision_Traiter_Clos_in tinyint = null,
	@va_NomSiteLocal_in             char(2)         = null
as
	declare @vl_Site T_SITE, @vl_typeSite tinyint

	/*! transfo en paramÉtres nulls bug applix */
	if @va_NumEvenement_in = 0 select @va_NumEvenement_in = null
	if @va_CleEvenement_in = 0 select @va_CleEvenement_in = null
	if @va_Horodate_in = XDC_DATE_NULLE select @va_Horodate_in = null
	if @va_Prevision_Traiter_Clos_in = 0 select @va_Prevision_Traiter_Clos_in = null
	if @va_PosteOperateur_in = 0 select @va_PosteOperateur_in = null

	/*A controle parametres en entree */
	if @va_NumEvenement_in  = null or @va_CleEvenement_in = null or
	   @va_Horodate_in = null or
	   @va_PosteOperateur_in = null
		return XDC_ARG_INV

	if @va_Prevision_Traiter_Clos_in not in (XDC_FMC_ETAT_PREV, XDC_FMC_ETAT_TRAI, XDC_FMC_ETAT_CLOS)
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
			from EXP..FMC_MNF
			where	numero = @va_NumEvenement_in and
				cle = @va_CleEvenement_in and
				validation = @va_Horodate_in and
				sit = @vl_Site)
		return XZAEC_FMC_CLOSE_PAS_ENR


	if @va_Prevision_Traiter_Clos_in = XDC_FMC_ETAT_PREV
	begin
		/*A fiche en prevision a inserer si non existante sinon */
		/*A modifier les valeurs                                */
		if exists ( select * from EXP..FMC_MNF
				where 	numero = @va_NumEvenement_in and
					cle = @va_CleEvenement_in and
					sit = @vl_Site)
			update EXP..FMC_MNF set
				nombre_de_manifestants = @va_Nombre_in,
				violence = @va_Violence_in,
				perception_peage = @va_Perception_in,
				zz = zz + 1
			where	numero = @va_NumEvenement_in and
				cle = @va_CleEvenement_in and
				sit = @vl_Site
		else
			insert EXP..FMC_MNF (numero, cle, validation,
			nombre_de_manifestants, violence, perception_peage, sit, zz)
			values (@va_NumEvenement_in, @va_CleEvenement_in, @va_Horodate_in,
			@va_Nombre_in, @va_Violence_in, @va_Perception_in, 
			@vl_Site, 1)
	end
	else if @va_Prevision_Traiter_Clos_in = XDC_FMC_ETAT_TRAI
		/*A insertion d'une fiche a traiter */
		insert EXP..FMC_MNF (numero, cle, validation,
		nombre_de_manifestants, violence, perception_peage, sit, zz)
		values (@va_NumEvenement_in, @va_CleEvenement_in, @va_Horodate_in,
		@va_Nombre_in, @va_Violence_in, @va_Perception_in, 
		@vl_Site, 1)
	else if @va_Prevision_Traiter_Clos_in = XDC_FMC_ETAT_CLOS
	begin

		/*A modification de la fiche pour cloturer */
		update EXP..FMC_MNF set
			nombre_de_manifestants = @va_Nombre_in,
			violence = @va_Violence_in,
			perception_peage = @va_Perception_in,
			zz = zz + 1
		where	numero = @va_NumEvenement_in and
			cle = @va_CleEvenement_in and
			sit = @vl_Site and
			validation = @va_Horodate_in
	end
	else return XDC_ARG_INV

	if @@rowcount != 1
		return XDC_NOK

	return XDC_OK

go
