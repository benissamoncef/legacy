/*X  Fichier : $Id: RS_route_DA.bas,v 1.2 1998/08/19 10:48:55 gesconf Exp $        Release : $Revision: 1.2 $        Date : $Date: 1998/08/19 10:48:55 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER RS_route_DA.bas
------------------------------------------------------
* DESCRIPTION :
* cree les routes entre les replication serveurs
* (a executer sous sa de REP_DA quand les autres REP
* tournent)
------------------------------------------------------
* HISTORIQUE :
*
* JMG	08/01/98 : creation (dem/1536) 1.1
* P.NIEPCERON   19/08/98        : Ajout route vers REP_HIS (1.2)
----------------------------------------------------
*/

/* route DA->CI */
create route to REP_CI
set username MIGRAZUR
set password MIGRAZUR
go

/* route DA->DS */
create route to REP_DS
set username MIGRAZUR
set password MIGRAZUR
go

/* route DA->HIS */
create route to REP_HIS
set username MIGRAZUR
set password MIGRAZUR
go

