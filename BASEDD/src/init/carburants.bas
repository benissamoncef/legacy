/*X  Fichier : $Id: carburants.bas,v 1.1 1995/03/15 11:06:33 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/03/15 11:06:33 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER carburants.bas
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* Initialisation de tables de la configuration
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	09/03/95	: Creation
-----------------------------------------------------*/


use CFG
go

/* conforme aux XDC_xxx */
insert TYP_CRB ( numero, nom, supprime)
values( 1, "SUPER          ", 0)
insert TYP_CRB ( numero, nom, supprime)
values( 2, "GAZOLE         ", 0)
insert TYP_CRB ( numero, nom, supprime)
values( 3, "SANS PLOMB 95  ", 0)
insert TYP_CRB ( numero, nom, supprime)
values( 4, "SANS PLOMB 98  ", 0)
insert TYP_CRB ( numero, nom, supprime)
values( 5, "GPL            ", 0)
go
