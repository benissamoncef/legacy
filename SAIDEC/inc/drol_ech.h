/*E Fichier : $Id: drol_ech.h,v 1.2 2011/02/17 19:28:37 gesconf Exp $     Release : $Revision: 1.2 $        Date : $Date: 2011/02/17 19:28:37 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME SAIDEC
------------------------------------------------------
* MODULE DROL_ECH * FICHIER drol_ech.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*
------------------------------------------------------
* HISTORIQUE :
* JMG 31/01/01	creation
* JPL	17/02/11 : Migration architecture HP ia64 (DEM 971) : declaration drol_ech_filtre  1.2
------------------------------------------------------ */

#ifndef DROL_ECH
#define DROL_ECH

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

#include "drol_tfm.h"

/* declarations de donnees globales */
typedef struct {
int	Numero;
int	District;
int	Scenario;
int	Dispo;
int	NumEvtU;
int	CleEvtU;
int	PrioriteU;
int	DistanceEvtU;
int	ScenarioC;
int	DistanceEvtC;
int	PrioriteEvtC;
} T_ECH;

extern XDY_Mot		vg_numero_plan;
extern XDY_District	vg_site;
extern XDY_Mot		vg_ordre_ech;

/* delaration de fonctions externes */
extern int drol_ech_init();
extern XDY_Booleen drol_ech_filtre(T_NOEUD *, XDY_District, XDY_Horodate);

#endif
