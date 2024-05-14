/*E*/
/*  Fichier : $Id: esur_art.c,v 1.3 1995/05/22 19:11:03 cornu Exp $      Release : $Revision: 1.3 $        Date : $Date: 1995/05/22 19:11:03 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TESUR * FICHIER esur_art.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module de gestion des fonction de commandes des taches equipements >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	23 Aug 1994	: Creation
* Mismer.D	version 1.2	25 Jan 1995	:
* Fontaine.C	version 1.3	22 Mai 1995	: Reprise historique
*
------------------------------------------------------*/

/* fichiers inclus */

#include <xzsem.h>
#include <xdc.h>
#include <xdy.h>
#include <xzst.h>

#include "ex_msok.h"
#include "esur_art.h"
#include "esur_cfg.h"
#include "esur_ctl.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: esur_art.c,v 1.3 1995/05/22 19:11:03 cornu Exp $ : esur_art" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Envoi des commandes d'arret ou de reconfiguration aux taches
*  equipement ou temporaire ou a TETIM
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ee_config ( char *pa_MsgType , int va_NumSock , char *pa_NomTache )	      


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
{
static char *version = "$Id: esur_art.c,v 1.3 1995/05/22 19:11:03 cornu Exp $ : ee_config " ;
XZEXT_MSG_SOCKET pl_message;

   /*A Construction du message */
   sprintf ( pl_message, "%s %s %s", pa_MsgType, pa_NomTache, XZEXC_FIN_MSG_SOCK );
   XZST_03EcritureTrace(XZSTC_DEBUG1, " Demande %s a %s ." , pa_MsgType, pa_NomTache );
   /*A Envoie de la demande d'arret */
   sem_wait(&vg_semaphore); 
   if ( write ( va_NumSock, pl_message, strlen (pl_message) ) <= 0 )
   { 
      /*A Ecriture trace impossible d'emettre une demande d'arret */
      XZST_03EcritureTrace(XZSTC_WARNING, " Impossible d'emettre une demande d'arret ou de de reconfig." );
   }
   sem_post(&vg_semaphore); 
        
   return ( XDC_OK );

}


