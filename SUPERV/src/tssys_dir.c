/*E*/  
/*Fichier : $Id: tssys_dir.c,v 1.6 2011/01/13 10:10:30 gesconf Exp $      Release : $Revision: 1.6 $        Date : $Date: 2011/01/13 10:10:30 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  SUPERV
------------------------------------------------------
* MODULE TSSYS * FICHIER tssys_dir.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Permet de transmettre les donnees relatives aux chnagements
*	d'etats des taches a la base de Donnee afin de calculer
*	l'indice qualite 
*
------------------------------------------------------
* HISTORIQUE :
*
* T.Milleville	12 Jan 1995	: Creation
* T.Milleville	23 Mar 1995	: Ajout pclose V1.2
* T.Milleville	25 Jul 1995	: Ajout de commentaires V1.3
* T.Milleville	2 Oct 1995	: Sortie de la fonction main avec par default le status = 1    V1.4
* T.Milleville	2 Oct 1995	: Appel du fichier de commande test_reseau_MIGRAZUR.sh V1.5
* JPL		13/01/11	: Migration architecture HP ia64 (DEM 971) : suppression warnings mineurs  1.6
------------------------------------------------------*/

/* fichiers inclus */

#include <stdio.h>
#include <stdlib.h>
#include "tssys_dir.h"

#include "xdc.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: tssys_dir.c,v 1.6 2011/01/13 10:10:30 gesconf Exp $ : tssys_dir" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Fonction principale de la tache tssys.x 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

main(int argc, char *argv[])

/*
* ARGUMENTS EN ENTREE :
*   argc	: Nb arguments
*   argv        : Valeurs des arguments
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Privilege Super-User
*
* FONCTION 
*	Execute la commande systemee 'landiag' et recuprer le  retour
*	de celle-ci. Si erreur alors reour 1 , sinon reour 0
*
------------------------------------------------------*/
{
	static char *version="$Id: tssys_dir.c,v 1.6 2011/01/13 10:10:30 gesconf Exp $ : main " ;
		/* Permet d'executer la commande landiag , de
		*	rediriger la sortie erreur dans la sortie
		*	output et de rechercher si une Erreur */
#define vl_ligneCommande  "landiag -t < " XDC_NSUP_PATHEXEC "/test_reseau_MIGRAZUR.sh 2>&1 | grep errno"
	FILE    *pl_cr	= NULL;
	char 	vl_Buf[80]	= "";
	int	vl_Status	= 1;


	
	if ( (pl_cr = popen(vl_ligneCommande, "r")) != NULL )
	{
		if (fscanf( pl_cr, "%s\n", vl_Buf ) != EOF )
		{	
			/*A Alors Erreur donc Machine deconnectee du Reseau */
			printf ("Attention Machine deconnecte du Reseau !!\n");
			vl_Status = 1;
		}
		else
		{
			printf (" Machine connectee au Reseau !!\n");
			/*A Machine connectee */
			vl_Status = 0;
		}
	}

	pclose(pl_cr);

	exit(vl_Status);

}	/* Fin main */


