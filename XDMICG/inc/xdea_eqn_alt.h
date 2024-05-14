/*E*/
/*  Fichier : $Id: xdea_eqn_alt.h,v 1.3 1996/11/29 13:29:18 niepceron Exp $      Release : $Revision: 1.3 $        Date : $Date: 1996/11/29 13:29:18 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE xdea * FICHIER xdea_eqn_alt.h
******************************************************
* DESCRIPTION DU MODULE :
*
*       calcul des equations d'alerte en polonaise inverse
*
******************************************************
* HISTORIQUE :
*
* Gaborit   23/01/95 : creation
* Niepceron 05/10/96 : ajout de l'arg de sortie niveau (DEM/1248) v1.3
********************************************************/

#ifndef XDEA_H
#define XDEA_H

/* fichiers inclus */

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include "xdy.h"
#include "xdc.h"
#include "xzaa.h"
#include "xzaac.h"
#include "xzst.h"

/* definitions de constantes */

#define XDEAC_pile 100
#define XDEAC_registre 10

/* definitions de types locaux */

typedef struct {
   char * nom;
   double valeur;
   } XDEAT_VAR;

/* definition de macro locales */

/* declaration de variables locales */

extern int xdea01_evaluer ( FILE *          pa_file_in,
                   XDEAT_VAR *     pa_variables_in,
                   int             va_nb_variables_in,
                   XDY_Horodate    va_Horodate_in,
                   XDY_TypeEqt     va_TypeEqt_in,
                   XDY_Eqt         va_Eqt_in,
                   XDY_Booleen	   va_Test_in,
                   XDY_Octet	   *pa_nivTFM);



#endif
