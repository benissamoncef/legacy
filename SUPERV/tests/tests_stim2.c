/*
* Fichier : tests_stim2.c
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SUPERV / TESTS
******************************************************
* DESCRIPTION DU MODULE :
*
* Ce module contient des tests des fonctions 
* de la tache tstim.
*
******************************************************
* HISTORIQUE :
*
* JPL	06/10/22 : Creation  1.1
*******************************************************/

/* fichiers inclus */
#include <xzsem.h>

#include "xzsa.h"

#include "stim_tra.h"

#define STIM_DONNEES
#include "stim_ser.h"


/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

/* declaration de fonctions internes */

/* definition de fonctions externes */



/*
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* Fonction d'affichage de la liste des reveils.
*
*****************************************************/
void	dump_Liste_reveils(void)
{
	SMT_ENRG_TIMER	*pl_Tempo = NULL;

	if ( pg_ListeTempo == NULL )
	{
		printf ( "Liste Vide\n");
	}
	else
	{
		for ( pl_Tempo = pg_ListeTempo;  pl_Tempo != NULL;  pl_Tempo = pl_Tempo->Suivant)
		{
			printf ( "	Horodate=%f	Cle=%s	Dg=%s	IdMsg=%d\n",
				 pl_Tempo->Horodate, pl_Tempo->Cle, pl_Tempo->Dg, pl_Tempo->IdMsg);
		}
	}
	printf ( "\n");
}



/*
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* Test de la suppression en liste des reveils.
*
*****************************************************/
void	test_sm_Supprimer_Liste(void)
{
	char	*vl_Racine_Dg		= "DGROUP_";
	char	*vl_Racine_Cle		= "CLE_";
	XZSCT_Datagroup	vl_DgX		= "UN_DG_X";
	XZSCT_Datagroup	vl_DgY		= "UN_DG_Y";
	XZSCT_Datagroup	vl_Dg_Suppr	= "DG_SUPPR";
	XZSMT_CleProg	vl_Cle		= "";
	int	i;

	SMT_ENRG_TIMER	tl_Reveils[1 + 8];

	/* Initialiser les elements a partir de 1 */
	for ( i = 1;  i <= 8;  i++ ) {
		tl_Reveils[i].Horodate = (double) i;
		tl_Reveils[i].Delai = i;
		tl_Reveils[i].IdMsg = i;

		sprintf (tl_Reveils[i].Dg, "%s%d", vl_Racine_Dg, i);
		sprintf (tl_Reveils[i].Cle, "%s%d", vl_Racine_Cle, i);

		tl_Reveils[i].Priorite = 0;
		tl_Reveils[i].Suivant = NULL;
	}


	/* Definir les reveils a supprimer par Dg et Id, avec cle nulle */
	sprintf (tl_Reveils[3].Dg, "%s", vl_Dg_Suppr);
	sprintf (tl_Reveils[5].Dg, "%s", vl_Dg_Suppr);
	sprintf (tl_Reveils[6].Dg, "%s", vl_Dg_Suppr);
	tl_Reveils[3].IdMsg = 11;
	tl_Reveils[5].IdMsg = 11;
	tl_Reveils[6].IdMsg = 11;

	sprintf (tl_Reveils[2].Dg, "%s", vl_Dg_Suppr);
	sprintf (tl_Reveils[7].Dg, "%s", vl_Dg_Suppr);
	tl_Reveils[2].IdMsg = 17;
	tl_Reveils[7].IdMsg = 17;

	/* Construire la liste des reveils */
	for ( i = 1;  i <= 8;  i++ ) {
		sm_Inserer_Liste (&tl_Reveils[i]);
	}

	printf ( "Liste initiale\n");
	dump_Liste_reveils ();


	/* Supprimer le premier element (numero 1) */
	sprintf (vl_Cle, "%s%d", vl_Racine_Cle, 1);
	sm_Supprimer_Liste (vl_Cle, vl_DgX, 101);
	printf ( "Suppression premier element\n");
	dump_Liste_reveils ();


	/* Supprimer le dernier element (numero 8) */
	sprintf (vl_Cle, "%s%d", vl_Racine_Cle, 8);
	sm_Supprimer_Liste (vl_Cle, vl_DgY, 102);
	printf ( "Suppression dernier element\n");
	dump_Liste_reveils ();


	/* Supprimer 3 elements differents du premier (numeros 3, 5, 6) */
	sm_Supprimer_Liste ("", vl_Dg_Suppr, 11);
	printf ( "Suppression elements avec IdMsg 11\n");
	dump_Liste_reveils ();


	/* Supprimer 2 elements dont le premier (numeros 2, 7) */
	sm_Supprimer_Liste ("", vl_Dg_Suppr, 17);
	printf ( "Suppression elements avec IdMsg 17, dont le premier\n");
	dump_Liste_reveils ();


	/* Supprimer l'element restant (numero 4) */
	sprintf (vl_Cle, "%s%d", vl_Racine_Cle, 4);
	sm_Supprimer_Liste (vl_Cle, vl_DgX, 201);
	printf ( "Suppression elements restants\n");
	dump_Liste_reveils ();
}



/*
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE STIM_DIR
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* Main de la tache tstim.x
*
******************************************************
* SEQUENCE D'APPEL :
*/

int main( int argc, char *argv[] )

/*
* PARAMETRES EN ENTREE : argv[0]= nom de l'application
*
* PARAMETRES EN SORTIE : neant
*
* VALEUR(S) DE RETOUR : neant
*
* CONDITION D'ERREUR : neant
*
* CONDITIONS LIMITES : neant
*
******************************************************/
{
	/* Initialisation du pointeur de debut de liste des tempos */
	pg_ListeTempo = NULL ;


	test_sm_Supprimer_Liste ();

	return (0);
}

