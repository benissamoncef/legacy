/*E*/
/*  Fichier : @(#)xzas80sp.prc	1.0      Release : 1.0        Date : 10/01/01
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas81sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module WEB : Gestion de la couleur des troncons web      
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG   11/10/08        creation DEM/836
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAS81
* 
* Arguments en entree
* smallint	niveau
* 
* Arguments en sortie
*
* aucun : 
* 
* Service rendu
*         retourne les troncons correspondant au niveau passe en entree
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

if exists (select * from sysobjects where name = 'XZAS81' and type = 'P')

    drop procedure XZAS81

go

create procedure XZAS81
        @va_niveau_in   tinyint  

as

select identifiant,
	niveau,
	nom_vue,
	etat,
	station1,
	station2,
	station3,
	station4,
	station5,
	autoroute,
	sens,
	PR_debut,
	PR_fin
	from CFG..WEB_TRO where niveau=@va_niveau_in

return (XDC_OK)

go
