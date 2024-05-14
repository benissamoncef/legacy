/*E*/
/*  Fichier : $Id: xzag03sp.prc,v 1.2 2002/03/07 10:03:52 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2002/03/07 10:03:52 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzag03sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Purge des bases
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	12/09/95	: Creation	(V 1.1)
* P.C.	06/03/02	: Correction pour HIS V1.2
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Rechercher les evenements d'un mois dans la base de historique
* 
* Sequence d'appel
* SP	XZAG03_Evts_Mois
* 
* Arguments en entree
* XDY_Mot	va_Mois_in
* XDY_Mot	va_Annee_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK	: probleme a l'insertion dans la base de sauvegarde
*                 ou donnee non trouvee
* XDC_ARG_INV	: parametre d'entree null ou date demandee trop proche
* 
* Conditions d'utilisation
* - Il faut que la date courante soit au moins superieure a
* au mois specifie plus 7 jours sinon erreur de parametre
* - Le mois vaut de 1 a 12
* - L'annee vaut au moins 1900
* - La periode de sauvegarde est du debut de mois - 7 jours a la
* la fin du mois + 7 jours
* 
* Fonction
-------------------------------------------------------*/

use PRC
go

create procedure XZAG;03
	@va_Mois_in	smallint = null,
	@va_Annee_in	smallint = null
as
#ifdef HIST
	declare @vl_Horodate1900 datetime, @vl_DateCourante datetime,
		@vl_HorodateDebut datetime, @vl_HorodateFin datetime, 
		@vl_NumEvt int, @vl_CleEvt tinyint, @vl_DeltaJours int

	/*A controle du parametre en entree */
	if @va_Mois_in = null or @va_Annee_in = null or 
	   @va_Mois_in > 12 or @va_Mois_in < 1 or @va_Annee_in < 1900
		return XDC_ARG_INV

	/* date de debut et de fin de sauvegarde */
	select	@vl_DateCourante = getdate(),
		@vl_Horodate1900 = dateadd(year, @va_Annee_in - 1900, "01/01/1900 00:00:00")
	select	@vl_HorodateDebut = dateadd(month, @va_Mois_in - 1, @vl_Horodate1900),
		@vl_HorodateFin = dateadd(month, @va_Mois_in, @vl_Horodate1900)
	select	@vl_DeltaJours = datediff(day, @vl_HorodateFin, @vl_DateCourante)
	select	@vl_HorodateDebut = dateadd(day, -7, @vl_HorodateDebut),
		@vl_HorodateFin = dateadd(day, 7, @vl_HorodateFin)

	/*A Controle que la date specifiee est anterieure a la date courante */
	/*A moins 7 jours                                                    */
	if @vl_DeltaJours < 7
		return XDC_ARG_INV


	/*A Rechercher la liste des evenements clos dans la base d'histo dont */
	/*A la cloture a ete faite durant le mois specifie                    */
	select 
		HIS..FMC_GEN.numero,
		HIS..FMC_GEN.cle,
		HIS..FMC_GEN.cause,
		HIS..FMC_GEN.cle_cause,
		CFG..TYP_FMC.classe
	into #EVT_DU_MOIS
	from HIS..FMC_GEN, CFG..TYP_FMC
	where	HIS..FMC_GEN.cloture >= @vl_HorodateDebut and
		HIS..FMC_GEN.cloture < @vl_HorodateFin and
		CFG..TYP_FMC.numero = HIS..FMC_GEN.type

	/* Si l'evenement est une consequence et que la cause fait      */
	/* partie de la liste des evenements clos pour le mois specifie */
	/* alors positionner a null la cause pour la procedure de       */
	/* sauvegarde de l'evenement dans la base historique            */
	/* (la cause est automatiquement sauvegardee avant ou apres cet */
	/*  appel)                                                      */
	select distinct
		g2.numero,
		g2.cle
	into #CAUSES
	from #EVT_DU_MOIS g1, #EVT_DU_MOIS g2
	where g2.cause = g1.numero and g2.cle_cause = g1.cle

	declare pointeur cursor
	for select numero,cle
	from #CAUSES
	open pointeur

	fetch pointeur into @vl_NumEvt, @vl_CleEvt
	while @@sqlstatus = 0
	begin
		update #EVT_DU_MOIS
		set cause = null, cle_cause = null
		where numero = @vl_NumEvt and cle = @vl_CleEvt
	
		fetch pointeur into @vl_NumEvt, @vl_CleEvt
	end
	close pointeur

	/*A retourner la liste des evenements du mois */
	select 
		numero,
		cle,
		cause,
		cle_cause,
		classe
	from #EVT_DU_MOIS
	order by numero,cle

	return XDC_OK
#else
	return XDC_NOK
#endif
go
