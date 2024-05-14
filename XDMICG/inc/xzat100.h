/*E*/
/* Fichier : $Id: xzat100.h,v 1.1 2012/07/04 11:02:58 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 11:02:58 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAT * FICHIER xzat100.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Equipements >
*
------------------------------------------------------
* HISTORIQUE :
*
* VRE 	06/02/12		creation (DEM/1016)	1.1
------------------------------------------------------ */

#ifndef xzat100
#define xzat100

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZATC_XZAT100_RPC_NAME "XZAT100"
#define XZATC_XZAT100_NB_PARM_RETURN 10  

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAT100_Utilisation_PRV  (XDY_Horodate,
				XDY_Mot,
				XDY_Octet *,
				XDY_Octet *,
				XDY_Bandeau *,
				XDY_VitesseRegulTxt *,
				XDY_Entier *, 
				XDY_Octet *,
				XDY_Octet *,
				XDY_Entier *,
				XDY_Entier *,
				XDY_NomSite *);


#endif
