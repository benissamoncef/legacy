/*X  Fichier : $Id: RS_route_DY.bas,v 1.3 2007/07/11 13:43:11 pc2dpdy Exp $        Release : $Revision: 1.3 $        Date : $Date: 2007/07/11 13:43:11 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER RS_route_DY.bas
------------------------------------------------------
* DESCRIPTION :
* cree les routes entre les replication serveurs
* (a executer sous sa de REP_DY quand les autres REP
* tournent)
------------------------------------------------------
* HISTORIQUE :
*
* JMG	08/01/98 : creation (dem/1536)
* P.NIEPCERON   19/08/98        : Ajout route vers REP_HIS (1.2)
* L.CLAUDEL     09/07/07        : Suppression DY (1.3)
----------------------------------------------------
*/

/* route DY->CI */
/*
create route to REP_CI
set username MIGRAZUR
set password MIGRAZUR
go
*/
/* route DY->HIS */
/*
create route to REP_HIS
set username MIGRAZUR
set password MIGRAZUR
go
*/

