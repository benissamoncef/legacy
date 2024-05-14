/*E*/
/*Fichier : $Id: ex_xzao64.c,v 1.1 1994/10/05 19:36:03 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/10/05 19:36:03 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER ex_xzao64.c
*               fait appel a xzag et xzao64
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  XZAG02_Connexion_Base permet de se connecter a la base PRC
*  du SQL_Srv de la machine passee en argument : On renseigne
*  une globale XZAGV_Connexion contenant l ID de connexion
*  Les codes de retour sont XDC_OK si tout s est bien passe 
*  sinon XDC_NOK voire le code d erreur Sybase
*  Fait appel a XZAO64_Ajouter_Machine
*  Se deconnecte du serveur SQL
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	09 Sep 1994	: Creation
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "@(#)ex_xzao64.c	1.1 10/03/94 : XZAO" ;

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
	XZAOT_ConfMachine 	va_Machine_in;
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
	strcpy(va_Machine_in.NomMachine,"PODMA");
	strcpy(va_Machine_in.NomDistrict,"Nice");
	
	retcode =  XZAO64_Ajouter_Machine(va_Basedd_in,va_Machine_in,&va_Resultat_out );
	
	printf("\n\n*********\nXZAO64_Ajouter_Machine retourne :%d\n******\n",retcode);

	printf("\nva_Resultat_out = %d \n",va_Resultat_out);
				
	/*A
	** Test 3
	*/
	
	printf("\n** Test 3 de la procedure stockee **\n");
	
	strcpy(va_Basedd_in,"CFG");
	strcpy(va_Machine_in.NomMachine,"PODMA");
	strcpy(va_Machine_in.NomDistrict,"Mandelieu");
	
	retcode =  XZAO64_Ajouter_Machine(va_Basedd_in,va_Machine_in,&va_Resultat_out );
	
	printf("\n\n*********\nXZAO64_Ajouter_Machine retourne :%d\n******\n",retcode);

	printf("\nva_Resultat_out = %d \n",va_Resultat_out);
				
	/*A
	** Test 5
	*/
	
	printf("\n** Test 5 de la procedure stockee **\n");
	
	strcpy(va_Basedd_in,"CFG");
	strcpy(va_Machine_in.NomMachine,"PODFA");
	strcpy(va_Machine_in.NomDistrict,"Nice");
	
	retcode =  XZAO64_Ajouter_Machine(va_Basedd_in,va_Machine_in,&va_Resultat_out );
	
	printf("\n\n*********\nXZAO64_Ajouter_Machine retourne :%d\n******\n",retcode);

	printf("\nva_Resultat_out = %d \n",va_Resultat_out);
				
	/*A
	** Test 6
	*/
	
	printf("\n** Test 6 de la procedure stockee **\n");
	
	strcpy(va_Basedd_in,"CFG");
	strcpy(va_Machine_in.NomMachine,"PODN1");
	strcpy(va_Machine_in.NomDistrict,"Mandelieu");
	
	retcode =  XZAO64_Ajouter_Machine(va_Basedd_in,va_Machine_in,&va_Resultat_out );
	
	printf("\n\n*********\nXZAO64_Ajouter_Machine retourne :%d\n******\n",retcode);

	printf("\nva_Resultat_out = %d \n",va_Resultat_out);
				
	/*A
	** Test 7
	*/
	
	printf("\n** Test 7 de la procedure stockee **\n");
	
	strcpy(va_Basedd_in,"CFG");
	strcpy(va_Machine_in.NomMachine,"PODN4");
	strcpy(va_Machine_in.NomDistrict,"Nice");
	
	retcode =  XZAO64_Ajouter_Machine(va_Basedd_in,va_Machine_in,&va_Resultat_out );
	
	printf("\n\n*********\nXZAO64_Ajouter_Machine retourne :%d\n******\n",retcode);

	printf("\nva_Resultat_out = %d \n",va_Resultat_out);
				
	/*A
	** Test 8
	*/
	
	printf("\n** Test 8 de la procedure stockee **\n");
	
	strcpy(va_Basedd_in,"CFG");
	strcpy(va_Machine_in.NomMachine,"SDDN1");
	strcpy(va_Machine_in.NomDistrict,"Nice");
	
	retcode =  XZAO64_Ajouter_Machine(va_Basedd_in,va_Machine_in,&va_Resultat_out );
	
	printf("\n\n*********\nXZAO64_Ajouter_Machine retourne :%d\n******\n",retcode);

	printf("\nva_Resultat_out = %d \n",va_Resultat_out);
				
	/*A
	** Test 9
	*/
	
	printf("\n** Test 9 de la procedure stockee **\n");
	
	strcpy(va_Basedd_in,"CFG");
	strcpy(va_Machine_in.NomMachine,"SDCI1");
	strcpy(va_Machine_in.NomDistrict,"Nice");
	
	retcode =  XZAO64_Ajouter_Machine(va_Basedd_in,va_Machine_in,&va_Resultat_out );
	
	printf("\n\n*********\nXZAO64_Ajouter_Machine retourne :%d\n******\n",retcode);

	printf("\nva_Resultat_out = %d \n",va_Resultat_out);
				
	/*A
	** Test 10
	*/
	
	printf("\n** Test 10 de la procedure stockee **\n");
	
	strcpy(va_Basedd_in,"CFG");
	strcpy(va_Machine_in.NomMachine,"SDDM1");
	strcpy(va_Machine_in.NomDistrict,"Mandelieu");
	
	retcode =  XZAO64_Ajouter_Machine(va_Basedd_in,va_Machine_in,&va_Resultat_out );
	
	printf("\n\n*********\nXZAO64_Ajouter_Machine retourne :%d\n******\n",retcode);

	printf("\nva_Resultat_out = %d \n",va_Resultat_out);
				
	retcode = XZAG03_Deconnexion_Base();
	printf("XZAG03_Deconnexion_Base retourne %d\n",retcode);
}
