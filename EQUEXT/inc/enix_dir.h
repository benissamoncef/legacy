/*E*/
/*Fichier :  $Id: enix_dir.h,v 1.2 2010/05/17 15:01:30 devgtie Exp $      Release : $Revision: 1.2 $        Date : $Date: 2010/05/17 15:01:30 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TENIX * FICHIER enix_dir.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier directeur de la tache TENIX, gtc Nice
*
------------------------------------------------------
* HISTORIQUE :
*
* AAZ       version 1.1 03 sept 2007    : GTC rénovée DEM 664
* ADB       22 mars 2010 : FT 3448 - Gestion des etats liaisons
*
------------------------------------------------------ */

#ifndef enix_dir
#define enix_dir
 
/* fichiers inclus */

#include        <xdy.h>
#include        <xdc.h>
#include        <xzst.h>
#include        <xzsc.h>

#include        <rtworks/common.h>
#include        <rtworks/ipc.h>

#include        "ex_msok.h"
#include        "etim_don.h"
#include        "enix_don.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

ENIX_DONNEES_Tube       *pg_debutTube       = NULL;
ENIX_DONNEES_Echangeur  *pg_debutEchangeur  = NULL;
/* AAZ 03/09/2007 */
ENIX_DONNEES_SIG        *pg_debutSIG        = NULL;
ENIX_DONNEES_ECL        *pg_debutECL        = NULL;
ENIX_DONNEES_VEN        *pg_debutVEN        = NULL;
ENIX_DONNEES_ENR        *pg_debutENR        = NULL;
ENIX_DONNEES_INC        *pg_debutINC        = NULL;
ENIX_DONNEES_EXP        *pg_debutEXP        = NULL;
ENIX_DONNEES_SYS        *pg_debutSYS        = NULL;
ENIX_DONNEES_LTN        *pg_debutLTN        = NULL;

XDY_NomMachine          vg_NomMachine;

XZEXT_MASK *            vg_SockMask         = NULL;
T_INT4                  vg_SockRT           = 0;
int                     vg_SockTWWI0        = 0;
int                     vg_SockTWWI1        = 0;
int                     vg_SockTETIM        = 0;

int                     vg_Port_SockTWWI0   = 0;
int                     vg_Port_SockTWWI1   = 0;

int                     vg_TypeEqt_LiaiTWWI0 = 0;
int						vg_NumEqt_LiaiTWWI0 = 0; 

int                     vg_TypeEqt_LiaiTWWI1 = 0;
int						vg_NumEqt_LiaiTWWI1 = 0; 

/*etat de connexion des sockets*/
int                     vg_etat_SockTWWI0   = XDC_OK;
int                     vg_etat_SockTWWI1   = XDC_OK;
int                     vg_maitre_actif     = XDC_MAITRE_INCONNU;

XDY_NomMachine          vg_nom_machine_TWWI0;
XDY_NomMachine          vg_nom_machine_TWWI1;

/* delaration de fonctions externes */
extern int              et_Redondance();
extern int              et_Synchro();
extern int              ex_lire_config_Wizcon();

#endif
