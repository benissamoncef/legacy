/*X  Fichier : $Id: categories.bas,v 1.2 1995/04/06 12:27:17 gaborit Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/04/06 12:27:17 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER categories.bas
------------------------------------------------------
* DESCRIPTION :
* renseigne les tables de CFG (È faire au CI)
------------------------------------------------------
* HISTORIQUE :
*
* B.G.     	??/11/95	: Creation 
* C.T.     	15/03/95	: Creation avec entete (V 1.1)
* BG 		06/04/95	: ajout des districts (1.2)
----------------------------------------------------*/

use CFG
go

delete ANN_AST_CAT
go

insert ANN_AST_CAT (numero,nom)
values (1,"GENERAL ESCOTA")

insert ANN_AST_CAT (numero,nom)
values (2,"NICE")

insert ANN_AST_CAT (numero,nom)
values (3,"MANDELIEU")

insert ANN_AST_CAT (numero,nom)
values (4,"LE CANNET")

insert ANN_AST_CAT (numero,nom)
values (5,"AUBAGNE")

insert ANN_AST_CAT (numero,nom)
values (6,"PEYRUIS")

insert ANN_AST_CAT (numero,nom)
values (7,"SAINT MAXIMIN")

insert ANN_AST_CAT (numero,nom)
values (8,"MEYRARGUES")

insert ANN_AST_CAT (numero,nom)
values (9,"MAINTENANCE")

go

