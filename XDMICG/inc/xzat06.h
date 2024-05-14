/*E*/
/*  Fichier : 	$Id: xzat06.h,v 1.3 2017/01/10 11:21:41 devgfi Exp $  Release : $Revision: 1.3 $ Date : $Date: 2017/01/10 11:21:41 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAT * FICHIER xzat06.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Equipements >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic       03 Nov 1994     : Creation
* volcic       30 Nov 1994     : Modif Nb parametres sorties (v1.2)
* JPL		05/12/16	: Ajout d'un parametre pour retour du type d'evenement (DEM 1193)  1.3
------------------------------------------------------ */

#ifndef xzat06
#define xzat06

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZATC_XZAT06_RPC_NAME "XZAT;06"

#define XZATC_XZAT06_NB_PARM_RETURN 9

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAT06_Utilisation_NAV ( 	XDY_Horodate,
					XDY_Eqt,
					XDY_Octet *,
					char *,
					XDY_Entier *,
					XDY_Octet *,
					XDY_Octet *,
					XDY_Entier *,
					XDY_Octet * );



#endif
