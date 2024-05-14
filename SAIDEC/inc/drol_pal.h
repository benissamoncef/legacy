/*E*/
/* Fichier : $Id: drol_pal.h,v 1.4 2011/02/17 19:33:33 gesconf Exp $        Release : $Revision: 1.4 $        Date : $Date: 2011/02/17 19:33:33 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME SAIDEC
------------------------------------------------------
* MODULE DROL_PAL * FICHIER drol_pal.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   fonctions C utilisees dans les regles RTie de choix
*   des PMV et d'elaboration des messages PMV.
*
------------------------------------------------------
* HISTORIQUE :
*
* IT    08 Sep 1994     : Creation
* JMG   12/11/04	: ajout extern vg_num_poste 1.2
* JMG	06/12/04	: ajout mode auto ou manuel
* JPL	17/02/11	: Migration architecture HP ia64 (DEM 971) : declaration drol_pal_init, drol_pal_filtre  1.4
------------------------------------------------------ */

#ifndef DROL_PAL
#define DROL_PAL

#include "xdc.h"
#include "xdy.h"
#include "xzst.h"
#include "xzat.h"
#include "xzia.h"
#include "xzao500.h"
#include "xzap26.h"

#include "drol_tfm.h"

typedef struct S_PERT_PAL {
        char            Libelle[200];
        char             Scenario[10];
        char             Nom_Scenario[80];
        T_CONDITION     *Condition;
	int		Mode;
        struct S_PERT_PAL  *Suivant;
} T_PERT_PAL;


/* declarations de donnees globales */
typedef struct {
int     Identifiant;
int     District;
int     Distance;
T_STRING        Scenario_Courant;
T_STRING        Scenario;
T_STRING        Nom_Scenario;
XDY_Booleen     Dispo;
int     NumEvt;
int     CleEvt;
int     Priorite;
int     DistanceEvt;
} T_PAL;

extern XDY_Mot          vg_numero_plan;
extern XDY_District     vg_site;
extern int		vg_num_poste;
extern XDY_Mot          vg_ordre_pal;


/* delaration de fonctions externes */
extern int drol_pal_init();
extern XDY_Booleen drol_pal_filtre(T_NOEUD *, XDY_District, XDY_Horodate);

#endif
