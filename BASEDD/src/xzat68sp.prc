/*E*/
/*  Fichier : $Id: xzat68sp.prc,v 1.4 2019/03/25 14:40:29 devgfi Exp $        $Revision: 1.4 $        $Date: 2019/03/25 14:40:29 $
-------------------------------------------------------------------------------------------
* ESCOTA * PROJET MIGRAZUR
-------------------------------------------------------------------------------------------
* SOUS-SYSTEME : BASEDD
-------------------------------------------------------------------------------------------
* FICHIER xzat68sp.prc
-------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Mise au neutre programmée a la date courante d'un PMVA.
*
-------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	15/05/13 : Creation (DEM 1061)  1.1
* JPL	24/07/17 : Support de l'indicateur de mode clignotant / alternat (DEM 1230)  1.2
* JPL	19/12/18 : Retour en paramètres de l'affichage de neutre prioritaire dans la période (DEM 1312)  1.3
* JPL	25/03/19 : Autoriser les programmations sur FMC non débutées (compatibilité de comportement)  1.4
-----------------------------------------------------------------------------------------*/


/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU
*
*  Retourne les donnees de mise au neutre programmee d'un PMVA donne
*  a l'horodate courante.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL
*
* SP	XZAT68_Neutre_Programme_PMVA
*
* PARAMETRES EN ENTREE :
*
* XDY_Eqt	va_NumEqt_in	: Numero du PMVA
*
*
* PARAMETRES EN SORTIE :
*
* XDY_Entier		va_NumEvt_out		: Cle de la FMC attachée
* XDY_District		va_CleEvt_out		: Cle de la FMC
* XDY_Ligne_PMV		va_Ligne1_out		: Texte première ligne
* XDY_Ligne_PMV		va_Ligne2_out		: Texte deuxième ligne
* XDY_Ligne_PMV		va_Ligne3_out		: Texte troisième ligne
* XDY_Ligne_PMV		va_Ligne4_out		: Texte quatrième ligne
* XDY_Ligne_PMV		va_Ligne5_out		: Texte cinquième ligne
* XDY_Ligne_PMV		va_Alternat1_out	: Texte première ligne alternat
* XDY_Ligne_PMV		va_Alternat2_out	: Texte deuxième ligne alternat
* XDY_Ligne_PMV		va_Alternat3_out	: Texte troisième ligne alternat
* XDY_Ligne_PMV		va_Alternat4_out	: Texte quatrième ligne alternat
* XDY_Ligne_PMV		va_Alternat5_out	: Texte cinquième ligne alternat
* XDY_Octet		va_ModeAffichage_out	: Mode d'affichage / clignotement
*
* CODE RETOUR :
*
* XDC_OK
* XDC_NOK           
* XDC_ARG_INV       : parametre d'entree obligatoire manquant (Null) ou invalide
* <0                : erreur sybase
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
*  Si une mise au neutre programmee existe pour le PMVA a la date courante,
*  retourne les donnees de neutre et la FMC attachee.
*
-----------------------------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAT68' and type = 'P')
	drop procedure XZAT68
go


create procedure XZAT68
	@va_NumEqt_in		T_EQUIPEMENT	= null,
	@va_NumEvt_out		int		= null	output,
	@va_CleEvt_out		T_DISTRICT	= null	output,
	@va_Ligne1_out		T_LIGNE_PMV	= null	output,
	@va_Ligne2_out		T_LIGNE_PMV	= null	output,
	@va_Ligne3_out		T_LIGNE_PMV	= null	output,
	@va_Ligne4_out		T_LIGNE_PMV	= null	output,
	@va_Ligne5_out		T_LIGNE_PMV	= null	output,
	@va_Alternat1_out	T_LIGNE_PMV	= null	output,
	@va_Alternat2_out	T_LIGNE_PMV	= null	output,
	@va_Alternat3_out	T_LIGNE_PMV	= null	output,
	@va_Alternat4_out	T_LIGNE_PMV	= null	output,
	@va_Alternat5_out	T_LIGNE_PMV	= null	output,
	@va_ModeAffichage_out	tinyint		= null	output
as
	declare	@vl_typeJourSemaine	tinyint,
		@vl_typeJourChome	tinyint
	declare	@vl_DebutPeriode	datetime,
		@vl_TypePeriode		tinyint

	declare	@vl_Maintenant		datetime,
		@vl_heure_mn		smallint


	/* Transformation des parametres d'entree nulls */
	if @va_NumEqt_in = 0	select @va_NumEqt_in = null


	/* Déterminer le type du jour courant */
	select @vl_Maintenant	= getdate ()
	select @vl_heure_mn	= (datepart (hour, @vl_Maintenant) * 60) + datepart (minute, @vl_Maintenant)

	if datediff (day, "01/01/2000", @vl_Maintenant) % 7 < 2
		select @vl_typeJourSemaine = XDC_JOUR_WEEKEND
	else
		select @vl_typeJourSemaine = XDC_JOUR_OUVRE

	select @vl_typeJourChome = type from CFG..CAL_VAC
	 where @vl_Maintenant between jour_debut and jour_fin
	having type = max (type)
	   and @vl_Maintenant between jour_debut and jour_fin
	--select @vl_typeJourSemaine, @vl_typeJourChome


	/* Sélectionner les neutres candidats à l'affichage pour le panneau dans la période courante */
	select	PRG.debut, PRG.type_jour
	  into	#PRG_PMA_PERIODES

	  from	EXP..PRG_PMA PRG, EXP..FMC_GEN FMC

	 where	PRG.numero = @va_NumEqt_in
	   and	@vl_Maintenant between PRG.debut and PRG.fin
	   and	( discontinu = XDC_FAUX  or
	      	  ( discontinu = XDC_VRAI  and
	      	    @vl_heure_mn >= (datepart (hour, PRG.debut) * 60) + datepart (minute, PRG.debut)  and
	      	    @vl_heure_mn <  (datepart (hour, PRG.fin) * 60) + datepart (minute, PRG.fin)  and
	      	    ( type_jour = @vl_typeJourChome  or
	      	      type_jour = @vl_typeJourSemaine  or
	      	      (type_jour = XDC_JOUR_OUVRE_TRV  and  @vl_typeJourSemaine = XDC_JOUR_OUVRE  and  @vl_typeJourChome is null)  or
	      	      type_jour = XDC_JOUR_TOUS
	      	    )
	      	  )
	      	)
	   and	FMC.numero = PRG.evenement  and  FMC.cle = PRG.cle
	   and	/*FMC.debut is not null  and*/  FMC.fin is null
	--select debut, type_jour from #PRG_PMA_PERIODES


	/* Sélectionner le neutre prioritaire dans la période */
	select	@vl_DebutPeriode = debut, @vl_TypePeriode = type_jour
	  from	#PRG_PMA_PERIODES
	having	type_jour = max (type_jour)

	if @@rowcount = 0
		return XDC_OK


	/* Retourner les données du neutre prioritaire */
	select	@va_NumEvt_out		= PRG.evenement,
		@va_CleEvt_out		= PRG.cle,
		@va_Ligne1_out		= PRG.ligne_1,
		@va_Ligne2_out		= PRG.ligne_2,
		@va_Ligne3_out		= PRG.ligne_3,
		@va_Ligne4_out		= PRG.ligne_4,
		@va_Ligne5_out		= PRG.ligne_5,
		@va_Alternat1_out	= PRG.ligne_1_alternat,
		@va_Alternat2_out	= PRG.ligne_2_alternat,
		@va_Alternat3_out	= PRG.ligne_3_alternat,
		@va_Alternat4_out	= PRG.ligne_4_alternat,
		@va_Alternat5_out	= PRG.ligne_5_alternat,
		@va_ModeAffichage_out	= PRG.clignotement

	from	EXP..PRG_PMA PRG

	where	PRG.numero	= @va_NumEqt_in
	  and	PRG.debut	= @vl_DebutPeriode
	  and	PRG.type_jour	= @vl_TypePeriode

	return XDC_OK
go
