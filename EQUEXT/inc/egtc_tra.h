/*E*/
/*Fichier : $Id: egtc_tra.h,v 1.2 1995/05/19 15:01:47 cornu Exp $       Release : $Revision: 1.2 $     Date : $Date: 1995/05/19 15:01:47 $
------------------------------------------------------
* GRANIOU *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEGTC * FICHIER egtc_tra.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Structure des donnees du fichier gerant les trames en    
* provenance de la GTC
*
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E	version 1.1	19 Dec 1994	: Creation
* Fontaine.C	version 1.2	19 Mai 1995	: Reprise entete
*
------------------------------------------------------ */

#ifndef egtc_tra
#define egtc_tra

/* fichiers inclus */

#include "egtc_don.h"

/* definitions de constantes exportees a deplace ds xdc.h */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
int  em_traitement_trame(int va_socket);

#endif
