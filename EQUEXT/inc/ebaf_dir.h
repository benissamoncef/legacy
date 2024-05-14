/*E*/
/* Fichier : $Id: ebaf_dir.h,v 1.1 2007/03/26 11:08:02 gesconf Exp $     Release : $Revision: 1.1 $       Date : $Date: 2007/03/26 11:08:02 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEBAF * FICHIER ebaf_dir.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module d'initialisation des structures en memoires
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	15/09/05	creation
*
------------------------------------------------------ */

#ifndef ebaf_dir
#define ebaf_dir

/* fichiers inclus */

#include	<xdy.h>
#include	<xdc.h>
#include	<xzsc.h>
#include	<xzst.h>

#include	<rtworks/common.h>
#include	<rtworks/ipc.h>

#include 	"ex_msok.h"
#include 	"etim_don.h"
#include 	"ebaf_don.h"
#include 	"ebaf_cmd.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

EBAF_DONNEES_BAF		*pg_debutBAF = NULL;
INFO_EQT_FEN_LCR		*pg_DebutListeBAFFLcr=NULL;
int    			vg_SockTIM	= 0;
int			vg_Mode_Fonct;
XDY_NomMachine		vg_NomMachine;
XDY_NomSite        	vg_NomSite;
int			vg_Contexte = 0;


/* delaration de fonctions externes */

XZEXT_MASK *		vg_SockMask = NULL;
T_INT4			vg_SockRT;

#endif
