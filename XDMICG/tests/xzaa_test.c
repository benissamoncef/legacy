/*E*/
/*Fichier : $Id: xzaa_test.c,v 1.1 1994/11/08 18:41:43 mercier Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/08 18:41:43 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAT * FICHIER xzaa_test.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Fichier permetant de tester les fonctions de xzat.c
* qui envoient des messages RTWks, et par la meme occasion
* permet de tester les callback de tacli qui sont declenchees sur
* la reception de ces messages.
* Il faut pour cela que tacli (BASEDD/bin) soit lance sur la meme
* machine que xzaa_test.x afin de s abonner aux memes data Groups.
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	11 Oct 1994	: Creation
------------------------------------------------------*/

/* fichiers inclus */

#include "xzaa.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzaa_test.c,v 1.1 1994/11/08 18:41:43 mercier Exp $ : xzaa_test" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void  main(int argc, char *argv[])

/*
* ARGUMENTS EN ENTREE : Aucun
*
*
* CONDITION D'UTILISATION
*	RTServeur doit etre lance sur la machine en local,
* 	le programme de test doit etre connu dans les fichiers
*	d administration de SUPERV.
*
*	Pour tester le declanchement des CallBacks sur l arrivee des
*	messages, il faut que la tache tacli (BASEDD/bin) soit connue dans
*	les fichiers d administration de SUPERV et qu elle soit lancee sue
* 	la meme machine (SITE) que le prog de test.
*	De plus le SQL serveur doit etre lance sur le site et connaissent les
*	SP appelees par tacli ( XZAA;01 ).
*	On teste :
*	xzaa01_Creer_Alerte (XDY_Horodate,XDY_TypeEqt,XDY_Eqt,XDY_TypeAlerte,XDY_TexteAlerte)
*
* FONCTION 
*   
*
------------------------------------------------------*/
{
	
	XDY_Horodate 	vl_Horodate_double	=4555.5;
	XDY_TypeEqt	vl_TypeEqt		=1;
	XDY_Eqt		vl_Eqt			=10;
	XDY_TypeAlerte	vl_TypeAlerte		=2;
	XDY_TexteAlerte vl_TexteAlerte		;
	
	strcpy(vl_TexteAlerte,"HELLO WORLD");
	
	XZST_03EcritureTrace( XZSTC_DEBUG1,"xzaa_test.x : Debut d execution");
	/*A
	** Connexion au RTserver et initialisation des variables
	*/
	if( XZSC_01InitCnxSup(  argv[ 1 ],argv[ 0 ],
				XZSCC_INTERDIRE,XZSCC_AUTORISER,XZSCC_INTERDIRE,NULL,
				"/dev/null" ) != XDC_OK )
	{
		/*B
		** Affichage Trace 
		*/
		XZST_03EcritureTrace( XZSTC_WARNING, "xzaa_test.x : Ne peut ouvrir la connexion avec le RTserver.\n" );
		exit( XDC_NOK );
	}
	else
	{
		/*B 
		** Affichage Trace 
		*/
		XZST_03EcritureTrace( XZSTC_DEBUG1, "xzaa_test.x : Ouverture de la connexion avec le RTserver.");
	}

	/*A
	** Declenchement de l envoi du message XDM_AA_alerte
	*/
	if( xzaa01_Creer_Alerte (vl_Horodate_double,vl_TypeEqt,vl_Eqt,vl_TypeAlerte,vl_TexteAlerte) != XDC_OK)
	{	
		XZST_03EcritureTrace( XZSTC_WARNING, "xzaa_test.x : xzaa01_Creer_Alerte() a echoue");
	}
	else
	{
		XZST_03EcritureTrace( XZSTC_DEBUG1,"xzaa_test.x : xzaa01_Creer_Alerte() a reussi");
	}	
	XZST_03EcritureTrace( XZSTC_DEBUG1,"xzaa_test.x : Fin d execution");
}
