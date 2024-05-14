	/*E*/
/* Fichier : $Id: drol_imu.h,v 1.1 2021/07/20 12:21:09 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2021/07/20 12:21:09 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME SAIDEC
------------------------------------------------------
* MODULE DROL_IMU * FICHIER drol_imu.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   fonctions C utilisees dans les regles RTie de choix
*   des PMV et d'elaboration des messages PMV.
*
------------------------------------------------------
* HISTORIQUE :
*
* ABE	21/05/2021	Creation IMU DEM-SAE155 1.1
------------------------------------------------------ */

#ifndef DROL_IMU
#define DROL_IMU

#include "xdc.h"
#include "xdy.h"
#include "xzst.h"
#include "xzat.h"
#include "xzia.h"
#include "xzao505.h"
#include "xzap26.h"

#include "drol_tfm.h"

typedef struct S_PERT_IMU {
        char            Libelle[200];
        char            Definition[10];
	XDY_NomAuto	NomAutoroute;
	XDY_Texte	Message;
	XDY_Texte	NomBICE;												
	XDY_Sens	Sens;
	XDY_PR		PR_Debut;
	XDY_PR		PR_Fin;											
	XDY_Entier	Cycles;
	XDY_Entier	Intervalle;
	XDY_Entier	Duree;
	XDY_ChaineIMU	ListeZone;        
	T_CONDITION     *Condition;
	int		Mode;
        struct S_PERT_IMU  *Suivant;
} T_PERT_IMU;


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
} T_IMU;

extern XDY_Mot          vg_numero_plan;
extern XDY_District     vg_site;
extern int		vg_num_poste;
extern XDY_Mot          vg_ordre_imu;


/* delaration de fonctions externes */
extern int drol_imu_init();
extern XDY_Booleen drol_imu_filtre(T_NOEUD *, XDY_District, XDY_Horodate);

#endif
