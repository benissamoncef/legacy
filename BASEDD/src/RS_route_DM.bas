/*X  Fichier : $Id: RS_route_DM.bas,v 1.4 1998/08/19 10:48:58 gesconf Exp $      Release : $Revision: 1.4 $        Date : $Date: 1998/08/19 10:48:58 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER RS_route_DN.bas
------------------------------------------------------
* DESCRIPTION :
* cree les routes entre les replication serveurs
* (a executer sous sa de REP_DM quand les autres REP
* tournent)
------------------------------------------------------
* HISTORIQUE :
*
* B.GABORIT	19/09/94	: Creation
* C.TORREGROSSA	21/03/95	: Modif nom de la replication (V 1.2)
*                                 et ne pas mettre une fin de commentaire
*                                 avec des tirets.
* JMG	08/01/98 : DC devient un PC niveau 2 (dem/1536) 1.3
* P.NIEPCERON   19/08/98        : Ajout route vers REP_HIS (1.4)
----------------------------------------------------
*/

/* route DM->CI */
create route to REP_CI
set username MIGRAZUR
set password MIGRAZUR
go

/* route DM->DN */
create route to REP_DN
set username MIGRAZUR
set password MIGRAZUR
go

/* route DM->DC*/
create route to REP_DC
set username MIGRAZUR
set password MIGRAZUR
go

/* route DM->HIS*/
create route to REP_HIS
set username MIGRAZUR
set password MIGRAZUR
go

