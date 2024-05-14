/*E*/
/*Fichier : $Id: xzaf16_test.c,v 1.1 1995/01/23 19:28:00 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/01/23 19:28:00 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAT * FICHIER xzaf16_test.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Fichier permetant de tester la fonction de xzaf16.c
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

static char *version = "@(#)xzaf16_test.c	1.1 01/17/95 XZAF16_Lire_Niveaux_Station";

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
*  Tester la fonction XZAF16_Lire_Niveaux_Station
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
*	XZAF16_Lire_Niveaux_Station 		() 
*
* FONCTION 
*   
*
------------------------------------------------------*/
{
	int		vl_Status;
	XDY_Horodate	vl_NbSecondes;
	XDY_Sens	vl_Sens1, vl_Sens2;
	XDY_Octet	vl_Niveau1, vl_Niveau2;
	

	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : main");
	XZST_03EcritureTrace(XZSTC_INFO,"IN : main");
	vl_Status = XZAG02_Connexion_Base( "CI");
	printf("XZAG02_Connexion_Base retourne :%d\n", vl_Status);


	printf("\n\n**** Test 1 ****\n\n");

	/*A
	 * Rechercher le nombre de secondes
	 */
	vl_Status = XZSM_13ConversionHorodateSec("18/01/1995 18:00:00", 
						 (double*)&vl_NbSecondes);

	/*A
	** Tester avec les arguments corrects
	*/
	vl_Status = XZAF16_Lire_Niveaux_Station(14, vl_NbSecondes, &vl_Sens1, &vl_Niveau1,
						&vl_Sens2, &vl_Niveau2);
	printf("OUT : XZAF16_Lire_Niveaux_Station : sens1=%d niveau1=%d sens2=%d niveau2=%d\n",
		vl_Sens1, vl_Niveau1, vl_Sens2, vl_Niveau2);
	printf("XZAF16_Lire_Niveaux_Station(14, %lf secondes) : code retourne %d \n", 
		vl_NbSecondes, vl_Status);

	printf("\n\n**** Test 2 ****\n\n");
	/*A
	** Tester avec les arguments corrects
	*/
	vl_Status = XZAF16_Lire_Niveaux_Station(14, vl_NbSecondes, NULL, NULL, NULL, NULL);
	printf("XZAF16_Lire_Niveaux_Station(14, %lf secondes, NULL, NULL, NULL, NULL) : code retourne %d\n \n", 
		vl_NbSecondes, vl_Status);
	
	vl_Status = XZAG03_Deconnexion_Base();
	printf("XZAG03_Deconnexion_Base retourne %d\n",vl_Status);
	
	exit (XDC_OK);
}
