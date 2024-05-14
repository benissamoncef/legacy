/*E*/
/* Fichier : $Id: drol_bad.h,v 1.2 2011/02/17 19:25:49 gesconf Exp $        Release : $Revision: 1.2 $        Date : $Date: 2011/02/17 19:25:49 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME SAIDEC
------------------------------------------------------
* MODULE DROL_BAD * FICHIER drol_bad.h
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
* JMG   12/11/04	: ajout extern vg_num_poste
* JMG	06/12/04	: ajout mode auto ou manuel ; archivee  1.1
* JPL	17/02/11	: Migration architecture HP ia64 (DEM 971) : declaration drol_bad_filtre  1.2
------------------------------------------------------ */

#ifndef DROL_BAD
#define DROL_BAD

#include "xdc.h"
#include "xdy.h"
#include "xzst.h"
#include "xzat.h"
#include "xzia.h"
#include "xzao500.h"
#include "xzap26.h"
#include "xzap33.h"

#include "drol_tfm.h"

typedef struct S_PERT_BAD {
        char            Libelle[200];
        T_CONDITION     *Condition;
	int		Automatique;
	int		DistanceMax;
        struct S_PERT_BAD  *Suivant;
} T_PERT_BAD;


/* declarations de donnees globales */
typedef struct {
int     Identifiant;
int     District;
int     Distance;
int	Type;
char        Scenario_Courant[10];
char        Scenario[10];
XDY_Booleen     Dispo;
int     NumEvt;
int     CleEvt;
int     Priorite;
int     DistanceEvt;
} T_BAD;

extern XDY_Mot          vg_numero_plan;
extern XDY_District     vg_site;
extern int		vg_num_poste;
extern XDY_Mot          vg_ordre_bad;


/* delaration de fonctions externes */
extern int drol_bad_init();
extern XDY_Booleen drol_bad_filtre(T_NOEUD *, XDY_District, XDY_Horodate);

#endif
