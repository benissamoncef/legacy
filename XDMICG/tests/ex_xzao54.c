/*E*/
/*Fichier : $Id: ex_xzao54.c,v 1.1 1995/10/30 14:50:19 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/10/30 14:50:19 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER ex_xzao54.c
*               fait appel a xzag et xzao54 et xzao55
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  XZAG02_Connexion_Base permet de se connecter a la base AdresseC
*  du SQL_Srv de la machine passee en argument : On renseigne
*  une globale XZAGV_Connexion contenant l ID de connexion
*  Les codes de retour sont XDC_OK si tout s est bien passe 
*  sinon XDC_NOK voire le code d erreur Sybase
*  Fait appel a XZAO54_Lire_Config_Portion et XZAO55_Ajouter_Portion
*  Se deconnecte du serveur SQL
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	18 Mai 1995	: Creation
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "$Id: ex_xzao54.c,v 1.1 1995/10/30 14:50:19 volcic Exp $ : ex_xzao54" ;

/* definition de fonctions externes */


int Fct ( XZAOT_ConfPortion	va_Portion_in, XDY_ResConf  va_Resultat_out )
{

    XZAO55_Ajouter_Portion ("CFG",va_Portion_in,&va_Resultat_out );
	
   return (XDC_OK);
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
*    Ouvre une connexion a la base AdresseC sur le serveur de donnees du Site
*    Initialise la variable globale au Module XZAGV_Connexion a la connexion 
*    Sybase si tout s est bien passe sinon elle est mise a NULL.
*
------------------------------------------------------*/
{
	XDY_Basedd		va_Basedd_in;
	FILE *vl_Fd;
	XDY_ResConf  		va_Resultat_out;
	
	int retcode;

	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : main");
	XZST_03EcritureTrace(XZSTC_INFO,"IN : main");
	retcode = XZAG02_Connexion_Base( "CI");
	printf("XZAG02_Connexion_Base retourne :%d\n",retcode);
 	
	
	/*A
	** Mise en base des portions
	*/
	
	printf("\n** Mise en base des Portions **\n");
	
	vl_Fd = fopen( "/users/volcic/migrazur/fichiers/dra/portion.txt", "r" );
	
	xzao54_Lire_Config_Portion ( vl_Fd, Fct, &va_Resultat_out ); 
	
	fclose(	vl_Fd );		
	
	retcode = XZAG03_Deconnexion_Base();
	printf("XZAG03_Deconnexion_Base retourne %d\n",retcode);
}
