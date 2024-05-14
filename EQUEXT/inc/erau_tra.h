/*E*/
/*  Fichier : $Id: erau_tra.h,v 1.3 1995/05/22 11:44:52 cornu Exp $      Release : $Revision: 1.3 $        Date : $Date: 1995/05/22 11:44:52 $
------------------------------------------------------
* GRANIOU *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TERAU * FICHIER erau_tra.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Structure des donnees du fichier de traitement de la tache TERAU
*
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E 	version 1.1	06 Dec 1994	: Creation
* Nagiel.E	version 1.2	24 Jan 1995	:
* Fontaine.C	version 1.3	22 Mai 1995 	: Reprise entete, historique
*
------------------------------------------------------ */

#ifndef erau_tra
#define erau_tra

/* fichiers inclus */

#include "erau_don.h"


/* definitions de constantes exportees a deplace ds xdc.h */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

int  em_traitement_trame(int va_socket);

#endif
