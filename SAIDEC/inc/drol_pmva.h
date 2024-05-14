/*E*/
/* Fichier : $Id: drol_pmva.h,v 1.2 2011/02/18 17:05:55 gesconf Exp $        Release : $Revision: 1.2 $        Date : $Date: 2011/02/18 17:05:55 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME SAIDEC
------------------------------------------------------
* MODULE DROL_PMVA * FICHIER drol_pmva.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*
------------------------------------------------------
* HISTORIQUE :
* JMG 20/06/05	creation
* JPL	18/02/11 : Migration architecture HP ia64 (DEM 971) : variables SPECIFIQUES PMVA; declar. drol_pmva_filtre  1.2
------------------------------------------------------ */

#ifndef DROL_PMVA
#define DROL_PMVA


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
#include "xzap28.h"
#include "xzit.h"

#include "drol_pmva_util.h"

/* declarations de donnees globales */

extern T_PERT_PMVA 	*vg_pert_pmva;

extern XDY_Mot		vg_numero_plan;
extern XDY_District	vg_site;
extern int		vg_num_poste;
extern XDY_Mot		vg_ordre_pmva;

/* delaration de fonctions externes */
extern int drol_pmva_init();
extern XDY_Booleen drol_pmva_filtre(T_NOEUD *, XDY_District, XDY_Horodate);

#endif
