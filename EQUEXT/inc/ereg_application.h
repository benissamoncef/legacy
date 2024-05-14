/*E*/
/* Fichier : $Id: ereg_application.h,v 1.1 2012/07/04 14:29:11 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 14:29:11 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEREG * FICHIER ereg_application.h
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

#ifndef ereg_application
#define ereg_application

/* fichiers inclus */
#include "xdy.h"
#include "xdm.h"
#include <xdc.h>
#include <xdg.h>
#include <xzst.h>
#include <xzsc.h>
#include <xzsm.h>
#include "ereg_cfg.h"
#include "ereg_trace.h"
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
void appli_analyse(T_ZONE_REGUL *pa_Zone);
void appli_process(T_ZONE_REGUL *pa_Zone);

#endif
