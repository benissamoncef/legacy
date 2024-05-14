/*E*/
/* Fichier : $Id: ereg_trace.h,v 1.1 2012/07/04 14:29:48 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 14:29:48 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEREG * FICHIER ereg_trace.h
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

#ifndef ereg_trace
#define ereg_trace

/* fichiers inclus */
#include "xdy.h"
#include "xdm.h"
#include <xdc.h>
#include <xdg.h>
#include <xzst.h>
#include <xzsc.h>
#include <xzsm.h>

#include "xzav01.h"

/* definitions de constantes exportees */
#define EREG_TRACE_TYPE_PARAM			XZAVC_REG_TRACE_PARAM
#define EREG_TRACE_TYPE_FONCTIONNEMENT 	XZAVC_REG_TRACE_FONCTIONNEMENT
#define EREG_TRACE_TYPE_DETECTION		XZAVC_REG_TRACE_DETECTION
#define EREG_TRACE_TYPE_AJUSTEMENT		XZAVC_REG_TRACE_AJUSTEMENT
#define EREG_TRACE_TYPE_FORCAGE			XZAVC_REG_TRACE_FORCAGE
#define EREG_TRACE_TYPE_ALERTE			XZAVC_REG_TRACE_ALERTE
#define EREG_TRACE_TYPE_REVEIL			XZAVC_REG_TRACE_REVEIL
#define EREG_TRACE_TYPE_VALIDATION		XZAVC_REG_TRACE_VALIDATION
#define EREG_TRACE_TYPE_APPLICATION		XZAVC_REG_TRACE_APPLICATION

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
int trace_init();
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


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  initialisation
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
XDY_Entier trace_process(XDY_Octet va_type,
						XDY_Eqt va_zone,
						XDY_Entier va_condition,
						XDY_Entier va_alerte,
						XDY_Entier va_numero_fmc,
						XDY_Entier va_cle_fmc,
						XDY_Eqt va_scenario,
						XDY_Octet va_acquittement,
						XDY_Eqt va_station,
						XDY_Sens va_sens_station,
				char* Texte, ...  );
/*
* ARGUMENTS EN ENTREE :
*  XDY_Entier va_type : utiliser les contantes XZAVC_REG_TRACE...
*  XDY_Nom va_texte   : texte de la trace. ce texte doit etre formate en respectant la regle definie pour chaque type de trace
*
* ARGUMENTS EN SORTIE :
*  Aucun
*
* CODE RETOUR : 
*  XDC_OK
*  XDC_NOK
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
------------------------------------------------------*/

#endif
