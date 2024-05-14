/*E*/
/* Fichier : $Id: epal_cfg.c,v 1.6 2021/01/20 12:22:34 akkaakka Exp $     Release : $Revision: 1.6 $      Date :  $Date: 2021/01/20 12:22:34 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEPAL * FICHIER epal_cfg.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module permetant d'initialiser les structures en memoire
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	14 Jan 1998	: Creation
* Mismer.D      version 1.2     29 avr 2004     : Ajout pilotage par scenario
* JMG   13/11/07        : ajout site de gestion DEM715
* PNI   17/07/12        : nb eqt ligne passe � 21 (au lieu de 19) V1.4
* JMg	14/06/19	: IP 1.5
* PNI 20/01/20 : correctif IP 1.6
* GGY 14/06/23 :  Correction ec_init pour IP (DEM471)
------------------------------------------------------*/

/* fichiers inclus */
#include <xzsem.h>

#include "epal_cfg.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */
extern int   vg_SockTEMIP;

/* declaration de fonctions internes */

/* definition de fonctions externes */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction effectue la lecture du fichier XDF_Config_Module_PAL_<NomMachine>
*  et met a jour le liste des structure de donnees type PAL;
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ec_lire_config_Module	( EPAL_CONFIG_PAL * va_Tab_Module, XDY_TypeEqt va_NumType )
			  
/*
* ARGUMENTS EN ENTREE :
*   va_NumType	: Numero du type de BRA
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
   static char *version = "$Id: epal_cfg.c,v 1.6 2021/01/20 12:22:34 akkaakka Exp $ : ec_lire_config_Module" ;

}






/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction effectue la lecture du fichier XDF_Config_PAL_<NomMachine>
*  et met a jour le liste des structure de donnees PAL;
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ec_lire_config_PAL	( XDY_NomMachine		pa_NomMachine )
			  
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
   static char *version = "$Id: epal_cfg.c,v 1.6 2021/01/20 12:22:34 akkaakka Exp $ : ec_lire_config_PAL" ;

   int				vl_ValRet = XDC_OK;
   int				vl_Fd,i,vl_NumEqt = 0;
   char 			pl_PathEtFic[XDC_PATH_ABS_SIZE];
   XDY_Texte			pl_LigneFich;
   int				vl_NbPICTO = 0;
   EPAL_DONNEES_PAL		vl_DonneesPAL = {0},
   				*pl_DonCrt;
   short			vl_NumeroLS = 0;
   int				vl_NbParam  = 0;
   XDY_NomST			vl_NomST;
   char				vl_mod[4];
   int				vl_site;
   char vl_adr[30];
        XZEXT_MSG_SOCKET             pl_message;
 
      /*A Ouverture du fichier de configuration des PICTO */
      /* Construction du nom du fichier XDF_Config_PAL_<NomMachine>  */
      sprintf ( pl_PathEtFic, "%s/%s_%s", XZEXC_PATH_CONFIG, XDF_Config_PAL, pa_NomMachine );
      
      /* Ouverture du fichier XDF_Config_PAL_<NomMachine>  */
      if ( (vl_Fd = open (pl_PathEtFic, O_RDONLY)) <= 0 )
      {
         /* Ecriture trace absence fichier */
         XZST_03EcritureTrace( XZSTC_WARNING, " Manque fichier %s. \n", pl_PathEtFic ); 
         /* Sortie de la fonction */
         return ( XDC_NOK );
      }
      /*A Marquage des structures afin de determiner les eventuelles suppression */
      for ( pl_DonCrt = pg_debutPAL; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant )
      {
      	 /* Effacement du numero de ST pour marquage */
         strcpy ( pl_DonCrt->Config.NumeroST, " ");
      }
       
      /*A Lecture de chaque ligne du fichier */
      while ( ex_LireLigneFich ( vl_Fd, pl_LigneFich, sizeof ( pl_LigneFich ) ) != XZEXC_EOF ) 
      {
         /* Lire ligne suivante si c'est une ligne de commentaire */
         if ( *pl_LigneFich != XDF_COMMENTAIRE )
         {
	    /*B Extraire la config PAL */
	    if ( *pl_LigneFich == 'N' )
	    {	       
               
               /*B Inserer dans la liste des structures config PAL */
               ed_ins_liste_PAL ( &vl_DonneesPAL, sizeof(vl_DonneesPAL.Config) );

               /* Mise en memoire PAL */

               /*B Extraire les parametre du PAL */

               vl_NbParam = sscanf ( pl_LigneFich, 	"N %hd %s %hd %3c %hd %d %s %d",  
					&vl_DonneesPAL.Config.Numero,
					vl_DonneesPAL.Config.NumeroST,
					&vl_NumeroLS,
					vl_DonneesPAL.Config.AdresseRGS,
					&vl_DonneesPAL.Config.TypePAL,
					&vl_site,
					vl_DonneesPAL.Config.AdresseIP,
               &vl_DonneesPAL.Config.Port);
	       vl_DonneesPAL.Config.SiteGestion=vl_site;
               vl_DonneesPAL.Config.NumeroLS = vl_NumeroLS;
            }
            
         /*}
      }*/
         
      /*B Inserer dans la liste des structures config PAL */
      ed_ins_liste_PAL ( &vl_DonneesPAL, sizeof(vl_DonneesPAL.Config) );
                    /* connnexion eventuelle a temip*/
                    if (strcmp(vl_DonneesPAL.Config.AdresseIP,XZECC_IP_VIDE)) {
                        /*PMV IP*/
                        if (vg_SockTEMIP==-1) {
                                sprintf(vl_adr,"%s%d",XZEXC_ADR_SOCK_TEIM,
                                                        XDC_EQT_PAL);
                                ex_cnx_client (    XZEXC_ADR_SOCK_TEPAL,
                                                vl_adr,
                                                &vg_SockMask,
                                                 &vg_SockTEMIP);
                        }
                        /*demande a TEMIP de connexion au PAL*/
                        sprintf ( pl_message, "%s%s %d %c%s",
                                XZEXC_CONFIG_IP,
                                vl_DonneesPAL.Config.AdresseIP,
                                vl_DonneesPAL.Config.Port,
                                XZECC_PROT_LCR,
                                XZEXC_FIN_MSG_SOCK );
                        sem_wait(&vg_semaphore); 
                        if ( write ( vg_SockTEMIP, pl_message, strlen (pl_message) ) == XDC_NOK) {
                              sem_post(&vg_semaphore); 
                                XZST_03EcritureTrace(XZSTC_WARNING, " connexion impossible");
                                return (XDC_NOK);
                        }
                        sem_post(&vg_semaphore); 
                    }
      }
   }
      /*A Fermeture du fichier XDF_Config_PAL_<NomMachine>  */
      XZSS_13FermerFichier( vl_Fd );
      
      /*A Suppression de la liste des donnees des elements ne faisant plus partie de la config */
      for ( pl_DonCrt = pg_debutPAL; pl_DonCrt != NULL; )
      {
         /* Si le numero de ST est effacer */
         if ( strlen ( pl_DonCrt->Config.NumeroST ) <= 1 )
         {
            /* Suppression de la liste */
            ed_sup_liste_PAL ( pl_DonCrt->Config.Numero );
            pl_DonCrt = pg_debutPAL;
         }
         else
         {
            /* Passage a l'element suivant */
            pl_DonCrt = pl_DonCrt->Suivant;
         }
      }
     
      /* Ecriture trace de la config pour debug et init de l'etat de service et de l'activite */
      for ( pl_DonCrt = pg_debutPAL; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant )
      {
          pl_DonCrt->Etat_Service  = 0;
          pl_DonCrt->Contraste = XDC_CONTRASTENORMAL;
          pl_DonCrt->ContrasteCmd = XZEXC_NOCMD;
          pl_DonCrt->NumCmd = XZEXC_NOCMD;
          XZST_03EcritureTrace ( XZSTC_WARNING, "Config PAL: %d %s %d %s %d %d %d %s %d",  
					pl_DonCrt->Config.Numero,
					pl_DonCrt->Config.NumeroST,
					pl_DonCrt->Config.NumeroLS,
					pl_DonCrt->Config.AdresseRGS,
					pl_DonCrt->Config.TypePAL,
					pl_DonCrt->Etat_Service,
					pl_DonCrt->Config.SiteGestion,
					pl_DonCrt->Config.AdresseIP,
					pl_DonCrt->Config.Port);
					
          pl_DonCrt->EtatCmd[0] = '\0';
         strcpy ( pl_DonCrt->Config.Module[0], "1.1");
	 strcpy ( pl_DonCrt->Config.Module[1], "1.2");
	 strcpy ( pl_DonCrt->Config.Module[2], "1.3");
	 strcpy ( pl_DonCrt->Config.Module[3], "1.4");
	 strcpy ( pl_DonCrt->Config.Module[4], "2.1");
	 strcpy ( pl_DonCrt->Config.Module[5], "2.2");
	 strcpy ( pl_DonCrt->Config.Module[6], "2.3");
	 strcpy ( pl_DonCrt->Config.Module[7], "2.4");
	 strcpy ( pl_DonCrt->Etat[0], "0");
	 strcpy ( pl_DonCrt->Etat[1], "0");
	 strcpy ( pl_DonCrt->Etat[2], "0");
	 strcpy ( pl_DonCrt->Etat[3], "0");
	 strcpy ( pl_DonCrt->Etat[4], "0");
	 strcpy ( pl_DonCrt->Etat[5], "0");
	 strcpy ( pl_DonCrt->Etat[6], "0");
	 strcpy ( pl_DonCrt->Etat[7], "0");

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
   static char *version = "$Id: epal_cfg.c,v 1.6 2021/01/20 12:22:34 akkaakka Exp $ : ec_controle" ;

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
*   Lecture des fichier de configuration des PAL et des Picto
*   Remplissage des structures de donnees
*   Connexion socket avec la tache protocole
*
------------------------------------------------------*/
{
   static char *version = "$Id: epal_cfg.c,v 1.6 2021/01/20 12:22:34 akkaakka Exp $ : ec_init" ;

   EPAL_DONNEES_PAL 		*pl_DonCrt;
   XZEXT_MASK *			pl_Socket;
   XZEXT_ADR_SOCK		pl_TacheTrt;
   XZEXT_ADR_SOCK		pl_TacheProt;
   int				vl_SockProt = 0;
   int				vl_Index = 0;
   short			vl_NbEqtLigne[21] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

   /*A
    * Lecture des fichiers de configuration et remplissage des structure de donnees 
    */
   /*A Configuration des PAL (appel de la fonction ec_lire_config_PAL)*/
   if ( ec_lire_config_PAL ( pa_NomMachine ) != XDC_OK) return (XDC_NOK);
   /*A Configuration des clignotements */
    ex_lire_cligno ( pa_NomMachine, vg_clignolent, vg_clignorapide );
   /*A Lecture user/password */
    ex_lire_idpassw ( pa_NomMachine, vg_idpassw );
   
   /*A Ouverture du fichier d'alarme */
   ex_OuvertureFichierMsg(vg_NomMachine);
   
   /*A Appel  xzat26_Init_Etats_Eqts pour Initialisation des etats equipement (HS ou Actif) */
   xzat26_Init_Etats_Eqts ( XDC_EQT_PAL, XDC_ETAT_TOUS_EQT, pa_NomMachine );

   	/*A
   	 * Connexion socket en mode client pour communiquer avec la tache TELMi
   	 */
   	/*A Pour tout les PAL rechercher si la tache protocole associee existe */
   	for ( pl_DonCrt = pg_debutPAL; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant )
   	{
      	/* Construction du nom de la tache protocole telmi*/
      	sprintf ( pl_TacheProt, "%s%d", XZEXC_ADR_SOCK_TELM, pl_DonCrt->Config.NumeroLS );
      	sprintf ( pl_TacheTrt, "%s%d", XZEXC_ADR_SOCK_PALTELM, pl_DonCrt->Config.NumeroLS );
      	vl_NbEqtLigne[pl_DonCrt->Config.NumeroLS]++;

      	if (strcmp(pl_DonCrt->Config.AdresseIP,XZECC_IP_VIDE)) 
      	{
	 		pl_DonCrt->Socket = vg_SockTEMIP;
		}

      	/*A Recherche sur tout le masque des socket si la socket exite */
      	for ( pl_Socket = vg_SockMask; pl_Socket != NULL; pl_Socket = pl_Socket->Suivant )
      	{
         	if (!strcmp(pl_DonCrt->Config.AdresseIP,XZECC_IP_VIDE)) 
         	{
            	if ( !strcmp ( pl_Socket->adresse, pl_TacheProt ) )
            	{
               		/* Mise a jour du numero de socket */
               		pl_DonCrt->Socket = pl_Socket->desc_sock;
               		/*B Arret: la socket avec la tache existe */
               		break;
            	}
        	}
      	}
      
      	/*A Si la socket avec la tache telmi n'existe pas */
      	if (!strcmp(pl_DonCrt->Config.AdresseIP,XZECC_IP_VIDE)) 
      	{
         	if ( pl_Socket == NULL )
         	{
           		/*A Appel de ex_cnx_client pour etablir la liaison avec la tache protocole */
           		if ( ex_cnx_client ( 	pl_TacheTrt,
              		pl_TacheProt,
              		&vg_SockMask,
              		&vl_SockProt ) != XDC_OK )
            	{
               		/*B Ecriture Trace */
               		XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de creer la connexion client  Trt=[%s] Prot=[%s].", pl_TacheTrt, pl_TacheProt);	
            	}
            	else
            	{
              		/* Memorisation du numero de socket */
               		pl_DonCrt->Socket = vl_SockProt;
            	}	
         	}
      	}	   
   	}

   /*A Initialisation memoire */
   /*A Pour tous les pmv et les picto -> demande d'etat d'affichage */
   for ( pl_DonCrt = pg_debutPAL; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant )
   {
	if (!strcmp(pl_DonCrt->Config.AdresseIP,XZECC_IP_VIDE))
       		EnvoyerTrame(     "PS",
                         XZEXC_TRAME_ETAT_POS_PAL,
                         pl_DonCrt->Config.AdresseRGS,
                         XZEXC_NOCMD,
	                 XZEXC_PRIORITEFAIBLE,
                         pl_DonCrt->Socket );
	else
       		EnvoyerTrameIP(     "PS",
                         XZEXC_TRAME_ETAT_POS_PAL,
                         pl_DonCrt->Config.AdresseIP,
                         pl_DonCrt->Config.Port,
                         pl_DonCrt->Config.AdresseRGS,
                         XZEXC_NOCMD,
	                 XZEXC_PRIORITEFAIBLE,
                         pl_DonCrt->Socket );
   }
   
   /*A
    * Calcul du nombre maxi de BRA par ligne
    */
   for ( vl_Index = 0; vl_Index < 21; vl_Index ++) 
      if ( *va_MaxEqtLigne < vl_NbEqtLigne[vl_Index] ) *va_MaxEqtLigne = vl_NbEqtLigne[vl_Index];
   
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
   static char *version = "$Id: epal_cfg.c,v 1.6 2021/01/20 12:22:34 akkaakka Exp $ : ec_sortir" ;

   EPAL_DONNEES_PAL 		*pl_PAL, 	*pl_PALALiberer;
   XZEXT_MASK *			pl_Socket;
   XZEXT_ADR_SOCK		pl_TacheProt,
   				pl_TacheTrt;
   XZEXT_MSG_SOCKET 		pl_message;
   XDY_Datagroup		pl_NomDG;
   int				vl_resultat;

   /*A Desabonnement a JourNuit */
   XDG_EncodeDG2 ( pl_NomDG, "Einit_JourNuit_PAL", vg_NomMachine) ;			
   /*XZEA29_Abt_Etat_JourNuit(XZEAC_FERMER_ABONNEMENT,pl_NomDG,ep_journuit,vg_NomSite,&vl_resultat);*/

  /*A
   * Liberation des ressources memoires
   */

   /*A Liberation des ressources memoires des donnees des PAL */
   for ( pl_PAL = pg_debutPAL; pl_PAL != NULL; free(pl_PALALiberer) )
   {
      pl_PALALiberer = pl_PAL;
      pl_PAL         = pl_PAL->Suivant;
   }  


   /*A
    * Envoie du message d'arret aux taches protocoles 
    */
    
   for ( pl_Socket = vg_SockMask; pl_Socket != NULL; pl_Socket = pl_Socket->Suivant )
   {
      if ( !strncmp ( pl_Socket->adresse, XZEXC_ADR_SOCK_TELM, strlen(XZEXC_ADR_SOCK_TELM) ) )
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

   /*A Fin de la tache */
   exit (0);
}
