/*E*/
/* Fichier : $Id: ervi_dir.h,v 1.1 2018/12/06 11:56:13 pc2dpdy Exp $     Release : $Revision: 1.1 $      Date :  $Date: 2018/12/06 11:56:13 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TECFE * FICHIER ervi_dir.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Structure du module directeur de la tache TECFE 
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG   14/11/18 : creation RMVI DEM1315 1.1
*
------------------------------------------------------ */

#ifndef ervi_dir
#define ervi_dir

/* fichiers inclus */

#include	<xdy.h>
#include	<xdc.h>
#include	<xzsc.h>
#include	<xzst.h>

#include	<rtworks/common.h>
#include	<rtworks/ipc.h>

#include 	"ervi_cmd.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

XDY_NomMachine		vg_NomMachine;
XDY_NomSite        	vg_NomSite;

/* delaration de fonctions externes */


#endif
