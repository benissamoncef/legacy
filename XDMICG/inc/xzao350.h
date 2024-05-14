/*E*/
/*Fichier : $Id: xzao350.h,v 1.2 2009/10/22 15:55:48 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2009/10/22 15:55:48 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao350.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture de la configuration du reseau
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	29 Jui 1995	: Creation DEM895
------------------------------------------------------ */

#ifndef xzao350
#define xzao350

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO350_RPC_NAME "XZAO350"

#define XZAOC_XZAO350_NB_PARM_RETURN 5

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO350_Zone_Tunnel (XDY_Autoroute   va_autoroute_in,
                        XDY_PR          va_pr_tete_in,
                        XDY_PR          va_pr_queue_in,
                        XDY_Octet       va_sens_in,
			XDY_Mot		va_tunnel_in,
                        XDY_Octet       *va_zone_out,
                        XDY_Octet       *va_type_out,
                        char       *va_groupe_out,
			XDY_PR	*va_PRPSTT_out);

			

extern tg_Connexion  *XZAGV_Connexion;
extern tg_Context    *XZAGV_Context;

#endif
