/*E*/
/* Fichier : $Id: epmv_cfg.h,v 1.5 1996/09/30 18:27:42 mismer Exp $      Release : $Revision: 1.5 $        Date : $Date: 1996/09/30 18:27:42 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEPMV * FICHIER epmv_cfg.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*    Module d'initialisation des structures en memoires 
*    de la tache TEPMV
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	07 Sep 1994	: Creation
* Nagiel.E	version 1.2	16 Dec 1994	: Ajout des fonctions de traitement picto et clignotement
* Mismer.D	version 1.3 	11 Mai 1995	: Ajout fonction de reconnexion aux taches protocoles
* Fontaine.C	version 1.4	18 Mai 1995	: Reprise entete
* Mismer.D	version 1.5	13 Sep 1996	: Ajout periode variable en fonction NB eqt (RADT_W)
*
------------------------------------------------------ */

#ifndef epmv_cfg
#define epmv_cfg

/* fichiers inclus */

#include "ex_msok.h"
#include "epmv_don.h"
#include "epmv_cmd.h"


/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */
extern 	T_INT4		vg_SockRT;
extern EPMV_DONNEES_PMV		*pg_debutPMV;
extern EPMV_DONNEES_TYPE_PMV	*pg_debutTypePMV;
extern EPMV_DONNEES_Picto	*pg_debutPicto;
extern EPMV_DONNEES_TYPE_Picto	*pg_debutTypePicto;
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

extern int ec_lire_config_PMV	( XDY_NomMachine );
			  
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
*  Cette fonction effectue la lecture du fichier XDF_Config_Type_PMV_<NomMachine>
*  et met a jour le liste des structure de donnees des types de PMV.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int ec_lire_config_TypePMV	( XDY_NomMachine );

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
*  Cette fonction effectue la lecture du fichier XDF_Config_Picto_<NomMachine>
*  et met a jour le liste des structure de donnees Picto;
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int ec_lire_config_Picto	( XDY_NomMachine );
			  
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
*  Cette fonction effectue la lecture du fichier XDF_Config_Type_Picto_<NomMachine>
*  et met a jour le liste des structure de donnees des types de Picto.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int ec_lire_config_TypePicto	( XDY_NomMachine );

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
