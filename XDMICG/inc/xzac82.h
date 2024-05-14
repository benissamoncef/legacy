/*E*/
/*X  Fichier : $Id: xzac82.h,v 1.2 2012/11/08 09:54:08 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2012/11/08 09:54:08 $
------------------------------------------------------
*  * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac82.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* PNI	23/08/06	creation
* VR	25/05/12 : Ajout DTP PMVA (DEM/1014 PMA)
------------------------------------------------------ */

#ifndef xzac82
#define xzac82

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZACC_XZAC82_RPC_NAME "XZAC82"

#define XZACC_XZAC82_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAC82_Commande_PMVA (  XDY_Horodate,
									 XDY_Entier,
									 XDY_Octet,
									 XDY_Operateur,
									 XDY_Eqt,
									 XDY_Octet,
									 char *,
									 char *,
									 char *,
									 char *,
									 char *,
									 char *,
									 char *,
									 char *,
									 char *,
									 char *,
									 XDY_Octet,
									 XDY_Octet,
									 XDY_Octet,
									 XDY_Octet,
									 XDY_Octet,
									 XDY_Octet,
									 XDY_Octet,
									 XDY_Entier,
									 XDY_Mot,
									 XDY_Octet,
									 XDY_Mot,
									 XDY_Mot,
									 XDY_Mot,
									 XDY_Mot,
									 XDY_Entier    );



#endif
