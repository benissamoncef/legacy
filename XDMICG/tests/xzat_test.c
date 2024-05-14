/*E*/
/*Fichier : $Id: xzat_test.c,v 1.2 1994/11/08 18:41:56 mercier Exp $      Release : $Revision: 1.2 $        Date : $Date: 1994/11/08 18:41:56 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAT * FICHIER xzat_test.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Fichier permetant de tester les fonctions de xzat.c
* qui envoient des messages RTWks, et par la meme occasion
* permet de tester les callback de tacli qui sont declenchees sur
* la reception de ces messages.
* Il faut pour cela que tacli (BASEDD/bin) soit lance sur la meme
* machine que xzat_test.x afin de s abonner aux memes data Groups.
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	11 Oct 1994	: Creation
------------------------------------------------------*/

/* fichiers inclus */

#include "xzat.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzat_test.c,v 1.2 1994/11/08 18:41:56 mercier Exp $ : xzat_test" ;

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
*	SP appelees par tacli (XZAT;22 XZAT;23 XZAT;24 XZAT;25 ).
*	On teste :
*	xzat22_CompteRendu_Tube		()
*	xzat23_CompteRendu_echangeur 	()
*	xzat24_CompteRendu_NAV 		()
*	xzat25_CompteRendu_PMV 		()
*
* FONCTION 
*   
*
------------------------------------------------------*/
{
	XDY_Mot		vl_Num_Action		=12;
	XDY_Horodate 	vl_Horodate_double	=4555.5;
	XDY_Booleen  	vl_ResultatAction	=1;
	
	XZST_03EcritureTrace( XZSTC_DEBUG1,"xzat_test.x : Debut d execution");
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
		XZST_03EcritureTrace( XZSTC_WARNING, "xzat_test.x : Ne peut ouvrir la connexion avec le RTserver.\n" );
		exit( XDC_NOK );
	}
	else
	{
		/*B 
		** Affichage Trace 
		*/
		XZST_03EcritureTrace( XZSTC_DEBUG1, "xzat_test.x : Ouverture de la connexion avec le RTserver.");
	}

	/*A
	** Declenchement de l envoi du message XDM_AT_TUBE
	*/
	if( xzat22_CompteRendu_Tube (vl_Num_Action,vl_Horodate_double,vl_ResultatAction) != XDC_OK)
	{
	
		XZST_03EcritureTrace( XZSTC_WARNING, "xzat_test.x : xzat22_CompteRendu_Tube() a echoue");
	}
	else
	{
		XZST_03EcritureTrace( XZSTC_DEBUG1,"xzat_test.x : xzat22_CompteRendu_Tube() a reussi");
	}
	
	/*A
	** Declenchement de l envoi du message XDM_AT_echangeur
	*/
	if( xzat23_CompteRendu_DemiEch (vl_Num_Action,vl_Horodate_double,vl_ResultatAction) != XDC_OK)
	{
	
		XZST_03EcritureTrace( XZSTC_WARNING, "xzat_test.x : xzat23_CompteRendu_echangeur() a echoue");
	}
	else
	{
		XZST_03EcritureTrace( XZSTC_DEBUG1,"xzat_test.x : xzat23_CompteRendu_echangeur() a reussi");
	}
	
	/*A
	** Declenchement de l envoi du message XDM_AT_NAV
	*/
	if( xzat24_CompteRendu_Nav (vl_Num_Action,vl_Horodate_double,vl_ResultatAction) != XDC_OK)
	{
	
		XZST_03EcritureTrace( XZSTC_WARNING, "xzat_test.x : xzat24_CompteRendu_NAV a echoue");
	}
	else
	{
		XZST_03EcritureTrace( XZSTC_DEBUG1,"xzat_test.x : xzat24_CompteRendu_NAV a reussi");
	}
	/*A
	** Declenchement de l envoi du message XDM_AT_PMV
	*/
	if( xzat25_CompteRendu_PMV (vl_Num_Action,vl_Horodate_double,vl_ResultatAction) != XDC_OK)
	{
	
		XZST_03EcritureTrace( XZSTC_WARNING, "xzat_test.x : xzat25_CompteRendu_PMV a echoue");
	}
	else
	{
		XZST_03EcritureTrace( XZSTC_DEBUG1,"xzat_test.x : xzat25_CompteRendu_PMV a reussi");
	}
	XZST_03EcritureTrace( XZSTC_DEBUG1,"xzat_test.x : Fin d execution");
}
