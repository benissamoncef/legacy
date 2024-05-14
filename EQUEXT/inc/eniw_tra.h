/*Fichier :  @(#)eniw_tra.h	1.2      Release : 1.2        Date : 04/24/02
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TENIW * FICHIER eniw_tra.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier de traitement de la
*  tache TENIW, gtc Nice
*
------------------------------------------------------
* HISTORIQUE :
*
* Moulin.D	version 1.1	07 Janv 2002	: Creation
* Moulin.D	version 1.2	23 Avr 2002	: Installation plate forme
* SFO           06-10-2014 : Adaptation pour compilation sous linux
------------------------------------------------------ */
 
#ifndef eniw_tra
#define eniw_tra

/* fichiers inclus */

#include "eniw_don.h"
#include "xzaac.h"


/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

#ifdef _HPUX_SOURCE
extern 	int			vg_Mode_Fonct = 0;
#else
int			vg_Mode_Fonct = 0;
#endif
extern 	int			vg_maitre_actif;

/* delaration de fonctions externes */
extern int			vg_Port_SockTWWI0;
extern int			vg_Port_SockTWWI1;

extern XDY_NomMachine vg_nom_machine_TWWI0;
extern XDY_NomMachine vg_nom_machine_TWWI1;

#endif
