/* Fichier : $Id: atdp_export.h,v 1.3 2016/09/29 15:41:38 pc2dpdy Exp $   Release : $Revision: 1.3 $   Date : $Date: 2016/09/29 15:41:38 $	*
*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : BASEDD				*
* FICHIER : atdp_export.h			*
*************************************************
* DESCRIPTION :
* Il contient les definitions des fonctions
* utilisees dans atdp_export.c
*************************************************
* HISTORIQUE :
*
* JMG	16/01/01	creation 1.1 DEM/837
* JMG	25/02/16	export ASF 1.2 DEM 1151
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
#include "xzlg.h"

/* definition des constantes */

/* d≈finition des variables globales » l'application */

/* declaration de fonctions internes */

/* definition de fonctions externes */
extern int atdp_export_init();
extern void atdp_export_ftp();
extern void atdp_import_ftp();
extern void atdp_export_purge();
extern void atdp_export_arret();
extern char * atdp_get_heure();
extern void atdp_xml_write2(XZLGT_Liste_Zdp, int, char *);
#endif
