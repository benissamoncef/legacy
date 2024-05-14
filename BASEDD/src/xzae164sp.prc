/*E*/
/*  Fichier : $Id: xzae164sp.prc,v 1.1 2001/04/19 16:24:23 gesconf Exp $      Release : $Revision: 1.1 $        Date : $Date: 2001/04/19 16:24:23 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae164sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
* purge les icones WEB
------------------------------------------------------
* HISTORIQUE :
*
* Guilhou	05 Mar 1999 : creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAE164_Init_Icones
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
-------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE164' and type = 'P')
	drop procedure XZAE164
go

create procedure XZAE164
as

	delete EXP..WEB_EVT
	delete EXP..WEB_TXT
	return XDC_OK
go

