/* Fichier : $Id: xdg.c,v 1.10 2010/11/30 18:25:49 gesconf Exp $      Release : $Revision: 1.10 $        Date : $Date: 2010/11/30 18:25:49 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE xdg * FICHIER xdg.c
******************************************************
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale
*   ou reference au fichier xdg.h >
*
******************************************************
* HISTORIQUE :
*
* T.Milleville  23 Aou 1994	: Creation
* JPL		30/11/10 : Migration architecture HP ia64 (DEM 961) : fichiers en-tete systeme inclus  1.10
*******************************************************/
/******************************************************
* TESTS UNITAIRES  : RELECTURE CROISEE
*
* NOM DU RELECTEUR : 
* DATE             : 
* COMMENTAIRES     :
*
*
*******************************************************/


/* fichiers inclus */

#include <stdio.h>
#include <string.h>

#include "xdg.h"
#include "xdc.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: xdg.c,v 1.10 2010/11/30 18:25:49 gesconf Exp $ : xdg" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/* Fichier : $Id: xdg.c,v 1.10 2010/11/30 18:25:49 gesconf Exp $      Release : $Revision: 1.10 $        Date : $Date: 2010/11/30 18:25:49 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE xdg * FICHIER xdg.c
******************************************************
* DESCRIPTION DU MODULE :
* Arguments :
* Datagroup	: Nom du datagroup
* mot		: numero du mot a extraire
* separateur	: caractere de separation de deux mots
* mot		: mot extrait
*
* Fonction :
* extrait d'une chaine de caractere un mot delimite par
* un caractere de separation
*   
*
******************************************************
* HISTORIQUE :
*
* T.Milleville   23 Aou 1994	: Creation
*******************************************************/
int XDG_strmot(char *pa_Datagroup, int va_mot, 
		char *va_separateur, char *pa_nom) 
{
	int lecture = 1;
	int  vl_Valret = XDC_OK;
	int vl_Fin = XDC_FAUX;
	char *vl_mot;

	vl_mot = (char *)NULL;
	pa_nom[0] = '\0';

   /* recherche du caractere separateur dans la chaine */
   if(strstr(pa_Datagroup,va_separateur)== NULL)
   {
      /* si demande du premier mot */
      if (va_mot == 1)
      {
	 /* on retourne la chaine toute entiere */
	 strcpy(pa_nom, pa_Datagroup);
	 vl_Valret = XDC_OK;
      }
      else
      {
	 vl_Valret = XDC_NOK; 
      }
      vl_Fin = XDC_VRAI;
   }

   /* il existe au moins un caractere separateur dans la chaine */
   if (vl_Fin == XDC_FAUX)
   {
        /* recherche de la premiere chaine delimitee 
	*	par le caractere separateur */	
	vl_mot =  (char *)strtok(pa_Datagroup,va_separateur);

	/* la chaine a extraire est la premiere */ 
        if (lecture == va_mot) 
    	{
		strcpy(pa_nom,vl_mot);
	}
	lecture++;
 	vl_mot = (char *)NULL;

        /* recherche de la chaine souhaitee delimite 
	*	par le caractere separateur */	
	while( ((vl_mot = (char *)strtok(vl_mot,va_separateur)) != NULL) 
					&& (lecture <= va_mot) ) 
  	{
       	   if (lecture == va_mot) strcpy(pa_nom,vl_mot);
           lecture++;
	   vl_mot = (char *)NULL;
        }
        if (va_mot>=lecture) vl_Valret = XDC_NOK;
   }
   
	return(vl_Valret);


}	/* fin XDG_strmot */

