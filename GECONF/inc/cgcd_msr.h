/*E Fichier : @(#)cgcd_msr.h	1.7      Release : 1.7        Date : 02/14/12
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME GECONF
------------------------------------------------------
* MODULE MVF * FICHIER MVF.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	18 Oct 1994	: Creation
***************
* MERCIER	21 Dec 1994	: Ajout de CGCDV_NOMMACHINE V 1.3
* Torregrossa	30 Oct 1995	:  modif interface focntion pour prise en compte 
*                                 sens de l'autoroute (A57) V 1.4
* MISMER	10 Fev 1997	: Ajout des fonction temps de parcours v1.5 (DEM/1395)
 * JMG		31 01 2012	: tdp etendu dem/1014 1.-
* VR		05 Juin 2012 : Ajout fonction cmvf_FctUti_MSR_XZAO466 (DEM/1014 PMA)
* LCL		15 Nov 2016	: Ajout fonction cmvf_FctUti_MSR_XZAO649_650 (LOT23)
* GGY		16/02/24		: Ajout des batiments (DEM-483)
------------------------------------------------------ */

#ifndef MVF2
#define MVF2

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
	#include        "cgcd_mvf_2.h"
	#include        "cgcd_mvf_3.h"
/* definitions de constantes exportees */

/* definitions des variables globales */

extern XZIAT_destinataire CGCDV_NOMMACHINE;
/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern int cmvf_FctUti_MSR_XZAO50_51( XDY_NomAuto,XDY_Mot,XDY_Octet, XDY_Entier);
extern int cmvf_FctUti_MSR_XZAO52_53( XZAOT_ConfDistrict, 	XDY_Entier);
extern int cmvf_FctUti_MSR_XZAO54_55( XZAOT_ConfPortion, 	XDY_Entier);
extern int cmvf_FctUti_MSR_XZAO56_57( XZAOT_ConfNoeud, 		XDY_Entier);
extern int cmvf_FctUti_MSR_XZAO58_59( XZAOT_ConfSegment, 	XDY_Entier);
extern int cmvf_FctUti_MSR_XZAO60_61( XZAOT_ConfPente, 		XDY_Entier);
extern int cmvf_FctUti_MSR_XZAO62_63( XZAOT_ConfInfluence, 	XDY_Entier);
extern int cmvf_FctUti_MSR_XZAO64_65( XZAOT_ConfSuppl, 		XDY_Entier);

extern int cmvf_FctUti_MSR_XZAO83_84( XZAOT_ConfEchang, 	XDY_Entier);
extern int cmvf_FctUti_MSR_XZAO85_86( XZAOT_ConfAire , 		XDY_Entier);
extern int cmvf_FctUti_MSR_XZAO87_88( XZAOT_ConfPeage , 	XDY_Entier);
extern int cmvf_FctUti_MSR_XZAO89_90( XZAOT_ConfPS , 		XDY_Entier);
extern int cmvf_FctUti_MSR_XZAO91_92( XZAOT_ConfReseau, 	XDY_Entier);
extern int cmvf_FctUti_MSR_XZAO93_94( XZAOT_ConfVille, 		XDY_Entier);
extern int cmvf_FctUti_MSR_XZAO120_121(XZAOT_ConfPnPol, 	XDY_Entier);
extern int cmvf_FctUti_MSR_XZAO145_146(XZAOT_ConfITPC, 		XDY_Entier);
extern int cmvf_FctUti_MSR_XZAO150_151(XZAOT_ConfPortail, 	XDY_Entier);
extern int cmvf_FctUti_MSR_XZAO405_406(XZAOT_ConfDestTDP, 	XDY_Entier);
extern int cmvf_FctUti_MSR_XZAO466(XZAOT_ConfDestPMA, XDY_Entier);
extern int cmvf_FctUti_MSR_XZAO461(XZAOT_ConfDestPMV, XDY_Entier);
extern int cmvf_FctUti_MSR_XZAO649_650( XZAOT_ConfViaduc , 		XDY_Entier);
extern int cmvf_FctUti_MSR_XZAO506_507( XZAOT_ConfNIC_IS , 		XDY_Entier);
extern int cmvf_FctUti_MSR_XZAO511_510 (XZAOT_ConfBat , XDY_Entier); 

#endif
