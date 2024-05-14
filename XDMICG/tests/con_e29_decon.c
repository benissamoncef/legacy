/*E*/
/*  Fichier : $Id: con_e29_decon.c,v 1.1 1994/09/15 17:37:31 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/09/15 17:37:31 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAE * FICHIER con_e29_decon.c
*               fait appel a xzag et xzae29
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  XZAG02_Connexion_Base permet de se connecter a la base PRC
*  du SQL_Srv de la machine passee en argument : On renseigne
*  une globale XZAGV_Connexion contenant l ID de connexion
*  Les codes de retour sont XDC_OK si tout s est bien passe 
*  sinon XDC_NOK voire le code d erreur Sybase
*  Fait appel a XZAE29_Decrire_Manifestation_PA
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
#include "xzae29.h"
#include "xzst.h"
#include "xzsc.h"


static char *version = "@(#)con_e29_decon.c	1.1 08/29/94 : XZAE" ;

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
	/* XDY_Horodate 	Horodate=22967767; */
	XDY_Horodate 	va_Horodate=765634332;
	XDY_Evt 	va_NumEvt;
	XZAET_Manifestation 	va_Manifestation;
	

	int retcode;

/*	retcode = XZSC_InitTrace();*/
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : main");
	XZST_03EcritureTrace(XZSTC_INFO,"IN : main");
	retcode = XZAG02_Connexion_Base( "SDCI1");
	printf("XZAG02_Connexion_Base retourne :%d\n",retcode);
 	
 	va_NumEvt.numero =1;
 	va_NumEvt.cle = 1;
	
	retcode =  XZAE29_Decrire_Manifestation_PA( va_Horodate,va_NumEvt, &va_Manifestation);
	
	printf("\n\n*********\nXZAE29_Decrire_Manifestation_PA retourne :%d\n******\n",retcode);

	printf("\nva_Manifestation->Nombre = %d\nva_Manifestation->Perception = %d\nva_Manifestation->Violence = %d\n",
		va_Manifestation.Nombre,
		va_Manifestation.Perception,
		va_Manifestation.Violence );
		
	retcode = XZAG03_Deconnexion_Base();
	printf("XZAG03_Deconnexion_Base retourne %d\n",retcode);
}
