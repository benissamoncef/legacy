/*E*/
/*Fichier : $Id: xdf.c,v 1.3 2018/10/25 17:01:17 devgfi Exp $      Release : $Revision: 1.3 $        Date : $Date: 2018/10/25 17:01:17 $
------------------------------------------------------------------------------
* ESCOTA  *  PROJET MIGRAZUR
------------------------------------------------------------------------------
* SOUS-SYSTEME  XDMICG
------------------------------------------------------------------------------
* MODULE XDF * FICHIER xdf.c
------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Ressources communes : gestion des fichiers.
*
*   Fournit des moyens generaux de gestion des ressources de donnees
*   telles que les fichiers.
*
------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL		19/05/16 : Creation  1.1
* JPL		31/05/16 : Redefinition fonction XDF01_ValeurNom  1.2
* JPL		25/10/18 : Constantes; trace du nom de fichier si introuvable  1.3
----------------------------------------------------------------------------*/

/* Fichiers inclus */

#include "xdc.h"
#include "xdy.h"
#include "xdf.h"
#include "xzst.h"


/* Declaration de variables internes */

static char *version = "$Id: xdf.c,v 1.3 2018/10/25 17:01:17 devgfi Exp $ : xdf.c" ;


/* Declaration de fonctions internes */


/* Definition de fonctions externes */



/*X---------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Fonction recherchant dans un fichier une ligne dont le premier champ
*  a pour valeur un nom donne.
*  Retourne cette ligne de texte, ainsi que le deuxieme champ pour le cas
*  ou il constitue l'unique valeur du premier.
*
------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XDF01_ValeurNom ( const char *va_Fichier,
                        const char *va_Nom,
                        char *pa_Valeur,
                        char *pa_Ligne )

/*
* PARAMETRES EN ENTREE :
*
*   va_Fichier         : Chemin d'acces au fichier a lire
*   va_Nom             : Le nom a rechercher dans le fichier
*
* PARAMETRES EN SORTIE :
*
*   pa_Valeur          : Valeur du deuxieme champ si elle est trouvee, "" sinon (si non NULL)
*   pa_Ligne           : Texte de la ligne trouvee (si non NULL)
*
*
* CODE RETOUR :
*   XDC_OK          si le nom est trouve dans le fichier
*   XDC_OBJ_AUCUN   si aucune ligne decrivant ce nom n'est trouvee
*   XDC_NOK         si le fichier est introuvable ou inaccessible
*   XDC_ARG_INV     si un parametre invalide est fourni
*
*
* CONDITION D'UTILISATION
*
*   Une zone ou retourner soit la valeur, soit la ligne complete doit etre fournie.
*   Les zones recevant les valeurs doivent etre de taille suffisante.
*
* FONCTION
*
-----------------------------------------------------------------------------------------*/
{
	static char *version = "$Id: xdf.c,v 1.3 2018/10/25 17:01:17 devgfi Exp $ : XDF01_ValeurNom" ;

	XZST_03EcritureTrace ( XZSTC_FONCTION, "IN : %s %s", version, ((va_Fichier == NULL) ? "" : va_Fichier) );

	/*
	** Controle des parametres obligatoires
	*/

	if ( (va_Nom == NULL) || (va_Nom[0] == XDC_FIN_CHN)  ||  (va_Fichier == NULL) || (va_Fichier[0] == XDC_FIN_CHN)  ||
	     ( (pa_Ligne == NULL) && (pa_Valeur == NULL) ) )
	{
		XZST_03EcritureTrace (XZSTC_WARNING, "XDF01_ValeurNom : parametre invalide");
		return (XDC_ARG_INV);
	}


	/*
	** Ouvrir le fichier en lecture
	*/

	FILE	*vl_fd = fopen (va_Fichier, "r");
	if (vl_fd == NULL) {
		XZST_03EcritureTrace (XZSTC_WARNING, "XDF01_ValeurNom : fichier '%s' inaccessible", va_Fichier);
		return (XDC_NOK);
	}


	/*
	** Initialiser les valeurs a retourner
	*/

	int	vl_cr = XDC_OBJ_AUCUN;
	if (pa_Valeur != NULL) strcpy (pa_Valeur, XDC_CHAINE_VIDE);
	if (pa_Ligne != NULL) strcpy (pa_Ligne, XDC_CHAINE_VIDE);


	/*
	** Balayer les lignes du fichier a la recherche du nom
	*/

	XDY_Texte	vl_ligne = XDC_CHAINE_VIDE;
	XDY_Texte	vl_txt_nom = XDC_CHAINE_VIDE, vl_txt_valeur = XDC_CHAINE_VIDE;
	int		vl_nb_val = 0;

	while ( (vl_cr != XDC_OK)  &&  fgets (vl_ligne, sizeof(vl_ligne), vl_fd) != NULL)
	{
		vl_nb_val = sscanf (vl_ligne, " %s %s", vl_txt_nom, vl_txt_valeur);
		if ( (vl_nb_val >= 1)  &&  (strcmp (vl_txt_nom, va_Nom) == 0) ) {
			vl_cr = XDC_OK;
			if ( (vl_nb_val == 2) && (pa_Valeur != NULL) ) strcpy (pa_Valeur, vl_txt_valeur);
			if (pa_Ligne != NULL) strcpy (pa_Ligne, vl_ligne);
		}
	}
	fclose (vl_fd);


	XZST_03EcritureTrace (XZSTC_FONCTION, "OUT : %s : Nom %s trouve.", version, (vl_cr == XDC_OK) ? "" : "non");
	return (vl_cr);
}
