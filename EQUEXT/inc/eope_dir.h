/*E*/
/* Fichier : $Id: eope_dir.h,v 1.2 1995/05/04 20:14:15 cornu Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/05/04 20:14:15 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME EQUEXT
------------------------------------------------------
* MODULE TEOPE * FICHIER eope_dir.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Declaration des variables et fonctions du fichier directeur de la tache TEOPE
*
------------------------------------------------------
* HISTORIQUE :
*
* Nouaille.M	Version 1.1	02 Dec 1994	: Creation
* Fontaine.C    Version 1.2     04 Mai 1995     : Reprise entete
------------------------------------------------------ */

#ifndef eope_dir
#define eope_dir

/* fichiers inclus */

#include	<xdy.h>
#include	<xdc.h>
#include	<xzst.h>

#include	<rtworks/common.h>
#include	<rtworks/ipc.h>


#include 	"ex_msok.h"
#include 	"eope_don.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */
 EOPE_DONNEES_OPER		*pg_debutOPER = NULL ;
 int				vg_Mode_Fonct ;
 XDY_NomMachine			vg_NomMachine;
 XDY_NomSite        		vg_NomSite;


/* delaration de fonctions externes */

XZEXT_MASK *		vg_SockMask = NULL ;
T_INT4			vg_SockRT;


#endif
