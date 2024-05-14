/*E*/
/*  Fichier : 	$Id: xzat07tst.c,v 1.1 1994/11/08 10:12:35 volcic Exp $  Release : $Revision: 1.1 $ Date : $Date: 1994/11/08 10:12:35 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAT * FICHIER xzattst07.c
*               fait appel a xzag et xzat07
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  XZAG02_Connexion_Base permet de se connecter a la base PRC
*  du SQL_Srv de la machine passee en argument : On renseigne
*  une globale XZAGV_Connexion contenant l ID de connexion
*  Les codes de retour sont XDC_OK si tout s est bien passe 
*  sinon XDC_NOK voire le code d erreur Sybase
*  Fait appel a XZAT07_Ajouter_Autoroute
*  Se deconnecte du serveur SQL
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	07 Nov 1994	: Creation
*
------------------------------------------------------*/

/* fichiers inclus */

#include "xzat.h"


static char *version = "$Id: xzat07tst.c,v 1.1 1994/11/08 10:12:35 volcic Exp $ : xzat07tst.c" ;

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  < Test xzat07.c >
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
*    Test de xzat07.c
*
------------------------------------------------------*/
{
	/* Horodate = 07/04/94 12:12:12 */
	XDY_Horodate 	Horodate = 765720732;
	XDY_Mot		va_NumEqt_in = 2;
	XDY_Octet 	va_DispoTube_out;
	XDY_Octet	va_Sequence_out;
	XDY_Entier 	va_NumEvt_out;
	XDY_Octet 	va_CleEvt_out;
	XDY_Octet 	va_Priorite_out;
	XDY_Entier 	va_DistanceEvt_out;
	
	int retcode;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : main");
	XZST_03EcritureTrace(XZSTC_INFO,"IN : main");
	retcode = XZAG02_Connexion_Base( "CI");
	printf("XZAG02_Connexion_Base retourne :%d\n",retcode);
 	
	printf("\n\n**** Test 1 ****\n\n");
	
	retcode =  XZAT07_Utilisation_Tunnel  ( 	Horodate,
				va_NumEqt_in,
				&va_DispoTube_out,
				&va_Sequence_out,
				&va_NumEvt_out,
				&va_CleEvt_out,
				&va_Priorite_out,
				&va_DistanceEvt_out );
				
	printf("\n\n*********\nXZAT07_Utilisation_Tunnel retourne :%d\n******\n",retcode);

	printf("\n\nDispoTube = %d\nSequence = %d\nNumEvt = %d\nCleEvt = %d\nPriorite = %d\nDistanceEvt = %d\n",
				va_DispoTube_out,
				va_Sequence_out,
				va_NumEvt_out,
				va_CleEvt_out,
				va_Priorite_out,
				va_DistanceEvt_out );
	
	retcode = XZAG03_Deconnexion_Base();
	printf("XZAG03_Deconnexion_Base retourne %d\n",retcode);
}
