/*E*/
/* Fichier : @(#)gmot_lit.c	1.2	      Release : 1.2        Date : 11/22/94
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_lit * FICHIER scon_lit.c
******************************************************
* DESCRIPTION DU MODULE :
*
* ce module contient les fonctions de lecture des champs
* de boites de dialogue utilisees par la tache tgmoti
*
******************************************************
* HISTORIQUE :
*
* T.Milleville	15 Nov 94 	: Creation
*******************************************************/

/* fichiers inclus */

#include "gmot_ser.h"
#include "gmot_var.h"

#include "gmot_lit.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)gmot_lit.c	1.2	 11/22/94 : gmot_lit" ;

/* declaration de fonctions internes */



/* definition de fonctions externes */


/* Fichier : @(#)gmot_lit.c	1.2	      Release : 1.2        Date : 11/22/94
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_lit
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*  cette fonction remplit les champs de pa_strucBasc a 
*  partir des donnees saisies dans BDBSD 
*
******************************************************
* SEQUENCE D'APPEL :
*/

int
LitChampsBSD( tg_Basculement *pa_strucBasc )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
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
   char *pl_text;

   /* recuperation du type de basculement desire  */
   /* toggle buttons BDBSD_TO_CYC et BDBSD_TO_NCY */
   if ( (XmToggleButtonGetState( BDBSD_TO_CYC ) ) == True )
     pa_strucBasc->Cyclique = TRUE ;
   else
     if ( (XmToggleButtonGetState( BDBSD_TO_NCY ) ) == True )
       pa_strucBasc->Cyclique = FALSE ;

   /* recuperation de la periode du basculement desire  */
   /* champs BDBSD_CH_PER                               */
   pl_text  = XmTextFieldGetString( BDBSD_CH_PER );
   pa_strucBasc->Periode = atoi ( pl_text );

   free( pl_text );

   if ((pa_strucBasc->Cyclique == TRUE) && 
       (pa_strucBasc->Periode == 0 ))
     return( cg_VIDE );
   else
     return( cg_NONVIDE );
}


/* Fichier : @(#)gmot_lit.c	1.2	      Release : 1.2        Date : 11/22/94
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_lit
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*  cette fonction remplit les champs de pa_strucBasc a 
*  partir des donnees saisies dans BDBSC 
*
******************************************************
* SEQUENCE D'APPEL :
*/

int
LitChampsBSC( tg_Basculement *pa_strucBasc )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
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
   char *pl_text;

   /* recuperation du type de basculement desire  */
   /* toggle buttons BDBSC_TO_CYC et BDBSD_TO_NCY */
   if ( (XmToggleButtonGetState( BDBSC_TO_CYC ) ) == True )
     pa_strucBasc->Cyclique = TRUE ;
   else
     if ( (XmToggleButtonGetState( BDBSC_TO_NCY ) ) == True )
       pa_strucBasc->Cyclique = FALSE ;

   /* recuperation de la periode du basculement desire  */
   /* champs BDBSC_CH_PER                               */
   pl_text  = XmTextFieldGetString( BDBSC_CH_PER );
   pa_strucBasc->Periode = atoi ( pl_text );

   free( pl_text );

   if ((pa_strucBasc->Cyclique == TRUE) && 
       (pa_strucBasc->Periode == 0 ))
     return( cg_VIDE );
   else
     return( cg_NONVIDE );
}


/* Fichier : @(#)gmot_lit.c	1.2	      Release : 1.2        Date : 11/22/94
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_lit
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*  cette fonction remplit les champs de pa_strucBasc a 
*  partir des donnees saisies dans BDBSC 
*
******************************************************
* SEQUENCE D'APPEL :
*/

int
LitChampsLIVL( XZSCT_NomMachine va_machine )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
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
   char *pl_text;

   /* recuperation du nom de machine */
   pl_text  = XmTextFieldGetString( LIVL_CH_MAC );

   strcpy( va_machine, pl_text );

   free( pl_text );

   if ( strcmp( va_machine, "" ) == 0 )
     return( cg_VIDE );
   else
     return( cg_NONVIDE );
}

