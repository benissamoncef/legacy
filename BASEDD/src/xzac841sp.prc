/*E*/
/*  Fichier : $Id: xzac841sp.prc,v 1.1 2017/03/08 10:29:33 devgfi Exp $      Release : $Revision: 1.1 $        Date : $Date: 2017/03/08 10:29:33 $
-------------------------------------------------------------------------------------------
* ESCOTA  *  PROJET MIGRAZUR / PASTRE
-------------------------------------------------------------------------------------------
* SOUS-SYSTEME : BASEDD
-------------------------------------------------------------------------------------------
* MODULE TANEU * FICHIER xzac841sp.prc
-------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Tracage des Mises au neutre des PMV et PMVA.
*
-------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	13/02/17 : Creation (DEM 1196)  1.1
-----------------------------------------------------------------------------------------*/


/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU
*
*  Trace dans une table dediee certains affichages commandes aux PMV et aux PMVA.
*  Les affichages actuellement traces sont les neutres Temps de parcours
*  et les neutres par defaut ("3 points").
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL
*
* SP	XZAC841_Tracer_Affichages_PMV_PMVA
*
* PARAMETRES EN ENTREE :
*
* XDY_TypeEqt	va_TypeEqt_in			: Type de l'equipement (PMV ou PMVA)
* XDY_Eqt	va_NumEqt_in			: Numero de l'equipement
* XDY_Horodate	va_Horodate_in			: Horodate de la commande d'affichage.
* XDY_Octet	va_Type_Affichage_in		: Type d'affichage commande a l'equipement
*
*
* PARAMETRES EN SORTIE :
*
* Aucun
*
*
* CODE RETOUR :
*
* XDC_OK
* XDC_NOK           : equipement inconnu
* XDC_ARG_INV       : parametre d'entree obligatoire manquant (Null) ou invalide
* <0                : erreur sybase
*
*
* CONDITIONS D'UTILISATION :
*
*  Cette procedure est utilisee uniquement sur les sites des districts.
*
*  Cette procedure est utilisee uniquement par d'autres procedures,
*  de maniere interne a la base de donnees.
*  De ce fait la pilotabilité de l'equipement, s'il existe, est supposee verifiee.
*
* FONCTION :
*
*  Si un type de mise au neutre tracé est commandé a un PMV ou un PMVA alors
*      si ce type de neutre est déja affiché sur ce panneau alors
*          il n'y a rien a faire (état inchange et deja tracé)
*      sinon
*          marquer la fin d'un éventuel autre type d'affichage tracé, et
*          enregistrer le début du nouvel affichage.
*
*  Si un autre etat est commandé a un PMV ou un PMVA (texte, extinction) alors
*      marquer la fin de tout type d'affichage en cours tracé.
*
-----------------------------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAC841' and type = 'P')
	drop procedure XZAC841
go


create procedure XZAC841
	@va_TypeEqt_in			tinyint		= null,
	@va_NumEqt_in			T_EQUIPEMENT	= null,
	@va_Horodate_in			datetime	= null,
	@va_Type_Affichage_in		tinyint		= null
as
	declare	@vl_Neutre_Demande	tinyint,
		@vl_Neutre_Actuel	tinyint,
		@vl_Debut_Neutre	datetime,
		@vl_Fin_Neutre		datetime,
		@vl_Debut_Der_Action	datetime


	/* Transformation des parametres d'entree nulls */


	/*A
	** Controle des parametres d'entree obligatoires
	*/

	if @va_TypeEqt_in not in (XDC_EQT_PMV, XDC_EQT_PMVA)
		return XDC_ARG_INV

	if @va_TypeEqt_in = XDC_EQT_PMV
	begin
		if @va_Type_Affichage_in in (XDC_PMV_3POINTS, XDC_PMV_HEURE, XDC_PMV_DEFAUT)
			select @vl_Neutre_Demande = XDC_PMV_3POINTS

		else if @va_Type_Affichage_in = XDC_PMV_TDP
			select @vl_Neutre_Demande = XDC_PMV_TDP

		else if @va_Type_Affichage_in not in (XDC_PMV_ETEINDRE, XDC_PMV_TEXTE)
			return XDC_ARG_INV
	end

	else
	if @va_TypeEqt_in = XDC_EQT_PMVA
	begin
		if @va_Type_Affichage_in in (XDC_PMVA_3POINTS, XDC_PMVA_HEURE, XDC_PMVA_DEFAUT)
			select @vl_Neutre_Demande = XDC_PMVA_3POINTS

		else if @va_Type_Affichage_in = XDC_PMVA_TDP
			select @vl_Neutre_Demande = XDC_PMVA_TDP

		else if @va_Type_Affichage_in not in (XDC_PMVA_ETEINDRE, XDC_PMVA_TEXTE)
			return XDC_ARG_INV
	end


	if @va_Horodate_in is null
		select @va_Horodate_in = XDC_DATE_NULLE

	/* Verifier l'existence de l'equipement */
	if not exists (select 1 from CFG..EQT_GEN where type = @va_TypeEqt_in and numero = @va_NumEqt_in)
		return XDC_NOK


	/*  Rechercher la derniere action effectuee sur cet equipement   */
	/*! hormis celle qui viendrait d'etre creee, a l'heure indiquee !*/

	select	@vl_Debut_Der_Action = heure_lancement
	from	EXP..ACT_GEN
	where	type = @va_TypeEqt_in  and  equipement = @va_NumEqt_in
	  and	heure_lancement < @va_Horodate_in
	having	type = @va_TypeEqt_in  and  equipement = @va_NumEqt_in
	  and	heure_lancement < @va_Horodate_in
	  and	heure_lancement = max (heure_lancement)


	/* Rechercher la plus recente mise au neutre tracee pour cet equipement */

	select	@vl_Neutre_Actuel	= type_neutre,
		@vl_Debut_Neutre	= debut,
		@vl_Fin_Neutre		= fin
	from	EXP..PMV_NTR
	where	type_eqt = @va_TypeEqt_in  and  numero_eqt = @va_NumEqt_in
	having	type_eqt = @va_TypeEqt_in  and  numero_eqt = @va_NumEqt_in
	   and	debut = max (debut)


	/* Si l'extinction ou l'affichage de texte est commande a l'equipement alors */
	/* marquer la fin de tout affichage de neutre en cours (apres la derniere action) */

	if ( (@va_TypeEqt_in = XDC_EQT_PMV)   and  @va_Type_Affichage_in in (XDC_PMV_ETEINDRE, XDC_PMV_TEXTE) )  or
	   ( (@va_TypeEqt_in = XDC_EQT_PMVA)  and  @va_Type_Affichage_in in (XDC_PMVA_ETEINDRE, XDC_PMVA_TEXTE) )
	begin
		if (@vl_Debut_Neutre > @vl_Debut_Der_Action)  and  (@vl_Fin_Neutre is null)
		begin
			update	EXP..PMV_NTR
			set	fin = @va_Horodate_in
			where	type_eqt = @va_TypeEqt_in  and  numero_eqt = @va_NumEqt_in
			  and	debut = @vl_Debut_Neutre
		end
	end
	else
	begin
		/* Sinon si l'equipement affiche un type de neutre different alors marquer la fin */

		if (@vl_Debut_Neutre > @vl_Debut_Der_Action)  and  (@vl_Fin_Neutre is null)  and
		   (@vl_Neutre_Actuel <> @vl_Neutre_Demande)
		begin
			update	EXP..PMV_NTR
			set	fin = @va_Horodate_in
			where	type_eqt = @va_TypeEqt_in  and  numero_eqt = @va_NumEqt_in
			  and	debut = @vl_Debut_Neutre
		end


		/* et s'il n'affiche pas le neutre demande alors tracer un nouvel affichage de neutre */

		if not ( (@vl_Debut_Neutre > @vl_Debut_Der_Action)  and  (@vl_Fin_Neutre is null)  and
		         (@vl_Neutre_Actuel = @vl_Neutre_Demande) )
		begin
			insert EXP..PMV_NTR (type_eqt, numero_eqt, debut, fin, type_neutre)
			values (@va_TypeEqt_in, @va_NumEqt_in, @va_Horodate_in, null, @vl_Neutre_Demande)
		end
	end

	return XDC_OK
go
