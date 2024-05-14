/*E Fichier : $Id: xzdd.h,v 1.3 1995/01/25 11:15:59 milleville Exp $	      Release : $Revision: 1.3 $        Date : $Date: 1995/01/25 11:15:59 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME XDMICG
------------------------------------------------------
* MODULE XZDD * FICHIER xzdd.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* les services fournis par SAIDEC :
*	XZDD01_PrendreEnCompteBDC()
*	XZDD02_CalculerPA()
*
------------------------------------------------------
* HISTORIQUE :
*
* IT	09 Sep 1994	: Creation
* IT	14 Dec 1994	: Ajout XZDDC_DIFF_SITE_DEV
------------------------------------------------------ */

#ifndef XZDD
#define XZDD

/* fichiers inclus */
#include <rtworks/common.h>
#include <rtworks/ipc.h>
#include "xdg.h"
#include "xdm.h"
#include "xdy.h"
#include "xzsc.h"
#include "xzst.h"

/* definitions de constantes exportees */
#define XZDDC_DIFF_SITE_DEV	"DEV"
#define XZDDC_PA_AVEC_TFM	1
#define XZDDC_PA_SANS_TFM	0
#define XZDDC_MODE_TEST		0
#define XZDDC_MODE_OPERATIONNEL	1

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
extern XDY_Booleen XZDD01_PrendreEnCompteBDC();
extern XDY_Booleen XZDD02_CalculerPA();

#endif
