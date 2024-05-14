/*E*/
/* Fichier : $Id: etmi_cfg.h,v 1.2 1995/05/04 20:14:25 cornu Exp $  Release : $Revision: 1.2 $        Date : 11/29/94
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME EQUEXT
------------------------------------------------------
* MODULE TETMi * FICHIER etmi_cfg.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Definition fonctions de configuration et de sortie tache TETMi
* 
*
------------------------------------------------------
* HISTORIQUE :
*
* Nouaille M.	Version 1.1	02 Dec 1994	: Creation
* Fontaine C.   Version 1.2     03 Mai 1995     : Reprise entete
------------------------------------------------------ */

#ifndef etmi_cfg
#define etmi_cfg

/* fichiers inclus */

#include "etmi_common.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Sortie de la tache TETMi
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* void ecfg_sortir( void )
*
*
* ARGUMENTS EN ENTREE :
*   
*
* ARGUMENTS EN SORTIE :
*   
*
* CODE RETOUR : 
*   
*   NEANT
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   Liberation ressources memoire.
*   Fermeture sockets.
*   
------------------------------------------------------*/
extern void ecfg_sortir( void );

#endif
