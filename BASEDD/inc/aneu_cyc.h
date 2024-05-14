/*E*/
/*  Fichier : $Id: aneu_cyc.h,v 1.2 2015/08/11 15:38:53 devgfi Exp $      Release : $Revision: 1.2 $        Date : $Date: 2015/08/11 15:38:53 $
-------------------------------------------------------------------------------------------
* ESCOTA  *  PROJET MIGRAZUR / PASTRE
-------------------------------------------------------------------------------------------
* SOUS-SYSTEME : BASEDD
-------------------------------------------------------------------------------------------
* MODULE TANEU * FICHIER aneu_cyc.h
-------------------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Interface externe du module des traitements cycliques de la tache TANEU.
*
-------------------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	28/01/13 : Creation (DEM 1061)  1.1
* NDE	11/08/15 : Nouvelle constante (DEM 1135) 1.2
*
-----------------------------------------------------------------------------------------*/

/* Definitions de constantes exportees */
/* ----------------------------------- */

#define		ANEU_PERIODE_MISE_AU_NEUTRE	6
#define		ANEU_PERIODE_PREMIERE_MIN	1




/* Declaration de fonctions externes */
/* --------------------------------- */

extern int	aneu_cyc_lancement_timer ( void );

extern int	aneu_cyc_arret_timer ( void );
