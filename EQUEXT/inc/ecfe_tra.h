/*E*/
/* Fichier : $Id: ecfe_tra.h,v 1.1 2019/01/29 11:05:40 pc2dpdy Exp $     Release : $Revision: 1.1 $      Date :  $Date: 2019/01/29 11:05:40 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEPAL * FICHIER ecfe_tra.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Declaration des structures utilisees lors du traitement
*   des trames de la tache TEPAL
*  
------------------------------------------------------
* HISTORIQUE :
*
* JMG   01/03/18 :      Creation CTRL_FEUX 1.1 DEM1284
*
------------------------------------------------------ */

#ifndef ecfe_tra
#define ecfe_tra

/* fichiers inclus */

#include "ecfe_don.h"
#include "xzaac.h"


/* definitions de constantes exportees a deplace ds xdc.h */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
int  em_traitement_trame(int va_socket);

#endif
