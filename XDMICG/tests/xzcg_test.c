/*E*/
/*Fichier : $Id: xzcg_test.c,v 1.3 1994/12/26 16:50:36 mercier Exp $      Release : $Revision: 1.3 $        Date : $Date: 1994/12/26 16:50:36 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAT * FICHIER xzcg_test.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Fichier permetant de tester les fonctions de xzcg.c
* qui envoient des messages RTWks, et par la meme occasion
* permet de tester les callback de tcgcd qui sont declenchees sur
* la reception de ces messages.
* Il faut pour cela que tcgcd (GECONF/bin) soit lance sur la meme
* machine que xzcg_test.x afin de s abonner aux memes data Groups.
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	13 Oct 1994	: Creation
------------------------------------------------------*/

/* fichiers inclus */

#include "xzcg.h"
#include "cgcd_vce.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzcg_test.c,v 1.3 1994/12/26 16:50:36 mercier Exp $ : xzcg_test" ;

/* declaration de fonctions internes */
int xzcg_test_Fctutil_Affiche(XZCGT_ComposantBL , int );
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
*	RTServeur doit etre lance sur la machine CI,
* 	le programme de test doit etre connu dans les fichiers
*	d administration de SUPERV.
*
*	Pour tester le declanchement des CallBacks sur l arrivee des
*	messages, il faut que la tache tcgcd (GECONF/bin) soit connue dans
*	les fichiers d administration de SUPERV et qu elle soit lancee sur
* 	la meme machine (SITE CI) que le prog de test.
*	On teste :
*	xzcg01_Livraison 		() 
*	xzcg03_Diffusion_Fichiers 	()
*
* FONCTION 
*   
*
------------------------------------------------------*/
{
	XDY_Mot		vl_Num_Action		=12;
	XDY_Horodate 	vl_Horodate_double	=4555.5;
	XDY_Booleen  	vl_ResultatAction	=1;
	XDY_NomSite     pl_NomSite		="";
	int            	vl_ValRet 		= XDC_OK;
	int		vl_Nbre_Elt_Fich_BL	=0;
	
	/*A
	** Connexion au RTserver et initialisation des variables
	*/
	if( XZSC_01InitCnxSup(  argv[ 1 ],argv[ 0 ],
				XZSCC_INTERDIRE,XZSCC_AUTORISER,XZSCC_INTERDIRE,NULL,
				"/dev/null" ) != XDC_OK )
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "xzcg_test.x : Ne peut ouvrir la connexion avec le RTserver.\n" );
		exit( XDC_NOK );
	}
	else 
	{
		XZST_03EcritureTrace( XZSTC_DEBUG1, "xzcg_test.x : Ouverture de la connexion avec le RTserver.");
	}	
	
	XZST_03EcritureTrace( XZSTC_DEBUG1,"xzcg_test.x : Debut d execution");
	
	/*A
	** Declenchement de l envoi du message XDM_CGD_LIV
	*/
	if( xzcg01_Livraison () != XDC_OK)
	{
	
		XZST_03EcritureTrace( XZSTC_WARNING, "xzcg_test.x : xzcg01_Livraison() a echoue");
	}
	else
	{
		XZST_03EcritureTrace( XZSTC_DEBUG1,"xzcg_test.x : xzcg01_Livraison() a reussi");
	}
	/*A
	** Envoie du nom de la machine sur la quelle on tourne afin de recevoir les
	** messages destines a l administrateur.
	*/
	if( xzcg07_EnvoieNomMachineLocal () != XDC_OK)
	{
	
		XZST_03EcritureTrace( XZSTC_WARNING, "xzcg_test.x : xzcg07_EnvoieNomMachineLocal() a echoue");
	}
	else
	{
		XZST_03EcritureTrace( XZSTC_DEBUG1,"xzcg_test.x : xzcg07_EnvoieNomMachineLocal() a reussi");
	}
	/*A
	** Declenchement de la sauvegarde de CFG sur bande.
	*/
/*	if( xzcg06_DuplicationBD (XZCGC_SAUVEGARDE_CFG) != XDC_OK)
	{
	
		XZST_03EcritureTrace( XZSTC_WARNING, "xzcg_test.x :xzcg06_DuplicationBD (SAUVEGARDE_CFG) a echoue");
	}
	else
	{
		XZST_03EcritureTrace( XZSTC_DEBUG1,"xzcg_test.x :xzcg06_DuplicationBD (SAUVEGARDE_CFG) a reussi");
	}
*/

	if( xzcg06_DuplicationBD (XZCGC_RESTAURATION_DANS_CFT) != XDC_OK)
	{
	
		XZST_03EcritureTrace( XZSTC_WARNING, "xzcg_test.x :xzcg06_DuplicationBD (SAUVEGARDE_CFG) a echoue");
	}
	else
	{
		XZST_03EcritureTrace( XZSTC_DEBUG1,"xzcg_test.x :xzcg06_DuplicationBD (SAUVEGARDE_CFG) a reussi");
	}

	/*A
	** Inhibe la verification checklist
	*/
	if(xzcg_ActivChecklist(XDC_NOK)!= XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "xzcg_test.x : xzcg_ActivChecklist : XDC_NOK est sortie en erreur");
	}
	else
	{
		XZST_03EcritureTrace( XZSTC_DEBUG1,"xzcg_test.x :xzcg_ActivChecklist (XDC_NOK) a reussi");
	}
	/*A
	** Declenchement de la restauration de CFG a partir de la bande sur la base CFT.
	** Cette etape est necessaire avant de pouvoir declencher une verification de coherence sur la base.
	*/
	/*if( xzcg06_DuplicationBD (XZCGC_RESTAURATION_DANS_CFT) != XDC_OK)
	{
	
		XZST_03EcritureTrace( XZSTC_WARNING, "xzcg_test.x :xzcg06_DuplicationBD (RESTAURATION_DANS_CFT) a echoue");
	}
	else
	{
		XZST_03EcritureTrace( XZSTC_DEBUG1,"xzcg_test.x :xzcg06_DuplicationBD (RESTAURATION_DANS_CFT) a reussi");
	}*/
	/*A
	** Declenchement de l envoi du message XDM_CGD_DIF
	*/

	if( xzcg03_Diffusion_Fichiers () != XDC_OK)
	{
	
		XZST_03EcritureTrace( XZSTC_WARNING, "xzcg_test.x : xzcg03_Diffusion_Fichiers() a echoue");
	}
	else
	{
		XZST_03EcritureTrace( XZSTC_DEBUG1,"xzcg_test.x : xzcg03_Diffusion_Fichiers() a reussi");
	}



/*	if( xzcg02_Lecture_BL (xzcg_test_Fctutil_Affiche,&vl_Nbre_Elt_Fich_BL) != XDC_OK)
	{
	
		XZST_03EcritureTrace( XZSTC_WARNING, "xzcg_test.x : xzcg02_Lecture_BL () a echoue");
	}
	else
	{
		XZST_03EcritureTrace( XZSTC_DEBUG1,"xzcg_test.x :xzcg02_Lecture_BL () a reussi");
	}
	
*/	
	/*if(xzcg04_Verification_Coherence(CVCEC_VERIF_SGBD) != XDC_OK)
	{
	
		XZST_03EcritureTrace( XZSTC_WARNING, "xzcg_test.x : xzcg04_Verification_Coherence () a echoue");
	}
	else
	{
		XZST_03EcritureTrace( XZSTC_DEBUG1,"xzcg_test.x : xzcg04_Verification_Coherence() a reussi");
	}*/
	
	XZST_03EcritureTrace( XZSTC_DEBUG1,"xzcg_test.x : Fin d execution");
}

/*X*/
/*
* Ceci est une fonction de test a passer a la fonction de lecture de BL.
*/
int xzcg_test_Fctutil_Affiche(XZCGT_ComposantBL va_Elt_BL, int va_Num_Enr)

{
	XZST_03EcritureTrace( XZSTC_FONCTION,"IN : xzcg_test_Fctutil_Affiche : debut d execution");
	
	XZST_03EcritureTrace( XZSTC_INFO,"xzcg_test_Fctutil_Affiche a recu l element %d: \n TypeFic = %s\nNomMachine = %s\nNomFichier = %s\nNumVersion = %s\nHorodate = %lf",
							va_Num_Enr,
							va_Elt_BL.TypeFic,
							va_Elt_BL.NomMachine,
							va_Elt_BL.NomFichier,
							va_Elt_BL.NumVersion,
							va_Elt_BL.Horodate);	
			
	XZST_03EcritureTrace( XZSTC_FONCTION,"OUT : xzcg_test_Fctutil_Affiche : Fin d execution");
	return(XDC_OK); 
}

