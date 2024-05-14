/*E*/
/* Fichier : $Id: eimu_dir.h,v 1.2 2021/05/04 13:25:09 pc2dpdy Exp $     Release : $Revision: 1.2 $      Date :  $Date: 2021/05/04 13:25:09 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEIMU * FICHIER eimu_dir.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Structure du module directeur de la tache TEIMU 
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	04/11/2020 : Création DEM-SAE155 1.1
*
------------------------------------------------------ */

#ifndef eimu_dir
#define eimu_dir

/* fichiers inclus */

#include	<xdy.h>
#include	<xdc.h>
#include	<xzsc.h>
#include	<xzst.h>

#include	<rtworks/common.h>
#include	<rtworks/ipc.h>

#include 	"ex_msok.h"
#include 	"etim_don.h"
#include 	"eimu_don.h"
#include 	"eimu_cmd.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

EIMU_DONNEES_IMU		*pg_debutIMU = NULL;
INFO_EQT_FEN_LCR		*pg_DebutListeIMUFLcr;
int    			vg_SockTIM	= 0;
int			vg_Mode_Fonct;
XDY_NomMachine		vg_NomMachine;
XDY_NomSite        	vg_NomSite;
int			vg_Contexte = 0;

/* delaration de fonctions externes */

XZEXT_MASK *		vg_SockMask = NULL;
T_INT4			vg_SockRT;
T_INT4			vg_SockTEOPC;

#endif
