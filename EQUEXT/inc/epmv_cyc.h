/*E*/
/* Fichier : $Id: epmv_cyc.h,v 1.2 1995/05/18 12:10:58 cornu Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/05/18 12:10:58 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEPMV * FICHIER epmv_cyc.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Module de declaration des structures cycliques de 
*   la tache TEPMV
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	02 Sep 1994	: Creation
* Fontaine.C	version 1.2	18 Mai 1995	: Reprise entete
------------------------------------------------------ */

#ifndef epmv_cyc
#define epmv_cyc

/* fichiers inclus */
#include	<xdy.h>
#include	<xdc.h>
#include	<xzsc.h>
#include	<xzst.h>

#include	<rtworks/common.h>
#include	<rtworks/ipc.h>

#include 	"ex_msok.h"
#include 	"etim_don.h"
#include 	"epmv_don.h"


/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

extern XZEXT_MASK *		vg_SockMask;
extern EPMV_DONNEES_PMV		*pg_debutPMV;
extern EPMV_DONNEES_TYPE_PMV	*pg_debutTypePMV;
extern EPMV_DONNEES_Picto	*pg_debutPicto;
extern EPMV_DONNEES_TYPE_Picto	*pg_debutTypePicto;

/* delaration de fonctions externes */

#endif
