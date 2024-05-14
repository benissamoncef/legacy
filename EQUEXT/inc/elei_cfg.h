/*E Fichier : $Id: elei_cfg.h,v 1.3 1995/05/23 09:45:41 cornu Exp $      Release : $Revision: 1.3 $        Date : $Date: 1995/05/23 09:45:41 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TELEi * FICHIER elei_cfg.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier de configuration de 
*  la tache TELEi.
*
------------------------------------------------------
* HISTORIQUE :
*
* Flaidar.A	version 1.1	07 Nov 1994	: Creation
* Flaidar.A	version 1.2	02 Dec 1994	:
* Fontaine.C	version 1.3	23 Mai 1995	: Reprise entete, historique
*
------------------------------------------------------ */

#ifndef elei_cfg
#define elei_cfg

/* fichiers inclus */

#include "elei_mdon.h"
#include "ex_msok.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* declaration de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Sortie de la tache telei.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*  void 	ecfg_sortir( void )
*
*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
*  Liberation ressources memoire.
*  Fermeture sockets.
*
------------------------------------------------------*/
extern void 	ecfg_sortir( void );


#endif
