/*X  Fichier : $Id: RS_route_DP.bas,v 1.2 1998/08/19 10:49:00 gesconf Exp $        Release : $Revision: 1.2 $        Date : $Date: 1998/08/19 10:49:00 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER RS_route_DP.bas
------------------------------------------------------
* DESCRIPTION :
* cree les routes entre les replication serveurs
* (a executer sous sa de REP_DP quand les autres REP
* tournent)
------------------------------------------------------
* HISTORIQUE :
*
* JMG	08/01/98 : creation (dem/1536)
* P.NIEPCERON   19/08/98        : Ajout route vers REP_HIS (1.2)
----------------------------------------------------
*/

/* route DP->CI */
create route to REP_CI
set username MIGRAZUR
set password MIGRAZUR
go

/* route DP->HIS */
create route to REP_HIS
set username MIGRAZUR
set password MIGRAZUR
go

