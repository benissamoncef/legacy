/*Fichier : $Id: elmi_dir.c,v 1.19 2019/06/04 13:40:55 devgtie Exp $      Release : $Revision: 1.19 $        Date : $Date: 2019/06/04 13:40:55 $
------------------------------------------------------
 * GTIE *  PROJET MIGRAZUR
------------------------------------------------------
 * SOUS-SYSTEME : EQUEXT
------------------------------------------------------
 * MODULE TELMi * FICHIER elmi_dir.c
------------------------------------------------------
 * DESCRIPTION DU MODULE :
 *
 * Programme principale de la tache TELMi
 *
------------------------------------------------------
 * HISTORIQUE :
 *
 * Flaidar.A	version 1.1	20 Oct 1994	: Creation
 * Mismer.D	version 1.2	18 Nov 1994	: Ajout repertoire socket et numero de ligne sur socket
 * Flaidar.A	version 1.3	01 Dec 1994	: Ajout verif. malloc O.K.
 * Mismer.D	version 1.4	13 Jan 1995	:
 * Volcic.F	version 1.5	13 Jan 1995	:
 * Mismer.D	version 1.6	26 Jan 1995	:
 * Mismer.D	version 1.7	08 Mar 1995	:
 * Mismer.D 	version 1.8 	19 Mai 1995 	: Modif gestion deconnexion ST
 * Fontaine.C	version 1.9	23 Mai 1995	: Reprise historique
 * Mismer.D 	version 1.10 	02 Jun 1995 	: Modif arg XZST_10ArchiverMsgSyst
 * Mismer.D 	version 1.11 	27 Jun 1995 	: Modif arg trace reconnexion ST
 * Mismer.D  	version 1.12	31 Aou 1995	: Modif connexion socket RTserver suite
 * Mismer.D      version 1.13    05 Mar 1996     : Appel ecom_traitement_liaison_ST a la place de edir_traitement_liaison_ST (DEM/1100)
 * Mismer.D      version 1.14    25 Mar 1997     : Ajout date pour TDP (DEM/1396)
 * JMG		version 1.15	22/06/01	: linux DEM/934
 * PNI		version 1.16	06/01/11	: force le niv de trace
 * JPL		25/02/11 : Migration architecture HP ia64 (DEM 975) : suppression warnings mineurs  1.17
 * PNI		version 1.18	04/06/13	: FATAL en cas de perte client sous linux comme sous HPUX
 * RGR 17/05/19 : DEM1331 - Ajout Mecanisme KEEPALIVE 1.19
------------------------------------------------------*/

/* fichiers inclus */
#include <xzsem.h>²

#include "ex_mdon.h"
#include "ex_mrtf.h"

#include "elmi_dir.h"


/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */


/* declaration de fonctions externes */

/* definitions fonctions internes */
int edir_detecte_type_sock( void );

/* definition de fonctions internes */

void edir_traitement_msg_sock( XZEXT_Msg_Socket * );
void edir_traitement_dcnx( void );
void edir_traitement_liste_cmdes_LCR( void );
/*void edir_traitement_liaison_ST( void );*/



/*
------------------------------------------------------
 * SERVICE RENDU :
 * Programme principal
------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

int main( int argc, char **argv )

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

  int	vl_ValRet 	= XDC_OK;		/* valeur retour */
  int	vl_EtatCnx 	= XZEXC_CONNEC_UNDEF;	/* etat connexion socket */
  XZEXT_Msg_Socket	vl_MsgSock;		/* struct. creation msg. socket */
  XZEXT_MASK *pl_Mask;
  XDY_Texte	vl_Texte;
  XZEXT_ADR_SOCK pl_SrvTelmi;
  XDY_NomSite pl_NomSite = "";

  /*
   * sequence initialisation tache TELMi
   */

  /* init a zero des descs. sockets specifiques taches TELxi */
  vg_DesSocks.curr = XZEXC_SOCKET_NULL;
  vg_DesSocks.ect = XZEXC_SOCKET_NULL;
  vg_DesSocks.lser = XZEXC_SOCKET_NULL;
  vg_DesSocks.rtsrv = XZEXC_SOCKET_NULL;

  /* Recuperation nom machine */
  strcpy( vg_Config.nom_machine, argv[1] );

  /* Recuperation no. ligne serie */
  vg_Config.no_ligne = atoi( argv[2] );

  XZST_03EcritureTrace( XZSTC_DEBUG1,"elmi_dir.x : Connexion au RTserver...\n" );

  /*A
   *  initialisation semaphore
   */
  sem_init(&vg_semaphore, 0, 1);

  /* SI Connexion au RTserver et initialisation des variables echoue */
  if( XZSC_01InitCnxSup(  NULL,
      argv[0],
      XZSCC_INTERDIRE,
      XZSCC_AUTORISER,
      XZSCC_INTERDIRE,
      ecfg_sortir,
      "/dev/null" ) != XDC_OK  )
  {
    /*B ALORS Affichage Trace "Ouverture connexion avec le RTserver impossible!!!" */
    XZST_03EcritureTrace( XZSTC_FATAL, "Ouverture connexion avec le RTserver impossible!!!" );
    /*B exit NOK */
    exit( XDC_NOK );
  }

  XZST_03EcritureTrace( 	XZSTC_DEBUG1, "Ouverture cnx. RTServer reussie ..." );

  /* Creation du repertoire pour les sockets */
  ex_rep_socket ( vg_Config.nom_machine );
  /* Lecture user/password */
  ex_lire_idpassw ( vg_Config.nom_machine, vg_idpassw );


  /*
   * connexion soket en mode serveur pour comms. avec la tache traitement
   */
  /* SI connexion echoue */
  sprintf ( pl_SrvTelmi, "%s%d", XZEXC_ADR_SOCK_TELM, vg_Config.no_ligne );
  if( ex_cnx_serveur( pl_SrvTelmi, &vg_SockList, &vg_DesSocks.ect ) != XDC_OK )
  {
    /* ALORS */
    /*B ecriture trace : " Connexion socket ecoute mode serveur impossible !" */
    XZST_03EcritureTrace( XZSTC_FATAL, " Connexion socket ecoute mode serveur impossible !" );
    /*B sortie XDC_NOK */
    exit( XDC_NOK );
  }

  XZST_03EcritureTrace( 	XZSTC_DEBUG1, " Nom machine %s, no. ligne serie %d ",
      vg_Config.nom_machine, vg_Config.no_ligne );

  /*
   * Initialisation configuration de la ligne serie ds. vg_Config
   */
  XZST_03EcritureTrace( XZSTC_WARNING, "elmi_dir TELMI ligne %d : initialisation config ...",
      vg_Config.no_ligne );

  if ( (vg_EtatST = ecom_init( &vg_Config, &vg_SockList, &vg_DesSocks )) != XDC_OK )
    XZST_03EcritureTrace( XZSTC_WARNING, " Config No port %d impossible",  vg_Config.no_port );
  else
    XZST_03EcritureTrace( XZSTC_WARNING, " Fin init config No port %d ",  vg_Config.no_port );

//  XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_WARNING|XZSTC_DEBUG1|XZSTC_INFO,XZSTC_NECRAN);
  XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_WARNING|XZSTC_INFO,XZSTC_NECRAN);

  /* Recuperation du site de la machine */
  if ( (vl_ValRet = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
  {
    XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom du site impossible ." );
  }

  /* Abonnement au DataGroup KEEPALIVE */
  XZSC_31AbtKeepAlive(pl_NomSite);

  /*
   * Boucle infinie FAIRE TOUJOURS
   */
  while( 1 )
  {

    /* traitement des eventuelles cmdes. LCR empilees ds. la liste de cmdes. non traitees */
    /* Surveillance liaison LS */
    ecom_traitement_liaison_ST( &vg_Config, &vg_SockList, &vg_DesSocks, &vg_EtatST );
    edir_traitement_liste_cmdes_LCR();

    /* SI attente message sur liste sockets : ecoute, lserie, donnees client echoue */
    if( ex_attendre_msg( vg_DesSocks.ect, &vg_SockList, &vg_DesSocks.curr ) != XDC_OK )
    {
      /* ALORS ecriture trace :  " Probleme attente message sur socket(s)." */
      XZST_03EcritureTrace( XZSTC_WARNING, " Probleme attente message sur socket(s)." );
      ex_ConnexionSocketRTserver ( &vg_DesSocks.rtsrv, vg_DesSocks.curr, &vg_SockList );
    }
    else
    {
      ex_ConnexionSocketRTserver ( &vg_DesSocks.rtsrv, 0, &vg_SockList );

      /*B SI socket active = socket RTServer */
      if( vg_DesSocks.curr == vg_DesSocks.rtsrv )
      {
        XZST_03EcritureTrace(XZSTC_DEBUG1, "elmi_dir.x : appel primitive standard lecture RTWorks" );

        /*B ALORS appel primitive standard lecture RTWorks */
      }
      /*B SINON SI evenement sur une socket diff. de la socket d'ecoute
       *  (si. evt. sur socket ecoute <=> cnx. d'un client geree par ex_attendre_msg())
       *  tout est O.K. => lecture sur socket active */
      else if( vg_DesSocks.curr != XZEXC_NO_EVT 	/* une socket active */
          && vg_DesSocks.curr != vg_DesSocks.ect) /* cas connexion client */
      {
        XZST_03EcritureTrace( XZSTC_DEBUG1, "elmi_dir.x : evenement sur une socket diff. de la socket d'ecoute...\n\n" );

        /*B detection adresse socket active */
        ecom_detecte_clnt( &vg_DesSocks, &vg_SockList );

        XZST_03EcritureTrace( XZSTC_DEBUG1, "elmi_dir.x : lecture sur socket %d active\n",
            vg_DesSocks.curr );

        /*B SI lecture sur socket active echoue */
        if( ex_lire_msg( vg_DesSocks.curr, vl_MsgSock.msg, &vl_MsgSock.len,
            &vg_SockList, &vl_EtatCnx ) != XDC_OK )
        {
          /*B ALORS ecriture trace :  Probleme lecture socket adresse <vg_DesSocks.curr_addr> */
          XZST_03EcritureTrace( XZSTC_WARNING, " Probleme lecture socket adresse %s...",
              vg_DesSocks.curr_addr );
        }

        /*B SINON */
        else
        {
          /*B CAS etat connexion socket active */
          switch( vl_EtatCnx )
          {
            /*B cas : XZEXC_DECONNEC */
            case XZEXC_DECONNEC:

              XZST_03EcritureTrace( XZSTC_WARNING, "Deconexion <%s>...\n", vg_DesSocks.curr_addr );

              /*B appel fonction traitement dcnxion : edir_traitement_dcnx() */
              edir_traitement_dcnx();
              break;

              /*B cas : XZEXC_CONNEC */
            case XZEXC_CONNEC :

              XZST_03EcritureTrace( XZSTC_DEBUG1, "Connexion <%s>...\n", vg_DesSocks.curr_addr );

              /*B appel fonction trait. mess. recu edir_traitement_msg_sock( &vl_MsgSock )*/
              edir_traitement_msg_sock( &vl_MsgSock );
              break;

              /*B cas par defaut */
            default :
              /*B ecriture trace : Etat connexion socket non defini..." */
              XZST_03EcritureTrace( XZSTC_WARNING, " Etat connexion socket non defini...");
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





/*X*/
/*------------------------------------------------------
 * SERVICE RENDU :
 * Traitement du message receptionne par elmi_dir sur la socket active.
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
 * pa_MsgSock	: pointeur sur le message lu par elmi_dir.
 *
 * ARGUMENTS EN SORTIE :
 * NEANT
 *
 * CODE RETOUR :
 *
 * NEANT
 *
 * CONDITION D'UTILISATION
 * - les champs de pa_DesSocks doivent contenir les descs. des sockets ouverts par elmi_dir.
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
void edir_traitement_msg_sock( XZEXT_Msg_Socket *pa_MsgSock )
{
  static char *version = "$Id: elmi_dir.c,v 1.19 2019/06/04 13:40:55 devgtie Exp $ : edir_traitement_msg_sock" ;

  XZEXT_Cmde_LCR 	vl_CmdeLCR;	/* structure depouillement message recu sur socket */
  XDY_Texte		vl_Texte;
  int vl_ValRet;
  /* buffer stokage trame lisible */
  char 		*pl_TrameLisible;


  /*
   * traitement message en fonction du type de la socket active
   */
  /* CAS type socket active */
  switch( edir_detecte_type_sock() )
  {
    case XZEXC_SOCK_ACT_DONN :	/* cas XZEXC_SOCK_ACT_DONN */
      /*B detection type message recu d'un client TEzzz sur une socket donnees */

      /*B SI demande d'arret ALORS appel ecfg_sortir() */
      if( strstr( pa_MsgSock->msg, XZEXC_DEMANDE_ARRET ) != NULL )
      {
        XZST_03EcritureTrace( XZSTC_WARNING, "edir_traitement_msg_sock : demande arret TELMi %d",
            vg_Config.no_ligne );

        ecfg_sortir();
      }
      /*B SINON SI demande init. ALORS appel ecom_init() */
      else if( strstr( pa_MsgSock->msg, XZEXC_DEMANDE_INIT ) != NULL )
      {
        XZST_03EcritureTrace( XZSTC_WARNING, "edir_traitement_msg_sock : demande initialisation TELMi %d",
            vg_Config.no_ligne );
        if ( (vg_EtatST = ecom_init( &vg_Config, &vg_SockList, &vg_DesSocks )) != XDC_OK )
          XZST_03EcritureTrace( XZSTC_WARNING, "edir_traitement_msg_sock : relance init telm%d", vg_Config.no_ligne );
        else
          XZST_03EcritureTrace( XZSTC_WARNING, "edir_traitement_msg_sock : relance init telm%d", vg_Config.no_ligne );

      }
      /*B SINON SI demande envoi trame LCR sur ligne serie geree
       *  ALORS appel epro_envoi_trame_LCR() */
      else if( strstr( pa_MsgSock->msg, XZEXC_DEMANDE_LCR ) != NULL )
      {
        /*B SI depouillement message recu sur socket donnees : ecom_extraire_cmd_LCR() OK */
        if( ecom_extraire_cmd_LCR( pa_MsgSock, &vl_CmdeLCR, vg_DesSocks.curr ) == XDC_OK )
        {
          /*B ALORS epro_envoi_trame_LCR() */
          epro_envoi_trame_LCR( &vl_CmdeLCR );
        }
        /*B SINON return */
        else
        {
          return;
        }
      }
      /*B SINON SI demande inconnue ( diff. de ARRET, INIT ou LCR )
       *  ALORS ecriture trace : " Recu demande TEzzz inconnue : <demande_TEzzz>" */
      else
      {
        XZST_03EcritureTrace( XZSTC_WARNING, " Recu demande TEzzz inconnue : %s", pa_MsgSock->msg );
      }
      break;

    case XZEXC_SOCK_ACT_LSER :	/* cas XZEXC_SOCK_ACT_LSER */
      /*B Normallement reception reponses equps. ds. epro_envoi_trame_LCR().
       *  si socket ligne serie active <=> reception reponse arrivee en retard.
       *  => ecriture trace : " Reponse TEDI/LCR perdue <trame_TEDI/LCR> "
       */
      if( (pl_TrameLisible = malloc( 4 * (size_t) pa_MsgSock->len + 1 )) )
      {
        XZST_03EcritureTrace( XZSTC_WARNING, " Trame TEDI/LCR perdue <%s> ",
            ecom_rendre_trame_lisible( pa_MsgSock->msg, pa_MsgSock->len, pl_TrameLisible ) );
        free( pl_TrameLisible );
      }
      break;

    default :	/* defaut ( XZEXC_SOCK_ACT_UNDEF ) */
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
int edir_detecte_type_sock( void )
{
  static char *version = "$Id: elmi_dir.c,v 1.19 2019/06/04 13:40:55 devgtie Exp $ : edir_detecte_type_sock" ;
  int		vl_TypeSockAct	= XZEXC_SOCK_ACT_UNDEF;

  /*
   * detection type socket active
   */
  /* SI socket RTServer active ALORS RETURN XZEXC_SOCK_ACT_RTSRV */
  if( vg_DesSocks.curr == vg_DesSocks.rtsrv )
  {
    return( XZEXC_SOCK_ACT_RTSRV );
  }
  /* SI socket ligne serie active ALORS return XZEXC_SOCK_ACT_LSER */
  if( vg_DesSocks.curr == vg_DesSocks.lser )
  {
    return( XZEXC_SOCK_ACT_LSER );
  }
  /* SI socket ecoute => connexion client => ALORS return XZEXC_SOCK_ACT_UNDEF */
  if( vg_DesSocks.curr == vg_DesSocks.ect )
  {
    return( XZEXC_SOCK_ACT_UNDEF );
  }

  /* SINON => socket donnees (client TEzzz) active : return XZEXC_SOCK_ACT_DONN */
  return( XZEXC_SOCK_ACT_DONN );
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
void edir_traitement_dcnx( void )
{
  static char *version = "$Id: elmi_dir.c,v 1.19 2019/06/04 13:40:55 devgtie Exp $ : edir_traitement_dcnx" ;

  XDY_Texte		vl_Msg;

  /* ecriture trace : Deconnexion socket adresse <vg_DesSocks.curr_addr>...*/
  XZST_03EcritureTrace( XZSTC_WARNING, " Deconnexion socket adr. <%s>.vg_DesSocks.curr=%d.vg_DesSocks.lser=%d.vg_DesSocks.ect=%d.vg_DesSocks.rtsrv=%d.", vg_DesSocks.curr_addr,
      vg_DesSocks.curr,vg_DesSocks.lser,vg_DesSocks.ect,vg_DesSocks.rtsrv);

  /* SI deconnexion socket serveur terminaux */
  if( vg_DesSocks.curr == vg_DesSocks.lser )
    vg_EtatST = XDC_NOK;
  /*#ifndef _HPUX_SOURCE*/
  if (vg_DesSocks.curr == vg_DesSocks.ect)
    XZST_03EcritureTrace( XZSTC_FATAL,"Perte client => ARRET !");
  /*#endif*/

  /* SI deconnexion socket RTWorks */
  if( vg_DesSocks.curr == vg_DesSocks.rtsrv )
  {
    /*B ALORS ecriture trace "Deconexion socket comms. avec RTServer"*/
    XZST_03EcritureTrace( XZSTC_WARNING,
        "Deconexion socket comms. avec RTServer !" );
  }
  if  ( (vg_DesSocks.curr != vg_DesSocks.rtsrv) && ( vg_DesSocks.curr != vg_DesSocks.lser ) && ( vg_DesSocks.curr != vg_DesSocks.ect ))
    XZST_03EcritureTrace( XZSTC_FATAL,"Perte socket ??? => ARRET !");

  /* SI deconnexion socket RTWorks */
  if ( (vg_DesSocks.curr == vg_DesSocks.rtsrv) && ( vg_DesSocks.curr == vg_DesSocks.lser ) );

}




/*X*/
/*------------------------------------------------------
 * SERVICE RENDU :
 * Traitement du message receptionne par elmi_dir sur la socket active.
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

void edir_traitement_liste_cmdes_LCR( void )

{

  XZEXT_Cmde_LCR 	vl_CmdeLCR;	/* structure cmde. LCR depilee */


  /* depilement de la liste des cmdes. LCR a traiter */
  /*    edir_traitement_liaison_ST( );*/

  /* TANT QUE la liste des cmdes. n'est pa vide */
  while( pg_ListeCmdesLCR != NULL )
  {
    /*B sortie de la 1ere cmde. ds. la liste : appel ecom_getfirst...() */
    ecom_getfirst_cmd_LCR( &vl_CmdeLCR, &pg_ListeCmdesLCR );

    /*B envoi de la cmde. depilee sur la ligne serie : appel envoi epro_envoi_trame_LCR( ) */
    epro_envoi_trame_LCR( &vl_CmdeLCR );
  }
  /* FIN TANT QUE */
}



/*X*/
/*------------------------------------------------------
 * SERVICE RENDU :
 * Traitement de la connexion avec le serveur de terminaux
 *
------------------------------------------------------
 * SEQUENCE D'APPEL :
 *
 *
 * void edir_traitement_liaison_ST( void );
 *
 * ARGUMENTS EN ENTREE :
 *
 * NEANT
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
 * Appel de cette fonction si la liaison avec le serveur de terminaux est HS
 *
 * FONCTION
 *
 * Essai de connexion sur la ligne serie
 *
------------------------------------------------------*/

void  edir_traitement_liaison_ST( void )
{
  static char *version = "$Id: elmi_dir.c,v 1.19 2019/06/04 13:40:55 devgtie Exp $ : edir_traitement_liaison_ST  ";

  XDY_Texte		vl_Texte = "";
  static int 		vl_CptTimeOut;
  int			vl_EtatLS = vg_EtatST;

  if ( (vl_CptTimeOut++ > (XZEXC_DUREE_ATTENTE*3)) && (vg_EtatST != XDC_OK) )
  {
    /* Essai connexion avec ST */
    vl_CptTimeOut = 0;
    if ( (vg_EtatST = ecom_init( &vg_Config, &vg_SockList, &vg_DesSocks )) != XDC_OK )
      XZST_03EcritureTrace( XZSTC_WARNING, "Connexion avec le Serveur de Terminaux coupee !" );
    else
      XZST_03EcritureTrace( XZSTC_WARNING, "Connexion avec le Serveur de Terminaux retablie !" );
  }

  if ( vl_EtatLS != vg_EtatST )
  {
    /* message pour l'admin. systeme XZST_10ArchiverMsgSyst() : Etat connexion ST */
    sprintf( vl_Texte, "TELMI ligne serie %d : connexion avec le Serveur de Terminaux %s %s !\0",
        vg_Config.no_ligne,
        vg_Config.ligne_serie.NumeroST,
        (vg_EtatST == XDC_OK) ?  "retablie" : "coupee" );
    XZST_10ArchiverMsgSyst( 0, vl_Texte, NULL );
  }

  if ( vg_EtatST == XDC_OK )
    vl_CptTimeOut = 0;

}
