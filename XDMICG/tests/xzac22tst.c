/*E*/
/*  Fichier : 	$Id: xzac22tst.c,v 1.3 1994/11/07 20:28:25 volcic Exp $  Release : $Revision: 1.3 $ Date : $Date: 1994/11/07 20:28:25 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzactst22.c
*               fait appel a xzag et xzac22
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  XZAG02_Connexion_Base permet de se connecter a la base PRC
*  du SQL_Srv de la machine passee en argument : On renseigne
*  une globale XZAGV_Connexion contenant l ID de connexion
*  Les codes de retour sont XDC_OK si tout s est bien passe 
*  sinon XDC_NOK voire le code d erreur Sybase
*  Fait appel a XZAC22_Ajouter_Autoroute
*  Se deconnecte du serveur SQL
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	07 Nov 1994	: Creation
********************************
* MERCIER	07 Nov 1994	: Rien 		V 1.2
* volcic	07 Nov 1994	: Changement Horodate (v1.3)
*
********************************
* MERCIER	07 Nov 1994	: Rien 		V 1.2
*
------------------------------------------------------*/

/* fichiers inclus */

#include "xzac.h"


static char *version = "$Id: xzac22tst.c,v 1.3 1994/11/07 20:28:25 volcic Exp $ : xzac22tst.c" ;

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  < Test xzac22.c >
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
*    Test de xzac22.c
*
------------------------------------------------------*/
{
	/* Horodate = 06/04/94 12:12:12 */
	XDY_Horodate 	Horodate = 765634332;
	XDY_Evt 	Evt;
	XDY_Booleen	va_SIS_out;
	XDY_Booleen	va_Secours_out;
	XDY_Booleen	va_Depanneur_out;
	
	int retcode;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : main");
	XZST_03EcritureTrace(XZSTC_INFO,"IN : main");
	retcode = XZAG02_Connexion_Base( "CI");
	printf("XZAG02_Connexion_Base retourne :%d\n",retcode);
 	
	printf("\n\n**** Test 1 ****\n\n");
	
	Evt.numero = 1000;
	Evt.cle = 1;
	
	retcode =  XZAC22_Rech_Intervenants_Sur_Place (Horodate,Evt,&va_SIS_out,&va_Secours_out,&va_Depanneur_out );
	
	printf("\n\n*********\nXZAC22_Rech_Intervenants_Sur_Place retourne :%d\n******\n",retcode);

	printf("\nva_SIS_out_out = %d\nva_Secours_out = %d\nva_Depanneur_out = %d \n",va_SIS_out,va_Secours_out,va_Depanneur_out);
				
	retcode = XZAG03_Deconnexion_Base();
	printf("XZAG03_Deconnexion_Base retourne %d\n",retcode);
}
