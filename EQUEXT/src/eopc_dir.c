/*Fichier : $Id: eopc_dir.c,v 1.1 2021/05/04 13:46:53 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2021/05/04 13:46:53 $
------------------------------------------------------
 * GTIE *  PROJET MIGRAZUR
------------------------------------------------------
 * SOUS-SYSTEME : EQUEXT
------------------------------------------------------
 * MODULE TELMi * FICHIER eopc_dir.c
------------------------------------------------------
 * DESCRIPTION DU MODULE :
 *
 * Programme principale de la tache TEOPC
 *
------------------------------------------------------
 * HISTORIQUE :
 *
 * ABE	18/02/21	Creation DEM-SAE155 1.1
 *
------------------------------------------------------*/

/* fichiers inclus */

#include <xzsem.h>
//#include "ex_mopc.h"
#include "eopc_dir.h"
#include "eimu_don.h"

/* definitions de constantes */								
#define C_CERTIFICATE_SUFFIXE_NAME "_client.der"
#define C_KEY_SUFFIXE_NAME "_client_key.pem"
/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */
XZEXT_IP_Config vm_config;
T_Tampon tg_tampons[C_MAX_TAMPON];
int  vg_presence_esclave;
int vg_current_client_socket;
/* declaration de fonctions externes */

/* definitions fonctions internes */
int edir_detecte_type_sock( void );

/* definition de fonctions internes */

void edir_traitement_msg_sock( XZEXT_Msg_Socket * );
void edir_traitement_dcnx( void );
void edir_traitement_liste_cmdes_LCR( void );
void edir_reveil_cb(T_IPC_CONN, T_IPC_CONN_PROCESS_CB_DATA, T_CB_ARG);
int edir_retourne_OPCcontext(int *indice,XDY_Eqt va_NumBICE);
void OPCdatachanged(void *NumBICE, char *nodeId, char *value);

/*void edir_traitement_liaison_ST( void );*/


/*X*/
/* Fichier : $Id: eopc_dir.c,v 1.1 2021/05/04 13:46:53 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2021/05/04 13:46:53 $
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
  XZEXT_Msg_Socket vl_MsgSock;		/* struct. creation msg. socket */
  XZEXT_MASK *pl_Mask;
  XDY_Texte vl_Texte;
  XZEXT_ADR_SOCK pl_SrvTeimi;
  XDY_Datagroup pl_dg_timer;
  XZEXT_MASK *pl_ListSock = NULL;
  XZSMT_CleProg vl_cle;
  XDY_NomSite pl_NomSite = "";
  int vl_indice;
  int vl_SockRet 	= 0;	
  int vl_port_serveur=0;							
  int vl_SocketRT = XZEXC_SOCKET_NULL;
  int i=0;


  /*
   * sequence initialisation tache TEOPC
   */

  /* init a zero des descs. sockets specifiques taches TELxi */
  vg_DesSocks.curr = XZEXC_SOCKET_NULL;
  vg_DesSocks.ect = XZEXC_SOCKET_NULL;
  vg_DesSocks.lser = XZEXC_SOCKET_NULL;
  vg_DesSocks.rtsrv = XZEXC_SOCKET_NULL;
  pg_ListeCmdesLCR = NULL;
  vg_presence_esclave  = XDC_FAUX;
  vg_ConfigE.No_port = -1;

  for  (vl_indice=0;vl_indice<C_MAX_TAMPON;vl_indice++) 
  {
  	strcpy(tg_tampons[vl_indice].adresse,"");
  	tg_tampons[vl_indice].port=-1;
  	strcpy(tg_tampons[vl_indice].tampon,"");
  }

  /* Recuperation nom machine */
  strcpy( vg_Config.Nom_machine, argv[2] );

	/*A
	*  initialisation semaphore
	*/
	sem_init(&vg_semaphore, 0, 1);

  /* Recuperation no. port pour le scoket serveur */
  vl_port_serveur = atoi( argv[3] );

  /* SI Connexion au RTserver et initialisation des variables echoue */
  if(XZSC_01InitCnxSup(NULL,
                       argv[0],
                       XZSCC_INTERDIRE,
                       XZSCC_AUTORISER,
                       XZSCC_INTERDIRE,
                       ecfg_sortir,
                       "/dev/null" ) != XDC_OK  )
  {
    XZST_03EcritureTrace( XZSTC_FATAL, "Ouverture connexion avec le RTserver impossible!!!" );
    exit( XDC_NOK );
  }

  XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_WARNING|XZSTC_INFO|XZSTC_FONCTION|XZSTC_INTERFACE,XZSTC_NECRAN);
  /*XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_WARNING|XZSTC_INFO|XZSTC_FONCTION|XZSTC_DEBUG1|XZSTC_DEBUG2|XZSTC_DEBUG3|XZSTC_INTERFACE,XZSTC_NECRAN);*/
																				
  XZST_03EcritureTrace(XZSTC_WARNING, "Ouverture cnx. RTServer reussie ..." );


  /* Creation du repertoire pour les sockets */
  ex_rep_socket ( vg_Config.Nom_machine );
												

  XZSM_00InitServices ();

/*  sprintf(vl_cle, "TEOPC_%d_%s",vg_Config.no_ligne, vg_Config.Nom_machine);
  sprintf(pl_dg_timer,"%s_%d_%s",XDG_ETEOPC, vg_Config.no_ligne, vg_Config.Nom_machine);
  XZSM_02AnnulerProgrammation(vl_cle, pl_dg_timer,1);*/
												
/*  if (TipcSrvProcessCbCreate(XDM_IdentMsg (XDM_ANEUTR_TIMER), //aneutr mise au neutre des PMV/PMVA ?
      (T_IPC_CONN_PROCESS_CB_FUNC)edir_reveil_cb,
      (T_CB_ARG) NULL)==NULL) {
    XZST_03EcritureTrace(XZSTC_WARNING, "pb appel TipcSrvProcessCbCreate sur XDM_ANEUTR_TIMER");
  }
  sprintf(pl_dg_timer,"%s_%d_%s",XDG_ETEOPC, vg_Config.no_ligne, vg_Config.Nom_machine);
  XDG_JoindreDG(pl_dg_timer);*/

  /*
   * connexion soket en mode serveur pour comms. avec la tache traitement
   */
  /* SI connexion echoue */
  sprintf ( pl_SrvTeimi, "%s%d", XZEXC_ADR_SOCK_TEIM, vg_Config.no_ligne );
  if( ex_cnx_serveur_IPV4( vl_port_serveur, &vg_SockList, &vg_DesSocks.ect ) != XDC_OK )
  {
    /* ALORS */
    /* ecriture trace : " Connexion socket ecoute mode serveur impossible !" */
    XZST_03EcritureTrace( XZSTC_FATAL, " Connexion socket ecoute mode serveur impossible !" );
    /* sortie XDC_NOK */
    exit( XDC_NOK );
  }else
XZST_03EcritureTrace(XZSTC_WARNING, "port=%d socket=%d",vl_port_serveur,vg_DesSocks.ect);

  for (pl_ListSock = vg_SockList ;  pl_ListSock != NULL; pl_ListSock = pl_ListSock->Suivant )
  {
    if (pl_ListSock->desc_sock==vg_DesSocks.ect)
    {
      pl_ListSock->Protocole = XZECC_PROT_SERVEUR;
      break;
    }
  }

  XZST_03EcritureTrace( XZSTC_FONCTION, " Nom machine %s, type eqt %d ",
      vg_Config.Nom_machine, vg_Config.no_ligne );

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
  XZST_03EcritureTrace(XZSTC_WARNING, "Debut boucle traitement message socket");
  while( 1 )
  {

    /* process events for each context*/
    for (i=0; i< vg_nb_connexion; i++)
    {	
        XZST_03EcritureTrace(XZSTC_WARNING, "EopcWork i=%d, endpoint:%s BICE:%d state:%d",i,vg_list_EopcContext[i].Context.ServerConfiguration.Endpoint,
                                            vg_list_EopcContext[i].NumBICE,vg_list_EopcContext[i].Context.State);
        XZST_03EcritureTrace(XZSTC_WARNING, "Callback =%d",vg_list_EopcContext[i].Context.ServerConfiguration.DataChangedCallBack==NULL?0:1);
        Eopc_Work(&vg_list_EopcContext[i].Context);
    }

    /* traitement des eventuelles cmdes. LCR empilees ds. la liste de cmdes. non traitees */
    /*edir_traitement_liste_cmdes_LCR();*/								

    /* SI attente message sur liste sockets : ecoute, lserie, donnees client echoue */
    if( ex_attendre_msg( vg_DesSocks.ect, &vg_SockList, &vg_DesSocks.curr ) != XDC_OK )
    {
      /* ALORS ecriture trace :  " Probleme attente message sur socket(s)." */
      XZST_03EcritureTrace( XZSTC_WARNING, " Probleme attente message sur socket(s)." );	
      ex_ConnexionSocketRTserver ( &vg_DesSocks.rtsrv, vg_DesSocks.curr, &vg_SockList );
    }
    else
    {
      XZST_03EcritureTrace( XZSTC_WARNING, "current sock :%d",vg_DesSocks.curr);
      ex_ConnexionSocketRTserver ( &vg_DesSocks.rtsrv, 0, &vg_SockList );
      XZST_03EcritureTrace( XZSTC_WARNING, "rtsrvsock :%d",vg_DesSocks.rtsrv);

      /* SI socket active = socket RTServer */
      if( vg_DesSocks.curr == vg_DesSocks.rtsrv )
      {
        XZST_03EcritureTrace(XZSTC_FONCTION, "eopc_dir.x : appel primitive standard lecture RTWorks" );

        /* ALORS appel primitive standard lecture RTWorks */
      }
      /* SINON SI evenement sur une socket diff. de la socket d'ecoute
       *  (si. evt. sur socket ecoute <=> cnx. d'un client geree par ex_attendre_msg())
       *  tout est O.K. => lecture sur socket active */
      else if( vg_DesSocks.curr != XZEXC_NO_EVT 	/* une socket active */
          && vg_DesSocks.curr != vg_DesSocks.ect) /* cas connexion client */
      {
        XZST_03EcritureTrace( XZSTC_FONCTION, "eopc_dir.x : evenement sur une socket diff. de la socket d'ecoute...\n\n" );

        /* detection adresse socket active */
        ecom_detecte_clnt( &vg_DesSocks, &vg_SockList );

        XZST_03EcritureTrace( XZSTC_FONCTION, "eopc_dir.x : lecture sur socket %d active\n",
            vg_DesSocks.curr );

        /* SI lecture sur socket active echoue */
        if( ex_lire_msg( vg_DesSocks.curr, vl_MsgSock.msg, &vl_MsgSock.len,
            &vg_SockList, &vl_EtatCnx ) != XDC_OK )
        {
          /* ALORS ecriture trace :  Probleme lecture socket adresse <vg_DesSocks.curr_addr> */
          XZST_03EcritureTrace( XZSTC_WARNING, "eopc_dir.x : Probleme lecture socket adresse %s...",
              vg_DesSocks.curr_addr );
        }

        /* SINON */
        else
        {
      XZST_03EcritureTrace( XZSTC_WARNING, " 3");
          /* CAS etat connexion socket active */
          switch( vl_EtatCnx )
          {
            /* cas : XZEXC_DECONNEC */
            case XZEXC_DECONNEC:

              XZST_03EcritureTrace( XZSTC_WARNING, "Deconnexion <%s>...\n", vg_DesSocks.curr_addr );

              /* appel fonction traitement dcnxion : edir_traitement_dcnx() */
              edir_traitement_dcnx();
              break;

              /* cas : XZEXC_CONNEC */
            case XZEXC_CONNEC :

              XZST_03EcritureTrace( XZSTC_FONCTION, "Connexion <%s>...\n", vg_DesSocks.curr_addr );

              /* appel fonction trait. mess. recu edir_traitement_msg_sock( &vl_MsgSock )*/
              edir_traitement_msg_sock( &vl_MsgSock );
              break;

              /* cas par defaut */
            default :
              /* ecriture trace : Etat connexion socket non defini..." */
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
void edir_traitement_msg_sock( XZEXT_Msg_Socket *pa_MsgSock )
{
  XZEXT_Cmde_OPC	vl_CmdeOPC;	/* structure depouillement message recu sur socket */
  XDY_Texte		vl_tag;
  XDY_Texte		vl_value;
  XDY_Texte		vl_Texte;
  XDY_Texte		vl_Certificate;
  XDY_Texte		vl_Key;
  int vl_ValRet;
  /* buffer stokage trame lisible */
  char 		*pl_TrameLisible;
  XDY_AdresseIP	vl_adresseIP;
  XDY_PortIP		vl_port;
  char vl_protocole;
  XDY_Entier vl_NumBice;
  XDY_Texte vl_tagOPC;
  EopcContext vl_Context;
  XZEXT_Liste_Cmdes_LCR_IP *vl_liste=NULL;
  int vl_TypeTrame;
  XDY_Horodate vl_horodate;
  int vl_js;
  int vl_indice;
  int index;
  list_EopcContext vl_EopcContext;
  int i=0;
								
  /*
   * traitement message en fonction du type de la socket active
   */
  /* CAS type socket active */
  XZSM_07LireHorodate( XZSMC_TYPE_SYSTEME , &vl_js , &vl_horodate );

  switch( edir_detecte_type_sock() )
  {
    case XZEXC_SOCK_ACT_DONN :	/* cas XZEXC_SOCK_ACT_DONN */
      /* detection type message recu d'un client TEzzz sur une socket donnees */

      /* SI demande d'arret ALORS appel ecfg_sortir() */
      if( strstr( pa_MsgSock->msg, XZEXC_DEMANDE_ARRET ) != NULL )
      {
        XZST_03EcritureTrace( XZSTC_WARNING, "edir_traitement_msg_sock : demande arret TEOPi %d",
            vg_Config.no_ligne );
	for (i=0; i< vg_nb_connexion; i++)
	{	
		Eopc_Stop(&vg_list_EopcContext[i].Context);
		Eopc_ResetContext(&vg_list_EopcContext[i].Context);
	}
		/*A liberation memoire occuppee par la liste de sockets 
	         *appel ecom_detruire_liste_sock() */
	ecom_detruire_liste_sock( &vg_SockList );
      }								
      else if (strstr(pa_MsgSock->msg, XZEXC_CONFIG_IP) != NULL) //Config socket Teopc-Eqt
      {
										
	/* créer l'object contexte associé à l'IMU*/
	Eopc_InitContext(&vl_EopcContext.Context);	

	strcpy(vl_Certificate,XDC_PATHEXE);
	strcat(vl_Certificate,"/");
	strcat(vl_Certificate,vg_Config.Nom_machine);
	strcat(vl_Certificate,C_CERTIFICATE_SUFFIXE_NAME);

	strcpy(vl_Key,XDC_PATHEXE);
	strcat(vl_Key,"/");
	strcat(vl_Key,vg_Config.Nom_machine);
	strcat(vl_Key,C_KEY_SUFFIXE_NAME);
	Eopc_SetClientCertificate(&vl_EopcContext.Context,vl_Certificate, vl_Key); //Set client certificate utile ?
												
        if (sscanf(&(pa_MsgSock->msg[strlen(XZEXC_CONFIG_IP)]), "%d %s %s %s %d %s %c",
            &vl_EopcContext.NumBICE,	
	    vl_EopcContext.EndPointOPCUA,
	    vl_EopcContext.UserOPCUA,
	    vl_EopcContext.PasswordOPCUA,
	    &vl_EopcContext.SousType,
	    vl_EopcContext.NomMachine,
	    &vl_protocole) != 7) 
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "edir_traitement_msg_sock config ip incorrecte %s", pa_MsgSock->msg);								
		return;
	}

        XZST_03EcritureTrace( XZSTC_FONCTION,"edir_traitement_msg_sock : Context : %d config ip NumBice:%d EnPointOPCUA:%s UserOPCUA:%s PasswordOPCUA:%s SousType:%d NomMachine:%s protocole:%c",
	    vg_nb_connexion,
	    vl_EopcContext.NumBICE,
	    vl_EopcContext.EndPointOPCUA,
	    vl_EopcContext.UserOPCUA,
	    vl_EopcContext.PasswordOPCUA,
	    vl_EopcContext.SousType,
	    vl_EopcContext.NomMachine,
	    vl_protocole
		);					

	/* Connexion à l'équipement */															
	Eopc_SetConfig(&vl_EopcContext.Context,
			vl_EopcContext.EndPointOPCUA, 
			vl_EopcContext.UserOPCUA, 
			vl_EopcContext.PasswordOPCUA);


	//Attente connexion
	for  (vl_indice=0;vl_indice<20;vl_indice++) {
		sleep(0.1);
		printf("eopcWork\n");
		Eopc_Work(&vl_EopcContext.Context);
		if (vl_EopcContext.Context.State==State_Connected)
		{								
			XZST_03EcritureTrace( XZSTC_FONCTION,"edir_traitement_msg_sock : config ip Connexion success BICE no %d endpoind:%s sate:%d",
			vl_EopcContext.NumBICE,
			vl_EopcContext.Context.ServerConfiguration.Endpoint,
			vl_EopcContext.Context.State);
			break;								
		}
       	}

	if (vl_EopcContext.Context.State==State_Connected)
	{	
		/* Link cb EOPC */
		//Eopc_SetDataChangeCallBack(&vl_EopcContext.Context, &vl_EopcContext.NumBICE, OPCdatachanged);

		strcpy(vg_Config.adresseIP,vl_adresseIP);
		vg_Config.No_port = vl_port;
		vg_Config.protocole = vl_protocole;
		if (vl_protocole == XZECC_PROT_LCR_E) {
		  vg_presence_esclave = XDC_VRAI;
		  vm_config = vg_Config;
		  vg_ConfigE = vg_Config;
		}


		/* Retourn le context en lien avec le numéro du BICE de la trame */
		//edir_retourne_OPCcontext(&vl_Context,vg_list_EopcContext[vg_nb_connexion].NumBICE);

		/*Eopc_Listen(&vg_list_EopcContext[vg_nb_connexion].Context, "[SeeBiceTags]ADD");
		Eopc_Listen(&vg_list_EopcContext[vg_nb_connexion].Context, "[SeeBiceTags]START");
		Eopc_Listen(&vg_list_EopcContext[vg_nb_connexion].Context, "[SeeBiceTags]RESET");
		Eopc_Listen(&vg_list_EopcContext[vg_nb_connexion].Context, "[SeeBiceTags]KILL");*/
		/*Eopc_Work(&vg_list_EopcContext[vg_nb_connexion].Context);
		sleep (20);*/
								
	/*	Eopc_Listen(&vl_Context, "[SeeBiceTags]ADD");
		Eopc_Listen(&vl_Context, "[SeeBiceTags]START");
		Eopc_Listen(&vl_Context, "[SeeBiceTags]RESET");
		Eopc_Listen(&vl_Context, "[SeeBiceTags]KILL");*/

		/*if (vl_protocole == XZECC_PROT_LCR) {
			for  (vl_indice=0;vl_indice<C_MAX_TAMPON;vl_indice++) {
				if (tg_tampons[vl_indice].port==-1)
					break;
			}
			strcpy(tg_tampons[vl_indice].adresse,vl_adresseIP);
			tg_tampons[vl_indice].port=vl_port;
			strcpy(tg_tampons[vl_indice].tampon,"");
		}*/

		//TOD
		/*if (ecom_init_IP(&vg_Config, &vg_SockList, &vg_DesSocks)) {
		  XZST_03EcritureTrace( XZSTC_WARNING, "pb connexion %s %d",
		      vl_adresseIP,vl_port);
		  return;
		}*/
	
		//Incrémentation du nombre de connexion en cours
		/*if (vg_list_EopcContext==NULL) //Creation
			vg_list_EopcContext = (list_EopcContext*) malloc((vg_nb_connexion+1)*sizeof(list_EopcContext));
		else //Ajoute une case
			vg_list_EopcContext = (list_EopcContext*) realloc(vg_list_EopcContext,(vg_nb_connexion+1)*sizeof(list_EopcContext));*/

		vg_list_EopcContext[vg_nb_connexion].NumBICE=vl_EopcContext.NumBICE;
		strcpy(vg_list_EopcContext[vg_nb_connexion].EndPointOPCUA,vl_EopcContext.EndPointOPCUA);
		strcpy(vg_list_EopcContext[vg_nb_connexion].UserOPCUA,vl_EopcContext.UserOPCUA);
		strcpy(vg_list_EopcContext[vg_nb_connexion].PasswordOPCUA,vl_EopcContext.PasswordOPCUA);
		vg_list_EopcContext[vg_nb_connexion].SousType=vl_EopcContext.SousType;
		strcpy(vg_list_EopcContext[vg_nb_connexion].NomMachine,vl_EopcContext.NomMachine);
		vg_list_EopcContext[vg_nb_connexion].Context=vl_EopcContext.Context;
    XZST_03EcritureTrace( XZSTC_FONCTION,"Context num:%d Callback =%d",vg_nb_connexion,vg_list_EopcContext[vg_nb_connexion].Context.ServerConfiguration.DataChangedCallBack==NULL?0:1);
    Eopc_SetDataChangeCallBack(&vg_list_EopcContext[vg_nb_connexion].Context, &vg_list_EopcContext[vg_nb_connexion].NumBICE, OPCdatachanged);
		
    vg_nb_connexion++;
	}
	else
	{
		XZST_03EcritureTrace( XZSTC_WARNING,"edir_traitement_msg_sock : config ip Connexion failed BICE no %d endpoind:%s ",vl_EopcContext.NumBICE,
							vl_EopcContext.Context.ServerConfiguration.Endpoint);
	}

      }
												
      else if(strstr(pa_MsgSock->msg, XZEXC_DEMANDE_OPC) != NULL) //Si demande OPC
      {
		/* Décode la trame et rempli la structure XZEXT_Cmde_OPC */
		if(ecom_extraire_cmd_OPC(pa_MsgSock->msg, &vl_CmdeOPC, vg_DesSocks.curr))
		{
			XZST_03EcritureTrace( XZSTC_WARNING, "edir_traitement_msg_sock : Erreur extraire cmd." );
			return;
		}
		vg_current_client_socket=vg_DesSocks.curr;

		/* Retourn le context en lien avec le numéro du BICE de la trame */
		if(edir_retourne_OPCcontext(&index, atoi(vl_CmdeOPC.idBICE)))
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"edir_traitement_msg_sock : Erreur, BICE non trouvee dans la liste");
			return;								
		}
			XZST_03EcritureTrace( XZSTC_FONCTION,"edir_traitement_msg_sock : LISTE IdOPC=%d index:%d",vg_list_EopcContext[index].NumBICE,index);

		XZST_03EcritureTrace( XZSTC_FONCTION,"edir_traitement_msg_sock :Commande OPC : socket:%d typemsg:%d idBICE:%s Nocmd:%d Prior:%d texte:%s",vg_DesSocks.curr,
								(vl_CmdeOPC.type_msg),(vl_CmdeOPC.idBICE),(vl_CmdeOPC.no_cmd),(vl_CmdeOPC.prior),vl_CmdeOPC.cmd);

		if (vl_CmdeOPC.type_msg==XZEXC_TRAME_ABONNEMENT_TAG)					
		{												
			/* Abonnement au tag OPC */
			//Eopc_Listen(&vl_Context,vl_CmdeOPC.cmd);
			Eopc_Listen(&vg_list_EopcContext[index].Context,vl_CmdeOPC.cmd);
			XZST_03EcritureTrace( XZSTC_FONCTION,"edir_traitement_msg_sock : Abonnement TAG:%s IdOPC=%s",vl_CmdeOPC.cmd,vl_CmdeOPC.idBICE);
			Eopc_Work(&vg_list_EopcContext[index].Context);
		}
		else if (vl_CmdeOPC.type_msg==XZEXC_TRAME_CMD_IMU_INT)
		{
			/* Séparation du tag et de la valeur */
			sscanf(vl_CmdeOPC.cmd, "%s %s",vl_tag,vl_value);
			
			/* Envoie de la commande OPC */
			Eopc_Write_Int32(&vg_list_EopcContext[index].Context,vl_tag,vl_value);
			Eopc_Work(&vg_list_EopcContext[index].Context);

			XZST_03EcritureTrace( XZSTC_FONCTION,"edir_traitement_msg_sock : Diffusion TAG:%s Value=%s IdOPC=%s",vl_tag,vl_value,vl_CmdeOPC.idBICE);
		}
		else if (vl_CmdeOPC.type_msg==XZEXC_TRAME_CMD_IMU_STRING)
		{
			/* Séparation du tag et de la valeur */
			sscanf(vl_CmdeOPC.cmd, "%s %s",vl_tag,vl_value);
			
			/* Envoie de la commande OPC */
			Eopc_Write_String(&vg_list_EopcContext[index].Context,vl_tag,vl_value);
			Eopc_Work(&vg_list_EopcContext[index].Context);

			XZST_03EcritureTrace( XZSTC_FONCTION,"edir_traitement_msg_sock : Diffusion TAG:%s Value=%s IdOPC=%s",vl_tag,vl_value,vl_CmdeOPC.idBICE);
		}		
      }
      /* SINON SI demande inconnue ( diff. de ARRET, INIT ou LCR )
       *  ALORS ecriture trace : " Recu demande TEzzz inconnue : <demande_TEzzz>" */
      else
      {
        XZST_03EcritureTrace( XZSTC_WARNING, " Recu demande TEzzz inconnue : %s", pa_MsgSock->msg );
      }
      break;

    case XZEXC_SOCK_ACT_LSER :	/* cas XZEXC_SOCK_ACT_LSER */
    case XZEXC_SOCK_ACT_IP_LCR:
      /* Normallement reception reponses equps. ds. epro_envoi_trame_LCR().
       *  si socket ligne serie active <=> reception reponse arrivee en retard.
       *  => ecriture trace : " Reponse TEDI/LCR perdue <trame_TEDI/LCR> "
       */
      //epro_traitement_msg_ip_lcr_e(pa_MsgSock, XDC_FAUX);
      break;

    case XZEXC_SOCK_ACT_IP_LCR_E:
      /* message equipement*/
      //epro_traitement_msg_ip_lcr_e(pa_MsgSock, XDC_VRAI);
      break;

    case XZEXC_SOCK_ACT_IP_DIASER :
      /*decodage trame DIASER*/
      /*et envoi des infos a TExx*/
      //epro_traitement_msg_ip_diaser(pa_MsgSock);
      XZST_03EcritureTrace( XZSTC_WARNING, "apres epro_traitement_msg_ip_diaser");
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
  XZEXT_MASK          *pl_ListSock = NULL;

  static char *version = "$Id: eopc_dir.c,v 1.1 2021/05/04 13:46:53 pc2dpdy Exp $ : edir_detecte_type_sock" ;
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

  for (pl_ListSock = vg_SockList ;  pl_ListSock != NULL; pl_ListSock = pl_ListSock->Suivant )
  {
    XZST_03EcritureTrace( XZSTC_FONCTION," socket %d %s '%c'", pl_ListSock->desc_sock, pl_ListSock->adresse,
        pl_ListSock->Protocole);
  }

  for (pl_ListSock = vg_SockList ;  pl_ListSock != NULL; pl_ListSock = pl_ListSock->Suivant )
  {
    XZST_03EcritureTrace( XZSTC_FONCTION," socket %d %s '%c'", pl_ListSock->desc_sock, pl_ListSock->adresse,
        pl_ListSock->Protocole);
    if ((pl_ListSock->Protocole != XZECC_PROT_DIASER)
        && (pl_ListSock->Protocole != XZECC_PROT_LCR)
        && (pl_ListSock->Protocole != XZECC_PROT_LCR_E)
        && (pl_ListSock->Protocole != XZECC_PROT_MIGRAZUR)
        && (pl_ListSock->Protocole != XZECC_PROT_SERVEUR))
      pl_ListSock->Protocole =  XZECC_PROT_MIGRAZUR;

    if (pl_ListSock->desc_sock==vg_DesSocks.curr)
    {
      if (pl_ListSock->Protocole == XZECC_PROT_DIASER) {
        XZST_03EcritureTrace( XZSTC_FONCTION,"XZECC_PROT_DIASER");
        return (XZEXC_SOCK_ACT_IP_DIASER);
      }
      else if (pl_ListSock->Protocole == XZECC_PROT_LCR) {
        XZST_03EcritureTrace( XZSTC_FONCTION,"XZECC_PROT_LCR");
        return  (XZEXC_SOCK_ACT_IP_LCR);
      }
      else if (pl_ListSock->Protocole == XZECC_PROT_LCR_E) {
        XZST_03EcritureTrace( XZSTC_FONCTION,"XZECC_PROT_LCR_E");
        return (XZEXC_SOCK_ACT_IP_LCR_E);
      }
      else if (pl_ListSock->Protocole == XZECC_PROT_MIGRAZUR) {
        XZST_03EcritureTrace( XZSTC_FONCTION,"XZECC_PROT_MIGRAZUR");
        return (XZEXC_SOCK_ACT_DONN); /* TExxx*/
      }
    }
  }
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
  static char *version = "$Id: eopc_dir.c,v 1.1 2021/05/04 13:46:53 pc2dpdy Exp $ : edir_traitement_dcnx" ;

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
    /* ALORS ecriture trace "Deconexion socket comms. avec RTServer"*/
    XZST_03EcritureTrace( XZSTC_WARNING,
        "Deconexion socket comms. avec RTServer !" );
  }
  if  ( (vg_DesSocks.curr != vg_DesSocks.rtsrv) && ( vg_DesSocks.curr != vg_DesSocks.lser ) && ( vg_DesSocks.curr != vg_DesSocks.ect ))
  {
    /*XZST_03EcritureTrace( XZSTC_FATAL,"Perte socket ??? => RECONNEXION !");*/

    XZST_03EcritureTrace( XZSTC_WARNING,"Perte socket => RECONNEXION !");

    //envoyer_trame_dai_ko(vg_DesSocks.ect, vg_DesSocks.curr_addr); 
    /*		if (ecom_init_IP(&vg_Config, &vg_SockList, &vg_DesSocks)) {
			XZST_03EcritureTrace( XZSTC_WARNING, "pb reconnexion ");
			return;
	}    */
  }

  /* SI deconnexion socket RTWorks */
  if ( (vg_DesSocks.curr == vg_DesSocks.rtsrv) && ( vg_DesSocks.curr == vg_DesSocks.lser ) );

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

void edir_traitement_liste_cmdes_LCR( void )

{
  static char *version = "$Id: eopc_dir.c,v 1.1 2021/05/04 13:46:53 pc2dpdy Exp $ :  edir_traitement_liste_cmdes_LCR ";

  XZEXT_Cmde_LCR_IP 	vl_CmdeLCR;	/* structure cmde. LCR depilee */


}


void edir_reveil_cb (T_IPC_CONN                      va_Cnx,
    T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
    T_CB_ARG                        pa_Arg)
{

  #ifndef _TIBCO_EMS  
  T_STR vl_cle;
  #else
  XDY_Nom vl_cle;
  #endif


  char vl_rgs[5];
  XZEXT_Cmde_LCR_IP vl_CmdeLCR;

  XZST_03EcritureTrace(XZSTC_WARNING,"edir_reveil_cb");
  if(!TipcMsgRead(pa_Data->msg,
      T_IPC_FT_STR, &vl_cle,
      NULL)) {
    XZST_03EcritureTrace(XZSTC_WARNING,"edir_reveil_cb : message non lu");
    return;
  }

  sscanf(vl_cle,"TEMIP_%s",vl_rgs);

  XZST_03EcritureTrace(XZSTC_FONCTION, "edir_reveil_cb : RGS %s",vl_rgs);

  /*recherche de la commande*/
  if (ecom_getfirst_cmd_LCR_IP2(&vl_CmdeLCR, &pg_ListeCmdesLCR, vl_rgs)==XDC_OK) {
    //epro_envoi_trame_LCR_IP( &vl_CmdeLCR, FALSE );
  }
}


/*X*/
/*------------------------------------------------------
 * SERVICE RENDU :
 * Retourne le context associé au numéro de BICE.
 *
------------------------------------------------------
 * SEQUENCE D'APPEL :
 *
 *
 * void edir_retourne_OPCcontext(EopcContext *Context, XDY_Eqt NumBICE);
 *
 * ARGUMENTS EN ENTREE :
 *
 * XDY_Eqt numéro de l'equipement
 *
 *
 * ARGUMENTS EN SORTIE :
 * Le contexte de communication opc associe a cet equipement
 *
 * CODE RETOUR :
 *
 * NEANT
 *
 * CONDITION D'UTILISATION
 *
 *
 * FONCTION
 *
 * retourn le contexte associe a un numero d'equipement
 *
------------------------------------------------------*/
int edir_retourne_OPCcontext(int *indice,XDY_Eqt va_NumBICE)
{
	int i;
	for (i=0; i< vg_nb_connexion; i++)
	{
		if (va_NumBICE==vg_list_EopcContext[i].NumBICE)
		{
			*indice=i;
			XZST_03EcritureTrace(XZSTC_WARNING,"edir_retourne_OPCcontext trouvee : EndPoint:%s",vg_list_EopcContext[*indice].Context.ServerConfiguration.Endpoint);
			return XDC_OK;
		}
	}
	return XDC_NOK;
}

void OPCdatachanged(void *va_NumBICE, char *nodeId, char *value)
{
	XZEXT_MSG_SOCKET pl_message;
	XDY_Eqt		NumBICE;

	NumBICE=*(XDY_Eqt*)va_NumBICE;

	XZST_03EcritureTrace(XZSTC_WARNING, "OPCdatachanged NumBICE=%d nodeId=%s value=%s",NumBICE,nodeId,value);
	
	sprintf(pl_message,"%s %s", nodeId,value);
	if (strstr(nodeId,EIMUC_TAG_MESSAGE_EN_COURS))
	{	
		if (EnvoyerTrameOPC(pl_message,
				 XZEXC_TRAME_ETAT_IMU_STRING,
				 NumBICE,
				 XZEXC_NOCMD,
				 XZEXC_PRIORITEFAIBLE,
				 vg_current_client_socket))
		XZST_03EcritureTrace(XZSTC_WARNING, "OPCdatachanged EnvoyerTrameOPC erreur. NumBICE:%d NodiID:%s value:%s",NumBICE,nodeId,value);
	}
	else
	{
		if (EnvoyerTrameOPC(pl_message,
				 XZEXC_TRAME_ETAT_IMU_INT,
				 NumBICE,
				 XZEXC_NOCMD,
				 XZEXC_PRIORITEFAIBLE,
				 vg_current_client_socket))
		XZST_03EcritureTrace(XZSTC_WARNING, "OPCdatachanged EnvoyerTrameOPC erreur. NumBICE:%d NodiID:%s value:%s",NumBICE,nodeId,value);
	}

	return ;
}				
