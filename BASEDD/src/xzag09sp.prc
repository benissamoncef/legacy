/*E*/
/*  Fichier : $Id: xzag09sp.prc,v 1.2 2002/03/07 10:04:41 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2002/03/07 10:04:41 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzag09sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Purge des bases
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	12/09/95	: Creation	(V 1.1)
* P.C.	06/03/02	: correction pour HIS V1.2 
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Transfere les donnees de disponibilite des equipements du mois dans la base de sauvegarde
* /restauration
* 
* Sequence d'appel
* SP	XZAG09_Transfert_Dispo_Mois
* 
* Arguments en entree
* XDY_Mot	va_Mois_in
* XDY_Mot	va_Annee_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK	: 
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
* Copier de la base historique dans la base sauvegarde/restauration
-------------------------------------------------------*/

use PRC
go

create procedure XZAG;09
	@va_Mois_in	smallint = null,
	@va_Annee_in	smallint = null
as
#ifdef HIST
	declare @vl_Status int, 
		@vl_NumEqt smallint, @vl_TypeEqt tinyint,
		@vl_Horodate1900 datetime, @vl_DateCourante datetime,
		@vl_HorodateDebut datetime, @vl_HorodateFin datetime, 
		@vl_DeltaJours int

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

	/*A Sauvegarder tous les etats de disponibilite des equipements du */
	/*A mois specifie                                                  */
	insert SVG..EQT_DSP
	select * from HIS..EQT_DSP 
	where (horodate < @vl_HorodateFin and horodate > @vl_HorodateDebut)

	/* ajouter la derniere disponibilite des equipements avant */
	/* la date de debut */
	declare Pointeur_evt cursor
	for select 
		numero,
		type
	from CFG..EQT_GEN

	open Pointeur_evt
	fetch Pointeur_evt into @vl_NumEqt, @vl_TypeEqt
	while @@sqlstatus = 0
	begin
		insert SVG..EQT_DSP
		select * from HIS..EQT_DSP
		where equipement = @vl_NumEqt and
			type = @vl_TypeEqt and horodate in
			(select max(horodate) from HIS..EQT_DSP
			 where horodate <= @vl_HorodateDebut and
				equipement = @vl_NumEqt and
				type = @vl_TypeEqt)
		
		fetch Pointeur_evt into @vl_NumEqt, @vl_TypeEqt
	end
	close Pointeur_evt

	return XDC_OK
#else
	return XDC_NOK
#endif
go
