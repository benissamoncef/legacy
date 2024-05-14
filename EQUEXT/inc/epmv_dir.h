/*E*/
/* Fichier : $Id: epmv_dir.h,v 1.5 1996/09/30 18:27:44 mismer Exp $      Release : $Revision: 1.5 $        Date : $Date: 1996/09/30 18:27:44 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEPMV * FICHIER epmv_dir.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Structure du module directeur de la tache TEPMV
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	02 Sep 1994	: Creation
* Nagiel.E	version 1.2	16 Dec 1994	:
* Mismer.D	version 1.3	08 Mar 1995	:
* Fontaine.C	version 1.4	18 Mai 1995	: Reprise entete, historique
* Mismer.D      version 1.5    13 Sep 1996     : Modif pour eclairage et periode (RADT_W)
*
------------------------------------------------------ */

#ifndef epmv_dir
#define epmv_dir

/* fichiers inclus */

#include	<xdy.h>
#include	<xdc.h>
#include	<xzsc.h>
#include	<xzst.h>

#include	<rtworks/common.h>
#include	<rtworks/ipc.h>

#include 	"ex_msok.h"
#include 	"epmv_cmd.h"
#include 	"epmv_tra.h"
#include 	"epmv_cfg.h"


/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

EPMV_DONNEES_PMV        *pg_debutPMV       = NULL;
EPMV_DONNEES_TYPE_PMV   *pg_debutTypePMV   = NULL;
EPMV_DONNEES_Picto      *pg_debutPicto     = NULL;
EPMV_DONNEES_TYPE_Picto *pg_debutTypePicto = NULL;
INFO_EQT_FEN_LCR	*pg_DebutListePMVFLcr  = NULL;
INFO_EQT_FEN_LCR	*pg_DebutListePictoFLcr = NULL;
XDY_NomMachine		vg_NomMachine;
XDY_NomSite        	vg_NomSite;
int			vg_Mode_Fonct;
int			vg_SockTIM = 0;
int			vg_Simulation = 0;
int			vg_Contexte = 0;

/* delaration de fonctions externes */

XZEXT_MASK *		vg_SockMask = NULL;
T_INT4			vg_SockRT;

#endif
