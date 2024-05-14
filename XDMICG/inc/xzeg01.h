/*E Fichier :$Id: xzeg01.h,v 1.1 2012/07/04 11:04:29 pc2dpdy Exp $	      Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 11:04:29 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME XDMICG
------------------------------------------------------
* MODULE XZEG01 * FICHIER xzeg01.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* les services fournis par SAIDEC :
*
------------------------------------------------------
* HISTORIQUE :
*
* VR	16 Fev 2012	: Creation (DEM/1016)
------------------------------------------------------ */

#ifndef XZEG
#define XZEG

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
int XZEG01_informer_regulation( XDY_Entier    	va_Num_FMC_in,
			XDY_Entier     	va_Cle_FMC_in,
			XDY_Entier	va_zone_in,
			XDY_Entier	va_scenario_in,
			XDY_Entier	va_Fin_FMC_in,
			char 	*va_site_in);

#endif
