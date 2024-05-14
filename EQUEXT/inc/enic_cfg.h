/*E*/
/* Fichier : $Id: enic_cfg.h,v 1.2 1995/05/22 15:46:44 cornu Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/05/22 15:46:44 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TENIC * FICHIER enic_cfg.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier de configuration de la
*  tache TENIC, gtc Nice
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	03 Nov 1994	: Creation
* Fontaine.C	version 1.2	22 Mai 1995	: Reprise entete
*
------------------------------------------------------ */

#ifndef enic_cfg
#define enic_cfg

/* fichiers inclus */

#include "ex_msok.h"
#include "enic_don.h"


/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */
extern 	T_INT4			vg_SockST;
extern 	XDY_NomSite        	vg_NomSite;

/* delaration de fonctions externes */


#endif
