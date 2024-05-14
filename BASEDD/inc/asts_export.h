/* Fichier : $RCSfile: asts_export.h,v $   Release : $Revision: 1.4 $   Date : $Date: 2007/03/26 13:52:20 $	*
*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : BASEDD				*
* FICHIER : asts_export.h			*
*************************************************
* DESCRIPTION :
* module externe de la tache serveur de donnees
* trafic pour le PC STRADA
* Il contient les definitions des fonctions
* utilisees dans asts_export.c
*************************************************
* HISTORIQUE :
*
* VATHELOT	29 Avr 1997 : Creation 			V1.1
* NIEPCERON	25 Fev 1999 : Modif interges dem/1724  V1.3
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

#include "xzas91.h"
#include "xzas54.h"
#include "xzas55.h"

/* definition des constantes */

/* d≈finition des variables globales » l'application */

/* definition des variables locales au module */
struct Id_Station{
        int		 	Numero;
	char			Nom[20];
        int			Sens;
        struct Id_Station	*Suivant;
        };

/* declaration de fonctions internes */

/* definition de fonctions externes */
extern int asts_export_init();
extern void asts_export_ftp();
extern void asts_export_purge();
extern void asts_export_catalog();
extern int asts_insere_station(int va_station,char *va_nom,int va_sens);
extern void asts_export_arret();
extern char * asts_get_heure();

#endif
