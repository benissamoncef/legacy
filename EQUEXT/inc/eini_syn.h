/*E*/
/* Fichier : $Id: eini_syn.h,v 1.5 2012/02/15 12:33:57 gesconf Exp $      Release : $Revision: 1.5 $        Date : $Date: 2012/02/15 12:33:57 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEINI * FICHIER eini_syn.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Ce module gere les structures de syn nees de la tache TEINI.
*  La variable globale pg_ListeMachine contient le pointeur de debut de liste.
*  
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	31 Aug 1994	: Creation
* Mismer.D	version 1.2	30 Jan 1995	:
* Mismer.D	version 1.3	09 Fev 1995	:
* Fontaine.C	version 1.4	19 Mai 1995	: Reprise entete, historique
* JPL	15/02/12 : Declaration ei_diffusion_fichier_standard, ei_diffusion_fichier_lcr (avec DEM 1018)  1.5
------------------------------------------------------ */

#ifndef eini_syn
#define eini_syn

/* fichiers inclus */

#include "eini_don.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */


/* declarations de synnees globales */

extern	EINI_MACHINE *		pg_ListeMachine;
extern	EINI_MACHINE *		pg_MachineCrt;
extern 	EINI_DONNEES_FEN_LCR *	pg_DebMacLCR;

/* delaration de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ec_diffusion_fichier ( 	XDY_NomMachine	pa_NomMachine,
				int		va_TypeMachine,
				int		va_TypeEqt );

/*
* ARGUMENTS EN ENTREE :
*   XDY_NomMachine	pa_NomMachine	nom machine a configurer
*   int			va_TypeMachine	Type de la machine (principale ou supleante)
*   int			va_TypeEqt 	type d'equipement a configurer
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   Diffussion des fichiers de configuration vers les machines concernees
*
------------------------------------------------------*/



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ei_diffusion_fichier_standard ( void );

/*
* ARGUMENTS EN ENTREE :
*   Aucun
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   Diffussion des fichiers de configuration vers les machines concernees
*
------------------------------------------------------*/



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ei_diffusion_fichier_lcr ( void );

/*
* ARGUMENTS EN ENTREE :
*   Aucun
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   Diffussion des fichiers de configuration vers les machines concernees
*
------------------------------------------------------*/

#endif
