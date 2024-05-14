/*E*/
/*  Fichier : $Id: con_e22_decon.c,v 1.1 1994/09/15 17:37:18 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/09/15 17:37:18 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAE * FICHIER con_e22_decon.c
*               fait appel a xzag et xzae22
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  XZAG02_Connexion_Base permet de se connecter a la base PRC
*  du SQL_Srv de la machine passee en argument : On renseigne
*  une globale XZAGV_Connexion contenant l ID de connexion
*  Les codes de retour sont XDC_OK si tout s est bien passe 
*  sinon XDC_NOK voire le code d erreur Sybase
*  Fait appel a XZAE22_Decrire_Accident_PA
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
#include "xzae22.h"
#include "xzst.h"
#include "xzsc.h"


static char *version = "@(#)con_e22_decon.c	1.1 08/24/94 : XZAE" ;

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
	/* XDY_Horodate 	Horodate=765634332; */
	XDY_Horodate 	Horodate=765634332;
	XDY_Evt 	NumEvt;
	XDY_Fonction 	FontionUtil;
	XZAET_Accident  Description;
	XZAET_ComplAcc 	Complement;

	int retcode;

/*	retcode = XZSC_InitTrace();*/
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : main");
	XZST_03EcritureTrace(XZSTC_INFO,"IN : main");
	retcode = XZAG02_Connexion_Base( "SDCI1");
	printf("XZAG02_Connexion_Base retourne :%d\n",retcode);
 	
 	NumEvt.numero = 3;
 	NumEvt.cle = 1;
	
	retcode =  XZAE22_Decrire_Accident_PA(Horodate,NumEvt,FontionUtil,&Description,&Complement);
	
	printf("\n\n*********\nXZAE22_Decrire_Accident_PA retourne :%d\n******\n",retcode);

 	printf("\nva_VL_out = %d\nva_PL_out = %d\nva_Remorque_out = %d\nva_Car_out = %d\nva_Moto_out = %d\nva_BlesseLeg_out = %d\nva_BlesseGra_out = %d\nva_Mort_out = %d\nva_VehEscota_out = %d\nva_MatDang_out = %d\nva_HomEscota = %d\nva_Personnalite = %d\nDegatsDom = %d\n",
 	Description.VL,
	Description.PL,
	Description.Remorque,
	Description.Car,
	Description.Moto,
	Description.BlesseLeg,
	Description.BlesseGra,
	Description.Mort,
	Description.VehEscota,
	Description.MatDang,
	Description.HomEscota,
	Description.Personnalite,
	Description.DegatsDom);
 	
 	printf("\nComplement.PassagersBus = %d\nComplement.Bebe = %d\nComplement.PersAgee = %d\nComplement.Animaux = %d\n",Complement.PassagersBus,Complement.Bebe,Complement.PersAgee,Complement.Animaux);

	retcode = XZAG03_Deconnexion_Base();
	printf("XZAG03_Deconnexion_Base retourne %d\n",retcode);
}
