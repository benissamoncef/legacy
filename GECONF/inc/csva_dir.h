/* Fichier : $Id: csva_dir.h,v 1.3 2016/05/25 17:07:16 devgfi Exp $        Release : $Revision: 1.3 $        Date : $Date: 2016/05/25 17:07:16 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME GECONF
------------------------------------------------------
* MODULE SVADIR * FICHIER SVADIR.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module Directeur de la tache TCSVA qui permet de sauvegarder
* restaurer les bases du CI.
* (DCG-002/0.1)
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	05 Jan 1995	: Creation
* JPL		24/05/2016	: Declaration fonction csva_Lire_Valeur_Symbole (DEM 1165)  1.3
------------------------------------------------------ */

#ifndef SVADIR
#define SVADIR

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
#include        "xdf.h"	
#include        "xzsm.h"
#include        "xzsc.h"
#include        "xzia.h"
#include	"csva_gca.h"
#include	"csva_inf.h"
#include	"csva_rsa.h"
#include	"csva_sbd.h"
#include	"csva_sva.h"
	
/* definitions de constantes exportees */

#define	CSVAC_TYPE_SYSTEM	(XZSMC_TYPE_SYSTEME)	
#define	CSVADC_TIMERCLE_SAUVINF "INFO ne pas oublier la sauvegarde du mois historise"

/* definitions de globales exportes */

XZIAT_destinataire 	CSVAV_NOMMACHINE;

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
/*
*  Fonction permettant de sortir proprement de la tache TCGCD
*/
extern void csva_sortir( );
/*  
*	csva_CanalComInit_cb est declenchee sur reception du message RTWks XDM_CSV_CANAL.
*  Permet de positionne la Globale CSVAV_NOMMACHINE vers la quelle on envoie les message 
*  d administration (via xzia08).
*
*/
extern void csva_CanalComInit_cb (	T_IPC_CONN,
        				T_IPC_CONN_PROCESS_CB_DATA,
        				T_CB_ARG);


extern int csva_Lire_Valeur_Symbole ( const char *, const char *, char * );

#endif
