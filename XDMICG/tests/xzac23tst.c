/*E*/
/*  Fichier : 	$Id: xzac23tst.c,v 1.1 1994/11/07 20:24:57 volcic Exp $  Release : $Revision: 1.1 $ Date : $Date: 1994/11/07 20:24:57 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzactst23.c
*               fait appel a xzag et xzac23
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  XZAG02_Connexion_Base permet de se connecter a la base PRC
*  du SQL_Srv de la machine passee en argument : On renseigne
*  une globale XZAGV_Connexion contenant l ID de connexion
*  Les codes de retour sont XDC_OK si tout s est bien passe 
*  sinon XDC_NOK voire le code d erreur Sybase
*  Fait appel a XZAC23_Ajouter_Autoroute
*  Se deconnecte du serveur SQL
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	07 Nov 1994	: Creation
------------------------------------------------------*/

/* fichiers inclus */

#include <time.h>
#include "xzac.h"


static char *version = "$Id: xzac23tst.c,v 1.1 1994/11/07 20:24:57 volcic Exp $ : xzac23tst.c" ;

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  < Test xzac23.c >
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
*    Test de xzac23.c
*
------------------------------------------------------*/
{
	/* Horodate = 06/04/94 12:12:12 */
	XDY_Horodate 	Horodate = 765634332;
	XDY_Evt 	Evt;
	char		Horo[20];
	struct tm	timer;
	
	int retcode;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : main");
	XZST_03EcritureTrace(XZSTC_INFO,"IN : main");
	retcode = XZAG02_Connexion_Base( "CI");
	printf("XZAG02_Connexion_Base retourne :%d\n",retcode);
 	
	printf("\n\n**** Test 1 ****\n\n");
	
	strcpy(Horo,"06/04/1994 12:12:12");
	
	/*retcode = XZSM_13ConversionHorodateSec(Horo,&Horodate);*/
	
	Evt.numero = 1000;
	Evt.cle = 1;
	
	retcode =  XZAC23_Rech_Partenaires_Informes (Horodate,Evt,43 );
	
	printf("\n\n*********\nXZAC23_Rech_Partenaires_Informes retourne :%d\n******\n",retcode);

				
	retcode = XZAG03_Deconnexion_Base();
	printf("XZAG03_Deconnexion_Base retourne %d\n",retcode);
}
