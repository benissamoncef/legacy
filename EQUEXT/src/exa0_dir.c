/*E*/
/*Fichier : $Id: exa0_dir.c,v 1.6 1995/09/08 15:21:29 volcic Exp $      Release : $Revision: 1.6 $        Date : $Date: 1995/09/08 15:21:29 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEXA0 * FICHIER exa0_dir.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Module directeur de la tache TEXA0, protocole de la
*  gtc Nice
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	22 Nov 1994	: Creation
* Mismer.D	version 1.2	09 Fev 1995	:
* Mismer.D	version 1.3	09 Mai 1995	:
* Mismer.D	version 1.4	10 Mai 1995	:
* Mismer.D	version 1.5	22 Mai 1995	: Reprise entete, historique
* Mismer.D	version 1.6	25 Aou 1995	: Modif gestion liaison AprilNet sur echec
*
------------------------------------------------------*/

/* fichiers inclus */

#include <xzsem.h>
#include "exa0_dir.h"
#include "xzsc.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: exa0_dir.c,v 1.6 1995/09/08 15:21:29 volcic Exp $ : exa0_dir" ;
extern XZSCT_NomTache		XZSCV_NomTache;
extern int			XZSCV_InitOk;
extern XZSTT_NiveauTrace	XZSTV_NiveauTrace;
extern int			XZSCV_TailleFicTrace;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/* Fichier : $Id: exa0_dir.c,v 1.6 1995/09/08 15:21:29 volcic Exp $      Release : $Revision: 1.6 $        Date : $Date: 1995/09/08 15:21:29 $
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
* Boucle sur les sockets et gestion des messages recus 
* 
------------------------------------------------------*/

int	argc;
char	**argv;
{
	int 			vl_ValRet	=XDC_OK,          	      	
				vl_SockRet	= 0,
				vl_resultat;
	XZEXT_MASK		vl_masque;
   	XZEXT_MASK *		pl_Socket;
	
	
	/*A
	 *  Recuperation du nom de la machine 
	 */
	strcpy(vg_NomMachine,argv[1]);

	/*A
	*  initialisation semaphore
	*/
	sem_init(&vg_semaphore, 0, 1);

	/*A Creation du repertoire pour les sockets */
	ex_rep_socket ( argv[1] );


	/*A
	* connexion soket en mode serveur pour comms. avec la tache traitement TENIC
	*/
	/*A SI connexion echoue */
	if( ex_cnx_serveur( 	XZEXC_ADR_SOCK_TEXA0_SRV, 
				&vg_SockMask, 
				&vg_SockSrvApr ) != XDC_OK )
	{
	    /*A ALORS */
	    /*B ecriture trace : " Connexion socket ecoute mode serveur impossible !" */
	    XZST_03EcritureTrace( XZSTC_FATAL, " Connexion socket ecoute mode serveur impossible !" );
	    /*B sortie XDC_NOK */
	    exit( XDC_NOK );
	}
	
	/*A 
	 * Appel de MCFG fonction ea_init 
	 */
        ex_ecriture_trace_cmd ( XDC_EQT_NIC, 0, "Lancement tache protocole AprilNET", NULL, 0 );
        ea_init(vg_NomMachine, argv[0] );
        


	/*A 
	 * Init de la Socket avec le coupleur AprilNet 
	 */
	vg_SockApril = vg_SockLms1[0].fd;
	/* Si socket April creee */
	if ( vg_SockApril > 0 )
	{
	   /* Insertion de la socket April dans le masque des socket */
	   vl_masque.desc_sock = vg_SockApril;
	   strcpy ( vl_masque.adresse, "April" );
	   ex_ins_masque ( &vl_masque, &vg_SockMask );
	}
	else
      	{
            XZST_03EcritureTrace(XZSTC_WARNING, "Echec recuperation socket April." );
            sleep ( 5 );
	}
	
	
	/*A Attente des messages sur les sockets (RTserver, d'ecoute et de donnees */
		
	while (1)
	{
	   
	   if ( ex_attendre_msg ( vg_SockSrvApr, &vg_SockMask, &vl_SockRet ) != XDC_OK )
	   {
              if ( (vl_SockRet == vg_SockApril) && (vg_SockApril != 0) )
              {
                 XZST_03EcritureTrace(XZSTC_WARNING, "Probleme retour fonction select sur socket AprilNet ." );
                 ex_fin_cnx (vg_SockApril, &vg_SockMask);
                 vg_SockApril = 0;
                 ea_sortir ();
              }
              else
              {
                 XZST_03EcritureTrace(XZSTC_WARNING, "Probleme retour fonction select sur socket %d .", vl_SockRet);
              }
              
	   }
	   else
	   {

              if ( vg_SockApril <= 0 )
              {
                 ea_init(vg_NomMachine, NULL );
           	 vg_SockApril = vg_SockLms1[0].fd;
	         /* Si socket April creee */
	         if ( vg_SockApril > 0 )
	         {
	            /* Insertion de la socket April dans le masque des socket */
	            vl_masque.desc_sock = vg_SockApril;
	            strcpy ( vl_masque.adresse, "April" );
	            ex_ins_masque ( &vl_masque, &vg_SockMask );
      		    ea_init_Flag_April();
	         }
	         else
      	         {
                    XZST_03EcritureTrace(XZSTC_WARNING, "Echec recuperation socket April." );
                    sleep ( 5 );
	         }
              }
              
              
	      /* Si une socket a ete activee */
	      if ( vl_SockRet != XZEXC_NO_EVT )
	      {
	         /* Selon le type de socket */
	         /* Socket April */
	         if ( vl_SockRet == vg_SockApril )
	         {	
	            /* Traiter les reponses AprilNet Manager */
	            lm_receive (MOD_NOWAIT);
	         }
	         else
	         {
	            /* Socket TENIC ( commande tache traitement ) */
	            if ( vl_SockRet == vg_SockSrvApr )
	            {
                       for ( pl_Socket = vg_SockMask; pl_Socket != NULL; pl_Socket = pl_Socket->Suivant )
                       {
                          if ( !strcmp ( pl_Socket->adresse, XZEXC_ADR_SOCK_NICXA0 ) )
                          {
                             /* Mise a jour du numero de socket */
                             vg_SockTENIC = pl_Socket->desc_sock;
                             break;
                          }
                       }
                    }
                    else
                    {
	               /* Appel de la fonction traitement des commandes */
	               ea_traitement_cmd ( vl_SockRet );
	            }
	         }

	      }   /* fin si socket active */
	   }   
	}   /* fin while */
	



}  /* Fin main */








int XZSC_InitCnxSup( char *NomApplication,
		       XZSCT_NomTache NomTache,
                       int AutoriserTime,
                       int AutoriserTrace,
                       int AutoriserSynchro,
                       void (*FonctionUtil)(),
		       char *DestiStd)
{

   int			vl_IndiceMsg	= 0;
   XZSCT_Datagroup	DGMachineTache 	= "\0";
   int			vl_ValRet 	= XDC_OK;
   char *		pl_P ;

	XZST_03EcritureTrace(XZSTC_INTERFACE,
		"   XZSC_01InitCnxSup : in  \
		Nom Appli %s -- Nom Tache %s -- Autoriser Time %d -- \
		Autoriser Trace  %d -- Autoriser Synchro %d -- \
		Std out %s \n",NomApplication, NomTache, AutoriserTime,
		AutoriserTrace, AutoriserSynchro, DestiStd);


   if(XZSCV_InitOk == XDC_FAUX)
   {
      if (NomTache != NULL)
      {
	 /* eventuellement, ne recupere que la partie */
         strncpy(XZSCV_NomTache
	        ,((pl_P = strrchr(NomTache,'/')) == NULL ? NomTache:pl_P+1)
		,sizeof(XZSCV_NomTache)-1
		) ;

	 /* Initialisation des variables globales de description
	    du noyau de supervision de la tache */
	 vl_ValRet = XZSC_InitDescNoyau();

         /* Redirection de la sortie standard et de la sortie d'erreur
	    standard */
         if ((vl_ValRet == XDC_OK) && (DestiStd != NULL))
	 {
	    if((freopen(DestiStd, "w", stdout) == NULL) ||
		(freopen(DestiStd, "w", stderr) == NULL))
            {
	       vl_ValRet = XZSCC_SYS_INV;
            }
         }

	 /* Affectation de l'indicateur d'initialisation */
         if(vl_ValRet == XDC_OK)
	 {
	    XZSCV_InitOk = XDC_VRAI;
         }

	 /* Mise en place eventuelle du systeme de trace */
	 if ((vl_ValRet == XDC_OK) && (AutoriserTrace == XZSCC_AUTORISER))
	 {
	    XZSCV_TailleFicTrace = 50000;
            vl_ValRet = XZST_01OuvertureTrace();
            XZSTV_NiveauTrace = XZSTC_FATAL | XZSTC_WARNING | XZSTC_INFO;
         }

      }
      else
      {
	 /* Nom de la tache indefini */
	 vl_ValRet = XZSCC_NOT_INV;
      }
   }
   else
   {
      /* initialisation deja effectuee */
      vl_ValRet = XZSCC_INI_INV;
   }


   return(vl_ValRet);

}	/* Fin XZSC_01InitCnxSup */








