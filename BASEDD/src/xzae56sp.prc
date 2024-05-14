/*E*/
/*  Fichier : $Id: xzae56sp.prc,v 1.9 2020/11/03 16:47:33 pc2dpdy Exp $      Release : $Revision: 1.9 $        Date : $Date: 2020/11/03 16:47:33 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae56sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	04/10/94	: Creation	(V 1.1)
* C.T.	10/11/94	: Modif bugs applix (V 1.3) non teste 
* C.T.	16/11/94	: Correction erreur de codage sur
*                         l'existance de l'evenement (V 1.4) 
*                         non teste
* B.G.	21/12/94	: Modif test enrichisseur (1.5) 
* C.T.	23/09/96        : Retour de XZAEC_FMC_PAS_ENR si pas les droits d'enrichir (V1.6) (CONF 4)
* JMG	01/08/98	: gestion multi-sites (dem/1536) 1.7
* C.T.	17/04/98        : Correction test dans le cas fmc close (1612) (V1.7)
* LCL   22/04/20        : MOVIS Ajout site local pilotage DEM-SAE93
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Mettre en base une fiche de meteo relative a un evenement existant
* 
* Sequence d'appel
* SP	XZAE56_Ecrire_Fiche_Meteo
* 
* Arguments en entree
* XDY_Entier		va_NumEvenement_in
* XDY_Octet		va_CleEvenement_in
* XDY_Horodate		va_Horodate_in
* XDY_PR		va_PRFin_in
* XDY_Octet		va_Perturbation_in	: type de perturbation
* XDY_Mot		va_Visibilite_in
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
* Selon le cas correspondant a la valeur de Prevision_Traiter_Clos :
* 	Prevision :	Inserer ou modifier dans la table METEO_HISTORIQUE
* 	Traiter :	Inserer dans la table METEO_HISTORIQUE
* 	Clos :		Modifier dans la table METEO_HISTORIQUE
---------------------------------------------------------*/

use PRC
go
create procedure XZAE;56
	@va_NumEvenement_in	int	= null,
	@va_CleEvenement_in	tinyint	= null,
	@va_Horodate_in		datetime = null,
	@va_PRFin_in		T_PR = null,
	@va_Perturbation_in	tinyint = null,
	@va_Visibilite_in	smallint = null,
	@va_PosteOperateur_in	T_EQUIPEMENT,
	@va_Prevision_Traiter_Clos_in tinyint = null,
	@va_NomSiteLocal_in             char(2)         = null
as
	declare @vl_Site T_SITE, @vl_typeSite tinyint, @vl_SiteFMC T_SITE

	/*! transfo en param…tres nulls bug applix */
	if @va_NumEvenement_in = 0 select @va_NumEvenement_in = null
	if @va_CleEvenement_in = 0 select @va_CleEvenement_in = null
	if @va_Horodate_in = XDC_DATE_NULLE select @va_Horodate_in = null
	if @va_Perturbation_in = 0 select @va_Perturbation_in = null
	if @va_PRFin_in = XDC_PR_INCONNU select @va_PRFin_in = null
	if @va_Prevision_Traiter_Clos_in = 0 select @va_Prevision_Traiter_Clos_in = null
	if @va_PosteOperateur_in = 0 select @va_PosteOperateur_in = null

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
	select @vl_SiteFMC = sit  
	from EXP..FMC_GEN , CFG..TYP_FMC
	where	EXP..FMC_GEN.numero = @va_NumEvenement_in and 
		EXP..FMC_GEN.cle = @va_CleEvenement_in and
		(EXP..FMC_GEN.poste_enrichisseur = @va_PosteOperateur_in or
		 EXP..FMC_GEN.poste_enrichisseur = null ) and
		CFG..TYP_FMC.numero = EXP..FMC_GEN.type and
		CFG..TYP_FMC.classe = XZAEC_ClasseMeteo and
		EXP..FMC_GEN.sit = @vl_Site

	if @@rowcount = 0 and @va_Prevision_Traiter_Clos_in != XDC_FMC_ETAT_CLOS
		return XZAEC_FMC_PAS_ENR

	if @va_Prevision_Traiter_Clos_in = XDC_FMC_ETAT_CLOS and
	   not exists(	select sit
			from EXP..FMC_MET
			where	numero = @va_NumEvenement_in and
				cle = @va_CleEvenement_in and
				horodate = @va_Horodate_in and
				sit = @vl_Site)
		return XZAEC_FMC_CLOSE_PAS_ENR


	if @va_Prevision_Traiter_Clos_in = XDC_FMC_ETAT_PREV
	begin
		/*A fiche en prevision a inserer si non existante sinon */
		/*A modifier les valeurs                                */
		if exists ( select * from EXP..FMC_MET
				where 	numero = @va_NumEvenement_in and
					cle = @va_CleEvenement_in and
					sit = @vl_Site)
			update EXP..FMC_MET set
				horodate = @va_Horodate_in,
				visibilite = @va_Visibilite_in,
				type_de_perturbation = @va_Perturbation_in,
				PR = @va_PRFin_in
			where	numero = @va_NumEvenement_in and
				cle = @va_CleEvenement_in and
				sit = @vl_Site
		else
			insert EXP..FMC_MET (numero, cle, horodate, visibilite,
			type_de_perturbation, PR, sit)
			values (@va_NumEvenement_in, @va_CleEvenement_in,
			@va_Horodate_in, @va_Visibilite_in, @va_Perturbation_in,
			@va_PRFin_in, @vl_Site)
	end
	else if @va_Prevision_Traiter_Clos_in = XDC_FMC_ETAT_TRAI
		/*A insertion d'une fiche a traiter */
		insert EXP..FMC_MET (numero, cle, horodate, visibilite,
		type_de_perturbation, PR, sit)
		values (@va_NumEvenement_in, @va_CleEvenement_in,
		@va_Horodate_in, @va_Visibilite_in, @va_Perturbation_in,
		@va_PRFin_in, @vl_Site)
	else if @va_Prevision_Traiter_Clos_in = XDC_FMC_ETAT_CLOS
	begin
		/*A modification de la fiche pour cloturer */
		update EXP..FMC_MET set
			visibilite = @va_Visibilite_in,
			type_de_perturbation = @va_Perturbation_in,
			PR = @va_PRFin_in
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
