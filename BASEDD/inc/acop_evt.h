/* Fichier : acop_evt.h   Release : 1.1   Date : 05/15/97	*
B
*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : BASEDD				*
* FICHIER : acop_evt.h				*
*************************************************
* DESCRIPTION :
* module externe de la tache client evtic
* pour le PC STRADA
* Il contient les definitions des fonctions
* utilisees dans acop_evt.c
*************************************************
* HISTORIQUE :
*
* GUILHOU	09 OCT 2008 : Creation 		V1.1 DEM/835
* JMG		25/02/16 : suppression loc_rds 1.2 DEM 1151
************************************************/

#ifndef acop_evt
#define acop_evt

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
#include "xzae182.h"
#include "xzae178.h"


/*liste des fmc a exporter (dynamique)*/
typedef struct {
        XDY_Entier      Numero;
	XDY_Octet       Cle;
	XDY_Entier	Type;
} T_FMC;

typedef struct S_Liste_FMC {
	T_FMC			FMC;
	struct S_Liste_FMC	*Suivant;
} T_Liste_FMC;


/*conversion type de fmc sae -> code attribut strada (statique)*/
typedef struct {
	int	Type;
	char	Code_Strada[5];
	char	XMLFile[100];
	char	Type_Datex[50];
} T_Type_FMC;


/* definition des constantes */

/* d�finition des variables globales � l'application */

/* definition des variables locales au module */

/* declaration de fonctions internes */

/* definition de fonctions externes */

extern int acop_xml_init();


#endif
