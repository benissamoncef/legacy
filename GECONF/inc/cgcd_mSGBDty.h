/*E Fichier : $Id: cgcd_mSGBDty.h,v 1.5 1996/12/05 23:06:22 mismer Exp $      Release : $Revision: 1.5 $        Date : $Date: 1996/12/05 23:06:22 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME GECONF
------------------------------------------------------
* MODULE MVF * FICHIER cgcd_mSGBDty.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* 	Fichier appartenant au module MVF contenant les fonctions
* utilisateur de modificationSGBDtype.
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	19 Oct 1994	: Creation
********************
* Mercier O.	28 Nov 1994	: Ajout		V 1.3
*	de cmvf_FctUti_MST_XZAN02_01.
* MISMER	06 Nov 1996	: Ajout de la fonction de lecture des alarme et alerte v1.5 (DEM/1232)
------------------------------------------------------ */

#ifndef MVF_SGBD_TYPE
#define MVF_SGBD_TYPE

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
        #include        "xzan.h"
	#include        "xzsc.h"
	#include        "xzsm.h"
	#include        "cgcd_mvf.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
extern int cmvf_FctUti_MST_XZAO101_70(XZAOT_ConfTypPMV, 	XDY_Entier);
extern int cmvf_FctUti_MST_XZAO102_71(XZAOT_ConfTypPicto, 	XDY_Entier);
extern int cmvf_FctUti_MST_XZAO154_155(XZAOT_ConfScenario, 	XDY_Entier);
extern int cmvf_FctUti_MST_XZAN02_01( 	XDY_Nom         ,
                                        XDY_Octet       , 
                                        XDY_Entier     	); 
extern int cmvf_FctUti_MSE_XZAO198_188(XZAOT_ConfTypeAlerte, 	XDY_Entier);
extern int cmvf_FctUti_MSE_XZAO196_189(XZAOT_ConfTypeAlarme, 	XDY_Entier);
#endif
