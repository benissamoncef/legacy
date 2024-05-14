/*E*/
/* Fichier : $Id: drol_tdp.h,v 1.3 2011/02/17 19:22:46 gesconf Exp $        Release : $Revision: 1.3 $        Date : $Date: 2011/02/17 19:22:46 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME SAIDEC
------------------------------------------------------
* MODULE DROL_TDP * FICHIER drol_tdp.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	24/02/97 : Creation 1.0
* JPL	17/02/11 : Migration architecture HP ia64 (DEM 971) : declaration drol_tdp_init  1.3
------------------------------------------------------ */

#ifndef DROL_TDP
#define DROL_TDP

/* fichiers inclus */


#include "xdc.h"
#include "xdy.h"
#include "xzst.h"
#include "xzao19.h"
#include "xzat.h"
#include "xzad04.h"
#include "xzea.h"


/* definitions de constantes exportees */
#define CLASSE_ZDP_TFM		"zdp_tfm"


#define ATT_ZDP_AUTOROUTE	"autoroute"
#define ATT_ZDP_SENS		"sens"
#define ATT_ZDP_LIBSENS		"libelle_sens"
#define ATT_ZDP_PTAMONT		"numero_point_amont"
#define ATT_ZDP_LIBAMONT	"libelle_point_amont"
#define ATT_ZDP_PTAVAL		"numero_point_aval"
#define ATT_ZDP_LIBAVAL		"libelle_point_aval"
#define ATT_ZDP_LONGUEUR	"longueur"
#define ATT_ZDP_TDP		"tdp"
#define ATT_ZDP_PERTURBE	"passage_perturbe"
#define ATT_ZDP_NORMAL		"retour_normale"
#define ATT_ZDP_PR		"pr"
#define ATT_ZDP_DESTINATION	"destination"
#define ATT_ZDP_FOURCHE		"zone_fourche"
#define ATT_ZDP_NUMEROZONE	"numero_zone"
#define ATT_ZDP_PR_ALT		"pr_alt"
#define ATT_ZDP_DESTINATION_ALT	"destination_alt"


/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */
extern int vg_num_zdp_pmv;
extern XDY_District     vg_site;


/* delaration de fonctions externes */
extern int drol_tdp_init();

extern void tdp_candidate();

extern char *calculer_tdp_pmv_dest(	XDY_PR va_pr_pmv, XDY_Autoroute va_autoroute, 
				XDY_Sens va_sens, XDY_Entier va_destination);


#endif
