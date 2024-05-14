/*E*/
/*  Fichier : 	$Id: xzat07.h,v 1.3 2009/10/22 15:46:13 pc2dpdy Exp $  Release : $Revision: 1.3 $ Date : $Date: 2009/10/22 15:46:13 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAT * FICHIER xzat07.h
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
* JMG	26/09/08		: ajout domaine GTC v1.3 DEM895
------------------------------------------------------ */

#ifndef xzat07
#define xzat07

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZATC_XZAT07_RPC_NAME "XZAT;07"

#define XZATC_XZAT07_NB_PARM_RETURN 9

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAT07_Utilisation_Tunnel ( 		XDY_Horodate,
						XDY_Eqt,
						XDY_Octet,
						XDY_Octet *,
						XDY_Octet *,
						XDY_Entier *,
						XDY_Octet *,
						XDY_Octet *,
						XDY_Entier *,
						XDY_Octet *);



#endif
