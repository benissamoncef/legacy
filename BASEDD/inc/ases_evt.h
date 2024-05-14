/* Fichier : ases_evt.h   Release : 1.1   Date : 05/15/97	*
B
*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : BASEDD				*
* FICHIER : ases_evt.h				*
*************************************************
* DESCRIPTION :
* module externe de la tache client evtic
* pour le PC STRADA
* Il contient les definitions des fonctions
* utilisees dans ases_evt.c
*************************************************
* HISTORIQUE :
*
* PENOT	09 MAI 1997 : Creation 		V1.1
* PN	09 DEC 1998 : Champs date sur 21 et non 20 dem/1724 v1.4
* PN	04 MAR 1999 : ajout gravite v1.5
* PN    09 Aou 1999 : ajout include xzasc v1.6
* PNI	04 Jan 2007 : Modif pour production fichiers passerelle PX_METACOR DEM605 V1.7
* niepceron	11/12/06 1.8 : LTV =6 DEM732
* niepceron	07/01/09 1.9 : LTV =7 DEM859
* PNI	21/10/09 1.10 : Ajout du champ PHR dans T_Type_FMC DEM901
* niepceron	21/12/09 1.9 : LTV =8 DEM925
* JMG		09/04/12 : regulation DEM 1016
* niepceron	08/11/12 1.10 : LTV =8 DEM925
************************************************/

#ifndef ases_evt
#define ases_evt

/* include system */
#include        <stdio.h>
#include        <string.h>
#include        <ctype.h>
#include 	<stdlib.h>
 
#include "xdm.h"
#include "xdg.h"
#include "xzst.h"
#include "xzsm.h"
#include "xzis.h"
#include "xzaec.h"
#include "xzasc.h"
#include "xzao00.h"

/* definition de structures */

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
        };
        typedef struct code_loc_file loc_rds_file;

/*liste des fmc a exporter (dynamique)*/
typedef struct {
        XDY_Entier      Numero;
	XDY_Octet       Cle;
	XDY_Entier	Type;
} T_FMC;

typedef struct S_Liste_FMC {
	T_FMC			FMC;
	struct S_Liste_FMC	*Suivant;
} T_Liste_FMC;


/*conversion type de fmc sae -> code attribut strada (statique)*/
typedef struct {
	int	Type;
	char	Code_Strada[5];
	char	PHR_Strada[5];
} T_Type_FMC;


/*structure du bloc accident*/
typedef struct {
	XDY_Octet		Vehicules;
	XDY_Octet		PL;
	XDY_Octet		Cars;
	XDY_Octet		Rem;
	XDY_Octet		Blesses;
	XDY_Octet		Morts;
	XDY_Booleen		Matiere;
} T_B_ACCI;


/*structure du bloc meteo*/
typedef struct {
	int		Longueur;
	XDY_Mot		Visibilite;
} T_B_METE;


/*structure du bloc optionnel Vehicule*/
typedef struct {
	char		Type[100];
	XDY_Mot		Volume;
	char		Matiere[50];
} T_B_VEHI;


/*structure du bloc optionnel Vehicule lent*/
typedef struct {
	XDY_Mot		Longueur;
	XDY_Octet	Vitesse;
} T_B_VEHL;


/*structure du bloc optionnel Manifestation*/
typedef struct {
	char		rien;
} T_B_MANI;


/*structure du bloc optionnel sous-concessionnaire*/
typedef struct {
	char		rien;
} T_B_SSCC;


/*structure du bloc optionnel Bouchon*/
typedef struct {
	int		Longueur;
	XDY_Octet	Type;
} T_B_BOUC;


/*structure du bloc optionnel fermeture echangeur*/
typedef struct {
	char		rien;
} T_B_ECHA;


/*structure du bloc optionnel delestage*/
typedef struct {
	char		rien;
} T_B_DELE;

/*structure du bloc optionnel regulation*/
typedef struct {
	int	Scenario;
	char Commandes[500];
} T_B_REGUL;

/*structure du bloc optionnel basculement*/
typedef struct {
	int		Longueur;
	int		Vitesse;
	int		VitesseOpposee;
} T_B_BASC;


/*structure du bloc optionnel Travaux*/
typedef struct {
	int		Longueur;
	int		Mobiles;
} T_B_TRAV;

/*structure des blocs optionnels*/
typedef struct {
	char		Date_Debut[22];
	char		Date_Fin[22];
	char		Fausse_Alerte;
	char		Fin;
	char		Cause[50];
	int		Veracite;
	char		Voies[10];
	char		Prevision;
	XDY_Entier	Duree;
	XDY_Octet	Trafic;
	XDY_Octet       Gravite;
	char		Comment[351];
	union {
	  T_B_ACCI	Accident;
	  T_B_METE	Meteo;
	  T_B_VEHI	Vehicule;
	  T_B_VEHL	VehicLent;
	  T_B_MANI	Manif;
	  T_B_SSCC	Concession;
	  T_B_BOUC	Bouchon;
	  T_B_ECHA	Echangeur;
	  T_B_DELE	Delestage;
	  T_B_BASC	Basculement;
	  T_B_TRAV	Travaux;
	  T_B_REGUL	Regulation;
	} 		Bloc;
} T_Bloc_Optio;



/*structure du bloc obligatoire*/
typedef struct {
	char		SNM[50];
	char		ERF[50];
	char		DOB[10];
	char		PHR[10];
	int		CLV;
	char		INP[22];
	char		STA[22];
	int		LTV;
	int		VNM;
} T_Bloc_Oblig;


/*structure du fichier d'evt strada*/
typedef struct {
	char		Localisation[200];
	char		nbvoiesop[15];
	T_Bloc_Oblig	Bloc_Oblig;
	T_Bloc_Optio	Bloc_Optio;
	loc_rds_file		loc;
} T_Fichier;


/* Definition de la structure de donn�es pour PX-METACOR */
/*structure du bloc accident*/
typedef struct {
	XDY_Octet		VL;
	XDY_Octet		PL;
	XDY_Octet		Blesses_graves;
} T_B_INC_M;

/*structure du bloc control d'acces*/
typedef struct {
	XDY_Octet		Entree1;
	XDY_Octet		Entree2;
	XDY_Octet		Sortie1;
	XDY_Octet		Sortie2;
} T_B_ACC_M;

/*structure du bloc optionnel Restriction de voies*/
typedef struct {
	char		rien;
} T_B_REST_M;

/*structure du bloc optionnel Restriction de voies*/
typedef struct {
	char		rien;
} T_B_BOUC_M;


/*structure du bloc optionnel Travaux*/
typedef struct {
	char		Loca_deb_type[10];
	char		Loca_deb_id[256];
	char		sns_deb_id[3];
	char		pk_deb[10];
	char		pr_deb[10];
	char		Loca_fin_type[10];
	char		Loca_fin_id[256];
	char		sns_fin_id[3];
	char		pk_fin[10];
	char		pr_fin[10];
	int		v_reg;
} T_B_REGUL_M;

/*structure des blocs optionnels*/
typedef struct {
	char		rien;
	union {
	  T_B_INC_M	Incident;
	  T_B_ACC_M	Acces;
	  T_B_REST_M	Restriction;
	  T_B_BOUC_M	Bouchon;
	  T_B_REGUL_M Regulation[21];
	} 		Bloc;
} T_Bloc_Optio_M;



/*structure du bloc obligatoire*/
typedef struct {
	char		Type[50];
	char		Ref[50];
	int		Evo;
	char		Debut_prevu[21];
	char		Fin_prevue[21];
	char		Debut[21];
	char		Fin[21];
	char		Loca_deb_type[11];
	char		Loca_deb_nom[256];
	char		Loca_deb_id[11];
	char		sns_deb_id[3];
	char		pk_deb[11];
	char		pr_deb[11];
	char		Loca_fin_type[11];
	char		Loca_fin_nom[256];
	char		Loca_fin_id[11];
	char		sns_fin_id[3];
	char		pk_fin[11];
	char		pr_fin[11];
	char		rest[513];
	char		etat[11];
} T_Bloc_Oblig_M;


/*structure du fichier d'evt Metacor*/
typedef struct {
	T_Bloc_Oblig_M	Bloc_Oblig;
	T_Bloc_Optio_M	Bloc_Optio;
} T_Fichier_METACOR;



/* definition des constantes */
#define CM_V_ALERT_C	9
/* d�finition des variables globales � l'application */

/* definition des variables locales au module */

/* declaration de fonctions internes */

/* definition de fonctions externes */

extern int ases_evt_init();

extern int ases_free_ressources();


#endif
