/*E*/
/* Fichier : @(#)gmot_ecr.c	1.2	      Release : 1.2        Date : 11/22/94
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_ecrit * FICHIER gmot_ecrit.c
******************************************************
* DESCRIPTION DU MODULE :
*
*
******************************************************
* HISTORIQUE :
*
* T.Milleville	15 Nov 94 	: Creation
*******************************************************/

/* fichiers inclus */

#include "gmot_ser.h"
#include "gmot_var.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)gmot_ecr.c	1.2	 11/22/94 : gmot_ecrit" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */




/* Fichier : @(#)gmot_ecr.c	1.2	      Release : 1.2        Date : 11/22/94
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_ecrit
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* cette fonction initialise le contenu de BDACA                         
*
******************************************************
* SEQUENCE D'APPEL :
*/

void
EcritChampsACA( int va_mode )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
  if ( va_mode == cg_VIDE )
  {
    /* selectionne un toggle button */
    XmToggleButtonSetState( BDACA_TO_SAN , True, False );
    XmToggleButtonSetState( BDACA_TO_AVE , False, False );
  }
}


/* Fichier : @(#)gmot_ecr.c	1.2	      Release : 1.2        Date : 11/22/94
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_ecrit
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* cette fonction initialise le contenu de BDASS                         
*
******************************************************
* SEQUENCE D'APPEL :
*/

void
EcritChampsASS( int va_mode )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
  XmString  vl_List[1];

  vl_List[0]=XmStringCreateLtoR("",(XmStringCharSet)XmSTRING_DEFAULT_CHARSET);

  if ( va_mode == cg_VIDE )
  {
    /* selectionne un toggle button */
    XmToggleButtonSetState( BDASS_TO_LA , True, False );
    XmToggleButtonSetState( BDASS_TO_AR , False, False );
    /* vide la liste */
    XtVaSetValues( BDASS_LI_SSS, XmNitems, vl_List, XmNitemCount, 1, NULL);
  }

  XmStringFree( vl_List[0] );
}



/* Fichier : @(#)gmot_ecr.c	1.2	      Release : 1.2        Date : 11/22/94
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_ecrit
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* cette fonction initialise le contenu de BDASU                         
*
******************************************************
* SEQUENCE D'APPEL :
*/

void
EcritChampsASU( int va_mode )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
  XmString  vl_List[1];

  vl_List[0]=XmStringCreateLtoR("",(XmStringCharSet)XmSTRING_DEFAULT_CHARSET);

  if ( va_mode == cg_VIDE )
  {
    /* selectionne un toggle button */
    XmToggleButtonSetState( BDASU_TO_LA , True, False );
    XmToggleButtonSetState( BDASU_TO_AR , False, False );
  }

  XmStringFree( vl_List[0] );
}


/* Fichier : @(#)gmot_ecr.c	1.2	      Release : 1.2        Date : 11/22/94
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_ecrit
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* cette fonction initialise le contenu de BDATA                         
*
******************************************************
* SEQUENCE D'APPEL :
*/

void
EcritChampsATA( int va_mode )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
  XmString  vl_List[1];

  vl_List[0]=XmStringCreateLtoR("",(XmStringCharSet)XmSTRING_DEFAULT_CHARSET);

  if ( va_mode == cg_VIDE )
  {
    /* selectionne un toggle button */
    XmToggleButtonSetState( BDATA_TO_LA , True, False );
    XmToggleButtonSetState( BDATA_TO_AR , False, False );
    /* vide la liste */
    XtVaSetValues( BDATA_LI_TAC, XmNitems, vl_List, XmNitemCount, 1, NULL);
  }

  XmStringFree( vl_List[0] );
}

