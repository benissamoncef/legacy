/*E*/
/*Fichier : $Id: xzao577.h,v 1.1 2012/07/04 17:54:02 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 17:54:02 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao577.h
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

#ifndef xzao577
#define xzao577

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO577_RPC_NAME "XZAO577"

#define XZAOC_XZAO577_NB_PARM_RETURN 1

/* definitions de types exportes */
int XZAO577_PRV_chantier( 		XDY_Autoroute 	va_Autoroute_In,
								XDY_Sens 		va_Sens_In,
								XDY_PR 			va_PR_In,
								XDY_PR 			va_PR_tete_In,
								XDY_District	va_District_In,
								XDY_Entier		va_Priorite_In,
								XDY_Horodate	va_Horodate_In,
								char*			va_Perturbation_in,
								XDY_FonctionInt pa_FonctionUtilisateur_in,
								XDY_Entier      *va_Resultat_out
								);


#endif
