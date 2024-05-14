/*E*/
/*Fichier : $Id: egtc_dir.h,v 1.4 1996/12/05 22:36:37 mismer Exp $       Release : $Revision: 1.4 $     Date : $Date: 1996/12/05 22:36:37 $
------------------------------------------------------
* GRANIOU *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEGTC * FICHIER egtc_dir.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Structure des donnees du fichier directeur de la tache TEGTC
*
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E	version 1.1	16 Dec 1994	: Creation
* Fontaine.C	version 1.2	19 Mai 1995	: Reprise entete
* Mismer.D	version 1.3 	15 Nov 1996	: Modif pour alerte DAI tunnel (DEM/1309)
* Mismer.D	version 1.4 	05 Dec 1996	: Suite modif pour alerte DAI tunnel (DEM/1309)
*
------------------------------------------------------ */

#ifndef egtc_dir
#define egtc_dir

/* fichiers inclus */

#include	<xdy.h>
#include	<xdc.h>
#include	<xzsc.h>
#include	<xzst.h>

#include	<rtworks/common.h>
#include	<rtworks/ipc.h>

#include 	"ex_msok.h"
#include 	"egtc_don.h"


/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

EGTC_DONNEES_GTC		vg_donneesGTC;
INFO_EQT_FEN_LCR	*pg_DebutListeGTCFLcr  = NULL;
XDY_NomMachine		vg_NomMachine;
XDY_NomSite        	vg_NomSite;
int			vg_Mode_Fonct;
int			vg_SockTIM= 0;
char			vg_idpassw[18];


/* delaration de fonctions externes */

XZEXT_MASK *		vg_SockMask = NULL;
T_INT4			vg_SockRT;

#endif
