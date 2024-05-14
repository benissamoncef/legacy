/*X  Fichier : @(#)numfichappel.bas	1.1      Release : 1.1        Date : 03/15/95
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER numfichappel.bas
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* Initialisation des compteurs
*
------------------------------------------------------
* HISTORIQUE :
*
* P.V.  17/07/96: Creation
-----------------------------------------------------*/


use EXP
go

insert LST_CPT (code,libelle,valeur,modifiable,date_raz)
values( 1, "Numero appel CI", 0, 1,"01/01/1997 0:00")
insert LST_CPT (code,libelle,valeur,modifiable,date_raz)
values( 2, "Numero appel DA", 0, 1,"01/01/1997 0:00")
insert LST_CPT (code,libelle,valeur,modifiable,date_raz)
values( 3, "Numero appel DP", 0, 1,"01/01/1997 0:00")
insert LST_CPT (code,libelle,valeur,modifiable,date_raz)
values( 4, "Numero appel DC", 0, 1,"01/01/1997 0:00")
insert LST_CPT (code,libelle,valeur,modifiable,date_raz)
values( 5, "Numero appel DS", 0, 1,"01/01/1997 0:00")
insert LST_CPT (code,libelle,valeur,modifiable,date_raz)
values( 6, "Numero appel DY", 0, 1,"01/01/1997 0:00")
go
