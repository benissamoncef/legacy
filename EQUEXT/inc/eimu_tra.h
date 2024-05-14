/*E*/
/* Fichier : $Id: eimu_tra.h,v 1.1 2020/11/04 09:26:49 pc2dpdy Exp $     Release : $Revision: 1.1 $      Date :  $Date: 2020/11/04 09:26:49 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEPAL * FICHIER eimu_tra.h
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

#ifndef eimu_tra
#define eimu_tra

/* fichiers inclus */

#include "eimu_don.h"
#include "xzaac.h"


/* definitions de constantes exportees a deplace ds xdc.h */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
int  em_traitement_trame(int va_socket);

#endif
