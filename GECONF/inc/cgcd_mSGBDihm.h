/*E Fichier : $Id: cgcd_mSGBDihm.h,v 1.3 1995/01/25 10:40:12 mercier Exp $      Release : $Revision: 1.3 $        Date : $Date: 1995/01/25 10:40:12 $
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
*  modificationSGBDihm.
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	19 Oct 1994	: Creation
------------------------------------------------------ */

#ifndef MVF_SGBD_IHM
#define MVF_SGBD_IHM

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
	#include        "xdf.h"
	#include        "xzao.h"
	#include        "xzia.h"
	#include        "xzsc.h"
	#include        "xzsm.h"
	
/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

extern  char CGCDG_Base_Courante[4];
extern XZIAT_destinataire CGCDV_NOMMACHINE;
extern FILE	*CGCDG_Fich_BilanConfig;

/* delaration de fonctions externes */

extern int cmvf_FctUti_MSI_XZAO62_63( XZAOT_ConfInfluence, 	XDY_Entier);
extern int cmvf_FctUti_MSI_XZAO118_119(XZAOT_ConfTroncon, 	XDY_Entier);



#endif
