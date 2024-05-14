/*E*/
/* Fichier : $Id: drol_baf.h,v 1.3 2017/07/04 16:43:27 pc2dpdy Exp $        Release : $Revision: 1.3 $        Date : $Date: 2017/07/04 16:43:27 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME SAIDEC
------------------------------------------------------
* MODULE DROL_BAF * FICHIER drol_baf.h
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
* JPL	17/02/11	: Migration architecture HP ia64 (DEM 971) : declaration drol_baf_init, drol_baf_filtre  1.2
* JMG	26/11/16 : ajout filtre peage amont lot 23 1.3
------------------------------------------------------ */

#ifndef DROL_BAF
#define DROL_BAF

#include "xdc.h"
#include "xdy.h"
#include "xzst.h"
#include "xzat.h"
#include "xzia.h"
#include "xzao500.h"
#include "xzap26.h"

#include "drol_tfm.h"

typedef struct S_PERT_BAF {
        char            Libelle[200];
        T_CONDITION     *Condition;
	int		Automatique;
	int		DistanceMax;
	int		Filtre_peage;
        struct S_PERT_BAF  *Suivant;
} T_PERT_BAF;


/* declarations de donnees globales */
typedef struct {
int     Identifiant;
int     District;
int     Distance;
int	Type;
int        Module11_Courant;
int        Module12_Courant;
int        Module15_Courant;
int        Module21_Courant;
int        Module22_Courant;
int        Module25_Courant;
int        Module11;
int        Module12;
int        Module15;
int        Module21;
int        Module22;
int        Module25;
XDY_Booleen     Dispo;
int     NumEvt;
int     CleEvt;
int     Priorite;
int     DistanceEvt;
} T_BAF;

extern XDY_Mot          vg_numero_plan;
extern XDY_District     vg_site;
extern int		vg_num_poste;
extern XDY_Mot          vg_ordre_baf;


/* delaration de fonctions externes */
extern int drol_baf_init();
extern XDY_Booleen drol_baf_filtre(T_NOEUD *, XDY_District, XDY_Horodate);

#endif
