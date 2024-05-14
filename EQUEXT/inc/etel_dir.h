/*E*/
/* Fichier : @(#)etel_dir.h	1.2      Release : 1.2        Date : 05/04/95
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME EQUEXT
------------------------------------------------------
* MODULE TETEL * FICHIER etel_dir.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Declaration des variables globales au fichier principal
*
*
------------------------------------------------------
* HISTORIQUE :
*
* Nouaille.M	Version 1.1	30 Nov 1994	: Creation
* Fontaine C.   Version 1.2     04 Mai 1995     : Reprise entete
* Diaz.L	Version 1.3     09 Oct 1998     : Ajout PABX
* SFO   	06-10-2014 : Compilation sous linux
------------------------------------------------------ */

#ifndef etel_dir
#define etel_dir

/* fichiers inclus */

#include	<xdy.h>
#include	<xdc.h>
#include	<xzst.h>

#include	<rtworks/common.h>
#include	<rtworks/ipc.h>


#include 	"ex_msok.h"
#include 	"etel_don.h"
#ifdef _HPUX_SOURCE
#include	"cstadrv.h"
#endif

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */
 ETEL_DONNEES_AUTOTEL		*pg_debutAUTOTEL = NULL ;
 ETEL_DONNEES_MAA		*pg_debutMAA = NULL ;
 int				vg_Mode_Fonct ;
 XDY_NomMachine			vg_NomMachine;
 int				vg_Type_Machine         = 0;
 XDY_NomSite        		vg_NomSite;
#ifdef _HPUX_SOURCE
 PbxLink			vg_Sock_Alcatel = -1;
 PbxLink			vg_Alcatel = -1;
#endif
 int				vg_Mac_PC2;
 char				vg_Numero_PO[4];


/* delaration de fonctions externes */

XZEXT_MASK *		vg_SockMask = NULL ;
T_INT4			vg_SockRT;


#endif
