/*E*/
/*Fichier : $Id: xzan37_test.c,v 1.1 1994/11/30 12:37:31 mercier Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/30 12:37:31 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAT * FICHIER xzan37_test.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Fichier permetant de tester la fonction de xzan34.c.
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	30 Nov 1994	: Creation
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
#include "xzan37.h"
#include "xzst.h"
#include "xzsc.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xzan37_test.c,v 1.1 1994/11/30 12:37:31 mercier Exp $ : xzan37_test" ;

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
*	
* FONCTION 
*   
*
------------------------------------------------------*/
{
	
	XDY_Octet	vl_NumTypeAstreinte_in=1;
	XDY_Nom 	vl_Nom_out;
	
	int 		vl_retcode;
		
	XZST_03EcritureTrace( XZSTC_DEBUG1,"xzan37_test.x : Debut d execution");
	 
	vl_retcode = XZAG02_Connexion_Base("CI");
	printf("XZAG02_Connexion_Base retourne :%d\n",vl_retcode);
	
	/*A
	** Declenchement de XZAN37_Nom_Fonction avec la globale d initialisation a vrai
	*/
	
	if( XZAN37_Nom_Fonction (vl_NumTypeAstreinte_in,vl_Nom_out) != XDC_OK)
	{	
		XZST_03EcritureTrace( XZSTC_WARNING, "xzan37_test.x : XZAN37_Nom_Fonction() a echoue");
	}
	else
	{
		XZST_03EcritureTrace( XZSTC_DEBUG1,"xzan37_test.x : XZAN37_Nom_Fonction() a reussi");
	}
	printf("XZAN37_Nom_Fonction a renvoye pour NumTypeAstreinte =%d un nom %s\n",vl_NumTypeAstreinte_in,vl_Nom_out);
	fflush(stdout);
	
	vl_retcode = XZAG03_Deconnexion_Base();
	printf("XZAG03_Deconnexion_Base retourne %d\n",vl_retcode);
	
	XZST_03EcritureTrace( XZSTC_DEBUG1,"xzan37_test.x : Fin d execution");
}
