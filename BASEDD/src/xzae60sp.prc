/*E*/
/*  Fichier : $Id: xzae60sp.prc,v 1.10 2020/11/03 16:53:20 pc2dpdy Exp $      Release : $Revision: 1.10 $        Date : $Date: 2020/11/03 16:53:20 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae60sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	04/10/94	: Creation	(V 1.1)
* C.T.	10/11/94	: Modif bugs applix (V 1.3)
* C.T.	28/11/94	: Correction sur le test de l'existance (V 1.4)
*                         non teste
* B.G.	21/12/94	: Modif test enrichisseur (1.5) 
* C.T.	23/09/96        : Retour de XZAEC_FMC_PAS_ENR si pas les droits d'enrichir (V1.6) (CONF 4)
* JMG	08/01/98	: gestion multi-sites (dem/1536) 1.8
* C.T.	17/04/98        : Correction cas update fmc close (1612) 1.9
* LCL	11/03/20	: MOVIS DEM-SAE93 1.10
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Modifier une fiche de bouchon relative a un evenement 
* de queue de bouchon existant
* 
* Sequence d'appel
* SP	XZAE60_Ecrire_Fiche_Bouchon
* 
* Arguments en entree
* XDY_Entier		va_NumEvenement_in
* XDY_Octet		va_CleEvenement_in
* XDY_Octet		va_Type_in
* XDY_Horodate		va_Horodate_in
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
* Modifier dans la table QUEUE_BOUCHON
-------------------------------------------------------*/

use PRC
go
create procedure XZAE;60
	@va_NumEvenement_in	int	= null,
	@va_CleEvenement_in	tinyint	= null,
	@va_Type_in		tinyint = null,
	@va_Horodate_in		datetime = null,
	@va_PosteOperateur_in	T_EQUIPEMENT,
	@va_Prevision_Traiter_Clos_in tinyint = null,
	@va_NomSiteLocal_in             char(2)         = null
as
	declare @vl_Site T_SITE, @vl_SiteFMC T_SITE, @vl_typeSite tinyint

	/*! transfo en param…tres nulls bug applix */
	if @va_NumEvenement_in = 0 select @va_NumEvenement_in = null
	if @va_CleEvenement_in = 0 select @va_CleEvenement_in = null
	if @va_Type_in = 0 select @va_Type_in = null
	if @va_PosteOperateur_in = 0 select @va_PosteOperateur_in = null
	if @va_Prevision_Traiter_Clos_in = 0 select @va_Prevision_Traiter_Clos_in = null
	if @va_Horodate_in = XDC_DATE_NULLE select @va_Horodate_in = null

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
		not exists ( select * from EXP..FMC_GEN, CFG..TYP_FMC 
			where	EXP..FMC_GEN.numero = @va_NumEvenement_in and 
				EXP..FMC_GEN.cle = @va_CleEvenement_in and
				(EXP..FMC_GEN.poste_enrichisseur = @va_PosteOperateur_in or
				 EXP..FMC_GEN.poste_enrichisseur = null ) and
				CFG..TYP_FMC.numero = EXP..FMC_GEN.type and
				CFG..TYP_FMC.classe = XZAEC_ClasseBouchon and
				EXP..FMC_GEN.sit = @vl_Site)
		return XZAEC_FMC_PAS_ENR

	if @va_Prevision_Traiter_Clos_in = XDC_FMC_ETAT_CLOS and
	   not exists(	select sit
			from EXP..FMC_BOU
			where	numero = @va_NumEvenement_in and
				cle = @va_CleEvenement_in and
				horodate = @va_Horodate_in and
				sit = @vl_Site)
		return XZAEC_FMC_CLOSE_PAS_ENR


	if @va_Prevision_Traiter_Clos_in = XDC_FMC_ETAT_PREV
	begin
		/*A fiche en prevision a inserer si non existante sinon */
		/*A modifier les valeurs                                */
		if exists ( select * from EXP..FMC_BOU
				where 	numero = @va_NumEvenement_in and
					cle = @va_CleEvenement_in and
					sit = @vl_Site)
			update EXP..FMC_BOU set
				horodate = @va_Horodate_in,
				type = @va_Type_in
			where	numero = @va_NumEvenement_in and
				cle = @va_CleEvenement_in and
				sit = @vl_Site
		else
			insert EXP..FMC_BOU (numero, cle, horodate, type, sit)
			values (@va_NumEvenement_in, @va_CleEvenement_in,
			@va_Horodate_in, @va_Type_in, @vl_Site)
	end
	else if @va_Prevision_Traiter_Clos_in = XDC_FMC_ETAT_TRAI
		/*A insertion d'une fiche a traiter */
		insert EXP..FMC_BOU (numero, cle, horodate, type, sit)
		values (@va_NumEvenement_in, @va_CleEvenement_in,
		@va_Horodate_in, @va_Type_in, @vl_Site)
	else if @va_Prevision_Traiter_Clos_in = XDC_FMC_ETAT_CLOS
	begin
		/*A modification de la fiche pour cloturer */
		update EXP..FMC_BOU set type = @va_Type_in
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
