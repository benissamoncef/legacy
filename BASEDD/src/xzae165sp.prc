/*E*/
/*  Fichier : $Id: xzae165sp.prc,v 1.3 2009/01/16 14:59:55 gesconf Exp $      Release : $Revision: 1.3 $      Date : $Date: 2009/01/16 14:59:55 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae165sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module WEB : 
* rechercher dans WEB_EVT les enregistrements 
* pour le bon niveau
*
------------------------------------------------------
* HISTORIQUE :
*
* Guilhou     16/01/01  : Creation
* JMG	16/01/09 : on recupere le type_web dans WEB_FMC en utilisant l icone (putain, 8 ans....)�
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAE165
* 
* Arguments en entree
* tinyint niveau 
* 
* Arguments en sortie
*
* aucun : 
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
-----------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE165' and type = 'P')
	drop procedure XZAE165
go

create procedure XZAE165
        @va_niveau_in tinyint

as

select distinct icone, type_web
into #TEMPO
from CFG..WEB_FMC

select identifiant,
       niveau,
       nom_vue,
       X,
       Y,
       facteurX,
       facteurY,
       rotation,
       w.icone,
       texte,
       datepart(day,horodate),
       datepart(month,horodate),
       datepart(year,horodate),
       datepart(hour,horodate),
       datepart(minute,horodate),
       t.type_web
from EXP..WEB_EVT w , #TEMPO t
where niveau = @va_niveau_in
and w.icone = t.icone

return (XDC_OK)

go
