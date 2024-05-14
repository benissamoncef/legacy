/*E*/
/*  Fichier : $Id: xzao150sp.prc,v 1.1 2010/10/01 16:25:03 gesconf Exp $        Release : $Revision: 1.1 $        Date : $Date: 2010/10/01 16:25:03 $
-------------------------------------------------------------------------------
* ESCOTA *  PROJET PASTRE
-------------------------------------------------------------------------------
* SOUS-SYSTEME  BASEDD
-------------------------------------------------------------------------------
* FICHIER xzao150sp.prc
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Lecture / ecriture de la configuration du reseau d'autoroutes
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	01/10/10 : Creation 1.1
-----------------------------------------------------------------------------*/

/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU
*
*  Liste des bifurcations d'autoroutes.
*
* -----------------------------------------------------------------------------
* SEQUENCE D'APPEL
*
* SP	XZAO150_Liste_Bifurcations
*
* PARAMETRES EN ENTREE :
*
*  Aucun
*
* PARAMETRES EN SORTIE :
*
*  Aucun
*
* VALEUR RENDUE :
*
* Aucune
*
* CODE RETOUR :
*
* XDC_OK
* XDC_NOK           : evenement inconnu ou de type incorrect pour l'operation
* <0                : erreur sybase
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
*  Retourne la liste des couples de points (autoroute1, PR1, autoroute2, PR2)
*   intersections d'autoroutes extraites de la table des portions.
*
-----------------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO150' and type = 'P')
	drop procedure XZAO150
go


create procedure XZAO150
as

	/*A
	** Rechercher les bifurcations d'autoroutes
	*/

	select autoroute1, pr1, autoroute2, pr2

	from CFG..RES_JON

go
