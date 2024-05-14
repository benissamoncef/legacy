/*E*/
/*Fichier : $Id: xzah11_test.c,v 1.1 1994/11/08 18:41:49 mercier Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/08 18:41:49 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAH01 * FICHIER xzah11_test.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Fichier permetant de tester les fonctions appelees par xzah11.c
* qui envoient des messages RTWks, et par la meme occasion
* permet de tester les callback de tacli qui sont declenchees sur
* la reception de ces messages.
* Il faut pour cela que tacli (BASEDD/bin) soit lance sur la meme
* machine que xzah11_test.x afin de s abonner aux memes data Groups.
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	31 Oct 1994	: Creation
------------------------------------------------------*/

/* fichiers inclus */

#include "xzah11.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzah11_test.c,v 1.1 1994/11/08 18:41:49 mercier Exp $ : xzah11_test" ;

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
*	SP appelees par tacli ( XZAH;01 XZAH;03 XZAH;04 XZAH;05 XZAH;08 XZAH;07 ...).
*	On teste :
*	
* FONCTION 
*   
*
------------------------------------------------------*/
{
	
	XZST_03EcritureTrace( XZSTC_DEBUG1,"xzah11_test.x : Debut d execution");
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
		XZST_03EcritureTrace( XZSTC_WARNING, "xzah11_test.x : Ne peut ouvrir la connexion avec le RTserver.\n" );
		exit( XDC_NOK );
	}
	else
	{
		/*B 
		** Affichage Trace 
		*/
		XZST_03EcritureTrace( XZSTC_DEBUG1, "xzah11_test.x : Ouverture de la connexion avec le RTserver.");
	}

	/*A
	** Declenchement de l envoi du message XDM_A_INIT_IHM sur Dg XDG_A_IHM_nomsite
	*/
	if( XZAH11_Init_IHM () != XDC_OK)
	{	
		XZST_03EcritureTrace( XZSTC_WARNING, "xzah11_test.x : XZAH11_Init_IHM () a echoue");
	}
	else
	{
		XZST_03EcritureTrace( XZSTC_DEBUG1,"xzah11_test.x : XZAH11_Init_IHM () a reussi");
	}	
	XZST_03EcritureTrace( XZSTC_DEBUG1,"xzah11_test.x : Fin d execution");
}
