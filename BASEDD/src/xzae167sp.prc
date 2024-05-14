/*E*/
/*  Fichier : $Id: xzae167sp.prc,v 1.2 2011/09/13 11:05:43 gesconf Exp $      Release : $Revision: 1.2 $        Date :  $Date: 2011/09/13 11:05:43 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae167sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
* purge les icones WEB
------------------------------------------------------
* HISTORIQUE :
*
* Guilhou	05 Mar 1999 : creation
* JPL		13/09/11 : Directives de compilation  1.2
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAE167_Supprimer_Message_Crise
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

if exists (select * from sysobjects where name = 'XZAE167' and type = 'P')
	drop procedure XZAE167
go


create procedure XZAE167
	@va_identifiant_in int
as

	update EXP..WEB_CRI
	set suppression = 1
	where identifiant = @va_identifiant_in
	return XDC_OK
go

