/*E*/
/* Fichier : $Id: ecfe_don.c,v 1.1 2019/01/29 11:07:58 pc2dpdy Exp $     Release : $Revision: 1.1 $      Date :  $Date: 2019/01/29 11:07:58 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TECFE * FICHIER ecfe_don.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module decrivant les structures de donn≈es internes
*
------------------------------------------------------
* HISTORIQUE :
*
* D.Mismer	version 1.1	14 Jan 1998	: Creation
* JMG	04/03/18 : Creation controleurs de feux CTRL_FEUX 1.1 DEM1284
*
------------------------------------------------------*/

/* fichiers inclus */

#include "ecfe_don.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: ecfe_don.c,v 1.1 2019/01/29 11:07:58 pc2dpdy Exp $ : ecfe_don" ;

/* declaration de fonctions internes */

EX_FONCT_INSERER   ( ECFE_DONNEES_CFE, ed_ins_liste_CFE, pg_debutCFE, Config.Numero, Suivant, Config )
EX_FONCT_SUPPRIMER ( ECFE_DONNEES_CFE, ed_sup_liste_CFE, pg_debutCFE, Config.Numero, Suivant)	

/* definition de fonctions externes */



