/*E*/
/*  Fichier : 	$Id: xzac86.h,v 1.1 2005/09/13 11:44:38 gesconf Exp $  Release : $Revision: 1.1 $ Date : $Date: 2005/09/13 11:44:38 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac86.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	09/06/05: Creation
------------------------------------------------------ */

#ifndef xzac86
#define xzac86

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZACC_XZAC86_RPC_NAME "XZAC86"

#define XZACC_XZAC86_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAC86_Recap_Info_PMVA ( 	XDY_Horodate,
					XDY_Evt,
					XDY_FonctionInt );



#endif
