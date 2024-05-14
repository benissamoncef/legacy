/*E*/
/*Fichier : @(#)erdt_renv.c	1.1      Release : 1.1        Date : 09/25/95
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TERDT * FICHIER erdt_dir.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  < Module directeur de la tache TERDT >
*
------------------------------------------------------
* HISTORIQUE :
*
* Volcic.F	version 1.1	05 Dec 1994	: Creation
* Volcic.F	version 1.2	28 Jan 1995	:
* Volcic.F	version 1.3	13 Fev 1995	:
* Volcic.F	version 1.4	20 Mar 1995	: Modif Nom Cstes TIMER
* Volcic.F	version 1.5	20 Mar 1995	:
* Volcic.F	version 1.6	09 Avr 1995	:
* Volcic.F	version 1.7	21 Avr 1995	: Reprise entete, historique
* Volcic.F	version 1.8	07 Jun 1995	: Ajout definition callback et de l'abonnement pour le mode lent
* Volcic.F	version 1.9	03 Jui 1995	: Modif recuperation machine suppleante
* JMG		version 1.10	31/10.18	: ajout IP DEM1306
------------------------------------------------------*/

/* fichiers inclus */

#include "erdt_dir.h"

static char *version = "@(#)erdt_renv.c	1.1 09/25/95 : erdt_dir";

/* definitions de constantes */

/* definitions de types locaux */

typedef struct {
	
	int		NumMsg;		/* Numero de message */
	XDY_Fonction	FoncCB;		/* Fonction a appeler */

} tm_callback;

/* definition de macro locales */

/* declaration de variables locales */

static tm_callback pm_TabCB[] =
{
   { XDM_ECMD_EQPT, 			(void (*)())er_controle_cb },
   { XDM_EMES_RADT_6mn, 		(void (*)())er_EMes_6mn_Voisin_cb },
   { XDM_EMES_RADT_6gl, 		(void (*)())er_EMes_6gl_Voisin_cb },
   { XDM_EConf_Voie_RADT, 		(void (*)())er_Conf_Voie_RADT_cb },
   { XDM_EDEM_Indiv, 			(void (*)())er_Dem_Indiv_cb },
   { XDM_EInit_IHM_6gl_Voie, 		(void (*)())er_Init_IHM_6gl_Voie_cb },
   { XDM_EInit_IHM_6gl_Chaussee, 	(void (*)())er_Init_IHM_6gl_Chaussee_cb },
   { XDM_EInit_IHM_Liaison, 		(void (*)())ep_Init_IHM_Liaison_cb },
   { XDM_ELCR, 				(void (*)())er_CMD_LCR },
   { XDM_MODE_LENT, 			(void (*)())er_Mode_Lent_cb }
};


/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Programme Principal
*  
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
* 
------------------------------------------------------*/

int	argc;
char	**argv;
{
 	ERDT_DONNEES_RADT 	*pl_listeRADT;
	int	 		vl_ValRet	= XDC_OK;          	      	
	int			vl_SockRet	= 0;
	int			vl_SockSRV	= 0;
	int			vl_SockTIM	= 0;
	int			vl_resultat	= 0;
	XDY_Entier		vl_retcode	= 0;
	XZEXT_MSG_SOCKET	pl_MsgTimer;
   	XDY_Datagroup		pl_DG_JN;
	XDY_Datagroup		pl_DG_CMD;
	XDY_Datagroup		pl_DG_IHM;
	XDY_Datagroup		pl_DG_Conf;
	int			vl_indice_callback = 0;
	
	int			vl_jourSemaine	= 0;
	XDY_Horodate		vl_horodateSec	= 0.0;
	XZSMT_Horodate		vl_heure_string;
	
	XDY_Texte		pl_PathEtFic	= "";
	XDY_Texte		pl_Heure	= "";
	int			vl_Fd 		= 0;
	
	/*A  Initialisation du contexte */
	vg_Contexte = XDC_JOUR;
	
	/*A  Recuperation du nom de la machine */
	strcpy ( vg_NomMachine, argv[1] );

	/*A Connexion au RTserver et initialisation des variables */
	if( XZSC_01InitCnxSup(  NULL,
				argv[ 0 ],
				XZSCC_INTERDIRE,
				XZSCC_AUTORISER,
				XZSCC_INTERDIRE,
				er_sortir,
				"/dev/null" ) != XDC_OK )
	{
	   XZST_03EcritureTrace( XZSTC_FATAL, "erdt_dir : Ne peut ouvrir la connexion avec le RTserver." );
	}
	else
	{
	   XZST_03EcritureTrace( XZSTC_DEBUG1, "erdt_dir : Ouverture de la connexion avec le RTserver." );
	}

	/*A Recuperation du site de la machine */
 	if ( (vl_ValRet = XZSC_04NomSite(vg_NomSite)) != XDC_OK)
	{
	   XZST_03EcritureTrace( XZSTC_WARNING, "erdt_dir : Recuperation du Nom du site impossible . " );
	} 

	/*A  Recuperation du nom de la machine suppleante */
	if ( ex_lire_suppleant ( &vg_NomSuppleant ) != XDC_OK )
	   XZST_03EcritureTrace( XZSTC_WARNING, "erdt_dir : Impossible de recuperer le nom de la machine suppleante de %s", vg_NomMachine );

	XZST_03EcritureTrace( XZSTC_INFO, "erdt_dir : Suppleant de %s = %s", vg_NomMachine, vg_NomSuppleant );
	
	/*A Rangement des sockets */
	ex_rep_socket ( argv[1] );
	
	/*A Connexion socket en mode client pour communiquer avec la tache TESRV */
	if ( ex_cnx_client ( 	XZEXC_ADR_SOCK_TERDT,
				XZEXC_ADR_SOCK_TESRV,
				&vg_SockMask,
				&vl_SockSRV ) != XDC_OK )
	{
	   XZST_03EcritureTrace( XZSTC_WARNING, "erdt_dir : Impossible de creer la connexion client avec TESRV  ." );	
	}	

	
	/*A Connexion socket en mode client pour communiquer avec la tache TETIM */
	if ( ex_cnx_client ( 	XZEXC_ADR_SOCK_RDTTIM,
				XZEXC_ADR_SOCK_TETIM_SRV,
				&vg_SockMask,
				&vl_SockTIM ) != XDC_OK )
	{
	   XZST_03EcritureTrace( XZSTC_WARNING, "erdt_dir : Impossible de creer la connexion client avec TETIM ." );	
	}	

	/*A Connexion socket en mode client pour communiquer avec la tache TETDC */
	if ( ex_cnx_client ( 	XZEXC_ADR_SOCK_RDTTDC,
				XZEXC_ADR_SOCK_TETDC_SRV,
				&vg_SockMask,
				&vg_SockTDC ) != XDC_OK )
	{
	   XZST_03EcritureTrace( XZSTC_WARNING, "erdt_dir : Impossible de creer la connexion client avec TETDC." );	
	}	

	/*A Appel de la fonction d'initialisation des tableaux de valeurs des mesures */
 	er_init_tab_mes ( );
 	
	/*A Appel de MCFG fonction er_init */
        if ( ( vl_retcode = er_init( vg_NomMachine ) ) != XDC_OK )
        {
           XZST_03EcritureTrace( XZSTC_FATAL, "erdt_dir : er_init retourne %d", vl_retcode );	
	}

	/*A Armement des Callback sur reception des message */
	 
        /*B Initialisation des callbacks de la tache */
        for (vl_indice_callback = 0; vl_indice_callback < (sizeof(pm_TabCB)/sizeof(pm_TabCB[0])); vl_indice_callback++)
	{
	   if(!TipcSrvProcessCbCreate(	XDM_IdentMsg(pm_TabCB[vl_indice_callback].NumMsg ) ,
						pm_TabCB[vl_indice_callback].FoncCB ,
						(T_CB_ARG)NULL) )
	   {
      	      XZST_03EcritureTrace(XZSTC_WARNING,
					"erdt_dir : erreur init callback '%s'",
					pm_TabCB[vl_indice_callback].FoncCB);
	   }
	}
	
	/*A Abonnement sur le message XDM_ECMD_JourNuit */
        XDG_EncodeDG2 ( pl_DG_JN, ERDTC_JOURNUIT_RADT, vg_NomMachine) ;
        if( ( vl_retcode = XZEA29_Abt_Etat_JourNuit(XZEAC_DEBUT_ABONNEMENT,pl_DG_JN,er_journuit,vg_NomSite,&vl_resultat) ) != XDC_OK)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"TERDT : Impossible de s'abonner a JourNuit");
           vg_Contexte=XDC_JOUR;
        }

        /*A Abonnement aux Datagroup */
	
	/*B Mode Lent : XDG_MODE_LENT_<NomMachine> */
	sprintf ( pl_DG_CMD, "%s_%s", XDG_MODE_LENT, vg_NomMachine) ;			
   	
   	/* joint ce datagroup */
   	if ( !XDG_JoindreDG( pl_DG_CMD ) )
   	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"erdt_dir : Abonnement au Datagroup %s non effectue ", pl_DG_CMD );
	}			
	
	/*B Messages de controle : XDG_ECMD_RADT_<NomMachine> */
	sprintf ( pl_DG_CMD, "%s_%s", XDG_ECMD_RADT, vg_NomMachine) ;			
   	
   	/* joint ce datagroup */
   	if ( !XDG_JoindreDG( pl_DG_CMD ) )
   	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"erdt_dir : Abonnement au Datagroup %s non effectue ", pl_DG_CMD );
	}			

	/*A Messages de demande de mesure RADT : XDG_EInit_IHM_RADT_NomSite */
	sprintf ( pl_DG_IHM, "%s_%s", XDG_EInit_IHM_RADT, vg_NomSite) ;			
 
   	/* joint ce datagroup */
   	if ( !XDG_JoindreDG( pl_DG_IHM ) )
   	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"erdt_dir : Abonnement au Datagroup %s non effectue ", pl_DG_IHM );
	}


	/*B Messages de commance de contraste : XDG_EInit_IHM_Liaison_<NomMachine> */
	XDG_EncodeDG2 ( pl_DG_IHM, XDG_EInit_IHM_Liaison, vg_NomMachine) ;			
   	/* joint ce datagroup */
   	if ( !XDG_JoindreDG( pl_DG_IHM ) )
   	{
	   /* Affichage Trace */
      	   XZST_03EcritureTrace(XZSTC_FATAL,"Abonnement au Datagroup %s non effectue ", pl_DG_IHM );
	}			

				
	/*A Messages de configuration de voie : XDG_ECONF_VOIE_RADT_<NomMachine> */
	sprintf ( pl_DG_Conf, "%s_%s", XDG_ECONF_VOIE_RADT, vg_NomMachine) ;			
 
   	/* joint ce datagroup */
   	if ( !XDG_JoindreDG( pl_DG_Conf ) )
   	{
	   XZST_03EcritureTrace(XZSTC_WARNING,"erdt_dir : Abonnement au Datagroup %s non effectue ", pl_DG_Conf );
	}			
	
	/*A Definition et initialisation des timers cycliques et Demande de l'heure courante RTWorks */
	if ( ( vl_retcode = XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME, &vl_jourSemaine, &vl_horodateSec ) ) != XDC_OK )
	{
	   if ( ( vl_retcode = XZST_10ArchiverMsgSyst ( 0 , "erdt_dir : Probleme pour recuperer l'heure RTWORKS, la tache TERDT ne peut pas demarer", NULL ) ) != XDC_OK )
	   {	
	      XZST_03EcritureTrace(XZSTC_WARNING,"erdt_dir : Appel XZST_10ArchiverMsgSyst retourne ", vl_retcode );
	   }
	   XZST_03EcritureTrace(XZSTC_FATAL,"erdt_dir : Appel XZSM_07LireHorodate retourne ", vl_retcode );
	}
	
	/*B Conversion de l'heure recue (en secondes) en date (char[20]) */
	if ( ( vl_retcode = XZSM_11ConversionHorodate ( vl_horodateSec, &vl_heure_string ) ) != XDC_OK )
	{
	   if ( ( vl_retcode = XZST_10ArchiverMsgSyst ( 0 , "erdt_dir : Probleme pour conversion heure RTWORKS, la tache TERDT ne peut pas demarer", NULL ) ) != XDC_OK )
	   {	
	      XZST_03EcritureTrace(XZSTC_WARNING,"erdt_dir : Appel XZST_10ArchiverMsgSyst retourne ", vl_retcode );
	
	   }
	   XZST_03EcritureTrace(XZSTC_FATAL,"erdt_dir : Appel XZSM_11ConversionHorodate retourne ", vl_retcode );
	}
	
	vg_Date_Changement = vl_horodateSec;
	vg_Date_Actuelle   = vl_horodateSec;
		
	/*B Programation du reveil a minuit (0h00) */
	ETIM_SYNCHRO_DATE ( XZEXC_HEURE_MINUIT, ERDTC_TIMER_MINUIT, vl_SockTIM, pl_MsgTimer )
	
 	/*B Programation de l'heure de la synchro date */
	ETIM_SYNCHRO_DATE ( XZEXC_HEURE_SYNCHRO, ERDTC_TIMER_SYNCHRO, vl_SockTIM, pl_MsgTimer )
	
        /*B Pour tous les RADT de la machine => demande mesure 1mn Q V TO, 6mn %PL et demande mesure 6mn Q V TO */
        for ( pl_listeRADT = pg_debutRADT; pl_listeRADT != NULL; pl_listeRADT = pl_listeRADT->Suivant )
        {
	   	
 	   /*B Ecriture de l'horodate des dernieres mesures recues dans un fichier */
   	   sprintf ( pl_PathEtFic, "%s/%s_%s_R%d", ERDTC_PATH_FIC_DYN, ERDTC_FIC_HEUR_QVTO, vg_NomMachine,  pl_listeRADT->ConfRADT.NumeroStation );
 	   if ( ( vl_Fd = open ( pl_PathEtFic, O_RDWR ) ) == -1 )
           {	
              XZST_03EcritureTrace( XZSTC_WARNING, "%s : erreur ouverture fichier %s.\n", version, pl_PathEtFic ); 
 	      vl_Fd = open ( pl_PathEtFic, O_CREAT, S_IWRITE | S_IREAD );
	      pl_listeRADT->ConfRADT.Demande_Histo = XDC_FAUX;	   
	      pl_listeRADT->ConfRADT.Horodate_Histo = vl_horodateSec;	   
	   }
	   else
	   {
	      read ( vl_Fd, pl_Heure, sizeof ( pl_Heure ) );
	      XZSM_13ConversionHorodateSec(pl_Heure,&pl_listeRADT->ConfRADT.Horodate_Histo);
	      close ( vl_Fd );
	   }
           
           /*B demande mesure 6mn %PL si le type de station n'est pas de type echangeur */ 
           if ( pl_listeRADT->ConfRADT.TypeStation != ERDTC_ECHANGEUR ) {
	      if (!strcmp(pl_listeRADT->ConfRADT.AdresseIP, XZECC_IP_VIDE)) {
              	if ( ( vl_retcode = er_EnvoyerTrame(	"MB LC",
                                   			XZEXC_TRAME_MES_6MN_PL,
                                   			pl_listeRADT->ConfRADT.RGS,
                                   			XZEXC_NOCMD,
	                           			XZEXC_PRIORITEFAIBLE,
                                   			pl_listeRADT->Socket ) ) != XDC_OK )
           	{
              	   XZST_03EcritureTrace( XZSTC_WARNING, "%s : EnvoyerTrame retourne %d", version, vl_retcode );
  	        }
               }
             else {
                                        EnvoyerTrameIP( "MB LC",
                                                        XZEXC_TRAME_MES_6MN_PL,
                                                        pl_listeRADT->ConfRADT.AdresseIP,
                                                        pl_listeRADT->ConfRADT.Port,
                                                        pl_listeRADT->ConfRADT.RGS,
                                                        XZEXC_NOCMD,
                                                        XZEXC_PRIORITEFAIBLE,
                                                        pl_listeRADT->Socket );
            }
	}
	   /*B demande mesure 6mn Q V TO */ 
	  if (!strcmp(pl_listeRADT->ConfRADT.AdresseIP, XZECC_IP_VIDE)) {
           if ( ( vl_retcode = er_EnvoyerTrame(	"MB",
                                   		XZEXC_TRAME_MES_6MN_Q_V_TO,
                                   		pl_listeRADT->ConfRADT.RGS,
                                   		XZEXC_NOCMD,
	                           		XZEXC_PRIORITEFAIBLE,
                                   		pl_listeRADT->Socket ) ) != XDC_OK )
           {
              XZST_03EcritureTrace( XZSTC_WARNING,"%s : EnvoyerTrame retourne %d", version, vl_retcode );
  	   } 
          }	
             else {
                                        EnvoyerTrameIP( "MB",
                                                        XZEXC_TRAME_MES_6MN_Q_V_TO,
                                                        pl_listeRADT->ConfRADT.AdresseIP,
                                                        pl_listeRADT->ConfRADT.Port,
                                                        pl_listeRADT->ConfRADT.RGS,
                                                        XZEXC_NOCMD,
                                                        XZEXC_PRIORITEFAIBLE,
                                                        pl_listeRADT->Socket );
            }
	   /*A demande mesure 1mn Q V TO si pas de mode lent */ 
           if ( vg_Mode_Lent == XDC_FAUX )
           {
	     if (!strcmp(pl_listeRADT->ConfRADT.AdresseIP, XZECC_IP_VIDE)) {
               if ( ( vl_retcode = er_EnvoyerTrame(	"Mm",
                                   			XZEXC_TRAME_MES_1MN_Q_V_TO,
                                   			pl_listeRADT->ConfRADT.RGS,
                                   			XZEXC_NOCMD,
	                           			XZEXC_PRIORITEFAIBLE,
                                   			pl_listeRADT->Socket ) ) != XDC_OK )
        	{
                   XZST_03EcritureTrace( XZSTC_WARNING,"%s : EnvoyerTrame retourne %d", version, vl_retcode );
  		}
 	      }
             else {
                                        EnvoyerTrameIP( "Mm",
                                                        XZEXC_TRAME_MES_1MN_Q_V_TO,
                                                        pl_listeRADT->ConfRADT.AdresseIP,
                                                        pl_listeRADT->ConfRADT.Port,
                                                        pl_listeRADT->ConfRADT.RGS,
                                                        XZEXC_NOCMD,
                                                        XZEXC_PRIORITEFAIBLE,
                                                        pl_listeRADT->Socket );
            }
  	    }   
  	
  	
  	} /* Fin demande mesures È l'init */
  	
  	/*A Attente des messages sur les sockets (RTserver, d'ecoute et de donnees */
	while (1)
	{
	   if ( ex_attendre_msg ( XZEXC_SOCKET_NULL, &vg_SockMask, &vl_SockRet ) != XDC_OK )
	   {
              XZST_03EcritureTrace(XZSTC_WARNING, "erdt_dir : Probleme retour fonction select  ." );
              ex_ConnexionSocketRTserver ( &vg_SockRT, vl_SockRet, &vg_SockMask );
           }
           else
           {
              ex_ConnexionSocketRTserver ( &vg_SockRT, 0, &vg_SockMask );

	      /*B Si tous les timers n'ont pas etes enclenches => appel de la fonction er_Lancement_Timer */
	      if ( vg_Timers_Enclenches == XDC_FAUX )
	      {
	         er_Lancement_Timer ( vl_SockTIM );  
	      }
	      
	      /*B Si une socket a ete activee */
	      if ( vl_SockRet != XZEXC_NO_EVT )
	      {
	         /*B Selon le type de socket */
	         /*B Si Socket TETIM (retour Timer) */
	          if ( vl_SockRet == vl_SockTIM )
	          {	
	             /*B Appel de la fonction er_traitement_cyclique de MCYC */
	             er_traitement_cyclique ( vl_SockTIM );
	          }
	          else
	          {
	             /*B Socket TESRV (Init ou Arret tache) */
	             if ( vl_SockRet == vl_SockSRV )
	             {	
	                /*B Appel de la fonction er_controle de MCFG */
	                er_controle ( vl_SockSRV, vg_NomMachine );
	             }
	             /*B Sinon */
	             else
	             {
	                /*B Appel de la fonction er_traitement_trame */
	                er_traitement_trame( vl_SockRet );
	             }
	         }
	      }   /* fin si socket active */
	   }   
	}   /* fin while */
}  /* Fin main */


