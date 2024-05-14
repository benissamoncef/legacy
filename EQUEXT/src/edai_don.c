/*E*/
/*Fichier : $Id: edai_don.c,v 1.6 1995/05/23 10:36:21 cornu Exp $      Release : $Revision: 1.6 $        Date : $Date: 1995/05/23 10:36:21 $
------------------------------------------------------
* GRANIOU *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEDAI * FICHIER edai_don.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Module de gestion des donnees de la tache TEDAI.
*
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E	version 1.1	18 Oct 1994	: Creation
* Nagiel.E	version 1.2	05 Jan 1995	:
* Nagiel.E	version 1.3	16 Jan 1995	:
* Nagiel.E	version 1.4	16 Jan 1995	:
* Volcic.F	version 1.5	04 Avr 1995	:
* Fontaine.C	version 1.6	23 Mai 1995	: Reprise entete, historique
*
------------------------------------------------------*/

/* fichiers inclus */

#include "edai_don.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: edai_don.c,v 1.6 1995/05/23 10:36:21 cornu Exp $ : edai_don" ;

/* declaration de fonctions internes */

EX_FONCT_INSERER   ( EDAI_DONNEES_DAI, ed_ins_liste_DAI, pg_debutDAI, Config.Numero, Suivant, Config )
EX_FONCT_SUPPRIMER ( EDAI_DONNEES_DAI, ed_sup_liste_DAI, pg_debutDAI, Config.Numero, Suivant)	
EX_FONCT_INSERER   ( EDAI_DONNEES_ANALYSEUR, ed_ins_liste_Analy, pg_debutAnaly, Config.Numero, Suivant, Config)
EX_FONCT_SUPPRIMER ( EDAI_DONNEES_ANALYSEUR, ed_sup_liste_Analy, pg_debutAnaly, Config.Numero, Suivant)	

/* definition de fonctions externes */



