/*E Fichier : $Id: cgcd_mvf.h,v 1.6 2019/01/24 20:10:03 devgfi Exp $      Release : $Revision: 1.6 $        Date : $Date: 2019/01/24 20:10:03 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME GECONF
------------------------------------------------------
* MODULE MMVF * FICHIER cgcd_mvf.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*  MMVF : contient les fonctions de la verification et de
*  modification des fichiers de configuration
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	13 Oct 1994	: Creation
*******************
* MERCIER O.	28 Nov 1994	: Ajout de XZAN		V 1.3
*******************
* MERCIER O.	21 Dec 1994	: Ajout de cgcd_liv.h	V 1.4
* JPL		13/07/2016	: Ajout declaration fonction "cmvf_Modif_SGBD_Equipements"  1.5
* JPL		24/01/2019	: Inclusion des déclarations spécifiques à la configuration du SAE  1.6
------------------------------------------------------ */

#ifndef MMVF
#define MMVF

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
	#include        "xzan.h"
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

/* definitions de types exportes */

/* definitions des variables globales */

extern XZIAT_destinataire CGCDV_NOMMACHINE;

/* definition de macro exportees */

/* declarations de donnees globales */

extern  char CGCDG_Base_Courante[4];

extern FILE	*CGCDG_Fich_BilanConfig;

/* delaration de fonctions externes */
extern int cmvf_Modif_SGBD_Machine(); 
extern int cmvf_Modif_SGBD_Type();
extern int cmvf_Modif_SGBD_Equipements();
extern int cmvf_Modif_SGBD_Ihm();
#endif
