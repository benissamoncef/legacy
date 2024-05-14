/*Fichier :  @(#)enix_tra.h	1.1      Release : 1.1        Date : 02/13/08
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TENIX * FICHIER enix_tra.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier de traitement de la
*  tache TENIX, gtc Nice
*
------------------------------------------------------
* HISTORIQUE :
*
* AAZ       version 1.1 03 Sept 2007    : GTC rénovée DEM 664
* SFO       06-10-2014 : generation pour linux: suppr. warnings
------------------------------------------------------ */
 
#ifndef enix_tra
#define enix_tra

/* fichiers inclus */

#include "enix_don.h"
#include "xzaac.h"


/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

#ifdef _HPUX_SOURCE
extern  int           vg_Mode_Fonct = 0;
#else
int           vg_Mode_Fonct = 0;
#endif
extern  int           vg_maitre_actif;

/* delaration de fonctions externes */
extern int            vg_Port_SockTWWI0;
extern int            vg_Port_SockTWWI1;

extern XDY_NomMachine vg_nom_machine_TWWI0;
extern XDY_NomMachine vg_nom_machine_TWWI1;

#endif
