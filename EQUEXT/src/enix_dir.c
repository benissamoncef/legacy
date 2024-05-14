/* Fichier : enix_dir.c
------------------------------------------------------
 * GTIE *  PROJET MIGRAZUR
------------------------------------------------------
 * SOUS-SYSTEME : EQUEXT
------------------------------------------------------
 * MODULE TENIX * FICHIER enix_dir.c
------------------------------------------------------
 * DESCRIPTION DU MODULE :
 *
 * C'est le module directeur de la tache GTC Nice
 *
------------------------------------------------------
 * HISTORIQUE :
 *
 * AAZ       version 1.1 17 Septembre 2007    : GTC renovee DEM 664
 * ADB    15 fevrier 2010      : Modification du nom de la connexion vers TESRV
 * ADB       22 mars 2010 : FT 3448 - Gestion des etats liaisons
 * PNI		18/01/15 : Ajout callback et_vide_cb sur XDM_EInit_IHM_NICE 1.4
 * RGR 17/05/19 : DEM1331 - Ajout Mecanisme KEEPALIVE 1.5
 * JPL	23/11/21 : Consultation echeance minuterie et traitement des alarmes en fin de boucle principale (SAE_339)  1.6
------------------------------------------------------*/

/* fichiers inclus */

#include <xzsem.h>
#include "enix_dir.h"

/* definitions de constantes */

/* definitions de types locaux */

typedef struct { int           NumMsg;        /* Numero de message */
XDY_Fonction  FoncCB;        /* Fonction a appeler */
} tm_callback;

/* definition de macro locales */

/* declaration de variables locales */

void    et_vide_cb   (   T_IPC_CONN,
    T_IPC_CONN_PROCESS_CB_DATA,
    T_CB_ARG );


/* AAZ 17/09/2007 Ajout GTC renovee */
static tm_callback pm_TabCB[] =
    {
        { XDM_ECMD_SIG,          (void (*)())et_cmd_sig_cb },
        { XDM_ECMD_ECL,          (void (*)())et_cmd_ecl_cb },
        { XDM_ECMD_VEN,          (void (*)())et_cmd_ven_cb },
        { XDM_ECMD_ENR,          (void (*)())et_cmd_enr_cb },
        { XDM_ECMD_EXP,          (void (*)())et_cmd_exp_cb },
        { XDM_ECMD_INC,          (void (*)())et_cmd_inc_cb },
        { XDM_ECMD_SEQSIG,       (void (*)())et_cmd_seq_sig_cb },
        { XDM_ECMD_SEQECL,       (void (*)())et_cmd_seq_ecl_cb },
        { XDM_ECMD_SEQVEN,       (void (*)())et_cmd_seq_ven_cb },
        { XDM_ECMD_SEQENR,       (void (*)())et_cmd_seq_enr_cb },
        { XDM_ECMD_SEQINC,       (void (*)())et_cmd_seq_inc_cb },
        { XDM_ECMD_SEQEXP,       (void (*)())et_cmd_seq_exp_cb },
        { XDM_ECMD_DOMAINE,      (void (*)())et_cmd_domaine_cb },
        { XDM_EInit_IHM_NICERenovee,    (void (*)())et_Init_IHM_TNIC_cb },
        { XDM_EInit_IHM_NICE,    (void (*)())et_vide_cb },
        { XDM_ECMD_EQPT,         (void (*)())et_controle_cb }
    };

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*
------------------------------------------------------
 * SERVICE RENDU :
 * Programme principal
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

void main( argc, argv )

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
 *
 * Initialisations :connexion rtserver, connection avec les taches TESRV
 * TETIM, init des structures en memoire, gestion des callbacks
 * init des timer cycliques
 * Boucle sur les sockets et gestion des messages recus
 *
------------------------------------------------------*/

int     argc;
char    **argv;
{
  int vl_ValRet = XDC_OK,
      vl_SockRet = 0,
      vl_SockSRV = 0,
      vl_resultat;
  XZEXT_MSG_SOCKET  pl_MsgTimer;
  int               vl_indice_callback;
  XDY_NomSite       pl_NomSite;
  XDY_Datagroup     pl_NomDG;
  int               vl_Instance_Tenic = 0;
  char              vl_NomExe[10];

  /*
   *  Recuperation du nom de la machine
   */
  strcpy( vg_NomMachine, argv[1] );

  /*
   * Recuperation du nom de l'exe en cours
   */
  strcpy( vl_NomExe, argv[0] );

  /* Pour l'instant on ne connait pas le maitre Wizcon */
  vg_maitre_actif = XDC_MAITRE_INCONNU;

  /*
   * Pour parametre ajoute a ex_lire_config_Wizcon
   * on recupere le numero d'instance de l'executable
   */
  vl_Instance_Tenic = vl_NomExe[4] - 48;
  vl_NomExe[5] = '\0';

	/*A
	*  initialisation semaphore
	*/
	sem_init(&vg_semaphore, 0, 1);


  if (ex_lire_config_Wizcon(vl_Instance_Tenic) != XDC_OK)
  {

    /* Affichage Trace */
    XZST_03EcritureTrace( XZSTC_WARNING, "Probleme de lecture du fichier de configuration, mise a jour donnees par defaut." );
    strcpy(vg_nom_machine_TWWI0,"SWDN1");
    strcpy(vg_nom_machine_TWWI1,"SWDN2");
    vg_Port_SockTWWI0 = 10000;
    vg_Port_SockTWWI1 = 10000;
  }
  XZST_03EcritureTrace( XZSTC_WARNING, "Numero instance %d ", vl_Instance_Tenic );

  /*
   * Connexion au RTserver et initialisation des variables
   */
  if( XZSC_01InitCnxSup( NULL,
      argv[ 0 ],
      XZSCC_INTERDIRE,
      XZSCC_AUTORISER,
      XZSCC_INTERDIRE,
      et_sortir,
      "/dev/null" ) != XDC_OK )
  {
    /* Affichage Trace */
    XZST_03EcritureTrace( XZSTC_WARNING, "Ne peut ouvrir la connexion avec le RTserver." );
    exit( XDC_NOK );
  }
  else
  {
    /* Affichage Trace */
    XZST_03EcritureTrace( XZSTC_DEBUG1, "Ouverture de la connexion avec le RTserver." );
  }

  /* Creation du repertoire pour les sockets */
  ex_rep_socket ( vg_NomMachine );

  /* Init pour Acknoledge Alarme */
  ex_init_alarme ( argv[1], argv[0] );

  /*XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_WARNING|XZSTC_DEBUG1|XZSTC_FONCTION|XZSTC_INTERFACE|XZSTC_MESSAGE,XZSTC_NECRAN);*/
  /*XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_WARNING|XZSTC_INFO|XZSTC_FONCTION|XZSTC_INTERFACE|XZSTC_MESSAGE,XZSTC_NECRAN);*/
  /*XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_WARNING|XZSTC_FONCTION,XZSTC_NECRAN);*/
  XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_WARNING|XZSTC_INFO|XZSTC_DEBUG1|XZSTC_FONCTION|XZSTC_INTERFACE|XZSTC_MESSAGE,XZSTC_NECRAN);
  /*
   * Connexion socket en mode client pour communiquer avec la tache TESRV
   */
  if ( ex_cnx_client ( vl_NomExe,
      XZEXC_ADR_SOCK_TESRV,
      &vg_SockMask,
      &vl_SockSRV ) != XDC_OK )

    /*if ( ex_cnx_client ( XZEXC_ADR_SOCK_TENIC,
                         XZEXC_ADR_SOCK_TESRV,
                         &vg_SockMask,
                         &vl_SockSRV ) != XDC_OK )*/
  {
    /* Ecriture Trace */
    XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de creer la connexion client TESRV ." );
  }    else
    XZST_03EcritureTrace( XZSTC_WARNING, "Connexion TESRV OK.%d",vl_Instance_Tenic );


  /*
   * Connexion socket en mode client pour communiquer avec la tache TETIM
   */
  if ( ex_cnx_client ( XZEXC_ADR_SOCK_NICTIM,
      XZEXC_ADR_SOCK_TETIM_SRV,
      &vg_SockMask,
      &vg_SockTETIM ) != XDC_OK )
  {
    /* Ecriture Trace */
    XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de creer la connexion client TETIM ." );
  }

  /*
   * Connexion socket en mode client pour communiquer avec la tache TWWI0
   */
  if ( ex_cnx_client_ST ( vg_nom_machine_TWWI0,
      vg_Port_SockTWWI0,
      &vg_SockMask,
      &vg_SockTWWI0 ) != XDC_OK )
  {
    /* Ecriture Trace */
    XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de creer la connexion client maitre Wizcon %s .",
        vg_nom_machine_TWWI0 );
    vg_etat_SockTWWI0=XDC_NOK;
  }
  else vg_etat_SockTWWI0=XDC_OK;

  ex_env_liaison ( vg_nom_machine_TWWI0, vg_etat_SockTWWI0, vg_TypeEqt_LiaiTWWI0, vg_NumEqt_LiaiTWWI0);
  /*
   * Connexion socket en mode client pour communiquer avec la tache TWWI1
   */
  if ( ex_cnx_client_ST ( vg_nom_machine_TWWI1,
      vg_Port_SockTWWI1,
      &vg_SockMask,
      &vg_SockTWWI1 ) != XDC_OK )
  {
    /* Ecriture Trace */
    XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de creer la connexion client esclave Wizcon %s .",
        vg_nom_machine_TWWI1 );
    vg_etat_SockTWWI1=XDC_NOK;
  }
  else vg_etat_SockTWWI1=XDC_OK;
  ex_env_liaison ( vg_nom_machine_TWWI1, vg_etat_SockTWWI1,  vg_TypeEqt_LiaiTWWI1, vg_NumEqt_LiaiTWWI1);

  /*
   * Appel de MCFG fonction et_init
   */
  XZST_03EcritureTrace( XZSTC_DEBUG1, "Appel de la fonction d'init." );
  et_init( vg_NomMachine );

  /*
   * Armement des Callback sur reception des message
   */

  /* Recuperation du site de la machine */
  if ( XZSC_04NomSite( pl_NomSite ) != XDC_OK)
  {
    XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom du site impossible ." );
  }

  /* Initialisation des callbacks de la tache */

  for ( vl_indice_callback = 0;
      vl_indice_callback < (sizeof(pm_TabCB)/sizeof(pm_TabCB[0]));
      vl_indice_callback++)
  {
    if( !TipcSrvProcessCbCreate( XDM_IdentMsg(pm_TabCB[vl_indice_callback].NumMsg ) ,
        pm_TabCB[vl_indice_callback].FoncCB ,
        (T_CB_ARG)NULL) )
    {
      XZST_03EcritureTrace( XZSTC_WARNING,
          "erreur init callback '%s'",
          pm_TabCB[vl_indice_callback].FoncCB);
    }
  }

  /*
   * Abonnement aux Datagroup
   */
  /* Messages de commande signalisation tunnel : XDG_ECMD_GTC_NICE */
  if ( !XDG_JoindreDG( XDG_ECMD_GTC_NICE ) )
  {
    /* Affichage Trace */
    XZST_03EcritureTrace( XZSTC_WARNING,
        "Abonnement au Datagroup %s non effectue ",
        XDG_ECMD_GTC_NICE );
  }

  /* Messages de commande signalisation tunnel : XDG_ECMD_GTC_NICE */
  XDG_EncodeDG2 ( pl_NomDG, "ECMD_NICE", vg_NomMachine ) ;
  if ( !XDG_JoindreDG( pl_NomDG ) )
  {
    XZST_03EcritureTrace( XZSTC_WARNING, "Abonnement au Datagroup %s non effectue ", XDG_ECMD_GTC_NICE );
  }

  /* Messages de abonnement : XDG_EInit_IHM_NICE */
  /* joint ce datagroup */
  XDG_EncodeDG2 ( pl_NomDG, XDG_EInit_IHM_NICE, pl_NomSite ) ;
  if ( !XDG_JoindreDG( pl_NomDG ) )
  {
    XZST_03EcritureTrace( XZSTC_WARNING, "Abonnement au Datagroup %s non effectue ", XDG_EInit_IHM_NICE );
  }

  /* Abonnement au DataGroup KEEPALIVE */
  XZSC_31AbtKeepAlive(pl_NomSite);

  /* Attente des messages sur les sockets (RTserver, d'ecoute et de donnees */
  XZST_03EcritureTrace( XZSTC_WARNING, "Boucle infinie sur attente des message recus." );

  while (1)
  {
    if ( ex_attendre_msg ( XZEXC_SOCKET_NULL,
        &vg_SockMask,
        &vl_SockRet ) != XDC_OK )
    {
      XZST_03EcritureTrace(XZSTC_WARNING, "Probleme retour fonction select  ." );
      ex_ConnexionSocketRTserver ( &vg_SockRT,
          vl_SockRet,
          &vg_SockMask );
    }
    else
    {
      ex_ConnexionSocketRTserver ( &vg_SockRT, 0, &vg_SockMask );

      /* Si une socket a ete activee */
      if ( vl_SockRet != XZEXC_NO_EVT )
      {
        XZST_03EcritureTrace( XZSTC_INFO, "Socket activee." );
        /* Selon le type de socket */
        /* Socket RT server */
        if ( vl_SockRet == vg_SockRT )
        {
          /* Le message est lu par les primitive standard de RTWorks */
        }
        else
        {
          /* Socket TETIM (retour Timer) */
          if ( vl_SockRet == vg_SockTETIM )
          {
            /* Appel de la fonction et_traitement_timers */
            et_traitement_timers ();
          }
          else
          {
            /* Socket TESRV (Init ou Arret tache) */
            if ( vl_SockRet ==  vl_SockSRV)
            {
              /* Appel de la fonction et_controle de MCFG */
              et_controle ( vl_SockSRV, vg_NomMachine );
            }
            else
            {
              /* Appel traitement de la trame */
              XZST_03EcritureTrace( XZSTC_INFO, "Socket Wizcon appel et_traitement." );
              et_traitement ( vl_SockRet );
            }
          }
        }
      }   /* fin si socket active */
    }


    /* Appel de la gestion de redondance */
    et_Redondance();

    /* Appel et_synchro */
    et_Synchro();

    if (XZSS_15ConsulterTimeout () == XDC_OUI)
    {
      XZSS_18AnnulerTimeout ();
      ex_traiter_alarmes ();
    }

  }   /* fin while */

}  /* Fin main */

void    et_vide_cb( T_IPC_CONN                  va_Cnx,
    T_IPC_CONN_PROCESS_CB_DATA  pa_Data,
    T_CB_ARG                    pa_ARG)

{
  XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : et_rien_cb" );
  XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : et_rien_cb" );
  return;
}
