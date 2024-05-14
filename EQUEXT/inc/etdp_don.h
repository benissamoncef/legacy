/*E*/
/*Fichier : $Id: etdp_don.h,v 1.20 2020/11/26 14:26:56 gesconf Exp $      Release : $Revision: 1.20 $        Date : $Date: 2020/11/26 14:26:56 $
------------------------------------------------------------------------------------------
* GTIE  *  PROJET MIGRAZUR
------------------------------------------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------------------------------------------
* MODULE TETDP * FICHIER etdp_don.h
------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees des fichiers de la tache TETDP
*
------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D      version 1.1     13 Fev 1997     : Creation (DEM/1396)
* Mismer.D      version 1.2     25 Mar 1997     : Affichage TDP sur PMV (DEM/1417)
* Mismer.D      version 1.3     05 Mai 1997     : Affichage TDP sur PMV suite (DEM/1417)
* Mismer.D      version 1.4     05 Jun 1997     : Ajout alerte sur panne zone (DEM/1447)
* P.N.          version 1.5     18 Jul 1997     : Modif pour affichage minutes pour TDP (DEM/1505)
* Mismer.D      version 1.6     13 Oct 1997     : Modif pour affichage minutes pour TDP (DEM/1505)
* Mismer.D      version 1.7     16 Oct 1997     : Modif pour affichage minutes pour TDP suite (DEM/1505)
* Niepceron     version 1.8     26 avr 2005     : Modif ETDP_TEXTE_TDP DEM464
* JPL		07/03/11 : Migration architecture HP ia64 (DEM 975) : declaration fonctions "lancement_timer_etdp"  1.9
* LCL		version 1.10	22/06/11	: TDP A8
* LCLC		version 1.11	23/06/11	: H minuscule
* JMG		version 1.12	13/12/11	: ETDPC_NB_MAX_ZONE + ETDPC_NB_MAX_UGTP plus grands
* JMG		version 1.13	31/01/12	: TdP etendu DEM/1014
* MG		version 1.14	08/06/12	: Ajout etdp_config_pmva (DEM/1014 PMA)
* PNI		version 1.15	30/01/13	: TdP etendu DEM/1014 : augmentation de ETDPC_NB_MAX_ZONE
* PNI		version 1.18    04/12/17	: Passage du code zone de 2 � 3 digits  ETDPC_LONG_DONNEE_6MN ETDPC_LONG_DONNEE_1H et type de TrameMB DEM1236
* PNI		version 1.19	06/06/18	: DEM1297 augmentation ETDPC_NB_MAX_ZONE
* PNI		version 1.20	26/11/20	:SAE168 : Mise en majuscule des TDP
* ABE	        version 1.21    26/07/21        : TDP FCD Hybride sur trajets hors réseau, ajout fonction calcul si dest est hors reseau DEM-SAE318
* ABE	                        17/10/22        : TDP FCD Hybride deplacement de la fonction calcul_zone_hors_reseau vers XZAO pour appel par taneu DEM-SAE318
---------------------------------------------------------------------------------------*/


/* Declaration des defines */
/* ----------------------- */

/* Constantes pour recuperer les minutes et les secondes de l'heure (char(20)) */

#define ETDPC_HEURE_STRING              11
#define ETDPC_DIXMINUTE_STRING          14
#define ETDPC_MINUTE_STRING             15
#define ETDPC_SECONDE_STRING            17

/* Constantes des timers des reveils cycliques ou non */

#define ETDPC_TIMER_6MN                 "UGTP_Timer_6mn"
#define ETDPC_TIMER_6MN_RAF             "UGTP_Timer_6raf"
#define ETDPC_TIMER_1H                  "UGTP_Timer_1h"
#define ETDPC_TIMER_SYNCHRO             "UGTP_Timer_Synchro"
#define ETDPC_TIMER_FEN_LCR             "UGTP_FLcr_NoEqt:"

#define ETDPC_DELAI_DDE_UGTP            "DelaiDemandeUGTP"
#define ETDPC_NUM_ALERTE_UGTP           "NumeroAlerteTDP"
#define ETDPC_LIB_ALERTE_UGTP           "LibelleAlerteTDP"
#define ETDPC_NUM_ALERTE_PANNE          "NumeroAlerteDefZone"
#define ETDPC_LIB_ALERTE_PANNE          "LibelleAlerteDefZone"

#define ETDPC_REVEIL_6MN                (360)
#define ETDPC_REVEIL_1H                 (3600)

#define ETDPC_STATUS_TMP_REEL_STRG      "@"
#define ETDPC_ETAT_ANT_OK               "ANT=0"
#define ETDPC_ETAT_TRANS_OK             "THS=0"
#define ETDPC_ETAT_ITP_OK               "ITP=0"
#define ETDPC_TDP_BLANC                 "    "

/*modif hilmarcher*/
/*#define ETDPC_TEXTE_TDP                 "   %d MINUTES  "*/
/*DEM 464*/
/*#define ETDPC_TEXTE_TDP                 "%3dmn"*/

#define ETDPC_TEXTE_TDP                 "%2dMN"
#define ETDPC_TEXTE_TDP_HOR             "%1dH%02d"

#define ETDPC_LIGNE_VIDE                "               "

#define ETDPC_LONG_DONNEE_6MN           7
#define ETDPC_LONG_DONNEE_1H            11
#define ETDPC_MES_INCONNU               2

#define ETDPC_FIC_STOCK_HORAIRE         "Fic_Stock_TDP_Horaire"
#define ETDPC_FIC_STOCK_6MN             "Fic_Stock_TDP_6mn"
#define ETDPC_FIC_HEUR_TDP              "Dern_heure_TDP"

#define ETDPC_NB_MAX_UGTP               10
#define ETDPC_NB_MAX_ZONE               500

#define ETDPC_LONG_TRAME		1024

/* definitions de types exportes */
/* ----------------------------- */

typedef enum
{
        Non_valide,
        Valide
}
TDP_Validation;

typedef struct
{
        TDP_Validation                  ChampValide;
        XDY_Mot                         NumITPDeb;
        XDY_Mot                         NumITPFin;
        XDY_Mot                         NumAntDeb;
        XDY_Mot                         NumAntFin;
        XDY_Mot                         NumCptDeb;
        XDY_Mot                         NumCptFin;
        XDY_Booleen                     EtatAlerte;
        XDY_Booleen                     EtatPanne;
        int                             Etat_Service;
        XDY_Booleen                     AlarmeAntenne;
        XDY_Booleen                     AlarmeITP;
        XDY_Booleen                     AlarmeTrans;
        XDY_Nom                         Nom;
}
ETDP_INFO_ZONE;

typedef struct
{
        ETDP_INFO_ZONE                  Zone[ETDPC_NB_MAX_ZONE+1];
        TDP_Validation                  ChampValide;
        XDY_Eqt                         Numero;
        XDY_NomST                       NomST;
        XDY_Mot                         NumLS;
        XDY_Rgs                         AdresseRGS;
        int                             SocketLS;
        int                             Etat_Service;
        FILE *                          Fd_Horaire;
        FILE *                          Fd_6mn;
        XDY_Mot                         Nb_Heure_Histo;
        XDY_Horodate                    Horodate_Histo;
        XDY_Horodate                    Horodate6mn;
        XDY_Horodate                    Horodate1H;
        char	                	TrameMB[ETDPC_LONG_TRAME];
        XDY_Mot                         StatusTR;
}
ETDP_DONNEES_UGTP;
                
typedef struct
{
        XDY_NomMachine                  NomMachine;
        XDY_NomSite                     NomSite;
        int                             Mode_Fonct;
        int                             DelaiDdeHoraire;
        int                             NumAlerte;
        XDY_TexteAlerte                 LibAlerte;
        int                             NumPanne;
        XDY_TexteAlerte                 LibPanne;
}
ETDP_DONNEES_CONFIG_TACHE;

typedef struct
{
        XDY_Eqt                         Numero[ETDPC_NB_MAX_ZONE+1];
        XDY_Mot                         TDP6mn[ETDPC_NB_MAX_ZONE+1];
        XDY_Mot                         Validite6mn[ETDPC_NB_MAX_ZONE+1];
        XDY_Mot                         TDPHoraire[ETDPC_NB_MAX_ZONE+1];
        XDY_Mot                         NbApp[ETDPC_NB_MAX_ZONE+1];
        XDY_Mot                         ValiditeHoraire[ETDPC_NB_MAX_ZONE+1];
        XDY_Mot                         TDP6mnRADT[ETDPC_NB_MAX_ZONE+1];
        XDY_Mot                         Validite6mnRADT[ETDPC_NB_MAX_ZONE+1];
}
ETDP_MESURE_ZONE_TDP;

typedef struct
{
        XZAOT_PMV_TDP                   PMV;
        XDY_Etat_PMV                    EtatPMV;
        XDY_Entier                      NumAction;
        XDY_Nom                         Nom;
	XDY_Mot				Destination;
	XDY_Mot				DestinationLointaine;
	XDY_Mot				DestinationNoeud;
	XZEZT_Liste_Zdp *		Graphe_Destination;
	XZEZT_Liste_Zdp *               Graphe_DestinationLointaine;
	XZEZT_Liste_Zdp *               Graphe_DestinationNoeud;
        void *                          suivant;
}
ETDP_CONFIG_PMV;


/* PMVA */
typedef struct
{
	XZAOT_PMV_TDP     	PMVA;
	XDY_Etat_PMVA      	EtatPMVA;
	XDY_Entier        	NumAction;
	XDY_Nom         	Nom;
	XDY_Octet			Destination1_proche;
	XDY_Octet			Destination1_loin;
	XDY_Octet			Destination2_proche;
	XDY_Octet			Destination2_loin;
	XZEZT_Liste_Zdp *	Graphe_Destination1;
	XZEZT_Liste_Zdp *	Graphe_Destination2;
	XDY_Octet           DestinationLointaine;
	XZEZT_Liste_Zdp *   Graphe_Destination1Lointaine;
	XZEZT_Liste_Zdp *   Graphe_Destination2Lointaine;
	void *              suivant;
}
ETDP_CONFIG_PMVA;

extern ETDP_DONNEES_CONFIG_TACHE        vg_ConfigTDP;
extern ETDP_DONNEES_UGTP                vg_DonneesUGTP[ETDPC_NB_MAX_UGTP];
extern ETDP_MESURE_ZONE_TDP             vg_Mesure[ETDPC_NB_MAX_UGTP];
extern XZEZT_Liste_Zdp *                pg_ListeZone;
extern XZEZT_Liste_Destinations *       pg_ListeDest;
extern ETDP_CONFIG_PMV *                pg_ListePMV;
extern ETDP_CONFIG_PMVA *               pg_ListePMVA;
extern EXT_NOM_EQT_LCR *                pg_DebutNomZDP;


/* Declaration des prototypes des fonctions externes */
/* ------------------------------------------------- */

extern int et_Stockage_Mesures_6mn      ( XDY_Eqt );
extern int et_Envoi_Mesure_6mn          ( XDY_Eqt, XDY_Booleen );
extern int et_Envoi_Mesure_6mn_par_zone ( XDY_Eqt );
extern int et_Stockage_Mesures_horaire  ( XDY_Eqt, XDY_Entier );
extern int et_Maj_Fichier_heure_TDP_1H  ( XDY_Eqt );
extern int et_Lecture_Fichier_heure_TDP_1H ( XDY_Eqt );
extern int et_Envoi_Mesure_TDP_Horaire  ( XDY_Eqt, XDY_Booleen );
extern int lancement_timer_etdp ( int, char * );
