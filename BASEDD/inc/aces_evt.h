/* Fichier : aces_evt.h   Release : 1.1   Date : 05/15/97	*
*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : BASEDD				*
* FICHIER : aces_evt.h				*
*************************************************
* DESCRIPTION :
* module externe de la tache client evtic
* pour le PC STRADA
* Il contient les definitions des fonctions
* utilisees dans aces_evt.c
*************************************************
* HISTORIQUE :
*
* PENOT	09 MAI 1997 : Creation 				V1.1
*PENOT	29 MAR 1998 : Libelles localisation 		V1.2
* NIEPCERON 05 MAR 1999 : TAILLE passe a 400 	V1.3
* F. LIZOT  01 MAR 2004 : Ajout de lnp a la structure code_att_nonobl (SAGA)	V1.4
************************************************/

#ifndef CLIENT_EVT
#define CLIENT_EVT

/* include system */
#include        <stdio.h>
#include        <string.h>
#include        <ctype.h>
#include 	<stdlib.h>
#include 	<malloc.h>
#include 	<string.h>
 
#include        "cstypes.h"
#include        "ctpublic.h"
#include "asql_com.h"

#include "xdm.h"
#include "xdg.h"
#include "xzst.h"
#include "xzsm.h"
#include "xzis.h"
#include "xzaec.h"
#include "xzao316.h"
#include "xzit.h"	/* Ajout SAGA */
#include "xzai500.h" 	/* Ajout SAGA */
#include "xzac80.h" 	/* Ajout SAGA */

/* definition des constantes */

#define CO_DELAI_FREQUENCE	360
#define CO_FIC_FREQUENCE	"frequence.txt"

#define CO_FIC_ATTSTR		"att_evt_strada.txt"
#define	CO_MAX_CARACTERE	250
#define TAILLE 			400
#define TAILLE_BD		250


#define CO_INCONNU_I		-1
#define CO_INCONNU_D		-1.0

#define C_PATH 			XDC_PATH_STRADA"/evtin/"
#define C_PATH_FTP		XDC_PATH_STRADA"/ftp/"
#define C_SCRIPT_SIT		C_PATH_FTP"sit_script_get"
#define C_PATH_LOC		XDC_PATH_STRADA"/localisation.txt"
#define C_FILE_LIB_LOC		"libelle_loc.txt"
#define CO_FIC_OBLI		"lib_obligatoires.txt"
#define NBR_PREMIER 		53
#define NBR_ELT_TABLE		59
#define NBR_ELT_OBLI		9

#define LOC_RDS_FRANCE		"F17"

/* codes de retour de la fonction aces_evt_cree_fmc (SAGA) */
#define XDC_DATEX_RIEN		0
#define XDC_DATEX_FMC		1
#define XDC_DATEX_TFM           2

/* definition de structures *//* structure utilisee pour stocker les libelles de localisation de STRADA_NET */
	typedef struct code_lib_obl {
		char    	clv[TAILLE];
		int		clv_b;	
		char  		dob[TAILLE];	
		int		dob_b;	
		char    	erf[TAILLE];	
		int		erf_b;	
		char    	inp[TAILLE];	
		int		inp_b;	
		char    	ltv[TAILLE];	
		int		ltv_b;	
		char    	pha[TAILLE];	
		int		pha_b;	
		char    	snm[TAILLE];	
		int		snm_b;	
		char    	sta[TAILLE];	
		int		sta_b;	
		char    	vnm[TAILLE];	
		int		vnm_b;	
	};
	
	typedef struct code_lib_obl lib_oblig;
	
	/* structure utilisee pour stocker les libelles de localisation de STRADA_NET */
	typedef struct code_lib_loc {
		char *    	l01_string[2];	
		int		l01_b;
		char * 		l02_string[23];
		int		l02_b;
		char * 		l03_string;
		int		l03_b;
		char * 		l04_string;
		int		l04_b;
		char * 		l05_string[5];
		int		l05_b;
		char * 		l06_string;
		int		l06_b;
		char * 		l07_string;
		int		l07_b;
		char * 		l08_string;
		int		l08_b;
		char * 		l09_string;
		int		l09_b;
		char * 		l10_string;
		int		l10_b;
		char * 		l11_string;
		int		l11_b;
		char * 		l12_string;
		int		l12_b;
	};
	
	typedef struct code_lib_loc lib_local;



	/* structure utilisee pour stocker les attributs de STRADA_NET */
	struct code_att{
	char *    	name;			/* code de l'evenement Strada 				*/
	int		validate;		/* determine si l'on veut conserver l'information 	*/
	XDY_Mot		type;			/* type de fmc associee 	*/
  	char *		string;			/* intitule du code       				*/
  	struct code_att* 	suivant;	/* pointeur sur l'attribut suivant			*/
	};
	typedef struct code_att * liste_att;
	
	/* structure utilisee pour stocker les localisations de STRADA_NET */
	struct code_loc {
	char	  	l01[10];		/* attribut 1  de la localisation RDS Strada 	*/
	char    	l02[10];		/* attribut 2  de la localisation RDS Strada 	*/
  	XDY_Octet	l03;			/* attribut 3  de la localisation RDS Strada 	*/
  	int		l04;			/* attribut 4  de la localisation RDS Strada 	*/
	char		l05[10];		/* attribut 5  de la localisation RDS Strada 	*/
	char   		l06[50];		/* attribut 6  de la localisation RDS Strada 	*/
	char   		l07[100];		/* attribut 7  de la localisation RDS Strada 	*/
  	int		l08;			/* attribut 8  de la localisation RDS Strada 	*/
  	int		l09;			/* attribut 9  de la localisation RDS Strada 	*/
	char    	l10[50];		/* attribut 10 de la localisation RDS Strada 	*/
  	int		l11;			/* attribut 11 de la localisation RDS Strada 	*/
  	int		l12;			/* attribut 12 de la localisation RDS Strada 	*/
  	
  	XDY_Booleen	l01_b;			/* attribut boolean 1  de la localisation RDS Strada chp obligat. */
  	XDY_Booleen	l02_b;			/* attribut boolean 2  de la localisation RDS Strada chp obligat. */
	XDY_Booleen	l03_b;			/* attribut boolean 3  de la localisation RDS Strada chp obligat. */
  	XDY_Booleen	l04_b;			/* attribut boolean 4  de la localisation RDS Strada chp obligat. */
  	XDY_Booleen	l05_b;			/* attribut boolean 5  de la localisation RDS Strada chp obligat. */
  	XDY_Booleen	l06_b;			/* attribut boolean 6  de la localisation RDS Strada chp obligat. */
	XDY_Booleen	l07_b;			/* attribut boolean 7  de la localisation RDS Strada chp obligat. */
  	XDY_Booleen	l08_b;			/* attribut boolean 8  de la localisation RDS Strada chp obligat. */
  	XDY_Booleen	l09_b;			/* attribut boolean 9  de la localisation RDS Strada chp obligat. */
  	XDY_Booleen	l10_b;			/* attribut boolean 10 de la localisation RDS Strada chp obligat. */
	XDY_Booleen	l11_b;			/* attribut boolean 11 de la localisation RDS Strada chp obligat. */
  	XDY_Booleen	l12_b;			/* attribut boolean 12 de la localisation RDS Strada chp obligat. */
	};
	typedef struct code_loc loc_rds;

	/* structure utilisee pour stocker les localisations de STRADA_NET */
	struct code_loc2 {
	char	  	l01[30];
	char	  	l02[30];
	int	  	l03;
	char	  	l04[30];
	int	  	l05;
	int	  	l06;
	int	  	l07;
	int		loc_b;
	char		string[CO_MAX_CARACTERE];
	};
	typedef struct code_loc2 localisation;
	
	/* structure utilisee pour stocker les attributs obligatoires associes aux evenements de STRADA_NET */
	struct code_att_obl {
	char	  	snm[50];		/* situation de reference 				*/
	int		snm_b;
  	char		erf[50];		/* element de reference      				*/
  	int		erf_b;
  	char		dob[40];		/* type de situation					*/
  	int		dob_b;
  	char		pha[50];		/* type de situation pour le moment			*/
  	int		pha_b;
	int		clv;			/* code version						*/
	int		clv_b;
  	char		inp[30];		/* heure d'entree					*/
  	int		inp_b;
  	char		sta[30];		/* heure de debut					*/
  	int		sta_b;
	int		ltv;			/* numero table de localisation RDS = 1 pour le moment	*/
	int		ltv_b;
	int		vnm;			/* numero de version d'information			*/
	int		vnm_b;
	};
	typedef struct code_att_obl att_obl;

	/* structure utilisee pour stocker les attributs non obligatoires associes aux evenements de STRADA_NET */
	struct code_att_nonobl {
	XDY_Horodate	  	sto;		/* STO 				*/
  	XDY_Booleen		end;		/* END      				*/
  	XDY_Octet		qin;		/* QIN      				*/
  	XDY_Booleen		can;		/* CAN					*/
  	XDY_Octet		nve;		/* NVE			*/
  	XDY_Octet		nhl;		/* NHL			*/
  	XDY_Octet		nhv;		/* NHV			*/
  	XDY_Octet		cav;		/* CAV			*/
  	XDY_Octet		nbu;		/* NBU			*/
  	XDY_Octet		inj;		/* INJ			*/
   	XDY_Octet		dea;		/* DEA			*/
  	char			loa[4];		/* LOAD					*/
   	XDY_Octet		sev;		/* SEV			*/
   	XDY_Entier		que;		/* QUE			*/
   	XDY_Entier		len;		/* LEN			*/
   	XDY_Mot		vis;		/* VIS			*/
   	XDY_Octet		mot;		/* MOT			*/
   	XDY_Octet		nln;		/* NLN			*/
   	char			lnk[50];	/* LNK			*/
   	char			dis[351];	/* DIS			*/
   	char			tet[4];		/* TET			*/
   	char			sur[351];	/* SUR			*/
   	int			duv;		/* DUV DUR			*/
	int                     lnp;            /* LNP (ajout SAGA)  	*/
	};
	typedef struct code_att_nonobl att_nonobl;



/* d≈finition des variables globales » l'application */
extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    * XZAGV_Context;

/* definition des variables locales au module */

/* declaration de fonctions internes */

/* definition de fonctions externes */

	/* procedure utilisee lors de l'initialisation du seveur evtic
 	  	- Programmation du timer de donees evenements
  	 	- lecture du fichier de config frequence.txt
   		- initialisation de la liste contenant les codes equipement Strada
   		  dans le fichier att_evt_strada.txt                                        */

   	extern int aces_evt_init();

	/* procedure permettant de recupere les fichiers evenements sur le PC Strada        */

	extern void aces_evt_ftp();
	
	
	/* procedure permettant de decoder les fichiers evenements, de verifier leur format
	   et de fournir les informations a la base de donnees                              */
	   
	extern void aces_evt_decode();

	/* procedure permettant de liberer la memoire de la liste chainee 
	   evenements_strada lorsque la tache client evenement est arretee             	    */

	extern void aces_evt_freemem();

#endif
