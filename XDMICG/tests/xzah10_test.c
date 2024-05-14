/*E*/
/*Fichier : $Id: xzah10_test.c,v 1.1 1994/11/08 18:35:15 mercier Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/08 18:35:15 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAT * FICHIER xzah10_test.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Fichier permetant de tester les fonctions de xzat.c
* qui envoient des messages RTWks, et par la meme occasion
* permet de tester les callback de tacli qui sont declenchees sur
* la reception de ces messages.
* Il faut pour cela que tacli (BASEDD/bin) soit lance sur la meme
* machine que xzah10_test.x afin de s abonner aux memes data Groups.
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	03 Nov 1994	: Creation
------------------------------------------------------*/

/* fichiers inclus */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include "ctpublic.h"
#include "cstypes.h"
#include "xdc.h"
#include "xdy.h"
#include "xzag.h"
#include "xzah10.h"
#include "xzst.h"
#include "xzsc.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzah10_test.c,v 1.1 1994/11/08 18:35:15 mercier Exp $ : xzah10_test" ;

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
*	SQL serveur doit etre lance.
*	
*	On teste sur SDCI1.
*	
* FONCTION 
*   
*
------------------------------------------------------*/
{
	
	int retcode;	
	XZST_03EcritureTrace( XZSTC_DEBUG1,"xzah10_test.x : Debut d execution");
	
	retcode = XZAG02_Connexion_Base( "CI");
	printf("XZAG02_Connexion_Base retourne :%d\n",retcode);
	
	XZAH10G_Init = XDC_VRAI;
	
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
		XZST_03EcritureTrace( XZSTC_WARNING, "xzah10_test.x : Ne peut ouvrir la connexion avec le RTserver.\n" );
		exit( XDC_NOK );
	}
	else
	{
		/*B 
		** Affichage Trace 
		*/
		XZST_03EcritureTrace( XZSTC_DEBUG1, "xzah10_test.x : Ouverture de la connexion avec le RTserver.");
	}

	/*A
	** Declenchement de XZAH10_Animer_IHM avec la globale d initialisation a vrai
	*/
	if( XZAH10_Animer_IHM () != XDC_OK)
	{	
		XZST_03EcritureTrace( XZSTC_WARNING, "xzah10_test.x : XZAH10_Animer_IHM() a echoue");
	}
	else
	{
		XZST_03EcritureTrace( XZSTC_DEBUG1,"xzah10_test.x : XZAH10_Animer_IHM() a reussi");
	}
	
	XZAH10G_Init = XDC_FAUX;
	/*A
	** Declenchement de XZAH10_Animer_IHM avec la globale d initialisation a FAUX
	*/
	if( XZAH10_Animer_IHM () != XDC_OK)
	{	
		XZST_03EcritureTrace( XZSTC_WARNING, "xzah10_test.x : XZAH10_Animer_IHM() a echoue");
	}
	else
	{
		XZST_03EcritureTrace( XZSTC_DEBUG1,"xzah10_test.x : XZAH10_Animer_IHM() a reussi");
	}
	
	
	
	
	retcode = XZAG03_Deconnexion_Base();
	printf("XZAG03_Deconnexion_Base retourne %d\n",retcode);
	
	XZST_03EcritureTrace( XZSTC_DEBUG1,"xzah10_test.x : Fin d execution");
}
