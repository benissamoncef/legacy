/*E*/
/*  Fichier : $Id: ewst_dir.h,v 1.1 2018/01/11 14:26:51 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2018/01/11 14:26:51 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TERAU * FICHIER ewst_dir.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier directeur de la tache TERAU
*
------------------------------------------------------
* HISTORIQUE :
*
* ABE	12/04/22	creation DEM-SAE375
*
------------------------------------------------------ */

#ifndef ewst_dir
#define ewst_dir

/* fichiers inclus */

#include	<xdy.h>
#include	<xdc.h>
#include	<xzst.h>

#include	<rtworks/common.h>
#include	<rtworks/ipc.h>

#include "ewst_cmd.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */
XDY_NomMachine		vg_NomMachine;
XDY_Booleen         vg_etat_alarme; //0 = pas d'alarme; 1 = alarme

/* delaration de fonctions externes */

#endif