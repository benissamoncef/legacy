/*E*/
/*Fichier : $Id: exa0_don.c,v 1.3 1995/05/29 14:38:47 volcic Exp $      Release : $Revision: 1.3 $        Date : $Date: 1995/05/29 14:38:47 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEXA0 * FICHIER exa0_don.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Module de definition des donnees internes a la tache 
*  TEXA0, protocole de la gtc Nice
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	22 Nov 1994	: Creation
* Fontaine.C	version 1.2	23 Mai 1995	: Reprise entete
* Mismer.D	version 1.3	29 Nov 1994	: Modif fonction supprimer cmmande en cours
*
------------------------------------------------------*/

/* fichiers inclus */

#include "exa0_don.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: exa0_don.c,v 1.3 1995/05/29 14:38:47 volcic Exp $ : exa0_don" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

EX_FONCT_INSERER   (  EXA0_DONNEES_Tube, ea_ins_liste_Tube, pg_debutTube, Numero, Suivant, Suivant )
EX_FONCT_INSERER   (  EXA0_DONNEES_Echangeur, ea_ins_liste_Ech, pg_debutEchangeur, NumeroDemiEch, Suivant, Suivant )


EX_FONCT_INSERER   (  EXA0_ENG_CMD, ea_ins_liste_cmd, pg_debutCmd, Horodate, Suivant, Suivant)

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Suppression d'une commande enregistree temporairement (attente passage flag a zero )
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void   ea_sup_liste_cmd   ( XDY_Horodate va_Horodate ) 

/*
* ARGUMENTS EN ENTREE :
*
* XDY_Horodate va_Horodate : Horodate de l'element a supprimer
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
*
* CONDITION D'UTILISATION
* 
*
* FONCTION
*
*  
------------------------------------------------------*/
{
   static char *version = "$Id: exa0_don.c,v 1.3 1995/05/29 14:38:47 volcic Exp $ : ea_sup_liste_cmd " ;
   EXA0_ENG_CMD *pl_EltsSup, *pl_ExSup; 

   for ( pl_ExSup = pl_EltsSup = pg_debutCmd; 
         pl_EltsSup != NULL; 
         pl_EltsSup = pl_EltsSup->Suivant ) 
   { 
      if ( pl_EltsSup->Horodate == va_Horodate ) 
      { 
         if ( ( pl_ExSup == pg_debutCmd ) && ( pl_ExSup == pl_EltsSup ) ) 
         { 
	    pg_debutCmd = pl_EltsSup->Suivant; 
         } 
         else 
         { 
	    pl_ExSup->Suivant = pl_EltsSup->Suivant; 
         } 
         free(pl_EltsSup); 
         break; 
      } 
      else 
         pl_ExSup = pl_EltsSup; 
   } 
}



