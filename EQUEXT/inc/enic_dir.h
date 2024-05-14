/*E*/
/* Fichier : $Id: enic_dir.h,v 1.2 1995/05/22 15:46:48 cornu Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/05/22 15:46:48 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TENIC * FICHIER enic_dir.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier directeur de la tache TENIC, gtc Nice
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	02 Nov 1994	: Creation
* Fontaine.C	version 1.2	22 Mai 1995	: Reprise entete 
*
------------------------------------------------------ */

#ifndef enic_dir
#define enic_dir

/* fichiers inclus */

#include	<xdy.h>
#include	<xdc.h>
#include	<xzst.h>
#include	<xzsc.h>

#include	<rtworks/common.h>
#include	<rtworks/ipc.h>

#include 	"ex_msok.h"
#include 	"etim_don.h"
#include 	"enic_don.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

ENIC_DONNEES_Tube	*pg_debutTube 		= NULL;
ENIC_DONNEES_Echangeur	*pg_debutEchangeur 	= NULL;

XDY_NomMachine		vg_NomMachine;

XZEXT_MASK *		vg_SockMask  = NULL;
T_INT4			vg_SockRT    = 0;
int			vg_SockTEXA0 = 0;
int			vg_SockTETIM = 0;

/* delaration de fonctions externes */

#endif
