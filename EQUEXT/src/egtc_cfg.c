/*E*/
/*Fichier : $Id: egtc_cfg.c,v 1.11 2011/03/17 14:53:08 gesconf Exp $       Release : $Revision: 1.11 $     Date : $Date: 2011/03/17 14:53:08 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEGTC * FICHIER egtc_cfg.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module d'init et de reinit, de controle et d'exit pour 
* la tache TEGTC
*
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E	version 1.1	16 Dec 1994	: Creation
* Nagiel.E	version 1.2	22 Dec 1995	:
* Nagiel.E	version 1.3	22 Dec 1995	: RAS
* Mismer.D	version 1.4	26 Jan 1995	:
* Volcic.F	version 1.5	09 Avr 1995	: Modif Init Etat ETOR 
* Fontaine.C	version 1.6	19 Mai 1995	: Reprise entete, historique
* Mismer.D	version 1.7	07 Jun 1995	: Modif condition prise en compte nouvelle config
* Mismer.D	version 1.8	08 Aou 1995	: Initialisation num alarme et alerte
* Mismer.D	version 1.9	10 Oct 1995	: Ajout appel xzat26 pour ETOR
* Mismer.D	version 1.10 	15 Nov 1996	: Modif pour alerte DAI tunnel (DEM/1309)
*
* JPL		17/03/11 : Migration architecture HP ia64 (DEM 975) : adequation format scanf et type d'entier  1.11
------------------------------------------------------*/

/* fichiers inclus */
#include <xzsem.h>

#include "xzat.h"

#include "ex_mrtf.h"

#include "egtc_cfg.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction effectue la lecture du fichier XDF_Config_RAU_<NomMachine>
*  et met a jour le liste des structure de donnees RAU;
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ec_lire_config_GTC	( XDY_NomMachine		pa_NomMachine )
			  
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
static char *version = "$Id: egtc_cfg.c,v 1.11 2011/03/17 14:53:08 gesconf Exp $ : ec_lire_config_GTC" ;

   int				vl_ValRet = XDC_OK;
   int				vl_Fd;
   char 			pl_PathEtFic[XDC_PATH_ABS_SIZE];
   XDY_Texte			pl_LigneFich;
   
   int				vl_poub1,vl_numetor,vl_numeqtetor,vl_NbParam ;
   int				vl_alarme,vl_alerte,vl_Index ;
   char			        vl_nom[XDC_Lon_NomEqt];
   char *			vl_debutnom;
  
      /*A Ouverture du fichier de configuration de la gtc */
      /* Construction du nom du fichier XDF_Config_GTC_<NomMachine>  */
      sprintf ( pl_PathEtFic, "%s/%s_%s", XZEXC_PATH_CONFIG, XDF_Config_GTC, pa_NomMachine );
      
      /* Ouverture du fichier XDF_Config_GTC_<NomMachine>  */
      if ( (vl_Fd = open (pl_PathEtFic, O_RDONLY)) <= 0 )
      {
         /* Ecriture trace absence fichier */
         XZST_03EcritureTrace( XZSTC_WARNING, " Manque fichier %s.", pl_PathEtFic ); 
         /* Sortie de la fonction */
         return ( XDC_NOK );
      }
 
      for ( vl_Index = 0; vl_Index < XDC_NBMAX_ETOR ; vl_Index++ )
      {
          vg_donneesGTC.ETOR[vl_Index].Alarme	 = 0;
	  vg_donneesGTC.ETOR[vl_Index].Alerte	 = 0;
	  vg_donneesGTC.ETOR[vl_Index].NoEqtETor = 0;
	  strcpy ( vg_donneesGTC.ETOR[vl_Index].Nom, "" );
      }
      				
      /*A Lecture de chaque ligne du fichier */
      while ( ex_LireLigneFich ( vl_Fd, pl_LigneFich, sizeof ( pl_LigneFich ) ) != XZEXC_EOF ) 
      {
          /* Lire ligne suivante si c'est une ligne de commentaire */
          if ( *pl_LigneFich != XDF_COMMENTAIRE )
          {  
             
	     /*B Si c'est une ligne de config RAU */
             if (*pl_LigneFich=='G')
             {
                /*B Extraire les parametres GTC */
                vl_NbParam = sscanf ( pl_LigneFich, "G %hd %s %hd %3c" 
                                            	  , &vg_donneesGTC.Numero
                                            	  , vg_donneesGTC.NumeroST
                                            	  , &vg_donneesGTC.NumeroLS
                                            	  , vg_donneesGTC.AdresseRGS);
	     
             }

             
	     /*B Si c'est une ligne de config pau */
             if (*pl_LigneFich=='E')
             {
                /*B Extraire les parametres pau */
                vl_NbParam = sscanf ( pl_LigneFich, 	"E %d %d %d %d %d %s",
                                              &vl_poub1,
                                              &vl_numetor,
                                              &vl_numeqtetor,
                                              &vl_alarme,
                                              &vl_alerte,
                                              vl_nom);
                vg_donneesGTC.ETOR[vl_numetor].NoEqtETor=vl_numeqtetor;
                vg_donneesGTC.ETOR[vl_numetor].Alarme=vl_alarme;
                vg_donneesGTC.ETOR[vl_numetor].Alerte=vl_alerte;
                vl_debutnom = strstr(pl_LigneFich,vl_nom);
                strcpy(vg_donneesGTC.ETOR[vl_numetor].Nom,vl_debutnom);
             }
             
         }
      }
      /*A Fermeture du fichier XDF_Config_GTC_<NomMachine>  */
      XZSS_13FermerFichier( vl_Fd );
      
          
      /* Ecriture trace de la config pour debug */
      vg_donneesGTC.Etat_Service = 0;
      XZST_03EcritureTrace ( XZSTC_DEBUG3, "Config GTC: %d %s %d %s %d",  
					vg_donneesGTC.Numero,
					vg_donneesGTC.NumeroST,
					vg_donneesGTC.NumeroLS,
					vg_donneesGTC.AdresseRGS,
					vg_donneesGTC.Etat_Service);				
                                            	  
      for ( vl_Index = 0; vl_Index < XDC_NBMAX_ETOR ; vl_Index++ )
      {
	  vg_donneesGTC.ETOR[vl_Index].Etat = (vl_Index < XDC_NB_ETOR) ? EGTCC_INIT_ETAT_TOR : 0;
	  vg_donneesGTC.ETOR[vl_Index].Etat_Service = 0;
          XZST_03EcritureTrace ( XZSTC_DEBUG3, "Config ETOR: %d %d %d %s",  
					vg_donneesGTC.ETOR[vl_Index].Alarme,
					vg_donneesGTC.ETOR[vl_Index].Alerte,
					vg_donneesGTC.ETOR[vl_Index].NoEqtETor,
					vg_donneesGTC.ETOR[vl_Index].Nom);				
      }
      /* Retourne compte rendu OK */
      return ( XDC_OK );
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
static char *version = "$Id: egtc_cfg.c,v 1.11 2011/03/17 14:53:08 gesconf Exp $ : ec_controle";
   int			vl_IndCnx = 0,
   			vl_LgMsg  = 0;
   XZEXT_MSG_SOCKET	pl_Msg;
   
   
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
  	     ec_init ( pa_NomMachine );
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

int	ec_init	( XDY_NomMachine  	pa_NomMachine )

/*
* ARGUMENTS EN ENTREE :
*
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
*   Appel de ex_mini
*   Lecture des fichier de configuration des PMV et des Picto
*   Remplissage des structures de donnees
*   Connexion socket avec la tache protocole
*
------------------------------------------------------*/
{
static char *version = "$Id: egtc_cfg.c,v 1.11 2011/03/17 14:53:08 gesconf Exp $ : ec_init";

   XZEXT_MASK *			pl_Socket;
   XZEXT_ADR_SOCK		pl_TacheTrt;
   XZEXT_ADR_SOCK		pl_TacheProt;
   int				vl_SockProt = 0;

   /*A
    * Lecture des fichiers de configuration et remplissage des structure de donnees 
    */
   /*A Configuration de la gtc (appel de la fonction ec_lire_config_GTC ) */
    if (ec_lire_config_GTC ( pa_NomMachine ) == XDC_NOK) return (XDC_NOK);

   /*A Lecture du password de la machine (user/password) */
   if ( ex_lire_idpassw ( pa_NomMachine, vg_idpassw ) != XDC_OK )
   {
   	XZST_03EcritureTrace(XZSTC_WARNING,"%s: ex_lire_idpassw retourne NOK", version );
	return (XDC_NOK);
   }
   XZST_03EcritureTrace( XZSTC_WARNING, "Pwd:<%s>.", vg_idpassw );	

   /*A Ouverture du fichier d'alarme */
   ex_OuvertureFichierMsg(vg_NomMachine);
   
   /*A
    * Connexion socket en mode client pour communiquer avec la tache TELMi
    */
   /*A Pour la gtc rechercher si la tache protocole associee existe */
   
      /* Construction du nom de la tache protocole telmi*/
      sprintf ( pl_TacheProt, "%s%d", XZEXC_ADR_SOCK_TELM, vg_donneesGTC.NumeroLS );
      sprintf ( pl_TacheTrt, "%s%d", XZEXC_ADR_SOCK_GTCTELM, vg_donneesGTC.NumeroLS );
      /*A Recherche sur tout le masque des socket si la socket exite */
      for ( pl_Socket = vg_SockMask; pl_Socket != NULL; pl_Socket = pl_Socket->Suivant )
      {
         if ( !strcmp ( pl_Socket->adresse, pl_TacheProt ) )
         {
            /* Mise a jour du numero de socket */
            vg_donneesGTC.Socket = pl_Socket->desc_sock;
            /*B Arret: la socket avec la tache existe */
            break;
         }
      }
      
      /*A Si la socket avec la tache telmi n'existe pas */
      if ( pl_Socket == NULL )
      {
         /*A Appel de ex_cnx_client pour etablir la liaison avec la tache protocole */
         if ( ex_cnx_client ( 	pl_TacheTrt,
				pl_TacheProt,
				&vg_SockMask,
				&vl_SockProt ) != XDC_OK )
	 {
	    /*B Ecriture Trace */
            XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de creer la connexion client  ." );	
	 }
	 else
	 {
	    /* Memorisation du numero de socket */
	    vg_donneesGTC.Socket = vl_SockProt;
	 }	
      }
      
      xzat26_Init_Etats_Eqts ( XDC_EQT_GTC, XDC_ETAT_TOUS_EQT, vg_NomMachine );
      xzat26_Init_Etats_Eqts ( XDC_EQT_TOR, XDC_ETAT_TOUS_EQT, vg_NomMachine );
      
   return (XDC_OK);
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
   static char *version = "@(#)epmv_cfg.c	1.6 12/16/94 : ec_sortir" ;

   XZEXT_MASK *			pl_Socket;
   XZEXT_ADR_SOCK		pl_TacheProt,
   				pl_TacheTrt;
   XZEXT_MSG_SOCKET 		pl_message;
   XDY_Datagroup		pl_NomDG;
   int				vl_resultat;

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

         sem_wait(&vg_semaphore);          
         /* Envoie de la demande d'arret */
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
