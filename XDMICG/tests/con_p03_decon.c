/*X  Fichier : @(#)con_p03_decon.c	1.1      Release : 1.1        Date : 08/24/94
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAP * FICHIER con_p03_decon.c
*               fait appel a xzag et xzap03
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  XZAG02_Connexion_Base permet de se connecter a la base PRC
*  du SQL_Srv de la machine passee en argument : On renseigne
*  une globale XZAGV_Connexion contenant l ID de connexion
*  Les codes de retour sont XDC_OK si tout s est bien passe 
*  sinon XDC_NOK voire le code d erreur Sybase
*  Fait appel a XZAP03_Ajout_Prop_NAV
*  Se deconnecte du serveur SQL
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	31 Aug 1994	: Creation
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
#include "xzap03.h"
#include "xzst.h"
#include "xzsc.h"


static char *version = "@(#)con_p03_decon.c	1.1 08/24/94 : XZAP" ;

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  < Appel la procedure stockee XZAP03SP >
*
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
	XDY_Mot 	NumPA_In=1;
	XDY_Mot 	Ordre_In=2;
	XDY_District	District_In=3;
	XDY_Phrase	Remarque;
	XDY_Octet 	PrioriteEvt_In=6;
	XDY_Eqt 	NoEqt_In=5;
	XDY_Commentaire	Explication;
	XDY_Nom		Libelle;
	XDY_Scenario	Scenario;
	XDY_Booleen	Realisable_In=1;


	int retcode;

/*	retcode = XZSC_InitTrace();*/


        strcpy(Remarque ,"Remarque NAV");

        strcpy(Explication ,"Ceci est une NAV");

        strcpy(Libelle ,"Libelle NAV");

        strcpy(Scenario ,"Scen");
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : main");
	XZST_03EcritureTrace(XZSTC_INFO,"IN : main");
	retcode = XZAG02_Connexion_Base( "SDCI1");
	printf("XZAG02_Connexion_Base retourne :%d\n",retcode);

	retcode =  XZAP03_Ajout_Prop_NAV( NumPA_In,Ordre_In,District_In,Remarque,PrioriteEvt_In,NoEqt_In,Explication,Libelle,Scenario,Realisable_In);
	
	
	printf("\n\n*********\nXZAP03_Ajout_Prop_NAV retourne :%d\n******\n",retcode);


	retcode = XZAG03_Deconnexion_Base();
	printf("XZAG03_Deconnexion_Base retourne %d\n",retcode);
}
