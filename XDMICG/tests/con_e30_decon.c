/*E*/
/*  Fichier : $Id: con_e30_decon.c,v 1.1 1994/09/15 17:37:38 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/09/15 17:37:38 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAE * FICHIER con_e30_decon.c
*               fait appel a xzag et xzae30
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  XZAG02_Connexion_Base permet de se connecter a la base PRC
*  du SQL_Srv de la machine passee en argument : On renseigne
*  une globale XZAGV_Connexion contenant l ID de connexion
*  Les codes de retour sont XDC_OK si tout s est bien passe 
*  sinon XDC_NOK voire le code d erreur Sybase
*  Fait appel a XZAE30_Decrire_Pb_Concessionnaire_PA
*  Se deconnecte du serveur SQL
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	09 Sep 1994	: Creation
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
#include "xzae.h"
#include "xzae30.h"
#include "xzst.h"
#include "xzsc.h"


static char *version = "@(#)con_e30_decon.c	1.1 08/30/94 : XZAE" ;

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
	
	XDY_Horodate 		va_Horodate=765634332;
	XDY_Evt 		va_NumEvt;
	XZAET_SousConcess 	va_SousConcess;
	

	int retcode;

/*	retcode = XZSC_InitTrace();*/
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : main");
	XZST_03EcritureTrace(XZSTC_INFO,"IN : main");
	retcode = XZAG02_Connexion_Base( "SDCI1");
	printf("XZAG02_Connexion_Base retourne :%d\n",retcode);
 	
 	va_NumEvt.numero =1;
 	va_NumEvt.cle = 1;
	
	retcode =  XZAE30_Decrire_Pb_Concessionnaire_PA( va_Horodate,va_NumEvt, &va_SousConcess);
	
	printf("\n\n*********\nXZAE30_Decrire_Pb_Concessionnaire_PA retourne :%d\n******\n",retcode);

	printf("\nva_SousConcess->Nom = %s\nva_SousConcess->Ferme = %d\nva_SousConcess->Penurie = %d\nva_SousConcess->Carburant = %d\n",
	va_SousConcess.Nom,
	va_SousConcess.Ferme,
	va_SousConcess.Penurie,
	va_SousConcess.Carburant );

	retcode = XZAG03_Deconnexion_Base();
	printf("XZAG03_Deconnexion_Base retourne %d\n",retcode);
}
