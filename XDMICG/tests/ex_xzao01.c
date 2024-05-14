/*E*/
/*Fichier : $Id: ex_xzao01.c,v 1.1 1994/10/05 19:34:38 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/10/05 19:34:38 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER ex_xzao01.c
*               fait appel a xzag et xzao01
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  XZAG02_Connexion_Base permet de se connecter a la base PRC
*  du SQL_Srv de la machine passee en argument : On renseigne
*  une globale XZAGV_Connexion contenant l ID de connexion
*  Les codes de retour sont XDC_OK si tout s est bien passe 
*  sinon XDC_NOK voire le code d erreur Sybase
*  Fait appel a XZAO51_Ajouter_Autoroute
*  Se deconnecte du serveur SQL
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	09 Sep 1994	: Creation
------------------------------------------------------*/

/* fichiers inclus */
#include "xzao.h"


static char *version = "@(#)ex_xzao01.c	1.1 10/03/94 : XZAO" ;

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
	XDY_Entier 	va_Resultat_out;
	
	int retcode;

/*	retcode = XZSC_InitTrace();*/
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : main");
	XZST_03EcritureTrace(XZSTC_INFO,"IN : main");
	retcode = XZAG02_Connexion_Base( "SDCI1");
	printf("XZAG02_Connexion_Base retourne :%d\n",retcode);
 	
	
	retcode =  XZAO51_Ajouter_Autoroute(va_Autoroute_in,
				&va_Resultat_out );
	
	printf("\n\n*********\nXZAO51_Ajouter_Autoroute retourne :%d\n******\n",retcode);

	printf("\nva_Resultat_out = %d \n,
				va_Resultat_out);
				
	retcode = XZAG03_Deconnexion_Base();
	printf("XZAG03_Deconnexion_Base retourne %d\n",retcode);
}
