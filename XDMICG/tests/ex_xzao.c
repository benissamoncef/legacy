/*E*/
/* Fichier : $Id: ex_xzao.c,v 1.1 2013/02/12 15:02:43 devgfi Exp $        Release : $Revision: 1.1 $        Date : $Date: 2013/02/12 15:02:43 $
------------------------------------------------------------------------------
* ESCOTA  *  PROJET MIGRAZUR
------------------------------------------------------------------------------
* SOUS-SYSTEME  XDMICG
------------------------------------------------------------------------------
* MODULE XZAO * FICHIER ex_xzao.c
*   fait appel a xzag et xzao25, xzao26, xzao27, xzao28
------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Se connecte a la base PRC du SQL_Serveur du CI.
*
*  Teste les fonctions ssuivantes :
*    XZAO25_Nom_Autoroute
*    XZAO26_Numero_Autoroute
*    XZAO27_Ordre_PR
*    XZAO28_Autoroute
*
*  Se deconnecte du serveur SQL
*
------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	12/02/13 : Creation  1.1
----------------------------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"


/* definition de variables globales */

static char *version = "$Id: ex_xzao.c,v 1.1 2013/02/12 15:02:43 devgfi Exp $ : ex_xzao.c" ;


/* definition de fonctions internes */



/*X---------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Test de la fonction XZAO25_Nom_Autoroute.
*
------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static void Test_XZAO25 ()

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
*
----------------------------------------------------------------------------*/
{
	XDY_Autoroute	 vl_NumAuto;
	XDY_NomAuto	 vl_NomAuto;
	int		 vl_cr;


	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : Test_XZAO25");

	printf ("\n** Test de la procedure XZAO25_Nom_Autoroute **\n");


	/* Cas normal */
	vl_NumAuto = 4;
	vl_cr = XZAO25_Nom_Autoroute ( vl_NumAuto, vl_NomAuto );
	printf("\n\n*****\nCR=%d - XZAO25_Nom_Autoroute %d = <%s>\n*****\n", vl_cr, vl_NumAuto, vl_NomAuto);
	XZST_03EcritureTrace (XZSTC_INFO, "CR=%d - XZAO25_Nom_Autoroute %d = <%s>", vl_cr, vl_NumAuto, vl_NomAuto);


	/* Cas hors reseau ESCOTA */
	vl_NumAuto = 10;
	vl_cr = XZAO25_Nom_Autoroute ( vl_NumAuto, vl_NomAuto );
	printf("\n\n*****\nCR=%d - XZAO25_Nom_Autoroute %d = <%s>\n*****\n", vl_cr, vl_NumAuto, vl_NomAuto);
	XZST_03EcritureTrace (XZSTC_INFO, "CR=%d - XZAO25_Nom_Autoroute %d = <%s>", vl_cr, vl_NumAuto, vl_NomAuto);


	/* Cas inconnue */
	vl_NumAuto = 15;
	vl_cr = XZAO25_Nom_Autoroute ( vl_NumAuto, vl_NomAuto );
	printf("\n\n*****\nCR=%d - XZAO25_Nom_Autoroute %d = <%s>\n*****\n", vl_cr, vl_NumAuto, vl_NomAuto);
	XZST_03EcritureTrace (XZSTC_INFO, "CR=%d - XZAO25_Nom_Autoroute %d = <%s>", vl_cr, vl_NumAuto, vl_NomAuto);


	/* Cas invalide */
	vl_NumAuto = XDC_AUT_INCONNUE;
	vl_cr = XZAO25_Nom_Autoroute ( vl_NumAuto, vl_NomAuto );
	printf("\n\n*****\nCR=%d - XZAO25_Nom_Autoroute %d = <%s>\n*****\n", vl_cr, vl_NumAuto, vl_NomAuto);
	XZST_03EcritureTrace (XZSTC_INFO, "CR=%d - XZAO25_Nom_Autoroute %d = <%s>", vl_cr, vl_NumAuto, vl_NomAuto);


	/* Cas normal */
	vl_NumAuto = 1;
	vl_cr = XZAO25_Nom_Autoroute ( vl_NumAuto, vl_NomAuto );
	printf("\n\n*****\nCR=%d - XZAO25_Nom_Autoroute %d = <%s>\n*****\n", vl_cr, vl_NumAuto, vl_NomAuto);
	XZST_03EcritureTrace (XZSTC_INFO, "CR=%d - XZAO25_Nom_Autoroute %d = <%s>", vl_cr, vl_NumAuto, vl_NomAuto);
}




/*X---------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Test de la fonction XZAO26_Numero_Autoroute.
*
------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static void Test_XZAO26 ()

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
*
----------------------------------------------------------------------------*/
{
	XDY_Autoroute	 vl_NumAuto;
	XDY_NomAuto	 vl_NomAuto;


	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : Test_XZAO26");

	printf ("\n** Test de la procedure XZAO26_Numero_Autoroute **\n");


	/* Cas normal */
	strcpy (vl_NomAuto, "A57");
	vl_NumAuto = XZAO26_Numero_Autoroute ( vl_NomAuto );
	printf("\n\n*****\nXZAO26_Numero_Autoroute %s retourne le numero : %d\n*****\n", vl_NomAuto, vl_NumAuto);
	XZST_03EcritureTrace (XZSTC_INFO, "XZAO26_Numero_Autoroute %s = %d", vl_NomAuto, vl_NumAuto );


	/* Cas normal */
	strcpy (vl_NomAuto, "A8");
	vl_NumAuto = XZAO26_Numero_Autoroute ( vl_NomAuto );
	printf("\n\n*****\nXZAO26_Numero_Autoroute %s retourne le numero : %d\n*****\n", vl_NomAuto, vl_NumAuto);
	XZST_03EcritureTrace (XZSTC_INFO, "XZAO26_Numero_Autoroute %s = %d", vl_NomAuto, vl_NumAuto );


	/* Cas non value */
	strcpy (vl_NomAuto, "");
	vl_NumAuto = XZAO26_Numero_Autoroute ( vl_NomAuto );
	printf("\n\n*****\nXZAO26_Numero_Autoroute %s retourne le numero : %d\n*****\n", vl_NomAuto, vl_NumAuto);
	XZST_03EcritureTrace (XZSTC_INFO, "XZAO26_Numero_Autoroute %s = %d", vl_NomAuto, vl_NumAuto );


	/* Cas invalide */
	strcpy (vl_NomAuto, "NULL");
	vl_NumAuto = XZAO26_Numero_Autoroute ( NULL );
	printf("\n\n*****\nXZAO26_Numero_Autoroute %s retourne le numero : %d\n*****\n", vl_NomAuto, vl_NumAuto);
	XZST_03EcritureTrace (XZSTC_INFO, "XZAO26_Numero_Autoroute %s = %d", vl_NomAuto, vl_NumAuto );


	/* Cas normal */
	strcpy (vl_NomAuto, "A570");
	vl_NumAuto = XZAO26_Numero_Autoroute ( "A570" );
	printf("\n\n*****\nXZAO26_Numero_Autoroute %s retourne le numero : %d\n*****\n", vl_NomAuto, vl_NumAuto);
	XZST_03EcritureTrace (XZSTC_INFO, "XZAO26_Numero_Autoroute %s = %d", vl_NomAuto, vl_NumAuto );
}




/*X---------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Test de la fonction XZAO27_Ordre_PR.
*
------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static void Test_XZAO27 ()

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
*
----------------------------------------------------------------------------*/
{
	XDY_Autoroute	vl_NumAuto;
	XDY_Mot		vl_ordre_Pr;


	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : Test_XZAO27");

	printf ("\n** Test de la procedure XZAO27_Ordre_PR **\n");


	/* Cas normal */
	vl_NumAuto = 1;
	vl_ordre_Pr = XZAO27_Ordre_PR ( vl_NumAuto );
	printf("\n\n*****\nXZAO27_Ordre_PR Autoroute %d = %d\n*****\n", vl_NumAuto, vl_ordre_Pr);
	XZST_03EcritureTrace (XZSTC_INFO, "XZAO27_Ordre_PR Autoroute %d = %d", vl_NumAuto, vl_ordre_Pr);


	/* Cas invalide */
	vl_NumAuto = 13;
	vl_ordre_Pr = XZAO27_Ordre_PR ( vl_NumAuto );
	printf("\n\n*****\nXZAO27_Ordre_PR Autoroute %d = %d\n*****\n", vl_NumAuto, vl_ordre_Pr);
	XZST_03EcritureTrace (XZSTC_INFO, "XZAO27_Ordre_PR Autoroute %d = %d", vl_NumAuto, vl_ordre_Pr);


	/* Cas non value */
	vl_NumAuto = XDC_AUT_INCONNUE;
	vl_ordre_Pr = XZAO27_Ordre_PR ( vl_NumAuto );
	printf("\n\n*****\nXZAO27_Ordre_PR Autoroute %d = %d\n*****\n", vl_NumAuto, vl_ordre_Pr);
	XZST_03EcritureTrace (XZSTC_INFO, "XZAO27_Ordre_PR Autoroute %d = %d", vl_NumAuto, vl_ordre_Pr);


	/* Cas normal, PR inverses */
	vl_NumAuto = 8;
	vl_ordre_Pr = XZAO27_Ordre_PR ( vl_NumAuto );
	printf("\n\n*****\nXZAO27_Ordre_PR Autoroute %d = %d\n*****\n", vl_NumAuto, vl_ordre_Pr);
	XZST_03EcritureTrace (XZSTC_INFO, "XZAO27_Ordre_PR Autoroute %d = %d", vl_NumAuto, vl_ordre_Pr);


	/* Cas normal */
	vl_NumAuto = 5;
	vl_ordre_Pr = XZAO27_Ordre_PR ( vl_NumAuto );
	printf("\n\n*****\nXZAO27_Ordre_PR Autoroute %d = %d\n*****\n", vl_NumAuto, vl_ordre_Pr);
	XZST_03EcritureTrace (XZSTC_INFO, "XZAO27_Ordre_PR Autoroute %d = %d", vl_NumAuto, vl_ordre_Pr);
}




/*X---------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Test de la fonction XZAO28_Autoroute.
*
------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static void Test_XZAO28 ()

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
*
----------------------------------------------------------------------------*/
{
	XDY_Autoroute		vl_NumAuto;
	XZAOT_Autoroute		vl_Auto;
	int			vl_cr;


	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : Test_XZAO28");

	printf ("\n** Test de la procedure XZAO28_Autoroute **\n");


	/* Cas normal - Recherche par numero */
	vl_cr = XZAO28_Autoroute ( 1, NULL, &vl_Auto );
	printf("\n\n*****\nCR=%d - Autoroute %d <%s>, INV=%d, Escota=%d, Debut/Fin %d/%d, Concession %d/%d\n*****\n",
				 vl_cr, vl_Auto.numero, vl_Auto.nom, vl_Auto.inversee, vl_Auto.escota,
				 vl_Auto.debut, vl_Auto.fin, vl_Auto.debut_concession, vl_Auto.fin_concession);
	XZST_03EcritureTrace (XZSTC_INFO, "CR=%d - Autoroute %d <%s>, INV=%d, Escota=%d, Debut/Fin %d/%d, Concession %d/%d",
				 vl_cr, vl_Auto.numero, vl_Auto.nom, vl_Auto.inversee, vl_Auto.escota,
				 vl_Auto.debut, vl_Auto.fin, vl_Auto.debut_concession, vl_Auto.fin_concession);


	/* Cas normal - Recherche par nom */
	vl_cr = XZAO28_Autoroute ( XDC_AUT_INCONNUE, "A50", &vl_Auto );
	printf("\n\n*****\nCR=%d - Autoroute %d <%s>, INV=%d, Escota=%d, Debut/Fin %d/%d, Concession %d/%d\n*****\n",
				 vl_cr, vl_Auto.numero, vl_Auto.nom, vl_Auto.inversee, vl_Auto.escota,
				 vl_Auto.debut, vl_Auto.fin, vl_Auto.debut_concession, vl_Auto.fin_concession);
	XZST_03EcritureTrace (XZSTC_INFO, "CR=%d - Autoroute %d <%s>, INV=%d, Escota=%d, Debut/Fin %d/%d, Concession %d/%d",
				 vl_cr, vl_Auto.numero, vl_Auto.nom, vl_Auto.inversee, vl_Auto.escota,
				 vl_Auto.debut, vl_Auto.fin, vl_Auto.debut_concession, vl_Auto.fin_concession);


	/* Cas introuvable */
	vl_cr = XZAO28_Autoroute ( 12, "A53", &vl_Auto );
	printf("\n\n*****\nCR=%d - Autoroute %d <%s>, INV=%d, Escota=%d, Debut/Fin %d/%d, Concession %d/%d\n*****\n",
				 vl_cr, vl_Auto.numero, vl_Auto.nom, vl_Auto.inversee, vl_Auto.escota,
				 vl_Auto.debut, vl_Auto.fin, vl_Auto.debut_concession, vl_Auto.fin_concession);
	XZST_03EcritureTrace (XZSTC_INFO, "CR=%d - Autoroute %d <%s>, INV=%d, Escota=%d, Debut/Fin %d/%d, Concession %d/%d",
				 vl_cr, vl_Auto.numero, vl_Auto.nom, vl_Auto.inversee, vl_Auto.escota,
				 vl_Auto.debut, vl_Auto.fin, vl_Auto.debut_concession, vl_Auto.fin_concession);


	/* Cas normal */
	vl_cr = XZAO28_Autoroute ( 11, NULL, &vl_Auto );
	printf("\n\n*****\nCR=%d - Autoroute %d <%s>, INV=%d, Escota=%d, Debut/Fin %d/%d, Concession %d/%d\n*****\n",
				 vl_cr, vl_Auto.numero, vl_Auto.nom, vl_Auto.inversee, vl_Auto.escota,
				 vl_Auto.debut, vl_Auto.fin, vl_Auto.debut_concession, vl_Auto.fin_concession);
	XZST_03EcritureTrace (XZSTC_INFO, "CR=%d - Autoroute %d <%s>, INV=%d, Escota=%d, Debut/Fin %d/%d, Concession %d/%d",
				 vl_cr, vl_Auto.numero, vl_Auto.nom, vl_Auto.inversee, vl_Auto.escota,
				 vl_Auto.debut, vl_Auto.fin, vl_Auto.debut_concession, vl_Auto.fin_concession);
}





/*X---------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Fonction principale d'initialisation et execution des tests.
*
------------------------------------------------------------------------------
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
*    Ouvre une connexion a la base PRC sur le serveur de donnees du Site CI.
*    Effectue ensuite les tests en sequence.
*    Ferme la connexion a la base de donnees.
*
----------------------------------------------------------------------------*/
{
	XDY_Basedd		va_Basedd_in;

	int retcode;

/*
	retcode = XZSC_InitTrace();
	XZST_08ForceNivVisuTraceLocal(XZSTC_INFO|XZSTC_FONCTION|XZSTC_INTERFACE|XZSTC_MESSAGE|XZSTC_DEBUG1, XZSTC_NECRAN);
*/

	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : main");
	XZST_03EcritureTrace(XZSTC_INFO,"IN : main");

	retcode = XZAG02_Connexion_Base( "CI");
	printf("XZAG02_Connexion_Base retourne :%d\n",retcode);


	Test_XZAO25 ();

	printf ("\n------------------------------------------------------------------------------\n");

	Test_XZAO26 ();

	printf ("\n------------------------------------------------------------------------------\n");

	Test_XZAO27 ();

	printf ("\n------------------------------------------------------------------------------\n");

	Test_XZAO28 ();


	retcode = XZAG03_Deconnexion_Base();
	printf("XZAG03_Deconnexion_Base retourne %d\n",retcode);
}
