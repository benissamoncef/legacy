/*E*/
/*Fichier : $Id: erdt_don.h,v 1.31 2021/01/20 07:41:05 gesconf Exp $      Release : $Revision: 1.31 $        Date : $Date: 2021/01/20 07:41:05 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TERDT * FICHIER erdt_don.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module decrivant les structures de donnees internes
*
------------------------------------------------------
* HISTORIQUE :
*
* Volcic.F	version 1.1	07 Dec 1994	: Creation
* Volcic.F	version 1.2	28 Jan 1995	:
* Volcic.F	version 1.3	13 Fev 1995	:
* Volcic.F	version 1.4	17 Mar 1995	:
* Volcic.F	version 1.5	20 Mar 1995	: Modif Nom Cstes TIMER
* Volcic.F	version 1.6	20 Mar 1995	:
* Volcic.F	version 1.7	06 Avr 1995	: Enlever #include "xzat.h" 
* Volcic.F	version 1.8	19 Avr 1995	:
* Volcic.F	version 1.9	04 Mai 1995	: Reprise historique
* Volcic.F	version 1.10	07 Jun 1995	: Ajout vg_Mode_lent et insertion liste chaussee
* Volcic.F	version 1.11	07 Jui 1995	: Modif Cstes
* Volcic.F	version 1.12	10 Sep 1995	: Modif Cstes
* Volcic.F	version 1.13	18 Sep 1995	: Modif Cstes
* Volcic.F	version 1.14	20 Oct 1995	: Ajout champ Horo_Extrapolation dans ERDT_CONFIG_RADT
* Volcic.F	version 1.15	14 Nov 1995	: Ajout ERDTC_STATUS_TMP_REEL_STRG
* Mismer.D      version 1.18    15 Jul 1996     : Correction pour pb trop de station (DEM/1150)
* Volcic.F	version 1.19	06 Sep 1996	: Modif Extension Ouest (RADT_W)
* Mismer.D	version 1.20	18 Sep 1996	: Modif Extension Ouest Suite (RADT_W)
* Mismer.D	version 1.21	01 Oct 1996	: Modif Extension Ouest niveau de service (RADT_W)
* Mismer.D	version 1.22	02 Oct 1996	: Modif pour mesures pesage (DEM/1227)
* Mismer.D	version 1.23	21 Oct 1996	: Modif station virtuelle EQT12 (DEM/1252)
* Mismer.D	version 1.24	12 Dec 1996	: Calcul des niveaux de services pourstations virtuelles (DEM/1338)
* Mismer.D	version 1.25	27 Jan 1997	: Modif calcul DIV su MI (DEM/1392)
* Hilmarcher    version 1.26    10 mai 2005     : Modif prise en compte alertes DEM465 
* JMG   31/10/07        : ajout site de gestion 1.27
* JPL	19/12/11	: declaration structure constante globale de mesures inconnues (DEM 997)  1.28
* VR	22/02/12		: Ajout ERDTC_NUM_REG_PARAMS (DEM/1016) 1.29
* JMG/PNI	31/10/18	: ajout IP / prise en compte du type de station sans trame inverse pour Toulon 1.30
* PNI	20/01/21	: Passage de ERDTC_NUM_MAX_STATION à 512 SAE-222 1.31
------------------------------------------------------ */

#ifndef erdt_don
#define erdt_don

/* fichiers inclus */


#include "xdg.h"
#include "ex_mdon.h"
#include "etim_don.h"
#include <xzea.h>
#include <xzer.h>
#include <xzse.h>
#include <xzamc.h>
#include <xzaac.h>
#include "ex_mlcr.h"

/* definitions de constantes exportees */

/* Constantes pour definition des tableaux */

#define ERDTC_NUM_MAX_STATION		512
#define ERDTC_NUM_REG_PARAMS		30
#define ERDTC_NB_STATION		32
#define ERDTC_NB_CHAUSSEE		9
#define ERDTC_NB_VOIE_CHAUS		5
#define ERDTC_NB_INDEX_1MN		6
#define ERDTC_NB_INDEX_6MN		10
#define ERDTC_NB_CHAUS_S_COUR		2
#define ERDTC_NB_CHAUS_SEUIL		2
#define ERDTC_NB_TYPE_NIVEAU		3
#define ERDTC_NB_CLAS_DIV		5
#define ERDTC_NB_REG_PARAMS		15
#define ERDTC_NB_MAX_CLAS		16
#define ERDTC_NB_DIV_MOY		2
#define ERDTC_NB_SEUIL			10
#define ERDTC_NB_CONTEXTE		6
#define ERDTC_NB_DEM_INDIV		25
#define ERDTC_NB_LS			10
#define ERDTC_DIM_TRAME_HIS		50000
#define ERDTC_DIM_TRAME			50000
#define ERDTC_NB_MOIS_AN		12
#define ERDTC_NB_JOUR_MOIS		31
#define ERDTC_BOUCL_SS_TRAME		99

/* Constantes pour recuperer les minutes et les secondes de l'heure (char(20)) */

#define ERDTC_HEURE_STRING		13
#define ERDTC_DIXMINUTE_STRING		14
#define ERDTC_MINUTE_STRING		15
#define ERDTC_SECONDE_STRING		17

/* Constantes des timers des reveils cycliques ou non */

#define ERDTC_TIMER_30S			"RADT_Timer_30s"
#define ERDTC_TIMER_1MN			"RADT_Timer_1mn"
#define ERDTC_TIMER_6MN			"RADT_Timer_6mn"
#define ERDTC_TIMER_30MN		"RADT_Timer_30mn"
#define ERDTC_TIMER_MINUIT		"RADT_Timer_0h"
#define ERDTC_TIMER_SYNCHRO		"RADT_Timer_Synchro"
#define ERDTC_TIMER_SEMAIME		"RADT_Timer_Semaine"
#define ERDTC_TIMER_WEEKEND		"RADT_Timer_WeekEnd"
#define ERDTC_TIMER_FERIE		"RADT_Timer_Ferie"

#define ERDTC_REVEIL_30S		30
#define ERDTC_REVEIL_1MN		60
#define ERDTC_REVEIL_6MN		360
#define ERDTC_REVEIL_30MN		1800
#define ERDTC_REVEIL_2MN		120


#define ERDTC_JOURNUIT_RADT		"Einit_JourNuit_RADT"

/* Caractere pour lecture fichier RADT Machine */

#define ERDTC_STATUS_TMP_REEL		'@'
#define ERDTC_STATUS_TMP_REEL_STRG	"@"
#define ERDTC_COMMENTAIRE		'#'
#define ERDTC_DOLLAR			'$'
#define ERDTC_BLANC			' '
#define ERDTC_EXTRACT_RADT_FIC		"R"
#define ERDTC_MES_VIDE			"0$0$0$"

/* Caractere pour lecture fichier calendrier Applix */

#define ERDTC_JOUR_SEMAIME		'S'
#define ERDTC_JOUR_WEEKEND		'W'
#define ERDTC_JOUR_FERIE		'F'

#define ERDTC_SEMAIME			1
#define ERDTC_WEEKEND			2
#define ERDTC_FERIE			3

/* Constantes Periodes */

#define ERDTC_30SECONDE			3
#define ERDTC_MINUTE			1
#define ERDTC_6MINUTE			6

/* Constante pour les valeurs inconnues Q V TO PL */

#define ERDTC_MES_INC			-9

/* Constantes pour les etats des voies */

#define ERDTC_NB_VOIE			8
#define ERDTC_INDEX_INCONNU		-1
#define ERDTC_INDEX_DEFAUT		0

/* Constantes pour demande type de donnees a extrapolee */

#define ERDTC_TOUS_SENS			14
#define ERDTC_TOUTES_VOIES		15
#define ERDTC_NON_PL			16
#define ERDTC_PL			17
#define ERDTC_QVTO_PL			18

/* Constantes pour Type de Station */

#define ERDTC_CHAUSSEE			2
#define ERDTC_PEAGE			4
#define ERDTC_ECHANGEUR			8
#define ERDTC_CHAU_PEAG			6
#define ERDTC_CHAU_ECHA			10
#define ERDTC_PEAG_ECHA			12
#define ERDTC_CHAU_PEAG_ECHA		14

/* Constantes pour Etat Trans */

#define ERDTC_EQT_LIBRE			1
#define ERDTC_EQT_OCCUP			2
#define ERDTC_EQT_PANNE			3

/* Constantes pour Etat Liaison avec le CI */

#define ERDTC_LIAISON_CI_OK		0
#define ERDTC_LIAISON_CI_HS		1

/* Constantes pour fichiers RADT */

#define ERDTC_EXT_FIC_TXT		".txt"
#define ERDTC_FIC_DIV_DFT		"dist_inter_vehi.txt"
#define ERDTC_FIC_DIV			"dist_inter_vehi"
#define ERDTC_FIC_SNT_DFT		"seuils_trafic_dai_defaut.txt"
#define ERDTC_FIC_SNT			"seuils_trafic_dai"
#define ERDTC_FIC_COEF_DFT		"coef_6min_gliss.txt"
#define ERDTC_FIC_COEF			"coef_6min_gliss"
#define ERDTC_FIC_EQA_DFT		"equation_alerte_defaut.txt"
#define ERDTC_FIC_EQA			"equation_alerte"
#define ERDTC_FIC_CAL_DFT		"calendrier_contexte.txt"
#define ERDTC_FIC_CAL			"calendrier_contexte"
#define ERDTC_FIC_HEUR_QVTO		"der_heure_MB_QVTO"
#define ERDTC_FIC_HEUR_PESAGE		"der_heure_PESAGE"
#define ERDTC_FIC_SAUV_QVTO		"Mesures_MB_QVTO"
#define ERDTC_FIC_STOCK_QVTO		"QVTO_A_Envoyer_A"
#define ERDTC_FIC_STOCK_PESAGE		"PESAGE_A_Envoyer_A"
#define ERDTC_PATH_FIC_DYN		"../../../fichiers/dyn"
#define ERDTC_PATH_FIC_SNT		"../../../fichiers/snt"
#define ERDTC_PATH_FIC_DFT		"../../../fichiers/dft"
#define ERDTC_PATH_FIC_EQA		"../../../fichiers/eqa"

/* Constantes pour envoi a TETDC */

#define ERDTC_MSG_TDC			1
#define ERDTC_DERN_MSG_TDC		0
#define ERDTC_MSG_CTXT			9
#define ERDTC_SEP_FLAG		  	"###"

/* definitions de types exportes */

typedef XDY_Mot			ERDT_ETAT_VOIE;

typedef XDY_EMes_RADT           ERDT_EMes_RADT;

typedef XDY_EMes_Poids          ERDT_EMes_Poids;

typedef struct	
{
	XDY_Eqt         NumeroStation;
        XDY_Mot		TypeStation;
        XDY_Mot		Virtuelle;
        XDY_NomEqt      NomServeur;
        XDY_Octet       Liaison;
        XDY_Rgs         RGS;
        XDY_NomEqt      Alertes;
	XDY_Booleen     DonneesIndividuelles;
        XDY_Booleen	CapteurPoids;
        XDY_Eqt         NumStationAmontS1;
        XDY_Eqt         NumStationAvalS1;
        XDY_Eqt         NumStationAmontS2;
        XDY_Eqt         NumStationAvalS2;
	XDY_District         SiteGestion;
	XDY_PortIP		Port;
	XDY_AdresseIP	AdresseIP;
	XDY_Octet	Type;
	XDY_NomEqt	IdLCR;
} ERDT_CONFIG_RADT;

typedef struct	
{
	XDY_Eqt         NumStation1;
	XDY_Eqt         NumStation2;
	XDY_Eqt         NumStation3;
	XDY_Eqt         NumStationVirtuelle;
        XDY_Eqt         Chaussee1;
        XDY_Eqt         Chaussee2;
        XDY_Eqt         Chaussee3;
        XDY_Sens        Sens;

} ERDT_CONF_RADT_VIRTUELLE;

typedef struct	
{
	XDY_Entier            Numero;
	XDY_Entier         Type;
    char     Nom[80];
    XDY_Valeur         Valeur;

} ERDT_CONF_REG_PARAMS;

typedef struct	
{
	XDY_Horodate	Horodate_Histo;
	XDY_Entier	Dern_Index;
	FILE		*Fd;
        XDY_Booleen	Demande_Histo;
        
} ERDT_DON_HIS_RADT;

typedef struct {

        XDY_Eqt         NumeroStation;
        XDY_Sens        Sens;
        XDY_Octet       Boucle;
        XDY_Octet       BoucleInv;
        XDY_Octet       Voie;
        XDY_Booleen     Poids;

} ERDT_CONFIG_PtMes;

		
typedef struct {

        XDY_Eqt         NumeroStation;
        XDY_Sens        Sens;

} ERDT_CONFIG_Chaussee;

typedef struct {
        XDY_Mot		Voie;
        int	        Classe[ERDTC_NB_MAX_CLAS];

} ERDT_DON_PESAGE;

typedef struct {

	XDY_Horodate	Horodate;
        XDY_Eqt         NumeroStation;
        XDY_Mot         Type;
        XDY_Mot         NbClasse;
        ERDT_DON_PESAGE	Mesure[ERDTC_NB_CHAUSSEE][ERDTC_NB_VOIE_CHAUS];

} ERDT_MESURE_PESAGE;


typedef struct	
{
	ERDT_CONFIG_RADT	ConfRADT;
	ERDT_DON_HIS_RADT	Don_HisRADT;
	ERDT_DON_HIS_RADT	Don_HisSilhouette;
	ERDT_DON_HIS_RADT	Don_HisEssieu;
	ERDT_DON_HIS_RADT	Don_HisPoids;
	XDY_Entier		Mode;
	XDY_Entier		Etat_Service;
	XDY_Mot			EtatTrans;
	XDY_Entier		Socket;
	void *			Suivant;

} ERDT_DONNEES_RADT;


typedef struct	
{
	ERDT_CONFIG_PtMes	ConfPtMes;
	ERDT_ETAT_VOIE		EtatVoie;
	XDY_Horodate		Sec_dernier_veh;
	void *			Suivant;

} ERDT_DONNEES_PtMes;


typedef struct	
{

	ERDT_CONFIG_Chaussee	ConfChaussee;
	void *			Suivant;

} ERDT_DONNEES_Chaussee;

typedef struct	
{

	XDY_Entier	Nb_Extrapolation;
	XDY_Mot		Chaussee;
	XDY_Mot		Voie;

} ERDT_EXTRAP;

/* declarations de donnees globales */

extern ERDT_DONNEES_RADT	*pg_debutRADT;
extern ERDT_DONNEES_PtMes	*pg_debutPtMes;
extern ERDT_DONNEES_Chaussee	*pg_debutChaussee;

extern int		 	pg_Tab_RADT[ERDTC_NUM_MAX_STATION];
extern int		 	pg_Nb_Voie[ERDTC_NUM_MAX_STATION][ERDTC_NB_CHAUSSEE];
extern int		 	pg_Nb_Donnees_Jour[ERDTC_NUM_MAX_STATION][ERDTC_NB_CHAUSSEE];
extern int		 	pg_Nb_Q_Nuls[ERDTC_NUM_MAX_STATION][ERDTC_NB_CHAUSSEE];
extern int		 	pg_Nb_PtMes[ERDTC_NUM_MAX_STATION];
extern int		 	pg_Nb_Chaussees[ERDTC_NUM_MAX_STATION];
extern int		 	pg_Seuils[ERDTC_NB_STATION][ERDTC_NB_CHAUS_SEUIL][ERDTC_NB_CONTEXTE][ERDTC_NB_SEUIL];
extern int		 	pg_Niveau[ERDTC_NB_STATION][ERDTC_NB_CHAUS_SEUIL][ERDTC_NB_TYPE_NIVEAU];
extern int		 	pg_DIV[ERDTC_NB_CLAS_DIV];
extern int		 	pg_DIV_Moy[ERDTC_NB_DIV_MOY];

extern ERDT_EMes_RADT		pg_ERDT_Mes_1mn[ERDTC_NB_STATION][ERDTC_NB_CHAUSSEE][ERDTC_NB_VOIE_CHAUS][ERDTC_NB_INDEX_1MN];
extern ERDT_EMes_RADT		pg_ERDT_Mes_6mn[ERDTC_NB_STATION][ERDTC_NB_CHAUSSEE][ERDTC_NB_VOIE_CHAUS][ERDTC_NB_INDEX_6MN];
extern ERDT_EMes_RADT		pg_ERDT_Mes_6mn_gl[ERDTC_NB_STATION][ERDTC_NB_CHAUSSEE][ERDTC_NB_VOIE_CHAUS][ERDTC_NB_INDEX_1MN];
extern ERDT_EMes_Poids         	pg_ERDT_Mes_6mn_PL[ERDTC_NB_STATION][ERDTC_NB_CHAUSSEE];
extern XDY_Mot                  vg_PL_Prec[ERDTC_NB_STATION][ERDTC_NB_CHAUSSEE][ERDTC_NB_VOIE_CHAUS];

extern XDY_Mes			vg_Mesures_Aucune;
extern XDY_NomMachine		vg_NomSuppleant;
extern XDY_Horodate		vg_Date_Actuelle;
extern XDY_Horodate		vg_Date_Changement;
extern int			vg_ContexteSeuils;
extern int                      vg_Contexte;
extern int			vg_mode_Fonct;
extern XDY_Booleen              vg_Mode_Lent;
extern XDY_NomSite		vg_NomSite;
extern XDY_Booleen		vg_Mac_Ext_Ouest;
extern char               	pg_Trame[ERDTC_NB_LS][ERDTC_DIM_TRAME_HIS];
extern char               	pg_Trame_PL[ERDTC_NB_LS][ERDTC_DIM_TRAME_HIS];
extern XDY_Booleen          	vg_Rep_His_PL[ERDTC_NB_LS];
extern XDY_Horodate          	pg_Heure_QVTO;   		
extern XDY_Booleen          	vg_TimersMin;   		
extern XDY_Booleen          	vg_Timers6Min;   		
extern XDY_Booleen          	vg_Timers30Min;   		
extern XDY_Booleen          	vg_Timers_Enclenches;   		
extern XDY_Booleen          	vg_Demande_Individuelle[];
extern XDY_Horodate          	vg_Horo_Dem_Ind[ERDTC_NB_DEM_INDIV];   		
extern FILE         	        *vg_Fd_Dem_Ind[ERDTC_NB_DEM_INDIV];	  		
extern ERDT_CONF_RADT_VIRTUELLE	pg_Tab_RADT_Virtuelle[ERDTC_NUM_MAX_STATION];
extern ERDT_CONF_REG_PARAMS		pg_Tab_REG_PARAMS[ERDTC_NUM_REG_PARAMS];
extern XDY_NomMachine		vg_NomMachine;
extern char			vg_idpassw[18];
extern XZEXT_MASK *		vg_SockMask;
extern int                	vg_Mode_Fonct;
extern T_INT4			vg_SockRT;
extern int         	        vg_SockTIM;	  		
extern int         	        vg_SockTDC;	  		
extern XDY_Mot			vg_Etat_Liaison_CI;
extern INFO_EQT_FEN_LCR         *pg_DebutListeRADTFLcr; 


/* delaration de fonctions externes */

EX_DECLA_INSERER   ( ERDT_DONNEES_RADT, er_ins_liste_RADT )	
EX_DECLA_SUPPRIMER ( er_sup_liste_RADT )	
EX_DECLA_SUPPRIMER ( er_sup_liste_PtMes )	
EX_DECLA_SUPPRIMER ( er_sup_liste_Chaussee )	


extern int    er_ins_liste_PtMes     (	 ERDT_DONNEES_PtMes *, int  );

extern int    er_ins_liste_Chaussee  (	 ERDT_DONNEES_Chaussee *, int  );

void    er_EMes_6mn_Voisin_cb ( T_IPC_CONN ,  T_IPC_CONN_PROCESS_CB_DATA , T_CB_ARG );


#endif
