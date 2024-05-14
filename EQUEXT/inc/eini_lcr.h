/*E*/
/* Fichier : $Id: eini_lcr.h,v 1.6 2011/03/17 11:29:33 gesconf Exp $      Release : $Revision: 1.6 $        Date : $Date: 2011/03/17 11:29:33 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEINI * FICHIER eini_lcr.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Ce module gere la creation des fichiers de config pour la fenetre LCR.
*  
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	13 Dec 1994	: Creation
* Mismer.D	version 1.2	09 Fev 1995	:
* Fontaine.C	version 1.3	19 Mai 1995	: Reprise entete, historique
* Mismer.D      version 1.4     06 Sep 1996     : Modif pour Extension Ouest   (RADT_W)
*
* JPL		17/03/2011 : Migration architecture HP ia64 (DEM 975) : declaration "ec_config_eqt_lcr"  1.6
------------------------------------------------------ */

#ifndef eini_lcr
#define eini_lcr

/* fichiers inclus */

#include "eini_don.h"

/* definitions de constantes exportees */

/* definitions de types exportes */


/* definition de macro exportees */


/* declarations de variables globales */

extern	XDY_Fichier             vg_FdConf[6];
extern  EINI_DONNEES_FEN_LCR *	pg_DebMacLCR;
extern  EINI_MACHINE *          pg_MachineCrt;

/* delaration de fonctions externes */
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction charge de constituer les fichier de configuration relatif a l'equipement
*  identifie par le numero d'equipement, pour une ou toutes les machines
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void	ec_config_lcr (	int		va_NumEqt );

/*
* ARGUMENTS EN ENTREE :
*   int			va_NumEqt
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
*  Recuperation en base avec XZAO125 de la liste des equipement par type
*
------------------------------------------------------*/


/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int  ec_mise_a_jour_lcr ( XDY_Eqt	va_TypeEqt,
			   	  XDY_Eqt	va_NumEqt );

/*
* ARGUMENTS EN ENTREE :
*   
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   rien 
*
* FONCTION 
*   
*
------------------------------------------------------*/



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction charge de constituer les fichier de configuration relatif a l'equipement
*  identifie par le numero d'equipement, pour une ou toutes les machines
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void	ec_fichier_lcr ( );

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
*
------------------------------------------------------*/



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction charge de constituer les fichier de configuration relatif a l'equipement
*  identifie par le numero d'equipement, pour une ou toutes les machines
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void	ec_config_eqt_lcr (	XDY_NomMachine,
					int	);

/*
* ARGUMENTS EN ENTREE :
*   XDY_NomMachine	pa_NomMachine
*   int			va_TypeEqt
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
*   Detruire les anciens fichiers
*   Creer les fichiers de config
*   Lire en base de donnees les parametres de configuration du type d'equipement concerne
*   Ecrire les donnees dans le fichier de configuration
*   Fermer les fichiers de configuration
*
------------------------------------------------------*/


#endif
