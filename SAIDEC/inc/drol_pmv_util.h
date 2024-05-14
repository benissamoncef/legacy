/*
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME SAIDEC
------------------------------------------------------
* MODULE DROL_PMV * FICHIER drol_pmv_util.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   fonctions C utilisees dans les regles RTie de choix
*   des PMV et d'elaboration des messages PMV.
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	2002	creation	1.1
* JMG	30/03/04	ajout picto et bandeau si bifurcation	1.2
* JPL	18/02/11 : Migration architecture HP ia64 (DEM 971) : constantes et types SPECIFIQUES PMV  1.5
* VR    25/11/11 : Ajout destination3 dans la declaration des parametres. (DEM/1014) 1.6
* JMG	23/11/16 : PMV SAGA 1.7
* JMG	26/06/17 : convergence 1.8
* JMG	12/08/18 : ajout type pmv . suppression SAGA DEM1306 1.9
* GGY	28/07/23 : Ajout Flash à T_PERT (DEM473)
* GGY	28/11/23 : Remplacement variable PMV SAGA pour PMV CNA 1 et 2 lignes
* GGY	28/07/23 : Ajout Flash à T_PERT (DEM473)
* JPL	08/11/23 : Ajout inclusion de "string.h"
------------------------------------------------------ */

#ifndef DROL_PMV_UTIL
#define DROL_PMV_UTIL

/* fichiers inclus */

#include <string.h>

#include "xdc.h"
#include "xdy.h"
#include "xzst.h"
#include "xzao15.h"
#include "xzao16.h"
#include "xzat.h"
#include "xzia.h"
#include "xzao.h"

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
	XDY_Autoroute	autoroute_influ;
	XDY_Sens	sens_influ;
	XDY_PR		pr_debut_influ;
	XDY_PR		pr_fin_influ;
	char		direction[20];
	XDY_Booleen	pmv_traite;
	XDY_PR		pr_1_pmv_amont;
	XDY_Sens	sens_1_pmv_amont;
	XDY_Autoroute	autoroute_1_pmv_amont;
	XDY_Entier	distance_1_pmv_amont;
	XDY_PR		pr_1_pmv_amont_bis;
	XDY_Sens	sens_1_pmv_amont_bis;
	XDY_Autoroute	autoroute_1_pmv_amont_bis;
	XDY_Entier	distance_1_pmv_amont_bis;
	XDY_PR		pr_1_pmv_amont_ter;
	XDY_Sens	sens_1_pmv_amont_ter;
	XDY_Autoroute	autoroute_1_pmv_amont_ter;
	XDY_Entier	distance_1_pmv_amont_ter;
} T_INFLU ;

typedef struct S_T_INFLU {
	T_INFLU		zone;
	struct S_T_INFLU 	*suivant;
	struct S_T_INFLU 	*precedent;
} T_Liste_Influ;


typedef struct {
	char    Texte[30];
	char    *(*Fct)();
} T_LIGNE_PMV;

typedef struct S_PERT {
        char            Libelle[200];
        int             Priorite;
        T_LIGNE_PMV     Ligne1;
        T_LIGNE_PMV     Ligne2;
        T_LIGNE_PMV     Ligne3;
        XDY_Booleen     L_Vide;
        T_LIGNE_PMV     Ligne1_Fin;
        T_LIGNE_PMV     Ligne2_Fin;
        T_LIGNE_PMV     Ligne3_Fin;
        XDY_Booleen     L_Fin_Vide;
        T_LIGNE_PMV     Bifurc1;
        T_LIGNE_PMV     Bifurc2;
        T_LIGNE_PMV     Bifurc3;
        XDY_Booleen     B_Vide;
        T_LIGNE_PMV     Bifurc1_Fin;
        T_LIGNE_PMV     Bifurc2_Fin;
        T_LIGNE_PMV     Bifurc3_Fin;
        XDY_Booleen     B_Fin_Vide;
        T_LIGNE_PMV     Bandeau;
        char            Pmv1[100];
        char            Pmv2[100];
        char            Picto[10];
        int             DMax1;
        int             DMax1_Bifurc;
        int             DMax2;
        int             DMax2_Bifurc;
        char            Picto_Bifurcation[10]; /*1.2*/
        T_LIGNE_PMV     Bandeau_Bifurcation; /*1.2*/
        T_CONDITION     *Condition;
	int		Auto; /*1.3*/
	T_LIGNE_PMV	Ligne1_1_15;
	T_LIGNE_PMV	Ligne1_2_15;
	T_LIGNE_PMV	Ligne2_2_15;
	int		Clignotement;
	XDY_Booleen Flash;
        struct S_PERT  *Suivant;
} T_PERT;

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
	XDY_Booleen	Presence_Bifurcation;
	char		Direction[20];
	char		Bandeau[20];
	XDY_Booleen	Dispo_Texte;
	XDY_Booleen	Dispo_Picto;
	char		Ligne1[20];
	char		Ligne2[20];
	char		Ligne3[20];
	char		Alternat1[20];
	char		Alternat2[20];
	char		Alternat3[20];
	char		Picto[30];
	int		Numero_Evt_U;
	int		Cle_Evt_U;
	int		Distance_U;
	int		Priorite_U;
	char		Nom_Pert[200];
	XDY_Booleen	Utilise;
	T_PERT		*Perturbation;
	int		Destination1;
	int		Destination2;
	int		Destination3;
	XDY_Booleen	PMV_SAGA;
	int		Type_SAGA;
	XDY_Octet	Type;
} T_PMV;

extern XZAOT_TypePMV	vg_type_pmv[25];

/* definition de macro exportees */

/* declarations de donnees globales */
#define C_NB_MAX_PMV	100
extern T_PMV	vg_pmv[C_NB_MAX_PMV];
extern T_PMV	vg_pmv_saga[C_NB_MAX_PMV];
extern int	vg_priorite_retenue;

/* delaration de fonctions externes */
extern void     tous_les_PMV_amont( XDY_Autoroute ,
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
	T_PERT		*);

extern void le_second_PMV_amont(XDY_Autoroute ,
        XDY_PR         , 
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
        T_PERT          *);

extern void     le_PMV_amont_aire( XDY_Autoroute ,
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
	T_PERT		*);

extern void     le_PMV_amont_sortie( XDY_Autoroute ,
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
	T_PERT		*);

extern void     les_PMV_ds_perturbation( XDY_Autoroute ,
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
	T_PERT		*);

extern void     le_PMV_sur_la_chaussee_opposee( XDY_Autoroute ,
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
	T_PERT		*);

extern char *pmv_mise_en_page(char*, int);
extern XDY_Booleen pmv_ligne_trop_longue(char *, int);
extern void     les_2_PMV_eligibles_en_amont( XDY_Autoroute ,
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
	T_PERT		*);

extern int pmv_init_vg_requetes();
extern int pmv_libere_ressources();
extern int pmv_init_appels_base();
extern void texte_pmv_de_base(char *);
extern void le_PMV_null();

extern void     le_premier_PMV_amont( XDY_Autoroute ,
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
	T_PERT		*);

extern void pmv_tabuler_ligne(char *, int, char *);
extern char *util_bool_to_str(XDY_Booleen, char *, char *);
extern XDY_Booleen util_pmv_amont_fmc(int, int, int, int, int, int);
extern XDY_Booleen util_pmv_amont_sortie(int, int, int, int, int, int);

#endif
