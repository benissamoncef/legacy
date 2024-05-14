/*E*/
/* Fichier : $Id: esx0_cyc.h,v 1.2 1995/05/23 11:42:34 cornu Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/05/23 11:42:34 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TESX0 * FICHIER esx0_cyc.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Structure des donnees du fichier de traitement cyclique
*   de la tache TESX0, protocole de la tache TESAT.
*
------------------------------------------------------
* HISTORIQUE :
*
* Gabert.P	version 1.1	22 Dec 1994	: Creation
* Fontaine.C	version 1.2	23 Mai 1995	: Reprise entete
*
------------------------------------------------------ */

#ifndef esx0_cyc
#define esx0_cyc

/* fichiers inclus */
#include	<xdy.h>
#include	<xdc.h>

#include 	"ex_msok.h"
#include 	"etim_don.h"
#include 	"esx0_don.h"

#include <sys/stat.h>

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */
extern XDY_NomMachine vg_NomMachine;

/* delaration de fonctions externes */

#endif
