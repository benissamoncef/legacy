/*E*/
/*  Fichier : 	$Id: xzac22.h,v 1.2 1994/11/17 11:38:14 volcic Exp $  Release : $Revision: 1.2 $ Date : $Date: 1994/11/17 11:38:14 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac22.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic       03 Nov 1994     : Creation
* volcic       07 Nov 1994     : Ajout Parametres Resultats (v1.2)
------------------------------------------------------ */

#ifndef xzac22
#define xzac22

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZACC_XZAC22_RPC_NAME "XZAC;22"

#define XZACC_XZAC22_NB_PARM_RETURN 3

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAC22_Rech_Intervenants_Sur_Place ( XDY_Horodate,
						XDY_Evt,
						XDY_Booleen *,
						XDY_Booleen *,
						XDY_Booleen * );



#endif
