/*E*/
/*  Fichier : 	$Id: xzag06tst.c,v 1.1 1994/11/08 18:41:47 mercier Exp $  Release : $Revision: 1.1 $ Date : $Date: 1994/11/08 18:41:47 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzag06tst.c
*               fait appel a xzag et xzac22
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  XZAG02_Connexion_Base permet de se connecter a la base PRC
*  du SQL_Srv de la machine passee en argument : On renseigne
*  une globale XZAGV_Connexion contenant l ID de connexion
*  Les codes de retour sont XDC_OK si tout s est bien passe 
*  sinon XDC_NOK voire le code d erreur Sybase
*  Fait appel a XZAG06_Purge_Base_Historique
*  Se deconnecte du serveur SQL
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	07 Nov 1994	: Creation
------------------------------------------------------*/

/* fichiers inclus */

#include "xzag06.h"


static char *version = "$Id: xzag06tst.c,v 1.1 1994/11/08 18:41:47 mercier Exp $ : xzag06tst.c" ;

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  < Test xzag06.c >
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
*    Test de xzag06.c
*
------------------------------------------------------*/
{
	/* Horodate = 06/04/94 12:12:12 */
	XDY_Horodate 	vl_Horodate = 768226332.;
	
	char		Horo[20];
	
	int vl_retcode;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : main");
	XZST_03EcritureTrace(XZSTC_INFO,"IN : main");
	vl_retcode = XZAG02_Connexion_Base( "CI");
	printf("XZAG02_Connexion_Base retourne :%d\n",vl_retcode);
 	
	printf("\n\n**** Test xzag06 ****\n\n");
	
	strcpy(Horo,"06/04/1994 12:12:12");
	
	/*vl_retcode = XZSM_13ConversionHorodateSec(Horo,&vl_Horodate);*/
	
	vl_retcode =  XZAG06_Purge_Base_Historique( vl_Horodate);
	printf("XZAG06_Purge_Base_Historique retourne %d",vl_retcode);
			
	vl_retcode = XZAG03_Deconnexion_Base();
	printf("XZAG03_Deconnexion_Base retourne %d\n",vl_retcode);
}
