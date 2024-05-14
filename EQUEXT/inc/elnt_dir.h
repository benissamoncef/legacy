/*E*/
/*Fichier : $Id: elnt_dir.h,v 1.3 1995/06/08 10:10:38 volcic Exp $      Release : $Revision: 1.3 $        Date : $Date: 1995/06/08 10:10:38 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TELNT * FICHIER elnt_dir.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier directeur de la 
*  tache TELNT.
*
------------------------------------------------------
* HISTORIQUE :
*
* Volcic.F 	version 1.1	10 Fev 1995	: Creation
* Fontaine.C	version 1.2	23 Mai 1995	: Reprise entete
* Volcic.F 	version 1.3	07 Jun 1995	: Modif extern fonction
*
------------------------------------------------------ */

#ifndef elen_dir
#define elen_dir

/* fichiers inclus */

#include "xdg.h"
#include "xdm.h"
#include "xzst.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

int el_Mode_Lent (	XDY_NomMachine,
			XDY_Booleen );

#endif

