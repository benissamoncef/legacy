/*E Fichier : $Id: acli_scl.h,v 1.6 1997/05/14 18:56:38 gaborit Exp $      Release : $Revision: 1.6 $        Date : $Date: 1997/05/14 18:56:38 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE ASCL * FICHIER ASCL.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Contient le Callback de TACLI permettant d envoyer a
* l IHM les donnees Traffic 6mins.
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	15 Nov 1994	: Creation
* C.T.		13 jan 1995	: Ajout fonction ascl02 (1.4)
* C.T.		13 jan 1995	: Ajout fonction ascl03 (1.5)
* P.N.		04 fev 1997	: Ajout fonction ascl07 (1.6)
* PNI		07 dec 2021 : Correction nombre de parametres ascl07
------------------------------------------------------ */

#ifndef ASCL
#define ASCL

/* fichiers inclus */
	/* includes systeme */
	
	#include        <stdio.h>
	#include        <malloc.h>

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
	#include 	"xzasc.h"
	#include 	"xzas06sp.h"
	#include 	"xzis.h"
	 
/* definitions de constantes exportees */

#define ACLIC_ASCL02_RPC_NAME "XZAS;02"
#define ACLIC_ASCL03_RPC_NAME "XZAS;03"
#define ACLIC_ASCL06_RPC_NAME "XZAS;06"
#define ACLIC_ASCL13_RPC_NAME "XZAS13"
#define ACLIC_ASCL14_RPC_NAME "XZAS14"

#define ACLIC_ASCL06_NB_PARM_RETURN 1

#define ACLIC_ASCL13_NB_PARM_RETURN 0
#define ACLIC_ASCL14_NB_PARM_RETURN 7

#define ACLIC_ASCL06_NB_LIG_MESURES (XZISC_NB_LIG_MESURES)

/* definitions de types exportes */

typedef struct {
	XDY_Mot Valeur;
	XDY_Octet Validite;
} ASCL06T_LigneDonnees;

typedef  XZIST_Tab_LignesDonnees 	ASCL06T_Tab_LignesDonnees;
typedef  XZIST_Ligne_INRETS_PR 		ASCL02T_Tab_LignesDonneesPR;
typedef  XZIST_Ligne_INRETS_Trafic 	ASCL02T_Tab_LignesDonneesTrafic;
typedef  XZIST_Ligne_PRBouchons 	ASCL03T_Tab_LignesDonneesBouchon;
 
/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
extern void ascl02 (	T_IPC_CONN 			va_Cnx,
        		T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        		T_CB_ARG                        pa_ARG) ;
extern void ascl03 (	T_IPC_CONN 			va_Cnx,
        		T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        		T_CB_ARG                        pa_ARG) ;
extern void ascl06 (	T_IPC_CONN 			va_Cnx,
        		T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        		T_CB_ARG                        pa_ARG) ;

extern void ascl07 (	T_IPC_CONN 			va_Cnx,
        		T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        		T_CB_ARG                        pa_ARG) ;
#endif
