/*E*/
/*Fichier : $Id: xzag08_test.c,v 1.1 1994/12/22 19:45:01 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/12/22 19:45:01 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAT * FICHIER xzag08_test.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Fichier permetant de tester la fonction de xzag08.c
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	20/12/1994	: Creation
*                         a tester le cas nominal
------------------------------------------------------*/

/* fichiers inclus */

#include "xzag.h"
#include <stdlib.h>
#include <stdio.h>

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzag08_test.c,v 1.1 1994/12/22 19:45:01 torregrossa Exp $ : xzag08_test" ;

/* declaration de fonctions internes */
int xzag08_Sauvegarder_Base(XDY_NomSite , char *, XZAGT_Volume, char *, 
				char *, char *, XDY_Entier, XZAGT_Bande, char *);

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void  main(int argc, char *argv[])

/*
* ARGUMENTS EN ENTREE : Aucun
*
*
* CONDITION D'UTILISATION
*	Pour tester avec la variable $exec_migrazur 
*       d'environnement positionnee ou non
*	On teste :
*	xzag08_Sauvegarder_Base 		() 
*
* FONCTION 
*   
*
------------------------------------------------------*/
{
	char	vl_Buffer[100];
	int	vl_Status;
	

	/*A
	** Tester avec la variable non positionnee ou positionnee
	*/
	vl_Status = XZAG08_Sauvegarder_Base("DN", "/device", "Volume", 
				XZAGC_SVG_GLOB, "CFG",
				"fictrace", 2000, "bande", "pocddn");
	printf("Echec sauvegarde base si variable non positionnee : code retourne %d \n", vl_Status);
	
	vl_Status = XZAG08_Sauvegarder_Base("DN", "/dev/rmt/0mn", "Volume", "modeinc", "CFG",
				"fictrace", 2000, "bande", "pocddn");
	/* argument incorrect : mode */
	printf("Echec sauvegarde base : mode incorrect : code retourne %d \n", vl_Status);
	
	vl_Status = XZAG08_Sauvegarder_Base("DN", "/dev/rmt/0mn", "Volume", XZAGC_SVG_GLOB, "CFG",
				"fictrace", 2000, "bande", "pocddn");
	/* argument incorrect : volume */
	printf("Echec sauvegarde base : volume incorrect : code retourne %d \n", vl_Status);
	
	vl_Status = XZAG08_Sauvegarder_Base("DN", "/dev/rmt/0mn", "Volume", NULL, "CFG",
				"fictrace", 2000, "bande", "pocddn");
	/* argument incorrect : mode null */
	printf("Echec sauvegarde base : mode null : code retourne %d \n", vl_Status);
	
	vl_Status = XZAG08_Sauvegarder_Base("DN", "/dev/rmt/0mn", "Volume", XZAGC_SVG_GLOB, "CFG",
				"fictrace", 2000, "bande", "pocddn");
	/* script non trouve */
	printf("Echec sauvegarde base : script non trouve : code retourne %d \n", vl_Status);
	
	vl_Status = XZAG08_Sauvegarder_Base("DN", "/dev/rmt/0mn", "Volume", XZAGC_SVG_GLOB, "TCFG",
				"fictrace", 2000, "bande", "pocddn");
	/* argument incorrect : nom base */
	printf("Echec sauvegarde base : nom base incorrecte : code retourne %d \n", vl_Status);
	
	vl_Status = XZAG08_Sauvegarder_Base("DN", "/dev/rmt/0mn", "Volume", XZAGC_SVG_GLOB, "CFG",
				"fictrace", 2000, "bande", "pocddn");
	/* argument incorrect : sauvegarde impossible */
	printf("Echec sauvegarde base : sauvegarde impossible : code retourne %d \n", vl_Status);
	
	exit (XDC_OK);
}
