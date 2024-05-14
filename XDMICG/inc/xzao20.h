/*E*/
/*Fichier : $Id: xzao20.h,v 1.4 2009/10/22 15:55:32 pc2dpdy Exp $      Release : $Revision: 1.4 $        Date : $Date: 2009/10/22 15:55:32 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao20.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	17 Oct 1994	: Creation
* C.T.          13 Jan 1995     : Ajout de la date en entree (V1.2)
* JMG		24/09/08 : ajout de nouveaux parametres pour nouvelle politique SAD GTC 1.4 DEM895
------------------------------------------------------ */

#ifndef xzao20
#define xzao20

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO20_RPC_NAME "XZAO;20"

#define XZAOC_XZAO20_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO20_Rech_Tunnels (XDY_Autoroute,
				XDY_PR,
				XDY_Octet,
				XDY_FonctionInt,
				XDY_Horodate,
				XDY_PR,
				XDY_Octet,
				char *,
				int,
				XDY_Octet,
				XDY_Octet,
				XDY_Octet,
				XDY_Octet,
				XDY_Octet,
				XDY_Octet,
				XDY_Octet,
				char *,
				int);

#endif
