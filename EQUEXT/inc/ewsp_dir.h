/*E*/
/*  Fichier : $Id: ewsp_dir.h,v 1.1 2018/01/11 14:26:51 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2018/01/11 14:26:51 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TERAU * FICHIER ewsp_dir.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier directeur de la tache TERAU
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	21/09/16	creatio
*
------------------------------------------------------ */

#ifndef ewsp_dir
#define ewsp_dir

/* fichiers inclus */

#include	<xdy.h>
#include	<xdc.h>
#include	<xzst.h>

#include	<rtworks/common.h>
#include	<rtworks/ipc.h>

#include "ewsp_cyc.h"
#include "ewsp_cmd.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */
XDY_NomMachine		vg_NomMachine;
XDY_NomSite        	vg_NomSite;
XDY_District		vg_NumeroSite;
/* delaration de fonctions externes */

#endif
