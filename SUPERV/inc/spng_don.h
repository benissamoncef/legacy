/*E*/
/* Fichier : $Id: spng_don.h,v 1.3 1995/07/20 14:01:24 milleville Exp $        Release : $Revision: 1.3 $        Date : $Date: 1995/07/20 14:01:24 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SPNG_DON  * FICHIER spng_don.h
******************************************************
* DESCRIPTION DU MODULE :
*
* Il s'agit du module decrivant les donnees et les
* variables communes a toute la tache.
*
******************************************************
* HISTORIQUE :
*
* T.Milleville 30 Sep 1994	: Creation
*
*******************************************************/

#ifndef SPNG_DON
#define SPNG_DON


/* fichiers inclus */
#include "xzsc.h"
#include "xzst.h"


/* definitions de constantes */

/* definitions des types globaux */

typedef struct {
                  XZSCT_NomMachine  NomMachine;
                  int               EtatMachine;
		  long	            Time;
                  void              *TestPrec;
		  void              *TestSuiv;
               } SGT_TEST_RES; 


#ifndef SPNG_DON_c

/* definition des variables globales */

extern XZSCT_NomSite vg_NomSite;
	/* Premier element de la liste des tests a effectuer */
extern SGT_TEST_RES *pm_PremTest; 
	/* Dernier element de la liste des tests a effectuer */
extern SGT_TEST_RES *pm_DernTest; 

/* definitions des fonction externes */


#endif


#endif

