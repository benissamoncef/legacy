/*E*/
/*  Fichier : 	$Id: xzag05tst.c,v 1.1 1994/11/08 18:41:45 mercier Exp $  Release : $Revision: 1.1 $ Date : $Date: 1994/11/08 18:41:45 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzag05tst.c
*               fait appel a xzag02 ,xzag05 et xzag03
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  XZAG02_Connexion_Base permet de se connecter a la base PRC
*  du SQL_Srv de la machine passee en argument : On renseigne
*  une globale XZAGV_Connexion contenant l ID de connexion
*  Les codes de retour sont XDC_OK si tout s est bien passe 
*  sinon XDC_NOK voire le code d erreur Sybase
*  Fait appel a XZAG05_Purge_Base_Exploitation
*  Se deconnecte du serveur SQL
*
------------------------------------------------------
* HISTORIQUE :
*
* MERCIER	07 Nov 1994	: Creation
------------------------------------------------------*/

/* fichiers inclus */

#include "xzag05.h"


static char *version = "$Id: xzag05tst.c,v 1.1 1994/11/08 18:41:45 mercier Exp $ : xzag05tst.c" ;

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  < Test xzag05.c >
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
*    Test de xzag05.c
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
 	
	printf("\n\n**** Test 1 ****\n\n");
	
	strcpy(Horo,"06/04/1994 12:12:12");
	
	/*vl_retcode = XZSM_13ConversionHorodateSec(Horo,&vl_Horodate);*/
	
	vl_retcode =  XZAG05_Purge_Base_Exploitation( vl_Horodate);
	printf("XZAG05_Purge_Base_Exploitation retourne %d",vl_retcode);
			
	vl_retcode = XZAG03_Deconnexion_Base();
	printf("XZAG03_Deconnexion_Base retourne %d\n",vl_retcode);
}
