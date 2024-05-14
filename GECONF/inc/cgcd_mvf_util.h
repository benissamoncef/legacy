/*E Fichier : $Id: cgcd_mvf_util.h,v 1.3 2016/07/13 18:13:38 devgfi Exp $      Release : $Revision: 1.3 $        Date : $Date: 2016/07/13 18:13:38 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME GECONF
------------------------------------------------------
* MODULE MMVF_UTIL * FICHIER MMVF_UTIL.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*	MMVF_UTIL contient des utilitaires pour MMVF:
*
**********************
*
* 	cmvf_Trace_TYPE_Result : fonction permettant de construire le message de trace 
* Bilan Config et de tester le mode d execution des Fct du type XZAO51 appelee 
* par les fcts utilisateurs.
* Cette fonction n est appelee que par les fct utilisateurs.
*
**********************
*
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	21 Oct 1994	: Creation
* JPL		13/07/16	: Declaration de la fonction cmvf_Trace_TYPE_Result  1.3
------------------------------------------------------ */

#ifndef MMVF_UTIL
#define MMVF_UTIL

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
	#include        "xzao.h"
	#include        "xzsc.h"
	#include        "xzsm.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

extern  char CGCDG_Base_Courante[4];

/* delaration de fonctions externes */

extern	int cmvf_Trace_TYPE_Result (XDY_ResConf, char *, int *);

#endif
