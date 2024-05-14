/*E*/
/*Fichier : $Id: xzaf03_test.c,v 1.1 1995/01/17 12:06:24 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/01/17 12:06:24 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAT * FICHIER xzag03_test.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Fichier permetant de tester la fonction de xzag03.c
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

static char *version = "$Id: xzaf03_test.c,v 1.1 1995/01/17 12:06:24 torregrossa Exp $ XZAF03_test";

/* declaration de fonctions internes */
int Fonction ( XDY_Evt , XDY_Booleen , XDY_Mot , 
		XDY_Entier , XDY_Voie , XDY_Voie ,
		XDY_Voie , XDY_Voie , XDY_Voie , XDY_Voie ,
		XDY_Voie , XDY_Voie , XDY_Voie , XDY_Voie ,
		XDY_Voie , XDY_Voie , XDY_Voie , XDY_Voie ,
		XDY_Voie , XDY_Voie , XDY_Voie , XDY_Voie ,
		XDY_Voie , XDY_Voie );

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  Tester la fonction XZAF03_Animer_Neut_Voies
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
*	XZAF03_Animer_Neut_Voies 	() 
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
	vl_Status = XZAF03_Animer_Neut_Voies(&Fonction);
	printf("XZAF03_Animer_Neut_Voies(%lf secondes) : code retourne  %d\n", 
		vl_NbSecondes, vl_Status);
	
	/*A
	** Tester avec un pointeur de fonction null
	*/
	vl_Status = XZAF03_Animer_Neut_Voies(NULL);
	printf("XZAF03_Animer_Neut_Voies(%lf secondes, null) : code retourne  %d\n", 
		vl_NbSecondes, vl_Status);
	
	vl_Status = XZAG03_Deconnexion_Base();
	printf("XZAG03_Deconnexion_Base retourne %d\n",vl_Status);
	
	exit (XDC_OK);
}

int Fonction ( XDY_Evt va_Evt, XDY_Booleen va_DebutFin, XDY_Mot va_Portion, 
		XDY_Entier va_Translation, XDY_Voie va_Voie1, XDY_Voie va_NVoie1,
		XDY_Voie va_Voie2, XDY_Voie va_NVoie2, XDY_Voie va_Voie3, XDY_Voie va_NVoie3,
		XDY_Voie va_Voie4, XDY_Voie va_NVoie4, XDY_Voie va_Voie5, XDY_Voie va_NVoie5,
		XDY_Voie va_Voie6, XDY_Voie va_NVoie6, XDY_Voie va_Voie7, XDY_Voie va_NVoie7,
		XDY_Voie va_Voie8, XDY_Voie va_NVoie8, XDY_Voie va_Voie9, XDY_Voie va_NVoie9,
		XDY_Voie va_Voie10, XDY_Voie va_NVoie10)
{
	printf("evenement : %d, %d", va_Evt.numero, va_Evt.cle);

	printf("DebutFin=%d, portion=%d, translation=%d\n",
		va_DebutFin, va_Portion, va_Translation);

	printf("voies : 1: %d, %d; 2: %d, %d; 3: %d, %d; 4:%d, %d; 5: %d, %d;\n 6:%d, %d; 7: %d, %d; 8: %d, %d; 9: %d, %d; 10:%d, %d", 
		va_Voie1, va_NVoie1, va_Voie2, va_NVoie2, va_Voie3, va_NVoie3, 
		va_Voie4, va_NVoie4, va_Voie5, va_NVoie5, va_Voie6, va_NVoie6, 
		va_Voie7, va_NVoie7, va_Voie8, va_NVoie8, va_Voie9, va_NVoie9,
		va_Voie10, va_NVoie10);


	return XDC_OK;
}
