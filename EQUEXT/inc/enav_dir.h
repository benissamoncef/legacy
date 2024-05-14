/*E*/
/* Fichier : $Id: enav_dir.h,v 1.7 1996/09/30 18:46:30 mismer Exp $      Release : $Revision: 1.7 $        Date : $Date: 1996/09/30 18:46:30 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TENAV * FICHIER enav_dir.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Structure du module directeur de la tache TENAV 
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	02 Sep 1994	: Creation
* Nagiel.E	version 1.2	21 Dec 1994	: 
* Nagiel.E	version 1.3	21 Dec 1994	: RAS
* Mismer.D	version 1.4	23 Jan 1995	:
* Fontaine.C	version 1.5	18 Mai 1995	: Reprise entete, historique
* Mismer.D	version 1.6	22 Sep 1995	: Ajout etat matrice NAV
* Mismer.D	version 1.7	13 Sep 1996	: Ajout init contexte (RADT_W)
*
------------------------------------------------------ */

#ifndef enav_dir
#define enav_dir

/* fichiers inclus */

#include	<xdy.h>
#include	<xdc.h>
#include	<xzsc.h>
#include	<xzst.h>

#include	<rtworks/common.h>
#include	<rtworks/ipc.h>

#include 	"ex_msok.h"
#include 	"etim_don.h"
#include 	"enav_don.h"
#include 	"enav_cmd.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

ENAV_DONNEES_NAV		*pg_debutNAV = NULL;
ENAV_DONNEES_TYPE_NAV		*pg_debutTypeNAV = NULL;
INFO_EQT_FEN_LCR		*pg_DebutListeNAVFLcr;
int    			vg_SockTIM	= 0;
int			vg_Mode_Fonct;
XDY_NomMachine		vg_NomMachine;
XDY_NomSite        	vg_NomSite;
int			vg_Contexte = 0;
COMP			vg_CompScenar[XZEXC_NB_SCENAR_PREDEF];

/* delaration de fonctions externes */

XZEXT_MASK *		vg_SockMask = NULL;
T_INT4			vg_SockRT;

#endif
