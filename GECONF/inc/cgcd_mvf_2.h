/*E Fichier : $Id: cgcd_mvf_2.h,v 1.3 2019/01/24 20:10:08 devgfi Exp $      Release : $Revision: 1.3 $        Date : $Date: 2019/01/24 20:10:08 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME GECONF
------------------------------------------------------
* MODULE MMVF_SGBD_RESEAU * FICHIER cgcd_mvf_2.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  MMVF_SGBD_RESEAU appartient au module MMVF.
*                   contient les fonctions de la verification et de
*  modification des fichiers de configuration pour le SGBDreseau
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	21 Oct 1994	: Creation
*************
* MERCIER	21 Dec 1004	: Ajout de CGCDV_NOMMACHINE V 1.2
* JPL		24/01/2019	: Inclusion des déclarations spécifiques à la configuration du SAE  1.3
------------------------------------------------------ */

#ifndef MMVF_SGBD_RESEAU
#define MMVF_SGBD_RESEAU

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
	#include        "xzao_cfg.h"
	#include        "xzao.h"
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
extern int cmvf_Trace_TYPE_Result(XDY_ResConf,char *,int *);
extern int cmvf_ModifSGBD_Reseau();

#endif
