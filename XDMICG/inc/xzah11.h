/*E Fichier : $Id: xzah11.h,v 1.2 2010/12/07 12:04:02 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2010/12/07 12:04:02 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAH11 * FICHIER XZAH11.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*	Module appartenant a BASEDD et declenchant un Callback de TACLI.
********************************
*	XZAH11_Init_IHM permet de reinitialiser la globale XZAHG_Init a XDC_Vrai.
*	Algo : Constitue un msg RTwks et
* 	envoie le message RTWks XDM_A_INIT_IHM.
*	Fonction permettant d envoyer le message XDM_A_INIT_IHM.
*	Le message RTwks ne contient pas d element.
*	Le message declenchera AHCL01 une CB de TACLI qui positionera la globale XZAHG_Init a XDC_Vrai.
* 
* 
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	28 Oct 1994	: Creation
* JMG	25/11/10 : compil conditionnelle linu
------------------------------------------------------ */

#ifndef XZAH11
#define XZAH11

/* fichiers inclus */
	#include        <stdio.h>
	#include        <string.h>
	#include        <ctype.h>

	/* include system necessaire pour Open Server et Open Client. */
#ifdef _HPUX_SOURCE
	#include        <ospublic.h>
	#include        <oserror.h>
#endif
	/* Include service migrazur */
	 #include "xdy.h"
	 #include "xdm.h"
	 #include "xzst.h"
	 #include "xzsc.h"
	 
/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*	XZAH11_Init_IHM permet de reinitialiser la globale XZAHG_Init a XDC_Vrai.
*	Algo : Constitue un msg RTwks et
* 	envoie le message RTWks XDM_A_INIT_IHM.
*	Fonction permettant d envoyer le message XDM_A_INIT_IHM.
*	Le message RTwks ne contient pas d element.
*	Le message declenchera AHCL01 une CB de TACLI qui positionera la globale XZAHG_Init a XDC_VRAI.
*/
extern int XZAH11_Init_IHM () ;

#endif
