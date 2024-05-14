/*E*/
/*  Fichier : 	$Id: xzac23.h,v 1.2 1994/11/17 11:38:15 volcic Exp $  Release : $Revision: 1.2 $ Date : $Date: 1994/11/17 11:38:15 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac23.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic       03 Nov 1994     : Creation
------------------------------------------------------ */

#ifndef xzac23
#define xzac23

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZACC_XZAC23_RPC_NAME "XZAC;23"

#define XZACC_XZAC23_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAC23_Rech_Partenaires_Informes ( 	XDY_Horodate,
						XDY_Evt,
						XDY_District,
						XDY_FonctionInt );



#endif
