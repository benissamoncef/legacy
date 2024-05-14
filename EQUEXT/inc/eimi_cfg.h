/*E*/
/* Fichier : $Id: eimi_cfg.h,v 1.1 2019/01/29 11:05:51 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2019/01/29 11:05:51 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TELMi * FICHIER eimi_cfg.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Structure des donnees du fichier de configuration de
*   la tache TELMi
* 
------------------------------------------------------
* HISTORIQUE :
*
* JMG   01/03/18 :      Creation CTRL_FEUX 1.1 DEM1284
*
------------------------------------------------------ */

#ifndef eimi_cfg
#define eimi_cfg

/* fichiers inclus */

#include "eimi_mdon.h"
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
