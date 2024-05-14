/*E*/
/*  Fichier : 	$Id: xzat08.h,v 1.2 1994/12/19 16:18:47 volcic Exp $  Release : $Revision: 1.2 $ Date : $Date: 1994/12/19 16:18:47 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAT * FICHIER xzat08.h
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
------------------------------------------------------ */

#ifndef xzat08
#define xzat08

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZATC_XZAT08_RPC_NAME "XZAT;08"

#define XZATC_XZAT08_NB_PARM_RETURN 8

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAT08_Utilisation_Echangeur ( 	XDY_Horodate,
						XDY_Eqt,
						XDY_Octet *,
						XDY_Octet *,
						XDY_Entier *,
						XDY_Octet *,
						XDY_Octet *,
						XDY_Entier * );



#endif
