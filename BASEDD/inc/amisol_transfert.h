/* Fichier : @(#)amisol_trasnfert.h  1.0   Release : 1.0   Date : 06/02   *
*************************************************
* Elcimai Informatique
*************************************************
* MODULE  : BASEDD                              
* FICHIER : amisol_transfert.h                       
*************************************************
* DESCRIPTION :
* Il contient les definitions des fonctions
* utilisees dans amisol_transfert.c
*************************************************
* HISTORIQUE :
*
************************************************/

#ifndef TRANSFERT
#define TRANSFERT

/* include system */
#include        <stdio.h>
#include        <string.h>
#include        <ctype.h>

#include "xdm.h"
#include "xdg.h"
#include "xzst.h"
#include "xzsm.h"
#include "xzia.h"

#include "amisol_tcpip.h"

/* definition des constantes */

/* definition des variables globales a l'application */

/* declaration de fonctions internes */

/* definition de fonctions externes */
extern int amisol_transfert_init();
extern void amisol_transfert_tcpip();
#endif


