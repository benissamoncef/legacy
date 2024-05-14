/*E*/
/*  Fichier : $Id: xzae166sp.prc,v 1.3 2011/09/20 16:12:58 gesconf Exp $      Release : $Revision: 1.3 $        Date :  $Date: 2011/09/20 16:12:58 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae166sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module WEB : 
* rechercher dans WEB_CRI le dernier message de crise
*
------------------------------------------------------
* HISTORIQUE :
*
* Guilhou     16/01/01  : Creation v1.1
* Hilmarcher  01/07/01  : modif format horodate v1.2
* JPL		12/09/11 : Support 2e portion de message pour longueur de 500 car. (DEM 993)  1.3
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAE166
* 
* Arguments en entree
* 
*  aucun
* 
* Arguments en sortie
*
* aucun
* 
* Service rendu
* 
* Code retour
* XDC_OK
* XDC_NOK	:
* XDC_ARG_INV	: parametres d'entree null ou site incorrect
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
*  Extrait et retourne les donnees du message d'identifiant maximal
-----------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE166' and type = 'P')
	drop procedure XZAE166
go


create procedure XZAE166

as

select local,
       delai,
       titre,
       message,
       suppression,
       identifiant,
       datepart(day,horodate),
       datepart(month,horodate),
       datepart(year,horodate),
       datepart(hour,horodate),
       datepart(minute,horodate),
       datepart(second,horodate),
       horodate,
       message2
from EXP..WEB_CRI
having identifiant = max(identifiant)

return (XDC_OK)

go
