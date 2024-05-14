/*E Fichier : @(#)etel_tra.h	1.2      Release : 1.2        Date : 05/04/95
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME EQUEXT
------------------------------------------------------
* MODULE TETEL * FICHIER etel_tra.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Declaration des variables utilisées par le fichier etel_tra.c
*
*
------------------------------------------------------
* HISTORIQUE :
*
* Nouaille M. Version 1.1	30 Nov 1994	: Creation
* Fontaine C. Version 1.2       04 Mai 1995     : Reprise entete
* Diaz.L      Version 1.3	09 Oct 1998     : Ajout PABX
------------------------------------------------------ */

#ifndef etel_tra
#define etel_tra

/* fichiers inclus */

#include "etel_don.h"
/* #include "xzaac.h" */
/* #include "ex_malt.h" */

/* definitions de constantes exportees a deplace ds xdc.h */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
int  et_analyser_trame(int va_socket);

#endif
