/*E*/
/*Fichier :  $Id: eini_don.h,v 1.25 2020/11/16 14:16:17 devgtie Exp $      Release : $Revision: 1.25 $        Date : $Date: 2020/11/16 14:16:17 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEINI * FICHIER eini_don.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Ce module gere les structures de donnees de la tache TEINI.
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	31 Aug 1994	: Creation
* Mismer.D	version 1.2	14 Dec 1994	:
* Mismer.D	version 1.3	25 Jan 1995	:
* Mismer.D	version 1.4	09 Fev 1995	:
* Mismer.D	version 1.5	14 Fev 1995	:
* Fontaine.C	version 1.6	19 Mai 1995	: Reprise entete, historique
* Mismer.D	version 1.7	30 Mai 1995	: Modif nom des equipemnts pour fichier LCR
* Mismer.D	version 1.8	16 Jun 1995	: Ajout du type machine dans la structure EINI_MACHINE
* Mismer.D	version 1.8	16 Jun 1995	: Ajout du nom machine suppleante dans la structure EINI_MACHINE
* Mismer.D	version 1.10	16 Jun 1995	: Rien
* Mismer.D	version 1.10	16 Jun 1995	: Ajout de l'alarme dans la structure EINI_DONNEES_FEN_LCR
* Mismer.D	version 1.11	05 Sep 1996	: Ajout variable globale vg_Type_Machine (RADT_W)
* Mismer.D	version 1.12	23 Oct 1996	: Modif pour Alerte PAU HS (DEM/1282)
* Mismer.D	version 1.13	07 Nov 1996	: Modif evolution METEO (DEM/1232)
* Mismer.D	version 1.14	10 Fev 1997	: Modif TDP (DEM/1395)
* Mismer.D	version 1.15	15 Jan 1998	: Modif PAL (DEM/)
* JMG		version 1.? 	11/10/05	: ajout PMVA 
* JMG   31/10/07        : ajout site de gestion 1.17
* JBL	01/12/07	: ajout GTC renovee (664)  1.18
* VR	19/12/11		: ajout PRV (DEM/1016) 1.19
* JPL	15/02/12	: Correction taille table des lignes serie d'une machine (DEM 1018)  1.20
* ABE	27-04-2015 : projet suppression ancienne GTC (GTCA)
* LCL	16/04/18	: Controleurs de feux CTRL_FEUX 1.22 DEM1284
* PNI	12/02/19	: Suppression du fichier SYSVIDEO 1.23
* CGR	29/07/20	: Ajout IMU 1.24 DEM-SAE155
* PNI	16/11/20	: Passge de EINI_MAX_LIGNE_PAR_EQT a  43  SAE_190 1.25
* LCL	21/11/21	: Correction generation Config_Lignes pour l'IMU DEM-473
* ABK	30/08/23	: Ajout de l'entrée PIC dans la tableau pg_TabConf (DEM/483)
------------------------------------------------------ */

#ifndef eini_don
#define eini_don

/* fichiers inclus */

#include <xdc.h>
#include <xdy.h>
#include <xdf.h>
#include <rtworks/common.h>
#include <rtworks/ipc.h>

#include <xzst.h>
#include <xzsm.h>
#include <xzsa.h>
#include <xzex.h>
#include <xzec.h>
#include <xzao.h>
#include <ex_mdon.h>

/* definitions de constantes exportees */

#define EINI_TYPE_CONF		1
#define EINI_PAS_TYPE_CONF	2

#define EINI_OUVERTURE_FIC	0
#define EINI_FERMETURE_FIC	1
#define EINI_DETRUIRE_FIC	2

#define EINI_NOM_BASE		XDC_BASE_CFG

#define EINI_MAX_LIGNE_PAR_EQT	43	/*les 2 derniers sont pour IP maitre et esclave*/

/* definitions de types exportes */

typedef struct
{
	char	NumeroST[251];	/* Identification du serveur de terminaux (adresse) */
	char	NumLigne[16];	/* Identification de la ligne */
	char	Protocole;	/* Type de protocole ( L, A, O, S ) */
	char 	Sens;		/* Mode de la liaison  ( M : maitre, E : esclave ) */

} EINI_LIGNE ;

typedef struct
{
	EINI_LIGNE	Ligne[EINI_MAX_LIGNE_PAR_EQT];

} EINI_EQT_LIGNE ;

typedef struct
{
	char		Equipement[4];
	char		Vitesse[6];
	int		NbEssais;
	int		NbCarPref;
	int		NbCarSuff;

} EINI_PARAM_LIGNE ;

typedef struct
{
	XDY_Nom		NomEqt;
	XDY_Eqt		NumEqt;
	XDY_Eqt		TypeEqt;
	XDY_NomEqt	NomLT;
	XDY_Mot		Abonne;
	XDY_Mot		Alarme;
        XDY_Mot		NumeroAutoroute;
        XDY_PR          PR;
        XDY_Mot		Sens;
	XDY_District    SiteGestion;
	void *		Suivant;

} EINI_DONNEES_FEN_LCR ;


typedef struct
{
	int 		TypeEqt;
	int		RangEqt;
	int		TypeFichier1;
	char		NomFichier1[32];
	int		TypeFichier2;
	char		NomFichier2[32];
	int		TypeFichier3;
	char		NomFichier3[32];
	int		TypeFichier4;
	char		NomFichier4[32];
	int		TypeFichier5;
	char		NomFichier5[32];
	int		TypeFichier6;
	char		NomFichier6[32];
}	EINI_TABLE_CONFIG;

/* definition de macro exportees */


/* declarations de donnees globales */
static  EINI_TABLE_CONFIG pg_TabConf[] =
   {
 	{ XZECC_TYPE_EQT_RAU, 0,	EINI_TYPE_CONF, 	XDF_Config_RAU,
 					0, "", 0, "", 0,"", 0,"", 0,"" },
 	{ XZECC_TYPE_EQT_TEL, 1,	EINI_TYPE_CONF, 	XDF_Config_AUTOTEL,
 					0, "", 0, "", 0,"", 0,"", 0,"" },
 	{ XZECC_TYPE_EQT_OPE, 2,	EINI_TYPE_CONF, 	XDF_Config_OPER, 
 					0, "", 0, "", 0,"", 0,"", 0,"" },
 	{ XZECC_TYPE_EQT_VID, 3,	
 					0,"",0,"",0, "",0,"", 0,"", 0,"" },
					
 	{ XZECC_TYPE_EQT_NIC, 4,	EINI_TYPE_CONF,		XDF_Sequence_GTCNiceRenovee,/* 3 */
								EINI_TYPE_CONF,		"Equipement_GTCNiceRenovee_TOUS", /*4*/
								EINI_TYPE_CONF,		XDF_Config_GTCNiceRenovee,
								EINI_TYPE_CONF,		XDF_Sequence_GTCNiceRenovee,/* 3 */
								EINI_TYPE_CONF,		"Equipement_GTCNiceRenovee_TOUS", /*4*/
								EINI_TYPE_CONF,		XDF_Config_GTCNiceRenovee},
								
 	{ XZECC_TYPE_EQT_SAT, 5,	EINI_TYPE_CONF, 	XDF_Config_SATIR,
 					0, "", 0, "",0,"", 0,"", 0,"" },
 	{ XZECC_TYPE_EQT_RDT, 6,	EINI_TYPE_CONF, 	XDF_Config_RADT,
 			 		EINI_TYPE_CONF, 	XDF_Config_Point_Mesure, 
					EINI_TYPE_CONF,		XDF_Config_Params_TERDT,
 					0,"", 0,"", 0,"" },
 	{ XZECC_TYPE_EQT_DAI, 7,	EINI_TYPE_CONF, 	XDF_Config_DAI, 	
 					EINI_TYPE_CONF, 	XDF_Config_Analyseur,   
 					0, "",0,"", 0,"", 0,"" },
	{ XZECC_TYPE_EQT_PMV, 8,	EINI_TYPE_CONF, 	XDF_Config_PMV, 	
 					0, "", 0, "",0,"", 0,"", 0,"" },
 	{ XZECC_TYPE_EQT_PICTO, 8, 	EINI_TYPE_CONF, 	XDF_Config_PICTO, 	
 					0, "", 0, "",0,"", 0,"", 0,"" },
 	{ XZECC_TYPE_EQT_NAV, 9,	EINI_TYPE_CONF, 	XDF_Config_NAV, 	
 					0, "", 0, "",0,"", 0,"", 0,"" },
 	{ XZECC_TYPE_EQT_GTC, 10,	EINI_TYPE_CONF, 	XDF_Config_GTC, 	
 					0, "", 0, "",0,"", 0,"", 0,"" },
 	{ XDC_EQT_GEN_LCR,    11,	EINI_TYPE_CONF, 	XDF_Config_EQT_GEN_LCR,
 					0, "", 0, "",0,"", 0,"", 0,"" },
 	{ XDC_EQT_TDP,        12,	EINI_TYPE_CONF, 	XDF_Config_TDP,
 					0, "", 0, "",0,"", 0,"", 0,"" },
 	{ XZECC_TYPE_EQT_PAL, 13,	EINI_TYPE_CONF, 	XDF_Config_PAL, 	
 					0, "", 0, "",0,"", 0,"", 0,"" },
	{ XZECC_TYPE_EQT_PMVA, 14,	EINI_TYPE_CONF,         XDF_Config_PMVA,
					0, "", 0, "",0,"", 0,"", 0,"" },
 	{ XZECC_TYPE_EQT_BAF, 15,	EINI_TYPE_CONF, 	XDF_Config_BAF, 	
 					0, "", 0, "",0,"", 0,"", 0,"" },
 	{ XZECC_TYPE_EQT_BAD, 16,	EINI_TYPE_CONF, 	XDF_Config_BAD, 	
 					0, "", 0, "",0,"", 0,"", 0,"" },
	{ XZECC_TYPE_EQT_PRV, 17,	EINI_TYPE_CONF, 	XDF_Config_PRV, 	
 					0, "", 0, "",0,"", 0,"", 0,"" },
	{ XZECC_TYPE_EQT_CFE, 18,	EINI_TYPE_CONF, 	XDF_Config_CFE, 	
 					0, "", 0, "",0,"", 0,"", 0,"" },
	{ XZECC_TYPE_EQT_CAP, 19,	EINI_TYPE_CONF, 	XDF_Config_CAP, 	
					0, "", 0, "",0,"", 0,"", 0,"" },
	{ XZECC_TYPE_EQT_IMU, 20,	EINI_TYPE_CONF, 	XDF_Config_IMU, 	
					0, "", 0, "",0,"", 0,"", 0,"" },
	{ XZECC_TYPE_EQT_PIC, 21,	EINI_TYPE_CONF, 	XDF_Config_PIC, 	
					0, "", 0, "",0,"", 0,"", 0,"" },
 	{ 0, 0,				EINI_PAS_TYPE_CONF,    XDF_Config_Lignes, 	
 					EINI_PAS_TYPE_CONF,    XDF_Presence_Equip,
 					0, "",0,"", 0,"", 0,"" }
   };
   
static int 	pg_SizeTabConf = sizeof(pg_TabConf)/sizeof(EINI_TABLE_CONFIG);

typedef struct
{
	XDY_NomMachine		NomMachine;
	XDY_NomMachine		NomMachineSup;
	int			TypeMachine;
	char			NumeroSTA[16];
	char			NumeroSTB[16];
	XZEXT_ENG_CONF_LIGNE	LigneSerie[XZECC_NB_LIGNE_SERIE +1];
	XZEXT_ENG_PRESENCE_EQT	Eqt[(sizeof(pg_TabConf)/sizeof(EINI_TABLE_CONFIG)) +1];
	EINI_EQT_LIGNE		LigneEqt[(sizeof(pg_TabConf)/sizeof(EINI_TABLE_CONFIG)) +1];
	void *			Suivant;

} EINI_MACHINE ;



/* delaration de fonctions externes */

extern	int		vg_Type_Machine;
extern  XDY_NomAuto	vg_NomAutoroute[350];

#define EINIC_NB_AUTOROUTE	sizeof(vg_NomAutoroute)/sizeof(XDY_NomAuto)

#endif
