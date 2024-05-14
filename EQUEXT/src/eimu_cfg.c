/*E*/
/* Fichier : $Id: eimu_cfg.c,v 1.3 2021/10/07 12:09:00 pc2dpdy Exp $     Release : $Revision: 1.3 $      Date :  $Date: 2021/10/07 12:09:00 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEIMU * FICHIER eimu_cfg.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module permetant d'initialiser les structures en memoire
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	01/01/2020 	: Creation 1.1
* CGR	01/02/2021	: Modifications DEM-SAE155 1.2
* ABE	07/10/2021	: Fix lire fichier pb si commentaire DEM-SAE155 1.3
* LCL		20/12/21	: MOVIS P2 ems SAE-312
------------------------------------------------------*/

/* fichiers inclus */
#include <xzsem.h>

#include "eimu_cfg.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */
extern int   vg_SockTEOPC;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction effectue la lecture du fichier XDF_Config_IMU_<NomMachine>
*  et met a jour le liste des structure de donnees IMU;
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ec_lire_config_IMU	( XDY_NomMachine pa_NomMachine )
			  
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
   static char *version = "$Id: eimu_cfg.c,v 1.3 2021/10/07 12:09:00 pc2dpdy Exp $ : ec_lire_config_IMU" ;

   int				vl_ValRet = XDC_OK;
   int				vl_Fd,i,vl_NumEqt = 0;
   char 			pl_PathEtFic[XDC_PATH_ABS_SIZE];
   XDY_Texte			pl_LigneFich;
   XDY_Texte			vl_endPointOPCUA;
   int				vl_NbPICTO = 0;
   EIMU_DONNEES_IMU		vl_DonneesIMU = {0},
		 		*pl_DonCrt;
   short			vl_NumeroLS = 0;
   int				vl_NbParam  = 0;
   XDY_NomST			vl_NomST;
   char				vl_mod[4];
   int				vl_site;
   char 			vl_adr[30];
   XZEXT_MSG_SOCKET             pl_message;
   int				vl_abonnement=1;
   EIMU_DONNEES_IMU		*pl_ListeIMU;
 
      /*A Ouverture du fichier de configuration des IMU */
      /* Construction du nom du fichier Config_IMU_<NomMachine>  */
      sprintf ( pl_PathEtFic, "%s/%s_%s", XZEXC_PATH_CONFIG, XDF_Config_IMU, pa_NomMachine );
      
      /* Ouverture du fichier XDF_Config_IMU_<NomMachine>  */
      if ( (vl_Fd = open (pl_PathEtFic, O_RDONLY)) <= 0 )
      {
         /* Ecriture trace absence fichier */
         XZST_03EcritureTrace( XZSTC_WARNING, " Manque fichier %s. \n", pl_PathEtFic ); 
         /* Sortie de la fonction */
         return ( XDC_NOK );
      }
       
	/*A Lecture de chaque ligne du fichier */
	while ( ex_LireLigneFich ( vl_Fd, pl_LigneFich, sizeof ( pl_LigneFich ) ) != XZEXC_EOF ) 
	{
		/* Lire ligne suivante si c'est une ligne de commentaire */
		if ( *pl_LigneFich != XDF_COMMENTAIRE )
		{
			/*B Extraire la config IMU */
			if ( *pl_LigneFich == 'N' )
			{	       				
				/*B Extraire les parametre de l IMU */
				XZST_03EcritureTrace( XZSTC_WARNING, "ligne fichier =%s", pl_LigneFich);									
				vl_NbParam = sscanf ( pl_LigneFich, "N\t%d\topc.tcp://%s\t%s\t%s\t%s\t%d\t%d\t%s\t%d",
				&vl_DonneesIMU.Config.Numero,							
				vl_endPointOPCUA,
				vl_DonneesIMU.Config.UserOPCUA,
				vl_DonneesIMU.Config.PasswordOPCUA,
				vl_DonneesIMU.Config.NomBICE,
				&vl_DonneesIMU.Config.PortMaitre,
				&vl_DonneesIMU.Config.SousType,
				vl_DonneesIMU.Config.NomMachine,
				&vl_DonneesIMU.Config.SiteGestion);

				vl_DonneesIMU.Config.NumeroEqtCommunication=vl_DonneesIMU.Config.Numero;
				vg_BICE_en_cours_pilotage=-1; //Phase config pas de pilotage

				// un pb avec sscanf et '://' oblige a passer par là
				strcpy(vl_DonneesIMU.Config.EndPointOPCUA,"opc.tcp://");									
				strcat(vl_DonneesIMU.Config.EndPointOPCUA,vl_endPointOPCUA);

				/* Est ce que ce endpoint est déjà présent dans la liste des abonnements */
				for ( pl_ListeIMU = pg_debutIMU; pl_ListeIMU != NULL ; pl_ListeIMU = pl_ListeIMU->Suivant )
				{	
					if (!strcmp(pl_ListeIMU->Config.EndPointOPCUA,vl_DonneesIMU.Config.EndPointOPCUA)&& pl_ListeIMU->Config.EndPointOPCUA!=NULL)	
					{
						/* oui */
						vl_abonnement=0;
						XZST_03EcritureTrace ( XZSTC_WARNING, "Endpoint deja existant dans la liste des IMU.\nEndpoint List :%s\n Endpoint Config :%s",
										pl_ListeIMU->Config.EndPointOPCUA,
										vl_DonneesIMU.Config.EndPointOPCUA);
						//On donne le numéro de l'équipement à qui envoyer les messages pour cette équipement
						vl_DonneesIMU.Config.NumeroEqtCommunication=pl_ListeIMU->Config.Numero; 
						break;												
					}
				}
				/*B Inserer dans la liste des structures config IMU */
          XZST_03EcritureTrace ( XZSTC_WARNING, "Ajout dans liste IMU: num:%d endpoint:%s user:%s pass:%s soustype:%d NomMachine:%s SiteGestion:%d",  
					vl_DonneesIMU.Config.Numero,
					vl_DonneesIMU.Config.EndPointOPCUA,
					vl_DonneesIMU.Config.UserOPCUA,
					vl_DonneesIMU.Config.PasswordOPCUA,
					vl_DonneesIMU.Config.SousType,
					vl_DonneesIMU.Config.NomMachine,
					vl_DonneesIMU.Config.SiteGestion);
				ed_ins_liste_IMU ( &vl_DonneesIMU, sizeof(vl_DonneesIMU.Config) );
				
			}		

			if(vl_abonnement)
			{
				/* connnexion eventuelle a teopc*/
				sprintf(vl_adr,"%s%d",XZEXC_ADR_SOCK_TEOPC,
				XDC_EQT_IMU);
				if (ex_cnx_client_IPV4 (    vl_DonneesIMU.Config.NomMachine,
								vl_DonneesIMU.Config.PortMaitre,
								&vg_SockMask,
								&vg_SockTEOPC))
				{
					XZST_03EcritureTrace( XZSTC_FATAL, "eimu_cfg : Erreur de connexion a l'eqt");
					return(XDC_NOK);
				}

            //ajout de la socket dans la structure
            for ( pl_ListeIMU = pg_debutIMU; pl_ListeIMU != NULL ; pl_ListeIMU = pl_ListeIMU->Suivant )
               if (pl_ListeIMU->Config.Numero==vl_DonneesIMU.Config.Numero)pl_ListeIMU->Socket = vg_SockTEOPC;

				/*demande a TEOPC de connexion a l IMU*/
				sprintf ( pl_message, "%s %d %s %s %s %d %s %c%s",
				XZEXC_CONFIG_IP,
				vl_DonneesIMU.Config.Numero,
				vl_DonneesIMU.Config.EndPointOPCUA,
				vl_DonneesIMU.Config.UserOPCUA,												
				vl_DonneesIMU.Config.PasswordOPCUA,
				vl_DonneesIMU.Config.SousType,
				vl_DonneesIMU.Config.NomMachine,
				XZECC_PROT_OPC,
				XZEXC_FIN_MSG_SOCK );
            sem_wait(&vg_semaphore); 
				if ( write ( vg_SockTEOPC, pl_message, strlen (pl_message) ) == XDC_NOK) 
				{
               sem_post(&vg_semaphore); 
					XZST_03EcritureTrace(XZSTC_WARNING, " connexion impossible");
					return (XDC_NOK);
				}
            sem_post(&vg_semaphore); 
				XZST_03EcritureTrace ( XZSTC_WARNING, "Demande abonnement Message:%s",pl_message);
			}
			vl_abonnement=1;
		}
	}
      /*A Fermeture du fichier Config_IMU_<NomMachine>  */
      XZSS_13FermerFichier( vl_Fd );
      
      /* Ecriture trace de la config pour debug et init de l'etat de service et de l'activite */
      for ( pl_DonCrt = pg_debutIMU; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant )
      {
          pl_DonCrt->Etat_Service  = 0;
          pl_DonCrt->NumCmd = XZEXC_NOCMD;
          XZST_03EcritureTrace ( XZSTC_WARNING, "Config IMU: num:%d endpoint:%s user:%s pass:%s soustype:%d NomMachine:%s SiteGestion:%d Socket:%d",  
					pl_DonCrt->Config.Numero,
					pl_DonCrt->Config.EndPointOPCUA,
					pl_DonCrt->Config.UserOPCUA,
					pl_DonCrt->Config.PasswordOPCUA,
					pl_DonCrt->Config.SousType,
					pl_DonCrt->Config.NomMachine,
					pl_DonCrt->Config.SiteGestion,
               pl_DonCrt->Socket);
					
          strcpy(pl_DonCrt->EtatCmd.ChaineIMU,"");

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
   static char *version = "$Id: eimu_cfg.c,v 1.3 2021/10/07 12:09:00 pc2dpdy Exp $ : ec_controle" ;

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
*   Lecture des fichier de configuration des IMU
*   Remplissage des structures de donnees
*   Connexion socket avec la tache protocole
*
------------------------------------------------------*/
{
   static char *version = "$Id: eimu_cfg.c,v 1.3 2021/10/07 12:09:00 pc2dpdy Exp $ : ec_init" ;

   EIMU_DONNEES_IMU 		*pl_DonCrt;
   XZEXT_MASK *			pl_Socket;
   XZEXT_ADR_SOCK		pl_TacheTrt;
   XZEXT_ADR_SOCK		pl_TacheProt;
   int				vl_SockProt = 0;
   int				vl_Index = 0;
   short			vl_NbEqtLigne[21] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
   char		vl_str[80] ;

   /*A
    * Lecture des fichiers de configuration et remplissage des structures de donnees 
    */
   /*A Configuration des IMU (appel de la fonction ec_lire_config_IMU)*/
   if ( ec_lire_config_IMU ( pa_NomMachine ) != XDC_OK) return (XDC_NOK);
   
   /*A Ouverture du fichier d'alarme */
   ex_OuvertureFichierMsg(vg_NomMachine);
   
   /*A Appel  xzat26_Init_Etats_Eqts pour Initialisation des etats equipement (HS ou Actif) */
   xzat26_Init_Etats_Eqts ( XDC_EQT_IMU, XDC_ETAT_TOUS_EQT, pa_NomMachine );

   /*A
    * Connexion socket en mode client pour communiquer avec la tache TEIMI
    */
   /*A Pour tous les IMU rechercher si la tache protocole associee existe */
   // for ( pl_DonCrt = pg_debutIMU; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant )
   // {
   //    /*A Recherche sur tout le masque des sockets si la socket exite */
   //    for ( pl_Socket = vg_SockMask; pl_Socket != NULL; pl_Socket = pl_Socket->Suivant )
   //    {
	// 	pl_DonCrt->Socket =  ;
   //    }
   // }
   /*A Abonnement pour chaque IMU a ADD START RESET ET KILL */
   for ( pl_DonCrt = pg_debutIMU; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant )
   {
       		/*EnvoyerTrameOPC(EIMUC_TAG_LISTE_ZONES, //OTA
                         XZEXC_TRAME_ABONNEMENT_TAG,
                         pl_DonCrt->Config.Numero,	
                         XZEXC_NOCMD,
	                 XZEXC_PRIORITEFAIBLE,
                         pl_DonCrt->Socket );*/	
		
/*int ex_attendre_msg (
 		int		pa_sd_sock_ect,
 		XZEXT_MASK * 	*pa_masque,
		int		*pa_evenement)
			
int ex_lire_msg (
 		int		va_sd_sock,
		char 		*pa_message,
 		int		*pa_lg_msg,
 		XZEXT_MASK *  	*pa_masque,
		int		*pa_deconnex)*/				
	
      EnvoyerTrameOPC(EIMUC_TAG_ADD,
                         XZEXC_TRAME_ABONNEMENT_TAG,
                         pl_DonCrt->Config.Numero,
                         XZEXC_NOCMD,
	                 XZEXC_PRIORITEFAIBLE,
                         pl_DonCrt->Socket );
		EnvoyerTrameOPC(EIMUC_TAG_RESET,
                         XZEXC_TRAME_ABONNEMENT_TAG,
                         pl_DonCrt->Config.Numero,
                         XZEXC_NOCMD,
	                 XZEXC_PRIORITEFAIBLE,
                         pl_DonCrt->Socket );
		EnvoyerTrameOPC(EIMUC_TAG_START,
                         XZEXC_TRAME_ABONNEMENT_TAG,
                         pl_DonCrt->Config.Numero,
                         XZEXC_NOCMD,
	                 XZEXC_PRIORITEFAIBLE,
                         pl_DonCrt->Socket );
		EnvoyerTrameOPC(EIMUC_TAG_KILL,
                         XZEXC_TRAME_ABONNEMENT_TAG,
                         pl_DonCrt->Config.Numero,
                         XZEXC_NOCMD,
	                 XZEXC_PRIORITEFAIBLE,	
                         pl_DonCrt->Socket );
		EnvoyerTrameOPC(EIMUC_TAG_STARTS,
                         XZEXC_TRAME_ABONNEMENT_TAG,
                         pl_DonCrt->Config.Numero,
                         XZEXC_NOCMD,
	                 XZEXC_PRIORITEFAIBLE,
                         pl_DonCrt->Socket );
		EnvoyerTrameOPC(EIMUC_TAG_KILLS,
                         XZEXC_TRAME_ABONNEMENT_TAG,
                         pl_DonCrt->Config.Numero,
                         XZEXC_NOCMD,
	                 XZEXC_PRIORITEFAIBLE,	
                         pl_DonCrt->Socket );
		EnvoyerTrameOPC(EIMUC_TAG_MESSAGE_EN_COURS,
                         XZEXC_TRAME_ABONNEMENT_TAG,
                         pl_DonCrt->Config.Numero,
                         XZEXC_NOCMD,
	                 XZEXC_PRIORITEFAIBLE,	
                         pl_DonCrt->Socket );
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
   static char *version = "$Id: eimu_cfg.c,v 1.3 2021/10/07 12:09:00 pc2dpdy Exp $ : ec_sortir" ;

   EIMU_DONNEES_IMU 		*pl_IMU, 	*pl_IMUALiberer;
   XZEXT_MASK *			pl_Socket;
   XZEXT_ADR_SOCK		pl_TacheProt,
   				pl_TacheTrt;
   XZEXT_MSG_SOCKET 		pl_message;
   XDY_Datagroup		pl_NomDG;
   int				vl_resultat;

  /*A
   * Liberation des ressources memoires
   */
XZST_03EcritureTrace(XZSTC_WARNING, "eimu_sortir IN");
   /*A Liberation des ressources memoires des donnees des IMU */
   for ( pl_IMU = pg_debutIMU; pl_IMU != NULL; free(pl_IMUALiberer) )
   {
      pl_IMUALiberer = pl_IMU;
      pl_IMU         = pl_IMU->Suivant;
   }  


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

   /*A Fin de la tache */
   exit (0);
}
