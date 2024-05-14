/*E*/
/* Fichier : $Id: eope_cfg.h,v 1.2 1995/05/04 20:14:12 cornu Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/05/04 20:14:12 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME EQUEXT
------------------------------------------------------
* MODULE TEOPE * FICHIER eope_cfg.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Definition fonctions de configuration et de sortie tache TEOPE
*
------------------------------------------------------
* HISTORIQUE :
*
* Nouaille.M	Version 1.1	02 Dec 1994	: Creation
* Fontaine.C    Version 1.2     04 Mai 1995     : Reprise entete
------------------------------------------------------ */

#ifndef eope_cfg
#define eope_cfg

/* fichiers inclus */

#include "ex_msok.h"
#include "eope_don.h"


/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* delaration de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction effectue la lecture du fichier XDF_Config_OPER_<NomMachine>
*  et met a jour le liste des structure de donnees OPERATOR;
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ec_lire_config_OPER	( XDY_NomMachine		pa_NomMachine );
			  
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
*   Lecture des fichier de configuration des OPERATOR
*   Remplissage des structures de donnees
*   Connexion socket avec la tache protocole TEOMi
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
*   Envoyer le message aux taches protocoles TEOMi
*   Attendre x secondes
*   Fermeture des sockets
*
------------------------------------------------------*/

#endif
