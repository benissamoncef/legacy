/*E*/
/*  Fichier : @(#)xzat067.h	1.1      Release : 1.1        Date : 10/05/94
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzat067.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* ABE	18/05/2021 : Création DEM-SAE244 1.1
------------------------------------------------------ */

#ifndef xzat067								
#define xzat067

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAT067_RPC_NAME "XZAT067"

#define XZAT067_NB_PARM_RETURN 12

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAT067_Utilisation_PAU(	XDY_Horodate	va_Horodate_in,
				XDY_District	va_Site_in,
				XDY_Octet	*va_DispoPAU_out,
				XDY_Entier	*va_NumEvt_out,
				XDY_Octet	*va_CleEvt_out,
				XDY_Octet	*va_Priorite_out,
				XDY_Entier	*va_DistanceEvt_out,
				XDY_Entier	*va_Action_out,
				XDY_District	*va_SiteAction_out,
				XDY_MessageSONO	*va_Message_out,
				XDY_Autoroute	*va_Autoroute_out,
				XDY_Sens	*va_Sens_out,
				XDY_PR		*va_PrDebut_out,
				XDY_PR		*va_PrFin_out);
						



#endif
