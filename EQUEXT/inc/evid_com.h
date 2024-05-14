/*E*/
/* Fichier : $Id: evid_com.h,v 1.1 2017/02/08 18:31:59 devgfi Exp $      Release : $Revision: 1.1 $        Date : $Date: 2017/02/08 18:31:59 $
--------------------------------------------------------------------------------
* ESCOTA *  PROJET MIGRAZUR
--------------------------------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
--------------------------------------------------------------------------------
* MODULE TEVID * FICHIER evid_com.h
--------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Fonctions de communication de la tache VIDEO.
*
--------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	02/11/16	: Creation pour communication IP (DEM 1189)  1.1
*
------------------------------------------------------------------------------*/

#ifndef evid_com
#define evid_com

/* fichiers inclus */

#include "ex_msok.h"
#include "evid_don.h"


/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */


/*X*/
/*------------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Connexion a l'un des points d'acces du Systeme video.
*
--------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int	ec_sysv_connecter ( int  pa_NumSysVideo );

/*
* ARGUMENTS EN ENTREE :
*
*   pa_NumSysVideo	: Identifiant de l'exemplaire a connecter.
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_ARG_INV si un parametre est invalide.
*   XDC_OK en cas de succes, XDC_NOK sinon.
*
* CONDITION D'UTILISATION
*
* FONCTION
*   Si le point d'acces est configure et si la connexion n'est pas etablie
*   alors tenter la connexion au serveur et en cas de succes,
*   mettre a jour les donnees de liaison.
*
------------------------------------------------------------------------------*/





/*X*/
/*------------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Purger les donnees eventuelles en provenance du Systeme video.
*
--------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int	ec_sysv_purger ( int  va_NumSysVideo );

/*
* ARGUMENTS EN ENTREE :
*
*   va_NumSysVideo	: Identifiant de l'exemplaire a purger.
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_ARG_INV si un parametre est invalide.
*   XDC_OK en cas de succes, XDC_NOK sinon.
*
* CONDITION D'UTILISATION
*
* FONCTION
*   Lit toutes donnees en arrivee (avec un delai maximal) sur le canal
*   de communication avec le serveur, si la liaison est etablie.
*
------------------------------------------------------------------------------*/





/*X*/
/*------------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Ecriture d'un message vers un point d'acces au Systeme video.
*
--------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int	ec_sysv_ecrire ( int va_NumSysVideo, char *pa_Message, int va_LgrMsg );

/*
* ARGUMENTS EN ENTREE :
*
*   va_NumSysVideo	: Identifiant de l'exemplaire a qui envoyer le message ;
*   va_Message		: Donnees du message ;
*   va_LgrMsg		: Longueur du message.
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_ARG_INV si un parametre est invalide.
*   XDC_OK en cas de succes, XDC_NOK sinon.
*
* CONDITION D'UTILISATION
*
* FONCTION
*   Ecrit les donnees sur le canal de communication avec le Systeme video
*   jusque envoi complet du message ou erreur d'ecriture.
*
------------------------------------------------------------------------------*/


#endif
