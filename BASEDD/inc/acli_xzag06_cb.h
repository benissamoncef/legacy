/*E Fichier : $Id: acli_xzag06_cb.h,v 1.5 2000/05/22 10:37:58 gesconf Exp $      Release : $Revision: 1.5 $        Date : $Date: 2000/05/22 10:37:58 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE ACLI_XZAG06 * FICHIER ACLI_XZAG06.h
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
* torregrossa	04 Sep 1995	: Changement d'include de purge (V 1.2)
* torregrossa	07 Jui 1996	: Correction offset de purge (V1.3)
* niepceron 	30 sep 1997	: ajout remontee d'erreur ds le journal d'adm v1.4
* guilhou	22 mai 2000	: purge sur HIS passe a 10 ans 1.5
------------------------------------------------------ */

#ifndef ACLI_XZAG06
#define ACLI_XZAG06

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
	#include	"xzag25.h"
/* definitions de constantes exportees */

#define AXZAG06C_TIMERCLE_XZAG06   (ACLIDC_TIMERCLE_XZAG06)
#define AXZAG06C_HEURE_DECLENCHEMENT_XZAG06 (ACLIDC_HEURE_DECLENCHEMENT_XZAG06)
#define AXZAG06C_OFFSET_PURGE_XZAG06 (24.*3600.*366.*10)

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
extern void axtimer_XZAG06_Purge_Base_Historique_cb 	(T_IPC_CONN ,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG); 

#endif
