/*E Fichier : $Id: acli_xzan34_cb.h,v 1.1 1994/12/16 10:00:39 mercier Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/12/16 10:00:39 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE ACLI_XZAN34CB * FICHIER ACLI_XZAN34CB.h
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
------------------------------------------------------ */

#ifndef ACLI_XZAN34CB
#define ACLI_XZAN34CB

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
	#include	"asql_com.h"
	#include	"acli_dir.h"
	#include	"xzan34.h"

/* definitions de constantes exportees */
#define AXZAN34C_TIMERCLE_XZAN34   (ACLIDC_TIMERCLE_XZAN34)
#define AXZAN34C_HEURE_DECLENCHEMENT_XZAN34 (ACLIDC_HEURE_DECLENCHEMENT_XZAN34)

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
/*	
*	axtimer_XZAN34_Epurer_Calendrier_Astreinte_cb recoit le message RTWks 
*  XDM_AN_PURGE sur XDG_AN_site,  
* appelle XZAN34_Epurer_Calendrier_Astreinte.
*/
extern void axtimer_XZAN34_Epurer_Calendrier_Astreinte_cb(T_IPC_CONN ,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG);
#endif
