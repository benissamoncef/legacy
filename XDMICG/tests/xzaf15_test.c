/*E*/
/*Fichier : $Id: xzaf15_test.c,v 1.1 1995/01/17 12:06:38 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/01/17 12:06:38 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAT * FICHIER xzaf15_test.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Fichier permetant de tester la fonction de xzaf15.c
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	17/01/1995	: Creation
------------------------------------------------------*/

/* fichiers inclus */

#include "xzaf.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzaf15_test.c,v 1.1 1995/01/17 12:06:38 torregrossa Exp $ XZAF15_Animer_Formation";

/* declaration de fonctions internes */
int FonctionMobile ( XDY_Octet , XDY_Mot , XDY_Sens , XDY_Voie ,
		XDY_Entier , XDY_Evt , XDY_Act );

int FonctionAffectVoies ( XDY_Evt , XDY_Booleen , XDY_Mot , 
		XDY_Entier , XDY_Voie , XDY_Voie ,
		XDY_Voie , XDY_Voie , XDY_Voie , XDY_Voie ,
		XDY_Voie , XDY_Voie , XDY_Voie , XDY_Voie ,
		XDY_Voie , XDY_Voie , XDY_Voie , XDY_Voie ,
		XDY_Voie , XDY_Voie , XDY_Voie , XDY_Voie ,
		XDY_Voie , XDY_Voie );

int FonctionBouchon ( XDY_Evt , XDY_Mot , XDY_Sens,
		XDY_Entier , XDY_Entier, XDY_Octet);

int FonctionVoiesNeut ( XDY_Evt , XDY_Mot , XDY_PR, XDY_Sens,
		XDY_Voie, XDY_Entier , XDY_Entier);

int FonctionRadtInverse ( XDY_Evt , XDY_Mot , XDY_PR, XDY_Sens,
		XDY_Voie, XDY_Entier , XDY_Entier,
		XDY_Mot, XDY_Sens);

int FonctionBouchonInverse ( XDY_Evt , XDY_Evt, XDY_Mot , XDY_PR, XDY_Sens,
		XDY_Voie, XDY_Entier , XDY_Entier,
		XDY_Octet);

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  Tester la fonction XZAF15_Animer_Formation
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
*	XZAF15_Animer_Formation 		() 
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

	/*A
	** Tester avec les arguments corrects
	*/
	vl_Status = XZAF15_Animer_Formation(vl_NbSecondes, FonctionMobile, FonctionBouchon, 
					FonctionRadtInverse, FonctionBouchonInverse,
					FonctionVoiesNeut, FonctionAffectVoies);
	printf("XZAF15_Animer_Formation(%lf secondes) : code retourne %d \n", 
		vl_NbSecondes, vl_Status);

	printf("\n\n**** Test 2 ****\n\n");
	/*A
	** Tester avec les arguments corrects
	*/
	vl_Status = XZAF15_Animer_Formation(vl_NbSecondes, NULL, NULL, NULL, NULL, NULL, NULL);
	printf("XZAF15_Animer_Formation(%lf secondes, NULL, NULL, NULL, NULL, NULL, NULL) : code retourne %d \n", 
		vl_NbSecondes, vl_Status);
	
	vl_Status = XZAG03_Deconnexion_Base();
	printf("XZAG03_Deconnexion_Base retourne %d\n",vl_Status);
	
	exit (XDC_OK);
}

int FonctionMobile ( XDY_Octet va_type, XDY_Mot va_Portion, XDY_Sens va_Sens, XDY_Voie va_Voie,
		XDY_Entier va_Translation, XDY_Evt va_Evt, XDY_Act va_Action)
{
	printf("\nevenement : %d, %d\n", va_Evt.numero, va_Evt.cle);

	printf("action : %d, %d\n", va_Action.numero, va_Action.cle);

	printf("type=%d, portion=%d, sens=%d , voie=%d, translation=%d\n",
		va_type, va_Portion, va_Sens, va_Voie, va_Translation);

	return XDC_OK;
}

int FonctionBouchon ( XDY_Evt va_Evt, XDY_Mot va_Portion,  XDY_Sens va_Sens,
		XDY_Entier va_Translation, XDY_Entier va_Extension,
		XDY_Octet va_Niveau)
{
	printf("evenement : %d, %d", va_Evt.numero, va_Evt.cle);

	printf("portion=%d, sens=%d, translation=%d, extension=%d, niveau=%d\n",
		va_Portion, va_Sens, va_Translation, va_Extension, va_Niveau);

	return XDC_OK;
}

int FonctionAffectVoies ( XDY_Evt va_Evt, XDY_Booleen va_DebutFin, XDY_Mot va_Portion, 
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

int FonctionVoiesNeut ( XDY_Evt va_Evt, XDY_Mot va_Portion,  XDY_PR va_PR, XDY_Sens va_Sens,
		XDY_Voie va_Voie, XDY_Entier va_Translation, XDY_Entier va_Extension)
{
	printf("evenement : %d, %d", va_Evt.numero, va_Evt.cle);

	printf("portion=%d, sens=%d, translation=%d, extension=%d, PR=%d\n",
		va_Portion, va_Sens, va_Translation, va_Extension, va_PR);

	return XDC_OK;
}

int FonctionRadtInverse ( XDY_Evt va_Evt, XDY_Mot va_Portion,  XDY_PR va_PR, XDY_Sens va_Sens,
		XDY_Voie va_Voie, XDY_Entier va_Translation, XDY_Entier va_Extension,
		XDY_Mot va_Station, XDY_Sens va_SensStation)
{
	printf("evenement : %d, %d", va_Evt.numero, va_Evt.cle);

	printf("portion=%d, sens=%d, translation=%d, extension=%d, PR=%d, station=%d, sensstation=%d\n",
		va_Portion, va_Sens, va_Translation, va_Extension, va_PR, va_Station, va_SensStation);

	return XDC_OK;
}

int FonctionBouchonInverse ( XDY_Evt va_EvtBasc, XDY_Evt va_EvtBou, 
		XDY_Mot va_Portion,  XDY_PR va_PR, XDY_Sens va_Sens,
		XDY_Voie va_Voie, XDY_Entier va_Translation, XDY_Entier va_Extension,
		XDY_Octet va_Niveau)
{
	printf("evenementBasc : %d, %d", va_EvtBasc.numero, va_EvtBasc.cle);
	printf("evenementBou : %d, %d", va_EvtBou.numero, va_EvtBou.cle);

	printf("portion=%d, sens=%d, translation=%d, extension=%d, PR=%d, niveau=%d\n",
		va_Portion, va_Sens, va_Translation, va_Extension, va_PR, va_Niveau);

	return XDC_OK;
}
