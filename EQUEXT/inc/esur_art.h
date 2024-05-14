/*X Fichier : $Id: esur_art.h,v 1.2 1995/05/22 19:18:18 cornu Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/05/22 19:18:18 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TESUR * FICHIER esur_art.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Structure des donnees du fichier gerant les fonctions de 
* commande de la tache TESUR
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	23 Aug 1994	: Creation
* Fontaine.C	version 1.2	22 Mai 1995	: Reprise entete
*
------------------------------------------------------ */

#ifndef esur_art
#define esur_art

/* fichiers inclus */
#include "ex_msok.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Envoi des commandes d'arret ou de reconfiguration aux taches
*  equipement ou temporaire ou a TETIM
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int ee_config ( char *pa_MsgType , int va_NumSock , char *pa_NomTache );	      


/*
* ARGUMENTS EN ENTREE :
*   pa_MsgType		: Entete du message a transmettre ( XZEXC_DEMANDE_ARRET, XZEXC_DEMANDE_RECONFIG )
*   va_NumSock		: Index dans le masque des sockets pour identite tache destinataire
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*   XDC_OK	succes
*   XDC_NOK	echec
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   Creation d'un message de controle de l'equipement et envoi du message
*   a la tache correspondante
*
------------------------------------------------------*/
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Diffusion vers les tache client de tesrv du changement ( mode isole ou normal )
*  provenant de TEMOD
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void ee_diff_chgt_mode ( char * pa_Msg );	      


/*
* ARGUMENTS EN ENTREE :
*   pa_MsgType		: message a transmettre ( XZEXC_MODE_FONCT_NORMAL, XZEXC_MODE_FONCT_ISOLE)  )
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*   XDC_OK	succes
*   XDC_NOK	echec
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   Envoi du changement de mode a toutes les taches traitements
*
------------------------------------------------------*/

#endif
