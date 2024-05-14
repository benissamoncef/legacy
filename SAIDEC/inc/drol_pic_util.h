/* Fichier : $Id: drol_pic_util.h
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME SAIDEC
------------------------------------------------------
* MODULE DROL_PIC * FICHIER drol_pic_util.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   fonctions C utilisees dans les regles de choix
*   des PIC et d'elaboration des messages PIC.
*
------------------------------------------------------
* HISTORIQUE :
*
* GGY	01/12/23	: Creation (DEM483)
----------------------------------------------------- */
#ifndef DROL_PIC_UTIL
#define DROL_PIC_UTIL

#include "xdc.h"
#include "xdy.h"
#include "xzst.h"
//#include "xzao15.h"
//#include "xzao16.h"
//#include "xzao443.h"
#include "xzat.h"
#include "xzia.h"

#include "drol_tfm.h"

/* definitions de constantes exportees */

/* definitions de types exportes */
//typedef struct {
//	XDY_Eqt		numero_zone;
//	XDY_Eqt		numero_pic;
//	XDY_Autoroute	autoroute_pic;
//	XDY_Sens	sens_pic;
//	XDY_PR		pr_pic;
//	XDY_Octet	district_pic;
//	XDY_Autoroute	autoroute_zone;
//	XDY_Sens	sens_zone;
//	XDY_PR		pr_debut_zone;
//	XDY_PR		pr_fin_zone;
//	char		direction[20];
//	XDY_Octet	type_zone;
//} T_ZONE_PIC ;

//typedef struct S_T_ZONE_PIC {
//	T_ZONE_PIC		zone;
//	struct S_T_ZONE_PIC 	*suivant;
//	struct S_T_ZONE_PIC 	*precedent;
//} T_Liste_Zone_PIC;

typedef struct {
	char    Texte[30];
	char    *(*Fct)();
} T_LIGNE_PIC;

typedef struct S_PERT_PIC {
	char            	Libelle[200];
	int             	Priorite;
	XDY_Bandeau_PIC		Bandeau;		
	XDY_Booleen			Flash;			
	XDY_Picto_PIC		Picto;
	XDY_Picto_PIC		PIC_Dans_Perturbation;	
	int					Automatique;
	T_CONDITION     	*Condition;
	int					Type_Zone;
	struct S_PERT_PIC  *Suivant;
} T_PERT_PIC;

typedef struct {
	int				Identifiant;
	char			Nom[30];
	XDY_Autoroute	Autoroute;
	XDY_PR			PR;
	XDY_Sens		Sens;
	int				Distance_C;
	int				District;
	int				Amont;
	int				Priorite_C;
	XDY_Booleen		Realisable;
	XDY_Booleen		Dispo_Texte;
	XDY_Bandeau_PIC	Bandeau;
	XDY_Booleen		Flash;	
	XDY_Picto_PIC	Picto;
	int				Numero_Evt_U;
	int				Cle_Evt_U;
	int				Distance_U;
	int				Priorite_U;
	char			Nom_Pert[200];
	XDY_Booleen		Utilise;
	int				Destination;
	T_PERT_PIC		*Perturbation;
} T_PIC;

/* definition de macro exportees */

/* declarations de donnees globales */
#define C_NB_MAX_PIC	100
extern T_PIC	vg_pic [C_NB_MAX_PIC];
extern int		vg_priorite_retenue;

extern char *pic_mise_en_page (char*, int);
extern int	pic_init_vg_requetes ();
extern int	pic_libere_ressources ();
extern int	pic_init_appels_base ();
extern void texte_pic_de_base ();
extern void le_PIC_null ();

extern void les_PIC_ds_perturbation(
	XDY_Autoroute,
	XDY_PR,
	XDY_PR,
	XDY_Sens,
	XDY_Horodate,
	XDY_District,
	int,
	char *,
	T_PERT_PIC *);

extern void	les_PIC_du_basculement(
	XDY_Autoroute	Autoroute,
	XDY_PR			PR,
	XDY_PR			PR_tete,
	XDY_Sens		Sens,
	XDY_Horodate	Horodate,
	XDY_District	District,
	int				Priorite,
	char			*Perturbation,
	T_PERT_PIC		*va_pert);

#endif
