/*  Fichier : $Id: esrv_dir.c,v 1.25 2020/11/03 18:12:32 pc2dpdy Exp $      Release : $Revision: 1.25 $        Date : $Date: 2020/11/03 18:12:32 $
------------------------------------------------------
 * GTIE *  PROJET MIGRAZUR
------------------------------------------------------
 * SOUS-SYSTEME : EQUEXT
------------------------------------------------------
 * MODULE TESRV * FICHIER esrv_dir.c
------------------------------------------------------
 * DESCRIPTION DU MODULE :
 *
 * < Module directeur de la tache TESRV >
 *
------------------------------------------------------
 * HISTORIQUE :
 *
 * Mismer.D	version 1.1	22 Aug 1994	: Creation
 * Mismer.D	version 1.2	28 Oct 1994	:
 * Mismer.D 	version 1.3	29 Nov 1994	:
 * Mismer.D  	version 1.4	09 Dec 1994	:
 * Mismer.D  	version 1.5	25 Jan 1995	:
 * Mismer.D  	version 1.6	25 Jan 1995	:
 * Mismer.D  	version 1.7	25 Jan 1995	:
 * Mismer.D  	version 1.8	25 Jan 1995	:
 * Mismer.D  	version 1.9	26 Jan 1995	:
 * Mismer.D  	version 1.10	31 Jan 1995	:
 * Mismer.D  	version 1.11	09 Fev 1995	:
 * Fontaine.C	version 1.12	22 Mai 1995	: Reprise entete, historique
 * Mismer.D  	version 1.13	02 Jun 1995	: Modif connexion socket RTserver
 * Mismer.D  	version 1.14	26 Jun 1995	: Modif connexion avec LT suppleant
 * Mismer.D  	version 1.15	29 Jun 1995	: Modif changement de mode ( isole ou normal )
 * Mismer.D  	version 1.16	31 Aou 1995	: Modif socket RTServer
 * Mismer.D  	version 1.17	07 Sep 1995	: Arret des taches ex-fils au demarrage
 * Mismer.D  	version 1.18	11 Oct 1995	: Suppression appel init alame pour openview
 * Mismer.D  	version 1.19	19 Oct 1995	: Modif dialogue avec tesur pendant le lancement des taches
 * Mismer.D  	version 1.20	08 Nov 1995	: Modif relance tetim.x
 * Mismer.D  	version 1.21	06 Fev 1995	: Modif pour renvoie etat serveur de terminaux
 * Mismer.D  	version 1.22	08 Fev 1995	: Ajout shunt message recu sur XDG_NSUPETAT
 * JPL		24/02/11 : Migration HP ia64 (DEM 975) : interface SS68 : type socket 'vg_SockRT'  1.23
 * RGR 17/05/19 : DEM1331 - Ajout Mecanisme KEEPALIVE 1.24
------------------------------------------------------*/

/* fichiers inclus */

#include <xzsem.h>
#include "ex_mrtf.h"

#include "esrv_art.h"
#include "esrv_dir.h"

/* definitions de constantes */

/* definitions de types locaux */


/* definition de macro locales */

/* declaration de variables locales */


XZEXT_MASK *	vg_SockMask = NULL;
int		vg_SockEct	= 0;
int		vg_SockTETIM	= 0;
int		vg_SockEctLT	= 0;
int		vg_SockTESRV	= 0;
T_SOCK		vg_SockRT	= 0;

/* declaration de fonctions internes */
void	ed_lancer_timer ( void );


/* definition de fonctions externes */
extern void ec_surveillance_tache ( );
extern void ec_verif_arret_tache ();




/* ------------------------------------------------------
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
 *   Abonnement au Datagroup XDG_EFIN_CONFIG_NomMachine.
 *
 *
------------------------------------------------------*/

int	argc;
char	**argv;
{
  XZSCT_NomTache vl_tache;
  int vl_ValRet 	= XDC_OK,
      vl_SockRet	= 0,
      vl_LgMsgSock	= 0,
      vl_IndCnx 	= 0,
      vl_EssaiSrvLT	= 0;
  XZEXT_MSG_SOCKET	pl_MsgSock;
  XDY_NomSite pl_NomSite;
  XDY_NomMachine pl_NomMachine;
  XDY_Datagroup		pl_NomDG;
  T_IPC_MSG              	vl_Msg;
  double			vl_Horodate;
  int           		vl_jourSemaine;
  char			vl_Repertoire[XDC_PATH_ABS_SIZE] = "";
  int                     vl_NbEqt        = 0,
      vl_NbConfLigne  = 0;



	/*A
	*  initialisation semaphore
	*/
	sem_init(&vg_semaphore, 0, 1);

  /*A
   * Connexion au RTserver et initialisation des variables
   */

  if ( XZSC_01InitCnxSup( NULL,
      argv[ 0 ],
      XZSCC_INTERDIRE,
      XZSCC_AUTORISER,
      XZSCC_INTERDIRE,
      ed_arreter,
      "/dev/null" ) != XDC_OK )
  {
    /*B Affichage Trace */
    XZST_03EcritureTrace( XZSTC_FATAL, "Ne peut ouvrir la connexion avec le RTserver." );
  }
  else
  {
    /*B Affichage Trace */
    XZST_03EcritureTrace( XZSTC_DEBUG1, "Ouverture de la connexion avec le RTserver." );
  }


  /*A  */
  if ( argv[ 1 ] != NULL )
  {
    vg_InitTranserver = XDC_VRAI;
  }

  /* Recuperation du site de la machine */
  if ( (vl_ValRet = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
  {
    XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom du site impossible ." );
  }


  /*A Recuperation du nom de la machine */
  if ( XZSC_07NomMachine(pl_NomMachine) != XDC_OK)
  {
    /* Ecriture Trace */
    XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom de la machine impossible ." );
  }

  /*A Abonnement au Datagroup XDG_EFIN_CONFIG_NomMachine */
  XDG_EncodeDG2 ( pl_NomDG, XDG_EFIN_CONFIG , pl_NomMachine) ;
  /*A joint ce datagroup */
  if ( !XDG_JoindreDG( pl_NomDG ) )
  {
    /* Affichage Trace */
    XZST_03EcritureTrace(XZSTC_WARNING,
        "Abonnement au Datagroup %s non effectue.", pl_NomDG );
  }

  /*A Abonnement au Datagroup XDG_NSUPETAT_NomMachine */
  XDG_EncodeDG2 ( pl_NomDG, XDG_NSUPETAT , pl_NomMachine) ;
  /*A joint ce datagroup */
  if ( !XDG_JoindreDG( pl_NomDG ) )
  {
    /* Affichage Trace */
    XZST_03EcritureTrace(XZSTC_WARNING,
        "Abonnement au Datagroup %s non effectue.", pl_NomDG );
  }

//XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_WARNING|XZSTC_INFO|XZSTC_FONCTION|XZSTC_INTERFACE|XZSTC_MESSAGE,XZSTC_NECRAN);
XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_WARNING|XZSTC_INFO|XZSTC_FONCTION|XZSTC_DEBUG1|XZSTC_DEBUG2|XZSTC_DEBUG3|XZSTC_INTERFACE,XZSTC_NECRAN);


  /*A Creation des repertoires EQUEXT ( ala, lcr, wrf, rdf ) */
  sprintf ( vl_Repertoire, "%s/%s", XZEXC_PATH_FIC, XZEXC_REP_ALA );
  if ( access ( vl_Repertoire,F_OK ) )
  {
    mkdir ( vl_Repertoire, S_IRWXU | S_IRWXG | S_IRWXO  );
    chmod ( vl_Repertoire, S_IRWXU | S_IRWXG | S_IRWXO  );
  }
  sprintf ( vl_Repertoire, "%s/%s", XZEXC_PATH_FIC, XZEXC_REP_LCR );
  if ( access ( vl_Repertoire,F_OK ) )
  {
    mkdir ( vl_Repertoire, S_IRUSR | S_IRGRP | S_IROTH | S_IWUSR | S_IWGRP | S_IWOTH | S_IXUSR | S_IXGRP | S_IXOTH );
    chmod ( vl_Repertoire, S_IRWXU | S_IRWXG | S_IRWXO  );
  }
  sprintf ( vl_Repertoire, "%s/%s", XZEXC_PATH_FIC, XZEXC_REP_WRF );
  if ( access ( vl_Repertoire,F_OK ) )
  {
    mkdir ( vl_Repertoire, S_IRUSR | S_IRGRP | S_IROTH | S_IWUSR | S_IWGRP | S_IWOTH | S_IXUSR | S_IXGRP | S_IXOTH );
    chmod ( vl_Repertoire, S_IRWXU | S_IRWXG | S_IRWXO  );
  }
  sprintf ( vl_Repertoire, "%s/%s", XZEXC_PATH_FIC, XZEXC_REP_RDF );
  if ( access ( vl_Repertoire,F_OK ) )
  {
    mkdir ( vl_Repertoire, S_IRUSR | S_IRGRP | S_IROTH | S_IWUSR | S_IWGRP | S_IWOTH | S_IXUSR | S_IXGRP | S_IXOTH );
    chmod ( vl_Repertoire, S_IRWXU | S_IRWXG | S_IRWXO  );
  }
  /*A Creation du repertoire pour les sockets */
  ex_rep_socket ( pl_NomMachine );

  /*A Arret des taches EQUEXT si elle sont presente mais orpheline */
  ex_lire_config_lignes ( pl_NomMachine, &vl_NbConfLigne );
  ex_lire_config_eqt    ( pl_NomMachine, &vl_NbEqt );
  ec_verif_arret_tache ();
  el_arret_tache ( 0, 0, XZECC_TACHE_TIM );


  /*A Connexion socket en mode serveur pour communiquer avec la machine suppleante */
  while ( ex_cnx_serveur_LT ( pl_NomMachine, XZEXC_PORT_SERVEUR_LT, &vg_SockMask, &vg_SockEctLT ) != XDC_OK )
  {
    /*B Ecriture Trace */
    XZST_03EcritureTrace( XZSTC_DEBUG1, "Impossible de creer la connexion serveur LT." );
    if ( vl_EssaiSrvLT++ > 5 )
      XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de creer la connexion serveur LT dans la minute." );
    sleep ( 10 );
    if ( vl_EssaiSrvLT++ > 10 )
      XZST_03EcritureTrace( XZSTC_FATAL, "Impossible de creer la connexion serveur LT dans les 2 minutes." );

  }

  /*A Connexion socket en mode serveur pour communiquer avec les taches traitements */
  if ( ex_cnx_serveur ( XZEXC_ADR_SOCK_TESRV, &vg_SockMask, &vg_SockEct ) != XDC_OK )
  {
    /*B Ecriture Trace */
    XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de creer la connexion serveur ." );
  }

  /*A Lancement de la tache TETIM */
  ed_lancer_timer ();

  /*A  Armement Callback sur Reception XDM_EFIN_CONFIG */
  if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_EFIN_CONFIG),
      ec_init_cb,(T_CB_ARG)NULL) == NULL)
  {
    /*B Ecriture Trace */
    XZST_03EcritureTrace( XZSTC_WARNING, "Callback sur XDM_EFIN_CONFIG non declenchee." );
  }

  /*A  Armement Callback sur Reception XDM_ECMD_EQPT */
  if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_ECMD_EQPT),
      ec_cmd_cb,(T_CB_ARG)NULL) == NULL)
  {
    /* Ecriture Trace */
    XZST_03EcritureTrace( XZSTC_WARNING, "Callback sur XDM_ECMD_EQPT non declenchee.");
  }

  /*A  Armement Callback sur Reception XDM_TSE_DIFE */
  if ( (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_TSE_DIFE),     ec_dde_etat_ST_cb,(T_CB_ARG)NULL) == NULL) ||
      (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_TSE_TACH),     ec_dde_etat_ST_cb,(T_CB_ARG)NULL) == NULL) ||
      (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_TST_TRAC),     ec_dde_etat_ST_cb,(T_CB_ARG)NULL) == NULL) ||
      (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_TSE_MODI),     ec_dde_etat_ST_cb,(T_CB_ARG)NULL) == NULL) ||
      (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_TSE_SSYS),     ec_dde_etat_ST_cb,(T_CB_ARG)NULL) == NULL) ||
      (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_TSE_MACH),     ec_dde_etat_ST_cb,(T_CB_ARG)NULL) == NULL) ||
      (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_TSE_APPLI),    ec_dde_etat_ST_cb,(T_CB_ARG)NULL) == NULL) ||
      (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_TSE_ISAG),     ec_dde_etat_ST_cb,(T_CB_ARG)NULL) == NULL) ||
      (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_TSE_ISSS),     ec_dde_etat_ST_cb,(T_CB_ARG)NULL) == NULL) ||
      (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_TSE_ISTC),     ec_dde_etat_ST_cb,(T_CB_ARG)NULL) == NULL) ||
      (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_TSE_STARTTMP), ec_dde_etat_ST_cb,(T_CB_ARG)NULL) == NULL) )
  {
    /* Ecriture Trace */
    XZST_03EcritureTrace( XZSTC_WARNING, "Callback sur XDM_TSE_DIFE ou TSE_TACH  ou TST_TRAC ou TSE_STARTTMP non declenchee.");
  }

  /* Memorisation lancement des taches non effectuee  */
  vg_ConfigTerm = XDC_NOK;

  /*A Demande d'initialisation de tous les equipement pour la machine locale a TEINI */
  /*A Emission du message XDM_EINIT sur le DatGroup XDG_EINIT_NomSite */

  XDG_EncodeDG2 ( pl_NomDG, XDG_EINIT , pl_NomSite);
  if (!TipcSrvMsgWrite (  pl_NomDG
      ,XDM_IdentMsg(XDM_EINIT)
      ,XDM_FLG_SRVMSGWRITE
      ,T_IPC_FT_STR,pl_NomMachine
      ,T_IPC_FT_INT4,XZECC_TYPE_EQT_ALL
      ,T_IPC_FT_INT4,XZECC_MACHINE_PRINC
      ,NULL))
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_EINIT non effectue." );
  }

  /* Abonnement au DataGroup KEEPALIVE */
  XZSC_31AbtKeepAlive(pl_NomSite);

  /*A Attente des messages sur les sockets (RTserver, d'ecoute et de donnees */
  while (1)
  {
    if ( ex_attendre_msg ( vg_SockEct, &vg_SockMask, &vl_SockRet ) != XDC_OK )
    {
      XZST_03EcritureTrace(XZSTC_WARNING, "Probleme retour fonction select." );
      ex_ConnexionSocketRTserver ( &vg_SockRT, vl_SockRet, &vg_SockMask );
      if ( vl_SockRet == vg_SockTESRV ) vg_SockTESRV = 0;
      if ( vl_SockRet == vg_SockTETIM ) vg_SockTETIM = 0;
    }
    else
    {
      ex_ConnexionSocketRTserver ( &vg_SockRT, 0, &vg_SockMask );
      if ( vg_SockTETIM == 0 )
      {
        ed_lancer_timer ();
        ec_verif_arret_tache ();
      }

      /*A Si une socket a ete activee */
      if ( vl_SockRet != XZEXC_NO_EVT )
      {
        /*A Si il s'agit d'une connexion du LT suppleant  */
        if ( vl_SockRet == vg_SockEctLT )
        {
          ex_acpt_cnx ( AF_INET, vg_SockEctLT, &vg_SockMask, &vg_SockTESRV );
          if (vg_SockTESRV)
          {
            XZST_03EcritureTrace(XZSTC_DEBUG1, "Connexion d'un LT :%d.", vg_SockTESRV );
            sprintf ( pl_MsgSock, "%s %s", pl_NomMachine, XZEXC_FIN_MSG_SOCK );
            send ( vg_SockTESRV, pl_MsgSock, (int) strlen(pl_MsgSock), 0 );
          }
        }

        XZST_03EcritureTrace(XZSTC_WARNING, "main : ici");
        /*A S'il s'agit d'une socket de donnee : lire le message */
        if ( (vl_SockRet != vg_SockEct) && (vl_SockRet != vg_SockEctLT) && (vl_SockRet != vg_SockRT) )
        {
//XZST_03EcritureTrace(XZSTC_WARNING, "main : avant ex_lire_msg");
          /*A Lire le message de la socket de donnees */
          if ( ex_lire_msg ( vl_SockRet, pl_MsgSock, &vl_LgMsgSock, &vg_SockMask, &vl_IndCnx ) != XDC_OK )
          {
            /*A Ecriture trace erreur lecture message socket sur esrv_dir */
            XZST_03EcritureTrace(XZSTC_WARNING, "erreur lecture message socket sur esrv_dir.");
          }
          else
          {
            /*A S'il ne s'agit pas d'un message de deconnexion */
            if ( (vl_IndCnx != XZEXC_DECONNEC) && (vl_LgMsgSock != 0) )
            {
//XZST_03EcritureTrace(XZSTC_WARNING, "main : apres if ( (vl_IndCnx != XZEXC_DECONNEC) && (vl_LgMsgSock != 0) )");
              /*A Appel de ee_diff_chgt_mode */
              ee_diff_chgt_mode ( pl_MsgSock, vl_SockRet );

//XZST_03EcritureTrace(XZSTC_WARNING, "main : apres ee_diff_chgt_mode");

              /*A Si il s'agit d'un reveil de TETIM */
              if ( strstr(pl_MsgSock, XZEXC_MSG_REVEIL) != NULL )
              {
//XZST_03EcritureTrace(XZSTC_WARNING, "main : apres if ( strstr(pl_MsgSock");
                if (vg_SockTESRV)
                {
                  sprintf ( pl_MsgSock, "%s %s", pl_NomMachine, XZEXC_FIN_MSG_SOCK );
//XZST_03EcritureTrace(XZSTC_WARNING, "main : avant send ( vg_SockTESRV");
                  send ( vg_SockTESRV, pl_MsgSock, (int) strlen(pl_MsgSock), 0 );
                }

                /*A si init terminee */
                if ( vg_ConfigTerm == XDC_NOK )
                {
//XZST_03EcritureTrace(XZSTC_WARNING, "main : apres if ( vg_ConfigTerm == XDC_NOK )");
                  /*A Appel de la fonction ec_config du module MCFG pour la configuration des  taches */
                  XZST_03EcritureTrace(XZSTC_WARNING,"Debut lancement de l'application sur Time-Out. " );
                  if ( ec_config ( XZECC_TYPE_EQT_ALL ) != XDC_OK )
                  {
                    vg_ConfigTerm = XDC_NOK;
                    /*A Ecriture trace Configuration de l'application non effectuee */
                    XZST_03EcritureTrace(XZSTC_WARNING,"Configuration de l'application non effectuee. " );
                  }
                  else
                  {
                    /* Memorisation lancement des taches effectuee avec succes */
                    XZST_03EcritureTrace(XZSTC_WARNING,"Application lancee avec succes " );
                    vg_ConfigTerm = XDC_OK;
                  }
                }
                /*A sinon */
                else
                  /*A Appel de ec_surveillance_tache */
 {
//XZST_03EcritureTrace(XZSTC_WARNING, "main avant ec_surveillance_tache"); 
                  ec_surveillance_tache ( );
//XZST_03EcritureTrace(XZSTC_WARNING, "main apres ec_surveillance_tache"); 
 }
              }
            }
            else
            {
              if ( vl_SockRet == vg_SockTESRV ) vg_SockTESRV = 0;
              if ( vl_SockRet == vg_SockTETIM ) vg_SockTETIM = 0;
              if ( (vl_SockRet != vg_SockTESRV) && (vl_SockRet != vg_SockTETIM) )
                ec_surveillance_tache ( );

            }
          }
        }   /* fin si socket de donnees */
      }   /* fin si socket active */
    }
  }   /* fin while */




}  /* Fin main */



/*X*/
/*------------------------------------------------------
 * SERVICE RENDU :
 *  Fonction effectuant les echanges avec TESUR pendant le lancement de l'appli.
 *
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

void	ed_dialogue_tesur ( void )

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
  static char *version = "$Id: esrv_dir.c,v 1.25 2020/11/03 18:12:32 pc2dpdy Exp $ :  ed_dialogue_tesur " ;
  XZEXT_MASK *	 vl_SockMask = NULL;
  XZEXT_MASK *	 pl_masque = NULL;
  XZEXT_MASK	 vl_masque;
  XZEXT_MSG_SOCKET pl_MsgSock;
  XDY_NomMachine   pl_NomMachine;
  int              vl_SockRet	= 0,
      vl_LgMsgSock	= 0,
      vl_IndCnx 	= 0;


  /*A Insertion du descripteur de la socket dans le masque de lecture */
  for ( pl_masque = vg_SockMask; pl_masque != NULL; pl_masque = pl_masque->Suivant )
  {
    /*A Si differente de la socket RTserver et differente de la socket d'ecoute */
    if ( (pl_masque->desc_sock == vg_SockEctLT ) || (pl_masque->desc_sock == vg_SockTESRV) || (pl_masque->desc_sock == vg_SockTETIM) )
    {
      vl_masque.desc_sock = pl_masque->desc_sock;
      vl_masque.Type = pl_masque->Type;
      strcpy ( vl_masque.adresse, pl_masque->adresse );
      ex_ins_masque ( &vl_masque, &vl_SockMask );
    }
  }

  if ( ex_attendre_msg ( XZEXC_SOCKET_NULL, &vl_SockMask, &vl_SockRet ) != XDC_OK )
  {
    XZST_03EcritureTrace(XZSTC_WARNING, "Probleme retour fonction select." );
    if ( vl_SockRet == vg_SockTESRV ) vg_SockTESRV = 0;
    if ( vl_SockRet == vg_SockTETIM ) vg_SockTETIM = 0;
  }
  else
  {
    /*A Si une socket a ete activee */
    if ( vl_SockRet != XZEXC_NO_EVT )
    {
      /*A Si il s'agit d'une connexion du LT suppleant  */
      if ( vl_SockRet == vg_SockEctLT )
      {
        ex_acpt_cnx ( AF_INET, vg_SockEctLT, &vg_SockMask, &vg_SockTESRV );
        if (vg_SockTESRV)
        {
          XZSC_07NomMachine(pl_NomMachine);
          sprintf ( pl_MsgSock, "%s %s", pl_NomMachine, XZEXC_FIN_MSG_SOCK );
          send ( vg_SockTESRV, pl_MsgSock, (int) strlen(pl_MsgSock), 0 );
          XZST_03EcritureTrace(XZSTC_WARNING, "Connexion de TESUR :%d et emission de <%s>.", vg_SockTESRV, pl_MsgSock );
        }
      }

      /*A S'il s'agit d'une socket de donnee : lire le message */
      if ( vl_SockRet != vg_SockEctLT )
      {
        /*A Lire le message de la socket de donnees */
        if ( ex_lire_msg ( vl_SockRet, pl_MsgSock, &vl_LgMsgSock, &vg_SockMask, &vl_IndCnx ) != XDC_OK )
        {
          /*A Ecriture trace erreur lecture message socket sur esrv_dir */
          XZST_03EcritureTrace(XZSTC_WARNING, "erreur lecture message socket sur esrv_dir.");
        }
        else
        {
          XZST_03EcritureTrace(XZSTC_DEBUG1, "Reception de <%s>.", pl_MsgSock );
          /*A S'il ne s'agit pas d'un message de deconnexion */
          if ( (vl_IndCnx != XZEXC_DECONNEC) && (vl_LgMsgSock != 0) )
          {
            /*A Si il s'agit d'un reveil de TETIM */
            if ( strstr(pl_MsgSock, XZEXC_MSG_REVEIL) != NULL )
            {
              if (vg_SockTESRV)
              {
                XZSC_07NomMachine(pl_NomMachine);
                sprintf ( pl_MsgSock, "%s %s", pl_NomMachine, XZEXC_FIN_MSG_SOCK );
                send ( vg_SockTESRV, pl_MsgSock, (int) strlen(pl_MsgSock), 0 );
                XZST_03EcritureTrace(XZSTC_DEBUG1, "Emission de <%s> vers TESUR.", pl_MsgSock );
              }
            }
          }
          else
          {
            if ( vl_SockRet == vg_SockTESRV ) vg_SockTESRV = 0;
            if ( vl_SockRet == vg_SockTETIM ) vg_SockTETIM = 0;
          }
        }
      }   /* fin si socket de donnees */
    }   /* fin si socket active */
  }

  for ( pl_masque = vl_SockMask; pl_masque != NULL; pl_masque = pl_masque->Suivant )
    ex_sup_masque ( pl_masque->desc_sock, &vl_SockMask);

}

/*------------------------------------------------------
 * SERVICE RENDU :
 *  Fonction effectuant les operations de terminaison de la tache.
 *
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

void	ed_arreter ( void )

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

  register int		vl_IndexTache = 0;
  int		vl_NbCarEmis = 0;
  XZSCT_NomTache	pl_NomTache;
  XZEXT_MASK *	pl_masque;

  /*A
   * Arret des taches equipements et la tache TETIM (appel de ed_arreter_tache_eqpt)
   */
  /*A Pour toutes les taches memorisees dans le masque */
  for ( pl_masque = vg_SockMask; pl_masque != NULL; pl_masque = pl_masque->Suivant )
  {
    /*A Si differente de la socket RTserver et differente de la socket d'ecoute */
    if ( ( pl_masque->Type == AF_UNIX ) &&
        ( pl_masque->desc_sock != vg_SockRT ) &&
        ( pl_masque->desc_sock != vg_SockEct ) &&
        ( strcmp (pl_masque->adresse, XZEXC_ADR_SOCK_TEMOD) ) )
    {
      /*A Envoie de la demande d'arret ( appel de ee_config ) */
      ee_config ( XZEXC_DEMANDE_ARRET, pl_masque->desc_sock, pl_masque->adresse );
    }
  }   /* fin for */


  /*
   *A Verification arret des taches
   */
  sleep ( XZEXC_ATTENTE_ARRET_TACHE );
  ec_verif_arret_tache ();

  /*A
   * Attente infinie sur tout les Pid des taches lancees
   */

  /*A Pour toutes les taches memorisees dans le masque */
  for ( pl_masque = vg_SockMask; pl_masque != NULL; pl_masque = pl_masque->Suivant )
  {
    /*A Si differente de la socket RTserver et differente de la socket d'ecoute */
    if ( ( pl_masque->Type == AF_UNIX ) &&
        ( pl_masque->desc_sock != vg_SockEct ) &&
        ( strcmp (pl_masque->adresse, XZEXC_ADR_SOCK_TEMOD) ) )
    {
      /*A Recherche du nom de la tache */
      strcpy ( pl_NomTache, pl_masque->adresse );
      strcat ( pl_NomTache, ".x" );
      /*A Attente tache terminee */
      while ( XZSS_20PidTache ( pl_NomTache ) != 0 )
      {
        sleep ( XZEXC_ATTENTE_ARRET_TACHE );
      }
    }
  }



  /*A
   * Fermeture de toutes les sockets
   */

  /*A Pour toute les sockets du masque */
  for ( pl_masque = vg_SockMask; pl_masque != NULL; pl_masque = pl_masque->Suivant )
  {
    /*A Fermeture de la socket courante */
    ex_fin_cnx ( pl_masque->desc_sock, &vg_SockMask );
  }

  XZST_03EcritureTrace(XZSTC_WARNING,"Sortie de tache TESRV .");
  /*A Arret de la tache */
  exit ( 0 );



}

/*X*/
/*------------------------------------------------------
 * SERVICE RENDU :
 *  Fonction effectuant le lancement de la tache tetim.
 *
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

void	ed_lancer_timer ( void )

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
  static char *version = "$Id: esrv_dir.c,v 1.25 2020/11/03 18:12:32 pc2dpdy Exp $ : ed_lancer_timer  " ;
  XZEXT_MSG_SOCKET	pl_MsgTimer;

  /*A Lancement de la tache TETIM */
  el_arret_tache ( 0, 0, XZECC_TACHE_TIM );
  if ( el_lancer_tache ( XZEXC_TACHE_TETIM, NULL ) != XDC_OK )
  {
    XZST_03EcritureTrace(XZSTC_WARNING,"Impossible lancer tache TETIM ." );
  }
  sleep (10);
  /*A
   * Connexion socket en mode client pour communiquer avec la tache TETIM
   */
  if ( ex_cnx_client ( XZEXC_ADR_SOCK_SRVTIM,
      XZEXC_ADR_SOCK_TETIM_SRV,
      &vg_SockMask,
      &vg_SockTETIM ) != XDC_OK )
  {
    XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de creer la connexion client TETIM ." );
  }
  else
  {
    XZST_03EcritureTrace( XZSTC_WARNING, "Connexion etablie avec TETIM ." );
  }

  if ( vg_SockTETIM )
  {
    /*A Demande de reveil pour time_out discordance */
    ETIM_REVEIL_CYCLIQUE ( 30, "TESRV_Timer_30s", vg_SockTETIM, pl_MsgTimer );
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

void	ed_arreter_tache_eqpt ( void )

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
  static char *version = "$Id: esrv_dir.c,v 1.25 2020/11/03 18:12:32 pc2dpdy Exp $ : ed_arreter_tache_eqpt " ;
  XZEXT_MASK *	pl_masque;


  /*A
   * Arret des taches equipements et la tache TETIM )
   */
  /*A Pour toutes les taches memorisees dans le masque */
  for ( pl_masque = vg_SockMask; pl_masque != NULL; pl_masque = pl_masque->Suivant )
  {
    /*A Si differente de la socket RTserver et differente de la socket d'ecoute */
    if ( ( pl_masque->Type == AF_UNIX ) &&
        ( pl_masque->desc_sock != vg_SockEct ) &&
        ( pl_masque->desc_sock != vg_SockTETIM ) &&
        ( strcmp (pl_masque->adresse, XZEXC_ADR_SOCK_TETIM) != 0 ) &&
        ( strcmp (pl_masque->adresse, XZEXC_ADR_SOCK_TEMOD) != 0 ) )
    {
      /*A Envoie de la demande d'arret ( appel de ee_config ) */
      ee_config ( XZEXC_DEMANDE_ARRET, pl_masque->desc_sock, pl_masque->adresse );
    }
  }   /* fin for */

  sleep ( XZEXC_ATTENTE_ARRET_TACHE );

  /*
   *A Verification arret des taches
   */
  ec_verif_arret_tache ();


}






