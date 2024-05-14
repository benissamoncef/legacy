/* Fichier : $Id: drol_pic.h
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME SAIDEC
------------------------------------------------------
* MODULE DROL_PMVA * FICHIER drol_pic.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
------------------------------------------------------
* HISTORIQUE :
* GGY	01/12/23	: Creation (DEM483)
------------------------------------------------------ */
#ifndef DROL_PIC
#define DROL_PIC

/* fichiers inclus */
/* include system */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "xdc.h"
#include "xdy.h"
#include "xzst.h"
#include "xzsm.h"
#include "xzit.h"

#include "drol_pic_util.h"

/* declarations de donnees globales */
extern T_PERT_PIC 	*vg_pert_pic;
extern XDY_Mot		vg_numero_plan;
extern XDY_District	vg_site;
extern int			vg_num_poste;
extern XDY_Mot		vg_ordre_pic;

/* delaration de fonctions externes */
extern int 			drol_pic_init ();
extern XDY_Booleen	drol_pic_filtre (T_NOEUD *, XDY_District, XDY_Horodate);

#endif
