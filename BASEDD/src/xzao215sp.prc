/*E*/
/*  Fichier : $Id: xzao215sp.prc,v 1.2 2015/02/17 22:07:50 devgfi Exp $        Release : $Revision: 1.2 $        Date : $Date: 2015/02/17 22:07:50 $
-------------------------------------------------------------------------------
* ESCOTA *  PROJET PASTRE
-------------------------------------------------------------------------------
* SOUS-SYSTEME  BASEDD
-------------------------------------------------------------------------------
* FICHIER xzao215sp.prc
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Lecture / ecriture de la configuration du reseau
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	25/09/12 : Creation (DEM 1032)  1.1
* JPL	11/02/15 : Retour de l'indicateur d'autoroute inversee (DEM 1118)  1.2
* PNI   18/08/23 : Filtre sur la colonne valide=XDC_VRAI SAE 510
-----------------------------------------------------------------------------*/

/*X*/
/*-----------------------------------------------------------------------------
* SERVICE RENDU
*
*  Retourne la liste des troncons pour la Viabilite Hivernale.
*
* SEQUENCE D'APPEL
*
* SP	XZAO215_Liste_Troncons_VH
*
* Arguments en entree : Aucun
*
*
* Arguments en sortie : Aucun
*
*
* Code retour :
*
* XDC_OK
* <0                : erreur sybase
*
* Conditions d'utilisation :
*
* La procedure stockee existe au CI et dans la base Historique.
*
* Fonction :
*
*  Select dans la table CCH_TRO
*
-----------------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO215' and type = 'P')
	drop procedure XZAO215
go


create procedure XZAO215
as

	/*A lecture des troncons dans l'ordre */
	select	T.numero,
		T.nom,
		T.district,
		T.autoroute,
		T.PR_debut,
		T.PR_fin,
		T.valide,
		I.inversee

	from	CFG..CCH_TRO T, CFG..RES_AUT_INV I

	where	I.numero = T.autoroute and T.valide=XDC_VRAI

	--order by T.autoroute, I.inversee * T.PR_debut
	order by T.numero

	return XDC_OK
go
