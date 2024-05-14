/*E*/
/*Fichier : $Id: xzaf02_test.c,v 1.1 1995/01/17 12:06:15 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/01/17 12:06:15 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAT * FICHIER xzag02_test.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Fichier permetant de tester la fonction de xzag02.c
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	20/12/1994	: Creation
*                         a tester le cas nominal
------------------------------------------------------*/

/* fichiers inclus */

#include "xzaf.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzaf02_test.c,v 1.1 1995/01/17 12:06:15 torregrossa Exp $ XZAF02_test";

/* declaration de fonctions internes */
int Fonction ( XDY_Octet , XDY_Mot , XDY_Sens , XDY_Voie ,
		XDY_Entier , XDY_Evt , XDY_Act );

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  Tester la fonction XZAF02_Animer_Objet_Mobile
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
*	XZAF02_Animer_Objet_Mobile 	() 
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
	 * Rechercher le nombre de secondes
	 */
	vl_Status = XZSM_13ConversionHorodateSec("12/01/1995 18:00:00", 
						 (double*)&vl_NbSecondes);
	vl_Status = XZAF10_Rechercher_Evts_Actions(vl_NbSecondes);

	/*A
	** Tester avec une horodate
	*/
	vl_Status = XZAF02_Animer_Objet_Mobile(vl_NbSecondes, Fonction);
	printf("XZAF02_Animer_Objet_Mobile(%lf secondes) : code retourne  %d\n", 
		vl_NbSecondes, vl_Status);
	
	printf("\n\n**** Test 2 ****\n\n");
	/*A
	** Tester avec un pointeur de fonction null
	*/
	/*A
	 * Rechercher le nombre de secondes
	 */
	vl_Status = XZAF02_Animer_Objet_Mobile(vl_NbSecondes, NULL);
	printf("XZAF02_Animer_Objet_Mobile(%lf secondes, null) : code retourne  %d\n", 
		vl_NbSecondes, vl_Status);
	
	vl_Status = XZAG03_Deconnexion_Base();
	printf("XZAG03_Deconnexion_Base retourne %d\n",vl_Status);
	
	exit (XDC_OK);
}

int Fonction ( XDY_Octet va_type, XDY_Mot va_Portion, XDY_Sens va_Sens, XDY_Voie va_Voie,
		XDY_Entier va_Translation, XDY_Evt va_Evt, XDY_Act va_Action)
{
	printf("\nevenement : %d, %d\n", va_Evt.numero, va_Evt.cle);

	printf("action : %d, %d\n", va_Action.numero, va_Action.cle);

	printf("type=%d, portion=%d, sens=%d , voie=%d, translation=%d\n",
		va_type, va_Portion, va_Sens, va_Voie, va_Translation);

	return XDC_OK;
}
