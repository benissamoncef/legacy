/*E*/
/*  Fichier : $Id: xzax01sp.prc,v 1.2 2018/12/11 17:20:38 devgfi Exp $        $Revision: 1.2 $        $Date: 2018/12/11 17:20:38 $
-------------------------------------------------------------------------------------------
* ESCOTA  *  PROJET MIGRAZUR
-------------------------------------------------------------------------------------------
* SOUS-SYSTEME : BASEDD
-------------------------------------------------------------------------------------------
* FICHIER xzax01sp.prc
-------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Liste des périodes de vacances ou fériées.
*
-------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	22/11/18 : Creation (DEM 1312)  1.1
* JPL	11/12/18 : Retour de l'horodate de fin en début de journée  1.2
-----------------------------------------------------------------------------------------*/


/*X*/
/*-----------------------------------------------------------------------------------------
* SERVICE RENDU
*
*  Retourne le contenu du calendrier de vacances défini,
*  c'est à dire la liste des périodes chômées avec leur type (vacances ou fériées).
*
-------------------------------------------------------------------------------------------
* SEQUENCE D'APPEL
*
* SP	XZAX01_Liste_Periodes_Vacances
*
* PARAMETRES EN ENTREE :
*
* Aucun
*
*
* PARAMETRES EN SORTIE :
*
* Aucun
*
* CODE RETOUR :
*
* XDC_OK
* < 0               : erreur sybase
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
*  Retourne le contenu de la table CAL_VAC ordonné chronologiquement.
*
-----------------------------------------------------------------------------------------*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAX01' and type = 'P')
	drop procedure XZAX01
go


create procedure XZAX01

as

	select	type,
		debut = jour_debut,
		fin   = convert (datetime, convert (date, jour_fin))

	from	CFG..CAL_VAC

	order by debut

	return XDC_OK
go
