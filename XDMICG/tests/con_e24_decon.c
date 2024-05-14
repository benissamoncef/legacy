/*E*/
/*  Fichier : $Id: con_e24_decon.c,v 1.1 1994/09/15 17:37:21 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/09/15 17:37:21 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAE * FICHIER con_e24_decon.c
*               fait appel a xzag et xzae24
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  XZAG02_Connexion_Base permet de se connecter a la base PRC
*  du SQL_Srv de la machine passee en argument : On renseigne
*  une globale XZAGV_Connexion contenant l ID de connexion
*  Les codes de retour sont XDC_OK si tout s est bien passe 
*  sinon XDC_NOK voire le code d erreur Sybase
*  Fait appel a XZAE24_Decrire_Travaux_PA
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
#include "xzae24.h"
#include "xzst.h"
#include "xzsc.h"


static char *version = "@(#)con_e24_decon.c	1.1 08/24/94 : XZAE" ;

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
	XZAET_Travaux 	va_Travaux;
	

	int retcode;

/*	retcode = XZSC_InitTrace();*/
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : main");
	XZST_03EcritureTrace(XZSTC_INFO,"IN : main");
	retcode = XZAG02_Connexion_Base( "SDCI1");
	printf("XZAG02_Connexion_Base retourne :%d\n",retcode);
 	
 	va_NumEvt.numero =1;
 	va_NumEvt.cle = 1;
	
	retcode =  XZAE24_Decrire_Travaux_PA( va_Horodate,va_NumEvt, &va_Travaux);
	
	printf("\n\n*********\nXZAE24_Decrire_Travaux_PA retourne :%d\n******\n",retcode);

	printf("\nva_Travaux.Nature = %d\nva_Travaux.Type = %d\nva_Travaux.Continu = %d\nva_Travaux.HoroDebC = %f\nva_Travaux.HoroFinC = %f\nva_Travaux.Mobile = %d\nva_Travaux.Autoroute = %d\nva_Travaux.PRqueueM = %d\nva_Travaux.PRteteM = %d\nva_Travaux.SensM = %d\nva_Travaux.PRtete = %d\nva_Travaux.VoiesRet = %d\nva_Travaux.Longueur = %d\nva_Travaux.LongueurM = %d\n",
	va_Travaux.Nature,
	va_Travaux.Type,
	va_Travaux.Continu,
	va_Travaux.HoroDebC,
	va_Travaux.HoroFinC,
	va_Travaux.Mobile,
	va_Travaux.Autoroute,
	va_Travaux.PRqueueM,
	va_Travaux.PRteteM,
	va_Travaux.SensM,
	va_Travaux.PRtete,
	va_Travaux.VoiesRet,
	va_Travaux.Longueur,
	va_Travaux.LongueurM);

	retcode = XZAG03_Deconnexion_Base();
	printf("XZAG03_Deconnexion_Base retourne %d\n",retcode);
}
