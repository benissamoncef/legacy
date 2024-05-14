/* Fichier : ecfe_cfg.c
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TECFE * FICHIER ecfe_cfg.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module permetant d'initialiser les structures en memoire
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	04/03/18 : Creation controleurs de feux CTRL_FEUX 1.1 DEM1284
* JPL	11/07/23 : Initialisation du mode de fonctionnement CFE (SAE-440)  1.2
------------------------------------------------------*/

/* fichiers inclus */

#include "ecfe_cfg.h"
#include "ex_mdia.h"

#include <xzsem.h>

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */
static char *version = "ecfe_cfg.c 1.2" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */





/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction effectue la lecture du fichier XDF_Config_CFE_<NomMachine>
*  et met a jour le liste des structure de donnees CFE;
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ec_lire_config_CFE	( XDY_NomMachine		pa_NomMachine )
			  
/*
* ARGUMENTS EN ENTREE :
*   pa_NomMachine	: Nom de la machine
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK	succes
*   XDC_NOK	echec
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   
*
------------------------------------------------------*/
{
   int				vl_ValRet = XDC_OK;
   int				vl_Fd,i,vl_NumEqt = 0;
   char 			pl_PathEtFic[XDC_PATH_ABS_SIZE];
   XDY_Texte			pl_LigneFich;
   int				vl_NbPICTO = 0;
   ECFE_DONNEES_CFE		vl_DonneesCFE = {0},
   				*pl_DonCrt;
   short			vl_NumeroLS = 0;
   int				vl_NbParam  = 0;
   XDY_NomST			vl_NomST;
   char				vl_mod[4];
   int				vl_site;
  
      /*A Ouverture du fichier de configuration des PICTO */
      /* Construction du nom du fichier XDF_Config_CFE_<NomMachine>  */
      sprintf ( pl_PathEtFic, "%s/%s_%s", XZEXC_PATH_CONFIG, XDF_Config_CFE, pa_NomMachine );
      
      /* Ouverture du fichier XDF_Config_CFE_<NomMachine>  */
      if ( (vl_Fd = open (pl_PathEtFic, O_RDONLY)) <= 0 )
      {
         /* Ecriture trace absence fichier */
         XZST_03EcritureTrace( XZSTC_WARNING, " Manque fichier %s. \n", pl_PathEtFic ); 
         /* Sortie de la fonction */
         return ( XDC_NOK );
      }
      /*A Marquage des structures afin de determiner les eventuelles suppression */
      for ( pl_DonCrt = pg_debutCFE; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant )
      {
      	 /* Effacement du numero de ST pour marquage */
         strcpy ( pl_DonCrt->Config.AdresseIP, " ");
      }
       
      /*A Lecture de chaque ligne du fichier */
      while ( ex_LireLigneFich ( vl_Fd, pl_LigneFich, sizeof ( pl_LigneFich ) ) != XZEXC_EOF ) 
      {
         /* Lire ligne suivante si c'est une ligne de commentaire */
         if ( *pl_LigneFich != XDF_COMMENTAIRE )
         {
	    /*B Extraire la config CFE */
	    if ( *pl_LigneFich == 'N' )
	    {	       
               
               /*B Inserer dans la liste des structures config CFE */
               ed_ins_liste_CFE ( &vl_DonneesCFE, sizeof(vl_DonneesCFE.Config) );

               /* Mise en memoire CFE */

               /*B Extraire les parametre du CFE */

               vl_NbParam = sscanf ( pl_LigneFich, 	"N %hd %s %hd %d",  
					&vl_DonneesCFE.Config.Numero,
					vl_DonneesCFE.Config.AdresseIP,
					&vl_DonneesCFE.Config.Port,
					&vl_site);
	       vl_DonneesCFE.Config.SiteGestion=vl_site;
            }
            
         }
      }
         
      /*B Inserer dans la liste des structures config CFE */
      ed_ins_liste_CFE ( &vl_DonneesCFE, sizeof(vl_DonneesCFE.Config) );
      
      /*A Fermeture du fichier XDF_Config_CFE_<NomMachine>  */
      XZSS_13FermerFichier( vl_Fd );
      
      /*A Suppression de la liste des donnees des elements ne faisant plus partie de la config */
      for ( pl_DonCrt = pg_debutCFE; pl_DonCrt != NULL; )
      {
         /* Si le numero de ST est effacer */
         if ( strlen ( pl_DonCrt->Config.AdresseIP ) <= 1 )
         {
            /* Suppression de la liste */
            ed_sup_liste_CFE ( pl_DonCrt->Config.Numero );
            pl_DonCrt = pg_debutCFE;
         }
         else
         {
            /* Passage a l'element suivant */
            pl_DonCrt = pl_DonCrt->Suivant;
         }
      }
     
      /* Ecriture trace de la config pour debug et init de l'etat de service et de l'activite */
      for ( pl_DonCrt = pg_debutCFE; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant )
      {
          pl_DonCrt->Etat_Service  = 0;
          XZST_03EcritureTrace ( XZSTC_WARNING, "Config CFE: %d %s %d %d %d",  
					pl_DonCrt->Config.Numero,
					pl_DonCrt->Config.AdresseIP,
					pl_DonCrt->Config.Port,
					pl_DonCrt->Etat_Service,
					pl_DonCrt->Config.SiteGestion);
					
          pl_DonCrt->EtatCmd = 0;
	  pl_DonCrt->Etat = XDC_CFE_PLN_NUL;
	  pl_DonCrt->Mode_Fct = XDC_CFE_FCT_NUL;
	  strcpy(pl_DonCrt->Libelle,"");

      }			
      /* Retourne compte rendu OK */
      return ( vl_ValRet );
}






/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet selon message socket recu de tesrv 
*    - soit de terminer la tache
*    - soit de configurer la tache
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ec_controle	( 	int 		va_NumSock,
				XDY_NomMachine	pa_NomMachine )

/*
* ARGUMENTS EN ENTREE :
*   va_NumSock		: Numero de la socket de TESRV
*   pa_NomMachine	: Nom de la machine
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*
* FONCTION 
*   Lecture du message socket
*   Si message d'init alors lancer la fonction ec_init
*   Si message d'arret alors lancer la fonction ec_sortir
*
------------------------------------------------------*/
{
   int			vl_IndCnx = 0,
   			vl_LgMsg  = 0;
   XZEXT_MSG_SOCKET	pl_Msg;
   XDY_Mot		vl_MaxEqtLigne = 0;
   
   
   /*A
    *  Lecture du message recu dans la socket appelante
    */
	 
   if ( ex_lire_msg ( va_NumSock, pl_Msg, &vl_LgMsg, &vg_SockMask, &vl_IndCnx ) != XDC_OK )
   {
      /* Ecriture trace Erreur lecture message socket. */
      XZST_03EcritureTrace( XZSTC_WARNING, " Erreur lecture message socket. " );
   }   
   else
   {
      /*A Si il s'agit d'une deconnexion */
      if ( vl_IndCnx == XZEXC_DECONNEC )
      {
         /*B Arreter la tache par appel de ec_sortir */
         ec_sortir ();
      }
   
      /* Si le message est correct  */
      if ( vl_LgMsg != 0 ) 
      {
  	  /*A Si il s'agit d'une demande d'arret */
  	  if ( strstr ( pl_Msg, XZEXC_DEMANDE_ARRET ) != NULL )
  	  {
  	     /*A Arreter la tache par appel de ec_sortir */
  	     ec_sortir ();
  	  }

  	  /*A Si il s'agit d'une demande d'Init */
  	  if ( strstr ( pl_Msg, XZEXC_DEMANDE_INIT ) != NULL )
  	  {
  	     /*A Reconfiguration de la tache par appel de ec_init */
  	     ec_init ( pa_NomMachine, &vl_MaxEqtLigne );
  	  }
  	  /*A Si il s'agit du changement du mode de fonctionnement */
  	  if ( strstr ( pl_Msg, XZEXC_MODE_FONCT_NORMAL ) != NULL )
  	  {
  	     /*A Maj de la variable en question */
  	     vg_Mode_Fonct = XZEXC_MODE_NORMAL;
  	     ex_RecopieFichierMsg(pa_NomMachine );
  	  }
  	  if ( strstr ( pl_Msg, XZEXC_MODE_FONCT_ISOLE ) != NULL )
  	  {
  	     /*A Maj de la variable en question */
  	     vg_Mode_Fonct = XZEXC_MODE_ISOLE;
  	  }
       }
    }
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Effectue l'initialisation de la tache
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ec_init	( XDY_NomMachine  	pa_NomMachine, XDY_Mot	*va_MaxEqtLigne )

/*
* ARGUMENTS EN ENTREE :
*
*   pa_NomMachine	: Nom de la machine
*
* ARGUMENTS EN SORTIE :
*   va_MaxEqtLigne	: Nombre maxi d'equipement par ligne serie
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*
* FONCTION 
*   Appel de ex_mini
*   Lecture des fichier de configuration des CFE et des Picto
*   Remplissage des structures de donnees
*   Connexion socket avec la tache protocole
*
------------------------------------------------------*/
{
   ECFE_DONNEES_CFE 		*pl_DonCrt;
   XZEXT_MASK *			pl_Socket;
   XZEXT_ADR_SOCK		pl_TacheTrt;
   XZEXT_ADR_SOCK		pl_TacheProt;
   int				vl_SockProt = 0;
   int				vl_Index = 0;
   short			vl_NbEqtLigne[21] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
 XZEXT_MSG_SOCKET             pl_message;

   /*A
    * Lecture des fichiers de configuration et remplissage des structure de donnees 
    */
   /*A Configuration des CFE (appel de la fonction ec_lire_config_CFE)*/
   if ( ec_lire_config_CFE ( pa_NomMachine ) != XDC_OK) return (XDC_NOK);
   
   /*A Ouverture du fichier d'alarme */
   ex_OuvertureFichierMsg(vg_NomMachine);
   
   /*A Appel  xzat26_Init_Etats_Eqts pour Initialisation des etats equipement (HS ou Actif) */
   xzat26_Init_Etats_Eqts ( XDC_EQT_CFE, XDC_ETAT_TOUS_EQT, pa_NomMachine );

   /*A
    * Connexion socket en mode client pour communiquer avec la tache TELMi
    */
   /*A Pour tout les CFE rechercher si la tache protocole associee existe */
   for ( pl_DonCrt = pg_debutCFE; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant )
   {
	    /* Memorisation du numero de socket */
	    pl_DonCrt->Socket = vg_SockTEMIP;

         /* Construction du message d'init de temip pour chaque eqt */
            sprintf ( pl_message, "%s%s %d %c%s",
                                XZEXC_CONFIG_IP,
				pl_DonCrt->Config.AdresseIP,
				pl_DonCrt->Config.Port,
				XZECC_PROT_DIASER,
                                XZEXC_FIN_MSG_SOCK );

         /* Envoie de la demande d'arret */
         sem_wait(&vg_semaphore); 
         if ( write ( vg_SockTEMIP, pl_message, strlen (pl_message) ) == XDC_NOK )
         { 
            /* Ecriture trace impossible d'emettre une demande d'arret */
             XZST_03EcritureTrace(XZSTC_WARNING, " Impossible d'emettre une demande d'arret." );
         }
         sem_post(&vg_semaphore); 
}
   /*A Initialisation memoire */
   /*A Pour tous les pmv et les picto -> demande d'etat d'affichage */
   for ( pl_DonCrt = pg_debutCFE; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant )
   {
       EnvoyerTrameDiaser(     "DI",
                         XZEXC_TRAME_ETAT_POS_CFE,
                         pl_DonCrt->Config.AdresseIP,
				pl_DonCrt->Config.Port,
                         XZEXC_NOCMD,
	                 XZEXC_PRIORITEFAIBLE,
                         vg_SockTEMIP );
   }
   
   return(XDC_OK);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Effectue les operation de terminaison de la tache
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ec_sortir	(  )

/*
* ARGUMENTS EN ENTREE :
*   aucun
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*
* FONCTION 
*   Liberation des ressources memoires
*   Envoyer le message aux taches protocoles
*   Attendre x secondes
*   Fermeture des sockets
*
------------------------------------------------------*/
{
   ECFE_DONNEES_CFE 		*pl_CFE, 	*pl_CFEALiberer;
   XZEXT_MASK *			pl_Socket;
   XZEXT_ADR_SOCK		pl_TacheProt,
   				pl_TacheTrt;
   XZEXT_MSG_SOCKET 		pl_message;
   XDY_Datagroup		pl_NomDG;
   int				vl_resultat;

  /*A
   * Liberation des ressources memoires
   */

   /*A Liberation des ressources memoires des donnees des CFE */
   for ( pl_CFE = pg_debutCFE; pl_CFE != NULL; free(pl_CFEALiberer) )
   {
      pl_CFEALiberer = pl_CFE;
      pl_CFE         = pl_CFE->Suivant;
   }  


   /*A
    * Envoie du message d'arret aux taches protocoles 
    */
    
   for ( pl_Socket = vg_SockMask; pl_Socket != NULL; pl_Socket = pl_Socket->Suivant )
   {
      if ( !strncmp ( pl_Socket->adresse, XZEXC_ADR_SOCK_TEIM, strlen(XZEXC_ADR_SOCK_TEIM) ) )
      {
         /* Construction du message d'arret */
            sprintf ( pl_message, "%s%s%s",
                                XZEXC_DEMANDE_ARRET,
                                pl_Socket->adresse,
                                XZEXC_FIN_MSG_SOCK );

         /* Envoie de la demande d'arret */
         sem_wait(&vg_semaphore); 
         if ( write ( pl_Socket->desc_sock, pl_message, strlen (pl_message) ) == XDC_NOK )
         { 
            /* Ecriture trace impossible d'emettre une demande d'arret */
             XZST_03EcritureTrace(XZSTC_WARNING, " Impossible d'emettre une demande d'arret." );
         }
         sem_post(&vg_semaphore); 

      }
   }
   
   /*A Attendre */
   sleep (XZEXC_ATTENTE_ARRET_TACHE);
   
   /*A
    * Fermeture de toutes les sockets 
    */
	 
   /* Pour toute les sockets du masque */
   for ( pl_Socket = vg_SockMask; pl_Socket != NULL; pl_Socket = pl_Socket->Suivant )
   {
      /* Si differente de la socket RTserver */
      if ( pl_Socket->desc_sock != vg_SockRT ) 
      {
         /* Fermeture de la socket courante */
         ex_fin_cnx ( pl_Socket->desc_sock, &vg_SockMask );
      }   
   }

   XZAG03_Deconnexion_Base();

   /*A Fin de la tache */
   exit (0);
}


