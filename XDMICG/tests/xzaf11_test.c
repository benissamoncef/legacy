/*E*/
/*Fichier : $Id: xzaf11_test.c,v 1.1 1995/01/17 12:07:48 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/01/17 12:07:48 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAT * FICHIER xzag11_test.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Fichier permetant de tester la fonction de xzag11.c
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

static char *version = "$Id: xzaf11_test.c,v 1.1 1995/01/17 12:07:48 torregrossa Exp $ XZAF11_test";

/* declaration de fonctions internes */
int Fonction ( XDY_Evt , XDY_Evt, XDY_Mot , XDY_PR, XDY_Sens,
		XDY_Voie, XDY_Entier , XDY_Entier,
		XDY_Octet);

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  Tester la fonction XZAF11_Animer_Bouchons_Inverses
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
*	XZAF11_Animer_Bouchons_Inverses 	() 
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
	vl_Status = XZAF11_Animer_Bouchons_Inverses(Fonction);
	printf("XZAF11_Animer_Bouchons_Inverses(%lf secondes) : code retourne  %d\n", 
		vl_NbSecondes, vl_Status);
	
	printf("\n\n**** Test 2 ****\n\n");
	/*A
	** Tester avec un pointeur de fonction null
	*/
	vl_Status = XZAF11_Animer_Bouchons_Inverses(NULL);
	printf("XZAF11_Animer_Bouchons_Inverses(%lf secondes, null) : code retourne  %d\n", 
		vl_NbSecondes, vl_Status);
	
	vl_Status = XZAG03_Deconnexion_Base();
	printf("XZAG03_Deconnexion_Base retourne %d\n",vl_Status);
	
	exit (XDC_OK);
}

int Fonction ( XDY_Evt va_EvtBasc, XDY_Evt va_EvtBou, 
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
