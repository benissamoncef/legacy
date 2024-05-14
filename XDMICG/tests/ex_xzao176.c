/*E*/
/*Fichier : $Id: ex_xzao176.c,v 1.1 1994/10/05 19:35:39 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/10/05 19:35:39 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER ex_xzao176.c
*               fait appel a xzag et xzao176
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  XZAG02_Connexion_Base permet de se connecter a la base PRC
*  du SQL_Srv de la machine passee en argument : On renseigne
*  une globale XZAGV_Connexion contenant l ID de connexion
*  Les codes de retour sont XDC_OK si tout s est bien passe 
*  sinon XDC_NOK voire le code d erreur Sybase
*  Fait appel a XZAO176_Ajouter_Demi_Ech_Ech
*  Se deconnecte du serveur SQL
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	09 Sep 1994	: Creation
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "@(#)ex_xzao176.c	1.1 10/03/94 : XZAO" ;

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
	XDY_Basedd		va_Basedd_in;
	XZAOT_ConfDemiEch	va_Demi_Ech_in;
	XDY_ResConf  		va_Resultat_out;
	
	int retcode;

/*	retcode = XZSC_InitTrace();*/
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : main");
	XZST_03EcritureTrace(XZSTC_INFO,"IN : main");
	retcode = XZAG02_Connexion_Base( "SDCI1");
	printf("XZAG02_Connexion_Base retourne :%d\n",retcode);
 	
	
	/*A
	** Test 1
	*/
	
	printf("\n** Test 1 de la procedure stockee **\n");
	
	strcpy(va_Basedd_in,"CFG");
	strcpy(va_Demi_Ech_in.NomDemiEch,"ECH 1");
	strcpy(va_Demi_Ech_in.Autoroute,"A800");
	strcpy(va_Demi_Ech_in.NomServeur,"SDCI1");
	va_Demi_Ech_in.PR = 175000;
	va_Demi_Ech_in.Sens = 1;
	va_Demi_Ech_in.AdresseEtat = 1;
	va_Demi_Ech_in.AdresseCommande = 2;
	va_Demi_Ech_in.AdresseDiscordance = 3;
	va_Demi_Ech_in.AdresseTelealarme = 4;
	
	
	retcode =  XZAO176_Ajouter_Demi_Ech (va_Basedd_in,va_Demi_Ech_in,&va_Resultat_out);
	
	printf("\n\n*********\nXZAO176_Ajouter_Demi_Ech retourne :%d\n******\n",retcode);

	printf("\nva_Resultat_out = %d \n",va_Resultat_out);
				
	/*A
	** Test 3
	*/
	
	printf("\n** Test 3 de la procedure stockee **\n");
	
	strcpy(va_Basedd_in,"CFG");
	strcpy(va_Demi_Ech_in.NomDemiEch,"ECH 1");
	strcpy(va_Demi_Ech_in.Autoroute,"A800");
	strcpy(va_Demi_Ech_in.NomServeur,"SDCI1");
	va_Demi_Ech_in.PR = 185000;
	va_Demi_Ech_in.Sens = 1;
	va_Demi_Ech_in.AdresseEtat = 3;
	va_Demi_Ech_in.AdresseCommande = 2;
	va_Demi_Ech_in.AdresseDiscordance = 5;
	va_Demi_Ech_in.AdresseTelealarme = 4;
	retcode =  XZAO176_Ajouter_Demi_Ech (va_Basedd_in,va_Demi_Ech_in,&va_Resultat_out );
	
	printf("\n\n*********\nXZAO176_Ajouter_Demi_Ech retourne :%d\n******\n",retcode);

	printf("\nva_Resultat_out = %d \n",va_Resultat_out);
				
	/*A
	** Test 5
	*/
	
	printf("\n** Test 5 de la procedure stockee **\n");
	
	strcpy(va_Basedd_in,"CFG");
	strcpy(va_Demi_Ech_in.NomDemiEch,"ECH 2");
	strcpy(va_Demi_Ech_in.Autoroute,"A800");
	strcpy(va_Demi_Ech_in.NomServeur,"SDCI1");
	va_Demi_Ech_in.PR = 185000;
	va_Demi_Ech_in.Sens = 1;
	va_Demi_Ech_in.AdresseEtat = 1;
	va_Demi_Ech_in.AdresseCommande = 2;
	va_Demi_Ech_in.AdresseDiscordance = 3;
	va_Demi_Ech_in.AdresseTelealarme = 4;
	
	retcode =  XZAO176_Ajouter_Demi_Ech (va_Basedd_in,va_Demi_Ech_in,&va_Resultat_out );
	
	printf("\n\n*********\nXZAO176_Ajouter_Demi_Ech retourne :%d\n******\n",retcode);

	printf("\nva_Resultat_out = %d \n",va_Resultat_out);
				
				
	retcode = XZAG03_Deconnexion_Base();
	printf("XZAG03_Deconnexion_Base retourne %d\n",retcode);
}
