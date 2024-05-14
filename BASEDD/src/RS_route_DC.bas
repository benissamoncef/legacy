/*X  Fichier : $Id: RS_route_DC.bas,v 1.3 1998/08/19 10:48:57 gesconf Exp $        Release : $Revision: 1.3 $        Date : $Date: 1998/08/19 10:48:57 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER RS_route_DC.bas
------------------------------------------------------
* DESCRIPTION :
* cree les routes entre les replication serveurs
* (a executer sous sa de REP_DC quand les autres REP
* tournent)
------------------------------------------------------
* HISTORIQUE :
*
* C.TORREGROSSA	11/09/96	: Creation
* JMG	08/01/98	: DC devient un PC niveau 2 (dem/1536) 1.2
* P.NIEPCERON   19/08/98        : Ajout route vers REP_HIS (1.3)
----------------------------------------------------
*/

/* route DC->CI */
create route to REP_CI
set username MIGRAZUR
set password MIGRAZUR
go

/* route DC->DS */
create route to REP_DS
set username MIGRAZUR
set password MIGRAZUR
go


/* route DC->DM */
create route to REP_DM
set username MIGRAZUR
set password MIGRAZUR
go

/* route DC->HIS */
create route to REP_HIS
set username MIGRAZUR
set password MIGRAZUR
go

