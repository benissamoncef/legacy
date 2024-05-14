/*E*/
/* Fichier : $Id: esx0_dir.h,v 1.2 1995/05/23 11:42:35 cornu Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/05/23 11:42:35 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TESX0 * FICHIER esx0_dir.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier directeur de la tache
*  TESX0, protocole de la tache TESAT.
*
------------------------------------------------------
* HISTORIQUE :
*
* Gabert.P	version 1.1	21 Dec 1994	: Creation
* Fontaine.C	version 1.2	23 Mai 1995	: Reprise entete
*
------------------------------------------------------ */

#ifndef esx0_dir
#define esx0_dir

/* fichiers inclus */

#include	<xdy.h>
#include	<xdc.h>
#include	<xzst.h>

/* RTW 
#include	<rtworks/common.h>
#include	<rtworks/ipc.h>
*/

#include 	"ex_msok.h"
#include 	"etim_don.h"
#include 	"esx0_don.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

XZEXT_MASK *		vg_SockMask = NULL;

#ifdef PGT
/* RTW
T_INT4			vg_SockRT;
*/
#endif
T_INT4			vg_SockRT;
T_INT4			vg_SockSX0;
XDY_NomMachine		vg_NomMachine;
XDY_NomSite        	vg_NomSite;

#endif
