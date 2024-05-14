/*X  Fichier : $Id: RS_route_DN.bas,v 1.3 1998/08/19 10:48:59 gesconf Exp $      Release : $Revision: 1.3 $        Date : $Date: 1998/08/19 10:48:59 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER RS_route_DN.bas
------------------------------------------------------
* DESCRIPTION :
* cree les routes entre les replication serveurs
* (a executer sous sa de REP_DN quand les autres REP
* tournent)
------------------------------------------------------
* HISTORIQUE :
*
* B.GABORIT	19/09/94	: Creation
* C.TORREGROSSA	21/03/95	: Modif nom de la replication (V 1.2)
*                                 et ne pas mettre une fin de commentaire
*                                 avec des tirets.
* P.NIEPCERON   19/08/98        : Ajout route vers REP_HIS (1.3)
----------------------------------------------------*/

/* route DN->CI */
create route to REP_CI
set username MIGRAZUR
set password MIGRAZUR
go

/* route DN->DM */
create route to REP_DM
set username MIGRAZUR
set password MIGRAZUR
go

/* route DN->HIS */
create route to REP_HIS
set username MIGRAZUR
set password MIGRAZUR
go

