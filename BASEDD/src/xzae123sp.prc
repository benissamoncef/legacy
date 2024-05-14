/*E*/
/*  Fichier : $Id: xzae123sp.prc,v 1.2 1997/12/01 15:36:16 verdier Exp $      Release : $Revision: 1.2 $        Date : $Date: 1997/12/01 15:36:16 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae123sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* lecture de la config des fmc a exporter vers Strada
------------------------------------------------------
* HISTORIQUE :
*
* Guilhou  08/05/97        : Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAE123_Lect_FMC_A_Exporter
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

if exists (select * from sysobjects where name = 'XZAE123' and type = 'P')
	drop procedure XZAE123
go

create procedure XZAE123
as
	/*A Recherche les infos associees a un evt */
	select type, autoroute, debut, fin
	from CFG..ADA_EVT

	return XDC_OK

go

