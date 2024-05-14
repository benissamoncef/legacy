/*E*/
/*Fichier :  $Id: eniw_dir.h,v 1.2 2002/04/24 08:48:57 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2002/04/24 08:48:57 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TENIW * FICHIER eniw_dir.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier directeur de la tache TENIW, gtc Nice
*
------------------------------------------------------
* HISTORIQUE :
*
* Moulin.D	version 1.1	07 Janv 2002	: Creation
* Moulin.D	version 1.2	23 Avr 2002	: Installation plate forme
*
------------------------------------------------------ */

#ifndef eniw_dir
#define eniw_dir
 
/* fichiers inclus */

#include	<xdy.h>
#include	<xdc.h>
#include	<xzst.h>
#include	<xzsc.h>

#include	<rtworks/common.h>
#include	<rtworks/ipc.h>

#include 	"ex_msok.h"
#include 	"etim_don.h"
#include 	"eniw_don.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

ENIW_DONNEES_Tube	*pg_debutTube 		= NULL;
ENIW_DONNEES_Echangeur	*pg_debutEchangeur 	= NULL;

XDY_NomMachine		vg_NomMachine;

XZEXT_MASK *		vg_SockMask  = NULL;
T_INT4			vg_SockRT    = 0;
int			vg_SockTWWI0= 0;
int			vg_SockTWWI1= 0;
int			vg_SockTETIM = 0;

int			vg_Port_SockTWWI0= 0;
int			vg_Port_SockTWWI1= 0;


/*etat de connexion des sockets*/
int			vg_etat_SockTWWI0= XDC_OK;
int			vg_etat_SockTWWI1= XDC_OK;
int			vg_maitre_actif= XDC_MAITRE_INCONNU;

XDY_NomMachine vg_nom_machine_TWWI0;
XDY_NomMachine vg_nom_machine_TWWI1;

/* delaration de fonctions externes */
extern int et_Redondance();
extern int et_Synchro();
extern int ex_lire_config_Wizcon();


#endif
