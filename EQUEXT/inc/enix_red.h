/*Fichier :  $Id: enix_red.h,v 1.2 2010/05/17 15:01:30 devgtie Exp $      Release : $Revision: 1.2 $        Date : $Date: 2010/05/17 15:01:30 $
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
* AAZ           version 1.1 03 Sept 2007        : GTC rénovée DEM 664
* ADB       22 mars 2010 : FT 3448 - Gestion des etats liaisons
*
------------------------------------------------------ */

#ifndef enix_red
#define enix_red
 
/* fichiers inclus */

#include "ex_msok.h"
#include "enix_don.h"
#include "xzaac.h"


/* definitions de constantes exportees */


/* definitions de types exportes */

/* definitions de macro exportees */

/* declarations de donnees globales */
extern int            vg_etat_SockTWWI0;
extern int            vg_etat_SockTWWI1;
extern int            vg_maitre_actif;

extern XDY_NomMachine vg_nom_machine_TWWI0;
extern XDY_NomMachine vg_nom_machine_TWWI1;

extern int            vg_Port_SockTWWI0;
extern int            vg_Port_SockTWWI1;

extern int      vg_TypeEqt_LiaiTWWI0 ;
extern int	vg_NumEqt_LiaiTWWI0 ; 

extern int      vg_TypeEqt_LiaiTWWI1 ;
extern int	vg_NumEqt_LiaiTWWI1 ; 



/* declarations de fonctions externes */

#endif
