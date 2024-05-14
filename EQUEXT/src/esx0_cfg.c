/*E*/
/*Fichier : $Id: esx0_cfg.c,v 1.5 1995/06/19 14:57:40 volcic Exp $      Release : $Revision: 1.5 $        Date : $Date: 1995/06/19 14:57:40 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TESX0 * FICHIER esx0_cfg.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Module de configuration de la tache TESX0, protocole 
*   de la tache TESAT
*
------------------------------------------------------
* HISTORIQUE :
*
* Gabert.P	version 1.1	21 Dec 1994	: Creation
* Mismer.D	version 1.2	26 Jan 1995	:
* Volcic.F	version 1.3	13 Fev 1995	: 
* Fontaine.C	version 1.4	23 Mai 1995	: Reprise entete, historique
* Volcic.F	version 1.5	02 Jun 1995	: Optimisation
*
------------------------------------------------------*/

/* fichiers inclus */

#include "esx0_cfg.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: esx0_cfg.c,v 1.5 1995/06/19 14:57:40 volcic Exp $ : epmv_cfg" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Effectue les operation de terminaison de la tache
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ec_sortir	(  )

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
{
   static char *version = "$Id: esx0_cfg.c,v 1.5 1995/06/19 14:57:40 volcic Exp $ : ec_sortir" ;

   XZEXT_MASK *			pl_Socket;

   /*A
    * Fermeture de toutes les sockets 
    */
	 
   /* Pour toute les sockets du masque */
   for ( pl_Socket = vg_SockMask; pl_Socket != NULL; pl_Socket = pl_Socket->Suivant )
   {
      /* Si differente de la socket RTserver */
      if ( pl_Socket->desc_sock != vg_SockRT ) 
      {
         /* Fermeture de la socket courante */
         ex_fin_cnx ( pl_Socket->desc_sock, &vg_SockMask );
      }   
   }

   /* Fin de la tache */
   exit (0);
}
