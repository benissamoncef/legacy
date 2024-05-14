/*X  Fichier : $Id: xdy.h,v 1.117 2021/06/15 17:24:36 devgfi Exp $      Release : $Revision: 1.117 $        Date : $Date: 2021/06/15 17:24:36 $
-------------------------------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
-------------------------------------------------------------------------------
* SOUS-SYSTEME  XDMICG
-------------------------------------------------------------------------------
* MODULE DEFTYPE * FICHIER xdy.h
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Definition des constantes generales   
*
-------------------------------------------------------------------------------
* HISTORIQUE :
*
* Mercier	17 Aout 94	: Creation
* Mismer	04 Oct	94	: Ajout des types XDY_EMes_DAI et XDY_ENiveau_Service
* Niepceron	10 Oct	94	: Modification de XDY_EtatCom par XDY_Octet dans les XDY_Etat_xxx (1.45)
*
* Mercier O.    13 Oct 1994     : Ajout de XDY_TypeFich et 
*					Correction de l entete V 1.49
* Niepceron	14 Oct	94	: Ajout XDY_CoefTranslation XDY_CoefExtension 1.49
* Nagiel.E	17 Oct	94	: Ajout XDY_EConf_Voies_DAI 1.50
* volcic	19 Oct	94	: Modif XDY_NomAuto (v1.53)
* volcic	19 Oct	94	: Modif XDY_NomAuto (v1.54)
* T.Milleville  26 Oct  94      : Ajout  XDY_DisplayUnix  V 1.55
* Niepceron     26 Oct  94      : Ajout  XDY_Act  V 1.56
*
* T.Milleville  26 Oct  94      : Ajout  XDY_DisplayUnix  V 1.55
* Nagiel.E	21 Nov	94	: Modif de XDY_Etat_NAV  V 1.59
* Nagiel.E	30 Nov	94	: Ajout XDY_NomST V 1.60
* Mismer.D	02 Dec	94	: RIEN			 V 1.61
* volcic	08 Dec	94	: Ajout XDY_FichEquext (v1.62)
*				        XDY_TxtAlarme
*				        XDY_TrameMesure
*
* Lecrivain     29 Dec  94      : XDY_Etat_Autotel, XDY_Etat_operator  V 1.68
* Nouaille M.   02 Jan  95      : Ajout XDY_Prefixe et XDY_Renvoi +
*				  Modif structurestelephobe et operator : changt cptr rendu 
*				  et etat communication (string -> octet) 
*				  passage en version V 1.69
* GABERT P      05 Jan  95      : XDY_Texte_SAT  V 1.70
* GABERT P      05 Jan  95      : XDY_FIC_ACC    V 1.71
*                                 XDY_FIC_TRA
*                                 XDY_FIC_TVX
*                                 XDY_FIC_MTO
*                                 XDY_FIC_PMV
*                                 XDY_FIC_FER
* GABERT P      10 Jan  95      : XDY_FIC_ACC    V 1.72
* H. GACOGNE    12 Jan  95      : XDY_FIC_DEB (V 1.77) (V 1.79)
* H. GACOGNE    12 Jan  95      : les voies sont de type XDY_Voie ( <> XDY_Entier) (V 1.80)
* TORREGROSSA   14 Mar  95      : XDY_TexteReveil (V 1.81)
* Niepceron     24 Mar  95      : Ajout  XDY_Nom_Ech  (V 1.82)
* Mismer.D	12 Nov 1996	: Modif METEO ajout XDY_EMes_Meteo (DEM/1232) (V 1.84)
*                                 modif de XDY_Etat_PMV
* Guilhou	02 mai 1997	: modif XDY_ZDP (V 1.85)
* Guilhou	07 mai 1997	: re-modif de XDY_ZDP (ajout champ traite) 1.86
* Guilhou	06 juin 1997	: re-re-modif de XDY_ZDP (dest tfm sur 50 car) 1.87 (dem/1440)
* Guilhou	29 sep 1997	: ajout de XDY_Destination_SAD 1.88 (dem/1502)
* JMG + DM	27/04/04	: chgt type XDY_Etat_PAL 1.89
* JMG		13/09/05	: ajout types PMVA BAF BAD 1.91
* PNI		06/04/06	: ajout de XDY_CodeCam 1.93 DEM565
* JPL		15/10/10	: Ajout de XDY_AdresseIP (DEM 954)  1.94
* JPL		17/11/10	: Suppression warnings mineurs pour compilation sur architecture HP ia64 (DEM 961)  1.95
* JMG		22/11/10	: ajout types radio
* JPL		08/12/10	: Migration architecture HP ia64 (DEM 961): XDY_Entier et XDY_PR "int" en mode LP64  1.97
* JPL		14/12/10	: Ajout de XDY_Protocole, XDY_Port et XDY_Encoder  (DEM 954)  1.98
* JPL		11/01/11	: Migration architecture HP ia64 (DEM 971): XDY_LongEntier long en tous modes  1.99
* VR            03/11/11        : Ajout Destination 2.0 (DEM/1014) 1.100
* SDU		10/02/12 	: DEM 1015 -Communautaire
* VR		29/05/12	: PRV DEM/1016
* JPL		18/06/12	: Ajout type de "numero" (textuel) de sortie (DEM 1033)  1.103
* VR		26/05/12 : Ajout DTP PMA (DEM/1014 PMA)
* JPL		08/02/13	: Ajout fonctions XDY01_Supprimer_Espaces_De_Fin et XDY02_Copie_Sans_Espaces_De_Fin  1.106
* JMG		16/04/16 : ajout SAGA 1.107
* JPL		26/04/17	: Ajout fonctions XDY03_Supprimer_Espaces et XDY04_Copie_Sans_Espaces  1.108
* JPL		26/09/17	: Champ 'Code_Zone' de type XDY_Mot dans XDY_ZDP (DEM 1236)  1.109
* LCL		22/02/18	: Ajout CTRL_FEUX 1.110 DEM1284
* JMG		17/10/18	: ajout XDY_PortIP DEM2306
* JMG		12/11/19	: surcharge TDP DEM1365 1.112
* JMG		12/01/20	: ajout IMU 1.113
* CGR		07/12/20	: ajout XDY_ChaineIMU et XDY_Etat_IMU DEM-SAE155 1.114
* ABE		01/04/21	: creation PAU_SONO DEM-SAE244 1.115
* ABE		08/06/21	: Fix SONO xdy_autoroute vers xdy_NomAutoroute DEM-SAE244 1.116
* ABK	17/08/23: Ajout du XDY_Etat_PIC

-----------------------------------------------------------------------------*/
/* Fichiers inclus */

#include "xdc.h"

#ifndef xdy
#define xdy


/* definitions de types exportes */

typedef char            XDY_DisplayUnix[80];    /* display : surfpo2:0.1 par ex. */

typedef char        	XDY_TypeFich[XDC_Lon_TypeFich]; /* type de fichier de conf*/

typedef char		XDY_Nom[XDC_Lon_Nom];   /* Nom (eqt,utilisateur,...) */

typedef unsigned char	XDY_Octet;            	/* Type Octet : CS_TINYINT */

typedef short       	XDY_Mot;		/* Type Mot   : CS_SMALLINT */

#if defined (_LP64)
typedef int		XDY_Entier;		/* Type Entier: CS_INT */
#else
typedef long		XDY_Entier;		/* Type Entier: CS_INT */
#endif

typedef long		XDY_LongEntier;		/* Encapsule le type C "long" en tous modes (printf ...) */


typedef char		XDY_NomMachine[XDC_Lon_NomMachine];	
						/* Nom d'une machine */

typedef unsigned char	XDY_TypeMachine;	/* Type d'une machine : CS_TINYINT */

typedef char          	XDY_NomSite[XDC_Lon_NomSite];		
						/* Nom d'un site */

typedef	unsigned char	XDY_Sens;		/* Type Sens : CS_TINYINT*/

typedef XDY_Entier	XDY_Alerte;		/* Numero d'alerte : CS_INT*/

typedef unsigned char   XDY_Voie;               /* Type Voie : CS_TINYINT*/

typedef int   		XDY_Valeur;

typedef char   		XDY_REG_PARAMS[15];

typedef char		XDY_Etat_Module[5];	/* Etat d'un module */
typedef char		XDY_Etat_Module_PAL[10];	/* Etat d'un module */

typedef char		XDY_NomTache[XDC_Lon_NomTache];	
						/* Nom d'une tache */

typedef char		XDY_Datagroup[XDC_Lon_Datagroup];	
						/* Nom d'un datagroup */

typedef char		XDY_NomSysteme[XDC_Lon_NomSysteme];	
						/* Nom d'un sous-systeme */

typedef void		(*XDY_Fonction)();	/* Nom d'une fonction utilisateur */

typedef int		(*XDY_FonctionInt)();	/* Nom d'une fonction utilisateur */

typedef int		XDY_Fichier;            /* File descriptor */

typedef double    	XDY_Horodate;		/* Type Horodate */

typedef char            XDY_Heure[9];           /* Heure au format HH:MM:SS */

typedef XDY_Octet	XDY_TypeEqt;		/* Type type d'equipement : CS_TINYINT */

typedef XDY_Mot		XDY_Eqt;		/* Numero d'equipement : CS_SMALLINT */

typedef XDY_Octet	XDY_TypeAlerte;		/* Type type d'alerte : CS_TINYINT*/

typedef XDY_Nom		XDY_NomType;		

typedef char		XDY_TexteAlerte[XDC_Lon_TexteAlerte];	/* Texte d'alerte */

typedef XDY_Mot         XDY_Portion;		/* Identifiant d'une portion : CS_SMALLINT*/

typedef XDY_Mot         XDY_Segment;		/* Identifiant d'un segment : CS_SMALLINT*/

typedef float		XDY_CoefTranslation; /* Coef de translation sur portion ou segment */
typedef float		XDY_CoefExtension;   /* Coef d'extension sur portion ou segment */

typedef float           XDY_Coeff;		/* Coef pour calcul donnees 6mn glissantes */

typedef double          XDY_Moyenne;		/* Moyenne pour calcul donnees 6mn glissantes */

typedef XDY_Octet       XDY_NoLS;		/* Numero de ligne serie : CS_TINYINT*/

typedef XDY_Octet       XDY_Autoroute;		/* Numero d'autoroute : CS_TINYINT*/

typedef char		XDY_NomAuto[XDC_Lon_NomAuto];		/* Nom d'autoroute */

#if defined (_LP64)
typedef int		XDY_PR;			/* Point de repere : CS_INT */
#else
typedef long		XDY_PR;			/* Point de repere : CS_INT */
#endif

typedef unsigned char	XDY_Booleen;		/* Booleen */

typedef XDY_Octet       XDY_QualAlarme;		/* Qualification alerte */

typedef XDY_Mot         XDY_TypeAlarme;		/* Type de l'alarme */

typedef XDY_Entier	XDY_Alarme;		/* Numero d'alarme */

typedef char		XDY_Phrase[XDC_Lon_Phrase];		/* */

typedef char            XDY_Commentaire[XDC_Lon_Commentaire];	/* Commentaire */

typedef XDY_Mot         XDY_Operateur;		/* Numero d'operateur */

typedef XDY_Entier      XDY_Etat_Eqt;		/* Disponible,indisponible,critique,marginal,inhibe,desactive : CS_INT*/

typedef char            XDY_NomEqt[XDC_Lon_NomEqt];		/* Nom de l'equipement */

typedef char            XDY_Tel[XDC_Lon_Tel];	/* Telephone */

typedef char            XDY_Ligne_PMV[XDC_Lon_Ligne_PMV];	
						/* Ligne texte PMV */

typedef char 		XDY_Ligne_PMVA[XDC_Lon_Ligne_PMVA] ;

typedef char 		XDY_Ligne_PRV[XDC_Lon_Ligne_PRV] ;

typedef char 		XDY_Picto_PIC[XDC_Lon_Picto_PIC] ;		/* Equipement PIC */

typedef char		XDY_Bandeau_PIC[XDC_Lon_Bandeau_PIC];

typedef char            XDY_Picto[XDC_Lon_Picto];		/* Symbole picto */

typedef XDY_Mot         XDY_Station;		/* Identification d'une station RADT */

typedef char            XDY_TexteAnn[XDC_Lon_TexteAnn];	
						/* Commentaire annuaire */

typedef XDY_Octet	XDY_District;		/* Numero de district */

typedef XDY_Entier      XDY_ResConf;		/* Valeur XDC_Ajout,XDC_Modif,XDC_Suppr,XDC_Echec */

typedef char            XDY_Rgs[XDC_Lon_Rgs];	/* */

typedef char            XDY_NomFich[XDC_Lon_NomFich];	/* Nom de fichier */

typedef char            XDY_Abrev[XDC_Lon_Abrev];	/* Abreviation */

typedef char            XDY_CodeEqt[XDC_Lon_CodeEqt];	/* Code equipement */

typedef char            XDY_CodeCam[XDC_Lon_CodeCam];	/* Code camera */

typedef char            XDY_Login[XDC_Lon_Login];	/* Identification de login */

typedef char		XDY_Basedd[XDC_Lon_BaseDD];     /* Nom de base de donnees */

typedef char            XDY_TxtPicto[XDC_Lon_TxtPicto];	/* Texte Picto */

typedef char            XDY_NomPicto[XDC_Lon_NomPicto];	/* Nom  Picto */

typedef char            XDY_Texte[XDC_Lon_Texte];	/* Morceau d'un texte */

typedef char		XDY_Scenario[XDC_Lon_Scenario];	/* Identifiant scenario */
typedef char		XDY_NomZone[XDC_Lon_ZoneRegul];	/* nom de la zone de regulation */

typedef char		XDY_VitesseRegulTxt[XDC_Lon_VitesseRegul];	/* Identifiant Vitesse PRV */
typedef XDY_Entier	XDY_VitesseRegul;	/* Vitesse */

typedef char		XDY_LibellePlanDeFeux[XDC_Lon_LibellePlanDeFeux];	/* Libelles plans de feux controleurs */ 

typedef char		XDY_Bandeau[XDC_Lon_Bandeau];	/* Identifiant Vitesse PRV */


typedef char		XDY_NomParamRegul[XDC_REG_LONG_NOM_PARAM];	/* Longueur des noms des parametres de regul */

typedef char		XDY_MatDang[XDC_Lon_MatDang];	/* Matieres dangereuses */

typedef char		XDY_TexteVeh[XDC_Lon_TexteVeh]; /* Conducteur, Nationalite, ... */

typedef char		XDY_EtatCom[XDC_Lon_EtatCom];   /* Etat d'un communication ( MAA, RAU...) */

typedef char		XDY_Msg_ES80[XDC_Lon_MsgES80];  /* Message ES80 */

typedef char	    XDY_NomST[XDC_Lon_NomEqt];	/* Nom Serveur de terminaux */

typedef char	    XDY_FichEquext[XDC_PATH_ABS_SIZE]; /* Nom Fichier Config Equext */

typedef char        XDY_TxtAlarme[XDC_Lon_Texte_Alarme]; /* Message Alarme pour reception trame incorrecte (Equext) */ 

typedef char        XDY_TrameMesure[XDC_Lon_TrameMesure]; /* Trame mesure TERDT (Equext) */ 

typedef char		XDY_Reponse_Oper[64+1];		/* Compte-rendu cmd. operator */
typedef char		XDY_Reponse[16+1];		/* Compte-rendu cmd. tel. */
typedef char		XDY_Num_Oper[6+1];		/* Numero abonne */
typedef char		XDY_Prefixe[3+1];		/* Prefixe d'appel */
typedef char		XDY_Renvoi[6+1];		/* Code de renvoi */
typedef char		XDY_Long_Buffer[1024+1];	/* Buffer lecture longue trame OPERATOR */

typedef char		XDY_Texte_SAT[XDC_Lon_Texte_SATIR];

typedef char		XDY_TexteReveil[XDC_Lon_TexteReveil];

typedef char		XDY_Nom_Ech[XDC_Lon_Ech];		/* Nom echangeurs */
typedef char		XDY_Sortie[XDC_Lon_Sortie + 1];		/* Numero de sortie */

typedef char		XDY_AdresseIP[XDC_Lon_AdresseIP+1];	/* Adresse IP camera */

typedef char		XDY_Protocole[XDC_Lon_Protocole+1];	/* Protocole dialogue camera */

typedef char		XDY_Encoder[XDC_Lon_Encoder+1];		/* Encoder dialogue camera */

typedef XDY_Mot		XDY_Port;				/* Port de communication (general) : CS_SMALLINT */

typedef int		XDY_PortIP;

typedef int   		XDY_TypeAppelRadio;
typedef char  		XDY_GroupeRadio[XDC_Lon_GroupeRadio];
typedef char  		XDY_Radio[XDC_Lon_Radio];

typedef	char		XDY_ChaineIMU[XDC_Lon_ChaineIMU];

typedef char            XDY_MessageSONO[XDC_Lon_MessageSono];               /* Message RAU Sonorisation */ 			

typedef struct {
        XDY_TypeEqt 	Typ_Eqt;               	/* Type d'equipement */
	XDY_Mot		NoEqt;			/* Numero d'equipement */
} XDY_Num_Eqt;					/* Equipement */

typedef struct {
	XDY_Mot 	Val;
	XDY_Octet	Indic;
} XDY_Val_Mes;					/* Description d'une mesure */

typedef struct {
	XDY_Val_Mes	ValQ;                   /* Quantite de vehicules */
	XDY_Val_Mes	ValV;			/* Vitesse de vehicules */
	XDY_Val_Mes     ValTO;			/* Taux d'occupation des voies */
	XDY_Val_Mes     ValPL;			/* % de poids lourds */
} XDY_Mes;					/* Valeurs des mesures RADT */

typedef struct {
	XDY_Horodate	Horodate;	
	XDY_Eqt		Numero;	
	XDY_Octet	Periode;
	XDY_Sens	Sens;
	XDY_Voie	Voie;
	XDY_Mes		Mesures;
} XDY_EMes_RADT;				/* Mesures RADT */

typedef struct {
	XDY_Horodate	Horodate;		/* Horodate des mesures */
	XDY_Eqt		Numero;			/* Numero d'equipement */
	XDY_Sens	Sens;			/* Sens */
	XDY_Mot		DIVcl1;			/* DIV classe 1 */
	XDY_Mot         DIVcl2;                 /* DIV classe 2 */
	XDY_Mot         DIVcl3;                 /* DIV classe 3 */
	XDY_Mot         DIVcl4;                 /* DIV classe 4 */
	XDY_Mot         DIVcl5;                 /* DIV classe 5 */
} XDY_EMes_Classe_DIV;

typedef struct {
	XDY_Horodate	Horodate;		/* Horodate des mesures */
	XDY_Eqt		Numero;			/* Numero d'equipement */
	XDY_Mot		NumAnalyseur;		/* Numero d'analyseur */
	XDY_Sens	Sens;			/* Sens */
	XDY_Octet	Chaussee;		/* Chaussee */
	XDY_Mot		VitesseVoie[5];		/* Vitesse par voie */
	XDY_Mot         LongueurVoie[5];        /* Longueur par voie */
	XDY_Mot         TO;              	/* Taux d'occupation */
	XDY_Mot         VitesseMoyenne;         /* Vitesse moyenne */
} XDY_EMes_DAI;

typedef struct {
	XDY_Horodate	Horodate;		/* Horodate des mesures */
	XDY_Eqt		Numero;			/* Numero d'equipement */
	XDY_Sens	Sens;			/* Sens */
	XDY_Mot		NivService;		/* Niveau de service (fluide ,sature ...) */
} XDY_ENiveau_Service;

typedef struct {
	XDY_Horodate	Horodate;		/* Horodate des mesures */
	XDY_Eqt		Numero;			/* Numero d'equipement */
	XDY_Sens	Sens;			/* Sens */
	XDY_Mot		Cumul;			/* Cumul poids */
	XDY_Mot		NB_PL;			/* Nombre poids lourds */
} XDY_EMes_Poids;

typedef struct {
	XDY_Horodate	Jour;			/* Horodate des mesures */
	XDY_Eqt		Numero;			/* Numero d'equipement */
	XDY_Booleen	Dispo;			/* Disponibilit� (XDC_VRAI ou XDC_FAUX) */
	XDY_Octet	Code;			/* Code type de pluie */
	XDY_Horodate	HeureDebut;		/* Horodate de debut pluie */
	XDY_Horodate	HeureFin;		/* Horodate de fin pluie */
} XDY_EMes_Meteo;

typedef struct {
	XDY_Eqt		Numero;			/* Numero d'equipement */
	XDY_Ligne_PMV	Texte1;			/* Texte 1ere ligne */
	XDY_Octet	Affichage1;		/* texte,temperature,heure */
	XDY_Booleen	Clign1;			/* Clignotement 1ere ligne */
	XDY_Ligne_PMV	Alternat1;		/* Texte 1ere ligne alternativement */
	XDY_Ligne_PMV   Texte2;                 /* Texte 2eme ligne */
	XDY_Octet       Affichage2;		/* texte,temperature,heure */
	XDY_Booleen     Clign2;                 /* Clignotement 2eme ligne */
	XDY_Ligne_PMV   Alternat2;		/* Texte 2eme ligne alternativement */
	XDY_Ligne_PMV	Texte3;			/* Texte 3eme ligne */
	XDY_Octet       Affichage3;		/* texte,temperature,heure */
	XDY_Booleen     Clign3;			/* Clignotement 3eme ligne */
	XDY_Ligne_PMV   Alternat3;		/* Texte 3eme ligne alternativement */
	XDY_Ligne_PMV	Texte4;			/* Texte 4eme ligne */
	XDY_Octet       Affichage4;		/* texte,temperature,heure */
	XDY_Booleen     Clign4;			/* Clignotement 3eme ligne */
	XDY_Ligne_PMV   Alternat4;		/* Texte 4eme ligne alternativement */
	XDY_Booleen	Flash;			/* Etat flash */
	XDY_Mot		Destination;		/* Numero de DEstination du tdp */
	XDY_Mot         DestinationAlternat;     /* Numero de DEstination du tdp */
	XDY_Octet	DepassSeuil;		/* Flag de depassement de seuil haut */
	XDY_Mot         DestinationNoeud;       /* Nouvelle destination*/
} XDY_Etat_PMV;					/* Etat d'un PMV */

typedef struct {
	XDY_Eqt		Numero;			/* Numero d'equipement */
	XDY_ChaineIMU 	ChaineIMU;
	int		Intervalle;
	int		Cycles;
	int		Duree;
	int		NoAction;
} XDY_Etat_IMU;					/* Etat d'un IMU */
	
typedef struct {								
	XDY_MessageSONO   Message;
	XDY_Mot	     Nb_PAUErreur; 
	XDY_Mot	     Nb_PAUPilotes;								
	XDY_NomAuto	Autoroute;
	XDY_Sens	Sens;
	XDY_PR		PR_debut;
	XDY_PR		PR_fin;
	int		NoAction;
} XDY_Etat_SONO; /* Etat d'un PAU SONO */

															
typedef struct {
	XDY_Eqt		Numero;			/* Numero d'equipement */
	XDY_Ligne_PMVA	Texte1;			/* Texte 1ere ligne */
	XDY_Octet	Affichage1;		/* texte,temperature,heure */
	XDY_Booleen	Clign1;			/* Clignotement 1ere ligne */
	XDY_Ligne_PMVA	Alternat1;		/* Texte 1ere ligne alternativement */
	XDY_Ligne_PMVA   Texte2;                 /* Texte 2eme ligne */
	XDY_Octet       Affichage2;		/* texte,temperature,heure */
	XDY_Booleen     Clign2;                 /* Clignotement 2eme ligne */
	XDY_Ligne_PMVA   Alternat2;		/* Texte 2eme ligne alternativement */
	XDY_Ligne_PMVA	Texte3;			/* Texte 3eme ligne */
	XDY_Octet       Affichage3;		/* texte,temperature,heure */
	XDY_Booleen     Clign3;			/* Clignotement 3eme ligne */
	XDY_Ligne_PMVA   Alternat3;		/* Texte 3eme ligne alternativement */
	XDY_Ligne_PMVA	Texte4;			/* Texte 4eme ligne */
	XDY_Octet       Affichage4;		/* texte,temperature,heure */
	XDY_Booleen     Clign4;			/* Clignotement 3eme ligne */
	XDY_Ligne_PMVA   Alternat4;		/* Texte 4eme ligne alternativement */
	XDY_Ligne_PMVA	Texte5;			/* Texte 1ere ligne */
	XDY_Octet	Affichage5;		/* texte,temperature,heure */
	XDY_Booleen	Clign5;			/* Clignotement 1ere ligne */
	XDY_Ligne_PMVA	Alternat5;		/* Texte 1ere ligne alternativement */
	XDY_Booleen	Flash;			/* Etat flash */
	XDY_Mot		Dest1_proche;		/* Numero de DEstination du tdp */
	XDY_Mot     Dest1_loin;     /* Numero de DEstination du tdp */
	XDY_Mot		Dest2_proche;		/* Numero de DEstination du tdp */
	XDY_Mot     Dest2_loin;      /* Numero de DEstination du tdp */
	XDY_Octet	DepassSeuil;		/* Flag de depassement de seuil haut */
} XDY_Etat_PMVA;     

typedef struct {
	XDY_Eqt         Numero;                 /* Numero d'equipement */
	char  Texte1[XDC_Lon_Ligne_Eqt_SAGA];                 /* Texte 1ere ligne */
	XDY_Octet       Affichage1;             /* texte,temperature,heure */
	char  Alternat1[XDC_Lon_Ligne_Eqt_SAGA];              /* Texte 1ere ligne alternativement */
	char  Texte2[XDC_Lon_Ligne_Eqt_SAGA];                 /* Texte 1ere ligne */
	XDY_Octet       Affichage2;             /* texte,temperature,heure */
	char  Alternat2[XDC_Lon_Ligne_Eqt_SAGA];              /* Texte 1ere ligne alternativement */
	char  Texte3[XDC_Lon_Ligne_Eqt_SAGA];                 /* Texte 1ere ligne */
	XDY_Octet       Affichage3;             /* texte,temperature,heure */
	char  Alternat3[XDC_Lon_Ligne_Eqt_SAGA];              /* Texte 1ere ligne alternativement */
	char  Texte4[XDC_Lon_Ligne_Eqt_SAGA];                 /* Texte 1ere ligne */
	XDY_Octet       Affichage4;             /* texte,temperature,heure */
	char  Alternat4[XDC_Lon_Ligne_Eqt_SAGA];              /* Texte 1ere ligne alternativement */
	char  Texte5[XDC_Lon_Ligne_Eqt_SAGA];                 /* Texte 1ere ligne */
	XDY_Octet       Affichage5;             /* texte,temperature,heure */
	char  Alternat5[XDC_Lon_Ligne_Eqt_SAGA];              /* Texte 1ere ligne alternativement */
	XDY_Octet	Clignotement;
	XDY_Mot	Luminosite;
	char	Picto[15];
	char	Bandeau[15];
	int	Barriere; /*0 ouverture 1 fermeture*/
	int	Scenario;
	XDY_Eqt	TypeSAGA;
} XDY_Etat_Eqt_SAGA;

typedef struct {
	XDY_Eqt		Numero;			/* Numero d'equipement */
	char		LibellePlanDeFeux[XDC_Lon_LibellePlanDeFeux];
	XDY_Octet	NoPlanDeFeux;
} XDY_Etat_CFE;

typedef struct {
	XDY_Eqt			Numero;			/* Numero d'equipement */
	char 	Bandeau[12];
	char Vitesse[6];
	int             NoCMD;
} XDY_Etat_PRV;                                 /* Etat d'un PRV */

typedef struct {
	XDY_Eqt			Numero;			/* Numero d'equipement */
	XDY_Bandeau_PIC	Bandeau;
	XDY_Booleen		Flash;			/* Etat flash */
	XDY_Picto_PIC	Picto;			/* Symbole picto */
	int             NoCMD;			/* No Commande */
} XDY_Etat_PIC;                     /* Etat d'un PIC */

typedef struct {
	XDY_Eqt		Numero;			/* Numero d'equipement */
	char		Texte[XDC_Lon_TextePicto];
						/* Texte picto */
	XDY_Picto	Picto;			/* Symbole picto */
	XDY_Octet 	Clign;			/* Clignotement */
	int		NoCMD;			/* No Commande */
} XDY_Etat_Picto;				/* Etat d'un picto */

typedef struct {
	XDY_Eqt     	Numero;                 /* Numero d'equipement */
	XDY_Etat_Module Modules[XDC_Lon_Nb_Mod_Nav];/* Etat de chaque module */
} XDY_Etat_NAV;					/* Etat d'un NAV */


typedef struct {
	XDY_Eqt     	Numero;                 /* Numero d'equipement */
	XDY_Etat_Module Modules[XDC_Lon_Nb_Mod_BAF];/* Etat de chaque module */
} XDY_Etat_BAF;					/* Etat d'un BAF */

typedef struct {
	XDY_Eqt     	Numero;                 /* Numero d'equipement */
	XDY_Etat_Module Modules[XDC_Lon_Nb_Mod_BAD];/* Etat de chaque module */
} XDY_Etat_BAD;					/* Etat d'un BAD */

typedef struct {
	XDY_Eqt     	Numero;                 /* Numero d'equipement */
	XDY_Etat_Module_PAL Modules[XDC_Lon_Nb_Mod_Pal];	/* Etat de chaque module */
} XDY_Etat_PAL;					/* Etat d'un PAL */

typedef struct {
	XDY_Num_Eqt    	Numero;                	/* Numero d'equipement */
	XDY_Octet       Etat;			/* Etat du TOR */
} XDY_Etat_GTC;

typedef struct {
	XDY_Eqt		Numero;			/* Numero de MAA */
	XDY_Mot		CodeCmd;		/* Code de la derniere commande */
	XDY_Tel		NumTel;			/* Numero de telephone */
	XDY_Tel		NumRepondeur;		/* Numero de repondeur */
	XDY_Octet	CrCmd;			/* Compte rendu de la commande */
	XDY_Octet	EtatCom;		/* Etat de la communication */
} XDY_Etat_MAA;

typedef struct {
	XDY_Eqt		Numero;			/* Numero de PAU */
	XDY_Eqt		NumPoste;		/* Numero de poste operateur */
	XDY_Mot		CodeCmd;		/* Code de la derniere commande */
	XDY_Octet	CrCmd;			/* Compte rendu de la commande */
	XDY_Octet	EtatComRAU;		/* Etat de la communication RAU */
	XDY_Octet	EtatComGN;		/* Etat de la communication GN */
} XDY_Etat_PAU;

typedef struct {
	XDY_Eqt		Numero;			/* Numero de Magneto */
	XDY_Eqt		NumImage;		/* Numero d'image enregistree */
	XDY_Mot		CodeCmd;		/* Code de la derniere commande */
	XDY_Octet	CrCmd;			/* Compte rendu de la commande */
} XDY_Etat_Magneto;

typedef struct {
	XDY_Eqt		Numero;			/* Numero de Camera */
	XDY_Mot		CodeCmd;		/* Code de la derniere commande */
	XDY_Octet	CrCmd;			/* Compte rendu de la commande */
	XDY_Eqt		NumMoniteur;		/* Numero de moniteur affectee */
	XDY_Eqt		NumPosition;		/* Numero de position affectee */
	XDY_Eqt		NumMagneto;		/* Numero de magneto associe */
	XDY_Octet	EtatCom;		/* Etat de la communication Video */
} XDY_Etat_Camera;

typedef struct {
	XDY_Eqt		Numero;			/* Numero de l'operateur */
	XDY_Mot		CodeCmd;		/* Code de la derniere commande */
	XDY_Octet	CrCmd;			/* Compte rendu de la commande */
	XDY_Msg_ES80	Msg;			/* Message transmis */
	XDY_Octet	EtatCom;		/* Etat de la communication Video */
} XDY_Etat_ES80;

typedef struct {
	XDY_Octet       TypEqt;
	XDY_Mot		NoTub_Ech;
	XDY_Octet	Type;
	XDY_Octet	NumBit;
	XDY_Booleen	Etat;
} XDY_Capt_Tube;				/* Etat des capteurs Tube (OP,CO,Lits,Extincteurs,...) */

typedef struct {
	XDY_Octet       classe;			/* Table en basse ou masque IHM */
	XDY_Octet       type;			/* Numero de type */
	XDY_Nom         nom;			/* Nom de type */
} XDY_Type_Evt;					/* Type d'evenement */

typedef struct {
	XDY_Entier	numero;			/* Numero d'evenement */
	XDY_Octet	cle;			/* Cle evenement */
} XDY_Evt;					/* Evenement */

typedef struct {
	XDY_Entier	numero;			/* Numero d'action */
	XDY_Octet	cle;			/* Cle action */
} XDY_Act;					/* action */


typedef struct {
	XDY_Eqt		NoEqtDAI;		/* Numero d'equipement DAI */
	XDY_Eqt		NoEqtAnaly;		/* Numero d'equipement d'analyseur */
	XDY_Mot		NoAnaly	;		/* Numero d'equipement d'analyseur */
	XDY_Voie	EtatVoie[5];		/* Normal,Inverse ou Neutralise */
}XDY_EConf_Voies_DAI;


typedef struct {
	XDY_Eqt				Numero;			/* Numero de l'equipement */
	XDY_Mot				CodeCmd;		/* Code de la derniere commande */
	XDY_Tel				NumAppel;		/* Numero d'appel */
	XDY_Tel				NumRepondeur;	/* Numero de ligne du repondeur */
	XDY_Octet			CptRendu;		/* Compte-rendu derniere commande */
	XDY_Octet			EtatCom;		/* Etat de la communication */
} XDY_Etat_Autotel;

typedef struct {
	XDY_Eqt				Numero;			/* Numero de l'equipement */
	XDY_Mot				CodeCmd;		/* Code de la derniere commande */
	XDY_Num_Oper			NumOper;		/* Numero de l'abonne */
	XDY_Msg_ES80			Msg;			/* Message transmis */
	XDY_Octet			CptRendu;		/* Compte-rendu derniere commande */
	XDY_Octet			EtatCom;		/* Etat de la communication */
} XDY_Etat_Operator;


typedef struct {

	XDY_Entier	Heure;
	XDY_Entier	Minute;
	XDY_Entier	Jour;
	XDY_Entier	Mois;
	XDY_Texte_SAT	Autoroute;
	XDY_Entier	Sens;
	float		PR;
	XDY_Entier	District;

	XDY_Texte_SAT	TypFic;
	XDY_Entier	Dest1;
	XDY_Entier	Dest2;
	XDY_Entier	Dest3;
	XDY_Entier	Dest4;
	XDY_Entier	Dest5;
	XDY_Entier	Dest6;
	XDY_Entier	Dest7;
	XDY_Entier	Dest8;

	XDY_Entier	NumEvt;
	XDY_Texte_SAT	DateSaisie;
	XDY_Texte_SAT	DateAlerte;
	XDY_Entier	RefAutoroute;
	XDY_Entier	RefSens;
	XDY_Entier	TypEvt;
	XDY_Entier	TypIncid;
	XDY_Entier	EchEntree;
	XDY_Entier	EchSortie;
	XDY_Entier	Barriere;
	float		PR_Evt;

	XDY_Entier	CoupVoie;
	XDY_Entier	Bascule;
	XDY_Voie	BAU_I;
	XDY_Voie	VL_I; 
	XDY_Voie	VM1_I;
	XDY_Voie	VM2_I;
	XDY_Voie	VR_I;
	XDY_Voie	TPC;
	XDY_Voie	VR;
	XDY_Voie	VM2;
	XDY_Voie	VM1;
	XDY_Voie	VL;
	XDY_Voie	BAU;
	XDY_Voie	ConfVoie;
                        
	XDY_Texte_SAT	DateRetNormale;
	XDY_Entier	Personnalite;
	XDY_Entier	MatDang;
	XDY_Entier	HomEscota;
	XDY_Entier	VehEscota;
	XDY_Entier	VLeger;
	XDY_Entier	PLourd;
	XDY_Entier	Moto;
	XDY_Entier	Car;
	XDY_Entier	Remorque;
	XDY_Entier	BlesseLeg;
	XDY_Entier	BlesseGra;
	XDY_Entier	Mort;
	XDY_Texte_SAT	CodeMatDang;

	XDY_Texte_SAT	Remarque;

	XDY_Texte_SAT	TexteComplementaire;

} XDY_FIC_ACC;
 

typedef struct {

	XDY_Entier	Heure;
	XDY_Entier	Minute;
	XDY_Entier	Jour;
	XDY_Entier	Mois;
	XDY_Texte_SAT	Autoroute;
	XDY_Entier	Sens;
	float		PR;
	XDY_Entier	District;

	XDY_Texte_SAT	TypFic;
	XDY_Entier	Dest1;
	XDY_Entier	Dest2;
	XDY_Entier	Dest3;
	XDY_Entier	Dest4;
	XDY_Entier	Dest5;
	XDY_Entier	Dest6;
	XDY_Entier	Dest7;
	XDY_Entier	Dest8;


        XDY_Entier	NumEvt;

	XDY_Texte_SAT	DateSaisie;
	XDY_Texte_SAT	DateAlerte;
	XDY_Entier	RefAutoroute;
        XDY_Entier	RefSens;
        XDY_Entier	TypDifficulte;
	XDY_Entier	EchSortie;
	XDY_Entier	EchEntree;
	XDY_Entier	Barriere;
	float		PR_Evt;
        
        XDY_Entier	Protection;
        XDY_Voie	ConfVoie;
        float		PR_Tete;

	XDY_Texte_SAT	DateRetNormale;

	XDY_Texte_SAT	Remarque;

	XDY_Texte_SAT	TexteComplementaire; 

} XDY_FIC_TRA;

	 
typedef struct {

	XDY_Entier	Heure;
	XDY_Entier	Minute;
	XDY_Entier	Jour;
	XDY_Entier	Mois;
	XDY_Texte_SAT	Autoroute;
	XDY_Entier	Sens;
	float		PR;
	XDY_Entier	District;

	XDY_Texte_SAT	TypFic;
	XDY_Entier	Dest1;
	XDY_Entier	Dest2;
	XDY_Entier	Dest3;
	XDY_Entier	Dest4;
	XDY_Entier	Dest5;
	XDY_Entier	Dest6;
	XDY_Entier	Dest7;
	XDY_Entier	Dest8;

	XDY_Texte_SAT	EtatTravaux;
 
	XDY_Texte_SAT	DateSaisie;
	XDY_Entier	RefAutoroute;
        XDY_Entier	RefSens;
        XDY_Entier	TypTravaux;
	XDY_Entier	TravauxNeufs;
	XDY_Entier	TravauxEntretien;
	float		PR_Debut;
	float		PR_Fin;
                        
        XDY_Entier	TravauxEnCours;
        XDY_Entier	FinTravaux;
        XDY_Entier	ChantierContinu;
	XDY_Entier	ChantierMobile;
	XDY_Entier	Basculement;

        XDY_Voie	BAU_I;
	XDY_Voie	VL_I;
	XDY_Voie	VM1_I;
	XDY_Voie	VM2_I;
	XDY_Voie	VR_I;
	XDY_Voie	TPC;
	XDY_Voie	VR;
	XDY_Voie	VM2;
	XDY_Voie	VM1;
	XDY_Voie	VL;
	XDY_Voie	BAU;
	XDY_Voie	ConfVoie;

	XDY_Texte_SAT	DateDebut;
	XDY_Texte_SAT	DateFin;

	XDY_Texte_SAT	TexteComplementaire; 

} XDY_FIC_TVX;

typedef struct {

	XDY_Entier	Heure;
	XDY_Entier	Minute;
	XDY_Entier	Jour;
	XDY_Entier	Mois;
	XDY_Texte_SAT	Autoroute;
	XDY_Entier	Sens;
	float		PR;
	XDY_Entier	District;

	XDY_Texte_SAT	TypFic;
	XDY_Entier	Dest1;
	XDY_Entier	Dest2;
	XDY_Entier	Dest3;
	XDY_Entier	Dest4;
	XDY_Entier	Dest5;
	XDY_Entier	Dest6;
	XDY_Entier	Dest7;
	XDY_Entier	Dest8;

	XDY_Entier	NumEvt;
	XDY_Texte_SAT	DateSaisie;
	XDY_Texte_SAT	DateAlerte;
	XDY_Entier	RefAutoroute;
        XDY_Entier	RefSens;
	float		PR_Fin;
                        
        XDY_Voie	ConfVoie;
        float		PR_TeteBouchon;
        XDY_Entier	TypPerturbation;
        XDY_Entier	FinPerturbation;
        XDY_Entier	Visibilite;

	XDY_Texte_SAT	DateRetNormale;

	XDY_Texte_SAT	Remarque;
	XDY_Texte_SAT	TexteComplementaire;

} XDY_FIC_MTO;


typedef struct {

	XDY_Entier	Heure;
	XDY_Entier	Minute;
	XDY_Entier	Jour;
	XDY_Entier	Mois;
	XDY_Texte_SAT	Autoroute;
	XDY_Entier	Sens;
	float		PR;
	XDY_Entier	District;

	XDY_Texte_SAT	TypFic;
	XDY_Entier	Dest1;
	XDY_Entier	Dest2;
	XDY_Entier	Dest3;
	XDY_Entier	Dest4;
	XDY_Entier	Dest5;
	XDY_Entier	Dest6;
	XDY_Entier	Dest7;
	XDY_Entier	Dest8;

	XDY_Texte_SAT	DateSaisie;
	XDY_Texte_SAT	DateAlerte;

	XDY_Entier	RefAutoroute;
	XDY_Entier	RefSens;
	float		PR_Evt;

	XDY_Entier	TypAffichage;
	XDY_Entier	CodeGare;
	XDY_Texte_SAT	NomGare; 
	XDY_Entier	CodeAutorite;
	XDY_Entier	FinMessage;

	XDY_Entier	CodeLienEvt;
	XDY_Entier	CodeTypEvt;
	XDY_Entier	CodeRefEvt;

	XDY_Texte_SAT	TexteLign1; 
	XDY_Texte_SAT	TexteLign2; 
	XDY_Texte_SAT	TexteLign3; 
	XDY_Texte_SAT	TexteLign4; 

} XDY_FIC_PMV;

typedef struct {

	XDY_Entier	Heure;
	XDY_Entier	Minute;
	XDY_Entier	Jour;
	XDY_Entier	Mois;
	XDY_Texte_SAT	Autoroute;
	XDY_Entier	Sens;
	float		PR;
	XDY_Entier	District;

	XDY_Texte_SAT	TypFic;
	XDY_Entier	Dest1;
	XDY_Entier	Dest2;
	XDY_Entier	Dest3;
	XDY_Entier	Dest4;
	XDY_Entier	Dest5;
	XDY_Entier	Dest6;
	XDY_Entier	Dest7;
	XDY_Entier	Dest8;

	XDY_Entier      NumEvt;
	XDY_Texte_SAT	DateSaisie;
	XDY_Texte_SAT	DateAlerte;
	XDY_Texte_SAT	DateRetNormale;

	XDY_Entier	RefAutoroute;
        XDY_Entier	RefSens;
        XDY_Entier	FinFermeture;
   	float		PR_Evt;

	XDY_Entier	CodeFermeture;

	XDY_Texte_SAT	Remarque; 
	XDY_Texte_SAT	TexteComplementaire; 
	XDY_Texte_SAT	NomEchangeBarriere; 
	XDY_Texte_SAT	CodeEchangeBarriere; 
	XDY_Texte_SAT	RefEvtLie; 

} XDY_FIC_FER;

typedef struct {
	XDY_Entier      Heure;
	XDY_Entier      Minute;
	XDY_Entier      Jour;
	XDY_Entier      Mois;
	XDY_Texte_SAT   Autoroute;
	XDY_Entier      Sens;
	float          PR;
	XDY_Entier      District;

	XDY_Texte_SAT   TypFic;
	XDY_Entier      Dest1;
	XDY_Entier      Dest2;
	XDY_Entier      Dest3;
	XDY_Entier      Dest4;
	XDY_Entier      Dest5;
	XDY_Entier      Dest6;
	XDY_Entier      Dest7;
	XDY_Entier      Dest8;
} XDY_FIC_DEB;


typedef XDY_Mot XDY_TDP;


typedef struct {
	XDY_Entier	Numero;
	XDY_Mot		Site;
	XDY_Nom		Libelle;
	XDY_Autoroute	Autoroute;
	XDY_NomEqt	NomAutoroute;
	XDY_Eqt		Maitre;
	XDY_PR		PR_Debut;
	XDY_PR		PR_Fin;
	XDY_Sens	Sens;
	XDY_TDP		TDP_Normal;
	XDY_TDP		TDP_Max;
	XDY_Entier	Seuil_Bas;
	XDY_Entier	Seuil_Haut;
	XDY_Booleen	Type_Zone;
	XDY_TDP         TDP_Courant;
	XDY_Octet	Franchissement_Seuil;
	XDY_Mot		Code_Zone;
	XDY_Entier	ZDP_Section;
	XDY_Octet	Traitee;
	XDY_TDP		TDP_Surcharge;
	XDY_Entier	Numero_Surcharge;
	XDY_Octet	Cle_Surcharge;
} XDY_ZDP;


typedef struct {
	XDY_Entier	Numero;
	char		Libelle_TFM[60];
	XDY_Nom		Libelle_PMV;
	XDY_Autoroute	Autoroute;
	XDY_PR		PR;
	XDY_Sens	Sens;
	XDY_Octet	Type_Localisation;
} XDY_Destination;

typedef struct {
	XDY_Entier      Numero;
	char            Libelle_TFM[60];
	XDY_Nom         Libelle_PMV;
	XDY_Autoroute   Autoroute;
	XDY_PR          PR;
	XDY_Sens        Sens;
	XDY_Octet       Type_Localisation;
	XDY_Mot		Site;
} XDY_Destination_SAD;

typedef struct {
	XDY_Horodate	Horodate;		/* Horodate des mesures */
	XDY_Eqt		Numero;			/* Numero de ZDP */
	XDY_Booleen	Dispo;			/* Disponibilit� (XDC_VRAI ou XDC_FAUX) */
	XDY_TDP		Temps;			/* Temps de parcours */
} XDY_EMes_TdP_6mn;				/* Mesure Temps de parcours 6 minutes */

typedef struct {
	XDY_Horodate	Horodate;		/* Horodate des mesures */
	XDY_Eqt		Numero;			/* Numero de ZDP */
	XDY_Booleen	Dispo;			/* Disponibilit� (XDC_VRAI ou XDC_FAUX) */
	XDY_TDP		Temps;			/* Temps de parcours moyen horaire */
	XDY_TDP		NbApp;			/* Nombre d'appariement */
} XDY_EMes_TdP_1h;				/* Mesure Temps de parcours horaire */

typedef XDY_Entier XDY_Evt_Commu;
typedef XDY_Entier XDY_Tendance_Commu;
typedef XDY_Entier XDY_Synthese_Commu;

#ifdef _TIBCO_EMS
/*surcharge des types smartsockets*/
#include <tibems/tibems.h>
#include <tibems/emsadmin.h>
#ifdef _RABBITMQ
#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/tcp_socket.h>
typedef amqp_envelope_t	T_IPC_MSG;
#else
typedef tibemsMsg	T_IPC_MSG;
#endif
typedef struct {
        T_IPC_MSG msg;
} T_IPC_CONN_PROCESS_CB_DATA_STRUCT, * T_IPC_CONN_PROCESS_CB_DATA ;


#endif
/***************** FIN DES DEFINITIONS DES TYPES EXPORTES  ****************/


extern	char	*XDY_Supprimer_Espaces_De_Fin (char *pa_chaine);

extern	char	*XDY_Copie_Sans_Espaces_De_Fin (char *pa_destination, char *pa_source);

extern	char	*XDY_Supprimer_Espaces (char *pa_chaine);

extern	char	*XDY_Copie_Sans_Espaces (char *pa_destination, char *pa_source);

#endif
