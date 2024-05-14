/*E*/
/*  Fichier : $Id: con_e31_decon.c,v 1.1 1994/09/15 17:37:40 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/09/15 17:37:40 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAE * FICHIER con_e31_decon.c
*               fait appel a xzag et xzae31
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  XZAG02_Connexion_Base permet de se connecter a la base PRC
*  du SQL_Srv de la machine passee en argument : On renseigne
*  une globale XZAGV_Connexion contenant l ID de connexion
*  Les codes de retour sont XDC_OK si tout s est bien passe 
*  sinon XDC_NOK voire le code d erreur Sybase
*  Fait appel a XZAE31_Decrire_Basculement_PA
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
#include "xzae31.h"
#include "xzst.h"
#include "xzsc.h"


static char *version = "@(#)con_e31_decon.c	1.1 08/31/94 : XZAE" ;

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
	/* XDY_Horodate 	Horodate=23167767; */
	XDY_Horodate 	va_Horodate=765639932;
	XDY_Evt 	va_NumEvt;
	XZAET_Basculement 	va_Description;
	XZAET_Voies		va_EtatVoies;
	

	int retcode;

/*	retcode = XZSC_InitTrace();*/
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : main");
	XZST_03EcritureTrace(XZSTC_INFO,"IN : main");
	retcode = XZAG02_Connexion_Base( "SDCI1");
	printf("XZAG02_Connexion_Base retourne :%d\n",retcode);
 	
 	va_NumEvt.numero =1;
 	va_NumEvt.cle = 1;
	
	retcode =  XZAE31_Decrire_Basculement_PA( va_Horodate,va_NumEvt, &va_Description, &va_EtatVoies);
	
	printf("\n\n*********\nXZAE31_Decrire_Basculement_PA retourne :%d\n******\n",retcode);
	
	printf("\nva_Description.PR = %d\nva_Description.Longueur = %d\n",va_Description.PR,va_Description.Longueur);
	
	printf("\nva_EtatVoies.VR = %d\nva_EtatVoies.VM2 = %d\nva_EtatVoies.VM1 = %d\nva_EtatVoies.VL = %d\nva_EtatVoies.BAU = %d\nva_EtatVoies.VR_I  = %d\nva_EtatVoies.VM2_I = %d\nva_EtatVoies.VM1_I = %d\nva_EtatVoies.VL_I = %d\nva_EtatVoies.BAU_I = %d\n",
	va_EtatVoies.VR ,
	va_EtatVoies.VM2,
	va_EtatVoies.VM1,
	va_EtatVoies.VL,
	va_EtatVoies.BAU,
	va_EtatVoies.VR_I,
	va_EtatVoies.VM2_I,
	va_EtatVoies.VM1_I,
	va_EtatVoies.VL_I,
	va_EtatVoies.BAU_I);

	retcode = XZAG03_Deconnexion_Base();
	printf("XZAG03_Deconnexion_Base retourne %d\n",retcode);
}
