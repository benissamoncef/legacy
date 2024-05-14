/*E*/
/* Fichier : $Id: exa0_dir.h,v 1.4 1997/05/06 19:58:13 mismer Exp $      Release : $Revision: 1.4 $        Date : $Date: 1997/05/06 19:58:13 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEXA0 * FICHIER exa0_dir.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Structure des donneesdu fichier directeur de la tache
*   TEXA0, protocole de la gtc Nice
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	23 Nov 1994	: Creation
* Fontaine.C	version 1.2	23 Mai 1995	: Reprise entete
* Mismer.D	version 1.3	06 Sep 1994	: Ajout vg_Etat_AprilNet
* Mismer.D      version 1.4     24 Avr 1997     : Ajout alerte monaco (DEM/1421)
*
------------------------------------------------------ */

#ifndef exa0_dir
#define exa0_dir

/* fichiers inclus */

#include	<xdy.h>
#include	<xdc.h>
#include	<xzst.h>
#include	<xzsc.h>

#include	<rtworks/common.h>
#include	<rtworks/ipc.h>

#include 	"exa0_don.h"
#include 	"exa0_cfg.h"
#include 	"exa0_ctl.h"
#include 	"exa0_cmd.h"
#include 	"exa0_tra.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */
#undef	FD_ZERO
#define	FD_ZERO(p)\
{\
    int i = sizeof(*(p))/sizeof((p)->fds_bits[0]);\
    while(i--)(p)->fds_bits[i] = 0;\
}

/* declarations de donnees globales */

XDY_NomMachine		vg_NomMachine;
int			vg_Mode_Fonct 		= 0;

EXA0_DONNEES_Tube	*pg_debutTube		= NULL;
EXA0_DONNEES_Echangeur	*pg_debutEchangeur	= NULL;
EXA0_ENG_CMD		*pg_debutCmd;

short			vg_TabFlag[XZETC_TAILLE_FLAG];
EXA0_DONNEES_GENERALE   vg_ConfigAdresse;
XZEXT_MASK *		vg_SockMask;
T_INT4			vg_SockRT = 0;
int			vg_SockTENIC = 0;
int			vg_SockSrvApr= 0;
int			vg_SockApril = 0;
int			vg_Etat_AprilNet = XDC_OK;
struct pollfd		*vg_SockLms1 = NULL;

lmoinaT vg_AssocApril;
errblT  vg_ErrApril;

short * pg_Telealarme		= NULL;
short * pg_Signalisation	= NULL;
short * pg_ETOR			= NULL;
short * pg_ITOR			= NULL;
short * pg_Alerte		= NULL;

short * pg_ResultTelealarme	= NULL;
short * pg_ResultSignalisation	= NULL;
short * pg_ResultETOR		= NULL;
short * pg_ResultITOR		= NULL;
short * pg_ResultAlerte		= NULL;

/* delaration de fonctions externes */

#endif
