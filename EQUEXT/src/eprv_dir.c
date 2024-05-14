/* Fichier : eprv_dir.c
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEPMV * FICHIER eprv_dir.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Module directeur de la tache TEPMV
* 
*
------------------------------------------------------
* HISTORIQUE :
*
* VR	19/12/11		: Creation (DEM/1016)
* RGR 17/05/19 : DEM1331 - Ajout Mecanisme KEEPALIVE 1.2
* ABE	31/05/21	: PRV sur IP DEM-SAE283 1.3								
* JPL	25/11/21 : Consultation echeance minuterie et traitement des alarmes en fin de boucle principale (SAE_339) 
*
------------------------------------------------------*/

/* fichiers inclus */

#include <xzsem.h>
#include "eprv_dir.h"

/* definitions de constantes */

/* definitions de types locaux */

typedef struct {
	int		NumMsg;		/* Numero de message */
	XDY_Fonction	FoncCB;		/* Fonction a appeler */
} tm_callback;

/* definition de macro locales */

/* declaration de variables locales */

/* declaration de variables globales */

static tm_callback pm_TabCB[] =
{
   { XDM_ECMD_EQPT, 		(void (*)())ep_controle_cb },
   { XDM_ECMD_PRV, 		(void (*)())ep_cmd_PRV_cb },
   { XDM_EInit_IHM_PRV, 	(void (*)())ep_Init_IHM_PRV_cb },
   { XDM_ECMD_Contraste, 	(void (*)())ep_contraste_cb },
   { XDM_EInit_IHM_Contraste, 	(void (*)())ep_Init_IHM_contraste_cb },
   { XDM_EInit_IHM_Liaison, 	(void (*)())ep_Init_IHM_Liaison_cb },
   { XDM_ELCR, 			(void (*)())ep_CMD_LCR }
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
	int 			vl_ValRet	=XDC_OK,          	      	
				vl_SockRet	= 0,
				vl_SockSRV	= 0,
				vl_resultat;
	XZEXT_MSG_SOCKET	pl_MsgTimer;
   	XDY_Datagroup		pl_NomDG;
	int			vl_indice_callback;
	XDY_Mot			vl_MaxEqtLigne = 0;
	XDY_Mot			vl_CoefTimer = 1;

	XZST_03EcritureTrace( XZSTC_FONCTION, "IN : main ");

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
	   XZST_03EcritureTrace( XZSTC_FATAL, "Ne peut ouvrir la connexion avec le RTserver." );
	}
	else
	{
	   /*B Affichage Trace */
	   XZST_03EcritureTrace( XZSTC_DEBUG1, "Ouverture de la connexion avec le RTserver." );
	}
	XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_WARNING|XZSTC_INFO|XZSTC_FONCTION|XZSTC_INTERFACE|XZSTC_MESSAGE,XZSTC_NECRAN);

	XZST_03EcritureTrace( XZSTC_INFO, " Connexion au RTserver et initialisation des variables");

	/*A
	 * Recuperation du site de la machine 
	 */
	if ( (vl_ValRet = XZSC_04NomSite(vg_NomSite)) != XDC_OK)
	{
	   /* Ecriture Trace */
           XZST_03EcritureTrace( XZSTC_FATAL, "Recuperation du Nom du site impossible." );
	} 

	XZST_03EcritureTrace( XZSTC_INFO, " Recuperation du Nom du site %s", vg_NomSite);

	/*A Creation du repertoire pour les sockets */
	ex_rep_socket ( argv[1] );
	
	vg_SockTEMIP=-1;					

	XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_WARNING|XZSTC_INFO|XZSTC_FONCTION|XZSTC_INTERFACE,XZSTC_NECRAN);
	//XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_WARNING|XZSTC_DEBUG1|XZSTC_DEBUG2|XZSTC_DEBUG3|XZSTC_INFO|XZSTC_FONCTION|XZSTC_INTERFACE|XZSTC_MESSAGE,XZSTC_NECRAN);

	/*A Init pour Acknoledge Alarme */
	ex_init_alarme ( argv[1], argv[0] );

	/*! Mise a jour variable simulation */
        if ( !access ( "simulPRV", F_OK ) )
           vg_Simulation = 1;
        else
           vg_Simulation = 0;
        
        XZST_03EcritureTrace( XZSTC_INFO, "simulation %d",vg_Simulation);

        /*A
	 * Connexion socket en mode client pour communiquer avec la tache TESRV 
	 */
        XZST_03EcritureTrace( XZSTC_INFO, "communiquer avec la tache TESRV ");
	if ( ex_cnx_client ( 	XZEXC_ADR_SOCK_TEPRV,
				XZEXC_ADR_SOCK_TESRV,
				&vg_SockMask,
				&vl_SockSRV ) != XDC_OK )
	{
	   /*B Ecriture Trace */
           XZST_03EcritureTrace( XZSTC_FATAL, "Impossible de creer la connexion client avec TESRV" );
	}	

	XZST_03EcritureTrace( XZSTC_INFO, " Connexion socket en mode client pour communiquer avec la tache TESRV");
	
	/*A 
	 * Connexion socket en mode client pour communiquer avec la tache TETIM 
	 */
	if ( ex_cnx_client ( 	XZEXC_ADR_SOCK_PRVTIM,
				XZEXC_ADR_SOCK_TETIM_SRV,
				&vg_SockMask,
				&vg_SockTIM ) != XDC_OK )
	{
	   /*B Ecriture Trace */
           XZST_03EcritureTrace( XZSTC_FATAL, "Impossible de creer la connexion client avec TETIM" );
	}	
	XZST_03EcritureTrace( XZSTC_INFO, " Connexion socket en mode client pour communiquer avec la tache TETIM");

	XZST_03EcritureTrace( XZSTC_INFO, " vg_NomMachine %s",vg_NomMachine);
	/*A 
	 * Appel de MCFG fonction ec_init 
	 */
        if ( ec_init(vg_NomMachine, &vl_MaxEqtLigne ) != XDC_OK)
	{
	   /*B Ecriture Trace */
           XZST_03EcritureTrace( XZSTC_FATAL, "Initialisation de la config echouee" );
	}	
        
        XZST_03EcritureTrace( XZSTC_INFO, " Appel de MCFG");
	/*A 
	 * Armement des Callback sur reception des message 
	 */
    XZST_03EcritureTrace( XZSTC_INFO, "  Armement des Callback sur reception des message ");
         /* Initialisation des callbacks de la tache */
	
		for (vl_indice_callback = 0; vl_indice_callback < (sizeof(pm_TabCB)/sizeof(pm_TabCB[0])); vl_indice_callback++)
		{
		   if(!TipcSrvProcessCbCreate(	XDM_IdentMsg(pm_TabCB[vl_indice_callback].NumMsg ) ,
							pm_TabCB[vl_indice_callback].FoncCB ,
							(T_CB_ARG)NULL) )
		   {
				  XZST_03EcritureTrace(XZSTC_WARNING, "erreur init callback '%s'",
						pm_TabCB[vl_indice_callback].FoncCB);
		   }
		}
	
         XZST_03EcritureTrace( XZSTC_INFO, "  Initialisation des callbacks ");

	/*A Abonnement sur le message XDM_ECMD_JourNuit */
	
	XDG_EncodeDG2 ( pl_NomDG, "Einit_JourNuit_PRV", vg_NomMachine) ;			
	if(XZEA29_Abt_Etat_JourNuit(XZEAC_DEBUT_ABONNEMENT,pl_NomDG,ep_journuit,vg_NomSite,&vl_resultat) != XDC_OK)
      	   XZST_03EcritureTrace(XZSTC_WARNING,"TEPRV : Impossible de s'abonner a JourNuit");
        XZST_03EcritureTrace( XZSTC_INFO, "  Abonnement sur le message JourNuit dans DG %s", pl_NomDG);

	/*A 
	 * Abonnement aux Datagroup 
	 */
    XZST_03EcritureTrace( XZSTC_INFO, "  Abonnement aux Datagroup ");
	/* Messages de commande PMV : XDG_ECMD_PRV<NomMachine> */
	XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_PRV, vg_NomMachine) ;
	XZST_03EcritureTrace( XZSTC_INFO, "  Messages de commande PRV ");
   	/* joint ce datagroup */
   	if ( !XDG_JoindreDG( pl_NomDG ) )
   	{
	   /* Affichage Trace */
      	   XZST_03EcritureTrace(XZSTC_FATAL, "Abonnement au Datagroup %s non effectue ", pl_NomDG );
	}
	XZST_03EcritureTrace( XZSTC_INFO, " Abonnement au Datagroup %s effectue ", pl_NomDG);

	/* Messages de commande PRV : XDG_EInit_IHM_PRV<NomSite> */
	XDG_EncodeDG2 ( pl_NomDG, XDG_EInit_IHM_PRV, vg_NomSite) ;
	XZST_03EcritureTrace( XZSTC_INFO, " Messages d'init IHM ");
   	/* joint ce datagroup */
   	if ( !XDG_JoindreDG( pl_NomDG ) )
   	{
	   /* Affichage Trace */
      	   XZST_03EcritureTrace(XZSTC_FATAL, "Abonnement au Datagroup %s effectue ", pl_NomDG );
	}			
	XZST_03EcritureTrace( XZSTC_INFO, " Abonnement au Datagroup %s effectue ", pl_NomDG);

	/* Messages de commande PICTO : XDG_ECMD_CONTRASTE_<NomSite> */
	XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_CONTRASTE, vg_NomSite) ;			
	XZST_03EcritureTrace( XZSTC_INFO, " Messages de contraste ");
  	/* joint ce datagroup */
   	if ( !XDG_JoindreDG( pl_NomDG ) )
   	{
	   /* Affichage Trace */
      	   XZST_03EcritureTrace(XZSTC_FATAL, "Abonnement au Datagroup %s non effectue ", pl_NomDG );
	}
	XZST_03EcritureTrace( XZSTC_INFO, " Abonnement au Datagroup %s effectue ", pl_NomDG);
				
	/* Messages de commande de contraste : XDG_EInit_IHM_CONTRASTE_<NomSite> */
	XDG_EncodeDG2 ( pl_NomDG, XDG_EInit_IHM_CONTRASTE, vg_NomSite) ;			
	XZST_03EcritureTrace( XZSTC_INFO, " Messages de contraste IHM ");
   	/* joint ce datagroup */
   	if ( !XDG_JoindreDG( pl_NomDG ) )
   	{
	   /* Affichage Trace */
      	   XZST_03EcritureTrace(XZSTC_FATAL, "Abonnement au Datagroup %s non effectue ", pl_NomDG );
	}			
	XZST_03EcritureTrace( XZSTC_INFO, " Abonnement au Datagroup %s effectue ", pl_NomDG);


	/* Messages de commance de contraste : XDG_EInit_IHM_Liaison_<NomMachine> */
	XDG_EncodeDG2 ( pl_NomDG, XDG_EInit_IHM_Liaison, vg_NomMachine) ;			
	XZST_03EcritureTrace( XZSTC_INFO, " Messages de liaison IHM ");
   	/* joint ce datagroup */
   	if ( !XDG_JoindreDG( pl_NomDG ) )
   	{
	   /* Affichage Trace */
      	   XZST_03EcritureTrace(XZSTC_FATAL, "Abonnement au Datagroup %s non effectue ", pl_NomDG );
	}			
	XZST_03EcritureTrace( XZSTC_INFO, " Abonnement au Datagroup %s effectue ", pl_NomDG);

  /* Abonnement au DataGroup KEEPALIVE */
  XZSC_31AbtKeepAlive(vg_NomSite);

	/*A
	 * Demande des timers cycliques
	 */
	/* Calcul du coef des timer en fonction du max d'equipement par ligne */
	if ( vl_MaxEqtLigne > 5 )  vl_CoefTimer = 2;
	if ( vl_MaxEqtLigne > 11 ) vl_CoefTimer = 3;
        XZST_03EcritureTrace(XZSTC_WARNING, "Nb eqt max = %d  Coef = %d ",  vl_MaxEqtLigne, vl_CoefTimer );

	/* Programation timer 1mn */
	ETIM_REVEIL_CYCLIQUE ( EPRVC_REVEIL_1MN * vl_CoefTimer, EPRVC_TIMER_1MN, vg_SockTIM, pl_MsgTimer )

	/* Programation timer 10s */
	ETIM_REVEIL_CYCLIQUE ( EPRVC_REVEIL_10S * vl_CoefTimer*2, EPRVC_TIMER_10S, vg_SockTIM, pl_MsgTimer )
	
	/* Programation de l'heure de la synchro date */
	XZST_03EcritureTrace(XZSTC_WARNING, "Programation Heure ");
	ETIM_SYNCHRO_DATE ( XZEXC_HEURE_SYNCHRO, "PRV_Timer_Synchro", vg_SockTIM, pl_MsgTimer )
	
	/*A Attente des messages sur les sockets (RTserver, d'ecoute et de donnees */
		
	while (1)
	{
	   
	   if ( ex_attendre_msg ( XZEXC_SOCKET_NULL, &vg_SockMask, &vl_SockRet ) != XDC_OK )
	   {
              XZST_03EcritureTrace(XZSTC_WARNING, "Probleme retour fonction select  ." );
              ex_ConnexionSocketRTserver ( &vg_SockRT, vl_SockRet, &vg_SockMask );
           }
           else
           {
              ex_ConnexionSocketRTserver ( &vg_SockRT, 0, &vg_SockMask );
	   
	      /* Si une socket a ete activee */
	      if ( vl_SockRet != XZEXC_NO_EVT )
	      {
	         /* Selon le type de socket */
	         /* Socket RT server */
	    	  /*XZST_03EcritureTrace( XZSTC_INFO, "Si une socket a ete activee ");*/
	         if ( vl_SockRet == vg_SockRT ) 	
	         {
	            /* Le message est lu par les primitive standard de RTWorks */
/*	            TipcSrvMainLoop(3.0);*/
		    	  XZST_03EcritureTrace( XZSTC_INFO, "Socket RT server ");
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
	               /* Socket TESRV (Init ou Arret tache) */
	               if ( vl_SockRet == vl_SockSRV )
	               {	
	                  /* Appel de la fonction ec_configuration de MCFG */
	                  ec_controle ( vl_SockSRV, vg_NomMachine );
	                  XZST_03EcritureTrace( XZSTC_INFO, "Appel de la fonction ec_configuration de MCFG ");
	               }
	               else
	               {
	                  /* Appel traitement de la trame */
	                  em_traitement_trame( vl_SockRet );
	                  XZST_03EcritureTrace( XZSTC_INFO, "Appel traitement de la trame ");
	               }
	            }
	         }
	      }   /* fin si socket active */
	   }

	   if (XZSS_15ConsulterTimeout () == XDC_OUI)
	   {
	      XZSS_18AnnulerTimeout ();
	      ex_traiter_alarmes ();
	   }
	}   /* fin while */
	

	XZST_03EcritureTrace( XZSTC_FONCTION, "OUT : fin main");

}  /* Fin main */


