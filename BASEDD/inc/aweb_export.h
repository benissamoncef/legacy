/* Fichier : $Id: aweb_export.h,v 1.2 2008/11/17 11:30:46 pc2dpdy Exp $   Release : $Revision: 1.2 $   Date : $Date: 2008/11/17 11:30:46 $	*
*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : BASEDD				*
* FICHIER : aweb_export.h			*
*************************************************
* DESCRIPTION :
* Il contient les definitions des fonctions
* utilisees dans aweb_export.c
*************************************************
* HISTORIQUE :
*
* JMG	16/01/01	creation 1.1
* JMG	11/10/08 	refonte tagen 1.2 DEM/836
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

#include "xzas80.h"
#include "xzaec.h"

typedef struct {
	int    Type_Web;
	char   Icone[50];
	int    Priorite;
	char   Texte_Web[50];
} T_Type_FMC;

/* definition des constantes */

/* d≈finition des variables globales » l'application */
extern T_Type_FMC              tg_types_fmc[];

/* declaration de fonctions internes */

/* definition de fonctions externes */
extern int aweb_export_init();
extern void aweb_export_ftp();
extern void aweb_export_purge();
extern void aweb_export_arret();
extern char * aweb_get_heure();
int aweb_ftp_calcul_troncons(char * , int , char * ,
			int , XDY_Octet , XDY_Octet ,
			XDY_Octet , XDY_Octet , XDY_Octet ,
			XDY_Autoroute , XDY_Sens , XDY_PR ,
			XDY_PR );

#endif
