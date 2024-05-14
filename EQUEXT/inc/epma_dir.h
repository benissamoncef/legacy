/*E*/
/* Fichier : $Id: epma_dir.h,v 1.1 2005/09/14 15:06:51 gesconf Exp $  Release : $Revision: 1.1 $      Date : $Date: 2005/09/14 15:06:51 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEPMV * FICHIER epma_dir.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Structure du module directeur de la tache TEPMVA
*
------------------------------------------------------
* HISTORIQUE :
*
* jmg	10/06/05	creation
*
------------------------------------------------------ */

#ifndef epma_dir
#define epma_dir

/* fichiers inclus */

#include	<xdy.h>
#include	<xdc.h>
#include	<xzsc.h>
#include	<xzst.h>

#include	<rtworks/common.h>
#include	<rtworks/ipc.h>

#include 	"ex_msok.h"
#include 	"epma_cmd.h"
#include 	"epma_tra.h"
#include 	"epma_cfg.h"


/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

EPMVA_DONNEES_PMVA        *pg_debutPMVA       = NULL;
INFO_EQT_FEN_LCR	*pg_DebutListePMVAFLcr  = NULL;
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
