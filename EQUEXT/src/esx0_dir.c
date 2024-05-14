/*E*/
/* Fichier : esx0_dir.c
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TESX0 * FICHIER esx0_dir.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Module directeur de la tache TESX0, protocole de la
*   tache TESAT.
*
------------------------------------------------------
* HISTORIQUE :
*
* Gabert.P	version 1.1	20 Dec 1994	: Creation
* Volcic.F	version 1.2	13 Fev 1995	:
* Volcic.F	version 1.3	15 Mar 1995	: Modif Diverses
* Fontaine.C	version 1.4	23 Mai 1995	: Reprise entete, historique
* Volcic.F	version 1.5	02 Jun 1995	: Modif deconnection tesat
* Mismer.D  	version 1.6	31 Aou 1995	: Modif connexion socket RTserver 
* Volcic.F	version 1.5	05 Dec 1995	: Ajout ex_init_alarme
* JPL	23/11/21 : Consultation echeance minuterie et traitement des alarmes en fin de boucle principale (SAE_339)  1.8
------------------------------------------------------*/

/* fichiers inclus */

#include <xzsem.h>
#include "esx0_dir.h"

#define CC_DEBUG_SANS_RT_SERVEUR

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: esx0_dir.c,v 1.7 1995/12/13 15:04:33 volcic Exp $ : esx0_dir" ;
int           vg_SockSAT 	= 0;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
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
* TETIM, TESAT Transfert des fichiers SATIR --> SAE
*                                     SAE   --> SATIR
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
				vl_SockTIM	= 0,
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

	/*A Creation du repertoire pour les sockets */
	ex_rep_socket ( argv[1] );

	/*A Init pour Acknoledge Alarme */
	ex_init_alarme ( argv[1], argv[0] );

	/*A 
	 * Connexion socket en mode client pour communiquer avec la tache TETIM 
	 */
	if ( ex_cnx_client ( 	XZEXC_ADR_SOCK_SX0TIM,
				XZEXC_ADR_SOCK_TETIM_SRV,
				&vg_SockMask,
				&vl_SockTIM ) != XDC_OK )
	{
	   /*B Ecriture Trace */
           XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de creer la connexion client  .");
	}	

	/*A 
	 * Connexion socket en mode serveur pour communiquer avec la tache TESAT 
	 */
        if ( ex_cnx_serveur (   XZEXC_ADR_SOCK_SX0SRV,
				&vg_SockMask,
				&vg_SockSAT ) != XDC_OK )
        {
          /*B Ecriture Trace */
          XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de creer la connexion serveur \n");
        }
 

	/*A
	 * Demande des timers cycliques
	 */
	 
	/* Programation timer 10s */
	ETIM_REVEIL_CYCLIQUE ( ESXC_REVEIL_10S, "SX_Timer_10s", vl_SockTIM, pl_MsgTimer )
	/* Programation timer 30s */
	ETIM_REVEIL_CYCLIQUE ( ESXC_REVEIL_30S, "SX_Timer_30s", vl_SockTIM, pl_MsgTimer )


	/* Programation de l'heure de la synchro date */
	/* ETIM_SYNCHRO_DATE ( XZEXC_HEURE_SYNCHRO , "SX_Timer_Synchro", vl_SockTIM, pl_MsgTimer ) */
	ETIM_SYNCHRO_DATE ( "18:13:00", "SX_Timer_Synchro", vl_SockTIM, pl_MsgTimer )
		
	
	/*A Attente des messages sur les sockets (RTserver, d'ecoute et de donnees */

	while (1)
	{
	   
	   if ( ex_attendre_msg ( vg_SockSAT , &vg_SockMask, &vl_SockRet ) != XDC_OK )
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
	            if ( vl_SockRet == vl_SockTIM )
	            {	
	               /* Appel de la fonction ec_traitement_cyclique de MCYC */
	               ey_traitement_cyclique ( vl_SockTIM );
	            }
	            else
	            {
	                if ( vl_SockRet != vg_SockSAT )
			{
	                  /* Appel traitement de la trame */
	                  em_traitement_trame( vl_SockRet );
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


