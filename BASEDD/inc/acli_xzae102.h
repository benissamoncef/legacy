/*E Fichier : $Id: acli_xzae102.h,v 1.1 1996/09/09 09:05:26 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1996/09/09 09:05:26 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE ACLI_XZAE102 * FICHIER ACLI_XZAE102.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* niepceron	27 Aou 1994	: Creation
------------------------------------------------------ */

#ifndef ACLI_XZAE102
#define ACLI_XZAE102

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
	#include	"xzae102.h"

/* definitions de constantes exportees */
#define AXZAE102C_TIMERCLE_XZAE102   "cle timer non cycl. IHM"
/*#define AXZAE102C_TIMERDELAI_XZAE102 3600.0*/
#define AXZAE102C_TIMERDELAI_XZAE102 60.0
/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
/*----------------------------------------------------
* SERVICE RENDU : 
*	axtimer_Ihm_cb recoit le message RTWks XDM_A_TIMER_RAFRAI_IHM,  
* appelle XZAE102_travaux_a_mettre_en_cours.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
extern void axtimer_trv_cours_cb 	(T_IPC_CONN ,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG); 
#endif
