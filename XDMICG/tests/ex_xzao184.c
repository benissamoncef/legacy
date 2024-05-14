/*E*/
/*Fichier : $Id: ex_xzao184.c,v 1.1 1994/10/05 19:35:47 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/10/05 19:35:47 $
------------------------------------------------------
* GTIE *  AdresseOJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER ex_xzao184.c
*               fait appel a xzag et xzao184
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  XZAG02_Connexion_Base permet de se connecter a la base AdresseC
*  du SQL_Srv de la machine passee en argument : On renseigne
*  une globale XZAGV_Connexion contenant l ID de connexion
*  Les codes de retour sont XDC_OK si tout s est bien passe 
*  sinon XDC_NOK voire le code d erreur Sybase
*  Fait appel a XZAO184_Ajouter_PnGTC_Ech
*  Se deconnecte du serveur SQL
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	09 Sep 1994	: Creation
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "@(#)ex_xzao184.c	1.1 10/03/94 : XZAO" ;

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
*    Ouvre une connexion a la base AdresseC sur le serveur de donnees du Site
*    Initialise la variable globale au Module XZAGV_Connexion a la connexion 
*    Sybase si tout s est bien passe sinon elle est mise a NULL.
*
------------------------------------------------------*/
{
	XDY_Basedd		va_Basedd_in;
	XZAOT_ConfPnGTC		va_PnGTC_in;
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
	strcpy(va_PnGTC_in.NomPnGTC,"PnGTC 1");
	strcpy(va_PnGTC_in.Autoroute,"A800");
	strcpy(va_PnGTC_in.NomMaitre,"ECH 1");
	va_PnGTC_in.PR = 185010;
	va_PnGTC_in.Sens = 1;
	va_PnGTC_in.RangComm = 1;
	va_PnGTC_in.RangComm2 = 1;
	va_PnGTC_in.RangEtat = 1;
	va_PnGTC_in.RangEtat2 = 1;
	va_PnGTC_in.BitDiscordance = 1;
	va_PnGTC_in.BitDiscordance = 1;
	
	retcode =  XZAO184_Ajouter_PnGTC (va_Basedd_in,va_PnGTC_in,&va_Resultat_out );
	
	printf("\n\n*********\nXZAO184_Ajouter_PnGTC retourne :%d\n******\n",retcode);

	printf("\nva_Resultat_out = %d \n",va_Resultat_out);
				
	/*A
	** Test 3
	*/
	
	printf("\n** Test 3 de la procedure stockee **\n");
	
	strcpy(va_Basedd_in,"CFG");
	strcpy(va_PnGTC_in.NomPnGTC,"PnGTC 1");
	strcpy(va_PnGTC_in.Autoroute,"A800");
	strcpy(va_PnGTC_in.NomMaitre,"ECH 1");
	va_PnGTC_in.PR = 189910;
	va_PnGTC_in.Sens = 1;
	va_PnGTC_in.RangComm = 1;
	va_PnGTC_in.RangComm2 = 1;
	va_PnGTC_in.RangEtat = 1;
	va_PnGTC_in.RangEtat2 = 1;
	va_PnGTC_in.BitDiscordance = 1;
	va_PnGTC_in.BitDiscordance = 1;
	
	retcode =  XZAO184_Ajouter_PnGTC (va_Basedd_in,va_PnGTC_in,&va_Resultat_out );
	
	printf("\n\n*********\nXZAO184_Ajouter_PnGTC retourne :%d\n******\n",retcode);

	printf("\nva_Resultat_out = %d \n",va_Resultat_out);
				
	/*A
	** Test 5
	*/
	
	printf("\n** Test 5 de la procedure stockee **\n");
	
	strcpy(va_Basedd_in,"CFG");
	strcpy(va_PnGTC_in.NomPnGTC,"PnGTC 1");
	strcpy(va_PnGTC_in.Autoroute,"A800");
	strcpy(va_PnGTC_in.NomMaitre,"ECH 1");
	va_PnGTC_in.PR = 189999;
	va_PnGTC_in.Sens = 1;
	va_PnGTC_in.RangComm = 2;
	va_PnGTC_in.RangComm2 = 1;
	va_PnGTC_in.RangEtat = 1;
	va_PnGTC_in.RangEtat2 = 1;
	va_PnGTC_in.BitDiscordance = 2;
	va_PnGTC_in.BitDiscordance = 1;
	
	retcode =  XZAO184_Ajouter_PnGTC (va_Basedd_in,va_PnGTC_in,&va_Resultat_out );
	
	printf("\n\n*********\nXZAO184_Ajouter_PnGTC retourne :%d\n******\n",retcode);

	printf("\nva_Resultat_out = %d \n",va_Resultat_out);
				
				
				
	
	retcode = XZAG03_Deconnexion_Base();
	printf("XZAG03_Deconnexion_Base retourne %d\n",retcode);
}
