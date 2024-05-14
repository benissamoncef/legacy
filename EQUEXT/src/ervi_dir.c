/*E*/
/* Fichier : $Id: ervi_dir.c,v 1.2 2019/06/04 13:45:03 devgtie Exp $     Release : $Revision: 1.2 $      Date :  $Date: 2019/06/04 13:45:03 $
------------------------------------------------------
 * GTIE *  PROJET MIGRAZUR
------------------------------------------------------
 * SOUS-SYSTEME : EQUEXT
------------------------------------------------------
 * MODULE TEPAL * FICHIER epal_dir.c
------------------------------------------------------
 * DESCRIPTION DU MODULE :
 *
 * C'est le module directeur de la tache TEPAL
 *
------------------------------------------------------
 * HISTORIQUE :
 *
 * JMG	14/11/18 : Creation  RMVI DEM1315 1.1
 * RGR 17/05/19 : DEM1331 - Ajout Mecanisme KEEPALIVE 1.2
 * LCL		25/09/2023	: Adaptation code pour rabbitmq DEM-483
*
------------------------------------------------------*/

/* fichiers inclus */

#include <xzsem.h>
#include "ervi_dir.h"
#include "ervi_cmd.h"
#include "xzems.h"

/* definitions de constantes */

/* definitions de types locaux */

typedef struct {
  int		NumMsg;		/* Numero de message */
  XDY_Fonction	FoncCB;		/* Fonction a appeler */
}tm_callback;

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: ervi_dir.c,v 1.2 2019/06/04 13:45:03 devgtie Exp $ : epal_dir" ;

/* declaration de fonctions internes */
void ec_sortir();
int     Demande_RMVI (char *va_site, char *va_requete, char *va_autoroute,
    int va_pr, XDY_Octet va_sens, char *va_type);

/* definition de fonctions externes */


/*X*/
/* Fichier : @(#)epal_dir.c	1.15      Release : 1.15        Date : 09/30/96
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
------------------------------------------------------*/

int	argc;
char	**argv;
{
  int             vl_IndPre       = 0;
  int             vl_Attente      = 0;

  T_IPC_MSG       pl_Msg;

  int 			vl_ValRet	=XDC_OK,
      vl_SockRet	= 0,
      vl_SockSRV	= 0,
      vl_resultat;
  XDY_Datagroup		pl_NomDG;
  int			vl_indice_callback;
  XDY_Mot			vl_MaxEqtLigne = 0;
  XDY_Mot			vl_CoefTimer = 1;

  /*A
   *  Recuperation du nom de la machine
   */
  strcpy(vg_NomMachine,argv[1]);

	/*A
	*  initialisation semaphore
	*/
	sem_init(&vg_semaphore, 0, 1);

  /*A
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
  }
  else
  {
    /*B Affichage Trace */
    XZST_03EcritureTrace( XZSTC_DEBUG1, "Ouverture de la connexion avec le RTserver." );
  }

  /*A
   * Recuperation du site de la machine
   */
  if ( (vl_ValRet = XZSC_04NomSite(vg_NomSite)) != XDC_OK)
  {
    /* Ecriture Trace */
    XZST_03EcritureTrace( XZSTC_FATAL, "Recuperation du Nom du site impossible . " );
  }

  XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_WARNING|XZSTC_INFO|XZSTC_FONCTION|XZSTC_INTERFACE|XZSTC_MESSAGE,XZSTC_NECRAN);

  /*A
   * Armement des Callback sur reception des message
   */

  /* Initialisation des callbacks de la tache */

  if(!TipcSrvProcessCbCreate(	XDM_IdentMsg(XDM_RMVI_DEMANDE ) ,
      en_demande_cb,
      (T_CB_ARG)NULL) )
  {
    XZST_03EcritureTrace(XZSTC_FATAL,
        "erreur init callback ");
  }

  /*A
   * Abonnement aux Datagroup
   */
  /* Messages de commande PAL : XDG_ECMD_PAL_<NomMachine> */
  /* joint ce datagroup */
  if ( !XDG_JoindreDG( XDG_RMVI ) )
  {
    /* Affichage Trace */
    XZST_03EcritureTrace(XZSTC_FATAL, "Abonnement au Datagroup %s non effectue ", pl_NomDG );
  }


  /*vl_Attente = Demande_RMVI ("DP","9999-26", "A50", 66000, 1, "ACCIDENT");*/

  /* Abonnement au DataGroup KEEPALIVE */
  XZSC_31AbtKeepAlive(vg_NomSite);

  /*A Attente des messages sur les sockets (RTserver, d'ecoute et de donnees */

  while (1)
  {
    /* Recuperation du msg RTworks suivant */
    pl_Msg = TipcSrvMsgNext(T_TIMEOUT_FOREVER);

    if(isMessageNotNULL(pl_Msg))
    {

      TipcSrvMsgProcess(pl_Msg);
      TipcMsgDestroy(pl_Msg);

      /* Memorisation Test Presence RTserver */
      vl_IndPre = 0;
    }

    /* erreur RTWORKS */
    else
    {
      /* Test de Presence RTserver */
      if (vl_IndPre == 0)
      {
        if (errno)
          XZST_03EcritureTrace (XZSTC_WARNING, "Erreur RTWorks num: %d -- errno %d \n",
              TutErrNumGet(), errno);
      }
      else if (vl_IndPre >= XDC_EXIT_RTCLIENT)
      {
        XZST_03EcritureTrace (XZSTC_WARNING, "Bouclage et arret tache tdrol %d \n",vl_IndPre);

        /*A Arret de la tache */
        ec_sortir();
      }

      /*B INdication Non Presence RTserver */
      if (errno)
        vl_IndPre++;


    }

  }   /* fin while */




}  /* Fin main */

void ec_sortir() {
  exit(0);
}


int     Demande_RMVI (char *va_site, char *va_requete, char *va_autoroute,
    int va_pr, XDY_Octet va_sens, char *va_type)
{

  XZSCT_Datagroup vl_Dg;

  XDG_EncodeDG2(vl_Dg, XDG_RMVI, va_site);

  if (!TipcSrvMsgWrite(XDG_RMVI
      ,XDM_IdentMsg(XDM_RMVI_DEMANDE)
      ,XDM_FLG_SRVMSGWRITE
      ,T_IPC_FT_STR,(T_STR) va_requete
      ,T_IPC_FT_STR,(T_STR) va_autoroute
      ,T_IPC_FT_INT4,(T_INT4) va_pr
      ,T_IPC_FT_INT2,(T_INT2) va_sens
      ,T_IPC_FT_STR,(T_STR) va_type
      ,NULL) )
  {
    /* Affichage Trace */
    XZST_03EcritureTrace(XZSTC_WARNING
        ," XZIS50_Demande_RMVI:pb emission message sur %s"
        ,vl_Dg);
    return(XDC_NOK);
  }
  XZST_03EcritureTrace(XZSTC_FONCTION
      ," XZIS50_Demande_RMVI:emission message sur %s",
      vl_Dg);

  return (XDC_OK);
}
