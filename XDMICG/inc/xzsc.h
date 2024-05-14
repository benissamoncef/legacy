/* Fichier : $Id: xzsc.h,v 1.18 2020/11/03 18:04:42 pc2dpdy Exp $  	Release : $Revision: 1.18 $        Date : $Date: 2020/11/03 18:04:42 $
 ******************************************************
 * STERIA *  PROJET MIGRAZUR
 ******************************************************
 * MODULE XZSCFINI * FICHIER xzsc.h
 ******************************************************
 * DESCRIPTION DU MODULE :
 *
 *   Description du module d'interface XZSCFINI
 *        - Declarations externes des fonctions
 *        - Declarations externes des variables
 *        - Declarations des constantes
 *        - Declarations des types
 *
 *   Reference DCG-021 chapitre 3.7
 *
 ******************************************************
 * HISTORIQUE :
 *
 *  T.Milleville	24 Aou 1994      : Creation
 *
 *  T.Milleville	11 Jan 1995      : Ajout du XZSCT_TypeFichier V1.5
 *  T.Milleville	11 Jan 1995      : Suppression du XZSCT_TypeFichier V1.6
 *  Niepceron    06 Jun 1995	 : XZSCT_Datagrpup fait reference a XDY_Datagroup v1.7
 ***
 * Niepceron	22 Aou 1996	: Ajout du type de machine PC simplie v1.8
 * Guilhou	23 aut 1996	: les types de machine sont en fait dans xdc.h v1.9
 * Niepceron	16 Dec 1997	: Ajout du type de machine PC niv2 avec et sans LT v1.11 (DEM/1533)
 * Niepceron    22 Dec 1997	: Ajout du tupe de machine STA (poste d'analyse) v1.12
 * Niepceron    01 Jul &998	: Ajout du type de machine Historique 1.13 (dem/1697)
 * JMG   31/10/07:ajout XZSC_22 1.14 ajout site de gestion DEM715
 * JPL   08/12/10 : Migration architecture HP ia64 (DEM 961) : declaration de XZSC_InitDescNoyau  1.15
 * JPL   08/12/10 : Migration architecture HP ia64 (DEM 961) : declaration de XZSC11 et XZSC15 a XZSC21  1.16
 * RGR   26/03/19 : DEM1331 Ajout service keep Alive     1.17
 * GGY  22/03/23  : Ajout fonction XZSC_AdresseIP, qui recupere l'adresse locale (10.xxx.xxx.xxx)
 *******************************************************/
#ifndef xzsc
#define xzsc

#include "xdy.h"
#include <rtworks/ipc.h>

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZSCC_NOM_INV 1
#define XZSCC_NOT_INV 2
#define XZSCC_FIC_INV 3
#define XZSCC_INI_INV 4
#define XZSCC_RTW_INV 5
#define XZSCC_SYS_INV 6

#define XZSCC_AUTORISER 1
#define XZSCC_INTERDIRE 2

#define XZSCC_NivTrace  	(0xffffffff)

#define XZSCC_TYPEM_INV   XDC_TYPEM_INV
#define XZSCC_TYPEM_ST		XDC_TYPEM_ST
#define XZSCC_TYPEM_SC		XDC_TYPEM_SC
#define XZSCC_TYPEM_SD		XDC_TYPEM_SD
#define XZSCC_TYPEM_LT		XDC_TYPEM_LT		
#define XZSCC_TYPEM_SP		XDC_TYPEM_SP	
#define XZSCC_TYPEM_PCS		XDC_TYPEM_PCS
#define XZSCC_TYPEM_PC2		XDC_TYPEM_PC2
#define XZSCC_TYPEM_PC2LT	XDC_TYPEM_PC2LT
#define XZSCC_TYPEM_STA		XDC_TYPEM_STA
#define XZSCC_TYPEM_HIS		XDC_TYPEM_HIS

#define XZSCC_ESCLAVE   0
#define XZSCC_MAITRE    1

#define XZSCC_TYPEP_NUL		0	
#define XZSCC_TYPEP_ADM		1	
#define XZSCC_TYPEP_GIT		2	
#define XZSCC_TYPEP_OTR		3	

/* definitions de types exportes */

typedef char XZSCT_NomMachine[20];
typedef int  XZSCT_TypeMachine;
typedef char XZSCT_NomTache[20];
typedef char XZSCT_NumVersion[20];
/*typedef char XZSCT_Datagroup[40];*/
typedef XDY_Datagroup XZSCT_Datagroup;
typedef char XZSCT_NomSSysteme[20];
typedef char XZSCT_NomPeriph[20];
typedef char XZSCT_NomInterface[20];
typedef char XZSCT_NomEntite[20];
/*typedef char XZSCT_NomSite[20];*/
typedef XDY_NomSite XZSCT_NomSite;
typedef int  XZSCT_TypeProfil; 


/* declaration des donnees globales */

/* declaration des fonctions externes */

/* Cette fonction est appelee directement par plusieurs modules d'initialisation */
extern int XZSC_InitDescNoyau (void);

/******************************************************
 * SEQUENCE D'APPEL :
 * XZSC_01InitCnxSup(NomTache, AutoriserTime,
 *		    AutoriserTrace, AutoriserSynchro,
 *		    FonctionUtil)*
 * Arguments
 * NomTache	  : nom de la tache provenant du "main"
 * 		de la tache.
 * AutoriserTime	  : XZSCC_AUTORISER ou XZSCC_INTERDIR
 *		(XZSCC_INTERDIR pour tache temporaire).
 * AutoriserTrace  : XZSCC_AUTORISER ou XZSCC_INTERDIR.
 * AutoriserSynchro: XZSCC_AUTORISER ou XZSCC_INTERDIR
 *		(XZSCC_INTERDIR pour tache temporaire).
 * FonctionUtil	  : pointeur sur la fonction utilisateur
 *		de sortie de la tache.
 *
 * Codes retour
 * XZSCC_NOM_INV		Nom passe en argument egal a NULL.
 * XZSCC_INI_INV 	Initialisation deja effectuee.
 * XZSCC_FIC_INV 	Erreur lecture fichier de
 *			configuration du noyau.
 * XZSCC_OK		ok
 *
 * Mecanismes
 * Rtworks : RTS3 , RTS9 et RTS10
 * Systeme : SYS8
 ******************************************************/

extern int XZSC_01InitCnxSup(char *NomApplication,
    XZSCT_NomTache NomTache,
    int AutoriserTime,
    int AutoriserTrace,
    int AutoriserSynchro,
    void (*FonctionUtil)(),
    char *DestiStd);


/******************************************************
 * SEQUENCE D'APPEL :
 * XZSC_02Priorite(Priorite)
 *
 * Arguments
 * Priorite :	Priorite de la machine (significatif
 *		pour un SD ou un SC).
 *
 * Codes retour
 * XZSCC_INI_INV	Initialisation non effectuee
 *		(XZSCV_InitOk = 0).
 * XZSCC_OK	ok
 *
 * Mecanismes
 ******************************************************/

extern int XZSC_02Priorite(int *Priorite);


/******************************************************
 * SEQUENCE D'APPEL :
 * XZSC_03PeriodeTestReseau(Periode)
 *
 * Arguments
 * Periode :	Periode des tests du reseau pour une
 *		machine de type SD ou SC.
 *
 * Codes retour
 * XZSCC_INI_INV	Initialisation non effectuee
 *		(XZSCV_InitOk = 0).
 * XZSCC_OK	ok
 *
 * Mecanismes
 ******************************************************/

extern int XZSC_03PeriodeTestReseau(int *Periode);


/******************************************************
 * SEQUENCE D'APPEL :
 * XZSC_04NomSite(NomSite)
 *
 * Arguments
 * NomSite :	Nom du site de la machine.
 *
 * Codes retour
 * XZSCC_INI_INV 	Initialisation non effectuee
 *			(XZSCV_InitOk = 0).
 * XZSCC_OK		ok
 *
 * Mecanismes
 ******************************************************/

extern int XZSC_04NomSite(XZSCT_NomSite NomSite);


/******************************************************
 * SEQUENCE D'APPEL :
 * XZSC_05TypeMachine(TypeMachine)
 *
 * Arguments
 * TypeMachine :	Type de la machine XZSCC_TYPEM_ST,
 *		XZSCC_TYPEM_SD ou XZSCC_TYPEM_SC.
 *
 * Codes retour
 * XZSCC_INI_INV		Initialisation non effectuee
 *			(XZSCV_InitOk = 0).
 * XZSCC_OK 		ok
 *
 * Mecanismes
 ******************************************************/

extern int XZSC_05TypeMachine(int *TypeMachine);



/******************************************************
 * SEQUENCE D'APPEL :
 * XZSC_06NomTache(NomTache)
 *
 * Arguments
 * NomTache : Nom de la tache.
 *
 * Codes retour
 * XZSCC_INI_INV	Initialisation non effectuee
 *		(XZSCV_InitOk = 0).
 * XZSCC_OK	ok
 *
 * Mecanismes
 ******************************************************/

extern int XZSC_06NomTache(XZSCT_NomTache NomTache);



/******************************************************
 * SEQUENCE D'APPEL :
 * XZSC_07NomMachine(NomMachine)
 *
 * Arguments
 * NomMachine : Nom de la machine.
 *
 * Codes retour
 * XZSCC_INI_INV	Initialisation non effectuee
 *		(XZSCV_InitOk = 0).
 * XZSCC_OK	ok
 *
 * Mecanismes
 ******************************************************/

extern int XZSC_07NomMachine(XZSCT_NomMachine NomMachine);


/******************************************************
 * SEQUENCE D'APPEL :
 * XZSC_08NomPeriphSR(NomPeriph)
 *
 * Arguments
 * NomPeriph : 	Nom du peripherique de sauvegarde /
 *		restauration.
 *
 * Codes retour
 * XZSCC_INI_INV	Initialisation non effectuee
 *		(XZSCV_InitOk = 0).
 * XZSCC_OK	 ok
 *
 * Mecanismes
 * ******************************************************/

extern int XZSC_08NomPeriphSR(XZSCT_NomPeriph NomPeriph);



/******************************************************
 * SEQUENCE D'APPEL :
 * XZSC_13InfoFicTrace(PathTrace, TailleFic,
 *		MaxTailleFic)
 *
 * Arguments
 * PathTrace :	 Chemin du repertoire des fichiers
 *		 traces.
 * TailleFic :	 Taille du fichier trace.
 * MaxTailleFic : Taille maximale des fichiers traces
 *		 de la machine.
 *
 * Codes retour
 * XZSCC_INI_INV	Initialisation non effectuee
 *		(XZSCV_InitOk = 0).
 * XZSCC_OK	ok
 *
 * Mecanismes
 * ******************************************************/

extern int  XZSC_13InfoFicTrace(char *PathTrace, 
    int *TailleFic,
    int *MaxTailleFic);


extern int XZSC_11NomImprimante(XZSCT_NomPeriph NomImprimante);


extern int XZSC_15NomImprimanteSecours(
    XZSCT_NomPeriph NomImprimanteSecours);


extern int XZSC_16ProfilMachine (XZSCT_TypeProfil *TypeProfil);


extern int XZSC_17DureeAbsenceCourte(int *Duree);


extern int XZSC_20TestConnexion (void);


extern int XZSC_21NomMachineRTserver (XZSCT_NomMachine va_NomMachine);


extern XDY_District XZSC_22NumeroSiteLocal();


extern char *XZSC23_Nom_From_Numero(XDY_District );

extern int XZSC_30ProgKeepAlive(XZSCT_NomSite va_NomSite);
extern int XZSC_30ProgKeepAliveHIS(XZSCT_NomSite va_NomSite);

extern int XZSC_31AbtKeepAlive(XZSCT_NomSite va_NomSite);

static void  XZSC_process_KEEPALIVE_cb(T_IPC_CONN                 va_cnx,
                                       T_IPC_CONN_PROCESS_CB_DATA pa_data,
                                       T_CB_ARG                   pa_arg);


/******************************************************
 * STERIA *  PROJET MIGRAZUR
 ******************************************************
 ******************************************************
 * ALGORITHME :
 *
 * Arguments
 *
 * Fonction
 * Recuperation de l'adresse IP locale
 *
 * Codes retour
 * XDC_NOK	erreur
 * XDC_OK	ok
 *
 * Mecanismes
 ********************************************************/
int XZSC_AdresseIP (XDY_Texte va_IP);

#endif
