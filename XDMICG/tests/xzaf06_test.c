/*E*/
/*Fichier : $Id: xzaf06_test.c,v 1.1 1995/01/17 12:07:23 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/01/17 12:07:23 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAT * FICHIER xzag06_test.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Fichier permetant de tester la fonction de xzag06.c
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	16/01/95	: Creation
------------------------------------------------------*/

/* fichiers inclus */

#include "xzaf.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzaf06_test.c,v 1.1 1995/01/17 12:07:23 torregrossa Exp $ XZAF06_test";

/* declaration de fonctions internes */
int Fonction ( XDY_Evt , XDY_Mot , XDY_PR, XDY_Sens,
		XDY_Voie, XDY_Entier , XDY_Entier);

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  Tester la fonction XZAF06_Animer_Voies_Neutralisees
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
*	XZAF06_Animer_Voies_Neutralisees 	() 
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

	/*A
	 * initialisation de la table EVT_FRM
	 */
	/*A
	 * Rechercher le nombre de secondes
	 */
	vl_Status = XZSM_13ConversionHorodateSec("12/01/1995 18:00:00", 
						 (double*)&vl_NbSecondes);
	vl_Status = XZAF10_Rechercher_Evts_Actions(vl_NbSecondes);

	printf("\n\n**** Test 1 ****\n\n");

	/*A
	** Tester avec une horodate
	*/
	vl_Status = XZAF06_Animer_Voies_Neutralisees(Fonction);
	printf("XZAF06_Animer_Voies_Neutralisees(%lf secondes) : code retourne  %d\n", 
		vl_NbSecondes, vl_Status);
	
	printf("\n\n**** Test 2 ****\n\n");
	/*A
	** Tester avec un pointeur de fonction null
	*/
	vl_Status = XZAF06_Animer_Voies_Neutralisees(NULL);
	printf("XZAF06_Animer_Voies_Neutralisees(%lf secondes, null) : code retourne  %d\n", 
		vl_NbSecondes, vl_Status);
	
	vl_Status = XZAG03_Deconnexion_Base();
	printf("XZAG03_Deconnexion_Base retourne %d\n",vl_Status);
	
	exit (XDC_OK);
}

int Fonction ( XDY_Evt va_Evt, XDY_Mot va_Portion,  XDY_PR va_PR, XDY_Sens va_Sens,
		XDY_Voie va_Voie, XDY_Entier va_Translation, XDY_Entier va_Extension)
{
	printf("evenement : %d, %d", va_Evt.numero, va_Evt.cle);

	printf("portion=%d, sens=%d, translation=%d, extension=%d, PR=%d\n",
		va_Portion, va_Sens, va_Translation, va_Extension, va_PR);

	return XDC_OK;
}
