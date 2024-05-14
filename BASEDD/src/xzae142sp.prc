/*E*/
/*  Fichier : $Id: xzae142sp.prc,v 1.4 1998/01/05 16:37:04 penot Exp $      Release : $Revision: 1.4 $        Date : $Date: 1998/01/05 16:37:04 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae142sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* lecture des evenemets strada en base
------------------------------------------------------
* HISTORIQUE :
*
*  Penot	05 Nov 1997       : Creation
* Guilhou	0( jan 1998	: tri par ordre alphabetique (dem/1521) 1.4
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAE142_Liste_Evenements
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

if exists (select * from sysobjects where name = 'XZAE142' and type = 'P')
	drop procedure XZAE142
go

create procedure XZAE142
as
	/*A Recherche les infos associees a un evt */
	select distinct reference, type, horodate,version,transmis
	from EXP..ADA_SYN
	group by reference
	having version = min (version)
	order by reference
	return XDC_OK

go

