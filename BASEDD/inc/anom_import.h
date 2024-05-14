/* Fichier : @(#)anom_import.h	1.1   Release : 1.1   Date : 04/19/01	*
*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : BASEDD				*
* FICHIER : anom_import.h			*
*************************************************
* DESCRIPTION :
* Il contient les definitions des fonctions
* utilisees dans anom_import.c
*************************************************
* HISTORIQUE :
*
* JMG	16/01/01	creation 1.1
************************************************/

#ifndef EXPORT
#define EXPORT

/* include system */
#include        <stdio.h>
#include        <string.h>
#include        <ctype.h>

#include "xdm.h"
#include "xdg.h"
#include "xzst.h"
#include "xzsm.h"
#include "xzia.h"

#include "xzae170.h"
#include "xzae171.h"
#include "xzae173.h"

/* definition des constantes */

/* d≈finition des variables globales » l'application */

/* declaration de fonctions internes */

/* definition de fonctions externes */
extern int anom_import_init();
extern void anom_import_arret();

#endif
