/*E*/
/*  Fichier : $Id: esrv_art.c,v 1.9 1995/08/25 10:45:38 volcic Exp $      Release : $Revision: 1.9 $        Date : $Date: 1995/08/25 10:45:38 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TESRV * FICHIER esrv_art.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module de gestion des fonction de commandes des taches equipements >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	23 Aug 1994	: Creation
* Mismer.D	version 1.2	28 Oct 1994	:
* Mismer.D	version 1.3	28 Nov 1994	: Ajout fonction diffusion changement de mode
* Mismer.D	version 1.4	09 Dec 1994	:
* Mismer.D	version 1.5	25 Jan 1995	:
* Fontaine.C	version 1.6	22 Mai 1995	: Reprise historique
* Mismer.D	version 1.7	06 Jun 1995	: Modif diffusion mode de fonctionnement (ISOLE ou NORMAL)
* Mismer.D	version 1.8	26 Jun 1995	: Modif arret tache traitement 
* Mismer.D	version 1.9	06 Jul 1995	: Modif fonction ee_diff_chgt_mode
*
------------------------------------------------------*/

/* fichiers inclus */

#include <xzsem.h>
#include <xdc.h>
#include <xdy.h>
#include <xzst.h>

#include "ex_msok.h"
#include "esrv_art.h"
#include "esrv_cfg.h"
#include "esrv_ctl.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: esrv_art.c,v 1.9 1995/08/25 10:45:38 volcic Exp $ : esrv_art" ;

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
static char *version = "$Id: esrv_art.c,v 1.9 1995/08/25 10:45:38 volcic Exp $ : ee_config " ;
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


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Diffusion vers les tache client de tesrv du changement ( mode isole ou normal )
*  provenant de TEMOD
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ee_diff_chgt_mode ( char *pa_Msg, int  va_Socket )	      


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
{
static char *version = "$Id: esrv_art.c,v 1.9 1995/08/25 10:45:38 volcic Exp $ : ee_diff_chgt_mode " ;
XDY_NomTache		vl_NomTache = "";
XZEXT_MASK *		pl_masque;
XZEXT_MSG_SOCKET	vl_Msg = "";


   strcpy ( vl_Msg, pa_Msg );
   /* Recherche si il s'agit d'un message de TEMOD */
   for ( pl_masque = vg_SockMask; pl_masque != NULL; pl_masque = pl_masque->Suivant )
      if ( pl_masque->desc_sock == va_Socket )
         if ( !strcmp( pl_masque->adresse, XZEXC_ADR_SOCK_TEMOD) )
            break;
         else
            return;

   if ( strstr( pa_Msg, XZEXC_DEMANDE_ARRET ) != NULL )
     strcpy ( vl_NomTache, strtok (pa_Msg+strlen(XZEXC_DEMANDE_ARRET), " ") );

   /*A Pour touche les sockets de la liste */
   for ( pl_masque = vg_SockMask; pl_masque != NULL; pl_masque = pl_masque->Suivant )
   {
      /*A Si Socket tache concernee trouvee*/
      if ( ( pl_masque->Type == AF_UNIX ) &&
           ( pl_masque->desc_sock != vg_SockEct ) &&
           ( pl_masque->desc_sock != vg_SockTETIM ) &&
           ( pl_masque->desc_sock != va_Socket ) &&
           ( strcmp (pl_masque->adresse, XZEXC_ADR_SOCK_TETIM) ) )
      {
         /*A Envoie changement de mode */
         if ( (vl_NomTache[0] == '\0') || 
              !strncmp (pl_masque->adresse, vl_NomTache, strlen(pl_masque->adresse)) )
         {
            sem_wait(&vg_semaphore); 
            if ( write ( pl_masque->desc_sock, vl_Msg, strlen (vl_Msg) ) <= 0 )
               XZST_03EcritureTrace(XZSTC_WARNING, " Impossible d'emettre un changement de mode." );
            sem_post(&vg_semaphore); 
         }
      }   
   }   	 

}
