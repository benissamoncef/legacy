/*Fichier : $Id: xzao5771.h
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao5771.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* GGY	08/12/23 : Creation (DEM-483)
------------------------------------------------------ */
#ifndef xzao5771
#define xzao5771

/* fichiers inclus */
/* definitions de constantes exportees */
#define XZAOC_XZAO5771_RPC_NAME "XZAO5771"
#define XZAOC_XZAO5771_NB_PARM_RETURN 1

/* definitions de types exportes */
int XZAO5771_PIC_chantier ( XDY_Autoroute 	va_Autoroute_In,
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
