/*E*/
/* Fichier : %W%      Release : %I%        Date : %G%
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEGAB * FICHIER egab_trace.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	12/01/18 creation GABEB 1.1
------------------------------------------------------ */

#ifndef egab_trace
#define egab_trace

/* fichiers inclus */
#include "xdy.h"
#include "xdm.h"
#include <xdc.h>
#include <xdg.h>
#include <xzst.h>
#include <xzsc.h>
#include <xzsm.h>

#include "xzav01.h"
#include "xzav04.h"

/* definitions de constantes exportees */
#define EGAB_TRACE_TYPE_PARAM			XZAVC_REG_TRACE_PARAM
#define EGAB_TRACE_TYPE_DETECTION		XZAVC_REG_TRACE_DETECTION
#define EGAB_TRACE_TYPE_AJUSTEMENT		XZAVC_REG_TRACE_AJUSTEMENT
#define EGAB_TRACE_TYPE_ALERTE			XZAVC_REG_TRACE_ALERTE
#define EGAB_TRACE_TYPE_REVEIL			XZAVC_REG_TRACE_REVEIL
#define EGAB_TRACE_TYPE_VALIDATION		XZAVC_REG_TRACE_VALIDATION
#define EGAB_TRACE_TYPE_APPLICATION		XZAVC_REG_TRACE_APPLICATION

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
						XDY_Eqt va_ech,
						XDY_Entier va_numero_fmc,
						XDY_Entier va_cle_fmc,
						XDY_Eqt va_cab1,
				XDY_Octet va_contexte,
				XDY_Entier va_alerte,
				XDY_Octet va_acquittement,
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
