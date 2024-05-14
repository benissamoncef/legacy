/*E*/
/*Fichier : $Id: xzaf14_test.c,v 1.1 1995/01/17 12:06:54 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/01/17 12:06:54 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAT * FICHIER xzag14_test.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Fichier permetant de tester la fonction de xzag14.c
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	16/01/1995	: Creation
------------------------------------------------------*/

/* fichiers inclus */

#include "xzaf.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzaf14_test.c,v 1.1 1995/01/17 12:06:54 torregrossa Exp $ XZAF14_Supprimer_Evts_Actions";

/* declaration de fonctions internes */
int XZAF10_Rechercher_Evts_Actions(XDY_Horodate);
int XZAF14_Supprimer_Evts_Actions();

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  Tester la fonction XZAF14_Supprimer_Evts_Actions
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void  main(int argc, char *argv[])

/*
* ARGUMENTS EN ENTREE : Aucun
*
*
* CONDITION D'UTILISATION
*	On teste :
*	XZAF14_Supprimer_Evts_Actions 		() 
*
* FONCTION 
*   
*
------------------------------------------------------*/
{
	int		vl_Status;
	XDY_Horodate	vl_NbSecondes;
	

	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : main");
	XZST_03EcritureTrace(XZSTC_INFO,"IN : main");
	vl_Status = XZAG02_Connexion_Base( "CI");
	printf("XZAG02_Connexion_Base retourne :%d\n", vl_Status);


	printf("\n\n**** Test 1 ****\n\n");

	/*A
	 * Supprimer le nombre de secondes
	 */
	vl_Status = XZSM_13ConversionHorodateSec("12/01/1995 18:00:00", 
						 (double*)&vl_NbSecondes);
	vl_Status = XZAF10_Rechercher_Evts_Actions(vl_NbSecondes);

	/*A
	** Tester avec la variable non positionnee ou positionnee
	*/
	vl_Status = XZAF14_Supprimer_Evts_Actions();
	printf("XZAF14_Supprimer_Evts_Actions() : code retourne %d \n", 
		vl_Status);
	
	vl_Status = XZAG03_Deconnexion_Base();
	printf("XZAG03_Deconnexion_Base retourne %d\n",vl_Status);
	
	exit (XDC_OK);
}
