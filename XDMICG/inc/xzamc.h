/*E*/
/*Fichier : $Id: xzamc.h,v 1.23 2020/07/23 10:37:20 gesconf Exp $      Release : $Revision: 1.23 $        Date : $Date: 2020/07/23 10:37:20 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAM * FICHIER xzamc.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Alarmes >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	08  Nov 1994	: Creation
* gaborit	04  jan 1995	: ajout XZAMC_CRITIQUE (1.8)
* Nouaille M.   04 Jan 1995	: ajout messages telephonie (1.9)
* Mismer   D.   12 Jan 1995	: ajout messages Mini GTC (1.11)
* Mismer   D.   12 Jan 1995	: Mise en place des alarmes majeures (1.12)
* volcic	07 Fev 1995	: ajout XZAMC_ERR_PORT3 (v1.13)
* Mismer   D.   01 Mar 1995	: ajout XZAMC_COM_TACHE_EQPT_HS (v1.14)
* volcic	17 Mai 1995	: Modif Alarmes majeures  (v1.15)
* volcic	11 Jui 1995	: Modif Alarmes majeure Def Lampe Tunnel (v1.16)
* volcic	06 Oct 1995	: Modif libelle Alarme numero 26 ERREUR CHECKSUM (v1.17)
* Mismer   D.   21 Mar 1996	: Ajout de type d'alarme en reserve pour mini GTC (v1.18)
* C.T. 	   	21 Mar 1996	: Ajout de type d'alarme fax (v1.19)
* Mismer   D.   15 Nov 1996	: Suppression des noms d'alarme et des urgence (config fichier ssyst EQUI) (DEM/1232) (v1.20)
* JMG		03/12/08	: ajout nouvelles alarmes RAU DEM/850
* JMG		14/09/16:	ajout alarmes SAGA 1.22 DEM1191
* PNI	21/07/2020	: ajout XZAMC_DEF_CAPT 1.23 SAE-153 
------------------------------------------------------ */

/* fichiers inclus */

/* definitions de constantes exportees */

	/*A    Alarmes Manuelles : Mineure/Majeure/Critique    */

#define XZAMC_MANUELLE_MINEURE		27
#define XZAMC_MANUELLE_MINEURE_NOM	"Manuelle mineure"

#define XZAMC_MANUELLE_MAJEURE		28
#define XZAMC_MANUELLE_MAJEURE_NOM	"Manuelle majeure"


#define XZAMC_MANUELLE_CRITIQUE		29
#define XZAMC_MANUELLE_CRITIQUE_NOM	"Manuelle critique"

/*A Type d'urgence Alarme urgent=majeure, non urgent=mineure */

#define XZAMC_MAJEURE			1
#define XZAMC_MINEURE			2
#define XZAMC_CRITIQUE			3

#define XZAMC_URGENT			XZAMC_MAJEURE
#define XZAMC_NON_URGENT		XZAMC_MINEURE

/*A Alarmes (nom du type sur 25 caracteres maxi) */

#define XZAMC_NON_REP			1
#define XZAMC_ERR_CRC			2
#define XZAMC_ERR_TRANS			3
#define XZAMC_ERR_RGS			4

/* Alarmes generales equipement LCR */

#define XZAMC_ERR_EDF			5
#define XZAMC_ERR_INTERNE		6
#define XZAMC_ERR_PORT1			7
#define XZAMC_ERR_PORT2			8
#define XZAMC_ERR_PORT3			59


/* Alarme Equipement  PMV */

#define XZAMC_ERR_MESS_PERDU		9
#define XZAMC_ERR_HORL_HS		10
#define XZAMC_ERR_PB_MODU_MIN		11
#define XZAMC_ERR_PB_MODU_MAJ		12
#define XZAMC_ERR_PB_LAMPE_MIN		13
#define XZAMC_ERR_PB_LAMPE_MAJ		14
#define XZAMC_ERR_PB_TEMP		15
#define XZAMC_ERR_PB_CELL		16
#define XZAMC_ERR_DEF_TRANS		18
#define XZAMC_ERR_DEF_ALIM_LAMP		19
#define XZAMC_ERR_DEF_BATTERIE		20
#define XZAMC_ERR_PAN_BATTERIE		21
#define XZAMC_ERR_LAMPE_HS		22


/* Alarme Equipement VIDEO */  

#define XZAMC_DEF_VIDEO			23
#define XZAMC_DEF_TRANS			24
#define XZAMC_DEF_ANA_ARR		25
#define XZAMC_ERR_CRC_TELEI		26

/* Alarme GTC de NICE */

#define XZAMC_ENERGIE          	 	30
#define XZAMC_LAMPE            	 	31
#define XZAMC_DISC_SEQUENCE     	32
#define XZAMC_DISC_PANNEAU      	33

/* Alarme Equipement NAV */

#define XZAMC_ERR_MOT_HS       		34
#define XZAMC_ERR_TRANS_HS      	35
#define XZAMC_ERR_MOD_HS      		36
#define XZAMC_ERR_ARM_OUV      		37
#define XZAMC_ERR_DEF_FCT_VERIN 	38
#define XZAMC_ERR_BOUT_VID      	39

/* Alarme Equipement RAU */

#define XZAMC_ERR_ANT	        	40
#define XZAMC_DEF_PHONIE		41
#define XZAMC_DEF_PILE			42
#define XZAMC_DEF_REC_PRIN_32_DBM       572
#define XZAMC_DEF_REC_SECO_32_DBM       573
#define XZAMC_DEF_REC_PRIN_HORS_SPEC    574
#define XZAMC_DEF_REC_SECO_ACTIF        575
#define XZAMC_DEF_EME_SECO_ACTIF        576
#define XZAMC_DEF_PRESENCE_EAU_INTERF   577
#define XZAMC_DEF_GENERAL_ANTENNE       578
#define XZAMC_DEF_REVEIL_IMPOSSIBLE     579
#define XZAMC_DEF_ALIMENTATION          580
#define XZAMC_DEF_RESEAU_MODE_SECURISE  581
#define XZAMC_DEF_ALIM_5V_FAIBLE        582
#define XZAMC_DEF_PARAMETRAGE           583
#define XZAMC_DEF_ABSENCE_CARTES_PO     584
#define XZAMC_DEF_ABSENCE_CARTES_ANTENN 585
#define XZAMC_DEF_TEMP_TROP_ELEVEE      586
#define XZAMC_DEF_ALIM_5V_FORTE         587
#define XZAMC_DEF_RESET_BP              588
#define XZAMC_DEF_REBOOT_WDG            589
#define XZAMC_DEF_ABSENCE_PAU           590
#define XZAMC_DEF_GENERAL_CONFIG        591
#define XZAMC_DEF_ABSENCE_PO_LOGGE      592
#define XZAMC_DEF_BORNE_HS		593
#define XZAMC_DEF_ACCU			594
#define XZAMC_DEF_CAPT			890

/* Alarme Equipement Automate telephonique */

#define XZAMC_DEF_TRANS_TEL		43
#define XZAMC_DEF_REP_TEL		44

/* Alarme SATIR */

#define XZAMC_DEF_CYCL_SATIR		45

/* definitions des urgences mini GTC */

#define XZAMC_DEF_INTER_GEN		46
#define XZAMC_DEF_ONDUL_BAT		47
#define XZAMC_DEF_ONDULEUR		48
#define XZAMC_DEF_CLIM			49
#define XZAMC_DEF_BUS_NORMAL_EST	50
#define XZAMC_DEF_BUS_NORMAL_OUEST	51
#define XZAMC_DEF_BUS_ONDULE_EST	52
#define XZAMC_DEF_BUS_ONDULE_OUEST	53
#define XZAMC_DEF_BAIE_VIDEO		54
#define XZAMC_DEF_BAIE_DAI		55
#define XZAMC_DEF_BAIE_INFO		56
#define XZAMC_DEF_PMV1			57
#define XZAMC_DEF_PMV2			58
#define XZAMC_COM_TACHE_EQPT_HS		59

/*A Alarmes FAX */

#define XZAMC_NUM_FAX_BRULE		65
#define XZAMC_NUM_FAX_BRULE_NOM		"NUMERO DE FAX BRULE"


/*SAGA*/
#define	XZAMC_DEF_PIXEL			11
#define	XZAMC_DEF_COMMANDE		33
#define	XZAMC_DEF_CAPTEUR		36
#define XZAMC_DEF_EQUIPEMENT		1

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

