/*Fichier :  $Id: eniw_red.h,v 1.2 2002/04/24 08:49:07 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2002/04/24 08:49:07 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TENIW * FICHIER eniw_tra.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier de traitement de la
*  tache TENIW, gtc Nice
*
------------------------------------------------------
* HISTORIQUE :
*
* Moulin.D	version 1.1	07 Janv 2002	: Creation
* Moulin.D	version 1.2	23 Avr 2002	: Installation plate forme
*
------------------------------------------------------ */

#ifndef eniw_red
#define eniw_red
 
/* fichiers inclus */

#include 	"ex_msok.h"
#include "eniw_don.h"
#include "xzaac.h"


/* definitions de constantes exportees */


/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */
extern int	vg_etat_SockTWWI0;
extern int	vg_etat_SockTWWI1;
extern int	vg_maitre_actif;

extern XDY_NomMachine vg_nom_machine_TWWI0;
extern XDY_NomMachine vg_nom_machine_TWWI1;

extern int	vg_Port_SockTWWI0;
extern int	vg_Port_SockTWWI1;

/* delaration de fonctions externes */

#endif
