/*E*/ 
/*Fichier :  $Id: eniw_cfg.h,v 1.2 2002/04/24 08:48:32 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2002/04/24 08:48:32 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TENIW * FICHIER eniw_cfg.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier de configuration de la
*  tache TENIW, gtc Nice
*
------------------------------------------------------
* HISTORIQUE :
*
* Moulin.D	version 1.1	07 Janv 2002	: Creation
* Moulin.D	version 1.2	23 Avr 2002	: Installation plate forme
*
------------------------------------------------------ */
 
#ifndef eniw_cfg
#define eniw_cfg

/* fichiers inclus */

#include "ex_msok.h"
#include "eniw_don.h"


/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */
extern 	T_INT4			vg_SockST;
extern 	XDY_NomSite        	vg_NomSite;

/* delaration de fonctions externes */

extern int			vg_Port_SockTWWI0;
extern int			vg_Port_SockTWWI1;

extern XDY_NomMachine vg_nom_machine_TWWI0;
extern XDY_NomMachine vg_nom_machine_TWWI1;
#endif
