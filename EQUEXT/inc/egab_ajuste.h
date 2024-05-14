/*E*/
/* Fichier : %W%      Release : %I%        Date : %G%
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEREG * FICHIER egab_ajuste.h
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

#ifndef egab_ajuste
#define egab_ajuste

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
#define C_CONTEXTE_CREATION	0
#define C_CONTEXTE_MODIF	1
#define C_CONTEXTE_RIEN		2
#define C_CONTEXTE_REVEIL	3

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
int ajuste_analyse();
/*
* ARGUMENTS EN ENTREE :
*  Aucun
*
* ARGUMENTS EN SORTIE :
*  Aucun
*
* CODE RETOUR :
*  XDC_OK
*  XDC_NOK
*
* CONDITION D'UTILISATION
*  Doit etre invoque pour l'init du module
*
* FONCTION
*   Chargement de la liste des zones puis de la liste des PRV
*
------------------------------------------------------*/

#endif
