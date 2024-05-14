/*E Fichier : $Id: cgcd_mvf_3.h,v 1.3 1995/01/25 10:40:17 mercier Exp $      Release : $Revision: 1.3 $        Date : $Date: 1995/01/25 10:40:17 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME GECONF
------------------------------------------------------
* MODULE MMVF_3 * FICHIER MMVF_3.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* 
------------------------------------------------------
* HISTORIQUE :
*
* mercier	21 Oct 1994	: Creation
*************
* MERCIER	21 Dec 1994	: Ajout de CGCDV_NOMMACHINE 	V 1.2
*************
* MERCIER	14 Jan 1995	: Ajout Fct utili du SAD	V 1.3
*
------------------------------------------------------ */

#ifndef MMVF_3
#define MMVF_3

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
	#include        "xzdc.h"
	#include        "xzsc.h"
	#include        "xzsm.h"
	#include        "cgcd_dir.h"
	#include        "cgcd_msr.h"
	#include        "cgcd_mvf_util.h"
	#include        "cgcd_mSGBDma.h"
	#include        "cgcd_mSGBDty.h"
	#include        "cgcd_mSGBDeq.h"
	#include        "cgcd_mSGBDihm.h"
	#include        "cgcd_dupli_SGBD.h"

/* definitions de constantes exportees */

/* definitions des globales */

extern XZIAT_destinataire CGCDV_NOMMACHINE;

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
extern int cmvf_ModifSGBD_RSAD	();
extern int cmvf_Verif_DEFT	();
extern int cmvf_Verif_SCNT	();
extern int cmvf_Verif_EQAL	();
extern int cmvf_FctUtil_RSAD	(XDY_Nom,XDY_Octet );

#endif
