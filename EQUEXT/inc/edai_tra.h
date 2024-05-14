/*Fichier :  $Id: edai_tra.h,v 1.4 2000/05/31 12:12:21 gesconf Exp $      Release : $Revision: 1.4 $        Date : $Date: 2000/05/31 12:12:21 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEDAI * FICHIER edai_tra.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier de traitement de la 
*  tache TEDAI. 
*
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E 	version 1.1	21 Oct 1994	: Creation
* Fontaine.C	version 1.2	23 Mai 1995	: Reprise entete
* Mismer.D      version 1.3     08 Jan 1998     : Modif new alerte DAI (DEM/APG17) fiche ANA74
* Mismer.D      version 1.4     30 Mai 2000     : Modif pour config avec 2 DAI sur un LT (DEM/102)
*
------------------------------------------------------ */

#ifndef edai_tra
#define edai_tra

/* fichiers inclus */

#include "edai_don.h"
#include "ex_msta.h"


/* definitions de constantes exportees a deplace ds xdc.h */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
int  em_traitement_trame(int va_socket);

#endif
