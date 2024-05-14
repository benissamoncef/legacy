/*E*/
/*  Fichier : $Id: xzao590sp.prc,v 1.1 2013/01/29 10:05:24 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2013/01/29 10:05:24 $
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
* SP   XZAO590_Lect_FMC_A_Exporter
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

if exists (select * from sysobjects where name = 'XZAO590' and type = 'P')
	drop procedure XZAO590
go

create procedure XZAO590
as
	/*A Recherche les infos associees a un evt */
	select *
	from CFG..REG_TYP

	return XDC_OK

go

