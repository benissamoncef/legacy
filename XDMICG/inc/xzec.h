/*E*/
/* Fichier : $Id: xzec.h,v 1.40 2020/11/03 18:04:39 pc2dpdy Exp $     Release : $Revision: 1.40 $      Date :  $Date: 2020/11/03 18:04:39 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE XZEC  * FICHIER xzec.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Structure des donnees du fichier xzec.h >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	24 Aug 1994	: Creation
* Mismer.D	version 1.2	28 Sep 1994	:
* Mismer.D	version 1.3	10 Oct 1994	: Ajout de XZEC02
* Nagiel.E	version 1.4	10 Oct 1994	: 
* Nagiel.E	version 1.5	10 Oct 1994	:
* Mismer.D	version 1.6	12 Oct 1994	: Ajout des constante pour le protocole AUTOTEL
* Nagiel.E	version 1.7	19 Oct 1994	: Ajout de XZECC_TYPE_EQT_ANA
* Nagiel.E	version 1.8	19 Oct 1994	:
* Mismer.D	version 1.9	21 Oct 1994	: Ajout des Bouchons XZEC20 XZEC21 XZEC22 XZEC23 XZEC24
* Nagiel.E	version 1.10	02 Nov 1994	: Ajout de XZECC_TYPE_EQT_MON XZECC_TYPE_EQT_CAM
* Flaidar.A	version 1.11	08 Nov 1994	:
* Flaidar.A	version 1.12	08 Nov 1994	:
* Mismer.D	version 1.13	09 Nov 1994	: Remise en etat des boulettes a Eric
* Mismer.D	version 1.14	18 Nov 1994	: Ajout XZECC_TYPE_EQT_ST
* Mismer.D	version 1.15	28 Nov 1994	: Passage des type config ligne et equipt dans xzex
* Mismer.D	version 1.16	02 Dec 1994	:
* Mismer.D	version 1.17	08 Dec 1994	: Modif nombre de ligne par machine
* Volcic.F	version 1.18	28 Dec 1994	: Ajout constantes pour verif coherence fichier seuils et DIV
* Nouaille.M	version 1.19	02 Jan 1995	: Modif equipement telephonique et operator
* Mismer.D	version 1.20	06 Jan 1995	:
* Lecrivain	version 1.21	27 Jan 1995	:
* Mismer.D	version 1.22	21 Fev 1995	:
* Mismer.D 	version 1.23	27 Fev 1995	: Ajout XZEC06_Inhibition_Eqt
* Fontaine.C	version 1.24	24 Mai 1995	: Reprise entete, historique
* Mismer.D	version 1.25	20 Sep 1995	: Ajout constante tache TETDC
* Mismer.D	version 1.26	05 Dec 1995	: Modif type SATIR
* Mismer.D	version 1.27	19 Mar 1996	: Modif modif max verif classe DIV (DEM/728)
* Mismer.D	version 1.28	07 Nov 1996	: Modif METEO (DEM/1232)
* Mismer.D	version 1.29	10 Fev 1997	: Ajout constantes TDP (DEM/1395)
* Mismer.D	version 1.30	20 Fev 1997	: Modif constantes TDP (DEM/1396)
* Mismer.D	version 1.31	25 Mar 1997	: Ajout constantes pour aff TDP sur PMV (DEM/1417)
* Mismer.D	version 1.32	15 Jan 1998	: Ajout constantes pour PAL (DEM/)
* JMG		version 1.33	13/09/05	: ajout constantes PMVA BAF BAD
* JBL		version xx.xx	05/04/09	: ajout 5e tenx
* VR		version 1.35	08/12/11 	: Ajout equipement PRV (DEM/1016)
* LCL		version 1.36	22/02/18	: Ajout equipement CTRL_FEUX DEM1284
* LCL		version 1.37	11/05/18	: Ajout equipement CAP GABEB DEM1283
* JMG		version 1.38	27/08/18	: ajout XZECC_IP_VIDE suppression SAGA DEM1306
* JMG		version 1.39	01/01/2020	: OPC
* CGR		version 1.40	30/07/20	: Ajout XZECC_NOM_IMU DEM-SAE155
------------------------------------------------------ */

#ifndef xzec
#define xzec

/* fichiers inclus */
#include	<xdy.h>
#include	<xdc.h>
#include	<xdm.h>
#include	<xdg.h>
#include	<xzst.h>
#include	<xzsc.h>
#include	"xzex.h"

/* definitions de constantes exportees */

#define	XZECC_TYPE_EQT_RAU	XDC_EQT_RAU	/* Type d'equipement : Systeme RAU */
#define	XZECC_TYPE_EQT_TEL	XDC_EQT_MAA	/* Type d'equipement : Automate telephonique */
#define	XZECC_TYPE_EQT_MAA	XDC_EQT_MAA	/* Type d'equipement : Automate telephonique */
#define	XZECC_TYPE_EQT_OPE	XDC_EQT_OPE	/* Type d'equipement : OPERATOR */
#define	XZECC_TYPE_EQT_VID	XDC_EQT_SYV	/* Type d'equipement : Systeme video */
#define	XZECC_TYPE_EQT_LTV	XDC_EQT_LTV	/* Type d'equipement : LTV */
#define	XZECC_TYPE_EQT_MAGN	XDC_EQT_MAG	/* Type d'equipement : Magneto */
#define	XZECC_TYPE_EQT_CAM	XDC_EQT_CAM	/* Type d'equipement : Camera */
#define	XZECC_TYPE_EQT_MON	XDC_EQT_MNT	/* Type d'equipement : Moniteur */
#define	XZECC_TYPE_EQT_NIC	XDC_EQT_NIC	/* Type d'equipement : GTC Nice */
#define	XZECC_TYPE_EQT_SAT	XDC_EQT_SAT	/* Type d'equipement : Systeme SATIR*/
#define	XZECC_TYPE_EQT_RDT	XDC_EQT_RAD	/* Type d'equipement : RADT */
#define	XZECC_TYPE_EQT_DAI	XDC_EQT_DAI	/* Type d'equipement : DAI */
#define	XZECC_TYPE_EQT_PMV	XDC_EQT_PMV	/* Type d'equipement : PMV */
#define	XZECC_TYPE_EQT_PICTO	XDC_EQT_PCT	/* Type d'equipement : PICTO */
#define	XZECC_TYPE_EQT_NAV	XDC_EQT_NAV	/* Type d'equipement : NAV */
#define	XZECC_TYPE_EQT_PAL	XDC_EQT_PAL	/* Type d'equipement : PAL */
#define	XZECC_TYPE_EQT_GTC	XDC_EQT_GTC	/* Type d'equipement : Mini GTC */
#define	XZECC_TYPE_EQT_ANA	XDC_EQT_ANA	/* Type d'equipement : Analyseur */
#define	XZECC_TYPE_EQT_ST	XDC_EQT_SRV	/* Type d'equipement : Serveur Terminaux */
#define XZECC_TYPE_EQT_PMVA	XDC_EQT_PMVA
#define XZECC_TYPE_EQT_PRV		XDC_EQT_PRV
#define XZECC_TYPE_EQT_PIC		XDC_EQT_PIC
#define XZECC_TYPE_EQT_BAF		XDC_EQT_BAF
#define XZECC_TYPE_EQT_BAD		XDC_EQT_BAD
#define XZECC_TYPE_EQT_CFE		XDC_EQT_CFE
#define XZECC_TYPE_EQT_CAP		XDC_EQT_CAP
#define XZECC_TYPE_EQT_IMU		XDC_EQT_IMU
#define	XZECC_TYPE_EQT_ALL	XDC_EQTALL	/* Type d'equipement : Tous */
#define	XZECC_ERR_TYPE_EQT	-1	/* Type d'equipement invalide */

#define	XZECC_TOUS_SITES	20	/* Tous les sites */
#define	XZECC_MACHINE_PRINC	22	/* Type de machine : principale */
#define	XZECC_MACHINE_SUPPL	23	/* Type de machine : suppleante */
#define	XZECC_TOUTES_MACHINES	XDC_TOUTES_MACHINES	/* Tous les machines */

#define	XZECC_EQT_MARCHE	XDC_EQT_MARCHE	/* Action sur l'equipt : mise en marche */
#define	XZECC_EQT_ARRET		XDC_EQT_ARRET	/* Action sur l'equipt : arret */
#define	XZECC_ERR_ARG		30	/* Argument invalide */
#define XZECC_TYPE_TACHE_TIM	20
#define XZECC_IP_VIDE		"xxx"


#define XZECC_PROT_LCR		'L'	/* Protocole LCR */
#define XZECC_PROT_APRIL	'A'	/* Protocole AprilNet */
#define XZECC_PROT_OPE		'O'	/* Protocole Operator */
#define XZECC_PROT_SAT		'S'	/* Protocole SATIR */
#define XZECC_PROT_TEL		'T'	/* Protocole AUTOTEL */
#define XZECC_SENS_MAITRE	'M'	/* Liaison Maitre */
#define XZECC_SENS_ESCLAVE	'E'	/* Liaison Escalve */
#define XZECC_PROT_DIASER	'D'
#define XZECC_PROT_MIGRAZUR	'Z'
#define XZECC_PROT_IP		'I'
#define XZECC_PROT_SERVEUR	'X'
#define XZECC_PROT_LCR_E	'W'
#define XZECC_PROT_OPC		'C'

#define XZECC_PROT_LCR_MAITRE	1
#define XZECC_PROT_LCR_ESCLAVE	2
#define XZECC_PROT_APRILNET	3
#define XZECC_PROT_OPERATOR	4
#define XZECC_PROT_AUTOTEL	5

#define XZECC_TACHE_LCR_MAITRE	"lm"
#define XZECC_TACHE_LCR_ESCLAVE	"le"
#define XZECC_TACHE_APRILNET	"ax0"
#define XZECC_TACHE_OPERATOR	"om"
#define XZECC_TACHE_AUTOTEL	"tm"
#define XZECC_TACHE_IP_MAITRE	"im"
#define XZECC_TACHE_IP_ESCLAVE	"ie"

#define XZECC_TACHE_RAU		"rau"
#define XZECC_TACHE_QTX		"qtx"
#define XZECC_TACHE_TDP		"tdp"
#define XZECC_TACHE_TEL		"tel"
#define XZECC_TACHE_OPE		"ope"
#define XZECC_TACHE_VID		"vid"
#define XZECC_TACHE_NIC		"nic"
#define XZECC_TACHE_SAT		"sat"
#define XZECC_TACHE_SX0		"sx0"
#define XZECC_TACHE_RDT		"rdt"
#define XZECC_TACHE_TDC		"tdc"
#define XZECC_TACHE_DAI		"dai"
#define XZECC_TACHE_PMV		"pmv"
#define XZECC_TACHE_NAV		"nav"
#define XZECC_TACHE_PAL		"pal"
#define XZECC_TACHE_GTC		"gtc"
#define XZECC_TACHE_TIM		"tim"
#define XZECC_TACHE_PMVA	"pma"
#define XZECC_TACHE_PRV		"prv"
#define XZECC_TACHE_PIC		"pic"
#define XZECC_TACHE_BAF		"baf"
#define XZECC_TACHE_BAD		"bad"
#define XZECC_TACHE_CFE		"cfe"
#define XZECC_TACHE_CAP		"cap"
#define XZECC_TACHE_IMU		"imu"

/* GTC Renovee */
#define XZECC_TACHE_NX1		"nx1"
#define XZECC_TACHE_NX2		"nx2"
#define XZECC_TACHE_NX3		"nx3"
#define XZECC_TACHE_NX4		"nx4"
#define XZECC_TACHE_NX5		"nx5"
#define XZECC_TACHE_NX6		"nx6"



#define XZECC_NOM_RAU		"RAU"
#define XZECC_NOM_QTX		"QTX"
#define XZECC_NOM_TDP		"UGTP"
#define XZECC_NOM_TEL		"TEL"
#define XZECC_NOM_OPE		"OPE"
#define XZECC_NOM_VID		"VIDEO"
#define XZECC_NOM_NIC		"NICE"
#define XZECC_NOM_SAT		"SAT"
#define XZECC_NOM_RDT		"RADT"
#define XZECC_NOM_DAI		"DAI"
#define XZECC_NOM_PMV		"PMV"
#define XZECC_NOM_PICTO		"PICTO"
#define XZECC_NOM_NAV		"NAV"
#define XZECC_NOM_PAL		"PAL"
#define XZECC_NOM_GTC		"GTC"
#define XZECC_NOM_PMVA		"PMVA"
#define XZECC_NOM_PRV		"PRV"
#define XZECC_TACHE_EQUEXT	"te"
#define XZECC_NOM_BAF		"BAF"
#define XZECC_NOM_BAD		"BAD"
#define XZECC_NOM_CFE		"CFE"
#define XZECC_NOM_CAP		"CAP"
#define XZECC_NOM_IMU		"IMU"
#define XZECC_NOM_PIC		"PIC"

#define XZECC_PORT_CONSOLE_TRANSERVER	150
#define XZECC_SU_TRANSERVER		"su\r"
#define XZECC_PSWD_TRANSERVER		"rcf\r"
#define XZECC_FIN1_TRANSERVER           "Password: "
#define XZECC_FIN2_TRANSERVER  		"] "
#define XZECC_FIN3_TRANSERVER  		"Session: voie"
#define XZECC_ADRESSE_PORT_ST		1999

#define XZECC_MAX_EQT		32
#define XZECC_NB_LIGNE_SERIE	45
#define XZECC_NUM_IP_MAITRE	41
#define XZECC_NUM_IP_ESCLAVE	42

#define XZECC_TAB_SEPAR 	"\t"		/* Tabulation */

#define XZECC_COMMENT 		"#"		/* Commentaire */

#define XZECC_FIN_STRING 	'\0'		/* Fin d'une chaine de caractere */

#define XZECC_LINE_FEED 	'\n'		/* Fin de ligne */

#define XZECC_EOF 		-1		/* Fin de fichier */


#define XZECC_DIV		1		/* Classe DIV */

#define XZECC_LONG_DIV		4		/* Max d'une Classe DIV */

#define XZECC_MAX_DIV		3500		/* Valeur Max d'une Classe DIV */


#define XZECC_SEUIL_V		2		/* Seuil de trafic V */

#define XZECC_LONG_SEUIL_V 	3		/* Max d'un Seuil de trafic V */

#define XZECC_MAX_SEUIL_V	200 		/* Valeur Max d'un Seuil de trafic V */


#define XZECC_SEUIL_Q		3		/* Seuil de trafic Q */

#define XZECC_LONG_SEUIL_Q 	5		/* Max d'un Seuil de trafic Q */

#define XZECC_MAX_SEUIL_Q	10000 		 /* Valeur Max d'un Seuil de trafic Q */


#define XZECC_SEUIL_TO		4		/* Seuil de trafic TO */

#define XZECC_LONG_SEUIL_TO 	3		/* Max d'un Seuil de trafic TO */

#define XZECC_MAX_SEUIL_TO	100 		/* Valeur Max d'un Seuil de trafic TO */


#define XZECC_COEFF		5		/* Seuil de coeff donnees glissantes */

#define XZECC_LONG_COEFF 	5		/* Max d'un coeff donnees glissantes*/

#define XZECC_MAX_COEFF		1 		/* Valeur Max d'un coeff  donnees glissantes*/



/* definitions de types exportes */
	
/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de configuration d'un ou de tous les types 
*  d'equipements externes d'une ou de toutes les machines
*  d'un ou de tous les sites
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern 	int XZEC01_Config (	XDY_NomSite	pa_NomSite,
				XDY_NomMachine	pa_NomMachine,
				int		va_TypeEquipement );		

/*
* ARGUMENTS EN ENTREE :
*   va_NomSite		nom du site ( PC district ou LT )
*   va_NomMachine	nom de la machine
*   va_TypeEquipement	type d'equipt a configurer (XZECC_TYPE_EQT_ )
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec envoi message
*   XZECC_ERR_TYPE_EQT	Type d'equipt invalide
*
* CONDITION D'UTILISATION
*   Doit etre invoquee lors d'une modification dans la configuration 
*   d'au moins un equipement. 
*
* FONCTION 
*   Construction du message XDM_ECONFIG_EQPT en specifiant le type 
*   de configuration et en fournissant les valeurs de TypeEquipement,
*   NomMachine, NomSite recues.
*   Envoi du message sur le DataGroup XDG_ECONFIG_EQPT_<NomSite>
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande de changement de contraste (surbrillance).
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern 	int XZEC02_Commande_Contraste (	int		va_Contraste,
					XDY_Eqt		va_NoEqt,
					XDY_TypeEqt	va_TypeEqt,
					XDY_NomSite	va_NomSite );	

/*
* ARGUMENTS EN ENTREE :
*   va_Contraste,	Surbrillance ou non
*   va_NoEqt		Surbrillance pour 1 eqt 
*                       soit pour tous les eqt du site avec XZEPC_SURB_EQTALL
*   va_NomSite		Nom du site
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec 
*
* CONDITION D'UTILISATION
*   Doit etre invoquee sur validation de la fenetre de pilotage PMV. 
*
* FONCTION 
*   Construction du message XDM_ECMD_Contraste en specifiant les valeurs recues.
*   Envoi du message sur le DataGroup XDG_ECMD_CONTRASTE_<NomMachine> du site concerne
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Met en ou hors service un equipement externe connecte a
*  une machine sur un site
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern 	int XZEC03_Mise_EnHorsSrv_Eqt (	XDY_NomSite	pa_NomSite,
					XDY_NomMachine	pa_NomMachine,
					int		va_TypeEquipement,
					int		va_NumEquipement,
					int		va_EnHorsService );		

/*
* ARGUMENTS EN ENTREE :
*   va_NomSite		nom du site ( PC district ou LT )
*   va_NomMachine	nom de la machine
*   va_TypeEquipement	type d'equipt a mettre En ou Hors service
*   va_NumEquipement	numero de l'equipement a mettre En ou Hors service
*   va_EnHorsService	type d'action sur l'equipement
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec envoi message
*   XZECC_ERR_ARG	argument va_EnHorsService invalide
*   XZECC_ERR_TYPE_EQT	Type d'equipt invalide
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   Construction du message XDM_ECMD_EQPT en specifiant le type 
*   d'action et en fournissant les valeurs des arguments recus.
*   Envoi du message sur le DataGroup XDG_ECMD_<TypeEquipement>_<NomSite> 
*   ou XDG_ECMD_<TypeEquipement>_<NomMachine> pour un LT.
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Active ou desactive un equipement externe connecte a
*  une machine sur un site
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern 	int XZEC04_Activ_Desactiv_Eqt (	XDY_NomSite	pa_NomSite,
					XDY_NomMachine	pa_NomMachine,
					int		va_TypeEquipement,
					int		va_NumEquipement,
					int		va_ActivDesactiv );		

/*
* ARGUMENTS EN ENTREE :
*   va_NomSite		nom du site ( PC district ou LT )
*   va_NomMachine	nom de la machine
*   va_TypeEquipement	type d'equipt a activer ou a desactiver
*   va_NumEquipement	numero de l'equipement a activer ou a desactiver
*   va_ActivDesactiv	type d'action sur l'equipement
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec envoi message
*   XZECC_ERR_ARG	argument va_ActivDesactiv invalide
*   XZECC_ERR_TYPE_EQT	Type d'equipt invalide
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   Construction du message XDM_ECMD_EQPT en specifiant le type 
*   d'action et en fournissant les valeurs des arguments recus.
*   Envoi du message sur le DataGroup XDG_ECMD_<TypeEquipement>_<NomSite> 
*   ou XDG_ECMD_<TypeEquipement>_<NomMachine> pour un LT.
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de demarrer ou d'arreter les taches temporaires liees
*  a un equipement externe.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern 	int XZEC05_Cmd_TacheTmp_Eqt (	XDY_NomSite	pa_NomSite,
					XDY_NomMachine	pa_NomMachine,
					int		va_TypeEquipement,
					int		va_MarcheArret );		

/*
* ARGUMENTS EN ENTREE :
*   va_NomSite		nom du site ( PC district ou LT )
*   va_NomMachine	nom de la machine
*   va_TypeEquipement	type d'equipt a concerne (XZECC_TYPE_EQT_ )
*   va_MarcheArret	type d'action sur les taches associees a l'equipement
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec envoi message
*   XZECC_ERR_ARG	argument va_MarcheArret invalide
*   XZECC_ERR_TYPE_EQT	Type d'equipt invalide
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   Construction du message XDM_ECMD_EQPT en specifiant le type 
*   d'action et en fournissant les valeurs des arguments recus.
*   Envoi du message sur le DataGroup XDG_ECTL_EQPT_<NomMachine> du site concerne.
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Inhibe ou a desinhibe un equipement externe connecte a
*  une machine sur un site
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZEC06_Inhibition_Eqt (	XDY_NomSite	pa_NomSite,
					XDY_NomMachine	pa_NomMachine,
					int		va_TypeEquipement,
					int		va_NumEquipement,
					int		va_EtatInhibition );		

/*
* ARGUMENTS EN ENTREE :
*   pa_NomSite		nom du site
*   pa_NomMachine	nom de la machine
*   va_TypeEquipement	type d'equipt a inhibe ou a desinhibe
*   va_NumEquipement	numero de l'equipement a inhibe ou a desinhibe
*   va_EtatInhibition	type d'action sur l'equipement
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK		succes
*   XDC_NOK		echec envoi message
*   XZECC_ERR_ARG	argument va_EtatInhibition invalide
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   Construction du message XDM_ECMD_EQPT en specifiant le type 
*   d'action et en fournissant les valeurs des arguments recus.
*   Envoi du message sur le DataGroup XDG_ECMD_<TypeEquipement>_<NomMachine>.
*
------------------------------------------------------*/
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Envoyer l'etat des alarmes des equipements.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int	XZEC07_Alarme_Eqt (XDY_TypeEqt va_TypeEqt, XDY_Eqt va_NumEqt, XDY_Etat_Eqt va_Etat_Eqt);

/*
* ARGUMENTS EN ENTREE :
* XDY_TypeEqt	Type de l'�quipement � rafra�chir
* XDY_Eqt	Num�ro de l'�quipement,
* XDY_Etat_Eqt	Etat de l'�quipement.
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
* XDC_NOK		Erreur dans l'ex�cution de la primitive
* XDY_OK		Ex�cution correcte de la primitive
*
* CONDITION D'UTILISATION
*
* FONCTION 
* Contr�le des param�tres en entr�e.
* Construction du message XDM_Etat_Dispo_Eqt et envoi dans le datagroup
* XDG_Etat_Dispo_Eqt
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Verification du fichier contenant les coefficients de calcul des donnees 6 minutes glissantes.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern	int XZEC20_Verif_CDMG (	XDY_Fichier	va_Fd,		
				XDY_Entier *	va_ResultatOut );

/*
* ARGUMENTS EN ENTREE :
*   XDY_Fichier		va_Fd	: File descriptor
*
* ARGUMENTS EN SORTIE :
*   XDY_Entier *	va_ResultatOut	: OK=NULL ou ligne de l'erreur du systeme
*
* CODE RETOUR : 
*   XDC_OK		succes
*   Numero de ligne contenant une erreur de synthaxe
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   Lecture de chaque ligne du fichier et controle de la synthaxe du contenu de la ligne.
*
------------------------------------------------------*/



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Verification du fichier de configuration des donnees classe de Distances Inter Vehiculaire.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern	int XZEC21_Verif_Classe_DIV (	XDY_Fichier	va_Fd,
					XDY_Entier *	va_ResultatOut );	

/*
* ARGUMENTS EN ENTREE :
*   XDY_Fichier		va_Fd	: File descriptor
*
* ARGUMENTS EN SORTIE :
*   XDY_Entier *	va_ResultatOut	: OK=NULL ou ligne de l'erreur du systeme
*
* CODE RETOUR : 
*   XDC_OK		succes
*   Numero de ligne contenant une erreur de synthaxe
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   Lecture de chaque ligne du fichier et controle de la synthaxe du contenu de la ligne.
*
------------------------------------------------------*/



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Verification du fichier contenant des seuils trafic pour alerte DAI.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern	int XZEC22_Seuil_Trafic (	XDY_Fichier	va_Fd,	
					XDY_Entier *	va_ResultatOut );	

/*
* ARGUMENTS EN ENTREE :
*   XDY_Fichier		va_Fd	: File descriptor
*
* ARGUMENTS EN SORTIE :
*   XDY_Entier *	va_ResultatOut	: OK=NULL ou ligne de l'erreur du systeme
*
* CODE RETOUR : 
*   XDC_OK		succes
*   Numero de ligne contenant une erreur de synthaxe
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   Lecture de chaque ligne du fichier et controle de la synthaxe du contenu de la ligne.
*
------------------------------------------------------*/



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Verification du fichier de configuration des scenarios NAV.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern	int XZEC23_Scenario_Nav (	XDY_Fichier	va_Fd,	
					XDY_Entier *	va_ResultatOut );	

/*
* ARGUMENTS EN ENTREE :
*   XDY_Fichier		va_Fd	: File descriptor
*
* ARGUMENTS EN SORTIE :
*   XDY_Entier *	va_ResultatOut	: OK=NULL ou ligne de l'erreur du systeme
*
* CODE RETOUR : 
*   XDC_OK		succes
*   Numero de ligne contenant une erreur de synthaxe
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   Lecture de chaque ligne du fichier et controle de la synthaxe du contenu de la ligne.
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Verification du fichier de configuration des equations d'alerte.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern	int XZEC24_Equation_Alerte (	XDY_Fichier	va_Fd,	
					XDY_Entier *	va_ResultatOut );

/*
* ARGUMENTS EN ENTREE :
*   XDY_Fichier		va_Fd		: File descriptor
*
* ARGUMENTS EN SORTIE :
*   XDY_Entier *	va_ResultatOut	: OK=NULL ou ligne de l'erreur du systeme
*
* CODE RETOUR : 
*   XDC_OK		succes
*   Numero de ligne contenant une erreur de synthaxe
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   Lecture de chaque ligne du fichier et controle de la synthaxe du contenu de la ligne.
*
------------------------------------------------------*/

#endif
