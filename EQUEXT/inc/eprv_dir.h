/*E*/
/* Fichier : $Id: eprv_dir.h,v 1.1 2012/07/04 14:28:10 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 14:28:10 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEPMV * FICHIER eprv_dir.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Structure du module directeur de la tache TEPRV
*
------------------------------------------------------
* HISTORIQUE :
*
* VR	19/12/11		: Creation (DEM/1016)
*
------------------------------------------------------ */

#ifndef eprv_dir
#define eprv_dir

/* fichiers inclus */

#include	<xdy.h>
#include	<xdc.h>
#include	<xzsc.h>
#include	<xzst.h>

#include	<rtworks/common.h>
#include	<rtworks/ipc.h>

#include 	"ex_msok.h"
#include 	"eprv_cmd.h"
#include 	"eprv_tra.h"
#include 	"eprv_cfg.h"


/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

EPRV_DONNEES_PRV        *pg_debutPRV       = NULL;
INFO_EQT_FEN_LCR	*pg_DebutListePRVFLcr  = NULL;
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
