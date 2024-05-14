/*E*/
/* Fichier : $Id: drol_prv.h,v 1.1 2012/07/04 10:18:17 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 10:18:17 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME SAIDEC
------------------------------------------------------
* MODULE DROL_PMVA * FICHIER drol_prv.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*
------------------------------------------------------
* HISTORIQUE :
* VR	20/12/11		: Creation (DEM/1016)
------------------------------------------------------ */

#ifndef DROL_PRV
#define DROL_PRV


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

#include "drol_prv_util.h"

/* declarations de donnees globales */

extern T_PERT_PRV 	*vg_pert_prv;

extern XDY_Mot		vg_numero_plan;
extern XDY_District	vg_site;
extern int		vg_num_poste;
extern XDY_Mot		vg_ordre_prv;

/* delaration de fonctions externes */
extern int drol_prv_init();
extern XDY_Booleen drol_prv_filtre(T_NOEUD *, XDY_District, XDY_Horodate);

#endif
