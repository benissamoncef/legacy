/*E*/
/* Fichier : $Id: drol_nav.h,v 1.4 2017/01/10 11:37:56 devgfi Exp $        Release : $Revision: 1.4 $        Date : $Date: 2017/01/10 11:37:56 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME SAIDEC
------------------------------------------------------
* MODULE DROL_NAV * FICHIER drol_nav.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*
------------------------------------------------------
* HISTORIQUE :
* JMG 31/01/01	creation 1.1
* JMG	14/11/05	passage en mode auto 1.2
* JPL	17/02/11 : Migration architecture HP ia64 (DEM 971) : declaration drol_nav_filtre  1.3
* JPL	05/12/16 : Ajout fonction drol_nav_filtre_pal; ajout nom scenario et conditions aux donnees perturbation (DEM 1193)  1.4
------------------------------------------------------ */

#ifndef DROL_NAV
#define DROL_NAV

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
int	Identifiant;
int	District;
int	Distance;
int	Type;
T_STRING	Mode_Courant;
T_STRING	Mode;
XDY_Booleen	Dispo;
int	NumEvt;
int	CleEvt;
int	Priorite;
int	DistanceEvt;
int	Auto;
} T_BRA;

typedef struct S_PERT_BRA {
	char		Libelle[80];
	char		Scenario[10];
	char		Nom_Scenario[80];
	char		Conditions[80];
	int		Auto;
	struct S_PERT_BRA *Suivant;
} T_PERT_BRA;

extern XDY_Mot		vg_numero_plan;
extern XDY_District	vg_site;
extern XDY_Mot		vg_ordre_nav;
extern int           vg_num_poste;

/* delaration de fonctions externes */
extern int drol_nav_init();
extern XDY_Booleen drol_nav_filtre(T_NOEUD *, XDY_District, XDY_Horodate);
extern XDY_Booleen drol_nav_filtre_pal(T_NOEUD *, XDY_District, XDY_Horodate);

#endif
