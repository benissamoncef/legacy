/* Fichier : $Id: elei_dir.c,v 1.17 2019/06/04 13:40:22 devgtie Exp $        Release : $Revision: 1.17 $        Date : $Date: 2019/06/04 13:40:22 $
------------------------------------------------------
 * GTIE *  PROJET MIGRAZUR
------------------------------------------------------
 * SOUS-SYSTEME : EQUEXT
------------------------------------------------------
 * MODULE TELEi * FICHIER elei_dir.c
------------------------------------------------------
 * DESCRIPTION DU MODULE :
 *
 * Programme principal de la tache controle protocole TEDI/LCR
 * en mode esclave TELEi.
 *
 *
------------------------------------------------------
 * HISTORIQUE :
 *
 * Flaidar.A	version 1.1	04 Nov 1994	: Creation
 * Flaidar.A	version 1.2	02 Dec 1994	: Ajout fonction traitement message recu sur une socket.
 *          	           	           	  Mise a jour procedure init, ajout envoi alarme.
 *          	           	                  Mise a jour procedure traitement deconnexion ST.
 * Mismer.D	version 1.3	23 Jan 1995	:
 * Mismer.D	version 1.4	11 Avr 1995	:
 * Mismer.D	version 1.5	10 Mai 1995	:
 * Mismer.D      Version 1.6     19 Mai 1995     : Modif gestion deconnexion ST
 * Fontaine.C	version 1.7	23 Mai 1995	: Reprise historique
 * Mismer.D 	version 1.8 	02 Jun 1995 	: Modif arg XZST_10ArchiverMsgSyst
 * Mismer.D 	version 1.9 	27 Jun 1995 	: Modif arg trace reconnexion ST
 * Mismer.D  	version 1.10	31 Aou 1995	: Modif connexion socket RTserver suite
 * Mismer.D  	version 1.11	15 Nov 1995	: Modif traitement sur message eqpt avant ACK ou NACK
 * Mismer.D  	version 1.12	07 Fev 1996	: Appel ecom_traitement_liaison_ST a la place de edir_traitement_liaison_ST
 *
 * ???		xx/07/07 : Traces   1.13
 * ???		xx/10/09 : Traces (ecom_rendre_trame_lisible)  1.14
 * JPL		25/02/11 : Migration architecture HP ia64 (DEM 975) : suppression warnings mineurs  1.15
 * JMG		01/03/11 : passage 64 bits, suppression d'un appel a ecom_rendre_trame_lisible incorrect 1.16
 * RGR 17/05/19 : DEM1331 - Ajout Mecanisme KEEPALIVE 1.17
------------------------------------------------------*/

/* fichiers inclus */
#include <xzsem.h>²

#include "ex_mrtf.h"

#include "elei_dir.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: elei_dir.c,v 1.17 2019/06/04 13:40:22 devgtie Exp $ : elei_dir" ;
XZEXT_MSG_SOCKET vm_MsgEqt = "";

/* declaration de fonctions externes */

/* definition de fonctions internes */

void edir_traitement_msg_sock( XZEXT_Msg_Socket * );
void edir_traitement_dcnx( void );
int edir_detecte_type_sock( void );


/*X*/
/*------------------------------------------------------
 * SERVICE RENDU :
 *  main de la tache TELEi.
 *
------------------------------------------------------
 * SEQUENCE D'APPEL :
 *
 *
 * void main( int argc, char **argv )
 *
 * ARGUMENTS EN ENTREE :
 *
 * argc 		: nbre. d'arguments.
 * argv[1]	: nom de la machine.
 * argv[2]	: no. de la ligne serie geree.
 *
 * ARGUMENTS EN SORTIE :
 * NEANT.
 *
 * CODE RETOUR :
 * NEANT.
 *
 *
 * CONDITION D'UTILISATION
 *
 *
 * FONCTION
 *
 * Programme principale tache TELEi.
 *
------------------------------------------------------*/
int main( int argc, char **argv )
{

  int			vl_EtatCnx = XZEXC_CONNEC_UNDEF;	/* etat connexion socket */
  XZEXT_Msg_Socket	vl_MsgSock;		/* struct. creation msg. socket */
  XDY_Texte		vl_Texte;		/* texte general use */
  int vl_ValRet;		/* stokage codes retour */
  XZEXT_ADR_SOCK	pl_SrvTelei;		/* adresse socket ecoute TELEi */
  XDY_NomSite pl_NomSite = "";


  /*A init a zero des descs. sockets specifiques taches TELxi */
  vg_DesSocks.curr = XZEXC_SOCKET_NULL;
  vg_DesSocks.ect = XZEXC_SOCKET_NULL;
  vg_DesSocks.lser = XZEXC_SOCKET_NULL;
  vg_DesSocks.rtsrv = XZEXC_SOCKET_NULL;

  /*A Recuperation nom machine */
  strcpy( vg_Config.nom_machine,argv[1] );

  /*A Recuperation no. ligne serie */
  vg_Config.no_ligne = atoi( argv[2] );

  XZST_03EcritureTrace( XZSTC_DEBUG1,"elei_dir.x : Connexion au RTserver...\n" );

	/*A
	*  initialisation semaphore
	*/
	sem_init(&vg_semaphore, 0, 1);

  /*A SI Connexion au RTserver et initialisation des variables echoue */
  if( XZSC_01InitCnxSup(  NULL,
      argv[0],
      XZSCC_INTERDIRE,
      XZSCC_AUTORISER,
      XZSCC_INTERDIRE,
      ecfg_sortir,
      "/dev/null" ) != XDC_OK  )
  {
    /*B ALORS Affichage Trace "Overture connexion avec le RTserver impossible!!!" */
    XZST_03EcritureTrace( XZSTC_WARNING, "elei_dir.x : Ouverture connexion avec le RTserver impossible!!!" );
    /*B exit NOK */
    exit( XDC_NOK );
  }

  XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_WARNING|XZSTC_INFO|XZSTC_FONCTION|XZSTC_INTERFACE|XZSTC_MESSAGE,XZSTC_NECRAN|XZSTC_DEBUG1|XZSTC_DEBUG2|XZSTC_DEBUG3);
  XZST_03EcritureTrace( XZSTC_DEBUG1, "elei_dir.x : Ouverture cnx. RTServer reussie ..." );


  /*! recuperation nom site ds. vg_Config.nom_site et mise a jour vg_AdrSAE */
  /*A Recuperation du site de la machine */
  if ( (vl_ValRet = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
  {
    /* Ecriture Trace */
    XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom du site impossible ." );
  }
  strcpy( vg_AdrSAE, XZEXC_RGS_SAE );
  vg_AdrSAE[2] = pl_NomSite[1];

  /*A Creation du repertoire pour les sockets */
  ex_rep_socket ( vg_Config.nom_machine );

  /*A
   * connexion soket en mode serveur pour comms. avec la tache traitement
   */
  /*A SI connexion echoue */
  sprintf ( pl_SrvTelei, "%s%d", XZEXC_ADR_SOCK_TELE, vg_Config.no_ligne );
  if( ex_cnx_serveur( pl_SrvTelei, &vg_SockList, &vg_DesSocks.ect ) != XDC_OK )
  {
    /*A ALORS */
    /*B ecriture trace : " Connexion socket ecoute mode serveur impossible !" */
    XZST_03EcritureTrace( XZSTC_WARNING, "elei_dir.x : Connexion socket ecoute mode serveur impossible !" );
    /*B sortie XDC_NOK */
    exit( XDC_NOK );
  }


  XZST_03EcritureTrace( XZSTC_DEBUG1, "elei_dir.x : Nom machine %s, no. ligne serie %d ",
      vg_Config.nom_machine, vg_Config.no_ligne );

  /*A
   * Initialisation configuration de la ligne serie ds. vg_Config.ligne_serie.
   */
  /*A
   * Initialisation configuration de la ligne serie ds. vg_Config
   */
  XZST_03EcritureTrace( XZSTC_DEBUG1, "elei_dir.x : lancement initialisation \n" );

  if ( (vg_EtatST = ecom_init( &vg_Config, &vg_SockList, &vg_DesSocks )) != XDC_OK )
    XZST_03EcritureTrace( XZSTC_WARNING, " Config No port %d impossible",  vg_Config.no_port );
  else
    XZST_03EcritureTrace( XZSTC_WARNING, " Fin init config No port %d ",  vg_Config.no_port );

  XZST_03EcritureTrace( XZSTC_DEBUG1, "elei_dir TELEi ligne %d: boucle infinie...",  vg_Config.no_ligne );

  /* Abonnement au DataGroup KEEPALIVE */
  XZSC_31AbtKeepAlive(pl_NomSite);

  /*A
   * Boucle infinie FAIRE TOUJOURS
   */
  while( 1 )
  {
    /*A Surveillance liaison LS */
    ecom_traitement_liaison_ST( &vg_Config, &vg_SockList, &vg_DesSocks, &vg_EtatST );

    /*A SI attente message sur liste sockets : ecoute, lserie, donnees client echoue */
    if( ex_attendre_msg( vg_DesSocks.ect, &vg_SockList, &vg_DesSocks.curr ) != XDC_OK )
    {
      /*A ALORS ecriture trace :  " Probleme attente message sur socket(s)." */
      ex_ConnexionSocketRTserver ( &vg_DesSocks.rtsrv, vg_DesSocks.curr, &vg_SockList );
      if ( vg_DesSocks.curr == vg_DesSocks.lser ) vg_EtatST = XDC_NOK;
    }
    else
    {
      ex_ConnexionSocketRTserver ( &vg_DesSocks.rtsrv, 0, &vg_SockList );

      /*B SI socket active = socket RTServer */
      if( vg_DesSocks.curr == vg_DesSocks.rtsrv )
      {
        /*B ALORS appel primitive standard lecture RTWorks */
      }
      /*B SINON SI socket active autre que la socket d'ecoute */
      else if( vg_DesSocks.curr != XZEXC_NO_EVT 	/* une socket active */
          && vg_DesSocks.curr != vg_DesSocks.ect) /* cas connexion client */
      {

        /*B detection adresse socket active */
        ecom_detecte_clnt( &vg_DesSocks, &vg_SockList );

        /*B SI lecture sur socket active echoue */
        if( ex_lire_msg( vg_DesSocks.curr, vl_MsgSock.msg, &vl_MsgSock.len,
            &vg_SockList, &vl_EtatCnx ) != XDC_OK )
        {
          /*B ALORS ecriture trace :  Probleme lecture socket adresse <vg_DesSocks.curr_addr> */
          XZST_03EcritureTrace( XZSTC_INFO, "elei_dir :  Probleme lecture socket adresse %s...",
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
              /*B appel fonction traitement dcnxion : edir_traitement_dcnx() */
              edir_traitement_dcnx();
              break;

              /*B cas : XZEXC_CONNEC */
            case XZEXC_CONNEC :
              /*B appel fonction trait. mess. recu edir_traitement_msg_sock( &vl_MsgSock )*/
              edir_traitement_msg_sock( &vl_MsgSock );
              break;

              /*B cas par defaut */
            default :
              /*B ecriture trace : Etat connexion socket non defini..." */
              XZST_03EcritureTrace( XZSTC_INFO, "elei_dir :  Etat connexion socket non defini...");
              break;
          }
          /* FIN CAS etat connexion */
        }
        /*B FIN SI */
      }
      /*B FIN SI */
    }
    /*A FIN SI */
  }
  /*A FIN FAIRE TOUJOURS */

}   




/*X*/
/*------------------------------------------------------
 * SERVICE RENDU :
 * Traitement du message receptionne par elie_dir sur la socket active.
 *
------------------------------------------------------
 * SEQUENCE D'APPEL :
 *
 *
 * void edir_traitement_msg_sock( XZEXT_Msg_Socket *pa_MsgSock )
 *
 * ARGUMENTS EN ENTREE :
 *
 * pa_MsgSock	: pointeur sur le message lu par elei_dir.
 *
 * ARGUMENTS EN SORTIE :
 * NEANT
 *
 * CODE RETOUR :
 *
 * NEANT
 *
 * CONDITION D'UTILISATION
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
  static char *version = "$Id: elei_dir.c,v 1.17 2019/06/04 13:40:22 devgtie Exp $ : mainedir_traitement_msg_sock";

  XDY_Texte		vl_TypeMsg;			/* juste pour recuperer le type de message */
  XDY_Texte		vl_Texte;			/* texte utilisation generale messages trace */
  int 		vl_ValRet;			/* stokage retour de certaines fonctions */
  int			vl_TypeTrame;			/* type trame de reponse tache traitement */
  char		*pl_TrameLisible;		/* pointeur sur la trame en format lisible */
  XZEXT_Msg_TEDI	vl_MsgTEDI;	/* stokage message TEDI a envoyer */
  XZEXT_Comp_Trame_TEDI vl_SkelTEDI;	/* composants trame TEDI pour EX_encapsuler_LCR */

  XZST_03EcritureTrace( XZSTC_INFO, "elei : Demande LCR :\n <%s>\n", pa_MsgSock->msg );

  /*A
   * traitement message en fonction du type de la socket active
   */
  /*A CAS type socket active */
  switch( edir_detecte_type_sock() )
  {
    /*A cas XZEXC_SOCK_ACT_DONN */
    case XZEXC_SOCK_ACT_DONN :	
      /*B detection type message recu d'un client TEzzz sur une socket donnees */

      /*B SI demande d'arret ALORS appel ecfg_sortir() */
      if( strstr( pa_MsgSock->msg, XZEXC_DEMANDE_ARRET ) != NULL )
      {
        XZST_03EcritureTrace( XZSTC_INFO, "edir_traitement_msg_sock : demande arret TELEi ligne serie %d",
            vg_Config.no_ligne );
        ecfg_sortir();
      }
      /*B SINON SI demande init. ALORS appel ecom_init() */
      else if( strstr( pa_MsgSock->msg, XZEXC_DEMANDE_INIT ) != NULL )
      {
        XZST_03EcritureTrace( XZSTC_INFO, "edir_traitement_msg_sock : demande initialisation ligne serie TELEi %d",
            vg_Config.no_ligne );
        if ( (vg_EtatST = ecom_init( &vg_Config, &vg_SockList, &vg_DesSocks )) != XDC_OK )
          XZST_03EcritureTrace( XZSTC_WARNING, "edir_traitement_msg_sock : relance init tele%d impossible", vg_Config.no_ligne );
        else
          XZST_03EcritureTrace( XZSTC_WARNING, "edir_traitement_msg_sock : relance init tele%d", vg_Config.no_ligne );

      }
      /* SINON SI demande de lcr tache traitement alors que l'on attendait */
      else if( vg_RepTEzzz == XDC_NOK && strstr( pa_MsgSock->msg, XZEXC_DEMANDE_LCR ) != NULL  )
      {

        /*A SI depouillement message socket echoue */
        if( sscanf( &(pa_MsgSock->msg[strlen(XZEXC_DEMANDE_LCR)]), "%d", &vl_TypeTrame ) != 1 )
        {
          /*A ALORS trace " Demande LCR incorrecte <message> */
          XZST_03EcritureTrace( XZSTC_WARNING, "elei : Demande LCR incorrecte :\n <%s>\n", pa_MsgSock->msg );
        }

        XZST_03EcritureTrace( XZSTC_INFO, "elei : Demande LCR2 :\n <%s>\n,%d", pa_MsgSock->msg,vl_TypeTrame );
        /*A CAS type message recu */
        switch( vl_TypeTrame )
        {
          /* cas XZEXC_TRAME_TELEI_NREP = ne pas envoyer de reponse */
          case XZEXC_TRAME_TELEI_NREP :
            /*B RIEN */
            break;

            /* cas XZEXC_TRAME_TELEI_ACK ou XZEXC_TRAME_TELEI_NAK = a envoyer l'acquitt. indiquee */
          default :
            /*B encapsulation et emission acquittement sur la ligne serie */
            sprintf( vl_SkelTEDI.adr_RGSB, "%c%d",
                vl_TypeTrame == XZEXC_TRAME_TELEI_ACK ? XZEXC_CAR_ACK : XZEXC_CAR_NAK,
                    XZEXC_NOBL_REQ_TEDI );
            vl_SkelTEDI.type_msg = XZEXC_MSG_ACQUIT;
            vl_SkelTEDI.long_pref = vg_Config.ligne_serie.NbCarPref;
            vl_SkelTEDI.long_suff = vg_Config.ligne_serie.NbCarSuff;

            /*B SI encapsulation echoue */
            if( EX_encapsuler_LCR( &vl_SkelTEDI, &vl_MsgTEDI ) != XDC_OK )
            {
              /*B ALORS trace Probleme encapsulation TEDI msg. <msg> */
              if( (pl_TrameLisible = malloc( 4*strlen( vl_SkelTEDI.adr_RGSB ) + 1 )) )
              {
                ecom_rendre_trame_lisible( vl_SkelTEDI.adr_RGSB, 2, pl_TrameLisible );
              }
              XZST_03EcritureTrace( XZSTC_WARNING, "epro_traitement_msg_equip_maitre : Probleme encapsulation TEDI msg. <%s>",
                  pl_TrameLisible );
              free( pl_TrameLisible );
            }
            /*B SI emission acquittement sur socket ligne serie echoue */
            sem_wait(&vg_semaphore); 
            if( write( vg_DesSocks.lser, vl_MsgTEDI.trame_TEDI, vl_MsgTEDI.len ) != vl_MsgTEDI.len )
            {
              /*B ALORS ecriture trace : "Emission message TEDI/LCR vers serv. terms. impossible" */
              if( (pl_TrameLisible = malloc( (4 * (size_t) vl_MsgTEDI.len) + 1 )) )
              {
                ecom_rendre_trame_lisible(vl_MsgTEDI.trame_TEDI, vl_MsgTEDI.len, pl_TrameLisible  );
              }
              XZST_03EcritureTrace( XZSTC_WARNING, " Emission message TEDI/LCR <%s> vers serv. terms. impossible ",
                  pl_TrameLisible );
              free( pl_TrameLisible );
            }
            sem_post(&vg_semaphore); 
            /*B FIN SI */
            break;
        }
        /*A FIN CAS type trame */

        /* mise flag reponse tache traitement a "reponse recue" */
        vg_RepTEzzz = XDC_OK;
      }
      /*B SINON SI demande inconnue ( diff. de ARRET ou INIT )
       *  ALORS ecriture trace : " Recu demande TEzzz inconnue : <demande_TEzzz>" */
      else
      {
        XZST_03EcritureTrace( XZSTC_INFO, "edir_traitement_msg_sock : Recu demande TEzzz inconnue : <%s>", pa_MsgSock->msg );
      }
      break;

      /*A cas XZEXC_SOCK_ACT_LSER : message equipement */
    case XZEXC_SOCK_ACT_LSER :
      /*B SI on attend reponse tache traiteemnt */
      if( vg_RepTEzzz == XDC_NOK )
      {
        /*B trace "elei : msg. lser <msg recu> avant reponse tache traitmnt au msg precedent !" */
        if( (pl_TrameLisible = malloc( 4*strlen( pa_MsgSock->msg ) + 1 )) )
        {
          ecom_rendre_trame_lisible( pa_MsgSock->msg, pa_MsgSock->len, pl_TrameLisible );
        }
        XZST_03EcritureTrace( XZSTC_WARNING, "elei : msg. lser <%s>\navant reponse tache traitmnt au msg precedent !",
            pl_TrameLisible );
        free( pl_TrameLisible );
        if ( strncmp ( vm_MsgEqt, pa_MsgSock->msg, pa_MsgSock->len ) )
          epro_traitement_msg_equip_maitre( pa_MsgSock );
      }
      /* SINON */
      else
      {
        epro_traitement_msg_equip_maitre( pa_MsgSock );
      }

      strcpy ( vm_MsgEqt, pa_MsgSock->msg );

      break;

      /*A defaut ( XZEXC_SOCK_ACT_UNDEF ) */
    default :	
      break;
  }
  /*A FIN CAS */

}



/*X*/
/*------------------------------------------------------
 * SERVICE RENDU :
 * Traitement du message receptionne par elei_dir sur la socket active.
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
 *  XZEXC_SOCK_ACT_DONN	: si evnmt. sur une socket de comms. avec un client
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
  static char *version = "$Id: elei_dir.c,v 1.17 2019/06/04 13:40:22 devgtie Exp $ : edir_detecte_type_sock" ;

  /*A
   * detection type socket active
   */
  /*A SI socket RTServer active ALORS RETURN XZEXC_SOCK_ACT_RTSRV */
  if( vg_DesSocks.curr == vg_DesSocks.rtsrv )
  {
    return( XZEXC_SOCK_ACT_RTSRV );
  }
  /*A SI socket ligne serie active ALORS return XZEXC_SOCK_ACT_LSER */
  if( vg_DesSocks.curr == vg_DesSocks.lser )
  {
    return( XZEXC_SOCK_ACT_LSER );
  }
  /*A SI socket ecoute => connexion client => ALORS return XZEXC_SOCK_ACT_UNDEF */
  if( vg_DesSocks.curr == vg_DesSocks.ect )
  {
    return( XZEXC_SOCK_ACT_UNDEF );
  }
  /*A SINON => socket donnees (client TEzzz) active : return XZEXC_SOCK_ACT_DONN */
  return( XZEXC_SOCK_ACT_DONN );
}



/*X*/
/*------------------------------------------------------
 * SERVICE RENDU :
 * Traitement d'un evenement deconnexion d'un client TEzzz.
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
 * Warning pour deconnexion RTServer ou Serveur de terminaux !
 *
------------------------------------------------------*/
void edir_traitement_dcnx( void )
{
  static char *version = "$Id: elei_dir.c,v 1.17 2019/06/04 13:40:22 devgtie Exp $ : edir_traitement_dcnx" ;

  XDY_Horodate	vl_ValHorodate;	/* stokage horodate valeur non convertie */
  int			vl_JourSem;	/* jour de la semaine */
  XDY_Texte		vl_Msg;		/* texte traces */

  /*A ecriture trace : Deconnexion socket adresse <vg_DesSocks.curr_addr>...*/
  XZST_03EcritureTrace( XZSTC_INFO, "edir_traitement_dcnx : Deconnexion socket adr. %s...", vg_DesSocks.curr_addr );

  /*A SI deconnexion socket serveur terminaux */
  if( vg_DesSocks.curr == vg_DesSocks.lser )
    vg_EtatST = XDC_NOK;

  /*A SI deconnexion socket RTWorks */
  if( vg_DesSocks.curr == vg_DesSocks.rtsrv )
  {
    /*B ALORS ecriture trace "Deconexion socket comms. avec RTServer"*/
    XZST_03EcritureTrace( XZSTC_INFO,
        "edir_traitement_dcnx : Deconexion socket comms. avec RTServer !" );
  }

  /*A Si socket TEzzz */
  if ( (vg_DesSocks.curr != vg_DesSocks.rtsrv) &&
      (vg_DesSocks.curr != vg_DesSocks.lser) &&
      (vg_DesSocks.curr != vg_DesSocks.ect) )
  {
    ecfg_sortir ( );
  }


}

