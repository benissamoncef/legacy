/*X  Fichier : $Id: motifs.bas,v 1.1 1995/03/15 11:18:49 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/03/15 11:18:49 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER motifs.bas
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* Initialisation des tables de configuration
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	09/03/95	: Creation
-----------------------------------------------------*/


use CFG
go

delete APL_MTF
go

insert APL_MTF (numero, nom)
values( 1, "TARIFS ESCOTA            ")
insert APL_MTF (numero, nom)
values( 2, "TARIFS AUTRES AUTOROUTES ")
insert APL_MTF (numero, nom)
values( 3, "TEMPS PARCOURS, DISTANCES")
insert APL_MTF (numero, nom)
values( 4, "ITINERAIRES              ")
insert APL_MTF (numero, nom)
values( 5, "ADRESSES, TEL. ESCOTA    ")
insert APL_MTF (numero, nom)
values( 6, "ADRESSES, TEL. SOCIETES  ")
insert APL_MTF (numero, nom)
values( 7, "CLASSES TARIFAIRES       ")
insert APL_MTF (numero, nom)
values( 8, "ABONNEMENTS              ")
insert APL_MTF (numero, nom)
values( 9, "METEO                    ")
insert APL_MTF (numero, nom)
values( 10, "COND. CIRCULATION        ")
insert APL_MTF (numero, nom)
values( 11, "CONVOIS EXCEPTIONNELS    ")
insert APL_MTF (numero, nom)
values( 12, "HAUTEURS SOUS OUVRAGE    ")
insert APL_MTF (numero, nom)
values( 13, "AUTRES                   ")
go
