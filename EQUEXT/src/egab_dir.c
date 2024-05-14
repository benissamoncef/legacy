/* Fichier : egab_dir.c
------------------------------------------------------
 * GTIE *  PROJET MIGRAZUR
------------------------------------------------------
 * SOUS-SYSTEME : EQUEXT
------------------------------------------------------
 * MODULE TEINI * FICHIER egab_dir.c
------------------------------------------------------
 * DESCRIPTION DU MODULE :
 *
 * < Module directeur de la tache TEGAB >
 *
------------------------------------------------------
 * HISTORIQUE :
 *
 * SDU       13/03/2012 : Creation DEM/1016
 * JMG	29/08/18 : timer synchro date
 * RGR 17/05/19 : DEM1331 - Ajout Mecanisme KEEPALIVE 1.3
 * JPL	23/11/21 : Consultation echeance minuterie et traitement des alarmes en fin de boucle principale (SAE_339)  1.4
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */
#include <xzsem.h>

#include "egab_dir.h"
#include "egab_cfg.h"
#include "egab_tra.h"
#include "egab_cyc.h"
#include "egab_trace.h"
#include "egab_detection.h"
#include "egab_application.h"
#include "egab_valide.h"
#include "xzav03.h"
/*#include "xzag.h"*/
#include "xzst.h"


/* definitions de constantes */

/* definitions de types locaux */


/* definition de macro locales */

/* declaration de variables locales */
static char *version = "%W% %G% : egab_dir" ;

/* declaration de variables globales */
XDY_NomSite     	vg_NomSite;
XDY_NomMachine  	vg_NomMachine;
XDY_TypeMachine		vg_TypeMachine		= 0;
int rt_error = 0;
int                      vg_SockTIM;
int                      vg_SockTEMIP;
int                      vg_SockTEEIP;
XZEXT_MASK *            vg_SockMask = NULL;
T_INT4                  vg_SockRT;
int vg_SockSRV      = 0;


/* declaration de fonctions internes */
void dir_init(int argc, char **argv);
void dir_sortie(void); 
void  ep_dispo_cb(T_IPC_CONN va_Cnx, T_IPC_CONN_PROCESS_CB_DATA pa_Data, T_CB_ARG pa_ARG);

/* declaration de fonctions externes */
extern void det_ideal_cb();

/*X*/
/*
------------------------------------------------------
 * SERVICE RENDU :
 * Programme principal
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

int main( argc, argv )

/*
 * ARGUMENTS EN ENTREE :
 *   argc : Nombre d'arguments
 *   argv : liste des arguments
 *
 * ARGUMENTS EN SORTIE :
 *   Aucune
 *
 *
 * CODE RETOUR :
 *   Aucun
 *
 * CONDITION D'UTILISATION
 *   Aucune
 *
 * FONCTION
 *   Connexion au RTServer avec initialisation des variables de
 *   supervision, recuperation du site de la machine
 *   ainsi que le type de celle-ci.
 *   Armements de callbacks.
 *   Abonnement au Datagroup XDG_EINIT_NomSite et XDG_ECONFIG_EQPT_NomSite.
 *   Constitution des fichier de config equipt pour toutes les machines (appel ed_config)
 *
------------------------------------------------------*/

int	argc;
char	**argv;
{
  T_IPC_MSG   pl_Msg;
  int vl_IndPre = 0;
  int                     vl_ValRet       =XDC_OK,
      vl_SockRet      = 0,
      vl_SockSRV      = 0,
      vl_resultat;

	/*A
	*  initialisation semaphore
	*/
	sem_init(&vg_semaphore, 0, 1);

  /*A
   * Initialisation du module directeur
   */
   dir_init(argc, argv);

   XZST_03EcritureTrace(XZSTC_INFO, "***** Fin de l'Init : Attente des donnees *****");

   while (1)
   {

     if ( ex_attendre_msg ( XZEXC_SOCKET_NULL, &vg_SockMask, &vl_SockRet ) != XDC_OK )
     {
       XZST_03EcritureTrace(XZSTC_WARNING, "Probleme retour fonction select  ." );
       ex_ConnexionSocketRTserver ( &vg_SockRT, vl_SockRet, &vg_SockMask );
     }
     else
     {
       ex_ConnexionSocketRTserver ( &vg_SockRT, 0, &vg_SockMask );

       /* Si une socket a ete activee */
       if ( vl_SockRet != XZEXC_NO_EVT )
       {

         /* Selon le type de socket */
         /* Socket RT server */
         if ( vl_SockRet == vg_SockRT )
         {
           /* Le message est lu par les primitive standard de RTWorks */
         }
         else
         {

           /* Socket TETIM (retour Timer) */
           if ( vl_SockRet == vg_SockTIM )
           {
             /* Appel de la fonction ec_traitement_cyclique de MCYC */
             ey_traitement_cyclique_gab ( vg_SockTIM );
           }
           else
           {

             /* Socket TESRV (Init ou Arret tache) */
             if ( vl_SockRet == vg_SockSRV )
             {
               /* Appel de la fonction ec_configuration de MCFG */
               XZST_03EcritureTrace(XZSTC_WARNING, "avant ec_controle");
               ec_controle ( vg_SockSRV, vg_NomMachine );
             }
             else
             {
               /* Appel traitement de la trame */
               XZST_03EcritureTrace(XZSTC_WARNING, "avant em_traitement_trame");
               em_traitement_trame( vl_SockRet );
             }
           }
         }
       }   /* fin si socket active */
     }

    if (XZSS_15ConsulterTimeout () == XDC_OUI)
    {
      XZSS_18AnnulerTimeout ();
      ex_traiter_alarmes ();
    }
   }   /* fin while */

}  /* Fin main */


/*X*/
/*------------------------------------------------------
 * SERVICE RENDU :
 *  initialisation du module Directeur de la tache.
 *
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */
void dir_init(int argc, char **argv)

/*
 * ARGUMENTS EN ENTREE :
 *
 *  aucun
 *
 * ARGUMENTS EN SORTIE :
 *  aucun
 *
 * CODE RETOUR :
 *  aucun
 *
 * CONDITION D'UTILISATION
 *
 * FONCTION
 *
------------------------------------------------------*/
{
  XDY_Datagroup		pl_NomDG;
  int            	    vl_ValRet = XDC_OK;
  T_INT4 				vl_TypeParam;
  XDY_Entier 	vl_cr;
  int vl_nb;
  char vl_adr[30];
  XZEXT_MSG_SOCKET        pl_MsgTimer;

  /*A
   * Connexion au RTserver et initialisation des variables
   */
  if ( XZSC_01InitCnxSup( argv[1],
      argv[ 0 ],
      XZSCC_INTERDIRE,
      XZSCC_AUTORISER,
      XZSCC_INTERDIRE,
      dir_sortie,
      "/dev/null" ) != XDC_OK )
  {
    /*B Affichage Trace */
    XZST_03EcritureTrace( XZSTC_WARNING, "Ne peut ouvrir la connexion de TEGAB avec le RTserver." );
    exit( XDC_NOK );
  }
  else
  {
    /*B Affichage Trace */
    XZST_03EcritureTrace( XZSTC_WARNING, "Ouverture de la connexion de TEGAB avec le RTserver." );
  }

  XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_WARNING|XZSTC_FONCTION|XZSTC_MESSAGE,XZSTC_NECRAN);

  /*A Recuperation du nom de la machine */
  if ( XZSC_07NomMachine(vg_NomMachine) != XDC_OK)
  {
    /* Ecriture Trace */
    XZST_03EcritureTrace( XZSTC_FATAL, "Recuperation du Nom de la machine impossible . " );
    exit( XDC_NOK );
  }

  /*A Recuperation du site de la machine */
  if ( (vl_ValRet = XZSC_04NomSite(vg_NomSite)) != XDC_OK)
  {
    /* Ecriture Trace */
    XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom du site impossible. Erreur : %d", vl_ValRet);
    exit( XDC_NOK );
  }

  XZSM_00InitServices();

  /*A Creation du repertoire pour les sockets */
  ex_rep_socket ( argv[1] );

  /*A Init pour Acknoledge Alarme */
  ex_init_alarme ( argv[1], argv[0] );

  /*A
          Connexion socket en mode client pour communiquer avec la tache TETIM
   */
  if ( ex_cnx_client (    XZEXC_ADR_SOCK_CAPTIM,
      XZEXC_ADR_SOCK_TETIM_SRV,
      &vg_SockMask,
      &vg_SockTIM ) != XDC_OK )
  {
    /*B Ecriture Trace */
    XZST_03EcritureTrace( XZSTC_FATAL, "Impossible de creer la connexion client 1 ." );
  }

  /*A
   * Connexion socket en mode client pour communiquer avec la tache TESRV
   */
  if ( ex_cnx_client (    XZEXC_ADR_SOCK_TECAP,
      XZEXC_ADR_SOCK_TESRV,
      &vg_SockMask,
      &vg_SockSRV ) != XDC_OK )
  {
    /*B Ecriture Trace */
    XZST_03EcritureTrace( XZSTC_FATAL, "Impossible de creer la connexion client 2 ." );
  }

  /*A
   * Connexion socket en mode client pour communiquer avec la tache TEMIP
   */
  sprintf(vl_adr,"%s%d",XZEXC_ADR_SOCK_TEIM,XDC_EQT_CAP);
  if ( ex_cnx_client (    XZEXC_ADR_SOCK_TECAP,
      vl_adr,
      &vg_SockMask,
      &vg_SockTEMIP ) != XDC_OK )
  {
    /*B Ecriture Trace */
    XZST_03EcritureTrace( XZSTC_FATAL, "Impossible de creer la connexion temip  ." );
  }

  /*A
   * Demande des timers cycliques
   */
  /* Programation timer 9s */
  ETIM_REVEIL_CYCLIQUE ( EGABC_REVEIL_1MN , EGABC_TIMER_1MN, vg_SockTIM, pl_MsgTimer )

  ETIM_SYNCHRO_DATE ( XZEXC_HEURE_SYNCHRO , "GAB_Timer_Synchro", vg_SockTIM, pl_MsgTimer )

  /* Initialisation du module CONFIG */
  if (cfg_init(vg_NomMachine, &vl_nb) == XDC_NOK)
  {
    XZST_03EcritureTrace( XZSTC_WARNING, "erreur Initialisation module Config.");
    dir_sortie();
  }

  /* Initialisation du module TRACE */
  if (trace_init() == XDC_NOK)
  {
    XZST_03EcritureTrace( XZSTC_WARNING, "ERREUR Initialisation module Trace.");
    dir_sortie();
  }

  /* Initialisation du module DETECTION */
  if (det_init(argv[2]) == XDC_NOK)
  {
    XZST_03EcritureTrace( XZSTC_WARNING, "ERREUR Initialisation module Detection.");
    dir_sortie();
  }

  /* Initialisation du module APPLICATION */
  if (appli_init() == XDC_NOK)
  {
    XZST_03EcritureTrace( XZSTC_WARNING, "ERREUR Initialisation module Application.");
    dir_sortie();
  }

  /* Initialisation du module VALIDE */
  if (valide_init() == XDC_NOK)
  {
    XZST_03EcritureTrace( XZSTC_WARNING, "ERREUR Initialisation module Valide.");
    dir_sortie();
  }

  egab_util_init();

  if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_ELCR),ep_CMD_LCR,
      (T_CB_ARG)NULL) == NULL )
  {
    XZST_03EcritureTrace( XZSTC_FATAL, "Erreur Armement Callback sur XDM_ELCR." );
    dir_sortie() ;
  }

  if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_IETAT_DISPO_EQT), ep_dispo_cb,
      (T_CB_ARG)NULL) == NULL )
  {
    XZST_03EcritureTrace( XZSTC_FATAL, "Erreur Armement Callback sur XDM_IETAT_DISPO_EQT");
    dir_sortie() ;
  }

  /* Abonnement au DataGroup KEEPALIVE */
  XZSC_31AbtKeepAlive(vg_NomSite);

}


/*X*/
/*------------------------------------------------------
 * SERVICE RENDU :
 *  Fonction effectuant les operations de terminaison de la tache.
 *
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

void dir_sortie ( void )

/*
 * ARGUMENTS EN ENTREE :
 *
 *  aucun
 *
 * ARGUMENTS EN SORTIE :
 *  aucun
 *
 * CODE RETOUR :
 *  aucun
 *
 * CONDITION D'UTILISATION
 *  aucun
 *
 * FONCTION
 *  D
 *
------------------------------------------------------*/
{
  XZEXT_MASK *                 pl_Socket;
  XZEXT_MSG_SOCKET             pl_message;

  XZST_03EcritureTrace(XZSTC_WARNING,"dir_sortie : IN");

  /*A Deconnexion de la base de donnees */
  XZAG03_Deconnexion_Base();

  XZST_03EcritureTrace(XZSTC_WARNING,"Sortie de tache TEGAB .");




  /*A
   * Envoie du message d'arret aux taches protocoles
   */

  for ( pl_Socket = vg_SockMask; pl_Socket != NULL; pl_Socket = pl_Socket->Suivant )
  {
    if ( !strncmp ( pl_Socket->adresse, XZEXC_ADR_SOCK_TEIM, strlen(XZEXC_ADR_SOCK_TEIM) ) )
    {
      /* Construction du message d'arret */
      sprintf ( pl_message, "%s%s%s",
          XZEXC_DEMANDE_ARRET,
          pl_Socket->adresse,
          XZEXC_FIN_MSG_SOCK );

      sem_wait(&vg_semaphore); 
      /* Envoie de la demande d'arret */
      if ( write ( pl_Socket->desc_sock, pl_message, strlen (pl_message) ) == XDC_NOK )
      {
        /* Ecriture trace impossible d'emettre une demande d'arret */
        XZST_03EcritureTrace(XZSTC_WARNING, " Impossible d'emettre une demande d'arret." );
      }
      sem_post(&vg_semaphore); 

    }
  }

  /*A Attendre */
  sleep (XZEXC_ATTENTE_ARRET_TACHE);

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

  XZST_03EcritureTrace(XZSTC_FONCTION,"dir_sortie : OUT");
  /*A Arret de la tache */
  exit ( 0 );
}



/*X*/
/*------------------------------------------------------
 * SERVICE RENDU :
 *  < Descriptif de la fonction >
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

int ey_traitement_cyclique ( int va_NumSock )

/*
 * ARGUMENTS EN ENTREE :
 *   < Valeur et descriptif des parametres en entree >
 *
 *
 * ARGUMENTS EN SORTIE :
 *   < Valeur et descriptif des parametres en sortie >
 *
 *
 * CODE RETOUR :
 *   <Valeur de retour et un commentaire explicatif >
 *
 * CONDITION D'UTILISATION
 *   < Explication succinte sur les conditions  d'utilisation
 *     de la fonction
 *
 * FONCTION
 *   < Bref descriptif fonctionnel >
 *
------------------------------------------------------*/
{
  int                  vl_IndCnx = 0,
      vl_LgMsg  = 0;
  XZEXT_MSG_SOCKET     pl_Msg, pl_MsgTimer;
  XDY_Horodate         vl_Horodate;
  XZSMT_Horodate       vl_Horodatedecompo;
  int                  vl_Delai;
  XZEXT_CLE_PROG       pl_CleProg;
  XZEXT_Trame          vl_tlcr;
  int                  vl_noeqt;
  int                  vl_Index = 0;
  char                 vl_Cas;

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
    /* Si le message est correct et si ce n'est pas une deconnexion */
    if ( ( vl_LgMsg != 0 ) && ( vl_IndCnx != XZEXC_DECONNEC ) )
    {
      /*A Si il s'agit d'un reveil cyclique */
      if ( strstr ( pl_Msg, XZEXC_MSG_REVEIL ) != NULL )
      {
      }
    }
  }
}


void ep_dispo_cb(T_IPC_CONN va_Cnx, T_IPC_CONN_PROCESS_CB_DATA pa_Data, T_CB_ARG pa_ARG)
{
  XZST_03EcritureTrace( XZSTC_INTERFACE, "IN : ep_dispo_cb");
#ifdef _TIBCO_EMS
  XDY_Mot 	vl_type, vl_num;
  XDY_Entier	vl_etat_eqt;
#else
  T_INT2 	vl_type, vl_num;
  T_INT4 	vl_etat_eqt;
#endif
  T_Liste_CAB *pl_cab=NULL;

  return;
  /*Recuperation des donnees du msg XDM_IETAT_DISPO_EQT*/
  if(! TipcMsgRead(pa_Data->msg,T_IPC_FT_INT2,&vl_type,
      T_IPC_FT_INT2,&vl_num,
      T_IPC_FT_INT4,&vl_etat_eqt,NULL))
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "appli_etat_dispo_prv_cb : Impossible de recuperer le message");
    return;
  }

  if (vl_type!=XDC_EQT_CAP)
    return;

  XZST_03EcritureTrace(XZSTC_INFO," numero=%d, type=%d, etat=%d", vl_num, vl_type, vl_etat_eqt);

  /*	pl_cab= pg_CAB;

	while(pl_cab!=NULL) {
		if (pl_cab->cab.config.NumeroCAP==vl_num)
			pl_cab->cab.Etat_Service  = vl_etat_eqt;
		pl_cab=  pl_cab->suivant;
	}
   */
}

