/*E*/
/* Fichier : $Id: evid_cfg.h,v 1.5 2017/02/28 14:50:04 devgfi Exp $      Release : $Revision: 1.5 $        Date : $Date: 2017/02/28 14:50:04 $
------------------------------------------------------
* GRANIOU *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEVID * FICHIER evid_cfg.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Structure des donnees du fichier de configuration de
* la tache VIDEO
*
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E	version 1.1	31 Oct 1994	: Creation
* Nagiel.E	version 1.2	26 Dec 1994	:
* Volcic.F	version 1.3	24 Jan 1995	:
* Fontaine.C	version 1.4	23 Mai 1995	: Reprise entete, historique
* JPL		09/11/16 : Ajout fonction ec_message_operateurs (DEM 1189)  1.5
*
------------------------------------------------------ */

#ifndef evid_cfg
#define evid_cfg

/* fichiers inclus */

#include "ex_msok.h"
#include "evid_don.h"


/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */
extern T_INT4			vg_SockRT;
extern XZEXT_MASK *		vg_SockMask;
extern XDY_NomSite        	vg_NomSite;

/* delaration de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction effectue la lecture du fichier XDF_Config_VIDEO_<NomMachine>
*  et met a jour le liste des structure de donnees VIDEO;
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int ec_lire_config_VIDEO	( XDY_NomMachine		pa_NomMachine );
			  
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
*  Cette fonction effectue la lecture du fichier XDF_Config_SYSVIDEO_<NomMachine>
*  et met a jour le liste des structure de donnees VIDEO;
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int ec_lire_config_SYSVIDEO	( XDY_NomMachine		pa_NomMachine );
			  
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

extern void	ec_controle	( 	int 		va_NumSock,
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

extern void	ec_init	( XDY_NomMachine 	pa_NomMachine );

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


/*X*/
/*------------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Envoi d'un message d'avertissement aux operateurs du site local.
*
--------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int	ec_message_operateurs (
	char		*pa_message,
	int		va_num_sysV);

/*
* ARGUMENTS EN ENTREE :
*   pa_message		: Texte du message.
*   va_num_sysV		: Identifiant de l'exemplaire du Systeme video emetteur.
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   XDC_OK en cas de succes, XDC_NOK sinon.
*
* CONDITION D'UTILISATION
*  Toutes fonctions.
*
* FONCTION 
*  Emet le message indique a destination des postes du site local.
*
------------------------------------------------------------------------------*/

#endif
