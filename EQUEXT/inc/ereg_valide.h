/*E*/
/* Fichier : $Id: ereg_valide.h,v 1.1 2012/07/04 14:30:08 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 14:30:08 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEREG * FICHIER ereg_valide.h
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

#ifndef ereg_valide
#define ereg_valide

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
#define EREG_VALIDE_SOUS_TYPE_ALERTE_TERMINER_FORCAGE		1
#define EREG_VALIDE_SOUS_TYPE_ALERTE_DECLENCHEMENT			2
#define EREG_VALIDE_SOUS_TYPE_ALERTE_AJUSTEMENT				3
#define EREG_VALIDE_SOUS_TYPE_ALERTE_ARRET					4
#define EREG_VALIDE_SOUS_TYPE_ALERTE_CHANGEMENT_CONTEXTE	5


/* definitions de types exportes */


/* definition de macro exportees */


/* declarations de variables globales */


/* declaration de fonctions externes */

/* declaration de structures*/


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

/* Todo : mettre les bons parametres */
int valide_scenario(T_ZONE_REGUL *);

#endif
