/*  Fichier : $Id: esur_dir.c,v 1.32 2020/11/03 18:13:16 pc2dpdy Exp $      Release : $Revision: 1.32 $        Date : $Date: 2020/11/03 18:13:16 $
------------------------------------------------------
 * GTIE *  PROJET MIGRAZUR
------------------------------------------------------
 * SOUS-SYSTEME : EQUEXT
------------------------------------------------------
 * MODULE TESUR * FICHIER esur_dir.c
------------------------------------------------------
 * DESCRIPTION DU MODULE :
 *
 *   Module directeur de la tache TESUR
 *
------------------------------------------------------
 * HISTORIQUE :
 *
 * Mismer.D	version 1.1	22 Aug 1994	: Creation
 * Mismer.D	version 1.2	25 Jan 1995	:
 * Mismer.D	version 1.3	25 Jan 1995	:
 * Mismer.D	version 1.4	26 Jan 1995	:
 * Mismer.D	version 1.5	28 Jan 1995	:
 * Mismer.D	version 1.6	31 Jan 1995	:
 * Mismer.D	version 1.7	09 Fev 1995	:
 * Mismer.D	version 1.8	10 Fev 1995	:
 * Mismer.D	version 1.9	15 Fev 1995	:
 * Mismer.D	version 1.10 	15 Fev 1995	:
 * Mismer.D	version 1.11	11 Mai 1995	: Modif condition reprise LT voisin
 * Fontaine.C	version 1.12	22 Mai 1995	: Reprise entete, historique
 * Mismer.D  	version 1.13	02 Jun 1995	: Modif connexion socket RTserver
 * Mismer.D  	version 1.14	04 Jul 1995	: Modif arret tache equipement
 * Mismer.D      version 1.15    07 Sep 1995     : Arret des taches ex-fils au demarrage
 * Mismer.D      version 1.16    08 Sep 1995     : Arret des taches ex-fils au demarrage (suite)
 * Mismer.D      version 1.17    09 Sep 1995     : Modif reprise voisin
 * Mismer.D      version 1.18    10 Oct 1995     : Suppression init alarme openview
 * Mismer.D      version 1.19    30 Oct 1995     : Ajout fonction ed_verif_voisin
 * Mismer.D      version 1.20    08 Nov 1995     : Correction relance tetim
 * Mismer.D  	version 1.21	06 Fev 1995	: Modif pour renvoie etat serveur de terminaux
 * Mismer.D  	version 1.22	08 Fev 1995	: Ajout shunt message recu sur XDG_NSUPETAT
 * Mismer.D 	version 1.23	26 Avr 1996 	: Ajout traitement LT isole (DEM/1052)
 * Mismer.D 	version 1.25	09 Mai 1996 	: Ajout demande config si fichier conf absent (DEM/1111)
 * JMG		version 1.26	21/06/10	: LINUX
 * JPL		24/02/11 : Migration architecture HP ia64 (DEM 975) : XZIA06 : utiliser un champ d'union  1.27
 * JPL		24/02/11 : Migration HP ia64 (DEM 975) : interface SS68 : type socket 'vg_SockRT'  1.28
 * JMG	26/09/16	: ST 40 1.29
 * RGR 17/05/19 : DEM1331 - Ajout Mecanisme KEEPALIVE 1.30
 * GR  10/07/20 : EMS 1.31
------------------------------------------------------*/

/* fichiers inclus */

#include <xzsem.h>
#include "xzia.h"

#include "ex_mrtf.h"

#include "esur_art.h"
#include "esur_dir.h"


/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

XZEXT_MASK *vg_SockMask = NULL;
int vg_SockEct = 0;
int vg_SockTETIM = 0;
T_SOCK vg_SockRT = 0;
int vg_SockLT = 0;
int vg_PresenceVoisin = 0;

/* declaration de fonctions internes */

void ed_recopie_fic_alarme(void);
void ed_lancer_timer(void);
void sp_dgstatus_cb(); /*LINUX*/
void sp_watch_cb(
    T_IPC_CONN va_Cnx,
    T_IPC_CONN_PROCESS_CB_DATA pa_Data,
    T_CB_ARG pa_Arg);

/* definition de fonctions externes */

/*
------------------------------------------------------
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
 *   Connexion au RTServer avec initialisation des variables de
 *   supervision, recuperation du site de la machine
 *   ainsi que le type de celle-ci.
 *   Armements de callbacks.
 *   Abonnement au Datagroup XDG_EFIN_CONFIG_NomMachine.
 *
 *
------------------------------------------------------*/

int argc;
char **argv;
{

  XZSCT_NomTache vl_tache;
  int vl_ValRet = XDC_OK,
      vl_SockRet = 0,
      vl_LgMsgSock = 0,
      vl_Reprise = XDC_NOK,
      vl_IndCnx = 0,
      vl_js = 0;
  double vl_TimeReveil = 0,
         vl_Horodate = 0;
  int vl_Minute = 0;
  XZEXT_MSG_SOCKET pl_MsgSock;
  XDY_NomSite pl_NomSite;
  XDY_Datagroup pl_NomDG;
  XDY_NomMachine pl_NomMachine;
  XDY_Texte vl_MsgOperateur;
  XZEXT_MSG_SOCKET pl_MsgTimer;
  int vl_NbEqt = 0,
      vl_NbConfLigne = 0;
  XZIAT_destinataire vl_Destinataire;
  XZSMT_CleProg vl_cle_timer = "";

  /*A
   *  initialisation semaphore
   */
  sem_init(&vg_semaphore, 0, 1);

  /*A
   * Connexion au RTserver et initialisation des variables
   */

  if (XZSC_01InitCnxSup(NULL,
                        argv[0],
                        XZSCC_INTERDIRE,
                        XZSCC_AUTORISER,
                        XZSCC_INTERDIRE,
                        ed_arreter,
                        "/dev/null") != XDC_OK)
  {
    /*B Affichage Trace */
    XZST_03EcritureTrace(XZSTC_FATAL, "Ne peut ouvrir la connexion avec le RTserver.");
  }
  else
  {
    /*B Affichage Trace */
    XZST_03EcritureTrace(XZSTC_DEBUG1, "Ouverture de la connexion avec le RTserver.");
  }

  XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL | XZSTC_WARNING | XZSTC_INFO | XZSTC_FONCTION | XZSTC_INTERFACE | XZSTC_MESSAGE, XZSTC_NECRAN);

  vg_PresenceVoisin = 0;
  /*A Recuperation du nom de la machine */
  while (1)
  {
    if ((lire_machine_sup() == XDC_OK) && (strlen(vg_NomMachine) > 0))
    {
      XZST_03EcritureTrace(XZSTC_WARNING, "nom machine <%s> .", vg_NomMachine);
      break;
    }
    else
    {
      XZST_03EcritureTrace(XZSTC_WARNING, "Impossible de trouver le nom de la machine surveillee .");
      sleep(60);
    }
  }

  /*A Recuperation du site de la machine */
  if ((vl_ValRet = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "Recuperation du Nom du site impossible .");
  }

  strcpy(vl_Destinataire.site, pl_NomSite);

  /*A Recuperation du nom de la machine */
  if (XZSC_07NomMachine(pl_NomMachine) != XDC_OK)
  {
    /* Ecriture Trace */
    XZST_03EcritureTrace(XZSTC_WARNING, "Recuperation du Nom de la machine impossible . ");
  }

  /*A Lancement de la tache TETIM */
  ed_lancer_timer();

  chdir(XDC_PATHEXE);

  /*A Creation du repertoire pour les sockets */
  ex_rep_socket(vg_NomMachine);
  /*A Lecture de la config de la machine surveillee */
  while ((ex_lire_config_lignes(vg_NomMachine, &vl_NbConfLigne) != XDC_OK) &&
         (ex_lire_config_eqt(vg_NomMachine, &vl_NbEqt) != XDC_OK))
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "Impossible de lire la config de la machine <%s> .", vg_NomMachine);
    /*A Emission du message XDM_EINIT sur le DatGroup XDG_EINIT_NomSite */
    XDG_EncodeDG2(pl_NomDG, XDG_EINIT, pl_NomSite);
    if (!TipcSrvMsgWrite(pl_NomDG, 
                        XDM_IdentMsg(XDM_EINIT), 
                        XDM_FLG_SRVMSGWRITE, 
                        T_IPC_FT_STR, vg_NomMachine, 
                        T_IPC_FT_INT4, XZECC_TYPE_EQT_ALL, 
                        T_IPC_FT_INT4, XZECC_MACHINE_SUPPL, 
                        NULL))
    {
      XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_EINIT non effectue.");
    }
    sleep(60);
  }
  /*A Arret des taches EQUEXT si elle sont presente mais orpheline */
  ec_verif_arret_tache();
  el_arret_tache(0, 0, XZECC_TACHE_TIM);

  /*A Connexion socket en mode serveur pour communiquer avec les taches traitements */
  if (ex_cnx_serveur(XZEXC_ADR_SOCK_TESRV, &vg_SockMask, &vg_SockEct) != XDC_OK)
  {
    /*B Ecriture Trace */
    XZST_03EcritureTrace(XZSTC_WARNING, "Impossible de creer la connexion serveur .");
  }

  /*A  Armement Callback sur Reception XDM_EFIN_CONFIG_SUR */
  if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_EFIN_CONFIG_SUR),
                             ec_init_cb, (T_CB_ARG)NULL) == NULL)
  {
    /*B Ecriture Trace */
    XZST_03EcritureTrace(XZSTC_WARNING, "Callback sur XDM_EFIN_CONFIG_SUR non declenchee.");
  }

  /*A  Armement Callback sur Reception XDM_ECMD_EQPT */
  if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_ECMD_EQPT),
                             ec_cmd_cb, (T_CB_ARG)NULL) == NULL)
  {
    /* Ecriture Trace */
    XZST_03EcritureTrace(XZSTC_WARNING, "Callback sur XDM_ECMD_EQPT non declenchee.");
  }

  /*A  Armement Callback sur Reception XDM_TSE_DIFE */
  if ((TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_TSE_DIFE), ec_dde_etat_ST_cb, (T_CB_ARG)NULL) == NULL) ||
      (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_TSE_TACH), ec_dde_etat_ST_cb, (T_CB_ARG)NULL) == NULL) ||
      (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_TST_TRAC), ec_dde_etat_ST_cb, (T_CB_ARG)NULL) == NULL) ||
      (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_TSE_MODI), ec_dde_etat_ST_cb, (T_CB_ARG)NULL) == NULL) ||
      (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_TSE_SSYS), ec_dde_etat_ST_cb, (T_CB_ARG)NULL) == NULL) ||
      (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_TSE_MACH), ec_dde_etat_ST_cb, (T_CB_ARG)NULL) == NULL) ||
      (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_TSE_APPLI), ec_dde_etat_ST_cb, (T_CB_ARG)NULL) == NULL) ||
      (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_TSE_ISAG), ec_dde_etat_ST_cb, (T_CB_ARG)NULL) == NULL) ||
      (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_TSE_ISSS), ec_dde_etat_ST_cb, (T_CB_ARG)NULL) == NULL) ||
      (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_TSE_ISTC), ec_dde_etat_ST_cb, (T_CB_ARG)NULL) == NULL) ||
      (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_TSE_STARTTMP), ec_dde_etat_ST_cb, (T_CB_ARG)NULL) == NULL))
  {
    /* Ecriture Trace */
    XZST_03EcritureTrace(XZSTC_WARNING, "Callback sur XDM_TSE_DIFE ou TSE_TACH  ou TST_TRAC ou TSE_STARTTMP non declenchee.");
  }

  /*A Abonnement au Datagroup XDG_EFIN_CONFIG_SUR_NomMachine */
  XDG_EncodeDG2(pl_NomDG, XDG_EFIN_CONFIG_SUR, vg_NomMachine);
  /*A joint ce datagroup */
  if (!XDG_JoindreDG(pl_NomDG))
  {
    /* Affichage Trace */
    XZST_03EcritureTrace(XZSTC_WARNING, "Abonnement au Datagroup %s non effectue.", pl_NomDG);
  }

  /*LINUX*/
#ifndef _TIBCO_EMS
  if (TipcSrvProcessCbCreate(XDM_IdentMsg(T_MT_MON_SUBJECT_SUBSCRIBE_STATUS), sp_dgstatus_cb, (T_CB_ARG)NULL) == NULL)
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "Callback sur T_MT_MON_SUBJECT_SUBSCRIBE_STATUS non declenchee.");
  }
#else
  if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_TSE_WATCH), sp_watch_cb, (T_CB_ARG)NULL) == NULL)
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "Callback sur XDM_TSE_WATCH non declenchee.");
  }

#endif
  XDG_EncodeDG2(pl_NomDG, vg_NomMachine, "tesrv.x");
  XDG_SurveillerDG(pl_NomDG);

#ifdef _TIBCO_EMS

  XDG_EncodeDG2(pl_NomDG, XDG_WATCH, vg_NomMachine);
  if (!XDG_JoindreDG(pl_NomDG))
  {
    /* Affichage Trace */
    XZST_03EcritureTrace(XZSTC_WARNING, "Abonnement au Datagroup %s non effectue.", pl_NomDG);
  }

  /* timer de surveillance des abonnements*/
  XDG_EncodeDG2(pl_NomDG, XDG_WATCH, vg_NomMachine);
  XDG_EncodeDG2(vl_cle_timer, pl_NomDG, "timer");
  if (XZSM_01DemanderProgrammation(0, 60, pl_NomDG, XDM_TSE_WATCH, vl_cle_timer, 10) != XDC_OK)
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "Erreur demande programmation WATCH sur Dg [%s]", pl_NomDG);
  }
  else
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "Programmation WATCH sur Dg [%s] effectuee", pl_NomDG);
  }

#endif

  /*A Abonnement au Datagroup XDG_NSUPETAT_NomMachine */
  XDG_EncodeDG2(pl_NomDG, XDG_NSUPETAT, vg_NomMachine);
  /*A joint ce datagroup */
  if (!XDG_JoindreDG(pl_NomDG))
  {
    /* Affichage Trace */
    XZST_03EcritureTrace(XZSTC_WARNING,
                         "Abonnement au Datagroup %s non effectue.", pl_NomDG);
  }

  /* Memorisation lancement des taches non effectuee  */
  vg_ConfigTerm = XDC_NOK;

  /* Abonnement au DataGroup KEEPALIVE */
  XZSC_31AbtKeepAlive(pl_NomSite);

  /*A Attente des messages sur les sockets (RTserver, d'ecoute et de donnees */
  while (1)
  {
    if (ex_attendre_msg(vg_SockEct, &vg_SockMask, &vl_SockRet) != XDC_OK)
    {
      XZST_03EcritureTrace(XZSTC_WARNING, "Probleme retour fonction select.");
      ex_ConnexionSocketRTserver(&vg_SockRT, vl_SockRet, &vg_SockMask);
      if (vl_SockRet == vg_SockLT)
        vg_SockLT = 0;
      if (vl_SockRet == vg_SockTETIM)
        vg_SockTETIM = 0;
    }
    else
    {
      ex_ConnexionSocketRTserver(&vg_SockRT, 0, &vg_SockMask);
      if (vg_SockTETIM == 0)
      {
        ed_lancer_timer();
        ec_verif_arret_tache();
      }

      /*A Si une socket a ete activee */
      if (vl_SockRet != XZEXC_NO_EVT)
      {
        /*A S'il s'agit d'une socket de donnee : lire le message */
        if ((vl_SockRet != vg_SockRT) && (vl_SockRet != vg_SockEct))
        {
          /*A Lire le message de la socket de donnees */
          if (ex_lire_msg(vl_SockRet, pl_MsgSock, &vl_LgMsgSock, &vg_SockMask, &vl_IndCnx) != XDC_OK)
          {
            /*A Deconnexion d'une socket */
            if (vl_SockRet == vg_SockLT)
            {
              ex_fin_cnx(vl_SockRet, &vg_SockMask);
              /*vg_PresenceVoisin = 1;*/
              vg_SockLT = 0;
            }
          }
          else
          {
            /*A S'il ne s'agit pas d'un message de deconnexion */
            if ((vl_IndCnx != XZEXC_DECONNEC) && (vl_LgMsgSock != 0))
            {
              if (strstr(pl_MsgSock, XZEXC_MSG_REVEIL) != NULL)
              {
                sscanf(&pl_MsgSock[strlen(XZEXC_MSG_REVEIL)], " %lf ", &vl_TimeReveil);
                XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME, &vl_js, &vl_Horodate);
                XZST_03EcritureTrace(XZSTC_DEBUG1, "Horodate:%lf  Reveil:%lf Voisin:%d  ConfigTerm:%d",
                                     vl_Horodate, vl_TimeReveil, vg_PresenceVoisin, vg_ConfigTerm);
                if (vl_Horodate - vl_TimeReveil < 10)
                {
                  /*LINUX*/
                  /*
                                if ( vg_SockLT == 0 )
                                   vg_PresenceVoisin = ( ex_cnx_client_ST( vg_NomMachine,
                                             XZEXC_PORT_SERVEUR_LT,
                                             &vg_SockMask,
                                             &vg_SockLT ) == XDC_OK ) ? 0 : 200;*/

                  /*if ( (vg_ConfigTerm == XDC_NOK) && (vg_PresenceVoisin++ > 2) )*/
                  if ((vg_ConfigTerm == XDC_NOK) && (vg_PresenceVoisin == 0)) /*LINUX*/
                  {
                    XZST_03EcritureTrace(XZSTC_WARNING, "1");
                    if ((ex_presence_machine(ec_Nom_ST()) == XDC_OK) || (ex_st40(vg_NomMachine) == XDC_OK))
                    {
                      XZST_03EcritureTrace(XZSTC_WARNING, "2");
                      if (vg_RepriseEnCours == XDC_NOK)
                      {
                        XZST_03EcritureTrace(XZSTC_WARNING, "3");
                        /*A Demande d'initialisation de tous les equipement pour la machine locale a TEINI */
                        /*A Emission du message XDM_EINIT sur le DatGroup XDG_EINIT_NomSite */
                        XDG_EncodeDG2(pl_NomDG, XDG_EINIT, pl_NomSite);
                        if (!TipcSrvMsgWrite(pl_NomDG, 
                                            XDM_IdentMsg(XDM_EINIT), 
                                            XDM_FLG_SRVMSGWRITE, 
                                            T_IPC_FT_STR, vg_NomMachine, 
                                            T_IPC_FT_INT4, XZECC_TYPE_EQT_ALL, 
                                            T_IPC_FT_INT4, XZECC_MACHINE_SUPPL, 
                                            NULL))
                        {
                          XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_EINIT non effectue.");
                        }
                      }
                      else
                      {
                        XZST_03EcritureTrace(XZSTC_WARNING, "Debut lancement de l'application. ");
                        if (ec_config(XZECC_TYPE_EQT_ALL) != XDC_OK)
                        {
                          XZST_03EcritureTrace(XZSTC_WARNING, "Configuration de l'application non effectuee. ");
                          vg_ConfigTerm = XDC_NOK;
                        }
                        else
                        {
                          vg_ConfigTerm = XDC_OK;
                          sprintf(vl_MsgOperateur, "Toutes les fonctions de %s sont operationnelles sur  %s .\0", vg_NomMachine, pl_NomMachine);
                          XZIA_06AffMsgSyst(vl_Destinataire, vl_MsgOperateur);
                          XZST_03EcritureTrace(XZSTC_WARNING, "Application lancee avec succes ");
                        }
                      }

                      if (vl_Reprise == XDC_NOK)
                      {
                        XZST_03EcritureTrace(XZSTC_WARNING, "Reprise de %s en cours .", vg_NomMachine);
                        /*A Envoie message reprise par LT voisin a l'operateur (XZIA06) */
                        sprintf(vl_MsgOperateur, "Reprise de %s par %s en cours.\0", vg_NomMachine, pl_NomMachine);
                        XZIA_06AffMsgSyst(vl_Destinataire, vl_MsgOperateur);
                        vl_Reprise = XDC_OK;
                      }
                    }
                    else
                    {
                      /*A Envoi passage en mode isole pour  TEINI */
                      /*A Emission du message XDM_EINIT sur le DatGroup XDG_EINIT_NomSite */
                      if (vl_Minute = !vl_Minute)
                      {
                        XDG_EncodeDG2(pl_NomDG, XDG_EINIT, pl_NomSite);
                        if (!TipcSrvMsgWrite(pl_NomDG, 
                                              XDM_IdentMsg(XDM_EINIT), 
                                              XDM_FLG_SRVMSGWRITE, 
                                              T_IPC_FT_STR, vg_NomMachine, 
                                              T_IPC_FT_INT4, XZECC_TYPE_EQT_ALL, 
                                              T_IPC_FT_INT4, XZEXC_MODE_ISOLE, 
                                              NULL))
                        {
                          XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_EINIT non effectue.");
                        }
                        else
                        {
                          XZST_03EcritureTrace(XZSTC_WARNING, "Envoi passage en mode isole a TEINI.");
                        }
                      }
                    }
                    if (vg_SockLT != 0)
                      ex_fin_cnx(vg_SockLT, &vg_SockMask);
                    vg_SockLT = 0;
                    /*vg_PresenceVoisin = 1; */
                  }
                  if (vg_ConfigTerm == XDC_OK)
                  {
                    /*A Appel de ec_surveillance_tache */
                    ec_surveillance_tache();
                    vl_Reprise = XDC_NOK;
                    vg_RepriseEnCours = XDC_NOK;
                  }
                }
              }
              else
              {
                XZST_03EcritureTrace(XZSTC_INFO, "RECEP <%s> .", pl_MsgSock);
                vg_RepriseEnCours = XDC_NOK;
                /*vg_PresenceVoisin = 0;*/
                ed_arreter_tache_eqpt();
              }
            }
            else
            {
              if (vl_SockRet == vg_SockLT)
              {
                /*vg_PresenceVoisin = 1;*/
                vg_SockLT = 0;
              }
              if (vl_SockRet == vg_SockTETIM)
                vg_SockTETIM = 0;
              if ((vl_SockRet != vg_SockLT) && (vl_SockRet != vg_SockTETIM) && (vg_ConfigTerm == XDC_OK))
                ec_surveillance_tache();
            }
          }
        } /* fin si socket de donnees */
      }   /* fin si socket active */
    }
  } /* fin while */

} /* Fin main */

/*X*/
/*------------------------------------------------------
 * SERVICE RENDU :
 *  Fonction effectuant les operations de terminaison de la tache.
 *
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

void ed_arreter(void)

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
 *  Fonction declenchee par SUPERV lors de l'arret de la tache
 *  TESRV
 *
 * FONCTION
 *  Envoi d'une demande d'arret a la tache timer TETIM et
 *  aux taches equipements
 *  Attente infinie sur tous les PID des taches lancees
 *  Fermeture de la socket d'ecoute
 *
------------------------------------------------------*/
{

  register int vl_IndexTache = 0;
  XZSCT_NomTache pl_NomTache;
  XZEXT_MASK *pl_masque;

  /*A
   * Arret des taches equipements et la tache TETIM (appel de ed_arreter_tache_eqpt)
   */
  /*A Pour toutes les taches memorisees dans le masque */
  for (pl_masque = vg_SockMask; pl_masque != NULL; pl_masque = pl_masque->Suivant)
  {
    /*A Si differente de la socket RTserver et differente de la socket d'ecoute */
    if ((pl_masque->Type == AF_UNIX) &&
        (pl_masque->desc_sock != vg_SockEct))
    {
      /*A Envoie de la demande d'arret ( appel de ee_config ) */
      ee_config(XZEXC_DEMANDE_ARRET, pl_masque->desc_sock, pl_masque->adresse);
    }
  } /* fin for */

  /*
   *A Verification arret des taches
   */
  ec_verif_arret_tache();
  ed_recopie_fic_alarme();

  /*A
   * Attente infinie sur tout les Pid des taches lancees
   */

  /*A Pour toutes les taches memorisees dans le masque */
  for (pl_masque = vg_SockMask; pl_masque != NULL; pl_masque = pl_masque->Suivant)
  {
    /*A Si differente de la socket RTserver et differente de la socket d'ecoute */
    if ((pl_masque->Type == AF_UNIX) &&
        (pl_masque->desc_sock != vg_SockEct))
    {
      /*A Recherche du nom de la tache */
      sprintf(pl_NomTache, "%s.xs", pl_masque->adresse);
      /*A Attente tache terminee */
      while (XZSS_20PidTache(pl_NomTache) != 0)
      {
        sleep(XZEXC_ATTENTE_ARRET_TACHE);
      }
    }
  }

  /*A
   * Fermeture de toutes les sockets
   */

  /*A Pour toute les sockets du masque */
  for (pl_masque = vg_SockMask; pl_masque != NULL; pl_masque = pl_masque->Suivant)
  {
    /*A Appel de la fonction ex_fin_cnx */
    ex_fin_cnx(pl_masque->desc_sock, &vg_SockMask);
  }

  XZST_03EcritureTrace(XZSTC_WARNING, "Sortie de tache TESRV .");
  /*A Arret de la tache */
  exit(0);
}

/*X*/
/*------------------------------------------------------
 * SERVICE RENDU :
 *  Fonction effectuant les echanges avec TESRV pendant le lancement de l'appli.
 *
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

int ed_verif_voisin(void)

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
 *  TESRV
 *
 * FONCTION
 *
 *
------------------------------------------------------*/
{
  static char *version = "$Id: esur_dir.c,v 1.32 2020/11/03 18:13:16 pc2dpdy Exp $ : ed_verif_voisin  ";
  XZEXT_MASK *vl_SockMask = NULL;
  XZEXT_MASK *pl_masque = NULL;
  XZEXT_MASK vl_masque;
  XZEXT_MSG_SOCKET pl_MsgSock;
  /*XDY_NomMachine   pl_NomMachine;*/
  int vl_SockRet = 0,
      vl_LgMsgSock = 0,
      vl_IndCnx = 0;

  /*A Insertion du descripteur de la socket dans le masque de lecture */
  for (pl_masque = vg_SockMask; pl_masque != NULL; pl_masque = pl_masque->Suivant)
  {
    /*A Si differente de la socket RTserver et differente de la socket d'ecoute */
    if ((pl_masque->desc_sock == vg_SockLT) || (pl_masque->desc_sock == vg_SockTETIM))
    {
      vl_masque.desc_sock = pl_masque->desc_sock;
      vl_masque.Type = pl_masque->Type;
      strcpy(vl_masque.adresse, pl_masque->adresse);
      ex_ins_masque(&vl_masque, &vl_SockMask);
    }
  }

  if (ex_attendre_msg(XZEXC_SOCKET_NULL, &vl_SockMask, &vl_SockRet) != XDC_OK)
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "Probleme retour fonction select.");
    if (vl_SockRet == vg_SockLT)
      vg_SockLT = 0;
    if (vl_SockRet == vg_SockTETIM)
      vg_SockTETIM = 0;
    /*vg_PresenceVoisin = 0;*/ /*LINUX*/
  }
  else
  {
    /*A Si une socket a ete activee */
    if (vl_SockRet != XZEXC_NO_EVT)
    {
      /*A Lire le message de la socket de donnees */
      if (ex_lire_msg(vl_SockRet, pl_MsgSock, &vl_LgMsgSock, &vg_SockMask, &vl_IndCnx) != XDC_OK)
      {
        /*A Ecriture trace erreur lecture message socket sur esrv_dir */
        XZST_03EcritureTrace(XZSTC_WARNING, "erreur lecture message socket sur esrv_dir.");
      }
      else
      {
        XZST_03EcritureTrace(XZSTC_DEBUG1, "Reception de <%s>.", pl_MsgSock);
        /*A S'il ne s'agit pas d'un message de deconnexion */
        if ((vl_IndCnx != XZEXC_DECONNEC) && (vl_LgMsgSock != 0))
        {
          /*A Si il s'agit d'un reveil de TETIM */
          if (strstr(pl_MsgSock, XZEXC_MSG_REVEIL) != NULL)
          {
            if (vg_SockLT == 0)
            {
              /* vg_PresenceVoisin = ( ex_cnx_client_ST( vg_NomMachine,
                                         XZEXC_PORT_SERVEUR_LT,
                                         &vg_SockMask,
                                         &vg_SockLT ) == XDC_OK ) ? 0 : 200;*/
            }
          }
          else
          {
            /*vg_PresenceVoisin = 0;*/ /*LINUX*/
          }
        }
        else
        {
          if (vl_SockRet == vg_SockLT)
            vg_SockLT = 0;
          if (vl_SockRet == vg_SockTETIM)
            vg_SockTETIM = 0;
          /*vg_PresenceVoisin = 0;*/
        }
      } /* fin si socket de donnees */
    }   /* fin si socket active */
  }

  for (pl_masque = vl_SockMask; pl_masque != NULL; pl_masque = pl_masque->Suivant)
    ex_sup_masque(pl_masque->desc_sock, &vl_SockMask);

  return (vg_PresenceVoisin);
}

/*X*/
/*------------------------------------------------------
 * SERVICE RENDU :
 *  Fonction effectuant le lancement de la tache tetim.
 *
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

void ed_lancer_timer(void)

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
  static char *version = "$Id: esur_dir.c,v 1.32 2020/11/03 18:13:16 pc2dpdy Exp $ : ed_lancer_timer  ";
  XZEXT_MSG_SOCKET pl_MsgTimer;

  /*A Lancement de la tache TETIM */
  el_arret_tache(0, 0, XZECC_TACHE_TIM);
  if (el_lancer_tache_eqt(XZECC_TACHE_TIM) != XDC_OK)
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "Impossible lancer tache tache TETIM .");
  }
  sleep(10);
  /*A
   * Connexion socket en mode client pour communiquer avec la tache TETIM
   */
  if (ex_cnx_client(XZEXC_ADR_SOCK_SRVTIM,
                    XZEXC_ADR_SOCK_TETIM_SRV,
                    &vg_SockMask,
                    &vg_SockTETIM) != XDC_OK)
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "Impossible de creer la connexion client TETIM .");
  }
  else
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "Connexion etablie avec TETIM .");
  }

  if (vg_SockTETIM)
  {
    /*A Demande de reveil pour time_out surveillance */
    XZST_03EcritureTrace(XZSTC_WARNING, "avant appel ETIM_REVEIL_CYCLIQUE");
    ETIM_REVEIL_CYCLIQUE(30, "TESUR_Timer_30s", vg_SockTETIM, pl_MsgTimer);
  }
  else
  {
    XZST_03EcritureTrace(XZSTC_WARNING, " PAS DE vg_SockTETIM");
  }
}

/*X*/
/*------------------------------------------------------
 * SERVICE RENDU :
 *  Fonction permettant de transmettre la demande d'arret aux taches traitement.
 *
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

void ed_arreter_tache_eqpt(void)

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
 *
 * FONCTION
 *  Pour chaque equipement transmettre la demmande d'arret a la tache traitement.
 *
------------------------------------------------------*/
{
  static char *version = "$Id: esur_dir.c,v 1.32 2020/11/03 18:13:16 pc2dpdy Exp $ : ed_arreter_tache_eqpt ";
  XZEXT_MASK *pl_masque;
  int vl_IndexEqt = 0;
  int vl_IndexLig = 0;
  int vl_NbTache = 0;
  int vl_DemArret = XDC_FAUX;
  XDY_NomSite pl_NomSite = "";
  XDY_Texte vl_MsgOperateur = "";
  FILE *vl_Cmd = NULL;
  char vl_Commande[XZSSC_LINE_CMD_SIZE] = "";
  char line[XZSSC_LINE_CMD_SIZE] = "";
  XZIAT_destinataire vl_Destinataire;

  /*A
   * Arret des taches equipements  )
   */
  /*A Pour toutes les taches memorisees dans le masque */
  for (pl_masque = vg_SockMask; pl_masque != NULL; pl_masque = pl_masque->Suivant)
  {
    /*A Si differente de la socket RTserver et differente de la socket d'ecoute */
    if ((pl_masque->Type == AF_UNIX) &&
        (pl_masque->desc_sock != vg_SockEct) &&
        (pl_masque->desc_sock != vg_SockTETIM) &&
        (strcmp(pl_masque->adresse, XZEXC_ADR_SOCK_TETIM) != 0))
    {
      /*A Envoie de la demande d'arret ( appel de ee_config ) */
      ee_config(XZEXC_DEMANDE_ARRET, pl_masque->desc_sock, pl_masque->adresse);
      vl_DemArret = XDC_VRAI;
    }
  } /* fin for */

  if (vl_DemArret == XDC_VRAI)
    sleep(XZEXC_ATTENTE_ARRET_TACHE);

  /*
   *A Verification arret des taches
   */

  strcpy(vl_Commande, "ps -ef|grep te|grep .xs|wc -l");
  if ((vl_Cmd = popen(vl_Commande, "r")) != NULL)
  {
    errno = 0;
    fgets(line, XDC_LINE_CMD_SIZE, vl_Cmd);
    if (errno)
      XZST_03EcritureTrace(XZSTC_WARNING, "ed_arreter_tache_eqpt: Erreur ps : %d ", errno);
    else
      vl_NbTache = atoi(line);
    pclose(vl_Cmd);
  }

  XZST_03EcritureTrace(XZSTC_INFO, "ed_arreter_tache_eqpt: vl_NbTache =  %d ", vl_NbTache);

  if (vl_NbTache != 2)
    ec_verif_arret_tache();

  if (vg_ConfigTerm == XDC_OK)
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "Arret des taches de %s.", vg_NomMachine);
    ed_recopie_fic_alarme();
    /*A Envoie message retour a la normale a l'operateur (XZIA06) */
    if (XZSC_04NomSite(pl_NomSite) != XDC_OK)
    {
      XZST_03EcritureTrace(XZSTC_WARNING, "Recuperation du Nom du site impossible .");
    }
    sprintf(vl_MsgOperateur, "Retour a la normale sur %s.\0", vg_NomMachine);
    strcpy(vl_Destinataire.site, pl_NomSite);
    XZIA_06AffMsgSyst(vl_Destinataire, vl_MsgOperateur);
  }
  vg_ConfigTerm = XDC_NOK;
}

/*X*/
/*------------------------------------------------------
 * SERVICE RENDU :
 *  Fonction permettant de les fichiers d'alarme sur la machine surveillee.
 *
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

void ed_recopie_fic_alarme(void)

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
 *
 * FONCTION
 *  Pour chaque equipement transmettre la demmande d'arret a la tache traitement.
 *
------------------------------------------------------*/
{
  static char *version = "$Id: esur_dir.c,v 1.32 2020/11/03 18:13:16 pc2dpdy Exp $ : ed_recopie_fic_alarme ";
  XDY_NomMachine pl_NomMachine = "";
  FILE *pl_Cmd = NULL;
  char vl_CmdShell[XZSSC_LINE_CMD_SIZE] = "";

  if (ex_presence_machine(vg_NomMachine) == XDC_OK)
  {
    /*A Recuperation du nom de la machine */
    if (XZSC_07NomMachine(pl_NomMachine) != XDC_OK)
    {
      /* Ecriture Trace */
      XZST_03EcritureTrace(XZSTC_WARNING, "Recuperation du Nom de la machine impossible . ");
    }
    sprintf(vl_CmdShell, "rcp %s/Ala_%s* %s:%s/fichiers/ala",
            XZEXC_PATH_FIC_ALARME,
            vg_NomMachine,
            vg_NomMachine,
            XDC_PATH_MIGRAZURHOME);
    if ((pl_Cmd = popen(vl_CmdShell, "r")) != NULL)
      pclose(pl_Cmd);
    sprintf(vl_CmdShell, "chmod 0644 %s/Ala_%s* ; rm %s/Ala_%s*",
            XZEXC_PATH_FIC_ALARME,
            vg_NomMachine,
            XZEXC_PATH_FIC_ALARME,
            vg_NomMachine);
    if ((pl_Cmd = popen(vl_CmdShell, "r")) != NULL)
      pclose(pl_Cmd);
  }
}

/*LINUX*/
#ifndef _TIBCO_EMS
void sp_dgstatus_cb(
    T_IPC_CONN va_Cnx,
    T_IPC_CONN_PROCESS_CB_DATA pa_Data,
    T_CB_ARG pa_Arg)

{

  T_STR pl_NomDatagroup = "";
  T_INT4 vl_NbAbonnes = 0;
  T_STR subject_name;
  T_STR *subscribe_client_names;
  T_STR start_client;
  T_STR stop_client;

  T_INT4 num_subscribe_client_names;

  XZST_03EcritureTrace(XZSTC_WARNING, "IN : sp_dgstatus_cb");
  TipcMsgSetCurrent(pa_Data->msg, 0);
  if (!TipcMsgNextStr(pa_Data->msg, &subject_name) || !TipcMsgNextStrArray(pa_Data->msg, &subscribe_client_names, &num_subscribe_client_names) || !TipcMsgNextStr(pa_Data->msg, &start_client) || !TipcMsgNextStr(pa_Data->msg, &stop_client))
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "Erreur reception DG_STATUS \n");
    XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : sp_dgstatus_cb");
    return;
  }

  XZST_03EcritureTrace(XZSTC_WARNING,
                       "%d abonnes sur '%s'",
                       num_subscribe_client_names, subject_name);
  vg_PresenceVoisin = num_subscribe_client_names;

  if (vg_PresenceVoisin > 0)
    ed_arreter_tache_eqpt();
}
/*LINUX*/
#endif

#ifdef _TIBCO_EMS
void sp_watch_cb(
    T_IPC_CONN va_Cnx,
    T_IPC_CONN_PROCESS_CB_DATA pa_Data,
    T_CB_ARG pa_Arg)
{
  tibemsErrorContext errorContext;
  #ifdef _RABBITMQ
  int consumerInfoCollection = NULL;
  #else
  tibemsCollection consumerInfoCollection = NULL;
  #endif
  char vl_NomMachine[100];
  char vl_str[100];
  XDY_Datagroup pl_NomDG;
  tibems_status status;
  tibemsCollection topicInfoCollection = NULL;
  tibems_int count;
  tibems_int countDurable;

  XZSC_07NomMachine(vl_NomMachine);
  XDG_EncodeDG2(pl_NomDG, vg_NomMachine, "tesrv.x");
  consumerInfoCollection = XZEMS_03IsTopic(pl_NomDG);
  vg_PresenceVoisin = 0;
  if (consumerInfoCollection == NULL)
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "sp_watch_cb : topic inexistant");
    XDG_tibemsCollection_Destroy(consumerInfoCollection);
    return;
  }


XZST_03EcritureTrace(XZSTC_WARNING, "sp_watch_cb A REECRIRE *:*:*:*:*:*:*:*:*:*:*:*:*:*:*:*:*:*:*:*:*:*:*:*:*:*:*:*:*:*:*:*:*:*:*");


  /*parcours de la collection*/
  status = tibemsCollection_GetFirst(consumerInfoCollection, &topicInfoCollection);
  if (status != TIBEMS_OK)
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "sp_watch_cb : erreur tibemsCollection_GetFirst  %s",
                         tibems_errorString(errorContext));
    XDG_tibemsCollection_Destroy(consumerInfoCollection);
    return;
  }
  status = tibemsTopicInfo_GetSubscriptionCount(topicInfoCollection, &count);
  if (status != TIBEMS_OK)
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "sp_watch_cb : erreur tibemsTopicInfo_GetSubscriptionCount %d %s",
                         status,
                         tibems_errorString(errorContext));
    XDG_tibemsCollection_Destroy(consumerInfoCollection);
    return;
  }

  status = tibemsTopicInfo_GetDurableSubscriptionCount(topicInfoCollection, &countDurable);
  if (status != TIBEMS_OK)
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "sp_watch_cb : erreur tibemsTopicInfo_GetDurableSubscriptionCount %d %s",
                         status,
                         tibems_errorString(errorContext));
    XDG_tibemsCollection_Destroy(consumerInfoCollection);
    return;
  }

  if (count - countDurable > 0)
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "sp_watch_cb : retourne %d abonne(s)", count - countDurable);
  }
  else
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "sp_watch_cb : retourne %d abonne donc pas d'abonne", count - countDurable);
     XDG_tibemsCollection_Destroy(consumerInfoCollection);
    return;
  }
  vg_PresenceVoisin = 1;
  XDG_tibemsCollection_Destroy(consumerInfoCollection);

  if (vg_PresenceVoisin > 0)
    ed_arreter_tache_eqpt();
}
#endif
