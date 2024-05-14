/*E*/
/* Fichier : $Id: drol_pmva_util.h,v 1.8 2020/12/22 07:19:05 gesconf Exp $        Release : $Revision: 1.8 $        Date : $Date: 2020/12/22 07:19:05 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME SAIDEC
------------------------------------------------------
* MODULE DROL_PMVA * FICHIER drol_pmva_util.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   fonctions C utilisees dans les regles de choix
*   des PMVA et d'elaboration des messages PMVA.
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	20/06/05	: Creation
* JMG	10/05/07	ajout presence bifurcation
* PNI	29/09/10	: modif constante C_NB_MAX_PMVA v1.4 DEM 958
* JPL	18/02/11 : Migration architecture HP ia64 (DEM 971) : constantes et types SPECIFIQUES PMVA  1.5
* VR	08/06/12 : Ajout des destinations dans T_PMVA (DEM/1014 PMA)
* PNI	22/12/20	: modif constante C_NB_MAX_PMVA v1.6 
----------------------------------------------------- */

#ifndef DROL_PMVA_UTIL
#define DROL_PMVA_UTIL


#include "xdc.h"
#include "xdy.h"
#include "xzst.h"
#include "xzao15.h"
#include "xzao16.h"
#include "xzao443.h"
#include "xzat.h"
#include "xzia.h"

#include "drol_tfm.h"

/* definitions de constantes exportees */


/* definitions de types exportes */
typedef struct {
	XDY_Eqt		numero_zone;
	XDY_Eqt		numero_pmv;
	XDY_Autoroute	autoroute_pmv;
	XDY_Sens	sens_pmv;
	XDY_PR		pr_pmv;
	XDY_Octet	district_pmv;
	XDY_Autoroute	autoroute_zone;
	XDY_Sens	sens_zone;
	XDY_PR		pr_debut_zone;
	XDY_PR		pr_fin_zone;
	char		direction[20];
	XDY_Octet	type_zone;
} T_ZONE ;

typedef struct S_T_ZONE {
	T_ZONE		zone;
	struct S_T_ZONE 	*suivant;
	struct S_T_ZONE 	*precedent;
} T_Liste_Zone;


typedef struct {
	char    Texte[30];
	char    *(*Fct)();
} T_LIGNE_PMVA;

typedef struct S_PERT_PMVA {
        char            Libelle[200];
        int             Priorite;
        T_LIGNE_PMVA    Ligne1;
        T_LIGNE_PMVA    Ligne2;
        T_LIGNE_PMVA    Ligne3;
        T_LIGNE_PMVA    Ligne4;
        T_LIGNE_PMVA    Ligne5;
        T_LIGNE_PMVA    Alternat1;
        T_LIGNE_PMVA    Alternat2;
        T_LIGNE_PMVA    Alternat3;
        T_LIGNE_PMVA    Alternat4;
        T_LIGNE_PMVA    Alternat5;
        XDY_Octet	Flash;
        XDY_Octet	PMVA_Dans_Perturbation;	
        XDY_Octet	Automatique;
        T_CONDITION     *Condition;
        int	Type_Zone;
        struct S_PERT_PMVA  *Suivant;
} T_PERT_PMVA;

typedef struct {
	int		Identifiant;
	char		Nom[30];
	XDY_Autoroute	Autoroute;
	XDY_PR		PR;
	XDY_Sens	Sens;
	int		Distance_C;
	int	District;
	int		Priorite_C;
	XDY_Booleen	Realisable;
	XDY_Booleen	Dispo_Texte;
	char		Ligne1[20];
	char		Ligne2[20];
	char		Ligne3[20];
	char		Ligne4[20];
	char		Ligne5[20];
	char		Alternat1[20];
	char		Alternat2[20];
	char		Alternat3[20];
	char		Alternat4[20];
	char		Alternat5[20];
	XDY_Booleen	Flash;
	char		Direction[20];
	int		Numero_Evt_U;
	int		Cle_Evt_U;
	int		Distance_U;
	int		Priorite_U;
	char		Nom_Pert[200];
	XDY_Booleen	Utilise;
	XDY_Booleen     Presence_Bifurcation;
	int		Dest_un_proche;
	int		Dest_un_loin;
	int		Dest_deux_proche;
	int		Dest_deux_loin;
	T_PERT_PMVA	Perturbation;
	XDY_Booleen	PMVA_SAGA;
} T_PMVA;

/* definition de macro exportees */

/* declarations de donnees globales */
#define C_NB_MAX_PMVA	200
extern T_PMVA	vg_pmva[C_NB_MAX_PMVA];
extern T_PMVA	vg_pmva_saga[C_NB_MAX_PMVA];
extern int	vg_priorite_retenue;

extern char *pmva_mise_en_page(char*, int);


extern int pmva_init_vg_requetes();
extern int pmva_libere_ressources();
extern int pmva_init_appels_base();
extern void texte_pmva_de_base();
extern void le_PMVA_null();

extern void     les_PMVA_amont( XDY_Autoroute ,
	XDY_PR		,
	XDY_PR		,
	XDY_Sens	,
	XDY_Horodate	,
	XDY_District	,
	int		,
	char		*,
	T_PERT_PMVA	);


extern void     les_PMVA_ds_perturbation( XDY_Autoroute ,
        XDY_PR          ,
        XDY_PR          ,
        XDY_Sens        ,
        XDY_Horodate    ,
        XDY_District    ,
        int             ,
        char    *,
        int             ,
        int             ,
        XDY_PR          ,
        XDY_PR          ,
        XDY_PR          ,
        XDY_PR          ,
        XDY_Autoroute   ,
        XDY_Autoroute   ,
        XDY_Autoroute   ,
        XDY_Autoroute   ,
        XDY_PR          ,
        XDY_PR          ,
        XDY_PR          ,
        XDY_PR          ,
        T_PERT_PMVA     );


#endif
