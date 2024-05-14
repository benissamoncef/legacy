/*E Fichier : $Id: xzav03.h,v 1.1 2012/07/04 17:53:34 pc2dpdy Exp $     Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 17:53:34 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME XDMICG
------------------------------------------------------
* MODULE XZAV03 * FICHIER xzav03.h
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
* VR	04/07/12 creation DEM/1016
------------------------------------------------------ */

#ifndef XZAV
#define XZAV

/* fichiers inclus */
#include <rtworks/common.h>
#include <rtworks/ipc.h>
#include "xdg.h"
#include "xdm.h"
#include "xdy.h"
#include "xzsc.h"
#include "xzst.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
int XZAV03_Demarrer_Simulation(XDY_Horodate Horodate_Debut,XDY_Horodate Horodate_Fin, XDY_Eqt NumPosteOperateur, XDY_NomMachine NomPosteOperateur);

#endif
