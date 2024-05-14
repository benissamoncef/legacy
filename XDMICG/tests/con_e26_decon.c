/*E*/
/*  Fichier : $Id: con_e26_decon.c,v 1.2 1994/09/23 17:23:53 volcic Exp $      Release : $Revision: 1.2 $        Date : $Date: 1994/09/23 17:23:53 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAE * FICHIER con_e26_decon.c
*               fait appel a xzag et xzae26
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  XZAG02_Connexion_Base permet de se connecter a la base PRC
*  du SQL_Srv de la machine passee en argument : On renseigne
*  une globale XZAGV_Connexion contenant l ID de connexion
*  Les codes de retour sont XDC_OK si tout s est bien passe 
*  sinon XDC_NOK voire le code d erreur Sybase
*  Fait appel a XZAE26_Decrire_Meteo_PA
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
#include "xzae26.h"
#include "xzst.h"
#include "xzsc.h"


static char *version = "@(#)con_e26_decon.c	1.1 08/26/94 : XZAE" ;

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
	XZAET_Meteo 	va_Meteo;
	

	int retcode;

/*	retcode = XZSC_InitTrace();*/
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : main");
	XZST_03EcritureTrace(XZSTC_INFO,"IN : main");
	retcode = XZAG02_Connexion_Base( "SDCI1");
	printf("XZAG02_Connexion_Base retourne :%d\n",retcode);
 	
 	va_NumEvt.numero =1;
 	va_NumEvt.cle = 1;
	
	retcode =  XZAE26_Decrire_Meteo_PA( va_Horodate,va_NumEvt, &va_Meteo);
	
	printf("\n\n*********\nXZAE26_Decrire_Meteo_PA retourne :%d\n******\n",retcode);

	printf("\nva_Meteo.PRFin = %d\nva_Meteo.Perturbation = %d\nva_Meteo.Visibilite  = %d\nva_Meteo.Longueur = %d\n",
	va_Meteo.PRFin,
	va_Meteo.Perturbation,
	va_Meteo.Visibilite,
	va_Meteo.Longueur);

	retcode = XZAG03_Deconnexion_Base();
	printf("XZAG03_Deconnexion_Base retourne %d\n",retcode);
}
