/*E*/
/*  Fichier : $Id: con_e21_decon.c,v 1.1 1994/09/15 17:37:16 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/09/15 17:37:16 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAE * FICHIER con_e21_decon.c
*               fait appel a xzag et xzae21
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  XZAG02_Connexion_Base permet de se connecter a la base PRC
*  du SQL_Srv de la machine passee en argument : On renseigne
*  une globale XZAGV_Connexion contenant l ID de connexion
*  Les codes de retour sont XDC_OK si tout s est bien passe 
*  sinon XDC_NOK voire le code d erreur Sybase
*  Fait appel a XZAE21_Localiser_Evt_PA
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
#include "xzae21.h"
#include "xzst.h"
#include "xzsc.h"


static char *version = "@(#)con_e21_decon.c	1.1 08/21/94 : XZAE" ;

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
	XDY_Autoroute	va_Autoroute_in;
	XDY_PR		va_PR_in;
	XDY_Sens	va_Sens_in;
	XDY_Octet 	va_NumSortieAmont_out;
	XDY_Entier 	va_DistanceSortieAmont_out;
	XDY_Octet 	va_NumSortieAval_out;
	XDY_Entier 	va_DistanceSortieAval_out;
	XDY_Octet 	va_NumPeageAmont_out;
	XDY_Entier 	va_DistancePeageAmont_out;
	XDY_Octet 	va_NumPeageAval_out;
	XDY_Entier 	va_DistancePeageAval_out;
	XDY_Entier 	va_LongueurPlateformePeageAval_out;
	XDY_Autoroute 	va_NumAutorouteBifAmont_out;
	XDY_Entier 	va_DistanceBifAmont_out;
	XDY_Autoroute 	va_NumAutorouteBifAval_out;
	XDY_Entier 	va_DistanceBifAval_out;
	XDY_Nom 	va_Descente_out;
	XDY_Nom 	va_Tunnel_out;
	
	int retcode;

/*	retcode = XZSC_InitTrace();*/
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : main");
	XZST_03EcritureTrace(XZSTC_INFO,"IN : main");
	retcode = XZAG02_Connexion_Base( "SDCI1");
	printf("XZAG02_Connexion_Base retourne :%d\n",retcode);
 	
	
	retcode =  XZAE21_Localiser_Evt_PA(va_Autoroute_in,
				va_PR_in,
				va_Sens_in,
				&va_NumSortieAmont_out,
				&va_DistanceSortieAmont_out,
				&va_NumSortieAval_out,
				&va_DistanceSortieAval_out,
				&va_NumPeageAmont_out,
				&va_DistancePeageAmont_out,
				&va_NumPeageAval_out,
				&va_DistancePeageAval_out,
				&va_LongueurPlateformePeageAval_out,
				&va_NumAutorouteBifAmont_out,
				&va_DistanceBifAmont_out,
				&va_NumAutorouteBifAval_out,
				&va_DistanceBifAval_out,
				&va_Descente_out,
				&va_Tunnel_out);
	
	printf("\n\n*********\nXZAE21_Localiser_Evt_PA retourne :%d\n******\n",retcode);

	printf("\nva_NumSortieAmont_out = %d \nva_DistanceSortieAmont_out = %d\nva_NumSortieAval_out = %d\nva_DistanceSortieAval_out = %d\nva_NumPeageAmont_out = %d\nva_DistancePeageAmont_out = %d\nva_NumPeageAval_out = %d\nva_DistancePeageAval_out = %d\nva_DistanceBifAvalPlateformePeageAval_out = %d\nva_NumAutorouteBifAmont_out = %d\nva_DistanceBifAmont_out = %d\nva_NumAutorouteBifAval_out = %d\nva_DistanceBifAval_out = %d\nva_Descente_out = %d\nva_Tunnel_out = %d\n",
				va_NumSortieAmont_out,
				va_DistanceSortieAmont_out,
				va_NumSortieAval_out,
				va_DistanceSortieAval_out,
				va_NumPeageAmont_out,
				va_DistancePeageAmont_out,
				va_NumPeageAval_out,
				va_DistancePeageAval_out,
				va_LongueurPlateformePeageAval_out,
				va_NumAutorouteBifAmont_out,
				va_DistanceBifAmont_out,
				va_NumAutorouteBifAval_out,
				va_DistanceBifAval_out,
				va_Descente_out,
				va_Tunnel_out);
				
	retcode = XZAG03_Deconnexion_Base();
	printf("XZAG03_Deconnexion_Base retourne %d\n",retcode);
}
