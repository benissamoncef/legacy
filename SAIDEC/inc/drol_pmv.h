/*E*/
/* Fichier : $Id: drol_pmv.h,v 1.3 2011/02/18 17:02:28 gesconf Exp $        Release : $Revision: 1.3 $        Date : $Date: 2011/02/18 17:02:28 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME SAIDEC
------------------------------------------------------
* MODULE DROL_PMV * FICHIER drol_pmv.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*
------------------------------------------------------
* HISTORIQUE :
* JMG 31/01/01	creation
* JPL	18/02/11 : Migration architecture HP ia64 (DEM 971) : declaration drol_pmv_filtre  1.3
------------------------------------------------------ */

#ifndef DROL_PMV
#define DROL_PMV


/* fichiers inclus */
/* include system */
#include        <stdio.h>
#include        <string.h>
#include        <ctype.h>
#include        <stdlib.h>

#include "xdc.h"
#include "xdy.h"
#include "xzst.h"
#include "xzsm.h"
#include "xzao000.h"
#include "xzit.h"

#include "drol_pmv_util.h"

/* declarations de donnees globales */

extern T_PERT 	*vg_pert;

extern XDY_Mot		vg_numero_plan;
extern XDY_District	vg_site;
extern XDY_Mot		vg_ordre_pmv;
extern int		vg_num_poste;

/* delaration de fonctions externes */
extern int drol_pmv_init();
extern XDY_Booleen drol_pmv_filtre(T_NOEUD *, XDY_District, XDY_Horodate);

#endif
