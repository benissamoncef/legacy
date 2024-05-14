/*E*/
/* Fichier : $Id: ereg_detection.h,v 1.1 2012/07/04 14:29:30 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 14:29:30 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEREG * FICHIER ereg_detection.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  
*
------------------------------------------------------
* HISTORIQUE :
*
* SDU		13/03/2012 : Creation DEM/1016
------------------------------------------------------ */

#ifndef ereg_detection
#define ereg_detection

/* fichiers inclus */
#include "xdy.h"
#include "xdm.h"
#include <xdc.h>
#include <xdg.h>
#include <xzst.h>
#include <xzsc.h>
#include <xzsm.h>
#include <xzamc.h>
#include <xzaac.h>
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
XDY_Entier det_init(char *va_scenarioDefaut);
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
*
------------------------------------------------------*/

#endif
