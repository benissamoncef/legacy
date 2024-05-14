/*E*/
/* Fichier : $Id: drol_tun.h,v 1.6 2021/06/03 07:55:17 pc2dpdy Exp $        Release : $Revision: 1.6 $        Date : $Date: 2021/06/03 07:55:17 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME SAIDEC
------------------------------------------------------
* MODULE DROL_TUN * FICHIER drol_tun.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   fonctions C utilisees dans les regles RTie de choix
*   des PMV et d'elaboration des messages PMV.
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG 	22/09/08	creation
* JMG	22/10/09	pastun DEM895
* JPL	17/02/11 : Migration architecture HP ia64 (DEM 971) : declaration drol_tun_init, drol_tun_filtre  1.3
* JMG   15/06/20	AMA: en accord mail: mise en conf de l'ajout de structure pour la GTC (mail 11/06/20)     
* ABE	03/06/21 : Ajout RAU SONO DEM-SAE244 1.5
------------------------------------------------------ */

#ifndef DROL_TUN
#define DROL_TUN

#include "xdc.h"
#include "xdy.h"
#include "xzst.h"
#include "xzat.h"
#include "xzia.h"
#include "xzao500.h"
#include "xzap26.h"
#include "xzeu.h"

#include "drol_tfm.h"

typedef struct {
int     Numero;
char	Nom[30];
int     District;
int     Sequence;
int     SequenceNat;
int     SequenceVEN;
int     SequenceECL;
int     Dispo;
int     NumEvtU;
int     CleEvtU;
int     PrioriteU;
int     DistanceEvtU;
int     SequenceC;
int     SequenceC_Nat;
int     DistanceEvtC;
int     PrioriteEvtC;
char    Libelle[100];
char    LibelleNat[100];
XDY_Octet	Automatique;
XDY_Booleen Renove;
XDY_Voie VR;
XDY_Voie VM2;
XDY_Voie VM1;
XDY_Voie VL;
XDY_Voie BAU;
} T_TUNNEL;
typedef struct S_ZONE_GTC {
	int	ID;
        char            Libelle[50];
	int	Autoroute;
	int	Sens;
	int	Debut;
	int	Fin;
	int		Type_Zone;
	struct S_ZONE_GTC *suivant;
} T_ZONE_GTC;

typedef struct S_Scenario {
	int	IdZone;
        char            Libelle[50];
	int	Scenario;
	char		Scenario_Nom[100];
	int		Priorite;
	int	Automatique;
	int	Scenario_Oppose;
	char		Scenario_Oppose_Nom[100];
	char		Groupe[5];
	int		Filtre_PSTT;
	int	Scenario_Nat;
	char		Scenario_Nat_Nom[100];
	int	Scenario_Nat_Oppose;
	char	Scenario_Nat_Oppose_Nom[100];
	T_ZONE_GTC	*Zone;
	struct S_Scenario *suivant;
} T_Scenario;

typedef struct S_PERT_GTC {
        char            Libelle[200];
        T_CONDITION     *Condition;
	int		Priorite;
	T_Scenario	*Scenarios;
	struct S_PERT_GTC *suivant;
} T_PERT_GTC;



typedef struct S_PERT_TUN {
        char            Libelle[200];
        T_CONDITION     *Condition;
	int		Automatique;
	int		Priorite;
	int		Type_Zone;
	char		Groupe[5];
	int		Sequence;
	int		Filtre_PSTT;
	XDY_Nom		Sequence_Nom;
	int		Sequence_Nat;
	XDY_Nom		Sequence_Nat_Nom;	
	int		Sequence_Opp;
	XDY_Nom		Sequence_Opp_Nom;
	int		Sequence_Nat_Opp;
	XDY_Nom		Sequence_Nat_Opp_Nom;
	int		Saga_Sud;
	int		Saga_Nord;
        struct S_PERT_TUN  *Suivant;
} T_PERT_TUN;

typedef struct {
	XDY_Nom		NomBase;
	XDY_Nom		Libelle;
} T_CFG_TUNNEL;
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
} T_TUN;

extern XDY_Mot          vg_numero_plan;
extern XDY_District     vg_site;
extern int		vg_num_poste;
extern XDY_Mot          vg_ordre_tun;


/* delaration de fonctions externes */
extern int drol_tun_init();
extern XDY_Booleen drol_tun_filtre(T_NOEUD *, XDY_District, XDY_Horodate, XDY_Octet);

#endif
