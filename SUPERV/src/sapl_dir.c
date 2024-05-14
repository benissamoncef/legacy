/* Fichier : $Id: sapl_dir.c,v 1.29 2021/07/19 16:13:52 pc2dpdy Exp $	$Revision: 1.29 $	Release : $Revision: 1.29 $        Date : $Date: 2021/07/19 16:13:52 $
 ******************************************************
 * STERIA *  PROJET MIGRAZUR
 ******************************************************
 * MODULE SAPL_DIR * FICHIER sapl_dir.c
 ******************************************************
 * DESCRIPTION DU MODULE :
 *
 * Ce module est le module directeur de la tache tsaplg
 *
 ******************************************************
 * HISTORIQUE :
 *
 * T.Milleville  10 Oct 1994 94	: Creation
 * T.Milleville  09 Avr 1995	: Ajout du traitement du message
 *	XDM_TSP_STARTTMP V1.4
 * B.G.		23 Jun 1995	: ajout delai si echec rtserver (1.5)
 * T.Milleville	20 Jul 1995	: Ajout test erreur RTworks dans
 * le traitement des msg V1.7
 * T.Milleville	4 Aou 1995	:  Ajout de l'option de relance du
 *	RTserver si erreur RTworks dans la fonction de sortie
 *	de la tache V1.9
 * T.Milleville	8 Aou 1995	:  Remplacement de SIGTERM par SIGKILL
 *	dans la fonction kill pour stopper un process V1.10
 * T.Milleville	15 Sep 1995	:  Redirection des stout et stderr dans /dev/null
 *		Option dans XZSC_01..  V1.12
 * T.Milleville	21 Sep 1995	:  Ajout de la destruction de la connexion
 *	avec RTserveur dans la fonction de sortie V1.12
 * T.Milleville	27 Sep 1995	: Suppression du sleep d'aatente lors
 *	d'une erreur RTworks V1.13
 * T.Milleville	6 Oct 1995	: Lancement de tsetat.x en utilisant une nouvelle
 *	fonction sp_startTache_Inter   V1.14
 * T.Milleville	9 Oct 1995	: Lancement de l'application complete
 *	meme si etat machine PASSIF V1.14
 * T.Milleville	03 Nov 1995	: Surveillance du datagroup _server
 *	si SD Actif ou SC Actif V1.14
 * T.Milleville	09 Nov 1995	: Modification de la constante C_NOMTACHE_RTSERVER afin
 *	de prendre en compte le chemin complet via la constante XDC_PATH_RTWORKS V1.16
 * P.Niepceron	24 Nov 1995	: Ajout d'un sleep sur une erreur RT du a un EAGAIN v1.16
 * P.Niepceron   30 Jan 1996	: Apres 500 erreur TWOULD_BLOCK et EAGAIN deconnex et reconnex (626) v1.17
 * P.Niepceron   25 Avr 1996	: On sort de la l'appli si la reconnex a echouee v1.18 (626 )
 * P.Niepceron   21 Jun 1996     : On sort de la l'appli meme si la reconnex reussi v1.20 (626)
 ***
 * P.Niepceron   22 Aou 1996     : Ajout du type PC simplifie 1.21
 * P.Niepceron   16 Dec 1997	: Ajout du type PC niv2 $Revision: 1.29 $ (DEM/1533)
 * PNI		02/12/2014	: Abt a XDM_TSE_KEEPALIVE pour activer socket de tsaplg DEM1116  1.24
 * JPL		08/02/18	: Migration Linux 64 bits (DEM 1274) : Parenthesage expressions; valeurs de retour des fonctions  1.25
 * RGR           25/03/2019      : Mise en place KEEPALIVE pour eviter deconnexion due au FireWall
 * JMG          15/02/2020      : EMS 1.27
 * PNI          09/03/2021      : Trace de keep alive 1.28
 * LCL		19/07/2021	: EMS 1.29
 * LCL		20/12/21	: MOVIS P2 ems SAE-312
 * GGY  22/03/23  : PRA - Changement de District - Envoi message XDG_MGA_CONFIG_PO - Ajout fonction sapl_lire_config_TOPS()
* KOU  25/09/2023	: Adaptation code pour rabbitmq DEM-483
 *******************************************************/

/* fichiers inclus */
#include <sys/signal.h>

#include <xzsem.h>
#include "xdc.h"
#include "xdg.h"
#include "xdm.h"

#include "xzsa.h"
#include "xzsc.h"
#include "xzst.h"
#include "xzsp.h"
#include "xzems.h"
#include "sapl_com.h"
#include "sapl_dir.h"
#include "sapl_don.h"
#include "sapl_ini.h"
#include "sapl_liv.h"
#include "sapl_sur.h"
#include "sapl_tra.h"

/* definitions de constantes (locales a ce fichier) */
#define SPC_SYNC_TIME_OUT 120 /* time-out attente demarrage tache */

#define C_NOMTACHE_RTSERVER XDC_PATH_RTWORKS "/rtserver" /* Process RTserver */

/* definitions de types locaux */
typedef struct
{
  int NumMsg;       /* Numero message RTW */
  void (*FoncCB)(); /* Fonction callback */
} tm_callback;

/* definition de macro locales */

/* declaration de variables locales */
static tm_callback pm_TabCB[] =
    {
        {XDM_TSP_APPL, (void (*)())sp_cmdappli_cb},
        {XDM_TSP_SSYS, (void (*)())sp_cmdssys_cb},
        {XDM_TSP_TACH, (void (*)())sp_cmdtach_cb},
        {XDM_TSP_STOP, (void (*)())sp_cmdmach_cb},
        {XDM_TSP_INIT, (void (*)())sp_init_cb},
#ifndef _TIBCO_EMS
        /*{ T_MT_DG_STATUS, (void (*)())sp_dgstatus_cb },*/
        {T_MT_MON_SUBJECT_SUBSCRIBE_STATUS, (void (*)())sp_dgstatus_cb}, /*TIBCO SS66*/
#else
        {XDM_TSE_WATCH, (void (*)())sp_watch_cb},
#endif
        {XDM_TSP_ETAT, (void (*)())sp_etat_cb},
        {XDM_TSP_STARTTMP, (void (*)())sp_starttmp_cb},
};

/* declaration de fonctions internes */
static void sp_synchrotache();
int sapl_lire_config_TOPS (XDY_Texte va_param, XDY_Texte va_IP, XDY_Texte va_display, XDY_Texte va_port);

/******************************************************
 * STERIA *  PROJET MIGRAZUR
 ******************************************************
 * MODULE : SAPL_DIR
 ******************************************************
 * DESCRIPTION DU MAIN :
 *
 *	phase d'init
 *	lancement de la tache passee en parametre
 *	armement des callbacks
 *	attente infinie en vue du traitement d'un message
 *
 ******************************************************
 * SEQUENCE D'APPEL :
 */

int main(int argc, char *argv[])

/*
 * PARAMETRES EN ENTREE : Nom de la tache a lancer
 *
 * PARAMETRES EN SORTIE : aucun
 *
 * VALEUR(S) DE RETOUR : SPC_EXIT_OK : programme termine correctement
 *			SPC_EXIT_NOK : programme termine avec erreur
 *
 * CONDITION D'ERREUR :
 *
 * - Absence de parametre en entree
 * - Impossibilite de se connecter au RT Serveur
 *
 * CONDITIONS LIMITES : neant
 *
 ******************************************************/

{
  XZSCT_NomSite vl_NomSite = "";
  XZSCT_NomMachine vl_NomMachine = "";
  XZSCT_TypeMachine vl_TypeMachine = 0;
  XZSCT_NomTache vl_NomTache = "";
  XDY_Texte vl_IP = "", vl_IP1 = "", vl_IP2 = "";
  XDY_Texte vl_display1 = "", vl_display2 = "";
  XDY_Texte vl_port1 = "", vl_port2 = "";
  XDY_Texte vl_displayPort1 = "", vl_displayPort2 = "";
  int vl_indice_callback = 0;
  XZSCT_Datagroup vl_datagroup = "";
  int vl_ValRet = XDC_OK;
  T_IPC_MSG pl_Msg;
  int vl_EtatMach = 0;
  char *vl_TachePing = "tspng.x";
  char *vl_TacheTimer = "tstim.x";
  int vl_IndPre = 0;
  int vl_Ind_errTWOULD = 0;
  int vl_Attente = 0;
  int vl_Pid_Rtserver = 0;
  int vl_errno = 0;
  int vl_rterrno = 0;
  XDY_Horodate vl_DateEnSeconde, vl_DateReveil;
  int vl_JourSemaine;
  XDY_Datagroup vl_dg = "";
  XZSMT_CleProg vl_cle_timer = "";
  int vl_timer_tstim = 0;
  #ifdef _RABBITMQ
  int *consumerInfoCollection = NULL;
  #else
  tibemsCollection consumerInfoCollection = NULL;
  #endif
  char vl_str[100];
  int vl_ind;

  /*A
   *  initialisation semaphore
   */
  sem_init(&vg_semaphore, 0, 1);

  /* Initialisation des variables de configuration */
  XZST_03EcritureTrace(XZSTC_FONCTION, "IN : main");

  /* Memorisation debut d'init */
  vg_saplFinInit = XDC_FAUX;
    XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL | XZSTC_DEBUG1 | XZSTC_WARNING | XZSTC_FONCTION | XZSTC_INFO | XZSTC_INTERFACE | XZSTC_MESSAGE, XZSTC_NECRAN);

  /*A Initialisation de la tache et connexion au RTServer */
  if ((vl_ValRet = XZSC_01InitCnxSup(argv[2],
                                     argv[0],
                                     XZSCC_INTERDIRE,
                                     XZSCC_AUTORISER,
                                     XZSCC_INTERDIRE,
                                     sp_sortir,
                                     "/dev/null")) == XDC_OK)
  {

    XZST_03EcritureTrace(XZSTC_WARNING, "apres XZSC_01InitCnxSup");
    XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL | XZSTC_DEBUG1 | XZSTC_WARNING | XZSTC_FONCTION | XZSTC_INFO | XZSTC_INTERFACE | XZSTC_MESSAGE, XZSTC_NECRAN);

    /*B Les process fils qui meurent ne deviennent
     *				pas des process ZOMBIE <defunct> */
    signal(SIGCHLD, SIG_IGN);

    /*B On redirige le Ctrl^C sur la fonction de sortie */
    signal(SIGINT, sp_sortir);

    XZST_03EcritureTrace(XZSTC_WARNING, "apres signal");
    /* Initialisation des variables de configuration */

    XZSM_00InitServices();

    /* Nom du Site */
    XZSC_04NomSite(vl_NomSite);
    /* Type de Machine */
    XZSC_05TypeMachine(&vl_TypeMachine);
    /* Nom de Tache */
    XZSC_06NomTache(vl_NomTache);
    /* Nom de Machine */
    XZSC_07NomMachine(vl_NomMachine);

    /* 1 argument = nom tache seta.x	*/
    /* 2 arguments = nom tache seta.x +	*/
    /* nom application			*/
    if (((argc == 2) || (argc == 3)) &&
        (strcmp(argv[0], argv[1]) != 0))
    /* le nom de la tache tseta.x
     * et different du nom de la tache tsapl.x  */
    {

      /*B Memorisation du nom de la tache de lancement */
      strcpy(vg_tacheSapl, argv[0]);

      /* Memorisation du nom de la tache passe en parametre */
      strcpy(vg_tacheSeta, argv[1]);

      /* Memorisation du nom de l'application a utiliser */
      if (argc == 3)
      {
        /* Nom passe en Argument */
        strcpy(vg_NomAppli, argv[2]);
      }
      else
      {
        /* si Argument null alors valeur par defaut */
        strcpy(vg_NomAppli, XDC_NOM_APPLICATION);
      }
      XZST_03EcritureTrace(XZSTC_WARNING, "avant sp_initTabSurvTache");

      /* Initialisation du tableau des taches avec vg_tacheSeta */
      sp_initTabSurvTache(vg_tacheSeta);

      /*  Synchronisation avec la tache passe en parametre */
      XZST_03EcritureTrace(XZSTC_WARNING,
                           "main : argv[1]='%s'", vg_tacheSeta);
      /*B Construction Datagroup NSUPAPLG_NomMachine **/
      XDG_EncodeDG2(vl_datagroup, XDG_NSUPAPLG, vl_NomMachine);
      sp_synchrotache(vg_tacheSeta, vl_datagroup);
      XZST_03EcritureTrace(XZSTC_WARNING, "apres sp_synchrotache ");
    }
    else
    {
      /* Sortie de l'application  */
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : main SPC_EXIT_NOK");
      XZST_03EcritureTrace(XZSTC_FATAL, "Argument invalide");
      XZST_02FermetureTrace();

      exit(SPC_EXIT_NOK);
    }

    XZST_03EcritureTrace(XZSTC_WARNING, "apres demarrage tsetat");
    /*A Initialiser administration systeme */
    XZSA_20InitAdmin();
    XZST_03EcritureTrace(XZSTC_WARNING, "apres XZSA_20InitAdmin");

    /*A Initialiser donnees et armement surveillance taches */
    sp_init();
    XZST_03EcritureTrace(XZSTC_WARNING, "apres sp_init");

    /*A Initialisation des callbacks de la tache */
    for (vl_indice_callback = 0;
         vl_indice_callback < (sizeof(pm_TabCB) / sizeof(pm_TabCB[0]));
         vl_indice_callback++)
    {
      if (!TipcSrvProcessCbCreate(
              XDM_IdentMsg(pm_TabCB[vl_indice_callback].NumMsg),
              pm_TabCB[vl_indice_callback].FoncCB,
              (T_CB_ARG)NULL))
      {
        XZST_03EcritureTrace(XZSTC_WARNING,
                             "erreur init callback '%s'",
                             pm_TabCB[vl_indice_callback].FoncCB);
      }
    }

    /*A Joint la datagroup XDG_NSUPAPLG_NomMachine */
    XDG_EncodeDG2(vl_datagroup, XDG_NSUPAPLG, vl_NomMachine);
    if (!XDG_JoindreDG(vl_datagroup))
    {
      XZST_03EcritureTrace(XZSTC_FATAL,
                           "Echec XDG_JoindreDG(%s)", vl_datagroup);
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : main SPC_EXIT_NOK");
      sp_sortir();
    }

    /*A Memorisation de la fin d'init */
    vg_saplFinInit = XDC_VRAI;

    /*A Recuperation de l'etat de la machine **/
    if ((vl_ValRet = XZSE_09EtatMachine(&vl_EtatMach)) != XDC_OK)
    {
      /*B Pb recuperation etat de la machine */
      XZST_03EcritureTrace(XZSTC_WARNING,
                           "Pb de recuperation etat machine \n");
    }

    /* Si SD, abonnement a d'autres datagroups **/
    if ((vl_TypeMachine == XZSCC_TYPEM_SD) &&
        (vl_EtatMach == XZSEC_ETAT_NACTIF))
    {
      /*B Abonnement au datagroup NSUPAPLG **/
      if (!XDG_JoindreDG(XDG_NSUPAPLG))
      {
        XZST_03EcritureTrace(XZSTC_FONCTION,
                             "OUT : main SPC_EXIT_NOK");
        XZST_03EcritureTrace(XZSTC_FATAL,
                             "Echec XDG_JoindreDG(%s)", XDG_NSUPAPLG);
        /*B Si abonnement a echouee, appel de la fonction
         *	de sortie **/
        sp_sortir();
      }
    }

    XZST_03EcritureTrace(XZSTC_WARNING, "avant XZSP_01CmdApplication");
    /*A Suivant l'etat de la machine , on lance l'application
     *	complete ou reduite **/
    if ((vl_EtatMach == XZSEC_ETAT_NACTIF) ||
        (vl_EtatMach == XZSEC_ETAT_NPASSIF))
    {
      /*B Si etat machine Actif , lancement de l'appli */
      vl_ValRet =
          XZSP_01CmdApplication(vl_NomMachine, XZSPC_MARCHE);
    }
    XZST_03EcritureTrace(XZSTC_WARNING, "apres XZSP_01CmdApplication");
#if 0
    else
    {
      /*A Suivant le type de redondance , lancement de l'aplication
       *	complete ou reduite **/
      if (REDONDANCE_INTERNE == XDC_VRAI)
      {
        /*B Redondance, donc lancement de l'appli */
        vl_ValRet = XZSP_01CmdApplication (
            vl_NomMachine , XZSPC_MARCHE );
      }
      else
      {
        /*B Pas redondance, donc lancement des taches
         *	PING et TIME  **/
        if ( (vl_ValRet = sp_startTache(vl_TachePing,-1)) != XDC_OK )
        {
          /*B Pb lancement tache PING */
          XZST_03EcritureTrace(XZSTC_WARNING,
              "Pb lancement tache PING \n");
        }

        if ( (vl_ValRet = sp_startTache(vl_TacheTimer,-1)) != XDC_OK )
        {
          /*B Pb lancement tache TIMER */
          XZST_03EcritureTrace(XZSTC_WARNING,
              "Pb lancement tache TIMER \n");
        }
      }
    }
#endif

    /*
  if ( ((vl_TypeMachine == XZSCC_TYPEM_SD) || (vl_TypeMachine == XZSCC_TYPEM_SC) || (vl_TypeMachine == XZSCC_TYPEM_PCS)
     || (vl_TypeMachine == XZSCC_TYPEM_PC2)  || (vl_TypeMachine == XZSCC_TYPEM_PC2LT))
    && (vl_EtatMach != XZSEC_ETAT_NPASSIF) )
  {
    sapl_init_surv_dbserver();
  }
     */

    /* Programmation du timer Keep Alive */
    /* Uniquement pour les serveurs de donnees, serveur de communication ou PC2/PC2LT */
    if ((vl_TypeMachine == XZSCC_TYPEM_SD) || (vl_TypeMachine == XZSCC_TYPEM_SC) ||
        ((((vl_TypeMachine == XZSCC_TYPEM_PC2) || (vl_TypeMachine == XZSCC_TYPEM_PC2LT)))))
    {
      XZST_03EcritureTrace(XZSTC_WARNING, "Appel XZSC_30ProgKeepAlive %s", vl_NomSite);
      XZSC_30ProgKeepAlive(vl_NomSite);
    }
    else
    {
      /* Abonnement au Keep Alive */
      XZST_03EcritureTrace(XZSTC_WARNING, "Appel XZSC_31AbtKeepAlive %s", vl_NomSite);
      XZSC_31AbtKeepAlive(vl_NomSite);
    }

#ifdef _TIBCO_EMS
    /*imer de surveillance des abonnements*/
    sp_startTache(vl_TacheTimer, -1);
    XDG_EncodeDG2(vl_dg, XDG_WATCH, vl_NomMachine);
    XDG_EncodeDG2(vl_cle_timer, vl_dg, "timer");
    XDG_JoindreDG(vl_dg);
    if (XZSM_01DemanderProgrammation(0, 10, vl_dg, XDM_TSE_WATCH, vl_cle_timer, 10) != XDC_OK)
    {
      XZST_03EcritureTrace(XZSTC_WARNING, "Erreur demande programmation WATCH \n");
    }
#endif

    /* Recuperation adresse IP */
    XZSC_AdresseIP (vl_IP);
    /* Determiner le mode de fonctionnement mono ou bi-ecran */
    if(strncmp(vl_NomTache, SAPL_RELAIS_BI, 4) != 0)
    {
      strcpy(vl_displayPort2, "");
      /* Lit le fichier TOP_nomMachine.txt */
      if(sapl_lire_config_TOPS(CO_FIC_TOPS_DISPLAY1, vl_IP1, vl_display1, vl_port1)==XDC_OK)
      {
        XZST_03EcritureTrace(XZSTC_WARNING, "Fichier TOPS_nomMachine.txt complet, mono-ecran.");
        strcat (vl_display1, ".");
        strcat (vl_display1, vl_port1);
        strcpy (vl_displayPort1, vl_IP1);
        strcat (vl_displayPort1, ":");
        strcat (vl_displayPort1, vl_display1);
      }
      else /* Fichier TOPS_nomMachine.txt incomplet ou inexistant */
      {
        strcpy(vl_displayPort1, vl_IP);
        strcat(vl_displayPort1, ":0.0");
      }
    }
    else /* Fonctionnement bi-ecran */
    {
      /* Lit le fichier TOP_nomMachine.txt */
      if((sapl_lire_config_TOPS(CO_FIC_TOPS_DISPLAY1, vl_IP1, vl_display1, vl_port1)==XDC_OK) && 
        (sapl_lire_config_TOPS(CO_FIC_TOPS_DISPLAY2, vl_IP2, vl_display2, vl_port2)==XDC_OK))
      {
        XZST_03EcritureTrace(XZSTC_WARNING, "Fichier TOPS_nomMachine.txt complet, bi-ecran.");
        strcat (vl_display1, ".");
        strcat (vl_display1, vl_port1);
        strcpy (vl_displayPort1, vl_IP1);
        strcat (vl_displayPort1, ":");
        strcat (vl_displayPort1, vl_display1);  

        strcat (vl_display2, ".");
        strcat (vl_display2, vl_port2);
        strcpy (vl_displayPort2, vl_IP2);
        strcat (vl_displayPort2, ":");
        strcat (vl_displayPort2, vl_display2);
      }
      else /* Fichier TOPS_nomMachine.txt incomplet ou inexistant */
      {
        strcpy(vl_displayPort1, vl_IP);
        strcat(vl_displayPort1, ":0.0");
        strcpy(vl_displayPort2, "");
      }
    }
    /*Envoi message XDG_MGA_CONFIG_PO */ 
    if (!TipcSrvMsgWrite( XDG_MGA_CONFIG_PO
                        ,XDM_IdentMsg(XDM_MGA_CONFIG_PO) 
                        ,XDM_FLG_SRVMSGWRITE
    ,T_IPC_FT_STR,(T_STR) vl_IP
    ,T_IPC_FT_STR,(T_STR) vl_NomMachine
    ,T_IPC_FT_STR,(T_STR) vl_NomSite
    ,T_IPC_FT_STR,(T_STR) vl_displayPort1
    ,T_IPC_FT_STR,(T_STR) vl_displayPort2
    ,NULL) )
    {
      /* Affichage Trace */
      XZST_03EcritureTrace(XZSTC_WARNING
                            ," sapl_dir:TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,...)" 
          ,XDG_MGA_CONFIG_PO,XDM_IdentMsg(XDM_MGA_CONFIG_PO));
    }
        


    /* Attente bloquante des messages du RTServer */
    while (1)
    {
      //XZST_03EcritureTrace(XZSTC_WARNING, "attente message - 1 seconde");
      /* attente infinie d'un message */
      pl_Msg = TipcSrvMsgNext(T_TIMEOUT_FOREVER);
      vl_errno = errno;

      /* si message recu */
      if(isMessageNotNULL(pl_Msg))
      {
        /* execution du call-back associe au message et destruction du message */
        TipcSrvMsgProcess(pl_Msg);
        TipcMsgDestroy(pl_Msg);

        /* Memorisation Test Presence RTserver */
        vl_IndPre = 0;
      }
      else
      /* erreur RTWORKS */
      {
#ifdef _TIBCO_EMS
        /* surveille si la tache tstim fonctionne */
        /* car si pas tstim, pas de reveil pour verifier le fonctionnement des taches */
        if (vl_timer_tstim >= 10)
        {
          vl_timer_tstim = 0;

          for (vl_ind = 0; vl_ind < vg_sp_nb_surv_tach; vl_ind++)
          {
            /* Uniquement pour tstim, et si tstim est surveille */
            if ((strlen(pg_sp_surv_tach[vl_ind].NomTache)>0) && (!strcmp(pg_sp_surv_tach[vl_ind].NomTache,vl_TacheTimer)))
            {
              sprintf(vl_str, "%s_%s", vl_NomMachine, pg_sp_surv_tach[vl_ind].NomTache);

              consumerInfoCollection = XZEMS_03IsTopic(vl_str);
              if (consumerInfoCollection == NULL)
              {
                XZST_03EcritureTrace(XZSTC_WARNING, "sp_watch_cb : mise a 0 du status '%s'", vl_str);
                sp_dgstatus(vl_str, 0);
              }
              else
              {
                XZST_03EcritureTrace(XZSTC_WARNING, "sp_watch_cb : mise a 1 du status '%s'", vl_str);
                sp_dgstatus(vl_str, 1);
              }

              XDG_tibemsCollection_Destroy(consumerInfoCollection);
            }
          }
        }
        vl_timer_tstim++;
#else
        /* Test de Presence RTserver */
        if (vl_IndPre == 0)
        {
          if (errno)
            XZST_03EcritureTrace(XZSTC_WARNING, "Erreur RTWorks num: %d -- errno %d \n",
                                 TutErrNumGet(), vl_errno);
        }
        else if (vl_IndPre >= (XDC_EXIT_RTCLIENT / 2))
        {
          XZST_03EcritureTrace(XZSTC_WARNING, "Bouclage et arret tache tsaplg  %d \n", vl_IndPre);
          /*A Arret de la tache */
          vl_IndPre = 0;

          /* Appel fonction de sortie */
          sp_sortir();
        }

        if ((vl_rterrno = TutErrNumGet()) != T_ERR_CONN_EOF)
        {
          if (errno)
            XZST_03EcritureTrace(XZSTC_WARNING, "Erreur RTWorks num: %d -- errno %d \n",
                                 TutErrNumGet(), vl_errno);

          /*B Teste de presence du RTserver si l'erreur est
           *		T_ERR_SRV_NOT_CONNECTED */
          if (TutErrNumGet() == T_ERR_SRV_NOT_CONNECTED)
          {
            /*B Teste presence RTserver */
            /*vl_Pid_Rtserver = XZSS_20PidTache(C_NOMTACHE_RTSERVER,vg_NomAppli);*/
            vl_Pid_Rtserver = XZSS_20PidTache(C_NOMTACHE_RTSERVER);
            XZST_03EcritureTrace(XZSTC_WARNING, "Retour PID rtserver %d \n",
                                 vl_Pid_Rtserver);

            if (vl_Pid_Rtserver == 0)
            {
              /* OK la tache est inexistante */
              vl_IndPre = 0;
              sp_sortir();
            }
          }

          /* INdication Non Presence RTserver */
          if (vl_errno)
            vl_IndPre++;

          /* Attente aleatoire pour permettre une reconnexion */
          /*vl_Attente = rand()%15;
      sleep( vl_Attente );*/
        }

        /* si l'erreur est du a un manque de ressource errno=EAGAIN alors on temporise */
        if (errno == EAGAIN)
        {
          if ((vl_rterrno == T_ERR_WOULD_BLOCK) &&
              (vl_TypeMachine != XZSCC_TYPEM_SD) &&
              (vl_TypeMachine != XZSCC_TYPEM_SC) &&
              (vl_TypeMachine != XZSCC_TYPEM_PC2) &&
              (vl_TypeMachine != XZSCC_TYPEM_PC2LT) && /* ajout PC2 modif $Revision: 1.29 $ */
              (vl_TypeMachine != XZSCC_TYPEM_PCS))     /* ajout PCS modif 1.21 */
          {
            XZST_03EcritureTrace(XZSTC_WARNING, "Erreur T_ERR_TWOULD_BLOCK ; donc on sort de l'appli !");
            /*sp_sortir();
       XZST_03EcritureTrace (XZSTC_FATAL,"On sort vraiment");
       exit(1);*/
            vl_Ind_errTWOULD++;

            if (vl_Ind_errTWOULD > 500)
            {
              XZST_03EcritureTrace(XZSTC_WARNING, "Reprise a chaud de la comm ");
              vl_rterrno = TipcSrvDestroy(T_IPC_SRV_CONN_WARM);
              XZST_03EcritureTrace(XZSTC_WARNING, "Destruction connex: retour=%d Erreur RTWorks num: %d -- errno %d \n",
                                   vl_rterrno, TutErrNumGet(), errno);
              vl_rterrno = TipcSrvCreate(T_IPC_SRV_CONN_FULL); /* v1.17 */
              XZST_03EcritureTrace(XZSTC_WARNING, "Creation connex: retour=%d Erreur RTWorks num: %d -- errno %d \n",
                                   vl_rterrno, TutErrNumGet(), errno);
              /*if ( vl_rterrno == FALSE )*/ /* v1.20*/
              /* Meme si la reconnex a reussi alors on sort de l'appli pour un relance */
              {
                sp_sortir();
                XZST_03EcritureTrace(XZSTC_FATAL, "On sort vraiment");
                exit(1);
              }
              vl_Ind_errTWOULD = 0;
            }
          }
          else
          {
            XZST_03EcritureTrace(XZSTC_WARNING, "Temporisation sur erreur EAGAIN 5 secondes");
            sleep(5);
            vl_Ind_errTWOULD = 0;
          }
        }
#endif
      }
    }
  }
  else
  {
    if (vl_errno)
      XZST_03EcritureTrace(XZSTC_WARNING, "Fausse erreur RTWorks? num: %d -- errno %d \n", TutErrNumGet(), errno);
    else if (vl_IndPre > 20)
    {
      XZST_03EcritureTrace(XZSTC_FATAL, "Echec sur XZSC_01InitCnxSup");
      XZST_03EcritureTrace(XZSTC_FATAL, "main probleme d'init");
      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : main SPC_EXIT_NOK");
      XZST_02FermetureTrace();
      exit(SPC_EXIT_NOK);
    }
  }

  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : main SPC_EXIT_OK");
  sp_sortir();

  return (0);
} /* main */

/******************************************************
 * STERIA *  PROJET MIGRAZUR
 ******************************************************
 * MODULE SAPL_DIR
 ******************************************************
 * DESCRIPTION DE LA FONCTION :
 *
 *	Lance la tache passee en parametre et attend
 *	un message de synchronisation de celle ci
 *
 ******************************************************
 * SEQUENCE D'APPEL :
 */

static void sp_synchrotache(XZSCT_NomTache va_NomTache,
                            XZSCT_Datagroup va_DataGroup)

/*
 * PARAMETRES EN ENTREE : - Nom de la tache a synchroniser
 *			 - Datagroup a joindre pour obtenir
 *			   la synchronisation
 *
 * PARAMETRES EN SORTIE : aucun
 *
 * VALEUR(S) DE RETOUR : aucune
 *
 * CONDITION D'ERREUR :
 *
 * - la tache va_NomTache n'a pu etre lancee
 * - la tache va_NomTache n'a pas envoye de message de synchro
 *
 * CONDITIONS LIMITES : neant
 *
 ******************************************************/
{
  T_IPC_MSG vl_Msg;
  T_STR pl_NomDatagroup = "";
  T_INT4 vl_NbAbonnes = 0;
  int vl_Result = 0;
  XZSCT_NomMachine vl_NomMachine = "";
  int vl_Cr = 0;

  XZST_03EcritureTrace(XZSTC_FONCTION,
                       "IN : sp_synchrotache : va_NomTache='%s' va_DataGroup='%s'",
                       va_NomTache, va_DataGroup);

  /*A Jonction du datagroup XDG_NSUPAPLG_NomMachine */
  if (!XDG_JoindreDG(va_DataGroup))
  {
    XZST_03EcritureTrace(XZSTC_FONCTION,
                         "OUT : sp_synchrotache SPC_EXIT_NOK");
    XZST_03EcritureTrace(XZSTC_FATAL,
                         "Echec XDG_JoindreDG(%s)", va_DataGroup);
    /*B Si jonction a echouee , appel de la fonction de sortie */
    sp_sortir();
  }

  /*A Lancement de la tache */
  /* Modif V1.14 */
  /*vl_Result = sp_startTache( va_NomTache, -1 ) ;*/
  XZSC_07NomMachine(vl_NomMachine);
  vl_Cr = sp_startTache_Inter(va_NomTache, vl_NomMachine);
  XZST_03EcritureTrace(XZSTC_WARNING, "apres sp_startTache_Inter %d", vl_Cr);
  if (vl_Cr > 0)
  {
    vl_Result = XDC_OK;
  }
  else
  {
    vl_Result = XDC_NOK;
  }

#ifndef _TIBCO_EMS
  /* Si la tache a ete lancee */
  if (vl_Result == XDC_OK)
  {
    /*A Attente d'un message de type XDM_TSE_MAJD
     *	si lancement de la tache OK */
    XZST_03EcritureTrace(XZSTC_WARNING, "avant TipcSrvMsgSearchType");
    vl_Msg =
        TipcSrvMsgSearchType(SPC_SYNC_TIME_OUT,
                             XDM_IdentMsg(XDM_TSE_MAJD));
    XZST_03EcritureTrace(XZSTC_WARNING, "apres TipcSrvMsgSearchType");

    /*A S'il n'y a pas de message a lire et
     *			que le timeout est ecoule */
    if (vl_Msg == NULL)
    {
      XZST_03EcritureTrace(XZSTC_WARNING,
                           "OUT : sp_synchrotache SPC_EXIT_NOK");
      XZST_03EcritureTrace(XZSTC_WARNING,
                           "Tache '%s' synchro non recu",
                           va_NomTache);
      /*B Si pas de message XDM_TSE_MAJD alors
       *	appel fonction de sortie **/
      sp_sortir();
    }
    else
    {
      /*B Prise en compte des parametres du message
       * si message recu */
      XZST_03EcritureTrace(XZSTC_WARNING, "avant tipc");
      TipcSrvMsgProcess(vl_Msg);
      TipcMsgDestroy(vl_Msg);
      XZST_03EcritureTrace(XZSTC_WARNING, "apres tipc");

      /*A Attente d'un message de type XDM_TSE_MAJE
       *	si lancement de la tache OK */
      vl_Msg =
          TipcSrvMsgSearchType(SPC_SYNC_TIME_OUT,
                               XDM_IdentMsg(XDM_TSE_MAJE));

      /*A S'il n'y a pas de message a lire et
       *                       que le timeout est ecoule */
      if (vl_Msg == NULL)
      {
        XZST_03EcritureTrace(XZSTC_WARNING,
                             "OUT : sp_synchrotache SPC_EXIT_NOK");
        XZST_03EcritureTrace(XZSTC_FATAL,
                             "Tache '%s' synchro non recu", va_NomTache);
        /*B Si pas de message XDM_TSE_MAJE alors
         *       appel fonction de sortie **/
        sp_sortir();
      }
      else
      {
        /*B Prise en compte des parametres du message
         * si message recu */
        TipcSrvMsgProcess(vl_Msg);
        TipcMsgDestroy(vl_Msg);
      }
    }
  }
  else
  {
    XZST_03EcritureTrace(XZSTC_WARNING,
                         "OUT : sp_synchrotache SPC_EXIT_NOK");

    /******* TESTE ******/
    XZST_03EcritureTrace(XZSTC_FATAL,
                         "Tache '%s' non lancee", va_NomTache);
    /*B Appel fonction de sortie si lancement de la tache
     *	non OK **/
    sp_sortir();
  }
  /*B  Quitte la jonction au datagroup NSUPAPLG_NomMachine */
  XDG_QuitterDG(va_DataGroup);
#endif

  XZST_03EcritureTrace(XZSTC_WARNING, "OUT : sp_synchrotache");

} /* sp_synchrotache */

/******************************************************
 * STERIA *  PROJET MIGRAZUR
 ******************************************************
 * MODULE SAPL_DIR
 ******************************************************
 * DESCRIPTION DE LA FONCTION :
 *
 *	Liberation des ressources
 *	Arret de l'application
 *	fin de l'execution
 *
 ******************************************************
 * SEQUENCE D'APPEL :
 */

void sp_sortir()
{
  XZSCT_Datagroup vl_datagroup = "";
  XZSCT_NomMachine vl_NomMachine = "";
  int vl_ind = 0;
  int vl_ValRet = 0;
  XZSCT_NomTache vl_maTache = "";
  XZSCT_TypeMachine vl_TypeMachine = 0;
  /* Chaine a envoyer a l'interpreteur de commande RTWorks */
  char CommandeRTW[100] = "\0";
  int vl_EtatMach = 0;

#define C_ARRET_RTSERVER "rtserver -server_names `uname -n` -stop"
#define C_ARRET_RTLM "rtlm -stop"

  XZST_03EcritureTrace(XZSTC_FONCTION, "IN : sp_sortir");

  /* recupere nom de tache locale */
  XZSC_06NomTache(vl_maTache);

#if 0
  /* On detruit la connexion completement */
  if (!TipcSrvDestroy(T_IPC_SRV_CONN_NONE))
  {
    XZST_03EcritureTrace(   XZSTC_WARNING,
        "Erreur deconnexion serveur \n");
  }
#endif

  /* Envoie du signal SIGKILL a toutes les taches */
  vl_ind = 0;
  while (vl_ind < vg_sp_nb_surv_tach)
  {
    if ((pg_sp_surv_tach[vl_ind].Pid != 0) &&
        (strcmp(pg_sp_surv_tach[vl_ind].NomTache, vl_maTache) != 0))
    {
      /*		vl_ValRet = kill(pg_sp_surv_tach[vl_ind].Pid,SIGKILL);*/
      vl_ValRet = 0;
      if (vl_ValRet == 0)
      {
        XZST_03EcritureTrace(XZSTC_WARNING,
                             "SIGKILL bien envoye a %s (PID=%d)",
                             pg_sp_surv_tach[vl_ind].NomTache,
                             pg_sp_surv_tach[vl_ind].Pid);
      }
      else
      {
        XZST_03EcritureTrace(XZSTC_INFO,
                             "Erreur lors de l'envoie de SIGKILL a %s (PID=%d)",
                             pg_sp_surv_tach[vl_ind].NomTache,
                             pg_sp_surv_tach[vl_ind].Pid);
      }
    }

    vl_ind = vl_ind + 1;
  }

  /* Type de Machine */
  XZSC_05TypeMachine(&vl_TypeMachine);

  /*A Recuperation de l'etat de la machine **/
  if ((vl_ValRet = XZSE_09EtatMachine(&vl_EtatMach)) != XDC_OK)
  {
    /*B Pb recuperation etat de la machine */
    XZST_03EcritureTrace(XZSTC_WARNING,
                         "Pb de recuperation etat machine \n");
  }

  /* fin du process si non SC et non SD */
  if ((vl_TypeMachine != XZSCC_TYPEM_SD) &&
      (vl_TypeMachine != XZSCC_TYPEM_SC) &&
      (vl_TypeMachine != XZSCC_TYPEM_PCS) &&
      (vl_TypeMachine != XZSCC_TYPEM_PC2) &&
      (vl_TypeMachine != XZSCC_TYPEM_PC2LT)) /* Ajout PCS modif 1.21 */ /* Ajout PC2 modif 1_22*/
  {
    XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : sp_sortir");
    /* On sort de la tache */
    /*	exit(SPC_EXIT_OK) ;*/
  }
  else
  {

    if (vl_EtatMach != XZSEC_ETAT_NPASSIF)
    {
      /*A arret du rtserver et du rtlm */
      /*system(C_ARRET_RTSERVER);
      system(C_ARRET_RTLM);*/

      XZST_03EcritureTrace(XZSTC_WARNING, "Repositionnement de l'option \
      server_start_max_tries a 2 pour relance RTserver \n");

      /*  Ajoute option de relance RTserver */
      /* Positionement de l'option server_start_max_tries a 2 */
      sprintf(CommandeRTW, "setopt server_start_max_tries %d", 2);
      TutCommandParseStr(CommandeRTW);
    }
  }

  XZST_03EcritureTrace(XZSTC_FATAL, "OUT : sp_sortir");
  exit(0);
} /* Fin sp_sortir */

/******************************************************
 * STERIA *  PROJET MIGRAZUR
 ******************************************************
 * MODULE SAPL_DIR
 ******************************************************
 * DESCRIPTION DE LA FONCTION :
 *
 *	Lit TOPS_NomMachine.txt dans le dossier
 *	fichiers/adm/ pour récupérer: NomMachine  
 *   + adresseIP + DisplayPort 1 ou 2 (selon va_param)
 *
 ******************************************************
 * SEQUENCE D'APPEL :
 */
int sapl_lire_config_TOPS(XDY_Texte va_param, XDY_Texte va_IP, XDY_Texte va_display, XDY_Texte va_port)
{
	XDY_Texte vl_NomFichier;
	XDY_Texte pl_param;
	FILE* pl_fp;
	XDY_Texte pl_ligne;
	XZSCT_NomMachine  pl_NomMachine = "\0";
	XDY_Texte vl_IP,vl_display,vl_port;
		
	XZSC_07NomMachine(pl_NomMachine);
	strcpy (vl_NomFichier,CO_FIC_TOPS_GEN_PATH);
	strcat(vl_NomFichier,pl_NomMachine);
	strcat(vl_NomFichier,".txt");
	
	/* Ouverture du fichier de config */
	if ( (pl_fp = fopen(vl_NomFichier, "r")) == NULL ) {
			XZST_03EcritureTrace(   XZSTC_WARNING, "erreur de lecture du fichier %s !"
									,vl_NomFichier );
			return(XDC_NOK);
	}

	/* On lit les lignes du fichier une par une */
	/* ------------------------------------------------------ */

	while ( fgets(pl_ligne, 200, pl_fp) != NULL) {
			/* On ignore les lignes commencant par un # */
			if ( pl_ligne[0] == '#' )
					continue;

			/* Pour les autres lignes on recherche la frequence de notre tache */
			if (sscanf(pl_ligne,"%s\t%s\t%s\t%s", pl_param, &vl_IP,&vl_display,&vl_port)==4)
			{
				if ( !strcmp(pl_param,va_param) ) 
				{
					strcpy(va_IP,vl_IP);
					strcpy(va_display,vl_display);
					strcpy(va_port,vl_port);
					XZST_03EcritureTrace(XZSTC_WARNING, "lecture fichier config TOP param fichier %s, ligne %s = %s:%s.%s",vl_NomFichier,pl_param,va_IP,va_display,va_port);
					fclose(pl_fp);
					return (XDC_OK);
				}				
			}
			else 
			{
				XZST_03EcritureTrace(XZSTC_WARNING, "le fichier %s n'a pas le format demande !",vl_NomFichier );
			}
	}
	fclose(pl_fp);
	return (XDC_NOK);
}
