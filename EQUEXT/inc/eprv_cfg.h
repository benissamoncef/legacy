/*E*/
/* Fichier : $Id: eprv_cfg.h,v 1.1 2012/07/04 14:27:40 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 14:27:40 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE Tepma * FICHIER eprv_cfg.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*    Module d'initialisation des structures en memoires 
*    de la tache Tepma
*
------------------------------------------------------
* HISTORIQUE :
*
* VR	19/12/11		: Creation (DEM/1016)
------------------------------------------------------ */

#ifndef epma_prv
#define epma_prv

/* fichiers inclus */

#include "ex_msok.h"
#include "eprv_don.h"
#include "eprv_cmd.h"


/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */
extern 	T_INT4		vg_SockRT;
extern EPRV_DONNEES_PRV		*pg_debutPRV;
extern XDY_NomSite        	vg_NomSite;
extern char			vg_idpassw[18];

/* delaration de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction effectue la lecture du fichier XDF_Config_PMV_<NomMachine>
*  et met a jour le liste des structure de donnees PMV;
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int ec_lire_config_PRV	( XDY_NomMachine );
			  
/*
* ARGUMENTS EN ENTREE :
*   pa_NomMachine	: Nom de la machine
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK	succes
*   XDC_NOK	echec
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet selon message socket recu de tesrv 
*    - soit de terminer la tache
*    - soit de configurer la tache
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void	ec_controle	( 	int, XDY_NomMachine );

/*
* ARGUMENTS EN ENTREE :
*   va_NumSock		: Numero de la socket de TESRV
*   pa_NomMachine	: Nom de la machine
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*
* FONCTION 
*   Lecture du message socket
*   Si message d'init alors lancer la fonction ec_init
*   Si message d'arret alors lancer la fonction ec_sortir
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Effectue l'initialisation de la tache
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int	ec_init	( XDY_NomMachine , XDY_Mot * );

/*
* ARGUMENTS EN ENTREE :
*
*   pa_NomMachine	: Nom de la machine
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*
* FONCTION 
*   Appel de ex_mini
*   Lecture des fichier de configuration des PMV et des Picto
*   Remplissage des structures de donnees
*   Connexion socket avec la tache protocole
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Effectue la connection avec avec la tache protocole
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void	ec_init_cnx	( );

/*
* ARGUMENTS EN ENTREE :
*
*  aucun
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*
* FONCTION 
*   Connexion socket avec la tache protocole
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Effectue les operation de terminaison de la tache
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void	ec_sortir	(  );

/*
* ARGUMENTS EN ENTREE :
*   aucun
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*
* FONCTION 
*   Liberation des ressources memoires
*   Envoyer le message aux taches protocoles
*   Attendre x secondes
*   Fermeture des sockets
*
------------------------------------------------------*/

#endif
