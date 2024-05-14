/*E*/
/*Fichier : $Id: xzao574.h,v 1.1 2012/07/04 17:53:45 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 17:53:45 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao574.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR       02 Fev 2012     : Creation (DEM/1016)
------------------------------------------------------ */

#ifndef xzao574
#define xzao574

#include "xdy.h"
#include "asql_com.h"

extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    *XZAGV_Context;


/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO574_RPC_NAME "XZAO574"


/* definitions de types exportes */

int XZAO574_Vitesse_PRV_amont ( XDY_Eqt va_Zone_in,
			XDY_Autoroute       va_Autoroute_in,
					XDY_PR 	va_PR_PMV_in, 	
				    XDY_Sens	 		va_Sens_in,
				int va_amont,
		    char *va_Vitesse_out
				 );


#endif
