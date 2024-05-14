/*E*/
/*  Fichier : 	$Id: xzac25.h,v 1.2 1994/11/17 11:38:16 volcic Exp $  Release : $Revision: 1.2 $ Date : $Date: 1994/11/17 11:38:16 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac25.h
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

#ifndef xzac25
#define xzac25

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZACC_XZAC25_RPC_NAME "XZAC;25"

#define XZACC_XZAC25_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAC25_Recap_Info_PMV ( 	XDY_Horodate,
					XDY_Evt,
					XDY_FonctionInt );



#endif
