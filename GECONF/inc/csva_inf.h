/*E Fichier : $Id: csva_inf.h,v 1.1 1995/01/25 10:40:36 mercier Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/01/25 10:40:36 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME GECONF
------------------------------------------------------
* MODULE INF * FICHIER INF.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*	Module de TCSVA contenant le callback informant 
* tous les 10 du mois qu il doit effectuer une sauvegarder 
* de la base histo du mois dernier.
* DCG-002/0.1
------------------------------------------------------
* HISTORIQUE :
*
* mercier	06 Jan 1995	: Creation
------------------------------------------------------ */

#ifndef INF
#define INF

/* fichiers inclus */
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
	#include        "xzca.h"

/* definitions de constantes exportees */

#define CINFC_HEURE_IHM_SAUVE 	"19:10:00"
#define CINFC_JOUR_IHM_SAUVE 	"16"	/* tous les dix du mois apparait le message*/
#define CINFC_CLE_TIMER_MENSUEL_INF	"archive"

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

extern XZIAT_destinataire 	CSVAV_NOMMACHINE;

/* delaration de fonctions externes */
/*
*	cinf_sauveinf_cb est declenchee sur reception du message RTWks XDM_CSV_INF.
*	Ce CallBack va prevenir l administrateur qu il doit effectuer l archivage .
*/

extern void cinf_sauvinf_cb (	T_IPC_CONN 			va_Cnx,
        			T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        			T_CB_ARG                        pa_ARG) ;
/*	
*  	cinf_ProgramTimer_Mois fonction qui permet de reprogrammer
* le timer pour le message declenchant l ouverture d une fenetre 
* d information pour l administrateur, lui rappelant qu il doit effectuer
* chaque mois l archivage de la base hist.
*	
*/

extern int cinf_ProgramTimer_Mois () ;

#endif
