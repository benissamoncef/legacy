/*E*/
/*  Fichier : $Id: con_e23_decon.c,v 1.1 1994/09/15 17:37:19 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/09/15 17:37:19 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAE * FICHIER con_e23_decon.c
*               fait appel a xzag et xzae23
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  XZAG02_Connexion_Base permet de se connecter a la base PRC
*  du SQL_Srv de la machine passee en argument : On renseigne
*  une globale XZAGV_Connexion contenant l ID de connexion
*  Les codes de retour sont XDC_OK si tout s est bien passe 
*  sinon XDC_NOK voire le code d erreur Sybase
*  Fait appel a XZAE23_Decrire_Bouchon_PA
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
#include "xzae23.h"
#include "xzst.h"
#include "xzsc.h"


static char *version = "@(#)con_e23_decon.c	1.1 08/23/94 : XZAE" ;

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
	/* XDY_Horodate 	Horodate=22767767; */
	XDY_Horodate 	va_Horodate=765634332;
	XDY_Evt 	va_NumEvt;
	 
	
        XDY_PR		va_PRQueue_out=0;
	XDY_Octet	va_Type_out=0;
	XDY_Entier	va_Longueur_out=0;
	XDY_Evt		va_NumEvtQueue;
	
	int retcode;

/*	retcode = XZSC_InitTrace();*/
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : main");
	XZST_03EcritureTrace(XZSTC_INFO,"IN : main");
	retcode = XZAG02_Connexion_Base( "SDCI1");
	printf("XZAG02_Connexion_Base retourne :%d\n",retcode);
 	
 	va_NumEvt.numero =1;
 	va_NumEvt.cle = 1;
	
	retcode =  XZAE23_Decrire_Bouchon_PA(va_Horodate,va_NumEvt, 
				&va_PRQueue_out,
				&va_Type_out,
				&va_Longueur_out,
				&va_NumEvtQueue);
	
	printf("\n\n*********\nXZAE23_Decrire_Bouchon_PA retourne :%d\n******\n",retcode);

	printf("\nPR = %d \nType = %d\nLongueur = %d\nNumEvt = %d\nCleEvt = %d\n",va_PRQueue_out,va_Type_out,va_Longueur_out,va_NumEvtQueue.numero,va_NumEvtQueue.cle);

	retcode = XZAG03_Deconnexion_Base();
	printf("XZAG03_Deconnexion_Base retourne %d\n",retcode);
}
