/* Fichier : apat_evt.h   Release : 1.0   Date : 30/04/20*
*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : BASEDD				*
* FICHIER : apat_evt.h				*
*************************************************
* DESCRIPTION :
* module externe de la tache client apat
* Il contient les definitions des fonctions
* utilisees dans apat_evt.c
*************************************************
* HISTORIQUE :
*
* ABE	22/07/2020 : Creation V1.1 DEM SAE149
************************************************/

#ifndef apat_evt
#define apat_evt

/* include system */
#include    <string.h>
#include    <ctype.h>
 
#include "xdm.h"
#include "xdg.h"
#include "xzst.h"
#include "xzsm.h"
#include "xzis.h"
#include "xzaec.h"
#include "xzasc.h"
#include "xzlg.h"

/*liste des fmc a exporter (dynamique)*/

/*conversion type de fmc sae -> code attribut strada (statique)*/

/* definition des constantes */
#define C_PAT_LIST_FILE_PATH	"/produits/migrazur/appliSD/fichiers/stra/pat/patlist.txt"	
/* dÅfinition des variables globales È l'application */

/* definition des variables locales au module */

/* declaration de fonctions internes */
void apat_envoie_pat_en_cours();

/* definition de fonctions externes */



#endif
