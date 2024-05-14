/*E Fichier : $Id: cgcd_dir.h,v 1.6 2019/05/15 16:48:17 devgtie Exp $      Release : $Revision: 1.6 $        Date : $Date: 2019/05/15 16:48:17 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME GECONF
------------------------------------------------------
* MODULE MDIR * FICHIER MDIR.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module directeur conforme au DCG-002/0.1 de la tache TCGCD
* permettant de gerer la configuration des versions logiciels.
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	11 Oct 1994	: Creation
*****************
* MERCIER	06 Dec 1994	: Ajout de cgcd_liv.h 	V 1.3
* JPL		24/05/2016	: Declaration variable CGCDV_NOM_PERIPH_SVG (DEM 1165)  1.5
* RGR		03/05/2019	: Retrait ipc_proto.h 
------------------------------------------------------ */

#ifndef MDIR
#define MDIR

/* fichiers inclus */
	/* includes systeme */
	
	#include        <stdio.h>

	/* includes Utilitaires */
	
		/* rtworks */
	#include        <rtworks/common.h>
	#include        <rtworks/ipc.h>
	
	/* includes applicatifs Migrazur */
		
	#include        "xdc.h"
	#include        "xdy.h"
	#include        "xdg.h"
	#include        "xdf.h"	
	#include        "xzsm.h"
	#include        "xzsc.h"
	#include        "xzia.h"
	
	#include        "cgcd_vce.h"
	#include        "cgcd_mvf.h"
	#include	"cgcd_liv.h"
	#include	"cgcd_dif.h"
	#include	"cgcd_util.h"
	
/* definitions de constantes exportees */

#define CGCDC_ENTETE_BILAN_CONFIG "******* SESSION DE CONFIGURATION BASE DU "
#define CGCDC_TYPE_SYSTEM 	(XZSMC_TYPE_SYSTEME)

/* definitions de types exportes */

/* definitions de globales exportes */

XZIAT_destinataire 	CGCDV_NOMMACHINE;
int			CGCDV_CHECKLIST;

XDY_Nom 		CGCDV_NOM_PERIPH_SVG;


/* definition de macro exportees */

/* delaration de fonctions externes */
/*
*  Fonction permettant de sortir proprement de la tache TCGCD
*/
extern void cgcd_sortir( );


extern int cgcd_Lire_Valeur_Symbole( );

#endif
