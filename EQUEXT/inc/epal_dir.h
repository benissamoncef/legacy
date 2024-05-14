/*E*/
/* Fichier : $Id: epal_dir.h,v 1.1 1998/01/30 15:25:24 mismer Exp $     Release : $Revision: 1.1 $      Date :  $Date: 1998/01/30 15:25:24 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEPAL * FICHIER epal_dir.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Structure du module directeur de la tache TEPAL 
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	15 Jan 1998	: Creation
*
------------------------------------------------------ */

#ifndef epal_dir
#define epal_dir

/* fichiers inclus */

#include	<xdy.h>
#include	<xdc.h>
#include	<xzsc.h>
#include	<xzst.h>

#include	<rtworks/common.h>
#include	<rtworks/ipc.h>

#include 	"ex_msok.h"
#include 	"etim_don.h"
#include 	"epal_don.h"
#include 	"epal_cmd.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

EPAL_DONNEES_PAL		*pg_debutPAL = NULL;
INFO_EQT_FEN_LCR		*pg_DebutListePALFLcr;
int    			vg_SockTIM	= 0;
int			vg_Mode_Fonct;
XDY_NomMachine		vg_NomMachine;
XDY_NomSite        	vg_NomSite;
int			vg_Contexte = 0;

/* delaration de fonctions externes */

XZEXT_MASK *		vg_SockMask = NULL;
T_INT4			vg_SockRT;

#endif
