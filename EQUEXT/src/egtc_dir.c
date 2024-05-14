/*E*/
/* Fichier : egtc_dir.c
 ------------------------------------------------------
 * GRANIOU *  PROJET MIGRAZUR
 ------------------------------------------------------
 * SOUS-SYSTEME : EQUEXT
 ------------------------------------------------------
 * MODULE TEGTC * FICHIER egtc_dir.c
 ------------------------------------------------------
 * DESCRIPTION DU MODULE :
 *
 *   Module directeur de la tache TEGTC
 *
 ------------------------------------------------------
 * HISTORIQUE :
 *
 * Nagiel.E	version 1.1	16 Dec 1994	: Creation
 * Volcic.F	version 1.2	09 Avr 1995	: Modif Abont J/N
 * Mismer.D	version 1.3	03 Mai 1995	: Ajout Abt etat liaison
 * Volcic.F	version 1.4	04 Mai 1995	:
 * Fontaine.C	version 1.5	19 Mai 1995	: Reprise entete, historique
 * Mismer.D  	version 1.6	07 Jun 1995	: Modif connexion socket RTserver
 * Mismer.D  	version 1.7	31 Aou 1995	: Modif connexion socket RTserver suite
 * Mismer.D  	version 1.8	11 Oct 1995	: Ajout appel fonction init Acknoledge alarme
 * Mismer.D	version 1.9 	15 Nov 1996	: Modif pour alerte DAI tunnel (DEM/1309)
 *
 * JPL		17/03/11 : Migration architecture HP ia64 (DEM 975) : suppression warnings mineurs  1.10
 * RGR	17/05/19 : DEM1331 - Ajout Mecanisme KEEPALIVE 1.11
 * JPL	17/11/21 : Consultation echeance minuterie et traitement des alarmes en fin de boucle principale (SAE_339)  1.12
 -----------------------------------------------------*/

/* fichiers inclus */

#include <xzsem.h>
#include "ex_mala.h"
#include "ex_mrtf.h"

#include "egtc_dir.h"
#include "egtc_tra.h"

/* definitions de constantes */

/* definitions de types locaux */

typedef struct
{
  int NumMsg; /* Numero de message */
  XDY_Fonction FoncCB; /* Fonction a appeler */
} tm_callback;

/* definition de macro locales */

/* declaration de variables locales */

static char *version =
    "$Id: egtc_dir.c,v 1.11 2019/06/04 13:37:46 devgtie Exp $ : egtc_dir";

static tm_callback pm_TabCB[] =
  {
    { XDM_ECMD_EQPT, (void
    (*)()) eg_controle_cb },
    { XDM_ECMD_GTC, (void
    (*)()) eq_commande_sortie_TOR },
    { XDM_EInit_IHM_GTC, (void
    (*)()) eg_Init_IHM_GTC_cb },
    { XDM_EInit_IHM_JourNuit, (void
    (*)()) eg_Init_IHM_JourNuit_cb },
    { XDM_EInit_IHM_Liaison, (void
    (*)()) ep_Init_IHM_Liaison_cb },
    { XDM_ELCR, (void
    (*)()) eg_CMD_LCR } };

/* declaration de fonctions internes */

/* definition de fonctions externes */
extern int
ey_traitement_cyclique(int);

/*X*/
/*------------------------------------------------------
 * SERVICE RENDU :
 * Programme principal
 ------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

int main(argc, argv)

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
   ------------------------------------------------------*/

  int argc;char **argv;
{
  int vl_ValRet = XDC_OK, vl_SockRet = 0, vl_SockSRV = 0, vl_resultat;
  XZEXT_MSG_SOCKET pl_MsgTimer;
  XDY_Datagroup pl_NomDG;
  int vl_indice_callback;
  int vl_ALIVE = 0;
  int vl_CodeRetour = 0;
  int vl_LgRetour = 0;

  /*A
   *  Recuperation du nom de la machine
   */
  strcpy(vg_NomMachine, argv[1]);

	/*A
	*  initialisation semaphore
	*/
	sem_init(&vg_semaphore, 0, 1);

  /*A
   * Connexion au RTserver et initialisation des variables
   */
  if (XZSC_01InitCnxSup(NULL, argv[0], XZSCC_INTERDIRE, XZSCC_AUTORISER,
      XZSCC_INTERDIRE, ec_sortir, "/dev/null") != XDC_OK)
  {
    /*B Affichage Trace */
    XZST_03EcritureTrace(XZSTC_FATAL,
        "Ne peut ouvrir la connexion avec le RTserver.");
  }
  else
  {
    /*B Affichage Trace */
    XZST_03EcritureTrace(XZSTC_DEBUG1,
        "Ouverture de la connexion avec le RTserver.");
  }

  /*A
   * Recuperation du site de la machine
   */
  if ((vl_ValRet = XZSC_04NomSite(vg_NomSite)) != XDC_OK)
  {
    /* Ecriture Trace */
    XZST_03EcritureTrace(XZSTC_FATAL,
        "Recuperation du Nom du site impossible . ");
  }

  /*A Creation du repertoire pour les sockets */
  ex_rep_socket(argv[1]);

  /*A Init pour Acknoledge Alarme */
  ex_init_alarme(argv[1], argv[0]);

  /*A
   * Connexion socket en mode client pour communiquer avec la tache TESRV
   */
  if (ex_cnx_client(XZEXC_ADR_SOCK_TEGTC, XZEXC_ADR_SOCK_TESRV, &vg_SockMask,
      &vl_SockSRV) != XDC_OK)
  {
    /*B Ecriture Trace */
    XZST_03EcritureTrace(XZSTC_FATAL,
        "Impossible de creer la connexion client  .");
  }

  /*A
   * Connexion socket en mode client pour communiquer avec la tache TETIM
   */
  if (ex_cnx_client(XZEXC_ADR_SOCK_GTCTIM, XZEXC_ADR_SOCK_TETIM_SRV,
      &vg_SockMask, &vg_SockTIM) != XDC_OK)
  {
    /*B Ecriture Trace */
    XZST_03EcritureTrace(XZSTC_FATAL,
        "Impossible de creer la connexion client  .");
  }

  /*A
   * Appel de MCFG fonction ec_init
   */
  if (ec_init(vg_NomMachine) != XDC_OK)
  {
    /*B Ecriture Trace */
    /*XZST_03EcritureTrace( XZSTC_FATAL, "Initialisation de la config echouee" );	*/
  }

  /*A
   * Armement des Callback sur reception des message
   */

  /* Initialisation des callbacks de la tache */

  for (vl_indice_callback = 0; vl_indice_callback < (sizeof(pm_TabCB)
      / sizeof(pm_TabCB[0])); vl_indice_callback++)
  {
    if (!TipcSrvProcessCbCreate(XDM_IdentMsg(
        pm_TabCB[vl_indice_callback].NumMsg),
        pm_TabCB[vl_indice_callback].FoncCB, (T_CB_ARG) NULL))
    {
      XZST_03EcritureTrace(XZSTC_WARNING, "erreur init callback '%s'",
          pm_TabCB[vl_indice_callback].FoncCB);
    }
  }

  /*A
   * Abonnement aux Datagroup
   */

  /* Messages de demande d'init : XDG_EInit_IHM_GTC_<NomMachine> */
  XDG_EncodeDG2(pl_NomDG, XDG_EInit_IHM_GTC, vg_NomMachine);
  /* joint ce datagroup */
  if (!XDG_JoindreDG(pl_NomDG))
  {
    /* Affichage Trace */
    XZST_03EcritureTrace(XZSTC_FATAL,
        "Abonnement au Datagroup %s non effectue ", pl_NomDG);
  }

  /* Messages de demande d'init : XDG_EInit_IHM_JOURNUIT_<NomSite> */
  XDG_EncodeDG2(pl_NomDG, XDG_EInit_IHM_JOURNUIT, vg_NomSite);
  /* joint ce datagroup */
  if (!XDG_JoindreDG(pl_NomDG))
  {
    /* Affichage Trace */
    XZST_03EcritureTrace(XZSTC_FATAL,
        "Abonnement au Datagroup %s non effectue ", XDG_EInit_IHM_JOURNUIT);
  }

  /* Messages de commance de contraste : XDG_EInit_IHM_Liaison_<NomMachine> */
  XDG_EncodeDG2(pl_NomDG, XDG_EInit_IHM_Liaison, vg_NomMachine);
  /* joint ce datagroup */
  if (!XDG_JoindreDG(pl_NomDG))
  {
    /* Affichage Trace */
    XZST_03EcritureTrace(XZSTC_FATAL,
        "Abonnement au Datagroup %s non effectue ", pl_NomDG);
  }

  /* Messages de controle : XDG_ECMD_GTC_<NomMachine> */
  XDG_EncodeDG2(pl_NomDG, XDG_ECMD_GTC, vg_NomMachine);
  /* joint ce datagroup */
  if (!XDG_JoindreDG(pl_NomDG))
  {
    /* Affichage Trace */
    XZST_03EcritureTrace(XZSTC_FATAL,
        "Abonnement au Datagroup %s non effectue ", pl_NomDG);
  }

  /* Abonnement au DataGroup KEEPALIVE */
  XZSC_31AbtKeepAlive(vg_NomSite);

  /*A
   * Demande des timers cycliques
   */

  /* Programation timer 10s */
  ETIM_REVEIL_CYCLIQUE ( EGTCC_REVEIL_20S, "GTC_Timer_20s", vg_SockTIM, pl_MsgTimer )

  /*A Attente des messages sur les sockets (RTserver, d'ecoute et de donnees */

  while (1)
  {

    if (ex_attendre_msg(XZEXC_SOCKET_NULL, &vg_SockMask, &vl_SockRet) != XDC_OK)
    {
      XZST_03EcritureTrace(XZSTC_WARNING, "Probleme retour fonction select  .");
      ex_ConnexionSocketRTserver(&vg_SockRT, vl_SockRet, &vg_SockMask);
    }
    else
    {
      ex_ConnexionSocketRTserver(&vg_SockRT, 0, &vg_SockMask);

      /* Si une socket a ete activee */
      if (vl_SockRet != XZEXC_NO_EVT)
      {
        /* Selon le type de socket */
        /* Socket RT server */
        if (vl_SockRet == vg_SockRT)
        {
          /* Le message est lu par les primitive standard de RTWorks */
        }
        else
        {
          /* Socket TETIM (retour Timer) */
          if (vl_SockRet == vg_SockTIM)
          {
            /* Appel de la fonction ec_traitement_cyclique de MCYC */
            ey_traitement_cyclique(vg_SockTIM);
          }
          else
          {
            /* Socket TESRV (Init ou Arret tache) */
            if (vl_SockRet == vl_SockSRV)
            {
              /* Appel de la fonction ec_configuration de MCFG */
              ec_controle(vl_SockSRV, vg_NomMachine);
            }
            else
            {
              /* Appel traitement de la trame */
              em_traitement_trame(vl_SockRet);
            }
          }
        }
      } /* fin si socket active */
    }

    if (vg_SockRT != 0)
    {
    }

    if (XZSS_15ConsulterTimeout () == XDC_OUI)
    {
      XZSS_18AnnulerTimeout ();
      ex_traiter_alarmes ();
    }

  } /* fin while */

} /* Fin main */

