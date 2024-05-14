/*E*/
/*Fichier : $Id: xzao575.h,v 1.1 2012/07/04 17:53:56 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 17:53:56 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao575.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR       08 Fev 2012     : Creation (DEM/1016)
------------------------------------------------------ */

#ifndef xzao575
#define xzao575

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO575_RPC_NAME "XZAO575"

#define XZAOC_XZAO575_NB_PARM_RETURN 1

/* definitions de types exportes */
int XZAO575_PMVA_Perturbation( XDY_Basedd 	va_Base,
							    XDY_Autoroute 	va_Autoroute_In,
								XDY_Sens 		va_Sens_In, 
								XDY_PR 		va_PR_Debut_In,
								XDY_PR 		va_PR_Fin_In,
								XDY_FonctionInt pa_FonctionUtilisateur_in,
								XDY_Entier      *va_Resultat_out				 
								);


#endif
