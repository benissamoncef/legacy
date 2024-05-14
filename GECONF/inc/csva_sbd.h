/*E Fichier : $Id: csva_sbd.h,v 1.14 2013/04/12 08:51:46 pc2dpdy Exp $        Release : $Revision: 1.14 $        Date : $Date: 2013/04/12 08:51:46 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME GECONF
------------------------------------------------------
* MODULE SBD * FICHIER SBD.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*	Module MSBD de TCSVA contenant le callback qui effectue
* une sauvegarde hebdomadaire des bases vidant les journaux des
* transactions.
* DCG-002/0.1
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	06 Jan 1995	: Creation
* Torregrossa	02 Mai 1996	: Modif heure de sauvegarde hebdo 2h->5h (1.2)
* niepceron	11 Mar 1997 	: La capacite doit etre egale a 70% de la capacite reelle 1.3 cf fait techn 143
* niepceron	21 sep 1998	: separation EXP/HIS : capa=17GB et CSBDC_SITE_SAUVEGARDE=HIS dem/1698 v1.4
* cluchague	16 jan 2003	: augmentation capacite et du device pour dds4 (pour sauvegarde hebdo HIS) 
				  sur SDHIS v1.5 (REPERE1)
* cluchague	17 fev 2004	: changement du device pour passage sdhis a L1000 (v1.6)     REPERE2 
* Hilmarcher    28/10/2004      : augmentation de la capacite pour sauvegarde HIS (v1.7)
* PFE		26/10/2006	: augmentation de la capacite pour sauvegarde HIS (v1.???) sccs marche pas
*				: CSBDC_CAPACITE_DAT 54000000 au lieu de 34000000
* PFE		06/12/2006      : changement du device pour passage SDHIS en rp3440 le device etait c0t3d0
				  il devient c3t3d0
* PFE		14/05/2007	: changement du device pour Probleme de DAT , le device etait c3t3d0, il devient c3t4d0
* PFE           11/02/2009      : changement du device pour Probleme de DAT , le device etait c3t4d0, il devient c3t5d0
* PFE           17/02/2009      : changement du device pour Probleme de DAT , le device etait c3t5d0, il devient c1t3d0
* PFE           20/11/2012      : changement du device pour Probleme de DAT , le device etait c1t3d0, il devient c1t4d0
------------------------------------------------------ */

#ifndef SBD
#define SBD

/* fichiers inclus */
	/* includes systeme */
	
	#include        <stdio.h>

	/* includes Utilitaires */
	
		/* rtworks */
	#include        <rtworks/common.h>
	#include        <rtworks/ipc.h>
	#include        <rtworks/ipc_proto.h>
	
	/* includes applicatifs Migrazur */
	
	#include        "xdc.h"
	#include        "xdy.h"
	#include        "xdg.h"
	#include        "xzsc.h"
	#include        "xzsm.h"
	#include        "xzcg.h"
	#include        "xzia.h"
	#include        "xzagc.h"

/* definitions de constantes exportees */

#define CSBDC_BASE_HEBDO		"hebdo"
/* REPERE1 */
#define CSBDC_CAPACITE_DAT              54000000 
/*#define CSBDC_CAPACITE_DAT		28000000  environ 0.70 * 40GB */
/*#define CSBDC_CAPACITE_DAT		17000000  environ 0.70 * 24GB */
#define CSBDC_NOMFIC_DAT		"HIS_CFG_HEBDOMAD"
#define CSBDC_FIC_TRACE_HEBDO		XDC_PATHCNFFICDYN "/" XDF_BILAN_SAUVEG_HEBDO
#define CSBDC_SVG_GLOB			XZAGC_SVG_GLOB
/* REPERE1 */
/*#define CSBDC_DEVICE_SAUVEGARDE		"/dev/rmt/0mn"*/
/*#define CSBDC_DEVICE_SAUVEGARDE		"/dev/rmt/c2t3d0BESTn"*/
/* REPERE2 */
#define CSBDC_DEVICE_SAUVEGARDE		"/dev/rmt/2mn"
#define CSBDC_SITE_SAUVEGARDE		"HIS"
#define CSBDC_NOM_VOLUME_HEBDO		"HISCFG"

#define CSBDC_CLE_TIMER_HEBDO_SBD	"SAUVE_AUTO"
#define CSBDC_HEURE_IHM_SAUVE		"05:00:00"
#define CSBDC_JOUR_SAUVE_HEBDO		2	/* 0 = DIM,...,2 = Mardi*/
#define CSBDC_DELAI_HEBDO		(3600*24*7)

/* definitions de types exportes */



/* definition de macro exportees */

/* declarations de donnees globales */
	
extern XZIAT_destinataire 	CSVAV_NOMMACHINE;

/* delaration de fonctions externes */

/*
*	csbd_sauvebd_cb est declenchee sur reception du message RTWks XDM_CSV_SBD.
*	Ce CallBack va effectuer une sauvegarde des bases sur bande et vide le journal
* des transactions,
* previent l administrateur de l etat de la sauvegarde.
*/
extern void csbd_sauvebd_cb (	T_IPC_CONN 			,
        			T_IPC_CONN_PROCESS_CB_DATA      ,
        			T_CB_ARG                        ) ;

/*
*	csbd_ProgramTimer_Hebdo fonction qui permet de reprogrammer
* le timer pour le message declenchant l ouverture d une fenetre 
* d information pour l administrateur, lui rappelant qu il doit effectuer
* chaque mois l archivage de la base hist.
*	
*/

extern int csbd_ProgramTimer_Hebdo () ;

#endif
