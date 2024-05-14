/*E*/
/* Fichier : eini_dir.c
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEINI * FICHIER eini_dir.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module directeur de la tache TEINI >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	26 Sep 1994	: Creation
* Mismer.D 	version 1.2	14 Dec 1994  	: 
* Mismer.D 	version 1.3	23 Jan 1995  	:
* Mismer.D 	version 1.4	25 Jan 1995  	: 
* Mismer.D 	version 1.5	14 Fev 1995  	: 
* Mismer.D 	version 1.6	01 Mar 1995   	:
* Mismer.D	version 1.7	09 Mar 1995	:
* Fontaine.C	version 1.8	19 Mai 1995	: Reprise entete, historique
* Mismer.D  	version 1.9	07 Jun 1995	: Modif connexion socket RTserver
* Mismer.D  	version 1.10	21 Jun 1995	: Modif surveillance LT
* Mismer.D      version 1.11    07 Sep 1995     : Arret des taches ex-fils au demarrage
* Mismer.D      version 1.12    08 Sep 1995     : Relance de tetim en cas de deconnexion
* Mismer.D  	version 1.13	11 Oct 1995	: Ajout appel fonction init Acknoledge alarme
* Mismer.D  	version 1.14	11 Oct 1995	: Correction pour probleme adresse in use
* Mismer.D  	version 1.15	18 Dec 1995	: Ajout filtre avant accept client
* Mismer.D  	version 1.16	26 Avr 1996	: Ajout traitement LT isolee (DEM/1052)
* Mismer.D  	version 1.17	02 Sep 1996	: Ajout type machine (RADT_W) 
* Mismer.D  	version 1.18	15 Jan 1998	: Ajout type machine PC Niveau 2 (DEM/1539) 
* JMG				06/10/04	: portage SS66 (T_MT_DG_STATUS)
* JMG			26/06/10:	portgae linux DEM/934
* JPL		17/03/11 : Migration architecture HP ia64 (DEM 975) : declaration fonctions (fichiers d'en-tete) 1.24
* JPL		17/03/11 : Migration HP ia64 (DEM 975) : interface SS68 : type socket et args. des fonctions IPC  1.25
* JMG		16/05/20 : EMS 1.26
* GR            09/07/2020 : EMS 1.27
* JPL		23/11/21 		: Consultation echeance minuterie et traitement des alarmes en fin de boucle principale (SAE_339)  1.29
* LCL		20/12/21		: MOVIS P2 ems SAE-312
* GGY		12/10/23		: Adaptaion CNA PODFA ou SDCA1 (DEM483)
* GGY		12/10/23		: Generation fichiers eqt + LCR pour CA (DEM483)
------------------------------------------------------*/

/* fichiers inclus */
#include <xzsem.h>

#include "xzag.h"
#include "xdg.h"

#include "ex_msok.h"
#include "ex_mrtf.h"
#include "ex_mala.h"
#ifdef _TIBCO_EMS
#include "xzems.h"
#endif

#include "eini_dir.h"
#include "eini_cfg.h"
#include "eini_syn.h"
#include "eini_lcr.h"

/* definitions de constantes */

/* definitions de types locaux */


/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: eini_dir.c,v 1.28 2020/11/03 18:09:38 pc2dpdy Exp $ : eini_dir" ;

XZEXT_MASK *	vg_SockMask = NULL;
int		vg_SockTETIM	= 0;
int		vg_SockEct	= 0;
int		vg_SockLT 	= 0;
T_SOCK		vg_SockRT	= 0;

XDY_Mot		vg_SitePrincipal = 0;
XDY_NomSite vg_NomSitePrincipal;

/* declaration de fonctions internes */
int el_lancer_tache ( 	char *, XDY_NomMachine );
void ed_arret_tache (   char    * );
void ed_lancer_timer ( void );
void ed_verif_socket ( int  );

/* definition de fonctions externes */
extern void ei_surveillance ( XZEXT_CLE_PROG );


void    ed_syn_cb (     T_IPC_CONN                      va_Cnx,
                        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
			                        T_CB_ARG                        pa_ARG );
						
/*X*/
/*
------------------------------------------------------
* SERVICE RENDU : 
* Programme principal
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int main( argc, argv )

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
*   Connexion au RTServer avec initialisation des variables de
*   supervision, recuperation du site de la machine
*   ainsi que le type de celle-ci.
*   Armements de callbacks.
*   Abonnement au Datagroup XDG_EINIT_NomSite et XDG_ECONFIG_EQPT_NomSite.
*   Constitution des fichier de config equipt pour toutes les machines (appel ed_config)
*
------------------------------------------------------*/

int	argc;
char	**argv;
{
        XZSCT_NomTache          vl_tache;
	XZEXT_MSG_SOCKET	pl_MsgSock;
        XDY_Horodate		vl_Horodate;
        int			vl_Delai;
        XZEXT_CLE_PROG		pl_CleProg;
	int            	      	vl_ValRet 	= XDC_OK,
				vl_SockRet	= 0,
				vl_SockLT	= 0,
				vl_LgMsgSock	= 0,
				vl_IndCnx 	= 0,
				vl_EssaiSrvLT	= 0;
   	XDY_NomSite        	pl_NomSite;
   	XDY_NomMachine       	pl_NomMachine;
   	XDY_Datagroup		pl_NomDG;
	T_IPC_MSG              	vl_Msg;
	
      char *vl_ch;




	/*A
	*  initialisation semaphore
	*/
	sem_init(&vg_semaphore, 0, 1);

	/*A
	 * Connexion au RTserver et initialisation des variables
	 */
	
	if ( XZSC_01InitCnxSup( NULL,
				argv[ 0 ],
				XZSCC_INTERDIRE,
				XZSCC_AUTORISER,
				XZSCC_INTERDIRE,
				ed_arreter,
				"/dev/null" ) != XDC_OK )
	{
	   /*B Affichage Trace */
	   XZST_03EcritureTrace( XZSTC_WARNING, "Ne peut ouvrir la connexion de TEINI avec le RTserver." );
	/*   exit( XDC_NOK );*/
	}
	else
	{
	   /*B Affichage Trace */
	   XZST_03EcritureTrace( XZSTC_DEBUG1, "Ouverture de la connexion de TEINI avec le RTserver." );
	}

	/*A Recuperation du nom de la machine */
	if ( XZSC_07NomMachine(pl_NomMachine) != XDC_OK)
	{
	   /* Ecriture Trace */
	   XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom de la machine impossible . " );
	} 

	/*A Recuperation du type de la machine */
        if ( XZSA_17TypeMachine ( pl_NomMachine, &vg_Type_Machine ) != XDC_OK )
        {
	  XZSA_20InitAdmin();
          if ( XZSA_17TypeMachine ( pl_NomMachine, &vg_Type_Machine ) != XDC_OK )
             XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de trouve le type de la machine" );
        }

	ex_rep_socket ( pl_NomMachine );        
	
	/*A Init pour Acknoledge Alarme */
	XZST_08ForceNivVisuTraceLocal(XZSTC_INTERFACE|XZSTC_WARNING|XZSTC_MESSAGE,XZSTC_NECRAN);
	ex_init_alarme ( pl_NomMachine, argv[0] );

	if ( (vg_Type_Machine != XZSCC_TYPEM_PCS) &&
	     (vg_Type_Machine != XZSCC_TYPEM_PC2) &&
	     (vg_Type_Machine != XZSCC_TYPEM_PC2LT) ) 
	   ed_arret_tache ( XZECC_TACHE_TIM );
	
	/*A Recuperation du site de la machine */
	if ( (vl_ValRet = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
	{
	   /* Ecriture Trace */
           XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom du site impossible." );
	} 

        /* TACLI_XX*/
        if ((vl_ch=strchr(argv[ 0 ], '_')) != NULL) {
                strncpy(pl_NomSite, vl_ch+1,2);
                XZST_03EcritureTrace( XZSTC_WARNING, "SITE FORCE : %s",pl_NomSite);
                strncpy(vl_tache,argv[ 0 ],5);
                strcat(vl_tache,".x");
        }
        else
                strcpy(vl_tache, argv[ 0 ]);

	/*A
	 * Connexion a la base de donnees 
	 */
	
	if ( ( vl_ValRet = XZAG02_Connexion_Base( pl_NomSite ) ) != XDC_OK )
	{
	   XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de se connecter a la base erreur : %d.", vl_ValRet );
	   ed_arreter();
	}

   	/* Pour eviter un CS_BUSY sur le ct_cmd_alloc */
   	sleep(10);

		/* Lecture site principal en base */
	if (XZAO00_SitePrincipal(&vg_SitePrincipal,vg_NomSitePrincipal) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_FATAL,"%s : Site principal non defini en base RES_DIS.",version );
		return XDC_NOK;
	}
	else
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "SitePrincipal = %d, NomSitePrincipal = %s.", vg_SitePrincipal, vg_NomSitePrincipal );
	}

	/*A Connexion socket en mode serveur pour communiquer avec les taches traitements */
	while ( ex_cnx_serveur_LT ( pl_NomMachine, XZEXC_PORT_SERVEUR_SD, &vg_SockMask, &vg_SockLT ) != XDC_OK )
	{
	   /*B Ecriture Trace */
           XZST_03EcritureTrace( XZSTC_DEBUG1, "Impossible de creer la connexion serveur LT." );
           if ( vl_EssaiSrvLT++ == 5 )
           {
               XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de creer la connexion serveur LT dans la minute." );
           }
           if ( vl_EssaiSrvLT > 10 )
           {
               XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de creer la connexion serveur LT dans les 2 minutes." );
               break;
           }
           sleep(10);	
	}

	/*A Connexion socket en mode serveur pour communiquer avec les taches traitements */
	if ( ex_cnx_serveur ( XZEXC_ADR_SOCK_TESRV, &vg_SockMask, &vg_SockEct ) != XDC_OK )
	{
	   /*B Ecriture Trace */
           XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de creer la connexion serveur ." );	
	}
	
        /*A Lancement de la tache TETIM */
        ed_lancer_timer ();
	
	/*A  Armement Callback sur Reception XDM_EINIT */
	if (!TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_EINIT),
		ed_syn_cb,(T_CB_ARG)NULL)) 
	{	
	   /*B Ecriture Trace */
	   XZST_03EcritureTrace( XZSTC_WARNING, "Callback sur XDM_EINIT non declenchee." );
	}

	/*A  Armement Callback sur Reception XDM_ECONFIG_EQPT */
        if (!TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_ECONFIG_EQPT),
		ed_config_cb,(T_CB_ARG)NULL)) 
        {
            /* Ecriture Trace */
            XZST_03EcritureTrace( XZSTC_WARNING, "Callback sur XDM_ECONFIG_EQPT non declenchee.");
        }


	/*A Abonnement au Datagroup XDG_EINIT_NomSite */
	XDG_EncodeDG2 ( pl_NomDG, XDG_EINIT , pl_NomSite) ;			
   	/*A joint ce datagroup */
   	if ( !XDG_JoindreDG( pl_NomDG ) )
   	{
	   /* Affichage Trace */
      	   XZST_03EcritureTrace(XZSTC_WARNING, "Abonnement au Datagroup %s non effectue ", pl_NomDG );
	}			

	/*A Abonnement au Datagroup XDG_ECONFIG_EQPT_NomSite */
	XDG_EncodeDG2 ( pl_NomDG, XDG_ECONFIG_EQPT , pl_NomSite) ;			
   	/*A joint ce datagroup */
   	if ( !XDG_JoindreDG( pl_NomDG ) )
   	{
	   /* Affichage Trace */
      	   XZST_03EcritureTrace(XZSTC_WARNING, "Abonnement au Datagroup %s non effectue ", pl_NomDG );
	}			

	/*A Creation Callback Surveillance */
	/*if ( !TipcSrvProcessCbCreate(TipcMtLookupByNum(T_MT_DG_STATUS), Analyse_Dg_Status, NULL) )*/
	/*TIBCO SS66*/
#ifndef _TIBCO_EMS
	if ( !TipcSrvProcessCbCreate(TipcMtLookupByNum(T_MT_MON_SUBJECT_SUBSCRIBE_STATUS), Analyse_Dg_Status, NULL) )
      	{
            XZST_03EcritureTrace(XZSTC_WARNING, "Echec creation Callback Surveillance" );
	}
#else
	if ( !TipcSrvProcessCbCreate(TipcMtLookupByNum(XDM_TSE_WATCH), Analyse_Dg_Status, NULL) )
      	{
            XZST_03EcritureTrace(XZSTC_WARNING, "Echec creation Callback Surveillance" );
	}
#endif

#ifdef _TIBCO_EMS
    /* timer de surveillance des abonnements*/
      XZSMT_CleProg vl_cle_timer = "";
      XDG_EncodeDG2(pl_NomDG, XDG_WATCH, pl_NomMachine);
      XDG_EncodeDG2(vl_cle_timer, pl_NomDG, "timer");
      XDG_JoindreDG(pl_NomDG);
      if ( XZSM_01DemanderProgrammation(0, 60,pl_NomDG, XDM_TSE_WATCH, vl_cle_timer, 10) != XDC_OK)
      {
         XZST_03EcritureTrace(XZSTC_WARNING, "Erreur demande programmation WATCH \n");
      }
#endif

	
	/*A Constitution des fichier de configuration equipement pour toutes les machines (appel de ed_config) */
	ec_config( XZECC_TOUTES_MACHINES, XZECC_TYPE_EQT_ALL );
	
	
	/*A Attente des messages sur les sockets (RTserver, d'ecoute et de donnees */
	while (1)
	{
	   if ( ex_attendre_msg ( vg_SockEct, &vg_SockMask, &vl_SockRet ) != XDC_OK )
	   {
              XZST_03EcritureTrace(XZSTC_WARNING, "Probleme retour fonction select." );
              ex_ConnexionSocketRTserver ( &vg_SockRT, vl_SockRet, &vg_SockMask );              
              if ( vl_SockRet == vg_SockTETIM ) vg_SockTETIM = 0;
           }
           else
           {
              ex_ConnexionSocketRTserver ( &vg_SockRT, 0, &vg_SockMask );
              if ( vg_SockTETIM == 0 ) ed_lancer_timer ();

	      /*A Si une socket a ete activee */
	      if ( vl_SockRet != XZEXC_NO_EVT )
	      {
	         /*A Si il s'agit de la socket RTserver*/
	         if ( vl_SockRet == vg_SockRT )
	         {
	            /*A Le message est lu par les primitive standard de RTWorks */
	         }

	         /*A Si il s'agit de la socket avec les LT */
	         if ( vl_SockRet == vg_SockLT )
	         {
	            ex_acpt_cnx ( AF_INET, vg_SockLT, &vg_SockMask, &vl_SockLT );
                    ed_verif_socket ( vl_SockLT );
                 }
	         
	         /*A S'il s'agit d'une socket de donnee : lire le message */
	         if ( (vl_SockRet != vg_SockRT) && (vl_SockRet != vg_SockLT) && (vl_SockRet != vg_SockEct) )
	         {
	            /*A Lire le message de la socket de donnees */
	            if ( ex_lire_msg ( vl_SockRet, pl_MsgSock, &vl_LgMsgSock, &vg_SockMask, &vl_IndCnx ) == XDC_OK )
	            {
	               /*A S'il ne s'agit pas d'un message de deconnexion */
	               if ( (vl_IndCnx != XZEXC_DECONNEC) && (vl_LgMsgSock != 0) )
	               {
  	                  if ( strstr( pl_MsgSock, XZEXC_MSG_REVEIL) != NULL)
  	                  {
  	                     sscanf ( &pl_MsgSock[strlen(XZEXC_MSG_REVEIL)], " %lf %d %s ",
  	  					         &vl_Horodate,
  	  					         &vl_Delai,
  	  					         pl_CleProg );
  	  		     ei_surveillance ( pl_CleProg );
  	  		     /*A Connexion socket en mode serveur pour communiquer avec les taches traitements */
  	  		     if ( !vg_SockLT && (ex_cnx_serveur_LT ( pl_NomMachine, XZEXC_PORT_SERVEUR_SD, &vg_SockMask, &vg_SockLT ) != XDC_OK) )
                               XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de creer la connexion serveur LT." );
  	                  }
  	                  else
  	                  {
                             sem_wait(&vg_semaphore); 
                             if ( write (vl_SockRet, pl_MsgSock, strlen(pl_MsgSock)) != strlen(pl_MsgSock) )
                             {
                                sem_post(&vg_semaphore); 
                               ex_fin_cnx ( vl_SockRet, &vg_SockMask );
                             }
                             else
                             {
                              sem_post(&vg_semaphore); 
                             }
  	                  }
	               }
	               else
	               {                          
	                  if ( vl_SockRet == vg_SockTETIM ) vg_SockTETIM = 0;
	               }
	            }
 		 }   /* fin si socket de donnees */
	      }   /* fin si socket active */
	   }   

	   if (XZSS_15ConsulterTimeout () == XDC_OUI)
	   {
	      XZSS_18AnnulerTimeout ();
	      ex_traiter_alarmes ();
	   }
	}   /* fin while */
	
	

}  /* Fin main */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction effectuant la reception des LT.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ed_reception ( void )

/*
* ARGUMENTS EN ENTREE :
*
*  aucun
*
* ARGUMENTS EN SORTIE :
*  aucun
*
* CODE RETOUR :
*  aucun
*
* CONDITION D'UTILISATION
* 
* FONCTION 
*
------------------------------------------------------*/
{
XZEXT_MASK *	 	vl_SockMask = NULL;
XZEXT_MASK *	 	pl_masque = NULL;
XZEXT_MASK	 	vl_masque;
XZEXT_MSG_SOCKET	pl_MsgSock;
XDY_Horodate		vl_Horodate;
int			vl_Delai;
double			vl_Time;
int            	      	vl_SockRet	= 0,
			vl_SockLT	= 0,
			vl_LgMsgSock	= 0,
			vl_IndCnx 	= 0;
    
   /*A Insertion du descripteur de la socket dans le masque de lecture */
   for ( pl_masque = vg_SockMask; pl_masque != NULL; pl_masque = pl_masque->Suivant )
   {
      /*A Si differente de la socket RTserver et differente de la socket d'ecoute */
       if ( (pl_masque->desc_sock != vg_SockRT) && (pl_masque->desc_sock != vg_SockEct) && (pl_masque->desc_sock != vg_SockTETIM) )
       {
          vl_masque.desc_sock = pl_masque->desc_sock;
          vl_masque.Type = pl_masque->Type;
          strcpy ( vl_masque.adresse, pl_masque->adresse );
          ex_ins_masque ( &vl_masque, &vl_SockMask );
       }
    }

   while (1)
   {
      if ( ex_attendre_msg_ST ( XZEXC_SOCKET_NULL, &vl_SockMask, 0.1, &vl_SockRet, &vl_Time ) != XDC_OK )
      {
          XZST_03EcritureTrace(XZSTC_WARNING, "Probleme retour fonction select." );
          ex_ConnexionSocketRTserver ( &vg_SockRT, vl_SockRet, &vg_SockMask );              
          break;
      }
      
      if ( vl_SockRet == XZEXC_NO_EVT )
          break;
      
      /*A Si il s'agit de la socket avec les LT */
      if ( vl_SockRet == vg_SockLT )
      {
        ex_acpt_cnx ( AF_INET, vg_SockLT, &vg_SockMask, &vl_SockLT );
        ed_verif_socket ( vl_SockLT );
      }
	 
      /*A Si une socket a ete activee */
      if ( (vl_SockRet != vg_SockRT) && (vl_SockRet != vg_SockLT) && (vl_SockRet != vg_SockEct) )
      {
	 /*A Lire le message de la socket de donnees */
	 if ( ex_lire_msg ( vl_SockRet, pl_MsgSock, &vl_LgMsgSock, &vg_SockMask, &vl_IndCnx ) != XDC_OK )
	 {
	    return;
	 }
	 if ( (vl_IndCnx != XZEXC_DECONNEC) && (vl_LgMsgSock != 0) )
	 {
            sem_wait(&vg_semaphore); 
            if ( write (vl_SockRet, pl_MsgSock, strlen(pl_MsgSock)) != strlen(pl_MsgSock) )
            {
               sem_post(&vg_semaphore); 
               ex_fin_cnx ( vl_SockRet, &vg_SockMask );
            }
            else
            {
               sem_post(&vg_semaphore); 
            }
 	    
 	 }
      } 
   } 
   
   for ( pl_masque = vl_SockMask; pl_masque != NULL; pl_masque = pl_masque->Suivant )
      ex_sup_masque ( pl_masque->desc_sock, &vl_SockMask);
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction la verif de sockets LT.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ed_verif_socket ( int va_socket )

/*
* ARGUMENTS EN ENTREE :
*
*  aucun
*
* ARGUMENTS EN SORTIE :
*  aucun
*
* CODE RETOUR :
*  aucun
*
* CONDITION D'UTILISATION
* 
* FONCTION 
*
------------------------------------------------------*/
{
XZEXT_MASK *	 	pl_masque = NULL;
XZEXT_MASK	 	vl_masque;
    
        for ( pl_masque = vg_SockMask; pl_masque != NULL; pl_masque = pl_masque->Suivant )
        {
          XZST_03EcritureTrace(XZSTC_DEBUG1, "Socket :%d  Adresse:%s.", pl_masque->desc_sock, pl_masque->adresse );
        }
        for ( pl_masque = vg_SockMask; pl_masque != NULL; pl_masque = pl_masque->Suivant )
        {
          if ( pl_masque->desc_sock == va_socket )
          {
             strcpy ( vl_masque.adresse, pl_masque->adresse ); 
             break;
          }
        }
        while ( pl_masque != NULL )
        {
          for ( pl_masque = vg_SockMask; pl_masque != NULL; pl_masque = pl_masque->Suivant )
          {
             if ( !strcmp ( vl_masque.adresse, pl_masque->adresse) && (pl_masque->desc_sock !=  va_socket) )
             {
                ex_fin_cnx ( pl_masque->desc_sock, &vg_SockMask );
                break;
             }
          }
       }
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction effectuant les operations de terminaison de la tache.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ed_arreter ( void )

/*
* ARGUMENTS EN ENTREE :
*
*  aucun
*
* ARGUMENTS EN SORTIE :
*  aucun
*
* CODE RETOUR :
*  aucun
*
* CONDITION D'UTILISATION
*  Fonction declenchee par SUPERV lors de l'arret de la tache
*  TEINI
* 
* FONCTION 
*  Deconnexion de la base de donnees (appel XZAG03)
*  Fermeture des fichier de configuration
*
------------------------------------------------------*/
{
register int		vl_IndexTache = 0;
	 XZSCT_NomTache	pl_NomTache;
	 XZEXT_MASK *	pl_masque;
         XZEXT_MSG_SOCKET pl_message;


      
      /*A Fermeture des fichier de configuration */
	/*A
	 * Arret des taches equipements et la tache TETIM (appel de ed_arreter_tache_eqpt)
	 */
	/*A Pour toutes les taches memorisees dans le masque */
        for ( pl_masque = vg_SockMask; pl_masque != NULL; pl_masque = pl_masque->Suivant )
        {
           /*A Si differente de la socket RTserver et differente de la socket d'ecoute */
           if ( ( pl_masque->Type == AF_UNIX ) &&
                ( pl_masque->desc_sock != vg_SockEct ) &&
                ( pl_masque->desc_sock != vg_SockTETIM ) )
           {
	      /*A Envoie de la demande d'arret  */
              /*A Construction du message */
              sprintf ( pl_message, "%s %s %s", XZEXC_DEMANDE_ARRET, pl_masque->adresse, XZEXC_FIN_MSG_SOCK );
              /*A Envoie de la demande d'arret */
              sem_wait(&vg_semaphore); 
              if ( write ( pl_masque->desc_sock, pl_message, strlen (pl_message) ) <= 0 )
              { 
                 /*A Ecriture trace impossible d'emettre une demande d'arret */
                 XZST_03EcritureTrace(XZSTC_WARNING, " Impossible d'emettre une demande d'arret a TEINI." );
              }
              sem_post(&vg_semaphore); 

	      /*A Recherche du nom de la tache */
	      strcpy ( pl_NomTache, pl_masque->adresse );
	      strcat ( pl_NomTache, ".x" );
	      /*A Attente tache terminee */
	      while ( XZSS_20PidTache ( pl_NomTache ) != 0 )
	      {
	         sleep ( XZEXC_ATTENTE_ARRET_TACHE );
	      }
	   }     
           ex_fin_cnx ( pl_masque->desc_sock, &vg_SockMask );
 	 }   /* fin for */	 

	 
      /*A Deconnexion de la base de donnees */
      XZAG03_Deconnexion_Base();
      
      XZST_03EcritureTrace(XZSTC_WARNING,"Sortie de tache TEINI .");
      /*A Arret de la tache */
      exit ( 0 );
      	


}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Fonction effectuant le lancement de la tache tetim.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void    ed_lancer_timer ( void )

/*
* ARGUMENTS EN ENTREE :
*
*  aucun
*
* ARGUMENTS EN SORTIE :
*  aucun
*
* CODE RETOUR :
*  aucun
*
* CONDITION D'UTILISATION
*
* FONCTION
*
------------------------------------------------------*/
{
        XZEXT_MSG_SOCKET        pl_MsgTimer;
        int			vl_SockRet = 0;
   	XDY_NomMachine       	pl_NomMachine;

	/*A Recuperation du nom de la machine */
	if ( XZSC_07NomMachine(pl_NomMachine) != XDC_OK)
	{
	   /* Ecriture Trace */
	   XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom de la machine impossible . " );
	} 
	
        /*A Lancement de la tache TETIM */
        if ( (vg_Type_Machine != XZSCC_TYPEM_PCS) &&
	     (vg_Type_Machine != XZSCC_TYPEM_PC2) &&
	     (vg_Type_Machine != XZSCC_TYPEM_PC2LT) )
        {
           ed_arret_tache ( XZECC_TACHE_TIM );
           if ( el_lancer_tache ( XZEXC_TACHE_TETIM, pl_NomMachine  ) != XDC_OK )
           {
              XZST_03EcritureTrace(XZSTC_WARNING,"Impossible lancer tache TETIM ." );
           } 
        }

	/*A Attente lancement tache TETIM */
        sleep ( 10 );  
	ex_attendre_msg ( vg_SockEct, &vg_SockMask, &vl_SockRet );
	/*A 
	 * Connexion socket en mode client pour communiquer avec la tache TETIM 
         */
	if ( ex_cnx_client ( XZEXC_ADR_SOCK_TEINI,
		             XZEXC_ADR_SOCK_TETIM_SRV,
			     &vg_SockMask,
			     &vg_SockTETIM ) != XDC_OK )
	{
	   /*B Ecriture Trace */
           XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de creer la connexion client TETIM ." );	
	}	
	else
	{
           XZST_03EcritureTrace( XZSTC_WARNING, "Connexion etablie avec TETIM ." );
	}

        if ( vg_SockTETIM )
        {
           /*A Demande de reveil pour time_out connexion serveur */
           sprintf ( pl_MsgTimer, "%s %lf %d %s %s",
                		XZEXC_DEMANDE_REVEIL,
               			(double)0,
                		60,
                		"TEINI_Timer_60s",
                		XZEXC_FIN_MSG_SOCK );
      sem_wait(&vg_semaphore); 
	   if ( write ( vg_SockTETIM, pl_MsgTimer, strlen (pl_MsgTimer) ) <= 0 )
              XZST_03EcritureTrace ( XZSTC_WARNING, "Impossible d'emettre un message sur la socket Timer." );
      sem_post(&vg_semaphore); 
	}

}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ed_syn_cb (	T_IPC_CONN 			va_Cnx,
        		T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        		T_CB_ARG                        pa_ARG )
 
/*
* ARGUMENTS EN ENTREE :
*  Message RTworks
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*  Fonction appelee lors de la reception du message
*  XDM_EINIT
*
* FONCTION 
*  Appel de MSYN en donnant le nom de la machine (une ou toutes) et le type de configuration
*
------------------------------------------------------*/
{
   XDY_NomMachine		pl_NomMachine;
   XDY_NomMachine		pl_NomMachineSup;

#ifndef _TIBCO_EMS
    T_STR			vl_NomMachine;
    T_INT4			vl_TypeMachine;
    T_INT4 			vl_TypeEqt;		 
#else
    XDY_NomMachine		vl_NomMachine;
    XDY_Entier			vl_TypeMachine;
    XDY_Entier 			vl_TypeEqt;		 
#endif   


   XZST_03EcritureTrace(XZSTC_WARNING, "IN: ed_syn_cb ");
		  
   /*A Recuperation des parametres du message XDM_EINIT */
   TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_STR,  &vl_NomMachine,
               T_IPC_FT_INT4, &vl_TypeEqt,
               T_IPC_FT_INT4, &vl_TypeMachine,
	       NULL); 
	       
   strcpy ( pl_NomMachine, vl_NomMachine );

   switch (vl_TypeMachine)
   {
      case XZEXC_MODE_NORMAL :
         if ( ex_presence_machine ( pl_NomMachine ) == XDC_OK )
         {
            XZST_03EcritureTrace(XZSTC_WARNING, "Fin mode isole sur %s", pl_NomMachine );
            ei_traitement_mode_isole (pl_NomMachine, XDC_OK ); 
         }
         break;
      
      case XZEXC_MODE_ISOLE :
/*         if ( ex_presence_machine ( pl_NomMachine ) != XDC_OK )*/
         {
            XZST_03EcritureTrace(XZSTC_WARNING, "Debut mode isole sur %s", pl_NomMachine );
            ei_traitement_mode_isole (pl_NomMachine, XDC_NOK ); 
         }
         break;

      case XZEXC_RECONFIG_LCR :
		 XZST_03EcritureTrace(XZSTC_WARNING, "Debut reconfig fichier LCR pour %s et type %d", pl_NomMachine, vl_TypeEqt );

	      ec_config_eqt_lcr ( pl_NomMachine, vl_TypeEqt );
         break;
      
      default :
         XZST_03EcritureTrace(XZSTC_WARNING, "Debut diffusion %s type equipement %d (pour %s)", 
   	      				      pl_NomMachine, 
                              vl_TypeEqt,
   					      (vl_TypeMachine == XZECC_MACHINE_PRINC) ? "TESRV" : "TESUR");
         /*A Appel ec_diffusion_fichier avec en parametre le nom de la machine passe en parametre */
         ec_diffusion_fichier ( pl_NomMachine, vl_TypeMachine, vl_TypeEqt );
         XZST_03EcritureTrace(XZSTC_WARNING, "Fin   diffusion %s", pl_NomMachine );
         break;
   }
   
   XZST_03EcritureTrace(XZSTC_WARNING, "OUT: ed_syn_cb ");

}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Appel du module de MALL et des modules de Mzzz selon le message XDM_ECONFIG_EQPT.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ed_config_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG)
 
/*
* ARGUMENTS EN ENTREE :
*  Message RTworks
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*  Fonction appelee lors de la reception du message
*  XDM_ECONFIG_EQPT
*
* FONCTION 
*  Appel de MALL et appel sequentiel des modules Mzzz selon le message specifie
*  Appel de MSYN en donnant le nom de la machine (une ou toutes) et le type de configuration
*
------------------------------------------------------*/
{
   XDY_NomMachine		pl_NomMachine;

#ifndef _TIBCO_EMS
    T_STR			vl_NomMachine;
    T_INT4 			vl_TypeEqt;		 
#else
    XDY_NomMachine		vl_NomMachine;
    XDY_Entier 			vl_TypeEqt;		 
#endif
   
   XZST_03EcritureTrace(XZSTC_FONCTION, "IN: ed_config_cb ");
		  
   /*A Recuperation des parametres du message XDM_ECONFIG_EQPT */
   TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_STR,  &vl_NomMachine,
               T_IPC_FT_INT4, &vl_TypeEqt,
	       NULL); 
	       
   strcpy ( pl_NomMachine, vl_NomMachine );
   
   /*A Appel de ec_config */
   XZST_03EcritureTrace(XZSTC_WARNING, "Debut de reconfig de %s Eqt:%d",pl_NomMachine,vl_TypeEqt);
   ec_config (  pl_NomMachine, vl_TypeEqt ); 
   XZST_03EcritureTrace(XZSTC_WARNING, "Fin   de reconfig de %s Eqt:%d",pl_NomMachine,vl_TypeEqt);
       
   XZST_03EcritureTrace(XZSTC_FONCTION, "OUT: ed_config_cb ");
	       
}	       


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction permettant de creer une tache quelconque dont le nom est specifie
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int el_lancer_tache ( 	char 		*pa_NomTache,
		 	XDY_NomMachine 	pa_NomMachine )

/*
* ARGUMENTS EN ENTREE :
*  pa_NomTache		Nom de la tache a creer
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*  			succes PID de la tache
*  XDC_NOK		echec
*
* CONDITION D'UTILISATION
*  
*
* FONCTION 
*   Creation de la tache TEzzz specifie en entree
*
------------------------------------------------------*/

{
  int			vl_ValRet 	= XDC_OK;
  XDY_NomTache		vl_NomTache	= "";
  int			vl_cr		= 0;
  static char		*vl_argvt[3]; 	/* tableau de pointeurs de strings */
  char                  vl_Commande [XZSSC_LINE_CMD_SIZE] = "";
  FILE*                 vl_Cmd          = NULL;
  int                   vl_Pid          = 0;

        sprintf ( vl_NomTache, "%s.x", pa_NomTache );
        if ( !strcmp(pa_NomTache, XZEXC_TACHE_TETIM) )
        {
           while ( (vl_Pid = XZSS_20PidTache(vl_NomTache)) != 0 )
          {
             /* Arret de la tache  */
             XZST_03EcritureTrace( XZSTC_INFO, "Kill de <%s>", vl_NomTache );
             sprintf ( vl_Commande, "kill %d", vl_Pid );
             if ( (vl_Cmd = popen (vl_Commande, "r"))!= NULL ) pclose ( vl_Cmd );
             sleep (1);
           }
        }
	
        /*A Construction du nom complet de la tache */
        chdir ( XZEXC_PATH_TACHE_EQUEXT );
	/*A Creation de la commande de lancement de la tache */
	vl_argvt[0] = vl_NomTache;		/* nom de la commande 		$0 */
	vl_argvt[1] = pa_NomMachine;		/* nom de la machine		$1 */
	vl_argvt[2] = "";			/* argument 2  de la commande	$2 */

	/*A lance la tache par le principe du fork unix */

	/*vl_cr = vfork();*/
	/*TIBCO SS66*/
	vl_cr = fork();

	if (vl_cr == 0 )
	{  /* process fils */
	   execvp( vl_argvt[0], vl_argvt );
           exit(0);
	}

	/*A Lecture du PID de la tache */
   	if ( XZSS_20PidTache ( vl_NomTache ) == 0 )
   	{
            XZST_03EcritureTrace( XZSTC_WARNING, "Echec du fork pour '%s'", vl_NomTache );
            ex_rep_socket ( pa_NomMachine );
	    return ( XDC_NOK );
   	}
	
       ex_rep_socket ( pa_NomMachine );
       return( XDC_OK );
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Fonction permettant d'arret une tache quelconque dont le nom est specifie
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ed_arret_tache (   char    *pa_NomTache )

/*
* ARGUMENTS EN ENTREE :
*  pa_NomTache          Nom de la tache a arreter
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
*                       succes PID de la tache
*  XDC_NOK              echec
*
* CONDITION D'UTILISATION
*
*
* FONCTION
*   Arret de la tache TEzzz specifie en entree
*
------------------------------------------------------*/

{
  XDY_NomTache          vl_NomTache;
  char                  vl_Commande [XZSSC_LINE_CMD_SIZE] = "";
  FILE*                 vl_Cmd = NULL;
  int                   vl_Pid = 0;

   sprintf ( vl_NomTache, "%s%s.x", XZECC_TACHE_EQUEXT, pa_NomTache );
        
   /*A Lecture du PID de la tache */
   if ( (vl_Pid = XZSS_20PidTache(vl_NomTache)) != 0 )
   {
       /* Arret de la tache  */
       XZST_03EcritureTrace( XZSTC_INFO, "Kill de <%s>", vl_NomTache );
       sleep (XZEXC_ATTENTE_ARRET_TACHE);
       sprintf ( vl_Commande, "kill %d", vl_Pid );
       if ( (vl_Cmd = popen (vl_Commande, "r"))!= NULL ) pclose ( vl_Cmd );
   }

}
        
