/*E*/
/*  Fichier : $Id: erau_cfg.h,v 1.4 2016/09/29 15:18:47 pc2dpdy Exp $      Release : $Revision: 1.4 $        Date : $Date: 2016/09/29 15:18:47 $
------------------------------------------------------
* GRANIOU *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TERAU * FICHIER erau_cfg.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Structur des donnees du fichier de config de la tache TERAU
*
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E	version 1.1	30 Nov 1994	: Creation
* Nagiel.E	version 1.2	24 Jan 1995	: 
* Fontaine.C	Version 1.3	22 Mai 1995	: Reprise entete, historique
* JMG		version 1.4	28/06/16	: zones de basculement RAU
------------------------------------------------------ */

#ifndef erau_cfg
#define erau_cfg

/* fichiers inclus */

#include "ex_msok.h"
#include "erau_don.h"
#include "xzao910.h"

/* definitions de constantes exportees */

/* definitions de types exportes */
typedef struct {
	char	zone[50];
	char	libelle[50];
	char 	etat[10];
	XDY_District	site;
	int	compteur;
} T_ZONE;

typedef struct S_T_ZONE {
	T_ZONE		zone;
	struct S_T_ZONE	*suivant;
} T_Liste_Zones;
/* definition de macro exportees */

/* declarations de donnees globales */
extern 	T_INT4		vg_SockRT;
extern 	XZEXT_MASK *	vg_SockMask;

/* delaration de fonctions externes */

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

int	ec_init	( XDY_NomMachine 	pa_NomMachine );

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
*   Envoyer le message aux taches protocoles
*   Attendre x secondes
*   Fermeture des sockets
*
------------------------------------------------------*/


void erau_basc_rau_cb(T_IPC_CONN                      va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
	                T_CB_ARG                        pa_ARG);
#endif
