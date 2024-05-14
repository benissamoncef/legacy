/*E*/
/*Fichier : $Id: ex_xzao166.c,v 1.1 1994/10/05 19:35:24 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/10/05 19:35:24 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER ex_xzao166.c
*               fait appel a xzag et xzao166
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  XZAG02_Connexion_Base permet de se connecter a la base PRC
*  du SQL_Srv de la machine passee en argument : On renseigne
*  une globale XZAGV_Connexion contenant l ID de connexion
*  Les codes de retour sont XDC_OK si tout s est bien passe 
*  sinon XDC_NOK voire le code d erreur Sybase
*  Fait appel a XZAO166_Ajouter_GTC_NICE
*  Se deconnecte du serveur SQL
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	09 Sep 1994	: Creation
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "@(#)ex_xzao166.c	1.1 10/03/94 : XZAO" ;

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
	XZAOT_ConfGTCNICE	va_GTC_Nice_in;
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
	strcpy(va_GTC_Nice_in.Nom,"GTC Nice");
	strcpy(va_GTC_Nice_in.NomMachine, "PODFA");
	va_GTC_Nice_in.LongueurITOR = 1;
	va_GTC_Nice_in.AdresseITOR = 1;
	va_GTC_Nice_in.LongueurETOR = 1;
	va_GTC_Nice_in.AdresseETOR = 1;
	va_GTC_Nice_in.LongTelealarme = 1;
	va_GTC_Nice_in.AdresseTelealarme = 1;
	va_GTC_Nice_in.LongTelecomEch = 1;
	va_GTC_Nice_in.AdresseTelecomEch = 1;
	va_GTC_Nice_in.LongTelecomTub = 1;
	va_GTC_Nice_in.AdresseTelecomTub = 1;
	va_GTC_Nice_in.LongTelesignEch = 1;
	va_GTC_Nice_in.AdresseTelesignEch = 1;
	va_GTC_Nice_in.LongTelesignTub = 1;
	va_GTC_Nice_in.AdresseTelesignTub = 1;
	va_GTC_Nice_in.LongSeqEch = 1;
	va_GTC_Nice_in.AdresseSeqEch = 1;
	va_GTC_Nice_in.LongSeqTub = 1;
	va_GTC_Nice_in.AdresseSeqTub = 1;
	
	printf("\n** OK1 **\n");
	
	retcode =  XZAO166_Ajouter_GTC_NICE(va_Basedd_in,va_GTC_Nice_in,&va_Resultat_out );
	
	printf("\n** OK2 **\n");
	
	printf("\n\n*********\nXZAO166_Ajouter_GTC_NICE retourne :%d\n******\n",retcode);

	printf("\nva_Resultat_out = %d \n",va_Resultat_out);
				
	/*A
	** Test 3
	*/
	
	printf("\n** Test 3 de la procedure stockee **\n");
	
	strcpy(va_Basedd_in,"CFG");
	strcpy(va_GTC_Nice_in.Nom,"GTC Nice");
	strcpy(va_GTC_Nice_in.NomMachine, "PODFA");
	va_GTC_Nice_in.LongueurITOR = 1;
	va_GTC_Nice_in.AdresseITOR = 2;
	va_GTC_Nice_in.LongueurETOR = 1;
	va_GTC_Nice_in.AdresseETOR = 2;
	va_GTC_Nice_in.LongTelealarme = 1;
	va_GTC_Nice_in.AdresseTelealarme = 2;
	va_GTC_Nice_in.LongTelecomEch = 2;
	va_GTC_Nice_in.AdresseTelecomEch = 1;
	va_GTC_Nice_in.LongTelecomTub = 1;
	va_GTC_Nice_in.AdresseTelecomTub = 5;
	va_GTC_Nice_in.LongTelesignEch = 1;
	va_GTC_Nice_in.AdresseTelesignEch = 1;
	va_GTC_Nice_in.LongTelesignTub = 1;
	va_GTC_Nice_in.AdresseTelesignTub = 1;
	va_GTC_Nice_in.LongSeqEch = 1;
	va_GTC_Nice_in.AdresseSeqEch = 1;
	va_GTC_Nice_in.LongSeqTub = 1;
	va_GTC_Nice_in.AdresseSeqTub = 1;
	
	retcode =  XZAO166_Ajouter_GTC_NICE(va_Basedd_in,va_GTC_Nice_in,&va_Resultat_out );
	
	printf("\n\n*********\nXZAO166_Ajouter_GTC_NICE retourne :%d\n******\n",retcode);

	printf("\nva_Resultat_out = %d \n",va_Resultat_out);
				
				
	
	retcode = XZAG03_Deconnexion_Base();
	printf("XZAG03_Deconnexion_Base retourne %d\n",retcode);
}
