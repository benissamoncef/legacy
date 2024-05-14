/*E*/ 
/*Fichier :  @(#)enix_cfg.h	1.1      Release : 1.1        Date : 02/13/08
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TENIX * FICHIER enix_cfg.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier de configuration de la
*  tache TENIX, gtc Nice
*
------------------------------------------------------
* HISTORIQUE :
*
* AAZ       version 1.1 03 sept 2007    : GTC rénovée DEM 664
* ADB       22 mars 2010 : FT 3448 - Gestion des etats liaisons
*
------------------------------------------------------ */
 
#ifndef enix_cfg
#define enix_cfg

/* fichiers inclus */

#include "ex_msok.h"
#include "enix_don.h"


/* definitions de constantes exportees */

/* definitions de types exportes */

/* definitions de macro exportees */

/* declarations de donnees globales */
extern  T_INT4        vg_SockST;
extern  XDY_NomSite   vg_NomSite;

/* delarations de fonctions externes */

extern int            vg_Port_SockTWWI0;
extern int            vg_Port_SockTWWI1;

extern XDY_NomMachine vg_nom_machine_TWWI0;
extern XDY_NomMachine vg_nom_machine_TWWI1;
extern int      vg_TypeEqt_LiaiTWWI0 ;
extern int	vg_NumEqt_LiaiTWWI0 ; 

extern int      vg_TypeEqt_LiaiTWWI1 ;
extern int	vg_NumEqt_LiaiTWWI1 ; 

#endif
