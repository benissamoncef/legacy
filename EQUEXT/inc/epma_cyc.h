/*E*/
/* Fichier : $Id: epma_cyc.h,v 1.1 2005/09/14 15:06:40 gesconf Exp $  Release : $Revision: 1.1 $      Date : $Date: 2005/09/14 15:06:40 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEPMV * FICHIER epma_cyc.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Module de declaration des structures cycliques de 
*   la tache TEPMVA
*
------------------------------------------------------
* HISTORIQUE :
*
* jmg	10/06/05	creation
------------------------------------------------------ */

#ifndef epma_cyc
#define epma_cyc

/* fichiers inclus */
#include	<xdy.h>
#include	<xdc.h>
#include	<xzsc.h>
#include	<xzst.h>

#include	<rtworks/common.h>
#include	<rtworks/ipc.h>

#include 	"ex_msok.h"
#include 	"etim_don.h"
#include 	"epma_don.h"


/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

extern XZEXT_MASK *		vg_SockMask;
extern EPMVA_DONNEES_PMVA		*pg_debutPMVA;

/* delaration de fonctions externes */

#endif
