/*E*/
/*6  Fichier : @(#)con_p06_decon.c	1.1      Release : 1.1        Date : 08/24/94
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAP * FICHIER con_p06_decon.c
*               fait appel a xzag et xzap06
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  XZAG03_Connexion_Base permet de se connecter a la base PRC
*  du SQL_Srv de la machine passee en argument : On renseigne
*  une globale XZAGV_Connexion contenant l ID de connexion
*  Les codes de retour sont XDC_OK si tout s est bien passe 
*  sinon XDC_NOK voire le code d erreur Sybase
*  Fait appel a XZAP06_Ajout_Prop_Envoi_TFM
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
#include "xzap06.h"
#include "xzst.h"
#include "xzsc.h"


static char *version = "@(#)con_p06_decon.c	1.1 08/24/94 : XZAP" ;

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  < Appel la procedure stockee XZAP06SP >
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
	XDY_Horodate	Horodate;
	XDY_Nom 	Libelle;
	XDY_Phrase	Remarque;


	int retcode;

/*	retcode = XZSC_InitTrace();*/

	/* Horodate_In.days = 34574;
	   Horodate_In.minutes = 1063; */
	
	Horodate = 778268580;   
	
        /* Libelle_In.len=25; */
        
        strcpy(Libelle,"Libelle Envoi_TFM");
        
        strcpy(Remarque,"Remarque Envoi_TFM");
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : main");
	XZST_03EcritureTrace(XZSTC_INFO,"IN : main");
	retcode = XZAG02_Connexion_Base( "SDCI1");
	printf("XZAG02_Connexion_Base retourne :%d\n",retcode);

	retcode =  XZAP06_Ajout_Prop_Envoi_TFM( NumPA_In,Ordre_In,District_In,Horodate,Libelle,Remarque);
	
	
	printf("\n\n*********\nXZAP06_Ajout_Prop_Envoi_TFM retourne :%d\n******\n",retcode);


	retcode = XZAG03_Deconnexion_Base();
	printf("XZAG03_Deconnexion_Base retourne %d\n",retcode);
}
