/*E*/
/*Fichier : @(#)test_xzdd02.c	1.1      Release : 1.1        Date : 09/29/94
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  XDMICG
------------------------------------------------------
* MODULE TEST * FICHIER test_xzdd02.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   cf test_xzdd02.h
*
------------------------------------------------------
* HISTORIQUE :
*
* IT	09 Sep 1994	: Creation
------------------------------------------------------*/

/* fichiers inclus */
#include <rtworks/common.h>
#include <rtworks/ipc.h>
#include "xdy.h"
#include "xdc.h"
#include "xzdd.h"
#include "xzsc.h"
#include "xzst.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */
static char *version = "@(#)test_xzdd02.c	1.1 09/29/94 : test_xzdd02" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */




extern int main(argc, argv)
int argc ;
char **argv ;
{
	XDY_Horodate    Horodate=0;
	XDY_Eqt         PosteOperateur=2;
	XDY_Evt		NumEvt;
	XDY_Octet       TypeEvt=5;
	XDY_Booleen     TypePA=XZDDC_PA_AVEC_TFM;
	XDY_Octet	Mode=XZDDC_MODE_TEST;
	XDY_Octet	code_retour;

	XZST_03EcritureTrace(XZSTC_INFO, "IN : main test_xzdd02");

	NumEvt.numero = 3;
	NumEvt.cle = 4;

	/* connexion au noyau de supervision pour que les types de messages soient initialises */
	code_retour = XZSC_01InitCnxSup(XDC_NOM_APPLICATION,    /* nom de l'application */
		argv[0],                /* nom de la tache */
		XZSCC_INTERDIRE,        /* autoriser time */
		XZSCC_AUTORISER,        /* autoriser trace */
		XZSCC_INTERDIRE,        /* autoriser synchro */
		NULL,                   /* fonction de sortie */
		"/dev/console"          /* redirection de stdout et stderr */
		);

	XZST_03EcritureTrace(XZSTC_INFO, "XZSC_01InitCnxSup retourne %d", code_retour);

	/* envoi d'un message */
	XZST_03EcritureTrace(XZSTC_INFO,
		"appel de XZDD02 avec %lf %d %d %d %d %d %d",
		Horodate, PosteOperateur, NumEvt.numero, NumEvt.cle, TypeEvt, TypePA, Mode);

	code_retour = XZDD02_CalculerPA(Horodate,
			PosteOperateur,
			NumEvt,
			TypeEvt,
			TypePA,
			Mode);
	
	XZST_03EcritureTrace(XZSTC_INFO, "XZDD02_Calculer PA retourne %d", code_retour);
	XZST_03EcritureTrace(XZSTC_INFO, "OUT : main test_xzdd02");

	exit(0);
}
