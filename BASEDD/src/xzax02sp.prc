/*E*/
/*  Fichier : $Id: xzax02sp.prc,v 1.2 2018/12/11 17:20:44 devgfi Exp $        $Revision: 1.2 $        $Date: 2018/12/11 17:20:44 $
-------------------------------------------------------------------------------------------
* ESCOTA  *  PROJET MIGRAZUR
-------------------------------------------------------------------------------------------
* SOUS-SYSTEME : BASEDD
-------------------------------------------------------------------------------------------
* FICHIER xzax02sp.prc
-------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Ajout d'une période de vacances au calendrier dédié.
*   Le type de jour chômé est fourni (vacances ou jours fériés).
*   La période est définie par ses dates de début et fin.
*
-------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	22/11/18 : Creation (DEM 1312)  1.1
* JPL	11/12/18 : Cadrage des horodates de début et fin en début / fin de journée  1.2
-----------------------------------------------------------------------------------------*/


/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU
*
*  Ajoute une période de vacances à la table du calendrier de vacances,
*  si elle ne chevauche aucune période de même type.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL
*
* SP	XZAX02_Ajouter_Periode_Vacances
*
* PARAMETRES EN ENTREE :
*
* XDY_Octet	va_Type_in		: type de période (vacances ou férié)
* XDY_Horodate	va_Jour_Debut_in	: jour de début de la période
* XDY_Horodate	va_Jour_Fin_in		: jour de fin de la période
*
*
* PARAMETRES EN SORTIE :
*
* Aucun
*
* CODE RETOUR :
*
* XDC_OK
* XDC_OBJ_EXISTE    : une période de même type et ayant des jours communs existe
* XDC_NOK
* XDC_ARG_INV       : parametre d'entree obligatoire manquant (Null) ou invalide
* < 0               : erreur sybase
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
*  Recherche une période de même type déjà définie couvrant une partie de cette période.
*  S'il n'en existe aucune, ajoute la période indiquée.
*  La date de début est cadrée à l'horodate de début de journée et
*  la date de fin est cadrée à l'horodate de fin de journée.
*
-----------------------------------------------------------------------------------------*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAX02' and type = 'P')
	drop procedure XZAX02
go


create procedure XZAX02
	@va_Type_in		tinyint		= null,
	@va_JourDebut_in	datetime	= null,
	@va_JourFin_in		datetime	= null
as

	/* Transformation des parametres d'entree nulls */
	if @va_JourDebut_in = XDC_DATE_NULLE	select @va_JourDebut_in = null
	if @va_JourFin_in = XDC_DATE_NULLE	select @va_JourFin_in = null


	/*A Verifier la validite des parametres d'entree */
	if @va_Type_in not in (XDC_JOUR_FERIE, XDC_JOUR_VACANCES)
		return XDC_ARG_INV

	if @va_JourDebut_in is null  or  @va_JourFin_in is null
		return XDC_ARG_INV

	if ( (@va_JourDebut_in > @va_JourFin_in)  or
	     (getdate() > @va_JourFin_in)
	   )
		return XDC_ARG_INV


	select @va_JourDebut_in = convert (datetime, convert (date, @va_JourDebut_in))
	select @va_JourFin_in = dateadd (second, 86399, convert (datetime, convert (date, @va_JourFin_in)))


	/*A Verifier qu'aucune période de même type et ayant des jours communs n'existe */
	if exists (select 1 from CFG..CAL_VAC
	            where type = @va_Type_in
	              and ( jour_debut <= @va_JourFin_in  and  jour_fin >= @va_JourDebut_in )
		)
		return XDC_OBJ_EXISTE


	/*A Ajouter la période de vacances au calendrier */
	insert CFG..CAL_VAC ( type, jour_debut, jour_fin )
	values ( @va_Type_in, @va_JourDebut_in, @va_JourFin_in )

	if @@rowcount <> 1
		return XDC_NOK

	return XDC_OK
go
