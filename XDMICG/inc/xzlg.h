/*E*/
/*  Fichier : 	$Id: xzlg.h,v 1.8 2021/06/30 07:30:55 pc2dpdy Exp $    Release : $Revision: 1.8 $        Date :  $Date: 2021/06/30 07:30:55 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME XDMICG
------------------------------------------------------
* MODULE XZLG * FICHIER XZLG.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
* JMG	01/11/08	creation passerelle generique 1.1
* JMG	15/02/11	modif structure code_loc_file datex v2 1.2
* VR 	07/10/11	Ajout vitesse et vitesse_opposee	1.3 (DEM/1015)
* VR	09/03/12 : Ajout XZLG565_GetFmcRegulation et XZLGT_REG(DEM/1016)
* JMG	15/12/15 : ajout RDS dans ZDP TDP_ASF 1.5 DEM 1151
* JMG	05/04/16 : ajout nom eqt dans ZDP TDP ASF 1.6
* ABE	15/04/21 : ajout debut_prevu et fin_prevu DEM-SAE130 1.7
* ABE	28/05/21 : Ajout Evt_datex2Fils et Evt_datex2Fils_version DEM-SAE296 DEM-SAE297 1.8
------------------------------------------------------ */

#ifndef XZLG
#define XZLG

/* fichiers inclus */
#include <errno.h>
#include <sys/stat.h>
/* Include Sybase */
#include <ctpublic.h>
#include <cstypes.h>
#include "xzag.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h> 
#include <sys/sem.h> 
#include "asql_com.h"

#include <xdf.h>
#include <xdy.h>
#include <xdc.h>
#include <xdg.h>
#include <xdm.h>

#include <xzsc.h>
#include <xzst.h>
#include <xzsm.h>

#include "xzaec.h"
#include "xzae41.h"
#include "xzae126.h"
#include "xzae45.h"
#include "xzae46.h"
#include "xzae47.h"
#include "xzae48.h"
#include "xzae49.h"
#include "xzae50.h"
#include "xzae51.h"
#include "xzae52.h"

/* definitions de constantes exportees */
#define XZLGC_CLE_FMC	123450
#define XZLGC_CLE_TDP	0555555
#define XZLGC_CLE_RADT	0112222
#define XZLGC_CLE_NOMBRE	0246

#define XZLGC_SEM_FMC	'F'
#define XZLGC_SEM_TDP	'T'
#define XZLGC_SEM_RDT	'Z'

#define XZLGC_NB_MAX_FMC	1200
#define XZLGC_NB_CLIENTS	2
#define XZLGC_DIFF_CLIENT_1	1	/*taweb*/
#define XZLGC_DIFF_CLIENT_2	2	/*tagen*/
#define XZLGC_DIFF_CLIENT_ALL	3

typedef struct {
	int	Nombre_FMC;
	int	Nombre_ZDP;
	int	Nombre_RDT;
	int	Checksum;
} XZLGT_Nombre_Mem;

/* definitions de types exportes */
typedef struct {
	XDY_TDP		Tdp;
	XDY_Horodate	Horodate;
	XDY_Octet	Validite;
} XZLGT_TDP;

struct code_loc {
        char            l01[10];                /* attribut 1  de la localisation RDS Strada    */
        char            l02[10];                /* attribut 2  de la localisation RDS Strada    */
        int             l03;                    /* attribut 3  de la localisation RDS Strada    */
        int             l04;                    /* attribut 4  de la localisation RDS Strada    */
        char            l05[10];                /* attribut 5  de la localisation RDS Strada    */
        char            l06[50];                /* attribut 6  de la localisation RDS Strada    */
        char            l07[100];               /* attribut 7  de la localisation RDS Strada    */
        int             l08;                    /* attribut 8  de la localisation RDS Strada    */
        int             l09;                    /* attribut 9  de la localisation RDS Strada    */
        char            l10[50];                /* attribut 10 de la localisation RDS Strada    */
        int             l11;                    /* attribut 11 de la localisation RDS Strada    */
        int             l12;                    /* attribut 12 de la localisation RDS Strada    */
        char            rpip[20];
        char            rnp[20];
        char            drp[20];
        char            dfpp[20];
        char            rpis[20];
        char            rns[20];
        char            drs[20];
        char            dfps[20];
        int             l01_b;
        int             l02_b;
        int             l03_b;
        int             l04_b;
        int             l05_b;
        int             l06_b;
        int             l07_b;
        int             l08_b;                  /* attribut boolean 8  de la localisation RDS Strada chp obligat. */
        int             l09_b;                  /* attribut boolean 9  de la localisation RDS Strada chp obligat. */
        int             l10_b;                  /* attribut boolean 10 de la localisation RDS Strada chp obligat. */
        int             l11_b;                  /* attribut boolean 11 de la localisation RDS Strada chp obligat. */
        int             l12_b;                  /* attribut boolean 12 de la localisation RDS Strada chp obligat. */
        };
 typedef struct code_loc loc_rds;

struct code_loc_file {
        char            l01[50];
        char            l02[50];
        char            l03[50];
        char            l04[50];
        char            l05[50];
        char            l06[50];
        char            l07[50];
        char            l08[50];
        char            l09[50];
        char            l10[50];
        char            l11[50];
        char            l12[50];
        char            rpip[20];
        char            rnp[20];
        char            drp[20];
        char            dfpp[20];
        char            rpis[20];
        char            rns[20];
        char            drs[20];
        char            dfps[20];
};
typedef struct code_loc_file XZLGT_RDS;

typedef struct S_XZLGT_Liste_Zdp {
	XDY_ZDP		Zdp;
	XZLGT_RDS       RDS;
	XZLGT_TDP	Tdp;
	XDY_Octet	Valide;
	XDY_Nom		Nom;
} XZLGT_Liste_Zdp;



typedef struct S_XZLGT_Liste_RADT {
	XDY_Mot			Numero;
	XDY_Sens		Sens;
	XDY_Octet		Niveau;
} XZLGT_Liste_RADT;



/*structure du bloc accident*/
typedef struct {
        XDY_Octet               Vehicules;
        XDY_Octet               PL;
        XDY_Octet               Cars;
        XDY_Octet               Rem;
        XDY_Octet               Blesses;
        XDY_Octet               Morts;
        XDY_Booleen             Matiere;
        XDY_Octet               Blesses_Legers;
        XDY_Octet               Blesses_Graves;
        XDY_Octet               Motos;
        XDY_Octet               VL;
        XDY_Booleen             VIP;
        XDY_Booleen             ESCOTA;
} XZLGT_ACCI;


/*structure du bloc meteo*/
typedef struct {
        int             Longueur;
        XDY_Mot         Visibilite;
        XDY_Octet       Type;
	char		Nature[30];
} XZLGT_METE;

/*structure du bloc optionnel Vehicule*/
typedef struct {
        char            Type[100];
        XDY_Mot         Volume;
        char            Matiere[50];
        XDY_Octet       TypeVehicule;
} XZLGT_VEHI;


/*structure du bloc optionnel Vehicule lent*/
typedef struct {
        XDY_Mot         Longueur;
        XDY_Octet       Vitesse;
	XDY_Mot         Hauteur;
	XDY_Mot         Largeur;
	XDY_Mot         Poids;
	char            Contenu[50];
} XZLGT_VEHL;


/*structure du bloc optionnel Manifestation*/
typedef struct {
        char            rien;
        XDY_Entier      Nombre;
} XZLGT_MANI;


/*structure du bloc optionnel sous-concessionnaire*/
typedef struct {
        char            rien;
        XDY_Nom         Concessionnaire;
        XDY_Octet       Carburant;
        XDY_Booleen     Penurie;
        XDY_Booleen     Fermeture;
} XZLGT_SSCC;


/*structure du bloc optionnel Bouchon*/
typedef struct {
        int             Longueur;
        XDY_Octet       Type;
} XZLGT_BOUC;


/*structure du bloc optionnel fermeture echangeur*/
typedef struct {
        char            rien;
        XDY_Booleen     Entree1;
        XDY_Booleen     Entree2;
        XDY_Booleen     Sortie1;
        XDY_Booleen     Sortie2;
	XDY_Octet	Nb_Entrees;
	XDY_Octet	Nb_Sorties;
} XZLGT_ECHA;


/*structure du bloc optionnel delestage*/
typedef struct {
        char            rien;
        XDY_Nom         Itineraire;
} XZLGT_DELE;



/*structure du bloc optionnel basculement*/
typedef struct {
        int             Longueur;
	int		Vitesse;
	int 		VitesseOpposee;
} XZLGT_BASC;


/*structure du bloc optionnel Travaux*/
typedef struct {
        int             Longueur;
        int             Mobiles;
        XDY_Octet       Type;
	int		Vitesse;
} XZLGT_TRAV;
/*structure du bloc optionnel regulation*/
typedef struct {
	int     Scenario;
	int	Vitesse;
	char Commandes[500];
} XZLGT_B_REGUL;


/*structure des blocs optionnels*/
typedef struct {
        char            Date_Debut[22];
        char            Date_Debut_Prevu[22];
        char            Date_Fin[22];
        char            Date_Fin_Prevu[22];
        char            Fausse_Alerte;
        char            Fin;
        char            Cause[50];
        XDY_Octet       Veracite;
        char            Comment[351];
	XDY_Evt		Evt_datex2Fils;
	int		Evt_datex2Fils_version;	
        union {
          XZLGT_ACCI      Accident;
          XZLGT_METE      Meteo;
          XZLGT_VEHI      Vehicule;
          XZLGT_VEHL      VehicLent;
          XZLGT_MANI      Manif;
          XZLGT_SSCC      Concession;
          XZLGT_BOUC      Bouchon;
          XZLGT_ECHA      Echangeur;
          XZLGT_DELE      Delestage;
          XZLGT_BASC      Basculement;
          XZLGT_TRAV      Travaux;
	  XZLGT_B_REGUL	  Regulation;
        }               Bloc;
} XZLGT_Bloc_Optio;

typedef struct S_XZLGT_Liste_FMC {
	XDY_Evt			Evt;
	XDY_Octet		Type;
	XDY_Evt			Evt_Cause;
	XDY_Octet		Type_Cause;
	XDY_Horodate		HValidation;
	XDY_Booleen		Prevision;
	XDY_Octet		Gravite;
	XDY_Voie		VR;
	XDY_Voie		VM2;
	XDY_Voie		VM1;
	XDY_Voie		VL;
	XDY_Voie		BAU;
	XDY_Voie		VR_I;
	XDY_Voie		VM2_I;
	XDY_Voie		VM1_I;
	XDY_Voie		VL_I;
	XDY_Voie		BAU_I;
	XDY_Autoroute		Autoroute;
	XDY_Sens		Sens ;
	XDY_PR			PR_Debut;
	XDY_PR			PR_Fin;
	XDY_Octet		Type_Point_Car;
	XDY_Octet		Numero_Point_Car;
	XDY_Evt			Evt_Consequence;
	XDY_Octet		Type_Consequence;
	XDY_Booleen		A_Traiter;
	int			Longueur;
	XDY_Booleen		Booleen1;
	XDY_Booleen		Booleen2;
	XDY_Booleen		Booleen3;
	XDY_Booleen		Booleen4;
	char			Nom1[200];
	XDY_Octet		Octet1;
	XDY_Octet		Octet2;
	char		Nb_Voies_Ouvertes[5];
	int			VNM;
	XZLGT_RDS			RDS;
	XZLGT_Bloc_Optio		Bloc_Optio;
	XDY_Octet			Diffusion;
	int				ZZ;
} XZLGT_Liste_FMC;




/*------------------------------------------------------
* SERVICE RENDU :
*  liste des  FMC
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZLG01_GetFMC(  XDY_Booleen         va_datex_in,
                        XDY_Booleen  va_confirme_in,
                XDY_Booleen va_fini_in,
                XDY_Booleen va_essai_in,
                XDY_Booleen va_exercice_in,
		XDY_Booleen va_flag_web_in);

/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK              succes
*   XDC_NOK             echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
------------------------------------------------------*/







/*X------------------------------------------------------
* SERVICE RENDU :
*
*  Liste des commentaires valides de la FMC
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZLG02_GetComment (   XDY_Entier      va_NumEvt_in,
                                        XDY_Octet       va_CleEvt_in,
                                        XDY_FonctionInt va_FonctionUtilisateur_in);


/*
*
* PARAMETRES EN ENTREE :
*
* int           va_NumEvt_in
* tinyint       va_CleEvt_in
* XDY_Fonction  va_FonctionUtilisateur_in
*
* PARAMETRES EN SORTIE :
*
* XDY_Entier    va_Resultat_out
*
* VALEUR RENDUE :
*
*  Liste des autoroutes
*
* CODE RETOUR :
*
* 0     XDC_OK
* 1     XDC_NOK
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
* Pour chaque autoroute appel de la fonction utilisateur avec
*
* MECANISMES :
*
------------------------------------------------------*/


int XZLG10_Memorise_Diffusion_FMC(XDY_Entier va_numero_in,
		XDY_Octet va_cle_in,
		XDY_Octet va_valeur);

int XZLG15_Memorise_Diffusion_Base(XDY_Entier va_numero_in,
			XDY_Octet va_cle_in);


/*------------------------------------------------------
* SERVICE RENDU :
*  lecture des caractéristiques d.une FMC Accident
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZLG41_GetFmcAccident ( XDY_Entier va_numero_in,
	XDY_Octet va_cle_in,
	XDY_Octet	*va_vl_out,
	XDY_Octet	*va_pl_out,
	XDY_Octet	*va_remorque_out,
	XDY_Octet	*va_car_out,
	XDY_Octet	*va_moto_out,
	XDY_Octet	*va_bl_out,
	XDY_Octet	*va_bg_out,
	XDY_Octet	*va_morts_out,
	XDY_Booleen	*va_matiere_out,
	XDY_Booleen	*va_vehEscota_out,
	XDY_Booleen	*va_persEscota_out,
	XDY_Booleen	*va_perso_out,
	XDY_Booleen	*va_degats_out);

/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK              succes
*   XDC_NOK             echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
------------------------------------------------------*/





/*------------------------------------------------------
* SERVICE RENDU :
*  lecture des caractéristiques d.une FMC Basculement
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZLG48_GetFmcBasculement( XDY_Entier va_numero_in,
	XDY_Octet va_cle_in,
	XDY_PR	*va_PR_out,
	XDY_Entier	*va_longueur_out,
	XDY_Octet	*va_VR_out,
	XDY_Octet	*va_VM2_out,
	XDY_Octet	*va_VM1_out,
	XDY_Octet	*va_VL_out,
	XDY_Octet	*va_BAU_out,
	XDY_Octet	*va_VR_I_out,
	XDY_Octet	*va_VM2_I_out,
	XDY_Octet	*va_VM1_I_out,
	XDY_Octet	*va_VL_I_out,
	XDY_Octet	*va_BAU_I_out,
	XDY_Entier	*va_vitesse_out,
	XDY_Entier	*va_vitesse_opposee_out);

/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK              succes
*   XDC_NOK             echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
------------------------------------------------------*/



/*------------------------------------------------------
* SERVICE RENDU :
*  lecture des caractéristiques d.une FMC convoi
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int XZLG46_GetFmcConvoi ( XDY_Entier va_numero_in,
	XDY_Octet va_cle_in,
	XDY_Mot	*va_longueur_out,
	XDY_Octet	*va_vitesse_out,
	XDY_Mot	*va_hauteur_out,
	XDY_Mot	*va_largeur_out,
	XDY_Mot	*va_poids_out,
	char	*va_contenu_out);


/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK              succes
*   XDC_NOK             echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
------------------------------------------------------*/





/*------------------------------------------------------
* SERVICE RENDU :
*  lecture des caractéristiques d.une FMC Bouchon
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int XZLG51_GetFmcBouchon( XDY_Entier va_numero_in,
	XDY_Octet va_cle_in,
	XDY_PR	*va_prQueue_out,
	XDY_Mot	*va_niveau_out,
	XDY_Entier	*va_longueur_out,
	XDY_Entier	*va_num_cause_out,
	XDY_Octet	*va_cle_cause_out);


/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK              succes
*   XDC_NOK             echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
------------------------------------------------------*/








/*------------------------------------------------------
* SERVICE RENDU :
*  lecture des caractéristiques d.une FMC delestge ou deviation
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int XZLG42_GetFmcDelestageDeviation (XDY_Entier va_numero_in,
	XDY_Octet va_cle_in,
	char *va_nomPlan_out,
	XDY_Entier	*va_numPlan_out,
	char *va_hierarchie_out,
	XDY_PR	*va_PR,
	XDY_PR	*va_pr2);


/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK              succes
*   XDC_NOK             echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
------------------------------------------------------*/


/*------------------------------------------------------
* SERVICE RENDU :
*  lecture des caractéristiques d.une FMC meteo
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int XZLG44_GetFmcMeteo (XDY_Entier va_numero_in,
	XDY_Octet va_cle_in,
	XDY_PR	*va_PRfin_out,
	XDY_Octet	*va_perturbation_out,
	XDY_Mot	*va_visibilite_out,
	XDY_Entier	*va_longueur_out,
	XDY_Octet	*va_nature_out,
	XDY_Nom		va_libelle_nature_out);



/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK              succes
*   XDC_NOK             echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
------------------------------------------------------*/


/*------------------------------------------------------
* SERVICE RENDU :
*  lecture des caractéristiques d.une FMC Regulation
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int XZLG565_GetFmcRegulation (XDY_Entier va_Numero_in,
        XDY_Entier  va_Cle_in,
        XDY_Entier  *va_Contexte_out,
		XDY_Entier	*va_Scenario_out,
        XDY_Entier  *va_Mesure_out,
        XDY_Entier  *va_Indicateur_Horaire_out,
        char*       va_Transitoire_out);



/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK              succes
*   XDC_NOK             echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
------------------------------------------------------*/




/*------------------------------------------------------
* SERVICE RENDU :
*  lecture des caractéristiques d.une FMC Manifestation, grève, opération escargot
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int XZLG47_GetFmcManifestation ( XDY_Entier va_numero_in,
	XDY_Octet va_cle_in,
	XDY_Mot	*va_nb_out,
	XDY_Booleen	*va_perception_out,
	XDY_Booleen	*va_violence_out);



/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK              succes
*   XDC_NOK             echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
------------------------------------------------------*/


/*------------------------------------------------------
* SERVICE RENDU :
*  lecture des caractéristiques d.une FMC travaux
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int XZLG49_GetFmcTravaux ( XDY_Entier va_numero_in,
	XDY_Octet va_cle_in,
	XDY_Booleen	*va_mobile_out,
	XDY_Booleen	*va_nature_out,
	XDY_Octet	*va_type_out,
	XDY_Booleen	*va_continu_out,
	XDY_Octet       *va_VR_out,
	XDY_Octet       *va_VM2_out,
	XDY_Octet       *va_VM1_out,
	XDY_Octet       *va_VL_out,
	XDY_Octet       *va_BAU_out,
	XDY_Octet       *va_VR_I_out,
	XDY_Octet       *va_VM2_I_out,
	XDY_Octet       *va_VM1_I_out,
	XDY_Octet       *va_VL_I_out,
	XDY_Octet       *va_BAU_I_out,
	XDY_PR		*va_PRdebut_VR_out,
	XDY_PR		*va_PRfin_VR_out,
	XDY_PR		*va_PRdebut_VM2_out,
	XDY_PR		*va_PRfin_VM2_out,
	XDY_PR		*va_PRdebut_VM1_out,
	XDY_PR		*va_PRfin_VM1_out,
	XDY_PR		*va_PRdebut_VL_out,
	XDY_PR		*va_PRfin_VL_out,
	XDY_PR		*va_PRdebut_BAU_out,
	XDY_PR		*va_PRfin_BAU_out,
	XDY_PR		*va_PRdebut_VR_I_out,
	XDY_PR		*va_PRfin_VR_I_out,
	XDY_PR		*va_PRdebut_VM2_I_out,
	XDY_PR		*va_PRfin_VM2_I_out,
	XDY_PR		*va_PRdebut_VM1_I_out,
	XDY_PR		*va_PRfin_VM1_I_out,
	XDY_PR		*va_PRdebut_VL_I_out,
	XDY_PR		*va_PRfin_VL_I_out,
	XDY_PR		*va_PRdebut_BAU_I_out,
	XDY_PR		*va_PRfin_BAU_I_out,
	XDY_PR		*va_pr_tete_out,
	XDY_Entier	*va_longueur_out,
	XDY_Entier	*va_vitesse_out);



/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK              succes
*   XDC_NOK             echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
------------------------------------------------------*/


/*------------------------------------------------------
* SERVICE RENDU :
*  lecture des caractéristiques d.une FMC Incident sous concessionnaire
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int XZLG50_GetFmcSousCnc ( XDY_Entier va_numero_in,
	XDY_Octet va_cle_in,
	char	*va_nom_out,
	XDY_Booleen	*va_ferme_out,
	XDY_Booleen	*va_penurie_out,
	XDY_Octet	*va_carburant_out);


/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK              succes
*   XDC_NOK             echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
------------------------------------------------------*/


/*------------------------------------------------------
* SERVICE RENDU :
*  lecture des voies
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZLG60_Lire_Config_Voies( XDY_Entier va_numero_in,
	XDY_Octet va_cle_in,
	XDY_Octet	*va_VR_out,
	XDY_Octet	*va_VM2_out,
	XDY_Octet	*va_VM1_out,
	XDY_Octet	*va_VL_out,
	XDY_Octet	*va_BAU_out,
	XDY_Octet	*va_VR_I_out,
	XDY_Octet	*va_VM2_I_out,
	XDY_Octet	*va_VM1_I_out,
	XDY_Octet	*va_VL_I_out,
	XDY_Octet	*va_BAU_I_out,
	XDY_Octet       *va_point_car_out,
	XDY_Entier      *va_num_point_car_out,
	XDY_Octet	*va_nb_entrees_out,
	XDY_Octet	*va_nb_sorties_out);
/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK              succes
*   XDC_NOK             echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
------------------------------------------------------*/



/*------------------------------------------------------
* SERVICE RENDU :
*  lecture des caractéristiques d.une FMC Fermeture Echangeur
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int XZLG52_GetFmcFermEchangeur( XDY_Entier va_numero_in,
	XDY_Octet va_cle_in,
	XDY_Booleen	*va_entree1_out,
	XDY_Booleen	*va_entree2_out,
	XDY_Booleen	*va_sortie1_out,
	XDY_Booleen	*va_sortie2_out);

/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK              succes
*   XDC_NOK             echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
------------------------------------------------------*/



/*------------------------------------------------------
* SERVICE RENDU :
* bouchon consequence ou pas
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int XZLG55_IsBouchonConsequence(XDY_Entier va_numero_in,
                        XDY_Octet va_cle_in,
                        XDY_Octet *va_nb_out,
			XDY_Entier	*va_num_out,
			XDY_Octet	*va_cle_out);


/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK              succes
*   XDC_NOK             echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
------------------------------------------------------*/





/*------------------------------------------------------
* SERVICE RENDU :
* lecture des caractéristiques d.une FMC Véhicule en Marche Arrière, Véhicule à Contresens, Véhicule Arrêté, Véhicule en Panne, Véhicule en Feu 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int XZLG45_GetFmcVehicule ( XDY_Entier va_numero_in,
                            XDY_Octet    va_cle_in,
                            XDY_Octet   *va_Type_out,
                            XDY_Octet   *va_passagers_out,
                            XDY_Booleen *va_bebe_out,
                            XDY_Booleen *va_persAgee_out,
                            XDY_Booleen *va_animaux_out,
                            XDY_Booleen *va_type_veh_escota_out,
                            XDY_Octet   *va_chargement_out,
                            char        *va_codemat_out,
                            char        *va_code_danger_out,
                            XDY_Mot     *va_code_nature_panne_out,
                            char        *va_nature_panne_out,
                            char        *va_positionnement_out,
                            XDY_Octet   *va_vehicule_roulant_out,
                            XDY_Octet   *va_pres_mat_dangereuse_out,
                            XDY_Mot     *va_nb_bebes_out,
                            XDY_Mot     *va_nb_agees_out);

/*ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK              succes
*   XDC_NOK             echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
------------------------------------------------------*/




/*------------------------------------------------------
* SERVICE RENDU :
*  lecture des voies
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZLG86_Lire_Config_Terrain( XDY_Autoroute va_autoroute_in,
	XDY_PR	va_PR_in,
        XDY_Octet va_sens_in,
        XDY_Octet       *va_VR_out,
        XDY_Octet       *va_VM2_out,
        XDY_Octet       *va_VM1_out,
        XDY_Octet       *va_VL_out,
        XDY_Octet       *va_BAU_out,
        XDY_Octet       *va_VR_I_out,
        XDY_Octet       *va_VM2_I_out,
        XDY_Octet       *va_VM1_I_out,
        XDY_Octet       *va_VL_I_out,
        XDY_Octet       *va_BAU_I_out);
       
/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK              succes
*   XDC_NOK             echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
------------------------------------------------------*/


/*------------------------------------------------------
* SERVICE RENDU :
*  lecture des voies
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZLG90_Recherche_FMC_Aval(XDY_Octet va_type_in, 
	XDY_Autoroute va_autoroute_in,
        XDY_PR  va_PR_in,
        XDY_Octet va_sens_in,
	int	va_distance_in,
        XDY_Booleen       *va_presence_out);

/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK              succes
*   XDC_NOK             echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
------------------------------------------------------*/


/*------------------------------------------------------
* SERVICE RENDU :
*  acces a la liste memoire des FMC
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
XZLGT_Liste_FMC *XZLG99_GetListe(XDY_Entier *va_nb);

/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK              succes
*   XDC_NOK             echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
------------------------------------------------------*/






/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  liste des zones de parcours definies dans le SAE
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZLG100_GetZones(  XDY_District         va_district_in,
                        XZLGT_Liste_Zdp  *va_liste_out);

/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK              succes
*   XDC_NOK             echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
------------------------------------------------------*/



/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  informations courantes de TDP d'une ZDP
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZLG101_GetZone_TDP(  XDY_Eqt         va_numero_in,
			XDY_TDP	*va_tdp_out,
			XDY_Horodate *va_horodate_out,
			XDY_Octet *va_validite_out);

/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK              succes
*   XDC_NOK             echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  elaboration TDP en fonction données RADT
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZLG102_GetRADT_TDP(  XDY_Autoroute         va_autoroute_in,
                        XDY_PR  va_debut_in,
			XDY_PR	va_fin_in,
			XDY_Sens	va_sens_in,
			XDY_Entier	*va_tdp_out);

/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK              succes
*   XDC_NOK             echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* filtrage d une zone
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZLG103_FiltreZone(  XDY_Eqt         va_numero_in,
                        XDY_Booleen  va_filtre_in);

/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK              succes
*   XDC_NOK             echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
------------------------------------------------------*/



/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* acces a pg_zdp
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

XZLGT_Liste_Zdp * XZLG104_GetListe();

/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK              succes
*   XDC_NOK             echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
------------------------------------------------------*/



/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  liberation des ressources memoire
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZLG199_FreeZones( );

/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK              succes
*   XDC_NOK             echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* liste des stations de comptage du SAE
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZLG200_GetStations( XZLGT_Liste_RADT	*va_liste_out);

/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK              succes
*   XDC_NOK             echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
------------------------------------------------------*/




/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* valeurs des derniers niveaux de trafic enregistres dans le SAE
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZLG202_GetNiveaux();

/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK              succes
*   XDC_NOK             echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
------------------------------------------------------*/



/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* infos courantes de trafic d'une station de comptage
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZLG201_GetTrafic(XDY_Eqt    va_numero_in,
                        XDY_Sens        va_sens_in,
			XDY_Entier	*va_Q_out,
			XDY_Entier	*va_V_out,
			XDY_Entier	*va_TO_out,
			XDY_Horodate	*va_horodate_out);

/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK              succes
*   XDC_NOK             echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
------------------------------------------------------*/



/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* parcours liste des stations
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZLG203_ParcoursRADT(XDY_FonctionInt    pa_fct_in);

/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK              succes
*   XDC_NOK             echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  dernieres infos de comptage pour un ensemble de stations
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZLG204_GetRADT_Data();

/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK              succes
*   XDC_NOK             echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  dernieres infos de comptage pour une station
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZLG205_GetRADT_Data_Station(int , XDY_Octet , XDY_FonctionInt );

/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK              succes
*   XDC_NOK             echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
------------------------------------------------------*/



/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
* infos courantes de niveau de trafic d'une station de comptage
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZLG206_Get_Niveau(XDY_Eqt    va_numero_in,
                        XDY_Sens        va_sens_in,
                        XDY_Octet    *va_niveau_out);

/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK              succes
*   XDC_NOK             echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
------------------------------------------------------*/
/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  liberation des ressources memoire
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZLG299_FreeRADT();

/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_OK              succes
*   XDC_NOK             echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*    trim
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void XZLG300_trim(char *);

/*
* ARGUMENTS EN ENTREE :
*       T_STR           Ligne;
*
* ARGUMENTS EN SORTIE :
*       T_STR           Ligne;
*
* CODE RETOUR :
*   aucun.
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*   parcours de la chaine
*
------------------------------------------------------*/

XZLGT_Liste_RADT *XZLG980_Get_RADT();
int XZLG981_Accede_Memoire_Partagee_RDT();
int XZLG982_Cree_Memoire_Partagee_RDT();
int XZLG983_Update_Checksum();
int XZLG984_Ecrit_Memoire_Partagee_FMC();
XZLGT_Liste_FMC *XZLG985_Get_FMC();
int XZLG986_Accede_Memoire_Partagee_FMC();
int XZLG987_Libere_Memoire(int va_type);
int XZLG988_Ecrit_Nombre(XZLGT_Nombre_Mem va_nombre);
int XZLG989_Libere_Memoire();
int XZLG990_Ecrit_Memoire_Partagee_ZDP();
XZLGT_Liste_Zdp *XZLG992_Get_ZDP();
XZLGT_Nombre_Mem *XZLG993_Get_Nombre();
int XZLG994_Accede_Memoire_Partagee_Nombre();
int XZLG995_Accede_Memoire_Partagee_TDP();
int XZLG996_Cree_Memoire_Partagee_TDP();
int XZLG997_Cree_Memoire_Partagee_FMC();
int XZLG999_Cree_Memoire_Partagee_Nombre();


void XZLG500_SemCall(int va_op, char);
int XZLG501_SemCree(char *va_nom, char);
int XZLG502_SemFree();
int XZLG503_SemGet(char *va_nom, char);
#endif
