/*E*/
/*Fichier : $Id: enav_tra.h,v 1.3 1995/05/18 18:45:49 cornu Exp $      Release : $Revision: 1.3 $        Date : $Date: 1995/05/18 18:45:49 $
------------------------------------------------------
* GRANIOU *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TENAV * FICHIER enav_tra.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Declaration des structures utilisees lors du traitement
*   des trames de la tache TENAV
*  
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E 	version 1.1	24 Nov 1994	: Creation
* Mismer.D	version 1.2	26 Dec 1994	:
* Fontaine.C	version 1.3	18 Mai 1995	: Reprise entete, historique
*
------------------------------------------------------ */

#ifndef enav_tra
#define enav_tra

/* fichiers inclus */

#include "enav_don.h"
#include "xzaac.h"
/*#include "ex_malt.h"*/


/* definitions de constantes exportees a deplace ds xdc.h */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
int  em_traitement_trame(int va_socket);

#endif
