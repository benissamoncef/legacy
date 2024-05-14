/*E*/
/*  Fichier : $Id: xzax03sp.prc,v 1.1 2018/11/30 19:03:58 devgfi Exp $        $Revision: 1.1 $        $Date: 2018/11/30 19:03:58 $
-------------------------------------------------------------------------------------------
* ESCOTA  *  PROJET MIGRAZUR / PASTRE
-------------------------------------------------------------------------------------------
* SOUS-SYSTEME : BASEDD
-------------------------------------------------------------------------------------------
* FICHIER xzax03sp.prc
-------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Supprime une période chômée du calendrier de vacances.
*
-------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	22/11/18 : Creation (DEM 1312)  1.1
-----------------------------------------------------------------------------------------*/


/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU
*
*  Supprime une période de vacances, définie par son type et sa date de début.
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL
*
* SP	XZAX03_Supprimer_Periode_Vacances
*
* PARAMETRES EN ENTREE :
*
* XDY_Octet	va_Type_in		: Type de période (vacances ou férié)
* XDY_Horodate	va_JourDebut_in		: Jour de début de la période
*
*
* PARAMETRES EN SORTIE :
*
* Aucun
*
* CODE RETOUR :
*
* XDC_OK
* XDC_NOK           : Période inexistante
* XDC_ARG_INV       : parametre d'entree obligatoire manquant (Null) ou invalide
* <0                : erreur sybase
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
-----------------------------------------------------------------------------------------*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAX03' and type = 'P')
	drop procedure XZAX03
go


create procedure XZAX03
	@va_Type_in		tinyint		= null,
	@va_JourDebut_in	datetime	= null
as

	/* Transformation des parametres d'entree nulls */
	if @va_JourDebut_in = XDC_DATE_NULLE
		select @va_JourDebut_in = null


	/*A Verifier la validite des parametres d'entree */
	if @va_Type_in not in (XDC_JOUR_FERIE, XDC_JOUR_VACANCES)
		return XDC_ARG_INV

	if @va_JourDebut_in is null
		return XDC_ARG_INV


	/*A Supprimer la mise période de vacances du calendrier */
	delete	CFG..CAL_VAC
	where	type = @va_Type_in
	  and	jour_debut = @va_JourDebut_in

	if @@rowcount = 0
		return XDC_OBJ_AUCUN

	return XDC_OK
go
