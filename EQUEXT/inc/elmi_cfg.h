/*E*/
/* Fichier : $Id: elmi_cfg.h,v 1.2 1995/05/23 09:45:45 cornu Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/05/23 09:45:45 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TELMi * FICHIER elmi_cfg.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Structure des donnees du fichier de configuration de
*   la tache TELMi
* 
------------------------------------------------------
* HISTORIQUE :
*
* Flaidar.A	version 1.1	25 Oct 1994	: Creation
* Fontaine.C	version 1.2	23 Mai 1995	: Reprise entete
*
------------------------------------------------------ */

#ifndef elmi_cfg
#define elmi_cfg

/* fichiers inclus */

#include "elmi_mdon.h"
#include "ex_msok.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Sortie de la tache TELMi
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
