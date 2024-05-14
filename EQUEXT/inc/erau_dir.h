/*E*/
/*  Fichier : $Id: erau_dir.h,v 1.6 1998/02/27 16:37:31 mismer Exp $      Release : $Revision: 1.6 $        Date : $Date: 1998/02/27 16:37:31 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TERAU * FICHIER erau_dir.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier directeur de la tache TERAU
*
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E	version 1.1	30 Nov 1994	: Creation
* Nagiel.E	version 1.2	24 Jan 1995	:
* Fontaine.C	version 1.3	22 Mai 1995	: Reprise entete, historique
* Mismer.D	version 1.4	06 Nov 1995	: Suppression vg_quarte
* Mismer.D	version 1.5	24 Oct 1996	: Modif pour Alerte PAU HS (DEM/12..)
* Mismer.D	version 1.6	21 Jan 1998	: Modif pour basculement RAU PC2 (DEM/1542)
*
------------------------------------------------------ */

#ifndef erau_dir
#define erau_dir

/* fichiers inclus */

#include	<xdy.h>
#include	<xdc.h>
#include	<xzst.h>

#include	<rtworks/common.h>
#include	<rtworks/ipc.h>

#include 	"ex_msok.h"
#include 	"erau_don.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

ERAU_DONNEES_PAU		*pg_debutPAU = NULL;
ERAU_DONNEES_SYSTRAU		vg_donsysrau;
INFO_EQT_FEN_LCR	*pg_DebutListeRAUFLcr  = NULL;
/* Pointeur liste Info PAU pour alerte PAU HS */
XZEXT_CONFIG_PAU	*pg_DebInfoPAU = NULL;
ERAU_CONFIG_VOISIN	*pg_DebListeVoisin = NULL;	
ERAU_PARAM_ALERTE	vg_Config_PAU_HS;

int			vg_Mode_Fonct;
int			vg_etatGN     = XZEXC_LIBRE;
int			vg_etatPO1    = XZEXC_LIBRE;
int			vg_etatPO2    = XZEXC_LIBRE;
int			vg_etatGNFut  = XZEXC_LIBRE;
int			vg_etatPO1Fut = XZEXC_LIBRE;
int			vg_etatPO2Fut = XZEXC_LIBRE;
int			vg_no_po_test = 0;
int			vg_test_auto_en_cours = XDC_FAUX;

int			vg_status_recu = XDC_FAUX;


XDY_NomMachine		vg_NomMachine;
XDY_NomSite        	vg_NomSite;
int        		vg_PC2;

/* delaration de fonctions externes */

XZEXT_MASK *		vg_SockMask = NULL;
T_INT4			vg_SockRT;

#endif
