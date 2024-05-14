/*E*/
/* Fichier : %W%      Release : %I%        Date : %G%
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEREG * FICHIER egab_valide.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  
*
------------------------------------------------------
* HISTORIQUE :
*
* SDU		15/03/2012 : Creation DEM/1016
------------------------------------------------------ */

#ifndef egab_valide
#define egab_valide

/* fichiers inclus */
#include "xdy.h"
#include "xdm.h"
#include <xdc.h>
#include <xdg.h>
#include <xzst.h>
#include <xzsc.h>
#include <xzsm.h>
#include "egab_cfg.h"
#include "egab_trace.h"


/* definitions de types exportes */


/* definition de macro exportees */


/* declarations de variables globales */
#define C_MANUEL	0
#define C_SEMI_AUTO	1
#define C_AUTO		2
#define C_ALERTE_FIN	0
#define C_ALERTE_MODIF	1
#define C_ALERTE_CREATION	2

/* declaration de fonctions externes */

/* declaration de structures*/
XDY_Octet	vm_mode;

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  initialisation
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int valide_init();
/*
* ARGUMENTS EN ENTREE :
*  Aucun
*
* ARGUMENTS EN SORTIE :
*  Aucun
*
* CODE RETOUR :
*  Aucun
*
* CONDITION D'UTILISATION
*  Doit etre invoque pour l'init du module
*
* FONCTION
*   Chargement de la liste des zones puis de la liste des PRV
*
------------------------------------------------------*/

int valide_termine(T_Liste_ECH *pa_ech, int va_mode, T_Bouchon va_bouchon);

#endif
