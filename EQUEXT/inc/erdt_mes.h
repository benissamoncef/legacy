/*E*/
/* Fichier : $Id: erdt_mes.h,v 1.1 1995/09/25 14:38:53 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/09/25 14:38:53 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEMES * FICHIER erdt_mes.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier directeur de la tache TEMES
*
------------------------------------------------------
* HISTORIQUE :
*
* Volcic.F	version 1.1	28 Jul 1995	: Creation
*
------------------------------------------------------ */

#ifndef erdt_mes
#define erdt_mes

/* fichiers inclus */

#include	<xdy.h>
#include	<xdc.h>
#include	<xzsc.h>
#include	<xzst.h>

#include	<rtworks/common.h>
#include	<rtworks/ipc.h>

#include 	"xdg.h"
#include 	"xzex.h"
#include 	"xzao000.h"
#include 	"ex_msok.h"
#include 	"etim_don.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

XDY_NomMachine			vg_NomMachine = "";
XDY_NomSite        		vg_NomSite = "";

/* delaration de fonctions externes */

XZEXT_MASK *			vg_SockMask = NULL;
T_INT4				vg_SockRT = NULL;

#endif
