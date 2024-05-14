/*Fichier : $Id: ex_mdia.h,v 1.1 2019/01/29 11:06:20 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2019/01/29 11:06:20 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  : EQUEXT
------------------------------------------------------
* MODULE COMMUN * FICHIER ex_mdia.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier de traitement de la
*  fenetre LCR.
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	27/02/18 : Creation controleurs de feux CTRL_FEUX 1.1 DEM1284
------------------------------------------------------ */

#ifndef ex_mdia
#define ex_mdia

/* fichiers inclus */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <xdy.h>
#include <xdc.h>
#include <xdf.h>
#include <xdl.h>
#include <xzsc.h>
#include <xzst.h>
#include <xzex.h>
#include <xzec.h>
#include <xzss.h>
#include <xzsm.h>
#include "ex_mdon.h"

#include	<rtworks/common.h>
#include	<rtworks/ipc.h>

/* definitions de constantes exportees */

#define XZEXC_TRAME_FENETRE_LCR		20

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */
/* declaration de fonctions externes */

extern int EnvoyerTrameDIASER ( 		XZEXT_Trame,
					int,
					char *,
					int,
					int,
					int,
					int);

#endif
