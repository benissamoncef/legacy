/*E*/
/* Fichier : $Id: epal_tra.h,v 1.1 1998/01/30 15:25:26 mismer Exp $     Release : $Revision: 1.1 $      Date :  $Date: 1998/01/30 15:25:26 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEPAL * FICHIER epal_tra.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Declaration des structures utilisees lors du traitement
*   des trames de la tache TEPAL
*  
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D 	version 1.1	15 Jan 1998	: Creation
*
------------------------------------------------------ */

#ifndef epal_tra
#define epal_tra

/* fichiers inclus */

#include "epal_don.h"
#include "xzaac.h"


/* definitions de constantes exportees a deplace ds xdc.h */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
int  em_traitement_trame(int va_socket);

#endif
