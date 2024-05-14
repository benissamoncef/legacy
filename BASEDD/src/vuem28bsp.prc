/*E*/
/*  Fichier : @(#)vuem28bsp.prc	1.1    Release : 1.1        Date : 05/27/97
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER vuem28bsp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* vues metiers
*
------------------------------------------------------
* HISTORIQUE :
*
* L.V.	16/05/97	: Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	VUEM28b_TDP
* 
* Arguments en entree
* 
* Arguments en sortie
*
* liste retournee
* smallint	numero de zdp
* char		libelle zone TDP
* 
* Service rendu
* retourne la liste des zdp
* 
* Code retour
* XDC_OK
* XDC_NOK	: actio non trouvee
* XDC_ARG_INV	: parametres d'entree null ou site incorrect
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
-----------------------------------------------------------------------*/

use PRC
go

create procedure VUEM28b
	
as

select distinct
	numero,
	libelle
from CFG..EQT_ZDP



go

