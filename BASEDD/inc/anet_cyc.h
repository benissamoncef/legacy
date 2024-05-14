/*E*/
/*  Fichier : $Id: anet_cyc.h,v 1.1 2017/04/24 10:49:03 devgfi Exp $      Release : $Revision: 1.1 $        Date : $Date: 2017/04/24 10:49:03 $
-------------------------------------------------------------------------------------------
* ESCOTA  *  PROJET MIGRAZUR
-------------------------------------------------------------------------------------------
* SOUS-SYSTEME : BASEDD
-------------------------------------------------------------------------------------------
* MODULE ANET * FICHIER anet_cyc.h
-------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Interface externe du module des traitements cycliques du composant ANET.
*
-------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	03/04/17 : Creation (DEM 1221)  1.1
*
-----------------------------------------------------------------------------------------*/

#include	<xzat01.h>


/* Definitions de constantes exportees */
/* ----------------------------------- */

#define		ANET_PERIODE_MISE_AU_NEUTRE	6
#define		ANET_PERIODE_PREMIERE_MIN	2




/* Declaration de fonctions externes */
/* --------------------------------- */

extern int	anet_cyc_lancement_timer ( void );

extern int	anet_cyc_arret_timer ( void );


extern int	anet_MAJ_etat_PMV_SAGA ( int, int, XZATT_EtatEqt );
