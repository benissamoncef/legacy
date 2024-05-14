/*X  Fichier : @(#)con_p02_decon.c	1.1      Release : 1.1        Date : 08/24/94
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAP * FICHIER con_p02_decon.c
*               fait appel a xzag et xzap02
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  XZAG02_Connexion_Base permet de se connecter a la base PRC
*  du SQL_Srv de la machine passee en argument : On renseigne
*  une globale XZAGV_Connexion contenant l ID de connexion
*  Les codes de retour sont XDC_OK si tout s est bien passe 
*  sinon XDC_NOK voire le code d erreur Sybase
*  Fait appel a XZAP02_Ajout_Prop_PMV
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
#include "xzap02.h"
#include "xzst.h"
#include "xzsc.h"


static char *version = "@(#)con_p02_decon.c	1.1 08/24/94 : XZAP" ;

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  < Appel la procedure stockee XZAP02SP >
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
	XDY_Phrase	Remarque_In;
	XDY_Octet 	PrioriteEvt_In=6;
	XDY_Eqt 	NoEqt_In=5;
	XDY_Commentaire	Explication_In;
	XDY_Ligne_PMV	Texte1PMV_In;
	XDY_Ligne_PMV	Texte2PMV_In;
	XDY_Ligne_PMV	Texte3PMV_In;
	XDY_Ligne_PMV	Texte4PMV_In;
	XDY_Ligne_PMV	Texte5PMV_In;
	XDY_Ligne_PMV	Texte6PMV_In;
	XDY_Nom		Libelle_In;
	XDY_TxtPicto	TextePicto_In;
	XDY_NomPicto	NomPicto_In;
	XDY_Booleen	Realisable_In=1;

	int retcode;

/*	retcode = XZSC_InitTrace();*/


        strcpy(Remarque_In,"Remarque");

        strcpy(Explication_In,"Ceci est un PMV");

        strcpy(Texte1PMV_In,"Tout est OK1");

        strcpy(Texte2PMV_In ,"Tout est OK2");

        strcpy(Texte3PMV_In ,"Tout est OK3");
  
        strcpy(Texte4PMV_In ,"Tout est OK4");

        strcpy(Texte5PMV_In ,"Tout est OK5");

        strcpy(Texte6PMV_In ,"Tout est OK6");

        strcpy(Libelle_In ,"Libelle");

        strcpy(TextePicto_In ,"TxtPic");

        strcpy(NomPicto_In ,"Nom");
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : main");
	XZST_03EcritureTrace(XZSTC_INFO,"IN : main");
	retcode = XZAG02_Connexion_Base( "SDCI1");
	printf("XZAG02_Connexion_Base retourne :%d\n",retcode);

	retcode =  XZAP02_Ajout_Prop_PMV( NumPA_In,Ordre_In,District_In,Remarque_In,PrioriteEvt_In,NoEqt_In,Explication_In,Texte1PMV_In,Texte2PMV_In,Texte3PMV_In,Texte4PMV_In,Texte5PMV_In,Texte6PMV_In,Libelle_In,TextePicto_In,NomPicto_In,Realisable_In);
	
	
	printf("\n\n*********\nXZAP02_Ajout_Prop_PMV retourne :%d\n******\n",retcode);


	retcode = XZAG03_Deconnexion_Base();
	printf("XZAG03_Deconnexion_Base retourne %d\n",retcode);
}
