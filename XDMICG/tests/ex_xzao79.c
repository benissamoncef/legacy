/*E*/
/*Fichier : $Id: ex_xzao79.c,v 1.1 1994/12/03 17:44:39 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/12/03 17:44:39 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER ex_xzao79.c
*               fait appel a xzag et xzao79
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  XZAG02_Connexion_Base permet de se connecter a la base PRC
*  du SQL_Srv de la machine passee en argument : On renseigne
*  une globale XZAGV_Connexion contenant l ID de connexion
*  Les codes de retour sont XDC_OK si tout s est bien passe 
*  sinon XDC_NOK voire le code d erreur Sybase
*  Fait appel a XZAO79_Lire_Liste_Picto
*  Se deconnecte du serveur SQL
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	3 Oct 1979	: Creation
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "$Id: ex_xzao79.c,v 1.1 1994/12/03 17:44:39 volcic Exp $ : ex_xzao79.c" ;

/* definition de fonctions externes */

int FctPicto(XZAOT_Picto vl_Arg1, XDY_Entier vl_Arg2)
{
   printf("\nPicto %d\n",vl_Arg2);
   return XDC_OK;
}




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
	
	XDY_Entier	vl_Res = 0;
	int retcode;

	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : main");
	XZST_03EcritureTrace(XZSTC_INFO,"IN : main");
	retcode = XZAG02_Connexion_Base( "DM");
	printf("XZAG02_Connexion_Base retourne :%d\n",retcode);
 	
	
	/*A
	** Test 1
	*/
	
	printf("\n** Test 1 de la procedure stockee **\n");
	
	retcode =  XZAO79_Lire_Liste_Picto ("CFG","POLTM04", FctPicto, &vl_Res );
	
	printf("\n\n*********\nXZAO79_Lire_Liste_Picto retourne :%d\n******\n",retcode);

	printf("\n Resultat = %d  \n",vl_Res);
	
	
	retcode = XZAG03_Deconnexion_Base();
	printf("XZAG03_Deconnexion_Base retourne %d\n",retcode);
}
