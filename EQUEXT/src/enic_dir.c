/*E*/
/* Fichier : enic_dir.c
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TENIC * FICHIER enic_dir.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* C'est le module directeur de la tache GTC Nice
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	02 Nov 1994	: Creation
* Mismer.D	version 1.2	09 Fev 1995	:
* Fontaine.C	version 1.3	22 Mai 1995	: Reprise entete, historique
* Mismer.D  	version 1.4	07 Jun 1995	: Modif connexion socket RTserver
* Mismer.D  	version 1.5	31 Aou 1995	: Modif connexion socket RTserver suite
* Mismer.D  	version 1.6	11 Oct 1995	: Ajout appel fonction init Acknoledge alarme
* JPL	23/11/21 : Consultation echeance minuterie et traitement des alarmes en fin de boucle principale (SAE_339)  1.7
------------------------------------------------------*/

/* fichiers inclus */

#include <xzsem.h>
#include "enic_dir.h"

/* definitions de constantes */

/* definitions de types locaux */

typedef struct {
	int		NumMsg;		/* Numero de message */
	XDY_Fonction	FoncCB;		/* Fonction a appeler */
}tm_callback;

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: enic_dir.c,v 1.6 1995/10/16 12:25:43 volcic Exp $ : enic_dir" ;

static tm_callback pm_TabCB[] =
{
   { XDM_ECMD_SIGT, 		(void (*)())et_cmd_sigt_cb },
   { XDM_ECMD_SIGE, 		(void (*)())et_cmd_sige_cb },
   { XDM_ECMD_SEQT, 		(void (*)())et_cmd_seqt_cb },
   { XDM_ECMD_SEQE, 		(void (*)())et_cmd_seqe_cb },
   { XDM_EInit_IHM_NICE, 	(void (*)())et_Init_IHM_TNIC_cb },
   { XDM_ECMD_EQPT, 		(void (*)())et_controle_cb },
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
* 
------------------------------------------------------*/

int	argc;
char	**argv;
{
	int 			vl_ValRet	=XDC_OK,          	      	
				vl_SockRet	= 0,
				vl_SockSRV	= 0,
				vl_resultat;
	XZEXT_MSG_SOCKET	pl_MsgTimer;
	int			vl_indice_callback;
   	XDY_NomSite        	pl_NomSite;
   	XDY_Datagroup		pl_NomDG;
	
	
	/*A
	 *  Recuperation du nom de la machine 
	 */
	strcpy( vg_NomMachine, argv[1] );

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
				et_sortir,
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

	/*A Creation du repertoire pour les sockets */
	ex_rep_socket ( vg_NomMachine );
	
	/*A Init pour Acknoledge Alarme */
	ex_init_alarme ( argv[1], argv[0] );

	/*A 
	 * Connexion socket en mode client pour communiquer avec la tache TESRV 
	 */
	if ( ex_cnx_client ( 	XZEXC_ADR_SOCK_TENIC,
				XZEXC_ADR_SOCK_TESRV,
				&vg_SockMask,
				&vl_SockSRV ) != XDC_OK )
	{
	   /*B Ecriture Trace */
           XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de creer la connexion client TESRV ." );	
	}	

	
	/*A 
	 * Connexion socket en mode client pour communiquer avec la tache TETIM 
	 */
	if ( ex_cnx_client ( 	XZEXC_ADR_SOCK_NICTIM,
				XZEXC_ADR_SOCK_TETIM_SRV,
				&vg_SockMask,
				&vg_SockTETIM ) != XDC_OK )
	{
	   /*B Ecriture Trace */
           XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de creer la connexion client TETIM ." );	
	}	

	/*A 
	 * Connexion socket en mode client pour communiquer avec la tache TEXA0 
	 */
	if ( ex_cnx_client ( 	XZEXC_ADR_SOCK_NICXA0,
				XZEXC_ADR_SOCK_TEXA0_SRV,
				&vg_SockMask,
				&vg_SockTEXA0 ) != XDC_OK )
	{
	   /*B Ecriture Trace */
           XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de creer la connexion client TEXA0 ." );	
	}	

	/*A 
	 * Appel de MCFG fonction et_init 
	 */
         et_init(vg_NomMachine);


	/*A 
	 * Armement des Callback sur reception des message 
	 */
	 
	/*A Recuperation du site de la machine */
	if ( XZSC_04NomSite(pl_NomSite) != XDC_OK)
	{
	   /* Ecriture Trace */
           XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom du site impossible ." );
	} 
	
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
	/* Messages de commande signalisation tunnel : XDG_ECMD_GTC_NICE */
   	if ( !XDG_JoindreDG( XDG_ECMD_GTC_NICE ) )
   	{
	   /* Affichage Trace */
      	   XZST_03EcritureTrace(XZSTC_WARNING,
	   "Abonnement au Datagroup %s non effectue ", XDG_ECMD_GTC_NICE );
	}	
			
	/* Messages de abonnement : XDG_EInit_IHM_NICE */
   	/* joint ce datagroup */
	XDG_EncodeDG2 ( pl_NomDG, XDG_EInit_IHM_NICE, pl_NomSite ) ;			
   	if ( !XDG_JoindreDG( pl_NomDG ) )
   	{
	   /* Affichage Trace */
      	   XZST_03EcritureTrace(XZSTC_WARNING,
	   "Abonnement au Datagroup %s non effectue ", XDG_EInit_IHM_NICE );
	}			


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
	         if ( vl_SockRet == vg_SockRT ) 	
	         {
	            /* Le message est lu par les primitive standard de RTWorks */
	         }
	         else
	         {
	            /* Socket TETIM (retour Timer) */
	            if ( vl_SockRet == vg_SockTETIM )
	            {	
	               /* Appel de la fonction et_traitement_timers */
	               et_traitement_timers ();
	            }
	            else
	            {
	               /* Socket TESRV (Init ou Arret tache) */
	               if ( vl_SockRet == vl_SockSRV )
	               {	
	                  /* Appel de la fonction et_controle de MCFG */
	                  et_controle ( vl_SockSRV, vg_NomMachine );
	               }
	               else
	               {
	                  /* Appel traitement de la trame */
	                  et_traitement ( vl_SockRet );
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

}  /* Fin main */

