/*X  Fichier : $Id: con_sp016_decon.c,v 1.2 1994/11/23 23:25:00 mercier Exp $	      Release : $Revision: 1.2 $        Date : $Date: 1994/11/23 23:25:00 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAG * FICHIER con_sp016_decon.c
*               fait appel a xzag et xzao16
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  XZAG02_Connexion_Base permet de se connecter a la base PRC
*  du SQL_Srv de la machine passee en argument : On renseigne
*  une globale XZAGV_Connexion contenant l ID de connexion
*  Les codes de retour sont XDC_OK si tout s est bien passe 
*  sinon XDC_NOK voire le code d erreur Sybase
*  Fait appel a XZAO16_Aire_Amont
*  Se deconnecte du serveur SQL
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	16 Aug 1994	: Creation
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
#include "xzao16.h"
#include "xzst.h"
#include "xzsc.h"


static char *version = "$Id: con_sp016_decon.c,v 1.2 1994/11/23 23:25:00 mercier Exp $ $Revision: 1.2 $ $Date: 1994/11/23 23:25:00 $ : con_sp016_decon" ;

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
	XDY_Autoroute Autoroute_In=1;
	XDY_PR		PR_In=150000;
	XDY_Sens	Sens_In=1;
	XDY_Mot		Numero_In=1;

	XDY_Octet 	NumAire_Out;
	XDY_Nom         NomAire_Out;
	XDY_Abrev       AbrevAire_Out;
	XDY_Autoroute 	Autoroute_Out;
	XDY_PR 		PR_Out;
	XDY_Sens 	Sens_Out;
	XDY_Entier 	Distance_Out;

	
	int retcode;

/*	retcode = XZSC_InitTrace();*/
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : main");
	XZST_03EcritureTrace(XZSTC_INFO,"IN : main");
	
	retcode = XZAG02_Connexion_Base( "CI");
	printf("XZAG02_Connexion_Base retourne :%d\n",retcode);

	retcode =  XZAO16_Aire_Amont( Autoroute_In , PR_In , Sens_In , Numero_In , &NumAire_Out, NomAire_Out, AbrevAire_Out , &Autoroute_Out , &PR_Out , &Sens_Out , &Distance_Out);

	printf("\n\n*********\nXZAO16_Aire_Amont retourne :%d\n******\n",retcode);

	printf("\nNumAire_Out=%d\nNomAire_Out=%s\nAbrevAire_Out=%s,\nAutoroute_Out = %d\nPR_Out = %d\nSens_Out = %d\nDistance_Out = %d\n\n",NumAire_Out,NomAire_Out,AbrevAire_Out,Autoroute_Out,PR_Out,Sens_Out,Distance_Out);

	retcode = XZAG03_Deconnexion_Base();
	printf("XZAG03_Deconnexion_Base retourne %d\n",retcode);
}
