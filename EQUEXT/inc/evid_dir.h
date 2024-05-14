/*E*/
/*Fichier :  $Id: evid_dir.h,v 1.7 2017/02/28 14:50:07 devgfi Exp $      Release : $Revision: 1.7 $        Date : $Date: 2017/02/28 14:50:07 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEVID * FICHIER evid_dir.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier directeur de la tache VIDEO.
*
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E	version 1.1	28 Oct 1994	: Creation
* Nagiel.E	version 1.2	26 Dec 1994	: 
* Volcic.F	version 1.3	24 Jan 1995	:
* Fontaine.C	version 1.4	23 Mai 1995	: Reprise entete, historique
* Mismer.D      version 1.5     18 Jan 1998     : Modif pour basculement PC2 (DEM/1541)
* Mismer.D      version 1.6     29 Mai 2000     : Modif pour basculement PC2 suite et fin (DEM/65)
* JPL		04/11/16 : Suppression etat PC2, LTV, magnetos; donnees de 2 Systemes video IP (DEM 1189)  1.7
*
------------------------------------------------------ */

#ifndef evid_dir
#define evid_dir

/* fichiers inclus */

#include	<xdy.h>
#include	<xdc.h>
#include	<xzst.h>

#include	<rtworks/common.h>
#include	<rtworks/ipc.h>

#include 	"ex_msok.h"
#include 	"evid_don.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

EVID_DONNEES_CAMERA		*pg_debutCamera = NULL;

//EVID_DONNEES_MONITEUR		*pg_debutMoniteur = NULL;

//INFO_EQT_FEN_LCR		*pg_DebutListeVIDFLcr;
int				vg_SockTIM = 0;

EVID_DONNEES_SYSTVIDEO	vg_sysvid[3];			/* Utilises : indices EVIDC_SYSV1 et EVIDC_SYSV2 */
int			vg_noSV;
int			vg_Mode_Fonct;

XDY_NomMachine		vg_NomMachine;
XDY_NomSite        	vg_NomSite;

/* delaration de fonctions externes */

XZEXT_MASK *		vg_SockMask = NULL;
T_INT4			vg_SockRT;

#endif
