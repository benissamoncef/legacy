/* Fichier : atdp_xml.h   Release : 1.1   Date : 05/15/97	*
B
*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : BASEDD				*
* FICHIER : atdp_xml.h				*
*************************************************
* DESCRIPTION :
* module externe de la tache client evtic
* pour le PC STRADA
* Il contient les definitions des fonctions
* utilisees dans atdp_xml.c
*************************************************
* HISTORIQUE :
*
* GUILHOU	09 OCT 2016 : Creation 		V1.1 DEM/81151
* JMG		25/05/16 : import TDP ASF 1.2
************************************************/

#ifndef atdp_xml
#define atdp_xml

/* include system */
#include        <stdio.h>
#include        <string.h>
#include        <ctype.h>
#include 	<stdlib.h>
 
#include "xdm.h"
#include "xdg.h"
#include "xzst.h"
#include "xzsm.h"
#include "xzis.h"
#include "xzaec.h"
#include "xzasc.h"
#include "xzlg.h"


/* definition des constantes */

/* definition des variables globales È l'application */
#define ETDPC_NB_MAX_ZONE	200

typedef struct
{
	XDY_Eqt                         Numero[ETDPC_NB_MAX_ZONE+1];
	XDY_Mot                         TDP6mn[ETDPC_NB_MAX_ZONE+1];
	XDY_Mot                         Validite6mn[ETDPC_NB_MAX_ZONE+1];
	XDY_Mot                         TDP6mnRADT[ETDPC_NB_MAX_ZONE+1];
	XDY_Mot                         Validite6mnRADT[ETDPC_NB_MAX_ZONE+1];
} T_Mesure;

/* definition des variables locales au module */

/* declaration de fonctions internes */

/* definition de fonctions externes */

extern int acop_xml_init();


#endif
