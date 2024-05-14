/*E*/
/* Fichier : $Id: epic_dir.h,v 1.1 2023/08/18 14:28:10 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2023/08/18 14:28:10 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEPMV * FICHIER epic_dir.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Structure du module directeur de la tache TEPICTO
*
------------------------------------------------------
* HISTORIQUE :
*
* ABK	17/08/23		: Creation (DEM/483)
*
------------------------------------------------------ */

#ifndef epic_dir
#define epic_dir

/* fichiers inclus */

#include	<xdy.h>
#include	<xdc.h>
#include	<xzsc.h>
#include	<xzst.h>

#include	<rtworks/common.h>
#include	<rtworks/ipc.h>

#include 	"ex_msok.h"
#include 	"epic_cmd.h"
#include 	"epic_tra.h"
#include 	"epic_cfg.h"


/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

EPIC_DONNEES_PIC        *pg_debutPIC       = NULL;
INFO_EQT_FEN_LCR	*pg_DebutListePICFLcr  = NULL;
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
