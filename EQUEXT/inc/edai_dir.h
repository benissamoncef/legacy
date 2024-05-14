/*E*/
/*Fichier :  $Id: edai_dir.h,v 1.8 2000/05/31 12:12:18 gesconf Exp $      Release : $Revision: 1.8 $        Date : $Date: 2000/05/31 12:12:18 $ ------------------------------------------------------
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEDAI * FICHIER edai_dir.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Structure des donnees du fichier directeur de la tache TEDAI
*
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E	version 1.1	17 Oct 1994	: Creation
* Mismer.D	version 1.2	02 Dec 1994	:
* Nagiel.E	version 1.3	22 Dec 1994	:
* Volcic.F	version 1.4	22 Mar 1995	: Modifs apres essais
* Fontaine.C	version 1.5	23 Mai 1995	: Reprise entete, historique
* Mismer.D	version 1.7 	07 Jan 1997	: Ajout trace config voie sur travaux (DEM/1155)
* Mismer.D      version 1.8     30 Mai 2000     : Modif pour config avec 2 DAI sur un LT (DEM/102)
*
------------------------------------------------------ */

#ifndef edai_dir
#define edai_dir

/* fichiers inclus */

#include	<xdy.h>
#include	<xdc.h>
#include	<xzst.h>

#include	<rtworks/common.h>
#include	<rtworks/ipc.h>

#include 	"ex_msok.h"
#include 	"etim_don.h"
#include 	"edai_don.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

EDAI_DONNEES_DAI		*pg_debutDAI = NULL;
EDAI_DONNEES_DAI		*pg_ConfDAI = NULL;
EDAI_DONNEES_ANALYSEUR		*pg_debutAnaly = NULL;
EDAI_DONNEES_ANALYSEUR		*pg_ConfAnaly = NULL;
INFO_EQT_FEN_LCR		*pg_DebutListeDAIFLcr  = NULL;
int			vg_Mode_Fonct;
XDY_NomMachine		vg_NomMachine;
XDY_NomSite        	vg_NomSite;
int			vg_SockTIM = 0;
int			vg_NumConf[128];
char			vg_idpassw[18] = "";

/* delaration de fonctions externes */

XZEXT_MASK *		vg_SockMask = NULL;
T_INT4			vg_SockRT;

#endif
