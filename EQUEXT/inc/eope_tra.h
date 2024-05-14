/*E Fichier : $Id: eope_tra.h,v 1.2 1995/05/04 20:14:17 cornu Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/05/04 20:14:17 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME EQUEXT
------------------------------------------------------
* MODULE TEOPE * FICHIER eope_tra.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Declaration des variables et fonctions du fichier de traitement du module TEOPE
*
------------------------------------------------------
* HISTORIQUE :
*
* Nouaille M.	Version 1.1	02 Dec 1994	: Creation
* Fontaine C.   Version 1.2     04 Mai 1995     : Reprise entete
------------------------------------------------------ */

#ifndef eope_tra
#define eope_tra

/* fichiers inclus */

#include "eope_don.h"
/* #include "xzaac.h" */
/* #include "ex_malt.h" */

/* definitions de constantes exportees a deplace ds xdc.h */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
int  et_analyser_trame(int va_socket);

#endif
