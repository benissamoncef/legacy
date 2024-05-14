/*E*/
/*  Fichier : 	$Id: xzat05tst.c,v 1.1 1994/11/08 10:10:42 volcic Exp $  Release : $Revision: 1.1 $ Date : $Date: 1994/11/08 10:10:42 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAT * FICHIER xzattst05.c
*               fait appel a xzag et xzat05
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  XZAG02_Connexion_Base permet de se connecter a la base PRC
*  du SQL_Srv de la machine passee en argument : On renseigne
*  une globale XZAGV_Connexion contenant l ID de connexion
*  Les codes de retour sont XDC_OK si tout s est bien passe 
*  sinon XDC_NOK voire le code d erreur Sybase
*  Fait appel a XZAT05_Ajouter_Autoroute
*  Se deconnecte du serveur SQL
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	07 Nov 1994	: Creation
*
------------------------------------------------------*/

/* fichiers inclus */

#include "xzat.h"


static char *version = "$Id: xzat05tst.c,v 1.1 1994/11/08 10:10:42 volcic Exp $ : xzat05tst.c" ;

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  < Test xzat05.c >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void  main()

/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
*
* FONCTION 
*    Test de xzat05.c
*
------------------------------------------------------*/
{
	/* Horodate = 06/04/94 12:12:12 */
	XDY_Horodate 	Horodate = 765634332;
	XDY_Mot		va_NumEqt_in = 2;
	XDY_Octet 	va_DispoPMV_out;
	XDY_Octet 	va_DispoPicto_out;
	XDY_Octet 	va_TypePMV_out;
	XDY_Ligne_PMV 	va_Ligne1_out;
	XDY_Ligne_PMV 	va_Ligne2_out;
	XDY_Ligne_PMV 	va_Ligne3_out;
	XDY_Ligne_PMV	va_Alternat1_out;
	XDY_Ligne_PMV 	va_Alternat2_out;
	XDY_Ligne_PMV 	va_Alternat3_out;
	XDY_Booleen 	va_Clignotement1_out;
	XDY_Booleen 	va_Clignotement2_out;
	XDY_Booleen 	va_Clignotement3_out;
	XDY_Booleen 	va_Flash_out;
	char 		va_TypePicto_out[XDC_LGR_NOM_PICTO];
	char 		va_TextePicto_out[XDC_LGR_TXT_PICTO];
	XDY_Octet 	va_NumTypePicto_out;
	XDY_Booleen 	va_ClignotementPicto_out;
	XDY_Entier 	va_NumEvt_out;
	XDY_Octet 	va_CleEvt_out;
	XDY_Octet 	va_Priorite_out;
	XDY_Entier 	va_DistanceEvt_out;
	
	int retcode;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : main");
	XZST_03EcritureTrace(XZSTC_INFO,"IN : main");
	retcode = XZAG02_Connexion_Base( "CI");
	printf("XZAG02_Connexion_Base retourne :%d\n",retcode);
 	
	printf("\n\n**** Test 1 ****\n\n");
	
	retcode =  XZAT05_Utilisation_PMV  ( 	Horodate,
				va_NumEqt_in,
				&va_DispoPMV_out,
				&va_DispoPicto_out,
				&va_TypePMV_out,
				&va_Ligne1_out,
				&va_Ligne2_out,
				&va_Ligne3_out,
				&va_Alternat1_out,
				&va_Alternat2_out,
				&va_Alternat3_out,
				&va_Clignotement1_out,
				&va_Clignotement2_out,
				&va_Clignotement3_out,
				&va_Flash_out,
				va_TypePicto_out,
				va_TextePicto_out,
				&va_NumTypePicto_out,
				&va_ClignotementPicto_out,
				&va_NumEvt_out,
				&va_CleEvt_out,
				&va_Priorite_out,
				&va_DistanceEvt_out );
				
	printf("\n\n*********\nXZAT05_Utilisation_PMV retourne :%d\n******\n",retcode);

	printf("\n\nDispoPMV = %d\nDispoPicto = %d\nTypePMV = %d\nLigne1 = %s\nLigne2 = %s\nLigne3 = %s\nAlternat1 = %s\nAlternat2 = %s\nAlternat3 = %s\nCligno1 = %d\nCligno2 = %d\nCligno3 = %d\nFlash = %d\nTypePicto = %s\nTextePicto = %s\nNumTypePicto = %d\nClignoPicto = %d\nNumEvt = %d\nCleEvt = %d\nPriorite = %d\nDistanceEvt = %d\n",
				va_DispoPMV_out,
				va_DispoPicto_out,
				va_TypePMV_out,
				va_Ligne1_out,
				va_Ligne2_out,
				va_Ligne3_out,
				va_Alternat1_out,
				va_Alternat2_out,
				va_Alternat3_out,
				va_Clignotement1_out,
				va_Clignotement2_out,
				va_Clignotement3_out,
				va_Flash_out,
				va_TypePicto_out,
				va_TextePicto_out,
				va_NumTypePicto_out,
				va_ClignotementPicto_out,
				va_NumEvt_out,
				va_CleEvt_out,
				va_Priorite_out,
				va_DistanceEvt_out );
	
	retcode = XZAG03_Deconnexion_Base();
	printf("XZAG03_Deconnexion_Base retourne %d\n",retcode);
}
