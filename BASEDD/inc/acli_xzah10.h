/*E Fichier : $Id: acli_xzah10.h,v 1.5 1998/06/03 11:59:03 gesconf Exp $      Release : $Revision: 1.5 $        Date : $Date: 1998/06/03 11:59:03 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE ACLI_XZAH10 * FICHIER ACLI_XZAH10.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	02 Nov 1994	: Creation
* gaborit	13 sep 1995	: modif delai et cle timer (1.3)
* niepceron	27 Aou 1996	: ajout de AXZAH10C_TIMERCLE_XZAH10_BLOC, AXZAH10C_TIMERDELAI_XZAH10_BLOC, axtimer_Ihm_bloc_cb (1.4)
* niepceron     04 Mai 1998	: AXZAH10C_TIMERDELAI_XZAH10_BLOC passe a 6mn (1.5)/
* niepceron     05 Mai 1998	: AXZAH10C_TIMERDELAI_XZAH10_BLOC passe a 6mn (1.5)/
*				  ajout axtimer_FMC_locales_cb 
------------------------------------------------------ */

#ifndef ACLI_XZAH10
#define ACLI_XZAH10

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
	#include        "xzis.h"
	#include	"asql_com.h"
	#include	"acli_dir.h"
	#include	"xzah10.h"

/* definitions de constantes exportees */
#define AXZAH10C_TIMERCLE_XZAH10   "cle timer non cycl. IHM"
#define AXZAH10C_TIMERDELAI_XZAH10 10.0
#define AXZAH10C_TIMERCLE_XZAH10_BLOC   "cle timer non cycl. IHM BLOC"
#define AXZAH10C_TIMERDELAI_XZAH10_BLOC 360.0
#define AXZAE101C_TIMERCLE_XZAE101   "cle timer non cycl. FMC LOC"
#define AXZAE101C_TIMERDELAI_XZAE101 30
/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
/*----------------------------------------------------
* SERVICE RENDU : 
*	axtimer_Ihm_cb recoit le message RTWks XDM_A_TIMER_RAFRAI_IHM,  
* appelle XZAH10_Animer_IHM.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
extern void axtimer_Ihm_cb 	(T_IPC_CONN ,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG); 
/*----------------------------------------------------
* SERVICE RENDU : 
*	axtimer_Ihm_cb recoit le message RTWks XDM_A_TIMER_RAFRAI_IHM_BLOC,  
* appelle XZAE101 et XZAH10_Animer_IHM.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
extern void axtimer_Ihm_bloc_cb 	(T_IPC_CONN ,T_IPC_CONN_PROCESS_CB_DATA,T_CB_ARG); 
/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	axtimer_FMC_locales_cb recoit le message RTWks XDM_A_TIMER_FMC_LOC,  
* appelle XZAE101_Liste_FMC_Locales.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void axtimer_FMC_locales_cb 	(T_IPC_CONN 			va_Cnx,
        		T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        		T_CB_ARG                        pa_ARG) ;
#endif
