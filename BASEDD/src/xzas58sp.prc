/*E*/
/*  Fichier : $Id: xzas58sp.prc,v 1.1 1999/02/26 11:21:42 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1999/02/26 11:21:42 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas55sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* lecture de la config des PMV a exporter vers Strada
------------------------------------------------------
* HISTORIQUE :
*
* Guilhou  23/11/98        : Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAS58_Lect_PMV_A_Exporter
*
* Arguments en entree
*
* Arguments en sortie
*
* Code retour
* XDC_OK
* <0 erreur sybase (voir Transact SQL p 13-15)
*
* Conditions d'utilisation
*
* Fonction
* Lecture des des infos associees a un evt
-------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAS58' and type = 'P')

    drop procedure XZAS58

go

create procedure XZAS58
as
	select numero
	from CFG..ADA_PMV

	return XDC_OK

go

