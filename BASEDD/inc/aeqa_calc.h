/*E Fichier : $Id: aeqa_calc.h,v 1.1 1995/01/27 12:50:37 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/01/27 12:50:37 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE AEQA * FICHIER aeqa_calc.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Contient le Callback de TAEQA permettant de calculer
* les equations d'alerte
*
------------------------------------------------------
* HISTORIQUE :
*
* gaborit	25 jan 1995	: Creation
------------------------------------------------------ */

#ifndef AEQA_CALC_H
#define AEQA_CALC_H

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
	#include        "xdf.h"
	#include        "xzag.h"
	#include        "xzaa.h"
	#include        "xzsc.h"
	#include        "xzsm.h"
	#include	"asql_com.h"
	#include 	"xdea_eqn_alt.h"
	 
/* definitions de constantes exportees */

/* definitions de types exportes */
 
/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
extern void aeqa_calc (	T_IPC_CONN 			va_Cnx,
        		T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        		T_CB_ARG                        pa_ARG) ;

#endif
