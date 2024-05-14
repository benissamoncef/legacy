/*E*/
/*  Fichier : 	$Id: xzac26.h,v 1.2 1994/11/17 11:38:18 volcic Exp $  Release : $Revision: 1.2 $ Date : $Date: 1994/11/17 11:38:18 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac26.h
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

#ifndef xzac26
#define xzac26

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZACC_XZAC26_RPC_NAME "XZAC;26"

#define XZACC_XZAC26_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAC26_Recap_Affichage_Gare ( XDY_Horodate,
					 XDY_Evt,
					 XDY_FonctionInt );



#endif
