/*E*/
/*  Fichier : $Id: xzae178sp.prc,v 1.1 2008/11/17 11:12:13 pc2dpdy Exp $      Release : $Revision: 1.1 $         Date : $Date: 2008/11/17 11:12:13 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae178sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
* config de WEB_FMC
------------------------------------------------------
* HISTORIQUE :
*
* JMG	11/10/08	creation DEM 836
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAE178
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

if exists (select * from sysobjects where name = 'XZAE178' and type = 'P')
	drop procedure XZAE178
go

create procedure XZAE178
as

	select type,
		nature,
		icone,
		priorite,
		type_web
	from CFG..WEB_FMC
	return XDC_OK
go

