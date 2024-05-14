/*E Fichier : $Id: acli_xzag05_cb.h,v 1.4 1998/09/22 13:53:20 gaborit Exp $      Release : $Revision: 1.4 $        Date : $Date: 1998/09/22 13:53:20 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE ACLI_XZAG05_CB * FICHIER ACLI_XZAG05_CB.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	07 Nov 1994	: Creation
* torregrossa	Sep 1995	: modif include (V 1.2)
* niepceron     30 sep 1997     : ajout remontee d'erreur ds le journal d'adm v1.3
* niecperon	02 Jul 1998	: separation EXP/HIS : AXZAG05C_OFFSET_PURGE_XZAG05 fait reference a XDC_OFFSET_EN_JOURS_PURGE_BASE v1.4
------------------------------------------------------ */

#ifndef ACLI_XZAG05_CB
#define ACLI_XZAG05_CB

/* fichiers inclus */
	/* includes systeme */
	
	#include        <stdio.h>

	/* includes Utilitaires */
	
		/* rtworks */
	#include        <rtworks/common.h>
	#include        <rtworks/ipc.h>
	#include        <rtworks/ipc_proto.h>
	
		/* sybase*/
	
	#include 	"cstypes.h"
	#include	"ctpublic.h"
	
	/* includes applicatifs Migrazur */
	
	#include        "xdc.h"
	#include        "xdy.h"
	#include        "xzag.h"
	#include        "xzsc.h"
	#include        "xzsm.h"
	#include        "xzst.h"
	#include	"asql_com.h"
	#include	"acli_dir.h"
	#include	"xzag15.h"

/* definitions de constantes exportees */

#define AXZAG05C_TIMERCLE_XZAG05   (ACLIDC_TIMERCLE_XZAG05)
#define AXZAG05C_HEURE_DECLENCHEMENT_XZAG05 (ACLIDC_HEURE_DECLENCHEMENT_XZAG05)
#define AXZAG05C_OFFSET_PURGE_XZAG05 (XDC_OFFSET_EN_JOURS_PURGE_BASE*24.*3600.)

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
extern void axtimer_XZAG05_Purge_Base_Exploitation_cb 	(T_IPC_CONN ,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG); 

#endif
