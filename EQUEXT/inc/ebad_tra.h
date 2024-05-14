/*E*/
/* Fichier : $Id: ebad_tra.h,v 1.1 2007/03/26 11:08:01 gesconf Exp $     Release : $Revision: 1.1 $       Date : $Date: 2007/03/26 11:08:01 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TENAV * FICHIER ebad_tra.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module d'initialisation des structures en memoires
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	15/09/05	creation
*
------------------------------------------------------ */

#ifndef ebad_tra
#define ebad_tra

/* fichiers inclus */

#include "ebad_don.h"
#include "xzaac.h"
/*#include "ex_malt.h"*/


/* definitions de constantes exportees a deplace ds xdc.h */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
int  em_traitement_trame(int va_socket);

#endif
