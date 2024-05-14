/*E*/
/*  Fichier : 	$Id: xzat280.h,v 1.2 2017/01/10 11:21:53 devgfi Exp $  Release : $Revision: 1.2 $ Date : $Date: 2017/01/10 11:21:53 $
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
* JMG	27/04/04	creation	1.1
* JPL	05/12/16	Ajout d'un parametre pour retour du type d'evenement (DEM 1193)  1.2
------------------------------------------------------ */

#ifndef xzat280
#define xzat280

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZATC_XZAT280_RPC_NAME "XZAT280"

#define XZATC_XZAT280_NB_PARM_RETURN 9

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAT280_Utilisation_PAL ( 	XDY_Horodate,
					XDY_Eqt,
					XDY_Octet *,
					char *,
					XDY_Entier *,
					XDY_Octet *,
					XDY_Octet *,
					XDY_Entier *,
					XDY_Octet * );



#endif
