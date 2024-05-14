/*E*/
/* Fichier : $Id: eomi_cfg.h,v 1.1 1995/01/04 21:49:17 nouaille Exp $  Release : $Revision: 1.1 $        Date : 11/29/94
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME EQUEXT
------------------------------------------------------
* MODULE TEOMi * FICHIER eomi_cfg.h
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
------------------------------------------------------ */

#ifndef eomi_cfg
#define eomi_cfg

/* fichiers inclus */

#include "eomi_common.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Sortie de la tache TEOMi
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
