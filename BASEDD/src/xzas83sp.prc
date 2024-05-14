/*E*/
/*  Fichier : @(#)xzas80sp.prc	1.0      Release : 1.0        Date : 10/01/01
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzas83sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module WEB : Gestion de la table d'autorisation des zdp 
*
------------------------------------------------------
* HISTORIQUE :
*
* Cluchague     22/12/04  : on renvoie aussi la table des tdp autorises ou non V1.2
* Cluchague     07/02/05  : on complete la table V1.3
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAS83 
* 
* Arguments en entree
* smallint	station
* tinyint	sens
* 
* Arguments en sortie
*
* aucun : 
* 
* Service rendu
*         Remplit simplement le champ etat de la table WEB_TRO en
*         pour tous les troncons satisfaisant au parametres d'entree
*         avec la couleur passee en parametre d'entree
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

drop proc XZAS83
go

create procedure XZAS83

as

create table #TMP_AUTORISATION_TDP
 (
	 identifiant char(100) null,
	 diffusion   char     null,
	 temps       int
 )

insert #TMP_AUTORISATION_TDP values ("Aix -> Italie","N",0)
insert #TMP_AUTORISATION_TDP values ("Italie -> Aix","N",0)
insert #TMP_AUTORISATION_TDP values ("Aix -> Capitou","N",0)
insert #TMP_AUTORISATION_TDP values ("Capitou -> Aix","N",0)
insert #TMP_AUTORISATION_TDP values ("Italie -> Capitou","N",0)
insert #TMP_AUTORISATION_TDP values ("Capitou -> Italie","N",0)
insert #TMP_AUTORISATION_TDP values ("Aix -> St Maximin","N",0)
insert #TMP_AUTORISATION_TDP values ("St Maximin -> Aix","N",0)
insert #TMP_AUTORISATION_TDP values ("St Maximin -> Cannet des Maures","N",0)
insert #TMP_AUTORISATION_TDP values ("Cannet des Maures -> St Maximin","N",0)
insert #TMP_AUTORISATION_TDP values ("Cannet des Maures -> Capitou","N",0)
insert #TMP_AUTORISATION_TDP values ("Capitou -> Cannet des Maures","N",0)
insert #TMP_AUTORISATION_TDP values ("Capitou -> Mougins","N",0)
insert #TMP_AUTORISATION_TDP values ("Mougins -> Capitou","N",0)
insert #TMP_AUTORISATION_TDP values ("Mougins -> Nice Promenade","N",0)
insert #TMP_AUTORISATION_TDP values ("Nice Promenade -> Mougins","N",0)
insert #TMP_AUTORISATION_TDP values ("Nice Promenade -> Monaco","N",0)
insert #TMP_AUTORISATION_TDP values ("Monaco -> Nice Promenade","N",0)
insert #TMP_AUTORISATION_TDP values ("Nice Promenade -> Menton","N",0)
insert #TMP_AUTORISATION_TDP values ("Menton -> Nice Promenade","N",0)
insert #TMP_AUTORISATION_TDP values ("Nice Promenade -> Italie","N",0)
insert #TMP_AUTORISATION_TDP values ("Italie -> Nice Promenade","N",0)
insert #TMP_AUTORISATION_TDP values ("Aix -> Nice Promenade","N",0)
insert #TMP_AUTORISATION_TDP values ("Nice Promenade -> Aix","N",0)

insert #TMP_AUTORISATION_TDP values ("PMV PR 18 -> Italie","N",0)
insert #TMP_AUTORISATION_TDP values ("PMV PR 217 -> Aix","N",0)
insert #TMP_AUTORISATION_TDP values ("PMV PR 18 -> Capitou","N",0)
insert #TMP_AUTORISATION_TDP values ("PMV PR 120 -> Aix","N",0)
insert #TMP_AUTORISATION_TDP values ("PMV PR 144 -> Italie","N",0)
insert #TMP_AUTORISATION_TDP values ("PMV PR 217 -> Capitou","N",0)
insert #TMP_AUTORISATION_TDP values ("PMV PR 18 -> St Maximin","N",0)
insert #TMP_AUTORISATION_TDP values ("PMV PR 60 -> Aix","N",0)
insert #TMP_AUTORISATION_TDP values ("PMV PR 55 -> Cannet des Maures","N",0)
insert #TMP_AUTORISATION_TDP values ("PMV PR 103 -> St Maximin","N",0)
insert #TMP_AUTORISATION_TDP values ("PMV PR 92 -> Capitou","N",0)
insert #TMP_AUTORISATION_TDP values ("PMV PR 120 -> Cannet des Maures","N",0)
insert #TMP_AUTORISATION_TDP values ("PMV PR 144 -> Mougins","N",0)
insert #TMP_AUTORISATION_TDP values ("PMV PR 161 -> Capitou","N",0)
insert #TMP_AUTORISATION_TDP values ("PMV PR 162 -> Nice Promenade","N",0)
insert #TMP_AUTORISATION_TDP values ("PMV PR 183 -> Mougins","N",0)
insert #TMP_AUTORISATION_TDP values ("PMV PR 185 -> Monaco","N",0)
insert #TMP_AUTORISATION_TDP values ("PMV PR 207 -> Nice Promenade","N",0)
insert #TMP_AUTORISATION_TDP values ("PMV PR 185 -> Menton","N",0)
insert #TMP_AUTORISATION_TDP values ("PMV PR 217 -> Nice Promenade","N",0)
insert #TMP_AUTORISATION_TDP values ("PMV PR 185 -> Italie","N",0)
insert #TMP_AUTORISATION_TDP values ("PMV PR 18 -> Nice","N",0)

insert #TMP_AUTORISATION_TDP values ("Cannet des Maures -> Avignon Nord","N",0)
insert #TMP_AUTORISATION_TDP values ("Cannet des Maures -> Valence Sud","N",0)
insert #TMP_AUTORISATION_TDP values ("Cannet des Maures -> Vienne","N",0)
insert #TMP_AUTORISATION_TDP values ("Cannet des Maures -> Salon-de-Provence A7 / A54 ","N",0)
insert #TMP_AUTORISATION_TDP values ("Aix -> Avignon Nord","N",0)
insert #TMP_AUTORISATION_TDP values ("Aix -> Valence Sud","N",0)
insert #TMP_AUTORISATION_TDP values ("Aix -> Vienne","N",0)
insert #TMP_AUTORISATION_TDP values ("Aix -> Salon-de-Provence A7 / A54 ","N",0)
insert #TMP_AUTORISATION_TDP values ("PMV PR 103 -> Avignon Nord","N",0)
insert #TMP_AUTORISATION_TDP values ("PMV PR 103 -> Valence Sud","N",0)
insert #TMP_AUTORISATION_TDP values ("PMV PR 103 -> Vienne","N",0)
insert #TMP_AUTORISATION_TDP values ("PMV PR 22 -> Avignon Nord","N",0)
insert #TMP_AUTORISATION_TDP values ("PMV PR 22 -> Valence Sud","N",0)
insert #TMP_AUTORISATION_TDP values ("PMV PR 22 -> Vienne","N",0)

/* on renvoie la table d'autorisation des tdp */
select * from #TMP_AUTORISATION_TDP
return (XDC_OK)

go

