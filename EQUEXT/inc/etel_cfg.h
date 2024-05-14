/*E*/
/* Fichier : @(#)etel_cfg.h	1.2      Release : 1.2        Date : 05/04/95
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME EQUEXT
------------------------------------------------------
* MODULE TETEL * FICHIER etel_cfg.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Definition fonctions de configuration et de sortie tache TETEL
*
*
------------------------------------------------------
* HISTORIQUE :
*
* Nouaille.M	Version 1.1	30 Nov 1994	: Creation
* Fontaine.C    Version 1.2     04 Mai 1995     : Reprise entete
* Diaz.L	Version 1.3     09 Oct 1998	: Ajout PABX
------------------------------------------------------ */

#ifndef etel_cfg
#define etel_cfg

/* fichiers inclus */

#include "ex_msok.h"
#include "etel_don.h"


/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */
/* extern T_INT4			vg_SockRT;	*/
/* extern XDY_NomSite        		vg_NomSite;	*/

/* delaration de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction effectue la lecture du fichier XDF_Config_AUTOTEL_<NomMachine>
*  et met a jour le liste des structure de donnees AUTOTEL;
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ec_lire_config_AUTOTEL	( XDY_NomMachine 		pa_NomMachine );
			  
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
*  Cette fonction effectue la lecture du fichier XDF_Config_MAA_<NomMachine>
*  et met a jour le liste des structure de donnees MAA;
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ec_lire_config_MAA	( XDY_NomMachine		pa_NomMachine );
			  
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

void	ec_controle	( 	int 		va_NumSock,
				XDY_NomMachine  pa_NomMachine );

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

void	ec_init	( XDY_NomMachine 	pa_NomMachine );

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
*   Initialisation 
*   Lecture des fichier de configuration des AUTOTEL et des MAA
*   Remplissage des structures de donnees
*   Connexion socket avec la tache protocole TETMi
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

void	ec_sortir	(  );

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
*   Envoyer le message aux taches protocoles TETMi
*   Attendre x secondes
*   Fermeture des sockets
*
------------------------------------------------------*/

#endif
