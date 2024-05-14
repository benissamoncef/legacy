/*E Fichier : $Id: drol_tfm.h,v 1.23 2020/02/21 09:33:49 devgfi Exp $        $Revision: 1.23 $        $Date: 2020/02/21 09:33:49 $
-------------------------------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
-------------------------------------------------------------------------------
* SOUS-SYSTEME SAIDEC
-------------------------------------------------------------------------------
* MODULE DROL_TFM * FICHIER drol_tfm.h
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Données de l'évènement (tous types confondus) nécessaires
* à l'élaboration du message RVA et à l'activation des équipements.
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* JMG 31/01/01	creation
* JMG	25/10/05 : ajout PMVA 1.2
* JPL	27/02/09 : Ajout champ 'feu maitrise' (DEM 845) 1.3
* PNI	30/03/09 : Ajout d'un flag cause_operation pour affichage cause sur PMVA DEM876 1.4
* PNI 09/10/09	: Config_Voies passe à char[10] pour ajouter le picto en sens inverse DEM915 1.5
* JPL	13/01/11 : Migration architecture HP ia64 (DEM 971) : declaration drol_msg_limites_amont et _aval  1.6
* JPL	17/02/11 : Migration architecture HP ia64 (DEM 971) : declaration drol_tfm_lit_infos  1.7
* JPL	12/10/11 : Ajout champ 'origine' aux donnees TFM (DEM 995)  1.8
* JMG	15/04/12 : regulation dem 1016 1.9
* JPL	09/07/12 : Ajout nom de sortie usager et indicateur de bifurcation a la localisation (DEM 1033)  1.10
* PNI	25/09/14 : Ajout Lib_Cause et Lib_Secours DEM1097 v1.11
* JMG   18/06/15 : ajout h debut et h debut prevu datetime DEM1130 1.12
* LCL	15/11/16 : Ajout localisation en tunnel/viaduc (LOT23) 1.13
* JPL	03/01/17 : Ajout constantes generales (modes)  1.14
* JPL	16/02/17 : Ajout du champ de numero de rappel client aux donnees fax (DEM 1192)  1.15
* ???	30/03/17 : Type_Cause 1.16
* JMG	20/06/17 : zone urbaine convergence 1.17
* JMG	08/03/18 : ajout Personnel travaux 1.18 DEM1282
* JPL	29/06/18 : Ajout nombre de passagers (DEM 1295)  1.19
* JMG	02/09/18 : ajout cause et cle_cause LOT25 DEM1301 1.20
* JMG	30/11/8 : ajout presence depanneur 1.21
* JPL	05/04/19 : Ajout de l'indicateur de présence d'écoulement (DEM 1326)  1.22
* JPL	25/09/19 : Ajout de l'indicateur de présence de bouchon (DEM 1346)  1.23
------------------------------------------------------ */

#ifndef DROL_TFM
#define DROL_TFM


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
#include "xzae200.h"
#include "xzae201.h"
#include "xzit.h"
#include "xzap25.h"
#include        "xdea_eqn_alt.h"

/* definition de macro exportees */
#define C_TYPE_PREDICAT         0
#define C_TYPE_COMPARATEUR      1

#define	DROL_MODE_MANUEL	0
#define	DROL_MODE_AUTOMATIQUE	1


/* declarations de donnees globales */

typedef struct {
	XDY_Booleen Prevu;
	XDY_Booleen Signale;
	XDY_Booleen Confirme;
	XDY_Booleen Termine;
	XDY_Nom	Origine;
	char	Autoroute[200];
	char    Duree[200];
	char	Secours[255];
	char	Debut[200];
	char	Date_Fin[40];
	char	Localisation[200];
	char	Neutralisation[200];
	char	Victimes[200];
	char	Gravite[200];
	char	Vehicules[200];
	char	Chaussee_Glissante[200];
	char	Conseil[200];
	char	Voies_Bloquees[200];
	char	Groupe_Nominal[200];
	char	Groupe_Verbal[200];
	char	Fin[200];
	char	Longueur_Bouchon[200];
	char	Periode[200];
	char	Retour_Normal[200];
	char	Visibilite[200];
	char	Cause[200];
	char	TDP[200];
	int	Type_Travaux;
	char	Config_Voies[9 + 1];
        XDY_Octet Datex;        /*SAGA*/
	char	Horaire_Debut_Prevu[5 + 1];
	char	Horaire_Fin_Prevue[5 + 1];
	char	Nuit[10];
	XDY_Octet	Nature_Travaux;
	XDY_Octet	Aire_Service;
	char 	Lib_Cause[100];
	char 	Lib_Secours[100];
	XDY_Horodate	HDebut;
	XDY_Horodate	HDebut_Prevu;
	XDY_Octet	Majeur;
	XDY_Mot		Type_Cause;
	XDY_Octet	Zone_Urbaine;
	XDY_Octet	Intervention;
	XDY_Octet	Presence_Depanneur;
} T_TFM;

typedef struct {
	char Valeur[255];
	char * (*Fct)();
} T_Modele;

typedef struct {
	int 	Autoroute;
	int	PR;
	int	Sens;
	XDY_NomAuto	NomAutoroute;
	int	TypePointCar;
	int	NumPointCar;
	char	NomPointCar[30 + 1];
	char	AbrevPointCar[20 + 1];
	XDY_Sortie	SortieUsager;
	XDY_Booleen	Bifurcation;
	int	NumSortieAmont;
	XDY_Booleen	SortieAmontEstBifurc;
	int	DistanceSortieAmont;
	int 	NumSortieAval;
	XDY_Booleen	SortieAvalEstBifurc;
	int	DistanceSortieAval;
	int	NumPeageAmont;
	int	DistancePeageAmont;
	int	NumPeageAval;
	int	DistancePeageAval;
	int	LongueurPlateforme;
	int	AutorouteBifAmont;
	XDY_NomAuto	NumAutorouteBifAmont;
	int     DistanceBifAmont;
	int	AutorouteBifAval;
	XDY_NomAuto	NumAutorouteBifAval;
	int     DistanceBifAval;
	XDY_Nom	Descente;
	XDY_Nom	Tunnel;
	XDY_Nom	NomSortieAmont;
	XDY_Nom	AbrevSortieAmont;
	XDY_Sortie	SortieAmontUsager;
	XDY_Nom	NomSortieAval;
	XDY_Nom	AbrevSortieAval;
	XDY_Sortie	SortieAvalUsager;
	XDY_Nom	NomPeageAmont;
	XDY_Nom	AbrevPeageAmont;
	XDY_Nom	NomPeageAval;
	XDY_Nom	AbrevPeageAval;
	int	Pente;
	int	Site;
	int	PRBifAmont;
	int	PRBifAval;
	int	PRDebutBifAmont;
	int	PRDebutBifAval;
	int	Longueur;
	XDY_Octet	BAU;
	XDY_Octet	VL;
	XDY_Octet	VM1;
	XDY_Octet	VM2;
	XDY_Octet	VR;
	XDY_Octet	VL_I;
	XDY_Octet       VM1_I;
	XDY_Octet       VM2_I;
	XDY_Octet       VR_I;
	int		NbVoies;
	int		NbVoiesCirculees;
	XDY_Booleen	Voies_Retrecies;
	XDY_Nom		Viaduc;
	int		ZoneUrbaine;
} T_Localisation;

typedef struct {
	char	Scenario[100];
	char	Zone[100];
	XDY_Eqt	Numero_Zone;
	char	Transitoire[100];
	char	Objet[100];
	char	Sens[50];
	char	Complement_Rappel[40];
	int	Nb_Vehicules;
	int	Type_Meteo;
	XDY_Booleen 	Personnalites;
	XDY_Booleen	Agent_Escota;
	int	Duree;
	int	Visibilite;
	int	Type_Vehicule;
	int	Nb_Passagers;
	int	Nb_VL;
	int	Nb_PL;
	int	Nb_Motos;
	int	Nb_Remorques;
	int	Nb_Victimes;
	int	Nb_Morts;
	int	Nb_BG;
	int	Nb_Cars;
	int	PR_Min;
	int	PR_Max;
	int	Type_Incident;	/*0=fermeture 1=penurie*/
	XDY_Booleen	Matieres_Dangereuses;
	XDY_Booleen	Animaux_Vivants;
	XDY_Booleen	Entree1;
	XDY_Booleen	Entree2;
	XDY_Booleen	Sortie1;
	XDY_Booleen	Sortie2;
	int	Numero_Carburant;
	int	Type_Bouchon;
	int	Nb_Manifestants;
	XDY_Booleen 	Violence;
	XDY_Booleen	Perception_Peage;
	XDY_Octet	Feu_Maitrise;
	XDY_Octet	Cause_Operation;
	int		Vitesse;
	int		Vitesse_Opposee;
	XDY_Octet	Personnel;
	XDY_Octet	Ecoulement;
	XDY_Octet	Presence_Bouchon;
	XDY_Entier	Numero_Cause;
	XDY_Octet	Cle_Cause;
} T_FAX;

typedef struct {
        int             *Slot;
        XDY_Booleen     (*Fct)();
        int     Valeur;
} T_COMP;

typedef struct {
        XDY_Booleen     (*Fct)();
} T_PREDICAT;

typedef struct S_CONDITION {
        char    Libelle[120];
        int     Type;
        union {
                T_COMP          Comp;
                T_PREDICAT      Predicat;
        } Fonction ;
        struct S_CONDITION *Suivant;
} T_CONDITION;

typedef struct S_CAS {
        char            Libelle[100];
        char            Libelle_Condition[160];
        T_CONDITION     *Condition;
        int             Interlocuteurs[80];
        int             Nombre_Interlocuteurs;
	XDY_Booleen	Majeur;
        struct S_CAS    *Suivant;
} T_CAS;

typedef struct S_NOEUD {
	XDY_Evt         NumEvt;
	int		Type;
	T_TFM           Infos_TFM;
	T_FAX           Infos_FAX;
	T_Modele	*Modele_TFM;
	T_Localisation	Localisation;
	T_Localisation	Localisation_Tete;
	int		Nombre_Lexemes;
	XDY_Octet	Trafic;
	T_CAS		*Cas;
	struct S_NOEUD  *Suivant;
	struct S_NOEUD  *Precedent;
} T_NOEUD;

extern T_NOEUD *vg_racine;
extern XDY_Booleen vg_mode_historique;

/* delaration de fonctions externes */
extern int drol_tfm_init();
extern void drol_trim();

extern int drol_tfm_lit_infos(XDY_Horodate, T_NOEUD *, XDY_District);
extern char *drol_tfm_genere(T_NOEUD *, XDY_Booleen);
extern char *drol_tfm_signature(XDY_Horodate);
extern char *drol_tfm_consequence(T_NOEUD *);
extern char *drol_tfm_elabore(int, int, int *,XDY_Horodate,int);

extern char *drol_msg_limites_amont(int, int);
extern char *drol_msg_limites_aval(int, int);

#endif
