/*E*/
/* Fichier : $Id: exa0_ctl.h,v 1.2 1995/05/23 18:39:12 cornu Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/05/23 18:39:12 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEXA0 * FICHIER exa0_ctl.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Structur des donnees du fichier de controle de la
* tache TEXA0, protocole de la gtc Nice
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	03 Nov 1994	: Creation
* Fontaine.C	version 1.2	23 Mai 1995	: Reprise entete
*
------------------------------------------------------ */

#ifndef exa0_ctl
#define exa0_ctl

/* fichiers inclus */

#include "exa0_don.h"
#include "exa0_cfg.h"
#include "exa0_cmd.h"
#include "exa0_tra.h"


/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

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

extern void	ea_controle	( 	int 		va_NumSock,
					XDY_NomMachine	pa_NomMachine );

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
*   Si message d'init alors lancer la fonction ea_init
*   Si message d'arret alors lancer la fonction ea_sortir
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

extern void	ea_init	( XDY_NomMachine  	pa_NomMachine,
			  XDY_NomTache		pa_NomTache );

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
*   Lecture des fichier de configuration 
*   Remplissage des structures de donnees
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

extern void	ea_sortir	(  );

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
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Trace des erreur AprilNet
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void ea_gestion_erreur ( errblT *	pa_ErrBlk,
			 	char *		pa_Msg );

/*
* ARGUMENTS EN ENTREE :
*   
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Compte rendu d'ecriture automate
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void ea_ecriture_automate ( char *	pa_CompteRendu );

/*
* ARGUMENTS EN ENTREE :
*   
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
*
------------------------------------------------------*/





















#endif
