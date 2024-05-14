/*E*/
/* Fichier : eope_dir.c
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  EQUEXT
------------------------------------------------------
* MODULE eope_dir * FICHIER eope_dir.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* C'est le module directeur de la tache TEOPE
*
------------------------------------------------------
* HISTORIQUE :
*
* Nouaille.M	Version 1.1	02 Dec 1994	: Creation
* Mismer.D      Version 1.2     28 Jan 1995     : RAS
* Fontaine.C    Version 1.3     17 Mai 1995     : Reprise historique
* Mismer.D  	version 1.4	07 Jun 1995	: Modif connexion socket RTserver
* Mismer.D  	version 1.5	31 Aou 1995	: Modif connexion socket RTserver suite
* Mismer.D  	version 1.8	11 Oct 1995	: Ajout appel fonction init Acknoledge alarme
* JPL	23/11/21 : Consultation echeance minuterie et traitement des alarmes en fin de boucle principale (SAE_339)  1.7
------------------------------------------------------*/

/* fichiers inclus */

#include <xzsem.h>
#include "eope_dir.h"

/* definitions de constantes */

/* definitions de types locaux */

typedef struct {
	int		NumMsg;		/* Numero de message */
	XDY_Fonction	FoncCB;		/* Fonction a appeler */
}tm_callback;

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: eope_dir.c,v 1.6 1995/10/16 12:25:44 volcic Exp $ : eope_dir" ;

static tm_callback pm_TabCB[] =
{
   { XDM_ECMD_EQPT, 		(void (*)())ed_controle_cb },
   { XDM_EINIT_ABT, 		(void (*)())ed_init_etat_OPER },
   { XDM_ECMD_OPER, 		(void (*)())ed_cmd_OPER },
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
* init des structures en memoire, gestion des callbacks
* 
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
   	XDY_Datagroup		pl_NomDG;
	int			vl_indice_callback;
	
	
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
	   exit( XDC_NOK );
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
           XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom du site impossible . " );
	} 

	/*A Creation du repertoire pour les sockets */
	ex_rep_socket ( argv[1] );
	
	/*A Init pour Acknoledge Alarme */
	ex_init_alarme ( argv[1], argv[0] );


	/*A 
	 * Connexion socket en mode client pour communiquer avec la tache TESRV 
	 */
	if ( ex_cnx_client ( 	XZEXC_ADR_SOCK_TEOPE,
				XZEXC_ADR_SOCK_TESRV,
				&vg_SockMask,
				&vl_SockSRV ) != XDC_OK )
	{
	   /*B Ecriture Trace */
           XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de creer la connexion client  ." );	
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
	/* Messages de controle : XDG_ECMD_OPER_<NomSite> */
	XDG_EncodeDG2 ( pl_NomDG, XDG_ECMD_OPER, vg_NomSite) ;			
   	/* joint ce datagroup */
   	if ( !XDG_JoindreDG( pl_NomDG ) )
   	{
	   /* Affichage Trace */
      	   XZST_03EcritureTrace(XZSTC_WARNING,
	   "Abonnement au Datagroup %s non effectue ", pl_NomDG );
	}			

	/* Messages de demande de lecture infos OPERATOR : XDG_EINIT_ABT_OPER_<NomSite> */
	XDG_EncodeDG2 ( pl_NomDG, XDG_EINIT_ABT_OPER, vg_NomSite) ;			
   	/* joint ce datagroup */
   	if ( !XDG_JoindreDG( pl_NomDG ) )
   	{
	   /* Affichage Trace */
      	   XZST_03EcritureTrace(XZSTC_WARNING,
	   "Abonnement au Datagroup %s non effectue ", pl_NomDG );
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
	   
	      if ( vl_SockRet != XZEXC_NO_EVT )
	      /* Si une socket a ete activee */
	      {
	         /* Selon le type de socket */
	         /* Socket RT server */
	         if ( vl_SockRet == vg_SockRT ) 	
	         {
	            /* Le message est lu par les primitive standard de RTWorks */
	         }
	         else
	         {
	            
                 /* Socket TESRV (Init ou Arret tache) */
	            if ( vl_SockRet == vl_SockSRV )
	            {	
	            /* Appel de la fonction de controle et
	               de configuration de MCFG */
	              ec_controle ( vl_SockSRV, vg_NomMachine );
	            }
	            else
	            {
	            /* Appel analyse des retours traitement de la trame */
	              et_analyser_trame( vl_SockRet );
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


