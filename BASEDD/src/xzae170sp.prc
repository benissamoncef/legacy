/*E*/
/*  Fichier : $Id: xzae170sp.prc,v 1.2 2005/04/28 18:56:37 gesconf Exp $      Release : $Revision: 1.2 $      Date : $Date: 2005/04/28 18:56:37 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae170sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module NOMADE : Retourne la liste des FMC pour nomade
* Remarque :                                                                   
*
------------------------------------------------------
* HISTORIQUE :
*
* Cluchague     07/12/01  : Creation
* Cluchague     28/07/04/05  : on ecarte tous les evenements dont la fin est < H 1 heure V1.2
-----------------------------------------------------*/
/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAE170
* 
* Arguments en entree
* 
* Arguments en sortie
*
* aucun : 
* 
* Service rendu
*         retourne les renseignements necessaires a l'etablissement des messages TFM
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

if exists (select * from sysobjects where name = 'XZAE170' and type = 'P')
	drop procedure XZAE170
go

create procedure XZAE170
as

declare
@vl_numero int

   create table #TMP_LISTE_FMC
      (
	etat         char(15) null,
	numero       int      null,
	cle          tinyint  null, 
	type         char(25) null,
	debut        char(20) null,
	localisation char(12) null
      )

/************************************************************************************************************************/
/************************************************************************************************************************/
/*                                      recuperation des infos en base                                                  */
/************************************************************************************************************************/
/************************************************************************************************************************/
insert #TMP_LISTE_FMC
select  /*etat="evt en cours",*/
	etat="c",
        G.numero,
	G.cle,
	T.abreviation,
        convert(char(10),G.debut,103) + ' ' + convert(char(8),G.debut,8),
	rtrim(R.nom)+'-'+convert(char(1),D.sens)+'-'+convert (char(6),D.PR)
from EXP..FMC_GEN G, EXP..FMC_HIS_DER D, CFG..TYP_FMC T,CFG..RES_AUT R
where G.numero = D.numero and G.cle = D.cle
and G.type = T.numero
and D.autoroute = R.numero
and G.debut is not null and G.fin is null and G.cloture is null 
and T.operation = 0 
and G.type!= 37 
order by G.debut desc

insert #TMP_LISTE_FMC
select  /*etat="evt termines",*/
	etat="t",
        G.numero,
	G.cle,
	T.abreviation,
        convert(char(10),G.debut,103) + ' ' + convert(char(8),G.debut,8),
	rtrim(R.nom)+'-'+convert(char(1),D.sens)+'-'+convert (char(6),D.PR)
from EXP..FMC_GEN G, EXP..FMC_HIS_DER D, CFG..TYP_FMC T,CFG..RES_AUT R
where G.numero = D.numero and G.cle = D.cle
and G.type = T.numero
and D.autoroute = R.numero
and G.debut is not null and G.fin is not null and G.cloture is null 
and datediff(minute,G.fin,getdate()) < 60  
and T.operation = 0 
and G.type!= 37 
order by G.debut desc

insert #TMP_LISTE_FMC
select  /*etat="tvx en cours",*/
	etat="c",
        G.numero,
	G.cle,
	T.abreviation,
        convert(char(10),G.debut,103) + ' ' + convert(char(8),G.debut,8),
	rtrim(R.nom)+'-'+convert(char(1),D.sens)+'-'+convert (char(6),D.PR)
from EXP..FMC_GEN G, EXP..FMC_HIS_DER D, CFG..TYP_FMC T,CFG..RES_AUT R
where G.numero = D.numero and G.cle = D.cle
and G.type = T.numero
and D.autoroute = R.numero
and G.debut is not null and G.fin is null and G.cloture is null 
and T.operation = 1 
order by G.debut desc

insert #TMP_LISTE_FMC
select  /*etat="tvx termines",*/
	etat="t",
        G.numero,
	G.cle,
	T.abreviation,
        convert(char(10),G.debut,103) + ' ' + convert(char(8),G.debut,8),
	rtrim(R.nom)+'-'+convert(char(1),D.sens)+'-'+convert (char(6),D.PR)
from EXP..FMC_GEN G, EXP..FMC_HIS_DER D, CFG..TYP_FMC T,CFG..RES_AUT R
where G.numero = D.numero and G.cle = D.cle
and G.type = T.numero
and D.autoroute = R.numero
and G.debut is not null and G.fin is not null and G.cloture is null 
and datediff(minute,G.fin,getdate()) < 60  
and T.operation = 1 
order by G.debut desc

/*valeurs a retourner*/
select  * from #TMP_LISTE_FMC

drop table #TMP_LISTE_FMC

return (XDC_OK)

go
