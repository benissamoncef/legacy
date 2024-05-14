/*E*/
/* Fichier : $Id: xzas91.h,v 1.2 2010/11/30 10:19:40 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2010/11/30 10:19:40 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAS91 * FICHIER XZAS06.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Appartient a TACLI : permet de declencher l animation des
* courbes des synoptiques.
*
------------------------------------------------------
* HISTORIQUE :
*
* niepceron	05 jan 2007	: Creation v1.1 DEM605
* JPL		30/11/10 : Migration architecture HP ia64 (DEM 961) : fichiers en-tete systeme inclus  1.2
------------------------------------------------------ */

#ifndef XZAS91
#define XZAS91

/* fichiers inclus */
	/* include system */
	#include        <stdio.h>
	#include        <string.h>
	#include        <ctype.h>
	#include        <fcntl.h>

#if defined (__hp9000s300)
	#include        <tcio.h>
#endif

	/* Include service migrazur */
	 #include "xdc.h"
	 #include "xdy.h"
	 #include "xdm.h"
	 
	 #include "xzst.h"
	 #include "xzsc.h"
	 #include "xzsm.h"
	#include "asql_com.h"

	 
/* definitions de constantes exportees */
#define XZASC_XZAS91_RPC_NAME "XZAS91"

#define XZASC_XZAS91_NB_PARM_RETURN 0

/* definitions de types exportes */


/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
extern int XZAS91_Derniere_Donnees_RADT (XDY_FonctionInt, XDY_Entier *	); 


extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    *XZAGV_Context;


#endif
