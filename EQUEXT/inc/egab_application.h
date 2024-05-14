/*E*/
/* Fichier : %W%      Release : %I%        Date : %G%
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEREG * FICHIER egab_application.h
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

#ifndef egab_application
#define egab_application

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
/* definitions de constantes exportees */

/* definitions de types exportes */


/* definition de macro exportees */


/* declarations de variables globales */

/* delaration de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  initialisation
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int appli_init(void);
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

/* Todo : mettre les bons parametres */

#endif
