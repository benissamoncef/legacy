/*X  Fichier : @(#)con_sp015_decon.c	1.1      Release : 1.1        Date : 08/24/94
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAG * FICHIER con_sp015_decon.c
*               fait appel a xzag et xzao15
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  XZAG02_Connexion_Base permet de se connecter a la base PRC
*  du SQL_Srv de la machine passee en argument : On renseigne
*  une globale XZAGV_Connexion contenant l ID de connexion
*  Les codes de retour sont XDC_OK si tout s est bien passe 
*  sinon XDC_NOK voire le code d erreur Sybase
*  Fait appel a XZAO15_PMV_amont
*  Se deconnecte du serveur SQL
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	16 Aug 1994	: Creation
------------------------------------------------------*/

/* fichiers inclus */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include "ctpublic.h"
#include "cstypes.h"
#include "xdc.h"
#include "xdy.h"
#include "xzag.h"
#include "xzao15.h"
#include "xzst.h"
#include "xzsc.h"


static char *version = "@(#)con_sp015_decon.c	1.1 08/24/94 : XZAG" ;

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
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
*    Ouvre une connexion a la base PRC sur le serveur de donnees du Site
*    Initialise la variable globale au Module XZAGV_Connexion a la connexion 
*    Sybase si tout s est bien passe sinon elle est mise a NULL.
*
------------------------------------------------------*/
{
	XDY_Autoroute  	Autoroute_In=1;
	XDY_PR		PR_In=180000;
	XDY_Sens	Sens_In=1;
	XDY_Octet	Numero_In=1;
	
	XDY_Eqt 	NumPMV_Out;
	XDY_District 	District_Out;
	XDY_Autoroute 	Autoroute_Out;
	XDY_PR 		PR_Out;
	XDY_Sens 	Sens_Out;
	XDY_Entier 	Distance_Out;
	
	XDY_Horodate	Nb_Second;

	int vl_retcode;

/*	vl_retcode = XZSC_InitTrace();*/
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : main");
	XZST_03EcritureTrace(XZSTC_INFO,"IN : main");
	
	vl_retcode = XZAG02_Connexion_Base( "CI");
	printf("XZAG02_Connexion_Base retourne :%d\n",vl_retcode);

	vl_retcode =  XZAO15_PMV_amont( Autoroute_In, 
					PR_In, 
					Sens_In, 
					Numero_In, 
					&NumPMV_Out, 
					&District_Out, 
					&Autoroute_Out, 
					&PR_Out, 
					&Sens_Out, 
					&Distance_Out);

	printf("\n\n*********\nXZAO15_PMV_amont retourne :%d\n******\n",vl_retcode);
	
	printf("\nNumPMV_Out = %d\nDistrict_Out = %d\nAutoroute_Out = %d\nPR_Out = %d\nSens_Out = %d\nDistance_Out = %d\n ",NumPMV_Out,District_Out,Autoroute_Out,PR_Out,Sens_Out,Distance_Out);

	vl_retcode = XZAG03_Deconnexion_Base();
	printf("XZAG03_Deconnexion_Base retourne %d\n",vl_retcode);
}
