/*E*/
/* Fichier : $Id: drol_prv_util.h,v 1.1 2012/07/04 10:18:21 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 10:18:21 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME SAIDEC
------------------------------------------------------
* MODULE DROL_PRV * FICHIER drol_prv_util.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   fonctions C utilisees dans les regles de choix
*   des PRV et d'elaboration des messages PRV.
*
------------------------------------------------------
* HISTORIQUE :
*
* VR	20/12/11		: Creation (DEM/1016)
----------------------------------------------------- */

#ifndef DROL_PRV_UTIL
#define DROL_PRV_UTIL


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
	XDY_Eqt		numero_prv;
	XDY_Autoroute	autoroute_prv;
	XDY_Sens	sens_prv;
	XDY_PR		pr_prv;
	XDY_Octet	district_prv;
	XDY_Autoroute	autoroute_zone;
	XDY_Sens	sens_zone;
	XDY_PR		pr_debut_zone;
	XDY_PR		pr_fin_zone;
	char		direction[20];
	XDY_Octet	type_zone;
} T_ZONE_PRV ;

typedef struct S_T_ZONE_PRV {
	T_ZONE_PRV		zone;
	struct S_T_ZONE_PRV 	*suivant;
	struct S_T_ZONE_PRV 	*precedent;
} T_Liste_Zone_PRV;


typedef struct {
	char    Texte[30];
	char    *(*Fct)();
} T_LIGNE_PRV;

typedef struct S_PERT_PRV {
        char            Libelle[200];
        int             Priorite;
        char    Vitesse[6];
        char    Bandeau[6];
        XDY_Octet	PRV_Dans_Perturbation;	
        int	Automatique;
        T_CONDITION     *Condition;
        int	Type_Zone;
        struct S_PERT_PRV  *Suivant;
} T_PERT_PRV;

typedef struct {
	int		Identifiant;
	char		Nom[30];
	XDY_Autoroute	Autoroute;
	XDY_PR		PR;
	XDY_Sens	Sens;
	int		Distance_C;
	int	District;
	int	Amont;
	int		Priorite_C;
	XDY_Booleen	Realisable;
	XDY_Booleen	Dispo_Texte;
	char		Vitesse[6];
	char		Bandeau[6];
	int		Numero_Evt_U;
	int		Cle_Evt_U;
	int		Distance_U;
	int		Priorite_U;
	char		Nom_Pert[200];
	XDY_Booleen	Utilise;
	int		Destination;
	T_PERT_PRV	*Perturbation;
} T_PRV;

/* definition de macro exportees */

/* declarations de donnees globales */
#define C_NB_MAX_PRV	100
extern T_PRV	vg_prv[C_NB_MAX_PRV];
extern int	vg_priorite_retenue;

extern char *prv_mise_en_page(char*, int);


extern int prv_init_vg_requetes();
extern int prv_libere_ressources();
extern int prv_init_appels_base();
extern void texte_prv_de_base();
extern void le_PRV_null();

extern void     les_PRV_ds_perturbation( XDY_Autoroute ,
        XDY_PR          ,
        XDY_PR          ,
        XDY_Sens        ,
        XDY_Horodate    ,
        XDY_District    ,
        int             ,
        char    *,
        T_PERT_PRV  *   );

extern void	les_PRV_du_basculement( XDY_Autoroute Autoroute,
	XDY_PR		PR,
	XDY_PR		PR_tete,
	XDY_Sens	Sens,
	XDY_Horodate	Horodate,
	XDY_District	District,
	int		Priorite,
	char	*Perturbation,
	T_PERT_PRV	*va_pert);

#endif
