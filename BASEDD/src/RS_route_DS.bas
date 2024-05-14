/*X  Fichier : $Id: RS_route_DS.bas,v 1.3 1998/08/19 10:49:02 gesconf Exp $        Release : $Revision: 1.3 $        Date : $Date: 1998/08/19 10:49:02 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER RS_route_DS.bas
------------------------------------------------------
* DESCRIPTION :
* cree les routes entre les replication serveurs
* (a executer sous sa de REP_DS quand les autres REP
* tournent)
------------------------------------------------------
* HISTORIQUE :
*
* C.TORREGROSSA	11/09/96	: Creation
* JMG	08/01/98 : DS devient un PC niveau 2 (dem/1536) 1.2
* P.NIEPCERON   19/08/98        : Ajout route vers REP_HIS (1.3)
----------------------------------------------------
*/

/* route DS->CI */
create route to REP_CI
set username MIGRAZUR
set password MIGRAZUR
go

/* route DS->DA */
create route to REP_DA
set username MIGRAZUR
set password MIGRAZUR
go


/* route DS->DC */
create route to REP_DC
set username MIGRAZUR
set password MIGRAZUR
go

/* route DS->HIS */
create route to REP_HIS
set username MIGRAZUR
set password MIGRAZUR
go

