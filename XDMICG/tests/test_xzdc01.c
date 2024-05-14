/*E*/
/*Fichier : @(#)test_xzdd02.c	1.1      Release : 1.1        Date : 09/29/94
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  XDMICG
------------------------------------------------------
* MODULE TEST * FICHIER test_xzdc01.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   cf test_xzdc01.h
*
------------------------------------------------------
* HISTORIQUE :
*
* IT	09 Sep 1994	: Creation
------------------------------------------------------*/

/* fichiers inclus */
#include "xdc.h"
#include "xdf.h"
#include "xdy.h"
#include "xzdc.h"
#include "xzan36.h"
#include "xzst.h"
#include "xzss.h"
#include "xzag.h"
#include "xzan36.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */
static char *version = "@(#)test_xzdc01.c	1.1 09/29/94 : test_xzdc01" ;
static XDY_Basedd 	vg_BaseVerifiee;
static XDY_Booleen	vg_Coherence;
static char		*vg_Explication;

/* declaration de fonctions internes */

/* definition de fonctions externes */







static void Fct(NomTypeAstreinte, NumTypeAstreinte)
XDY_Nom		NomTypeAstreinte;
XDY_Octet	NumTypeAstreinte;
{
	XDY_Booleen	Existence=TRUE;
	int		cr;
	char		Explication[255];

	XZST_03EcritureTrace(XZSTC_FONCTION,
		"IN : FctU, NomTypeAstreinte = %s, NumTypeAstreinte = %d",
		NomTypeAstreinte, NumTypeAstreinte);

	/*A on appelle XZAN36 */
	cr = XZAN36_Existence_Astreinte(vg_BaseVerifiee, NumTypeAstreinte, &Existence);
	
	/*A si le type est inconnu ou n'existe pas en base */
	if ( (cr != XDC_OK) || (Existence != TRUE) )
	{
		XZST_03EcritureTrace(XZSTC_WARNING,
			"Le type d'astreinte %d (%s) n'existe pas dans la base de donnees %s",
			NumTypeAstreinte, NomTypeAstreinte, vg_BaseVerifiee);
	
		/*B on positionne vg_Coherence a FALSE */
		vg_Coherence = FALSE;
	
		/*B on construit l'explication */
		sprintf(Explication,
			"Le type d'astreinte %d (%s) est utilise dans la base de connaissances.\n",
			NumTypeAstreinte, NomTypeAstreinte);
		vg_Explication = realloc(vg_Explication, strlen(vg_Explication) + strlen(Explication) + 2);
		strcat(vg_Explication, Explication);
	}

	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : FctU");
}



extern int main(argc, argv)
int argc ;
char **argv ;
{
	char		NomFichier[80];
	FILE		*Fd;
	int		NbEnrg;
	int		cr;

	XZST_03EcritureTrace(XZSTC_INFO, "IN : main test_xzdc01");

	/* ouverture du fichier des types d'astreintes */
	/*strcpy(NomFichier, XDC_PATH_BDC_COURANTE);
	strcat(NomFichier, "/");
	strcat(NomFichier, XDF_ASTREINTES_BDC);*/
	strcpy(NomFichier, XDF_ASTREINTES_BDC);
	Fd = fopen(NomFichier, "r");
	if (Fd == NULL)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "erreur en ouverture du fichier %s", NomFichier);
		exit(0);
	}
	XZST_03EcritureTrace(XZSTC_INFO, "fichier ouvert en lecture");

	/* connexion a la base de donnees */
	strcpy(vg_BaseVerifiee, "CI");
	if (XZAG02_Connexion_Base(vg_BaseVerifiee) != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,
			"erreur lors de la connexion a la base de donnees %s", vg_BaseVerifiee);
		exit(0);
	}
	XZST_03EcritureTrace(XZSTC_INFO, "connexion a la base de donnees %s", vg_BaseVerifiee);

	/* appel du service */
	vg_Coherence = TRUE;
	vg_Explication = NULL;
	XZST_03EcritureTrace(XZSTC_INFO, "appel de XZDC01");
	cr = XZDC01_LireAstreintesSAD(Fd, Fct, &NbEnrg);
	if ( cr != XDC_OK )
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "erreur dans la verification du fichier %s", NomFichier);
		exit(0);
	}
	XZST_03EcritureTrace(XZSTC_INFO, "%d enregistrements lus", NbEnrg);
	
	/* exploitation du resultat */
	if ( vg_Coherence == FALSE )
		XZST_03EcritureTrace(XZSTC_INFO, "MODIFICATION REFUSEE : \n%s", vg_Explication);
	else
		XZST_03EcritureTrace(XZSTC_INFO, "MODIFICATION ACCEPTEE");

	/* sortie */
	XZST_03EcritureTrace(XZSTC_INFO, "OUT : main test_xzdc01");
	free(vg_Explication);
	XZAG03_Deconnexion_Base();
	fclose(Fd);
	exit(0);
}





