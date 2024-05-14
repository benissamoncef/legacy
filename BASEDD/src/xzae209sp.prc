/*E*/
/*  Fichier : $Id: xzae209sp.prc,v 1.5 2020/11/03 16:34:31 pc2dpdy Exp $        Release : $Revision: 1.5 $        Date : $Date: 2020/11/03 16:34:31 $
------------------------------------------------------------------------------------
* ESCOTA *  PROJET PASTRE
------------------------------------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------------------------------------
* FICHIER xzae209sp.prc
------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Changement de type d'une fiche main courante
*  avec prise en compte d'un changement de classe du type.
*
------------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	30/03/07 : Creation 1.1
* JPL	03/04/07 : Version operationnelle 1.2
* JPL	26/10/07 : Plus de suppression d'enregistrement de l'ancien type (fait par la purge)
* 		    ni de changement de type (fait par la validation xzae11) (DEM 720) 1.3
* JMG	03/12/08 : ajout validation dans FMC_ACC 1.4 DEM/852
* LCL   22/04/20 : MOVIS Ajout site local pilotage DEM-SAE93
------------------------------------------------------------------------------------
*/

/*X*/
/*----------------------------------------------------------------------------------
* Service rendu
*
* Modifie dynamiquement le type d'une fiche main courante,
*  en effectuant les operations requises dans les tables des donnees
*  specifiques aux types en cas de changement de classe du type.
*
* Sequence d'appel
* SP	XZAE209_Modifier_Type_Fiche_MC
*
* Arguments en entree
* XDY_Entier	va_NumEvenement_in
* XDY_Octet	va_CleEvenement_in
* XDY_Octet	va_Prevision_Traiter_Clos_in
* XDY_Octet	va_NouveauType_in
*
* Arguments en sortie
*
* Code retour
* XDC_OK
* XDC_NOK		: site local incorrect, insertion ou mise a jour impossible
* XDC_ARG_INV		: argument d'entree null, ou valeur incorrecte
* XZAEC_FMC_PAS_ENR	: pas le droit d'enrichir
* XZAEC_FMC_PAS_RETYPAGE : retypage non autorise
* XZAEC_FMC_INC_RETYPAGE : ancien et nouveau type de classes incompatibles
* <0 erreur sybase (voir Transact SQL p 13-15)
*
* Conditions d'utilisation
*  L'evenement doit etre en prevision ou a traiter.
*
* Fonction
*
*  Si le nouveau type et l'ancien sont de classes differentes alors
*    - creer un enregistrement dans la table des donnees
*      specifiques a la classe du nouveau type si necessaire ;
*    - ou bien supprimer les eventuels enregistrements existants superflus
*      dans la table des donnees specifiques a la classe du nouveau type
*      (cas des vehicules d'un accident) ;
*
*    - Ne pas supprimer l'enregistrement existant dans la table des donnees
*      specifiques a la classe de l'ancien type (type actuel) au cas ou
*      l'operation de retypage ne serait pas conduite jusqu'a son terme.
*      Cet enregistrement devenant "orphelin" lors du changement effectif
*      du type (dans la table Fmc generique) sera supprime lors de la purge.
*
------------------------------------------------------------------------------------
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE209' and type = 'P')
	drop procedure XZAE209
go


create procedure XZAE209
			@va_NumEvenement_in		int  = null,
			@va_CleEvenement_in		tinyint = null,
			@va_Prevision_Traiter_Clos_in	tinyint = null,
			@va_NouveauType_in		smallint = null,
			@va_NomSiteLocal_in             char(2)         = null
as
	declare @vl_Type smallint,
	        @vl_Classe tinyint, @vl_NouvelleClasse tinyint,
	        @vl_SiteLocal T_SITE, @vl_Site T_SITE,
	        @vl_NumVehicule smallint,
	        @vl_Erreur bit


	/*! transfo en parametres nulls bug applix */
	if @va_NumEvenement_in = 0 select @va_NumEvenement_in = null
	if @va_CleEvenement_in = 0 select @va_CleEvenement_in = null
	if @va_Prevision_Traiter_Clos_in = 0 select @va_Prevision_Traiter_Clos_in = null
	if @va_NouveauType_in = 0 select @va_NouveauType_in = null


	/*A initialiser le code d'erreur resultat de l'operation */
	select @vl_Erreur = XDC_NON


	/*A
	 * Controle des parametres obligatoires
	 * ------------------------------------
	 */

	if @va_NumEvenement_in = null or
	   @va_CleEvenement_in = null or
	   @va_Prevision_Traiter_Clos_in = null or
	   @va_NouveauType_in = null
		return XDC_ARG_INV


	/*A
	 * Rechercher le site local
	 * ------------------------
	 */
        /* MOVIS */
        if @va_NomSiteLocal_in = null
                select  @vl_SiteLocal = numero
                from CFG..RES_DIS
                where serveur = @@servername
        else
                select  @vl_SiteLocal = numero
                from CFG..RES_DIS
                where code = @va_NomSiteLocal_in

	if @vl_SiteLocal is null
		return XDC_NOK


	/*A
	 * Verifier l'existence de l'evenement sur le site local
	 * -----------------------------------------------------
	 */

	select
		@vl_Site = sit,
		@vl_Type = type,
		@vl_Classe = classe
	from EXP..FMC_GEN, CFG..TYP_FMC
	where EXP..FMC_GEN.numero = @va_NumEvenement_in and EXP..FMC_GEN.cle = @va_CleEvenement_in
	  and CFG..TYP_FMC.numero = EXP..FMC_GEN.type

	/* si l'evenement n'est pas trouve, il est probablement dans l'etat clos */
	if @@rowcount != 1
		return XZAEC_FMC_PAS_RETYPAGE

	/* si le site n'est pas le site local, l'operateur n'a pas le droit d'enrichir */
	if @vl_Site != @vl_SiteLocal
		return XZAEC_FMC_PAS_ENR


	/*A
	 * Verifier l'existence et la validite du nouveau type
	 * ---------------------------------------------------
	 */

	if @va_NouveauType_in = @vl_Type
		return XDC_OK

	/*A le nouveau type ne peut etre 'Tete de bouchon' */
	if @va_NouveauType_in = XZAEC_FMC_TeteBouchon
		return XDC_ARG_INV

	/*A le nouveau type ne peut etre 'Poste operateur' */
	if @va_NouveauType_in = XZAEC_FMC_PosteOperateur
		return XDC_ARG_INV

	/*A pas de retypage possible d'une fmc close */
	if @va_Prevision_Traiter_Clos_in = XDC_FMC_ETAT_CLOS
		return XZAEC_FMC_PAS_RETYPAGE

	/*A verifier que le nouveau type existe, determiner sa classe */
	select @vl_NouvelleClasse = classe
	from CFG..TYP_FMC
	where numero = @va_NouveauType_in  and  supprime = XDC_FAUX

	if @@rowcount != 1
		return XDC_ARG_INV

	/*A verifier que les classes des deux types sont identiques ou compatibles */
	if @vl_NouvelleClasse != @vl_Classe and
	   not (@vl_Classe = XZAEC_ClasseAccident and
	           @vl_NouvelleClasse = XZAEC_ClasseVehicule) and
	   not (@vl_Classe = XZAEC_ClasseVehicule and
	           @vl_NouvelleClasse = XZAEC_ClasseAccident)
		return XZAEC_FMC_INC_RETYPAGE


	/*A
	 * Effectuer toutes les modifications au sein d'une transaction :
	 *  modifier les engeristrements adequats des tables specifiques
	 * --------------------------------------------------------------
	 */

	begin tran

		/*A
		 * Sur retypage de fmc Vehicule en Accident
		 *  creer un enregistement Accident
		 *  et conserver l'enregistement Vehicule
		 * ----------------------------------------
		 */

		if (@vl_Classe = XZAEC_ClasseVehicule)  and  (@vl_NouvelleClasse = XZAEC_ClasseAccident)
		begin
			if not exists ( select * from EXP..FMC_ACC
					where numero = @va_NumEvenement_in and cle = @va_CleEvenement_in )
			begin
				insert EXP..FMC_ACC (numero, cle, sit,
							personnalites, personnel_ESCOTA, degats_domaine,
							vehicule_ESCOTA, matieres_dangereuses, zz, validation)
				values (@va_NumEvenement_in, @va_CleEvenement_in, @vl_Site,
					   XDC_NON, XDC_NON, XDC_NON,
					   XDC_NON, XDC_NON, 1, getdate())

				if @@rowcount != 1
					select @vl_Erreur = XDC_OUI
			end
		end


		/*A
		 * Sur retypage de fmc Accident en Vehicule
		 *  creer un enregistement Vehicule s'il n'en existe aucun
		 *   sinon ne conserver que le premier vehicule
		 * -------------------------------------------------------
		 */

		if (@vl_Classe = XZAEC_ClasseAccident)  and  (@vl_NouvelleClasse = XZAEC_ClasseVehicule)
		begin
			/* rechercher le vehicule de plus petit numero */
			select @vl_NumVehicule = min (numero_vehicule)
			from EXP..FMC_VEH
			where numero = @va_NumEvenement_in and cle = @va_CleEvenement_in

			if @vl_NumVehicule = null
			begin
				/*B creer le vehicule obligatoire */
				/*! ne pas inserer de chaine NULLe */
				insert EXP..FMC_VEH
					(numero, cle, numero_vehicule, sit, zz,
					bebe, personne_agee, animaux_vivants, vehicule_ESCOTA,
					matieres_dangereuses, commentaire,
					numero_fiche_appel, site_creation)
				values (@va_NumEvenement_in, @va_CleEvenement_in, 1, @vl_Site, 1,
					XDC_NON, XDC_NON, XDC_NON, XDC_NON,
					XDC_CHAINE_VIDE, XDC_CHAINE_VIDE,
					null, @vl_Site)

				if @@rowcount != 1
					select @vl_Erreur = XDC_OUI
			end
			else
				/* supprimer les donnees des vehicules superflus */
				delete EXP..FMC_VEH
				where numero = @va_NumEvenement_in and cle = @va_CleEvenement_in
				  and numero_vehicule > @vl_NumVehicule
		end


	if @vl_Erreur = XDC_OUI
		rollback tran
	else
		commit tran


	if @vl_Erreur = XDC_OUI
		return XDC_NOK

	return XDC_OK
go
