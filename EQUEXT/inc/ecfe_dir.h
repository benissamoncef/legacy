/*E*/
/* Fichier : $Id: ecfe_dir.h,v 1.1 2019/01/29 11:05:37 pc2dpdy Exp $     Release : $Revision: 1.1 $      Date :  $Date: 2019/01/29 11:05:37 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TECFE * FICHIER ecfe_dir.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Structure du module directeur de la tache TECFE 
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG   01/03/18 :      Creation CTRL_FEUX 1.1 DEM1284
*
------------------------------------------------------ */

#ifndef ecfe_dir
#define ecfe_dir

/* fichiers inclus */

#include	<xdy.h>
#include	<xdc.h>
#include	<xzsc.h>
#include	<xzst.h>

#include	<rtworks/common.h>
#include	<rtworks/ipc.h>

#include 	"ex_msok.h"
#include 	"etim_don.h"
#include 	"ecfe_don.h"
#include 	"ecfe_cmd.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

ECFE_DONNEES_CFE		*pg_debutCFE = NULL;
INFO_EQT_FEN_LCR		*pg_DebutListeCFEFLcr;
int    			vg_SockTIM	= 0;
int			vg_Mode_Fonct;
XDY_NomMachine		vg_NomMachine;
XDY_NomSite        	vg_NomSite;
int			vg_Contexte = 0;
int			vg_SockTEMIP=0;

/* delaration de fonctions externes */

XZEXT_MASK *		vg_SockMask = NULL;
T_INT4			vg_SockRT;

#endif
