/*E*/
/*Fichier : $Id: evid_cyc.h,v 1.5 2017/02/28 14:50:05 devgfi Exp $       Release : $Revision: 1.5 $     Date : $Date: 2017/02/28 14:50:05 $
------------------------------------------------------
* ESCOTA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEVID * FICHIER evid_cyc.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier gerant les demandes
*  cycliques de la tache VIDEO.
*
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E	version 1.1	23 Dec 1994	: Creation
* Nagiel.E	version 1.2	17 Jan 1995	:
* Volcic.F	version 1.3	24 Jan 1995	:
* Fontaine.C	version 1.4	23 Mai 1995	: Reprise entete, historique
* JPL		02/11/16	: Declaration de la fonction ey_traitement_cyclique (DEM 1189)  1.5
*
------------------------------------------------------ */

#ifndef evid_cyc
#define evid_cyc

/* fichiers inclus */
#include 	"evid_don.h"

/* definitions de constantes exportees */
#define EVIDC_PERIODE_CNX	2		/* Periode de reconnexion, en minutes */


/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */


/*X*/
/*------------------------------------------------------------------------------
* SERVICE RENDU :
*
*   Gestion des traitements cycliques de la tache.
--------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern	int ey_traitement_cyclique ( int va_NumSock );

/*
* ARGUMENTS EN ENTREE :
*   va_NumSock        : Socket de provenance du signal de declenchement.
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
*
* CONDITION D'UTILISATION
*
* FONCTION
*
------------------------------------------------------------------------------*/

#endif
