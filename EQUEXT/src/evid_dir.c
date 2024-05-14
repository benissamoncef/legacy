/* Fichier : $Id: evid_dir.c,v 1.17 2019/06/04 13:48:01 devgtie Exp $        Release : $Revision: 1.17 $        Date : $Date: 2019/06/04 13:48:01 $
------------------------------------------------------
 * GTIE *  PROJET MIGRAZUR
------------------------------------------------------
 * SOUS-SYSTEME : EQUEXT
------------------------------------------------------
 * MODULE TEVID * FICHIER evid_dir.c
------------------------------------------------------
 * DESCRIPTION DU MODULE :
 *
 *   Module directeur de la tache VIDEO
 *
------------------------------------------------------
 * HISTORIQUE :
 *
 * Nagiel.E	version 1.1	28 Oct 1994	: Creation
 * Mismer.D	version 1.2	14 Dec 1994	:
 * Nagiel.E	version 1.3	23 Dec 1994	:
 * Nagiel.E	version 1.4	26 Dec 1994	:
 * Mismer.D	version 1.5	17 Jan 1995	:
 * Volcic.F	version 1.6	24 Jan 1995	:
 * Fontaine.C	version 1.7	23 Mai 1995	: Reprise historique
 * Mismer.D  	version 1.8	07 Jun 1995	: Modif connexion socket RTserver
 * Mismer.D  	version 1.9	31 Aou 1995	: Modif connexion socket RTserver suite
 * Mismer.D  	version 1.10	11 Oct 1995	: Ajout appel fonction init Acknoledge alarme
 * Mismer.D	version 1.11	18 Jan 1998	: Modif pour basculement PC2
 * Mismer.D	version 1.12	29 Mai 2000	: Modif pour basculement PC2 Suite et fin (DEM/65)
 * Hilmarcher    version 1.13                    : modif pour utilisation cam pc niv 2 depuis CI
 * PNI		v1.15		07/07/2008	: ajout de SPDA pour l'appel a XZAE103 DEM790
 * PNI		23/06/09 v1.5	: renommage SPDA en SPPR DEM887
 * JPL		03/11/16 : Suppression init alarme, abonnements etat site et equipements, traitement trame (DEM 1189)  1.16
 * RGR 17/05/19 : DEM1331 - Ajout Mecanisme KEEPALIVE 1.17
------------------------------------------------------*/

/* fichiers inclus */

#include <xzsem.h>
#include "ex_mrtf.h"

#include "evid_cyc.h"
#include "evid_dir.h"

/* definitions de constantes */

/* definitions de types locaux */

typedef struct {
  int		NumMsg;		/* Numero de message */
  XDY_Fonction	FoncCB;		/* Fonction a appeler */
}tm_callback;

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: evid_dir.c,v 1.17 2019/06/04 13:48:01 devgtie Exp $ : evid_dir" ;

static tm_callback pm_TabCB[] =
    {
        { XDM_ECMD_VIDEO, 		(void (*)())ev_cmd_VIDEO_cb },
        { XDM_ELCR, 			(void (*)())ev_CMD_LCR }
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

int main ( argc, argv )

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
 * Initialisations :connexion rtserver, connection avec les taches TESRV,
 * init des structures en memoire, gestion des callbacks
 * Boucle sur les sockets et gestion des messages recus
 *
------------------------------------------------------*/

int	argc;
char	**argv;
{
  int 			vl_ValRet	=XDC_OK,
      vl_SockRet	= 0,
      vl_SockSRV	= 0,
      vl_Type;
  XDY_Datagroup		pl_NomDG;
  int			vl_indice_callback;
  XZEXT_MSG_SOCKET	pl_MsgTimer;


  /*A
   *  Recuperation du nom de la machine
   */

  strcpy(vg_NomMachine,argv[1]);
  XZST_03EcritureTrace( XZSTC_WARNING, "Nom de la machine : VIDEO : %s", vg_NomMachine );

	/*A
	*  initialisation semaphore
	*/
	sem_init(&vg_semaphore, 0, 1);

  /*
   * Connexion au RTserver et initialisation des variables
   */
  if( XZSC_01InitCnxSup(  NULL,
      argv[ 0 ],
      XZSCC_INTERDIRE,
      XZSCC_AUTORISER,
      XZSCC_INTERDIRE,
      ec_sortir,
      "/dev/null" ) != XDC_OK )
  {
    /*B Affichage Trace */
    XZST_03EcritureTrace( XZSTC_WARNING, "Ne peut ouvrir la connexion avec le RTserver." );
    exit( XDC_NOK );
  }
  else
  {
    /*B Affichage Trace */
    XZST_03EcritureTrace( XZSTC_DEBUG1, "Ouverture de la connexion avec le RTserver." );
  }

  XZST_03EcritureTrace( XZSTC_WARNING, "FIN INIT DE CONNECTION : ");


  /*A Creation du repertoire pour les sockets */
  ex_rep_socket ( argv[1] );


  /*A
   * Recuperation du site de la machine
   */
  if ( (vl_ValRet = XZSC_04NomSite(vg_NomSite)) != XDC_OK)
  {
    /* Ecriture Trace */
    XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom du site impossible . " );
  }

  /*A
   * Connexion socket en mode client pour communiquer avec la tache TESRV
   */
  if ( ex_cnx_client ( 	XZEXC_ADR_SOCK_TEVID,
      XZEXC_ADR_SOCK_TESRV,
      &vg_SockMask,
      &vl_SockSRV ) != XDC_OK )
  {
    /*B Ecriture Trace */
    XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de creer la connexion client TESRV." );
  }

  /*A
   * Connexion socket en mode client pour communiquer avec la tache TETIM
   */
  if ( ex_cnx_client ( 	XZEXC_ADR_SOCK_VIDTIM,
      XZEXC_ADR_SOCK_TETIM_SRV,
      &vg_SockMask,
      &vg_SockTIM ) != XDC_OK )
  {
    /*B Ecriture Trace */
    XZST_03EcritureTrace( XZSTC_FATAL, "Impossible de creer la connexion client TETIM." );
  }

  /*A
   * Appel de MCFG fonction ec_init
   */
  ec_init(vg_NomMachine);

  /*A
   * Armement des Callback sur reception des message
   */

  /* Initialisation des callbacks de la tache */

  for (vl_indice_callback = 0; vl_indice_callback < (sizeof(pm_TabCB)/sizeof(pm_TabCB[0])); vl_indice_callback++)
  {
    if(!TipcSrvProcessCbCreate(	XDM_IdentMsg(pm_TabCB[vl_indice_callback].NumMsg ) ,
        pm_TabCB[vl_indice_callback].FoncCB ,
        (T_CB_ARG)NULL) )
    {
      XZST_03EcritureTrace(XZSTC_WARNING,
          "erreur init callback '%s'",
          pm_TabCB[vl_indice_callback].FoncCB);
    }
  }

  /*A
   * Abonnement aux Datagroup
   */
  /* Messages de controle : XDG_ECMD_VIDEO_<NomSite> */
  XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_VIDEO, vg_NomSite) ;
  /* joint ce datagroup */
  if ( !XDG_JoindreDG( pl_NomDG ) )
  {
    /* Affichage Trace */
    XZST_03EcritureTrace(XZSTC_WARNING,
        "Abonnement au Datagroup %s non effectue ", pl_NomDG );
  }

  /* Messages de controle : XDG_ECMD_VIDEO_<NomMachine> */
  XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_VIDEO, vg_NomMachine) ;
  /* joint ce datagroup */
  if ( !XDG_JoindreDG( pl_NomDG ) )
  {
    /* Affichage Trace */
    XZST_03EcritureTrace(XZSTC_WARNING,
        "Abonnement au Datagroup %s non effectue ", pl_NomDG );
  }

  /* Abonnement au DataGroup KEEPALIVE */
  XZSC_31AbtKeepAlive(vg_NomSite);

  /* Messages de demande d'etat video : XDG_EINIT_ABT_VIDEO */
  //	XDG_EncodeDG2 ( pl_NomDG, XDG_EINIT_ABT_VIDEO, vg_NomSite ) ;
  //   	/* joint ce datagroup */
  //   	if ( !XDG_JoindreDG( pl_NomDG ) )
  //   	{
  //	   /* Affichage Trace */
  //      	   XZST_03EcritureTrace(XZSTC_WARNING,
  //	   "Abonnement au Datagroup %s non effectue ", pl_NomDG );
  //	}

  /* Programation timer 1MN */
  ETIM_REVEIL_CYCLIQUE ( EVIDC_REVEIL_1MN, EVIDC_TIMER_1MN, vg_SockTIM, pl_MsgTimer );


  /*XZST_08ForceNivVisuTraceLocal (XZSTC_INFO|XZSTC_FONCTION|XZSTC_INTERFACE|XZSTC_MESSAGE|XZSTC_DEBUG1|XZSTC_DEBUG2|XZSTC_DEBUG3, XZSTC_NECRAN);*/
  XZST_08ForceNivVisuTraceLocal (XZSTC_INFO|XZSTC_MESSAGE|XZSTC_DEBUG1|XZSTC_DEBUG2, XZSTC_NECRAN);

  /*A Attente des messages sur les sockets (RTserver, d'ecoute et de donnees */
  while (1)
  {
    /*XZST_03EcritureTrace ( XZSTC_DEBUG3, "Attente infinie d'un message" );*/

    if ( ex_attendre_msg ( XZEXC_SOCKET_NULL, &vg_SockMask, &vl_SockRet ) != XDC_OK )
    {
      XZST_03EcritureTrace(XZSTC_WARNING, "Probleme retour fonction select  ." );
      ex_ConnexionSocketRTserver ( &vg_SockRT, vl_SockRet, &vg_SockMask );
    }
    else
    {
      ex_ConnexionSocketRTserver ( &vg_SockRT, 0, &vg_SockMask );

      /*A Si une socket a ete activee */
      if ( vl_SockRet != XZEXC_NO_EVT )
      {
        /* Selon le type de socket */
        /* Socket RT server */
        if ( vl_SockRet == vg_SockRT )
        {
          /*B Le message est lu par les primitive standard de RTWorks */
        }
        else
        {
          /* Socket TETIM (retour Timer) */
          if ( vl_SockRet == vg_SockTIM )
          {
            /* Appel de la fonction ec_traitement_cyclique de MCYC */
            ey_traitement_cyclique ( vg_SockTIM );
          }
          else
          {
            /*B Socket TESRV (Init ou Arret tache) */
            if ( vl_SockRet == vl_SockSRV )
            {
              /* Appel de la fonction ec_configuration de MCFG */
              ec_controle ( vl_SockSRV, vg_NomMachine );
            }
            else
            {
              /*B Aucune autre socket n'est scrutee ici */
              XZST_03EcritureTrace(XZSTC_WARNING, "Evenement inattendu .");
            }
          }
        }
      }   /* fin si socket active */
    }
  }   /* fin while */

}  /* Fin main */
