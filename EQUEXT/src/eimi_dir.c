/*Fichier : $Id: eimi_dir.c,v 1.8 2021/05/04 13:25:02 pc2dpdy Exp $      Release : $Revision: 1.8 $        Date : $Date: 2021/05/04 13:25:02 $
------------------------------------------------------
 * GTIE *  PROJET MIGRAZUR
------------------------------------------------------
 * SOUS-SYSTEME : EQUEXT
------------------------------------------------------
 * MODULE TELMi * FICHIER eimi_dir.c
------------------------------------------------------
 * DESCRIPTION DU MODULE :
 *
 * Programme principale de la tache TELMi
 *
------------------------------------------------------
 * HISTORIQUE :
 *
 * JMG	04/03/18 : Creation controleurs de feux CTRL_FEUX 1.1 DEM1284
 * JMG	27/02/19 : correction port 34000 1.2
 * RGR 17/05/19 : DEM1331 - Ajout Mecanisme KEEPALIVE 1.3
 * GR  09/07/2020 : EMS 1.4
* jmg	05/01/2021 : bloc inter dem sae 215 1.5
* jmg 03/02/21 gestion port esclave pour reconnexio, 1.6
------------------------------------------------------*/

/* fichiers inclus */
#include <xzsem.h>

#include "ex_mdon.h"
#include "ex_mrtf.h"

#include "eimi_dir.h"
#include "eimi_mdon.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */
XZEXT_IP_Config vm_config;
T_Tampon tg_tampons[C_MAX_TAMPON];
int vg_presence_esclave;

/* declaration de fonctions externes */

/* definitions fonctions internes */
int edir_detecte_type_sock(void);

/* definition de fonctions internes */

void edir_traitement_msg_sock(XZEXT_Msg_Socket *);
void edir_traitement_dcnx(void);
void edir_traitement_liste_cmdes_LCR(void);
void edir_reveil_cb(T_IPC_CONN, T_IPC_CONN_PROCESS_CB_DATA, T_CB_ARG);

/*void edir_traitement_liaison_ST( void );*/

/*X*/
/* Fichier : $Id: eimi_dir.c,v 1.8 2021/05/04 13:25:02 pc2dpdy Exp $      Release : $Revision: 1.8 $        Date : $Date: 2021/05/04 13:25:02 $
------------------------------------------------------
 * SERVICE RENDU :
 * Programme principal
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

int main(int argc, char **argv)

/*
 * ARGUMENTS EN ENTREE :
 *   argc : Nombre d'arguments
 *   argv : liste des arguments
 *
 * ARGUMENTS EN SORTIE :
 *   Aucun
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
 *   Programme principale tache TELMi.
 *
------------------------------------------------------*/
{

  int vl_ValRet = XDC_OK;              /* valeur retour */
  int vl_EtatCnx = XZEXC_CONNEC_UNDEF; /* etat connexion socket */
  XZEXT_Msg_Socket vl_MsgSock;         /* struct. creation msg. socket */
  XZEXT_MASK *pl_Mask;
  XDY_Texte vl_Texte;
  XZEXT_ADR_SOCK pl_SrvTeimi;
  XDY_Datagroup pl_dg_timer;
  XZEXT_MASK *pl_ListSock = NULL;
  XZSMT_CleProg vl_cle;
  XDY_NomSite pl_NomSite = "";
  int vl_indice;

  /*
   * sequence initialisation tache TELMi
   */

  /* init a zero des descs. sockets specifiques taches TELxi */
  vg_DesSocks.curr = XZEXC_SOCKET_NULL;
  vg_DesSocks.ect = XZEXC_SOCKET_NULL;
  vg_DesSocks.lser = XZEXC_SOCKET_NULL;
  vg_DesSocks.rtsrv = XZEXC_SOCKET_NULL;
  pg_ListeCmdesLCR = NULL;
  vg_presence_esclave = XDC_FAUX;
  vg_ConfigE.No_port = -1;

  /* Recuperation nom machine */
  strcpy(vg_Config.Nom_machine, argv[1]);

  for (vl_indice = 0; vl_indice < C_MAX_TAMPON; vl_indice++)
  {
    strcpy(tg_tampons[vl_indice].adresse, "");
    tg_tampons[vl_indice].port = -1;
    strcpy(tg_tampons[vl_indice].tampon, "");
  }
  /* Recuperation no. ligne serie */
  /* en realite c est le type d equipement*/
  vg_Config.no_ligne = atoi(argv[2]);

  /*A
   *  initialisation semaphore
   */
  sem_init(&vg_semaphore, 0, 1);

  /* SI Connexion au RTserver et initialisation des variables echoue */
  if (XZSC_01InitCnxSup(NULL,
                        argv[0],
                        XZSCC_INTERDIRE,
                        XZSCC_AUTORISER,
                        XZSCC_INTERDIRE,
                        ecfg_sortir,
                        "/dev/null") != XDC_OK)
  {
    XZST_03EcritureTrace(XZSTC_FATAL, "Ouverture connexion avec le RTserver impossible!!!");
    exit(XDC_NOK);
  }

  XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL | XZSTC_WARNING | XZSTC_INFO | XZSTC_FONCTION | XZSTC_INTERFACE, XZSTC_NECRAN);
  /*XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_WARNING|XZSTC_INFO|XZSTC_FONCTION|XZSTC_DEBUG1|XZSTC_DEBUG2|XZSTC_DEBUG3|XZSTC_INTERFACE,XZSTC_NECRAN);*/
  XZST_03EcritureTrace(XZSTC_WARNING, "Ouverture cnx. RTServer reussie ...");

  /* Creation du repertoire pour les sockets */
  ex_rep_socket(vg_Config.Nom_machine);

  XZSM_00InitServices();
  sprintf(vl_cle, "TEMIP_%d_%s", vg_Config.no_ligne, vg_Config.Nom_machine);
  sprintf(pl_dg_timer, "%s_%d_%s", XDG_ETEMIP, vg_Config.no_ligne, vg_Config.Nom_machine);
  XZSM_02AnnulerProgrammation(vl_cle, pl_dg_timer, 1);

  if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_ANEUTR_TIMER),
                             (T_IPC_CONN_PROCESS_CB_FUNC)edir_reveil_cb,
                             (T_CB_ARG)NULL) == NULL)
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "pb appel TipcSrvProcessCbCreate sur XDM_ANEUTR_TIMER");
  }
  sprintf(pl_dg_timer, "%s_%d_%s", XDG_ETEMIP, vg_Config.no_ligne, vg_Config.Nom_machine);
  XDG_JoindreDG(pl_dg_timer);

  /*
   * connexion soket en mode serveur pour comms. avec la tache traitement
   */
  /* SI connexion echoue */
  sprintf(pl_SrvTeimi, "%s%d", XZEXC_ADR_SOCK_TEIM, vg_Config.no_ligne);
  if (ex_cnx_serveur(pl_SrvTeimi, &vg_SockList, &vg_DesSocks.ect) != XDC_OK)
  {
    /* ALORS */
    /* ecriture trace : " Connexion socket ecoute mode serveur impossible !" */
    XZST_03EcritureTrace(XZSTC_FATAL, " Connexion socket ecoute mode serveur impossible !");
    /* sortie XDC_NOK */
    exit(XDC_NOK);
  }

  for (pl_ListSock = vg_SockList; pl_ListSock != NULL; pl_ListSock = pl_ListSock->Suivant)
  {
    if (pl_ListSock->desc_sock == vg_DesSocks.ect)
    {
      pl_ListSock->Protocole = XZECC_PROT_SERVEUR;
      break;
    }
  }

  XZST_03EcritureTrace(XZSTC_FONCTION, " Nom machine %s, type eqt %d ",
                       vg_Config.Nom_machine, vg_Config.no_ligne);

  /* Recuperation du site de la machine */
  if ((vl_ValRet = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "Recuperation du Nom du site impossible .");
  }

  /* Abonnement au DataGroup KEEPALIVE */
  XZSC_31AbtKeepAlive(pl_NomSite);

  /*
   * Boucle infinie FAIRE TOUJOURS
   */
  while (1)
  {

    /* traitement des eventuelles cmdes. LCR empilees ds. la liste de cmdes. non traitees */
    /*edir_traitement_liste_cmdes_LCR();*/

    /* SI attente message sur liste sockets : ecoute, lserie, donnees client echoue */
    if (ex_attendre_msg(vg_DesSocks.ect, &vg_SockList, &vg_DesSocks.curr) != XDC_OK)
    {
      /* ALORS ecriture trace :  " Probleme attente message sur socket(s)." */
      XZST_03EcritureTrace(XZSTC_WARNING, " Probleme attente message sur socket(s).");
      ex_ConnexionSocketRTserver(&vg_DesSocks.rtsrv, vg_DesSocks.curr, &vg_SockList);
    }
    else
    {
      ex_ConnexionSocketRTserver(&vg_DesSocks.rtsrv, 0, &vg_SockList);

      /* SI socket active = socket RTServer */
      if (vg_DesSocks.curr == vg_DesSocks.rtsrv)
      {
        XZST_03EcritureTrace(XZSTC_FONCTION, "eimi_dir.x : appel primitive standard lecture RTWorks");

        /* ALORS appel primitive standard lecture RTWorks */
      }
      /* SINON SI evenement sur une socket diff. de la socket d'ecoute
       *  (si. evt. sur socket ecoute <=> cnx. d'un client geree par ex_attendre_msg())
       *  tout est O.K. => lecture sur socket active */
      else if (vg_DesSocks.curr != XZEXC_NO_EVT        /* une socket active */
               && vg_DesSocks.curr != vg_DesSocks.ect) /* cas connexion client */
      {
        XZST_03EcritureTrace(XZSTC_FONCTION, "eimi_dir.x : evenement sur une socket diff. de la socket d'ecoute...\n\n");

        /* detection adresse socket active */
        ecom_detecte_clnt(&vg_DesSocks, &vg_SockList);

        XZST_03EcritureTrace(XZSTC_FONCTION, "eimi_dir.x : lecture sur socket %d active\n",
                             vg_DesSocks.curr);

        /* SI lecture sur socket active echoue */
        if (ex_lire_msg(vg_DesSocks.curr, vl_MsgSock.msg, &vl_MsgSock.len,
                        &vg_SockList, &vl_EtatCnx) != XDC_OK)
        {
          /* ALORS ecriture trace :  Probleme lecture socket adresse <vg_DesSocks.curr_addr> */
          XZST_03EcritureTrace(XZSTC_WARNING, " Probleme lecture socket adresse %s...",
                               vg_DesSocks.curr_addr);
        }

        /* SINON */
        else
        {
          XZST_03EcritureTrace(XZSTC_WARNING, " 3");
          /* CAS etat connexion socket active */
          switch (vl_EtatCnx)
          {
          /* cas : XZEXC_DECONNEC */
          case XZEXC_DECONNEC:

            XZST_03EcritureTrace(XZSTC_WARNING, "Deconexion <%s>...\n", vg_DesSocks.curr_addr);

            /* appel fonction traitement dcnxion : edir_traitement_dcnx() */
            edir_traitement_dcnx();
            break;

            /* cas : XZEXC_CONNEC */
          case XZEXC_CONNEC:

            XZST_03EcritureTrace(XZSTC_FONCTION, "Connexion <%s>...\n", vg_DesSocks.curr_addr);

            /* appel fonction trait. mess. recu edir_traitement_msg_sock( &vl_MsgSock )*/
            edir_traitement_msg_sock(&vl_MsgSock);
            break;

            /* cas par defaut */
          default:
            /* ecriture trace : Etat connexion socket non defini..." */
            XZST_03EcritureTrace(XZSTC_WARNING, " Etat connexion socket non defini...");
            break;
          }
          /* FIN CAS( vl_EtatCnx ) */
        }
        /* FIN SI( ex_lire_msg()... SINON... */
      }
      /* FIN SI socket server SINON SI( socket active autre que socket ecoute ) */
    }
    /* FIN SI( ex_attendre()... SINON.. */
  }
  /* FIN FAIRE TOUJOURS */
}

/*------------------------------------------------------
 * SERVICE RENDU :
 * Traitement du message receptionne par eimi_dir sur la socket active.
 *
------------------------------------------------------
 * SEQUENCE D'APPEL :
 *
 *
 * void edir_traitement_msg_sock( XZEXT_Msg_Socket *pa_MsgSock )
 *
 * ARGUMENTS EN ENTREE :
 *
 * pa_DesSocks	: pointeur sur les descs. sockets specifiques tache TELMi.
 * pa_MsgSock	: pointeur sur le message lu par eimi_dir.
 *
 * ARGUMENTS EN SORTIE :
 * NEANT
 *
 * CODE RETOUR :
 *
 * NEANT
 *
 * CONDITION D'UTILISATION
 * - les champs de pa_DesSocks doivent contenir les descs. des sockets ouverts par eimi_dir.
 * - pa_MsgSock doit contenir un message
 *
 * FONCTION
 *
 * - identification de la socket active ( serveur ou client ).
 * - traitement du message en fonction de la socket active.
 * - reinit, sortie ou envoi message vers une des sockets en fonction du resultat
 *   du traitement.
 *
------------------------------------------------------*/
void edir_traitement_msg_sock(XZEXT_Msg_Socket *pa_MsgSock)
{

  XZEXT_Cmde_LCR_IP vl_CmdeLCR;    /* structure depouillement message recu sur socket */
  XZEXT_Cmde_LCR_IP vl_CmdeLCR2;   /* structure depouillement message recu sur socket */
  XZEXT_Cmde_LCR_IP vl_cmd_purge;  /* structure depouillement message recu sur socket */
  XZEXT_Cmde_DIASER vl_CmdeDIASER; /* structure depouillement message recu sur socket */
  XDY_Texte vl_Texte;
  int vl_ValRet;
  /* buffer stokage trame lisible */
  char *pl_TrameLisible;
  XDY_AdresseIP vl_adresseIP;
  XDY_PortIP vl_port;
  char vl_protocole;
  XZEXT_Liste_Cmdes_LCR_IP *vl_liste = NULL;
  int vl_TypeTrame;
  XDY_Horodate vl_horodate;
  int vl_js;
  int vl_indice;
  int vl_purge = XDC_NOK;
  XZEXT_Msg_Socket vl_MsgSock;
  XZSMT_Horodate vl_Horodate; /* horodate en format lisible */

  /*
   * traitement message en fonction du type de la socket active
   */
  /* CAS type socket active */
  XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME, &vl_js, &vl_horodate);

  switch (edir_detecte_type_sock())
  {
  case XZEXC_SOCK_ACT_DONN: /* cas XZEXC_SOCK_ACT_DONN */
    /* detection type message recu d'un client TEzzz sur une socket donnees */

    /* SI demande d'arret ALORS appel ecfg_sortir() */
    if (strstr(pa_MsgSock->msg, XZEXC_DEMANDE_ARRET) != NULL)
    {
      XZST_03EcritureTrace(XZSTC_WARNING, "edir_traitement_msg_sock : demande arret TELMi %d",
                           vg_Config.no_ligne);

      ecfg_sortir();
    }
    /* SINON SI demande envoi trame LCR sur ligne serie geree
     *  ALORS appel epro_envoi_trame_LCR() */
    else if (strstr(pa_MsgSock->msg, XZEXC_DEMANDE_LCR) != NULL)
    {
      /*rajouter le cas de XZEXC_TRAME_TELEI_ACK ou XZEXC_TRAME_TELEI_NAK = a envoyer l'acquitt. indiquee*/
      if (sscanf(&(pa_MsgSock->msg[strlen(XZEXC_DEMANDE_LCR)]), "%d", &vl_TypeTrame) != 1)
      {
        /* ALORS trace " Demande LCR incorrecte <message> */
        XZST_03EcritureTrace(XZSTC_WARNING, "elei : Demande LCR incorrecte :\n <%s>\n", pa_MsgSock->msg);
      }

      XZST_03EcritureTrace(XZSTC_WARNING, "type trame %d", vl_TypeTrame);
      if ((vl_TypeTrame == XZEXC_TRAME_TELEI_ACK) || (vl_TypeTrame == XZEXC_TRAME_TELEI_NAK))
      {
        ecom_extraire_cmd_LCR_IP(pa_MsgSock, &vl_CmdeLCR, vg_DesSocks.curr);
        vl_CmdeLCR.type_msg = XZEXC_MSG_ACQUIT;
        sprintf(vl_CmdeLCR.rgs, "%c",
                vl_TypeTrame == XZEXC_TRAME_TELEI_ACK ? XZEXC_CAR_ACK : XZEXC_CAR_NAK);
        sprintf(vl_CmdeLCR.ack, "%s%d", vl_CmdeLCR.rgs, XZEXC_NOBL_REQ_TEDI);
        vl_CmdeLCR.horodate = vl_horodate;
        epro_envoi_trame_LCR_IP(&vl_CmdeLCR, FALSE);
      }
      /* SI depouillement message recu sur socket donnees : ecom_extraire_cmd_LCR_IP() OK */
      else if (ecom_extraire_cmd_LCR_IP(pa_MsgSock, &vl_CmdeLCR, vg_DesSocks.curr) == XDC_OK)
      {
        /*purge les anciennes commandes*/
        vl_purge = 0;
        vl_purge = ecom_purge_cmd(vl_CmdeLCR.adresseIP,
                                  vl_CmdeLCR.port,
                                  &pg_ListeCmdesLCR, &vl_cmd_purge);

        /*recherche si deja une commande en cours pour cet equipement */
        if (ecom_existe_cmd_LCR_IP(vl_CmdeLCR.adresseIP,
                                   vl_CmdeLCR.port,
                                   pg_ListeCmdesLCR, &vl_CmdeLCR2) != XDC_OK)
        {
          /* ALORS epro_envoi_trame_LCR() */
          XZST_03EcritureTrace(XZSTC_FONCTION, "pas de cmd en cours => on envoie");
          epro_envoi_trame_LCR_IP(&vl_CmdeLCR, TRUE);
          XZST_03EcritureTrace(XZSTC_WARNING, "apres epro_envoi_trame_LCR_IP");
        }
        else
        {
          if (vl_purge > 0)
          {
            XZST_03EcritureTrace(XZSTC_FONCTION, "on a purge donc on envoie a TEXxx un CR NON TRANSMIS");
            sprintf(vl_MsgSock.msg, "rep. teimi %s %d %d %d %s %d %d %s %s %s",
                    ecom_lire_horodate(&vl_Horodate),
                    1, 1,
                    vl_cmd_purge.no_cmd,
                    vl_cmd_purge.adresseIP,
                    vl_cmd_purge.type_cmd,
                    XZEXC_TRAME_NON_TRANSM,
                    XZEXC_SEP_TRAME,
                    vl_cmd_purge.cmd_LCR,
                    XZEXC_FIN_MSG_SOCK);
            vl_MsgSock.len = (int)strlen(vl_MsgSock.msg);
            sem_wait(&vg_semaphore);
            if (write(vl_cmd_purge.sock_clnt, vl_MsgSock.msg, vl_MsgSock.len) != vl_MsgSock.len)
            {
              XZST_03EcritureTrace(XZSTC_WARNING, "eimi_dir : Emission message <%s> vers client TEzzz impossible.",
                                   vl_MsgSock.msg);
            }
            sem_post(&vg_semaphore);

            XZST_03EcritureTrace(XZSTC_FONCTION, "on a purge donc on envoie la cmd suivante");
            epro_envoi_trame_LCR_IP(&vl_CmdeLCR2, FALSE);
          }
          XZST_03EcritureTrace(XZSTC_FONCTION, "cmd en cours => on stocke cmd %d", vl_CmdeLCR.no_cmd);
          vl_CmdeLCR.horodate = vl_horodate;
          ecom_append_cmd_LCR_IP(&vl_CmdeLCR, &pg_ListeCmdesLCR);
          /*epro_armer_timeout(vl_CmdeLCR.adresseIP);*/
        }
      }
      /* SINON return */
      else
      {
        return;
      }
    }
    else if (strstr(pa_MsgSock->msg, XZEXC_DEMANDE_DIASER) != NULL)
    {
      /* SI depouillement message recu sur socket donnees : ecom_extraire_cmd_DIASER OK */
      if (ecom_extraire_cmd_DIASER(pa_MsgSock, &vl_CmdeDIASER, vg_DesSocks.curr) == XDC_OK)
      {
        /* ALORS epro_envoi_trame_DIASER() */
        epro_envoi_trame_DIASER(&vl_CmdeDIASER);
      }
      /* SINON return */
      else
      {
        return;
      }
    }
    else if (strstr(pa_MsgSock->msg, XZEXC_CONFIG_IP) != NULL)
    {
      if (sscanf(&(pa_MsgSock->msg[strlen(XZEXC_CONFIG_IP)]), "%s %d %c",
                 vl_adresseIP,
                 &vl_port, &vl_protocole) != 3)
      {
        XZST_03EcritureTrace(XZSTC_WARNING, "edir_traitement_msg_sock config ip incorrecte %s",
                             pa_MsgSock->msg);
        return;
      }
      XZST_03EcritureTrace(XZSTC_FONCTION, "config ip %s %d %c", vl_adresseIP, vl_port, vl_protocole);
      strcpy(vg_Config.adresseIP, vl_adresseIP);
      vg_Config.No_port = vl_port;
      vg_Config.protocole = vl_protocole;
      if (vl_protocole == XZECC_PROT_LCR_E)
      {
        vg_presence_esclave = XDC_VRAI;
        vm_config = vg_Config;
        vg_ConfigE = vg_Config;
      }

      if (vl_protocole == XZECC_PROT_LCR)
      {
        for (vl_indice = 0; vl_indice < C_MAX_TAMPON; vl_indice++)
        {
          if (tg_tampons[vl_indice].port == -1)
            break;
        }
        strcpy(tg_tampons[vl_indice].adresse, vl_adresseIP);
        tg_tampons[vl_indice].port = vl_port;
        strcpy(tg_tampons[vl_indice].tampon, "");
      }
      if (ecom_init_IP(&vg_Config, &vg_SockList, &vg_DesSocks))
      {
        XZST_03EcritureTrace(XZSTC_WARNING, "pb connexion %s %d",
                             vl_adresseIP, vl_port);
        envoyer_trame_dai_ko(0, vl_adresseIP);
        return;
      }
    }

    /* SINON SI demande inconnue ( diff. de ARRET, INIT ou LCR )
     *  ALORS ecriture trace : " Recu demande TEzzz inconnue : <demande_TEzzz>" */
    else
    {
      XZST_03EcritureTrace(XZSTC_WARNING, " Recu demande TEzzz inconnue : %s", pa_MsgSock->msg);
    }
    break;

  case XZEXC_SOCK_ACT_LSER: /* cas XZEXC_SOCK_ACT_LSER */
  case XZEXC_SOCK_ACT_IP_LCR:
    /* Normallement reception reponses equps. ds. epro_envoi_trame_LCR().
     *  si socket ligne serie active <=> reception reponse arrivee en retard.
     *  => ecriture trace : " Reponse TEDI/LCR perdue <trame_TEDI/LCR> "
     */
    epro_traitement_msg_ip_lcr_e(pa_MsgSock, XDC_FAUX);
    break;

  case XZEXC_SOCK_ACT_IP_LCR_E:
    /* message equipement*/
    epro_traitement_msg_ip_lcr_e(pa_MsgSock, XDC_VRAI);
    break;

  case XZEXC_SOCK_ACT_IP_DIASER:
    /*decodage trame DIASER*/
    /*et envoi des infos a TExx*/
    epro_traitement_msg_ip_diaser(pa_MsgSock);
    XZST_03EcritureTrace(XZSTC_WARNING, "apres epro_traitement_msg_ip_diaser");
    break;

  default: /* defaut ( XZEXC_SOCK_ACT_UNDEF ) */
    break;
  }
}

/*X*/
/*------------------------------------------------------
 * SERVICE RENDU :
 * Detection type de la socket active
 *
------------------------------------------------------
 * SEQUENCE D'APPEL :
 *
 *
 * int edir_detecte_type_sock( void )
 *
 * ARGUMENTS EN ENTREE :
 *
 * NEANT : utilise la var. globale vg_DesSocks.
 *
 *
 * ARGUMENTS EN SORTIE :
 * NEANT
 *
 * CODE RETOUR :
 *
 *  XZEXC_SOCK_ACT_UNDEF	: si connexion client = evnmt. sur socket ecoute vg_DesSocks.ect
 *  XZEXC_SOCK_ACT_LSER	: si evnmt. sur socket ligne serie geree
 *  XZEXC_SOCK_ACT_DONN	: si evnmt. sur un socket de comms. avec un client
 *
 * CONDITION D'UTILISATION
 *
 *
 * FONCTION
 *
 *
------------------------------------------------------*/
int edir_detecte_type_sock(void)
{
  XZEXT_MASK *pl_ListSock = NULL;

  static char *version = "$Id: eimi_dir.c,v 1.8 2021/05/04 13:25:02 pc2dpdy Exp $ : edir_detecte_type_sock";
  int vl_TypeSockAct = XZEXC_SOCK_ACT_UNDEF;

  /*
   * detection type socket active
   */
  /* SI socket RTServer active ALORS RETURN XZEXC_SOCK_ACT_RTSRV */
  if (vg_DesSocks.curr == vg_DesSocks.rtsrv)
  {
    return (XZEXC_SOCK_ACT_RTSRV);
  }
  /* SI socket ligne serie active ALORS return XZEXC_SOCK_ACT_LSER */
  if (vg_DesSocks.curr == vg_DesSocks.lser)
  {
    return (XZEXC_SOCK_ACT_LSER);
  }
  /* SI socket ecoute => connexion client => ALORS return XZEXC_SOCK_ACT_UNDEF */
  if (vg_DesSocks.curr == vg_DesSocks.ect)
  {
    return (XZEXC_SOCK_ACT_UNDEF);
  }

  for (pl_ListSock = vg_SockList; pl_ListSock != NULL; pl_ListSock = pl_ListSock->Suivant)
  {
    XZST_03EcritureTrace(XZSTC_FONCTION, " socket %d %s '%c'", pl_ListSock->desc_sock, pl_ListSock->adresse,
                         pl_ListSock->Protocole);
  }

  for (pl_ListSock = vg_SockList; pl_ListSock != NULL; pl_ListSock = pl_ListSock->Suivant)
  {
    XZST_03EcritureTrace(XZSTC_FONCTION, " socket %d %s '%c'", pl_ListSock->desc_sock, pl_ListSock->adresse,
                         pl_ListSock->Protocole);
    if ((pl_ListSock->Protocole != XZECC_PROT_DIASER) && 
        (pl_ListSock->Protocole != XZECC_PROT_LCR) && 
        (pl_ListSock->Protocole != XZECC_PROT_LCR_E) && 
        (pl_ListSock->Protocole != XZECC_PROT_MIGRAZUR) && 
        (pl_ListSock->Protocole != XZECC_PROT_SERVEUR))
      pl_ListSock->Protocole = XZECC_PROT_MIGRAZUR;

    if (pl_ListSock->desc_sock == vg_DesSocks.curr)
    {
      if (pl_ListSock->Protocole == XZECC_PROT_DIASER)
      {
        XZST_03EcritureTrace(XZSTC_FONCTION, "XZECC_PROT_DIASER");
        return (XZEXC_SOCK_ACT_IP_DIASER);
      }
      else if (pl_ListSock->Protocole == XZECC_PROT_LCR)
      {
        XZST_03EcritureTrace(XZSTC_FONCTION, "XZECC_PROT_LCR");
        return (XZEXC_SOCK_ACT_IP_LCR);
      }
      else if (pl_ListSock->Protocole == XZECC_PROT_LCR_E)
      {
        XZST_03EcritureTrace(XZSTC_FONCTION, "XZECC_PROT_LCR_E");
        return (XZEXC_SOCK_ACT_IP_LCR_E);
      }
      else if (pl_ListSock->Protocole == XZECC_PROT_MIGRAZUR)
      {
        XZST_03EcritureTrace(XZSTC_FONCTION, "XZECC_PROT_MIGRAZUR");
        return (XZEXC_SOCK_ACT_DONN); /* TExxx*/
      }
    }
  }
  return (XZEXC_SOCK_ACT_DONN);
}

/*X*/
/*------------------------------------------------------
 * SERVICE RENDU :
 * Traitement evennement deconnexion client TEzzz.
 *
------------------------------------------------------
 * SEQUENCE D'APPEL :
 *
 *
 * void edir_traitement_dcnx( void )
 *
 * ARGUMENTS EN ENTREE :
 *
 * NEANT : utilise la var. globale vg_DesSocks et vg_SockList
 *
 *
 * ARGUMENTS EN SORTIE :
 * NEANT
 *
 * CODE RETOUR :
 *
 * NEANT
 *
 * CONDITION D'UTILISATION
 *
 * Appel de cette fonction si la lecture sur une socket ds. la liste retourne XZEXC_DECONNEC.
 *
 * FONCTION
 *
 * Verifie quelle est la socket deconnectee.
 * Si socket serveur terminaux deconnectee => envoi message a toutes les taches TEzzz.
 * Si toutes les TEzzz se sont deconetcees, appel ecfg_sortir().
 *
------------------------------------------------------*/
void edir_traitement_dcnx(void)
{
  static char *version = "$Id: eimi_dir.c,v 1.8 2021/05/04 13:25:02 pc2dpdy Exp $ : edir_traitement_dcnx";
  XZEXT_MASK *pl_ListSock = NULL;

  XDY_Texte vl_Msg;

  /* ecriture trace : Deconnexion socket adresse <vg_DesSocks.curr_addr>...*/
  XZST_03EcritureTrace(XZSTC_WARNING, " Deconnexion socket adr. <%s>.vg_DesSocks.curr=%d.vg_DesSocks.lser=%d.vg_DesSocks.ect=%d.vg_DesSocks.rtsrv=%d.", vg_DesSocks.curr_addr,
                       vg_DesSocks.curr, vg_DesSocks.lser, vg_DesSocks.ect, vg_DesSocks.rtsrv);

  /* SI deconnexion socket serveur terminaux */
  if (vg_DesSocks.curr == vg_DesSocks.lser)
    vg_EtatST = XDC_NOK;
  /*#ifndef _HPUX_SOURCE*/
  if (vg_DesSocks.curr == vg_DesSocks.ect)
    XZST_03EcritureTrace(XZSTC_FATAL, "Perte client => ARRET !");
  /*#endif*/

  /* SI deconnexion socket RTWorks */
  if (vg_DesSocks.curr == vg_DesSocks.rtsrv)
  {
    /* ALORS ecriture trace "Deconexion socket comms. avec RTServer"*/
    XZST_03EcritureTrace(XZSTC_WARNING,
                         "Deconexion socket comms. avec RTServer !");
  }
  if ((vg_DesSocks.curr != vg_DesSocks.rtsrv) && (vg_DesSocks.curr != vg_DesSocks.lser) && (vg_DesSocks.curr != vg_DesSocks.ect))
  {
    /*XZST_03EcritureTrace( XZSTC_FATAL,"Perte socket ??? => RECONNEXION !");*/

    XZST_03EcritureTrace(XZSTC_WARNING, "Perte socket => RECONNEXION !");

    sleep(60);
    envoyer_trame_dai_ko(0, vg_Config.adresseIP);
    /*		if (ecom_init_IP(&vg_Config, &vg_SockList, &vg_DesSocks)) {
      XZST_03EcritureTrace( XZSTC_WARNING, "pb reconnexion ");
      return;
  }    */
  }

  /* SI deconnexion socket RTWorks */
  if ((vg_DesSocks.curr == vg_DesSocks.rtsrv) && (vg_DesSocks.curr == vg_DesSocks.lser))
    ;
}

/*X*/
/*------------------------------------------------------
 * SERVICE RENDU :
 * Traitement du message receptionne par eimi_dir sur la socket active.
 *
------------------------------------------------------
 * SEQUENCE D'APPEL :
 *
 *
 * void edir_traitement_liste_cmdes_LCR( void );
 *
 * ARGUMENTS EN ENTREE :
 *
 * NEANT : utilise la var. globale vg_DesSocks et vg_SockList
 *
 *
 * ARGUMENTS EN SORTIE :
 * NEANT
 *
 * CODE RETOUR :
 *
 * NEANT
 *
 * CONDITION D'UTILISATION
 *
 * Appel de cette fonction si la lecture sur une socket ds. la liste retourne XZEXC_DECONNEC.
 *
 * FONCTION
 *
 * Verifie quelle est la socket deconnectee.
 * Si socket serveur terminaux deconnectee => envoi message a toutes les taches TEzzz.
 * Si toutes les TEzzz se sont deconetcees, appel ecfg_sortir().
 *
------------------------------------------------------*/

void edir_traitement_liste_cmdes_LCR(void)

{
  static char *version = "$Id: eimi_dir.c,v 1.8 2021/05/04 13:25:02 pc2dpdy Exp $ :  edir_traitement_liste_cmdes_LCR ";

  XZEXT_Cmde_LCR_IP vl_CmdeLCR; /* structure cmde. LCR depilee */
}

void edir_reveil_cb(T_IPC_CONN va_Cnx,
                    T_IPC_CONN_PROCESS_CB_DATA pa_Data,
                    T_CB_ARG pa_Arg)
{

#ifndef _TIBCO_EMS
  T_STR vl_cle;
#else
  XDY_Nom vl_cle;
#endif

  char vl_rgs[5];
  XZEXT_Cmde_LCR_IP vl_CmdeLCR;

  XZST_03EcritureTrace(XZSTC_WARNING, "edir_reveil_cb");
  if (!TipcMsgRead(pa_Data->msg,
                   T_IPC_FT_STR, &vl_cle,
                   NULL))
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "edir_reveil_cb : message non lu");
    return;
  }

  sscanf(vl_cle, "TEMIP_%s", vl_rgs);

  XZST_03EcritureTrace(XZSTC_FONCTION, "edir_reveil_cb : RGS %s", vl_rgs);

  /*recherche de la commande*/
  if (ecom_getfirst_cmd_LCR_IP2(&vl_CmdeLCR, &pg_ListeCmdesLCR, vl_rgs) == XDC_OK)
  {
    epro_envoi_trame_LCR_IP(&vl_CmdeLCR, FALSE);
  }
}
