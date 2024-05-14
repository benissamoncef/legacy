/* Fichier : $Id: xdy.c,v 1.6 2021/06/15 17:27:40 devgfi Exp $      $Revision: 1.6 $        $Date: 2021/06/15 17:27:40 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE xdy * FICHIER xdy.c
******************************************************
* DESCRIPTION DU MODULE :
*
* Useful functions 
*
******************************************************
* HISTORIQUE :
*
* W.Heil	13 Jun 1994	: Creation
* JPL		30/11/10 : Migration architecture HP ia64 (DEM 961) : fichiers en-tete systeme inclus  1.2
* JPL		08/02/13 : Ajout fonctions XDY01_Supprimer_Espaces_De_Fin et XDY02_Copie_Sans_Espaces_De_Fin  1.3
* JPL		19/05/16 : XDY01_Supprimer_Espaces_De_Fin : supprimer tous caracteres d'espacement  1.4
* JPL		26/04/17 : Ajout fonctions XDY03_Supprimer_Espaces et XDY04_Copie_Sans_Espaces  1.5
* JPL		15/06/21 : Suppression des declarations de variables en registre; noms des fonctions  1.6
*******************************************************/

/* fichiers inclus */

#include "xdy.h"
#include "string.h"
#include <ctype.h>

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xdy.c,v 1.6 2021/06/15 17:27:40 devgfi Exp $ : xdy.c" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*****************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE xdy
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* converts a string into a uppercase-string
*
******************************************************
* SEQUENCE D'APPEL :
*/

void strupper(char *pa_string)

/*
* PARAMETRES EN ENTREE / SORTIE: pa_string enters in lowercase,
*                                leaves in uppercase
*
*
* VALEUR(S) DE RETOUR : 
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
******************************************************/
{
   int i;

   for (i=0;(i < (int)strlen(pa_string));i++)
       pa_string[i] = toupper(pa_string[i]);
}


/*X*/
/*****************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE xdy
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* converts a string into a lowercase-string
*
******************************************************
* SEQUENCE D'APPEL :
*/

void strlower(char *pa_string)

/*
* PARAMETRES EN ENTREE / SORTIE: pa_string enters in uppercase,
*                                leaves in lowercase
*
*
* VALEUR(S) DE RETOUR : 
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
******************************************************/
{
   int i;

   for (i=0;(i < (int)strlen(pa_string));i++)
       pa_string[i] = tolower(pa_string[i]);
}




/*X*/
/*------------------------------------------------------------------------------
* ESCOTA  *  PROJET MIGRAZUR
--------------------------------------------------------------------------------
* MODULE xdy
--------------------------------------------------------------------------------
* DESCRIPTION DE LA FONCTION :
*
*  Supprime les espaces en fin d'une chaine de caracteres.
*
--------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

char *XDY_Supprimer_Espaces_De_Fin (char *pa_chaine)

/*
* PARAMETRES EN ENTREE / SORTIE :
*   pa_chaine         : chaine a traiter.
*
*
* VALEUR(S) DE RETOUR : 
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
------------------------------------------------------------------------------*/
{
	int	i;

	i = strlen (pa_chaine) - 1;
	while (i >= 0  &&  isspace(pa_chaine[i]))
	{
		i--;
	}
	pa_chaine[++i] = '\0';

	return (pa_chaine);
}




/*X*/
/*------------------------------------------------------------------------------
* ESCOTA  *  PROJET MIGRAZUR
--------------------------------------------------------------------------------
* MODULE xdy
--------------------------------------------------------------------------------
* DESCRIPTION DE LA FONCTION :
*
*  Copie une chaine de caracteres en supprimant les espaces en fin de chaine.
*
--------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

char *XDY_Copie_Sans_Espaces_De_Fin (char *pa_destination, char *pa_source)

/*
* PARAMETRES EN ENTREE / SORTIE :
*   pa_destination    : chaine resultat ;
*   pa_source         : chaine a copier.
*
*
* VALEUR(S) DE RETOUR : 
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
------------------------------------------------------------------------------*/
{
	strcpy (pa_destination, pa_source);
	return (XDY_Supprimer_Espaces_De_Fin (pa_destination));
}




/*X*/
/*------------------------------------------------------------------------------
* ESCOTA  *  PROJET MIGRAZUR
--------------------------------------------------------------------------------
* MODULE xdy
--------------------------------------------------------------------------------
* DESCRIPTION DE LA FONCTION :
*
*  Supprime les espaces aux extremites d'une chaine de caracteres.
*
--------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

char	*XDY_Supprimer_Espaces (char *pa_chaine)

/*
* PARAMETRES EN ENTREE / SORTIE :
*
*   pa_chaine         : chaine a traiter.
*
*
* VALEUR(S) DE RETOUR :
*
*   La chaine modifiee.
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
------------------------------------------------------------------------------*/
{
	return (XDY_Copie_Sans_Espaces (pa_chaine, pa_chaine));
}




/*X*/
/*------------------------------------------------------------------------------
* ESCOTA  *  PROJET MIGRAZUR
--------------------------------------------------------------------------------
* MODULE xdy
--------------------------------------------------------------------------------
* DESCRIPTION DE LA FONCTION :
*
*  Copie une chaine de caracteres en supprimant les espaces aux extremites.
*
--------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

char	*XDY_Copie_Sans_Espaces (char *pa_destination, char *pa_source)

/*
* PARAMETRES EN ENTREE / SORTIE :
*
*   pa_destination    : chaine resultat ;
*   pa_source         : chaine a copier.
*
*
* VALEUR(S) DE RETOUR :
*
*   La chaine resultat.
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
------------------------------------------------------------------------------*/
{
	char	*pl_source;
	char	*pl_dest = pa_destination;

	for (pl_source = pa_source;  isspace (*pl_source);  pl_source++) {
	}

	while (*pl_source) {
		*pl_dest++ = *pl_source++;
	}

	do {
		pl_dest--;
	} while ( (pl_dest >= pa_destination)  &&  isspace (*pl_dest) );

	*++pl_dest = '\0';

	return (pa_destination);
}
