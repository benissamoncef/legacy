/*E*/
/*Fichier :  $Id: eniw_ctl.c,v 1.2 2002/04/24 08:47:08 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2002/04/24 08:47:08 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TENIW * FICHIER eniw_ctl.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module de gestion cyclique de la tache TENIW, gtc Nice
*
------------------------------------------------------
* HISTORIQUE :
*
* Moulin.D	version 1.1	07 Janv 2002	: Creation
* Moulin.D	version 1.2	23 Avr 2002	: Installation PLate forme
*
------------------------------------------------------*/
 
/* fichiers inclus */
#include <xzsem.h>

#include "eniw_ctl.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: eniw_ctl.c,v 1.2 2002/04/24 08:47:08 gesconf Exp $      : eniw_ctl" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */





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

void	et_controle	( 	int 		va_NumSock,
				XDY_NomMachine	pa_NomMachine )

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
*   Si message d'init alors lancer la fonction et_init
*   Si message d'arret alors lancer la fonction et_sortir
*
------------------------------------------------------*/
{
   static char *version = "$Id: eniw_ctl.c,v 1.2 2002/04/24 08:47:08 gesconf Exp $      : et_controle" ;

   int			vl_IndCnx = 0,
   			vl_LgMsg  = 0;
   XZEXT_MSG_SOCKET	pl_Msg;
   
   
   /*A
    *  Lecture du message recu dans la socket appelante
    */
	 
   if ( ex_lire_msg ( va_NumSock, pl_Msg, &vl_LgMsg, &vg_SockMask, &vl_IndCnx ) != XDC_OK )
   {
      /* Ecriture trace Erreur lecture message socket. */
      XZST_03EcritureTrace( XZSTC_WARNING, " Erreur lecture message socket. " );
   }   
   else
   {
      /*A Si il s'agit d'une deconnexion */
      if ( vl_IndCnx == XZEXC_DECONNEC )
      {
         /*A Arreter la tache par appel de et_sortir */
         et_sortir ();
      }
   
      /* Si le message est correct  */
      if ( vl_LgMsg != 0 ) 
      {
  	  /*A Si il s'agit d'une demande d'arret */
  	  if ( strstr ( pl_Msg, XZEXC_DEMANDE_ARRET ) != NULL )
  	  {
  	     /*A Arreter la tache par appel de et_sortir */
  	     et_sortir ();
  	  }

  	  /*A Si il s'agit d'une demande d'Init */
  	  if ( strstr ( pl_Msg, XZEXC_DEMANDE_INIT ) != NULL )
  	  {
  	     /*A Reconfiguration de la tache par appel de et_init */
  	     et_init ( pa_NomMachine );
  	  }
       }
    }
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Effectue l'initialisation de la tache
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	et_init	( XDY_NomMachine  	pa_NomMachine )

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
{
   static char *version = "$Id: eniw_ctl.c,v 1.2 2002/04/24 08:47:08 gesconf Exp $      : et_init" ;
   XZEXT_MSG_SOCKET 		pl_message;


   /*A
    * Lecture des fichiers de configuration et remplissage des structure de donnees 
    */
   /*A Configuration des tubes et echangeurs (appel de la fonction et_lire_config_GTCNice) */
    et_lire_config_GTCNice  ( pa_NomMachine );
   /*A Configuration des Sequence (appel de la fonction et_lire_config_Sequence ) */
    et_lire_config_Sequence ( pa_NomMachine );
   /*A Configuration des Capteurs et Panneaux (appel de la fonction et_lire_config_Capt_Pan ) */
    et_lire_config_Capt_Pan  ( pa_NomMachine );

   /*A Appel  xzat26_Init_Etats_Eqts pour Initialisation des etats equipement (HS ou Actif) */
   xzat26_Init_Etats_Eqts ( XDC_EQT_TUB, XDC_ETAT_TOUS_EQT, pa_NomMachine );
   xzat26_Init_Etats_Eqts ( XDC_EQT_ECH, XDC_ETAT_TOUS_EQT, pa_NomMachine );
   xzat26_Init_Etats_Eqts ( XDC_EQT_PAN, XDC_ETAT_TOUS_EQT, pa_NomMachine );
   xzat26_Init_Etats_Eqts ( XDC_EQT_CPT, XDC_ETAT_TOUS_EQT, pa_NomMachine );

   /*A
    * Envoie du message de demande d'init donnees 
    */
    
         /* Construction du message d'init */
            sprintf ( pl_message, "%d|1|%s", ENIW_CMD_DEMANDE_INIT, XZEXC_FIN_MSG_SOCK_WIZCON );
         /* Envoie de la demande d'init */
         sem_wait(&vg_semaphore); 
         if ( write ( vg_SockTWWI0, pl_message, strlen (pl_message) ) == XDC_NOK )
         { 
            /* Ecriture trace impossible d'emettre une demande d'arret */
             XZST_03EcritureTrace(XZSTC_WARNING, " Impossible d'emettre une demande d'init vers maitre Wizcon." );
         }
         sem_post(&vg_semaphore); 

         /* Construction du message d'init */
            sprintf ( pl_message, "%d|1|%s", ENIW_CMD_DEMANDE_INIT, XZEXC_FIN_MSG_SOCK_WIZCON );
         /* Envoie de la demande d'init */
         sem_wait(&vg_semaphore); 
         if ( write ( vg_SockTWWI1, pl_message, strlen (pl_message) ) == XDC_NOK )
         { 
            /* Ecriture trace impossible d'emettre une demande d'arret */
             XZST_03EcritureTrace(XZSTC_WARNING, " Impossible d'emettre une demande d'init vers esclave Wizcon." );
         }
         sem_post(&vg_semaphore); 

	/* Programation timer pour relecture flag AprilNet */
	ETIM_REVEIL_CYCLIQUE ( ENICC_REVEIL_15MN, "Nice0_0_0", vg_SockTETIM, pl_message )


}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Effectue les operation de terminaison de la tache
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	et_sortir	(  )

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
{
   static char *version = "$Id: eniw_ctl.c,v 1.2 2002/04/24 08:47:08 gesconf Exp $      : et_sortir" ;

   ENIW_DONNEES_Tube		*pl_DonCrtTube,
   				*pl_DonSupTube;
   ENIW_DONNEES_Echangeur	*pl_DonCrtEch,
   				*pl_DonSupEch;
   XZEXT_MSG_SOCKET 		pl_message;

  /*A
   * Liberation des ressources memoires
   */
   
      /*A Liberation des ressources memoires des donnees Tube */
   
      for ( pl_DonCrtTube = pl_DonSupTube = pg_debutTube;
            pl_DonCrtTube != NULL; 
            pl_DonSupTube = pl_DonCrtTube )
      {
      	 /* Liberation memoire */
      	 et_sup_liste_Sequence ( pl_DonCrtTube->ListeSequence );
      	 et_sup_liste_Panneau  ( pl_DonCrtTube->ListePanneau );
      	 et_sup_liste_Capteur  ( pl_DonCrtTube->ListeCapteur );
         pl_DonCrtTube = pl_DonCrtTube->Suivant;
         free ( pl_DonSupTube );
      }
      pg_debutTube = NULL;
 
      /*A Suppression des structures de donnees Echangeur */
      for ( pl_DonCrtEch = pl_DonSupEch = pg_debutEchangeur;
            pl_DonCrtEch != NULL; 
            pl_DonSupEch = pl_DonCrtEch )
      {
      	 /* Liberation memoire */
      	 et_sup_liste_Sequence ( pl_DonCrtTube->ListeSequence );
      	 et_sup_liste_Panneau  ( pl_DonCrtEch->ListePanneau );
         pl_DonCrtEch = pl_DonCrtEch->Suivant;
         free ( pl_DonSupEch );
      }
      pg_debutEchangeur = NULL;
      

   /*A
    * Envoie du message d'arret aux taches protocoles 
    */
    
         /* Construction du message d'arret */
            sprintf ( pl_message, "%d|1|%s", ENIW_CMD_DEMANDE_ARRET, XZEXC_FIN_MSG_SOCK_WIZCON );
         /* Envoie de la demande d'arret */
         sem_wait(&vg_semaphore); 
         if ( write ( vg_SockTWWI0, pl_message, strlen (pl_message) ) == XDC_NOK )
         { 
            /* Ecriture trace impossible d'emettre une demande d'arret */
             XZST_03EcritureTrace(XZSTC_WARNING, " Impossible d'emettre une demande d'arret vers maitre Wizcon ." );
         }
         sem_post(&vg_semaphore); 

         /* Construction du message d'arret */
            sprintf ( pl_message, "%d|1|%s", ENIW_CMD_DEMANDE_ARRET, XZEXC_FIN_MSG_SOCK_WIZCON );
         /* Envoie de la demande d'arret */
         sem_wait(&vg_semaphore); 
         if ( write ( vg_SockTWWI1, pl_message, strlen (pl_message) ) == XDC_NOK )
         { 
            /* Ecriture trace impossible d'emettre une demande d'arret */
             XZST_03EcritureTrace(XZSTC_WARNING, " Impossible d'emettre une demande d'arret vers esclave Wizcon ." );
         }
         sem_post(&vg_semaphore); 

   /*A Attendre */
   sleep (XZEXC_ATTENTE_ARRET_TACHE);
   
   
   /* Fin de la tache */
   exit (0);
}
