/*E*/
/* Fichier : $Id: spng_don.c,v 1.4 1995/07/25 09:48:25 milleville Exp $	Release : $Revision: 1.4 $        Date : $Date: 1995/07/25 09:48:25 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SPNG_DON * FICHIER spng_don.c
******************************************************
* DESCRIPTION DU MODULE :
*
* Il s'agit du module decrivant les donnees et les 
* variables communes a toute la tache. tspng.x
*
******************************************************
* HISTORIQUE :
*
* T.Milleville  30 Sep 1994	: Creation
* T.Milleville  25 Jul 1995	: Mise a jour commentaires V1.4
*
*******************************************************/


/* fichiers inclus */
#include "xzsc.h"

#define SG_DONNEES_C
#include "spng_don.h"



/* definitions de constantes */

/* definitions des types globaux */

/* definitions des variables globales */
XZSCT_NomSite vg_NomSite	= "";
	/* Premier element de la liste des tests reseau en cours */
SGT_TEST_RES *pm_PremTest = NULL; 
	/* Dernier element de la liste des tests reseau en cours */
SGT_TEST_RES *pm_DernTest = NULL; 

static char *version = "$Id: spng_don.c,v 1.4 1995/07/25 09:48:25 milleville Exp $ : SPNG_DON" ;


/* definition des fonctions externes */




