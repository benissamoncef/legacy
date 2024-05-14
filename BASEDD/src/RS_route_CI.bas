/*X  Fichier : $Id: RS_route_CI.bas,v 1.6 2007/07/11 13:43:11 pc2dpdy Exp $      Release : $Revision: 1.6 $        Date : $Date: 2007/07/11 13:43:11 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER RS_route_CI.bas
------------------------------------------------------
* DESCRIPTION :
* cree les routes entre les replication serveurs
* (a executer sous sa de REP_CI quand les autres RS
* tournent)
------------------------------------------------------
* HISTORIQUE :
*
* B.GABORIT	19/09/94	: Creation
* C.TORREGROSSA	21/03/95	: Modif nom de la replication (V 1.2)
*                                 et ne pas mettre une fin de commentaire
*                                 avec des tirets.
* C.TORREGROSSA	11/09/96	: Ajout route vers PC simplifie DC et DS (RADT 1.3)
* C.TORREGROSSA	18/02/98	: Ajout route vers PC niveau 2 DA, niveau 3 DP et DY (1.4)
* P.NIEPCERON	19/08/98	: Ajout route vers REP_HIS (1.5)
* L.CLAUDEL     09/07/07        : Suppression DY (1.6)
----------------------------------------------------
*/

/* route CI->DN */
create route to REP_DN
set username MIGRAZUR
set password MIGRAZUR
go

/* route CI->DM */
create route to REP_DM
set username MIGRAZUR
set password MIGRAZUR
go


/* route CI->DC */
create route to REP_DC
set username MIGRAZUR
set password MIGRAZUR
go


/* route CI->DS */
create route to REP_DS
set username MIGRAZUR
set password MIGRAZUR
go

/* route CI->DA */
create route to REP_DA
set username MIGRAZUR
set password MIGRAZUR
go

/* route CI->DP */
create route to REP_DP
set username MIGRAZUR
set password MIGRAZUR
go

/* route CI->HIS */
create route to REP_HIS
set username MIGRAZUR
set password MIGRAZUR
go

