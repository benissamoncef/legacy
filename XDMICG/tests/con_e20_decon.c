/*E*/
/*  Fichier : $Id: con_e20_decon.c,v 1.2 1994/12/01 16:11:24 volcic Exp $      Release : $Revision: 1.2 $        Date : $Date: 1994/12/01 16:11:24 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAE * FICHIER con_e20_decon.c
*               fait appel a xzag et xzae20
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  XZAG02_Connexion_Base permet de se connecter a la base PRC
*  du SQL_Srv de la machine passee en argument : On renseigne
*  une globale XZAGV_Connexion contenant l ID de connexion
*  Les codes de retour sont XDC_OK si tout s est bien passe 
*  sinon XDC_NOK voire le code d erreur Sybase
*  Fait appel a XZAE20_Decrire_Evt_PA
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
#include "xzae20.h"
#include "xzst.h"
#include "xzsc.h"


static char *version = "@(#)con_e20_decon.c	1.1 08/24/94 : XZAE" ;

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int *FonctionUtil(char *Commentaire)
{
   printf("%s\n",Commentaire);
   return XDC_OK;
}


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
	XDY_Horodate 	Horodate=781782500; 
	XDY_Evt 	NumEvt;
	XDY_Fonction 	FonctionUtil;
	XZAET_Evenement Description;
	XZAET_Voies 	va_VoiesCirculees;

	int retcode;

/*	retcode = XZSC_InitTrace();*/
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : main");
	XZST_03EcritureTrace(XZSTC_INFO,"IN : main");
	retcode = XZAG02_Connexion_Base( "CI");
	printf("XZAG02_Connexion_Base retourne :%d\n",retcode);
 	
 	NumEvt.numero = 200;
 	NumEvt.cle = 43;
	
	retcode =  XZAE20_Decrire_Evt_PA(Horodate,NumEvt, FonctionUtil, &Description,  &va_VoiesCirculees);
	
	printf("\n\n*********\nXZAE20_Decrire_Evt_PA retourne :%d\n******\n",retcode);

	printf("\n Description.Numero = %d\nDescription.TypeCause = %d\nDescription.NomPtCar = %s\n",Description.Numero,Description.TypeCause,Description.NomPtCar);
        
        printf("\nva_VoiesCirculees.VR = %d\nva_VoiesCirculees.VM2 = %d\nva_VoiesCirculees.VM1 = %d\nva_VoiesCirculees.VL = %d\nva_VoiesCirculees.BAU = %d\nva_VoiesCirculees.VR_I  = %d\nva_VoiesCirculees.VM2_I = %d\nva_VoiesCirculees.VM1_I = %d\nva_VoiesCirculees.VL_I = %d\nva_VoiesCirculees.BAU_I = %d\n",
	va_VoiesCirculees.VR ,
	va_VoiesCirculees.VM2,
	va_VoiesCirculees.VM1,
	va_VoiesCirculees.VL,
	va_VoiesCirculees.BAU,
	va_VoiesCirculees.VR_I,
	va_VoiesCirculees.VM2_I,
	va_VoiesCirculees.VM1_I,
	va_VoiesCirculees.VL_I,
	va_VoiesCirculees.BAU_I);
	
	retcode = XZAG03_Deconnexion_Base();
	printf("XZAG03_Deconnexion_Base retourne %d\n",retcode);
}
