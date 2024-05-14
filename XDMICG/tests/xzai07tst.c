/*E*/
/*Fichier : $Id: xzai07tst.c,v 1.1 1994/11/08 17:42:33 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/08 17:42:33 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAI * FICHIER xzai07tst.c
*               fait appel a xzag et xzai07
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  XZAG02_Connexion_Base permet de se connecter a la base PRC
*  du SQL_Srv de la machine passee en argument : On renseigne
*  une globale XZAGV_Connexion contenant l ID de connexion
*  Les codes de retour sont XDC_OK si tout s est bien passe 
*  sinon XDC_NOK voire le code d erreur Sybase
*  Fait appel a XZAI07_Ajouter_Autoroute
*  Se deconnecte du serveur SQL
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	08 Nov 1994	: Creation
*
------------------------------------------------------*/

/* fichiers inclus */

#include "xzai.h"


static char *version = "$Id: xzai07tst.c,v 1.1 1994/11/08 17:42:33 volcic Exp $ : xzai07tst.c" ;

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  < Test xzai07.c >
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
*    Test de xzai07.c
*
------------------------------------------------------*/
{
	/* Horodate = 06/04/94 12:12:12 */
	XDY_Evt 	Evt;
	XDY_Horodate 	Horodate;
	XDY_Booleen 	Confidentiel;

	int retcode;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : main");
	XZST_03EcritureTrace(XZSTC_INFO,"IN : main");
	retcode = XZAG02_Connexion_Base( "CI");
	printf("XZAG02_Connexion_Base retourne :%d\n",retcode);
 	
	printf("\n\n**** Test 1 ****\n\n");
	
	
	Evt.numero = 1;
	Evt.cle = 1;
	
	retcode =  XZAI07_Rechercher_Message_A_Diffuser  ( Evt,
							&Horodate,
							&Confidentiel );
				
	printf("\n\n*********\nXZAI07_Rechercher_Message_A_Diffuser retourne :%d\n******\n",retcode);

	printf("\n\nHorodate = %lf\nConfidentiel = %d\n\n",
				 Horodate,
				 Confidentiel);
	
	
	
	
	
	printf("\n\n**** Test 4 ****\n\n");
	
	
	Evt.numero = 2;
	Evt.cle = 2;
	
	retcode =  XZAI07_Rechercher_Message_A_Diffuser  ( Evt,
							&Horodate,
							&Confidentiel );
				
	printf("\n\n*********\nXZAI07_Rechercher_Message_A_Diffuser retourne :%d\n******\n",retcode);

	printf("\n\nHorodate = %lf\nConfidentiel = %d\n\n",
				 Horodate,
				 Confidentiel);
	
	
	
	
	
	
	printf("\n\n**** Test 3 ****\n\n");
	
	
	Evt.numero = 2;
	Evt.cle = 1;
	
	retcode =  XZAI07_Rechercher_Message_A_Diffuser  ( Evt,
							&Horodate,
							&Confidentiel );
				
	printf("\n\n*********\nXZAI07_Rechercher_Message_A_Diffuser retourne :%d\n******\n",retcode);

	printf("\n\nHorodate = %lf\nConfidentiel = %d\n\n",
				 Horodate,
				 Confidentiel);
	
	retcode = XZAG03_Deconnexion_Base();
	printf("XZAG03_Deconnexion_Base retourne %d\n",retcode);
}
