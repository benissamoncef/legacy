/*E Fichier : $Id: cgcd_mSGBDma.h,v 1.3 1994/12/26 16:47:40 mercier Exp $      Release : $Revision: 1.3 $        Date : $Date: 1994/12/26 16:47:40 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME GECONF
------------------------------------------------------
* MODULE MVF * FICHIER MVF.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* 	Fichier appartenant au module MVF contenant les fonctions
*  modificationSGBDmachine. 
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	19 Oct 1994	: Creation
*******************
* MERCIER O.	21 Dec 1994	: Ajout de cgcd_liv.h	V 1.3
------------------------------------------------------ */

#ifndef MVF_MODIF_SGBD_MACHINE
#define MVF_MODIF_SGBD_MACHINE

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
	#include        "cgcd_mvf.h"

/* definitions de constantes exportees */

/* definitions des variables globales */

extern XZIAT_destinataire CGCDV_NOMMACHINE;
/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern int cmvf_FctUti_MSM_XZAO95_64(XZAOT_ConfMachine, 	XDY_Entier);
extern int cmvf_FctUti_MSM_XZAO96_65(XZAOT_ConfSuppl, 		XDY_Entier);
extern int cmvf_FctUti_MSM_XZAO97_66(XZAOT_ConfST, 		XDY_Entier);
#endif
