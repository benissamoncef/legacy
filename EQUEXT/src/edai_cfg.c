/*E*/
/*Fichier : $Id: edai_cfg.c,v 1.22 2021/06/24 09:01:37 pc2dpdy Exp $      Release : $Revision: 1.22 $       Date : $Date: 2021/06/24 09:01:37 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEDAI * FICHIER edai_cfg.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module de configuration de la tache TEDAI.
* 
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E	version 1.1	18 Oct 1994	: Creation
* Nagiel.E	version 1.2	29 Nov 1994	:
* Nagiel.E	version 1.3	22 Dec 1994	:
* Nagiel.E	version 1.4	22 Dec 1994	:
* Nagiel.E 	version 1.5	05 Jan 1995	:
* Nagiel.E	version 1.6	16 Jan 1995	:
* Nagiel.E	version 1.7	16 Jan 1995	:
* Mismer.D	version 1.8	26 Jan 1995	:
* Mismer.D	version 1.9	09 Fev 1995	:
* Mismer.D	version 1.10	17 Mar 1995	:
* Volcic.F	version 1.11	22 Mar 1995	: Modifs apres essais
* Fontaine.C	version 1.12	23 Mai 1995	: Reprise entete, historique
* Mismer.D	version 1.13	07 Jun 1995	: Modif condition prise en compte nouvelle config
* Mismer.D	version 1.14	15 Nov 1995	: Ajout FATAL sur echec connexion tache protocole
* Mismer.D	version 1.15	12 Jun 1996	: Appel de ex_lire_idpassw a la place de ec_lire_idpassw (DEM/1135)
* Mismer.D	version 1.16 	15 Nov 1996	: Modif pour alerte DAI tunnel (DEM/1309)
* JMG   13/11/07        : ajout site de gestion DEM715
* JPL	30/11/11	: Migration architecture HP ia64 (DEM 975) : suppression warnings mineurs  1.19
* JMG	01/06/19  : passgae IP DEM1333 1.20
* CGR	16/09/20	: Ajout deuxieme adresse IP DEM-SAE175 1.21
* LCL	24/06/21	: Ajout trace si fichier de conf errone DEM-SAE-276 1.22
------------------------------------------------------*/

/* fichiers inclus */

#include "xzat.h"

#include "ex_mrtf.h"

#include "edai_adt.h"
#include "edai_cfg.h"

#include <xzsem.h>

/* definitions de constantes */
#define SG_PING         "/etc/ping"
#define SG_TO_PING      8
#define SG_BYTES_PING   64
#define SG_COUNT_PING   1
#define SG_TOKEN_TST    "64"

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: edai_cfg.c,v 1.22 2021/06/24 09:01:37 pc2dpdy Exp $ : edai_cfg" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */
extern int   vg_SockTEMIP;
int vm_principal;


void checkPasserelle(char *);

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction effectue la lecture du fichier XDF_Config_DAI_<NomMachine>
*  et met a jour le liste des structure de donnees DAI;
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ec_lire_config_DAI	( XDY_NomMachine		pa_NomMachine )
			  
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
   static char *version = "$Id: edai_cfg.c,v 1.22 2021/06/24 09:01:37 pc2dpdy Exp $ : ec_lire_config_DAI" ;

   int				vl_ValRet = XDC_OK;
   int				vl_Fd;
   char 			pl_PathEtFic[XDC_PATH_ABS_SIZE];
   char 			vl_adr[40];
   XDY_Texte			pl_LigneFich;
   int				vl_NbDAI = 0;
   EDAI_DONNEES_DAI		vl_DonneesDAI = {0},
   				*pl_DonCrt;
   short 			vl_NumeroLSEsclave  = 0,
   				vl_NumeroLSMaitre = 0;
   int				vl_NbParam  = 0;
   int				vl_site;
  XZEXT_MSG_SOCKET             pl_message;
 
      /*A Ouverture du fichier de configuration des DAI */
      /* Construction du nom du fichier XDF_Config_DAI_<NomMachine>  */
      sprintf ( pl_PathEtFic, "%s/%s_%s", XZEXC_PATH_CONFIG, XDF_Config_DAI, pa_NomMachine );
      
      /* Ouverture du fichier XDF_Config_DAI_<NomMachine>  */
      if ( (vl_Fd = open (pl_PathEtFic, O_RDONLY)) <= 0 )
      {
         /* Ecriture trace absence fichier */
         XZST_03EcritureTrace( XZSTC_WARNING, " Manque fichier %s. \n", pl_PathEtFic ); 
         /* Sortie de la fonction */
         return ( XDC_NOK );
      }
      /*A Marquage des structures afin de determiner les eventuelles suppression */
      for ( pl_DonCrt = pg_debutDAI; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant )
      {
      	 /* Effacement du numero de ST pour marquage */
         strcpy ( pl_DonCrt->Config.NumeroST, " ");
      }
 
      /*A Lecture de chaque ligne du fichier */
      while ( ex_LireLigneFich ( vl_Fd, pl_LigneFich, sizeof ( pl_LigneFich ) ) != XZEXC_EOF ) 
      {
         XZST_03EcritureTrace( XZSTC_WARNING, " ligne %s", pl_LigneFich ); 
          /* Lire ligne suivante si c'est une ligne de commentaire */
          if ( *pl_LigneFich != XDF_COMMENTAIRE )
          {
             /*B Extraire les parametre du DAI */
             vl_NbParam = sscanf ( pl_LigneFich, 	"%hd %s %hd %hd %3c %d %d %s %d %s %d %s %d %s",  
					&vl_DonneesDAI.Config.Numero,
					vl_DonneesDAI.Config.NumeroST,
					&vl_NumeroLSMaitre,
					&vl_NumeroLSEsclave,
					vl_DonneesDAI.Config.AdresseRGS,
					&vl_site,
					&vl_DonneesDAI.Config.Port,
					vl_DonneesDAI.Config.AdresseIP,
					&vl_DonneesDAI.Config.PortE,
					vl_DonneesDAI.Config.AdresseIPE,
					&vl_DonneesDAI.Config.Port2,
					vl_DonneesDAI.Config.Adresse2IP,
					&vl_DonneesDAI.Config.Port2E,
					vl_DonneesDAI.Config.Adresse2IPE);
	     vl_DonneesDAI.Config.SiteGestion=vl_site;
             vl_DonneesDAI.Config.NoLSMaitre = vl_NumeroLSMaitre;
             vl_DonneesDAI.Config.NoLSEsclave = vl_NumeroLSEsclave;

             /* Si tout les parametre ont ete lu */
             if ( vl_NbParam == 14 )
             {
                 /*B Inserer dans la liste des structures config DAI */
                 if ( ed_ins_liste_DAI ( &vl_DonneesDAI, sizeof(vl_DonneesDAI.Config) ) == XDC_OK )
                 {
                    /* Incrementation du nombre de DAI */
                    vl_NbDAI++;

		    /*connnexion eventuelle a temip*/
                if (strcmp(vl_DonneesDAI.Config.AdresseIP,XZECC_IP_VIDE)) {
                        /*RADT IP*/
                        if (vg_SockTEMIP==-1) {
                                sprintf(vl_adr,"%s%d",XZEXC_ADR_SOCK_TEIM,
                                                XDC_EQT_DAI);
                                ex_cnx_client (    XZEXC_ADR_SOCK_TEDAI,
                                                        vl_adr,
                                                        &vg_SockMask,
                                                         &vg_SockTEMIP);
                        }
vm_principal=XDC_OK;
			if (vm_principal==XDC_OK)
                          sprintf ( pl_message, "%s%s %d %c%s",
                                XZEXC_CONFIG_IP,
                                vl_DonneesDAI.Config.AdresseIP,
                                vl_DonneesDAI.Config.Port,
                                XZECC_PROT_LCR,
                                XZEXC_FIN_MSG_SOCK );
			else			
                          sprintf ( pl_message, "%s%s %d %c%s",
                                XZEXC_CONFIG_IP,
                                vl_DonneesDAI.Config.Adresse2IP,
                                vl_DonneesDAI.Config.Port2,
                                XZECC_PROT_LCR,
                                XZEXC_FIN_MSG_SOCK );
                        sem_wait(&vg_semaphore); 
                        if ( write ( vg_SockTEMIP, pl_message, strlen (pl_message) ) == XDC_NOK) {
                                XZST_03EcritureTrace(XZSTC_WARNING, " connexion impossible");
                                 sem_post(&vg_semaphore); 
                                return (XDC_NOK);
                        }
         sem_post(&vg_semaphore); 
			if (vm_principal==XDC_OK)
                          sprintf ( pl_message, "%s%s %d %c%s",
                                XZEXC_CONFIG_IP,
                                vl_DonneesDAI.Config.AdresseIPE,
                                vl_DonneesDAI.Config.PortE,
                                XZECC_PROT_LCR_E,
                                XZEXC_FIN_MSG_SOCK );
			else
                          sprintf ( pl_message, "%s%s %d %c%s",
                                XZEXC_CONFIG_IP,
                                vl_DonneesDAI.Config.Adresse2IPE,
                                vl_DonneesDAI.Config.Port2E,
                                XZECC_PROT_LCR_E,
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
	     else
	     {
         		XZST_03EcritureTrace( XZSTC_WARNING, "Le fichier de configuration n'est pas conforme (%d colonnes sur 14 attendues)", vl_NbParam ); 
	     }
         }
      }
      sleep(5);
      /*A Fermeture du fichier XDF_Config_DAI_<NomMachine>  */
      XZSS_13FermerFichier( vl_Fd );
      
      /*A Suppression de la liste des donnees des elements ne faisant plus partie de la config */
      for ( pl_DonCrt = pg_debutDAI; pl_DonCrt != NULL; )
      {
         /* Si le numero de ST est effacer */
         if ( strlen ( pl_DonCrt->Config.NumeroST ) <= 1 )
         {
            /* Suppression de la liste */
            ed_sup_liste_DAI ( pl_DonCrt->Config.Numero );
            pl_DonCrt = pg_debutDAI;
         }
         else
         {
            /* Passage a l'element suivant */
            pl_DonCrt = pl_DonCrt->Suivant;
         }
      }

      /* Ecriture trace de la config pour debug */
      for ( pl_DonCrt = pg_debutDAI; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant )
      {
          pl_DonCrt->Etat_Service  = 0;
          XZST_03EcritureTrace ( XZSTC_WARNING, "Config DAI: %d %s %d %d %s %d %s %d %s %d",  
					pl_DonCrt->Config.Numero,
					pl_DonCrt->Config.NumeroST,
					pl_DonCrt->Config.NoLSMaitre,
					pl_DonCrt->Config.NoLSEsclave,
					pl_DonCrt->Config.AdresseRGS,
					pl_DonCrt->Config.SiteGestion,
					pl_DonCrt->Config.AdresseIP,
					pl_DonCrt->Config.Port,
					pl_DonCrt->Config.AdresseIPE,
					pl_DonCrt->Config.PortE);				
      }
      /* Retourne compte rendu OK */
      return ( vl_ValRet );
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction effectue la lecture du fichier XDF_Config_Analy_<NomMachine>
*  et met a jour le liste des structure de donnees Analy;
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ec_lire_config_Analy	( XDY_NomMachine		pa_NomMachine )
			  
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
   static char *version = "$Id: edai_cfg.c,v 1.22 2021/06/24 09:01:37 pc2dpdy Exp $ : ec_lire_config_Analy" ;

   int				vl_ValRet = XDC_OK;
   int				vl_Fd;
   char 			pl_PathEtFic[XDC_PATH_ABS_SIZE];
   XDY_Texte			pl_LigneFich;
   int				vl_NbAnaly = 0;
   EDAI_DONNEES_ANALYSEUR	vl_DonneesAnaly = {0},
   				*pl_DonCrt;
   int				vl_NbParam  = 0;
   T_BOOL			vl_EtatDg = FALSE;
   XDY_Datagroup		pl_NomDG;
   int				vl_indtaberr;
   int				vl_site;
   
  
      /*A Ouverture du fichier de configuration des Analyseurs */
      /* Construction du nom du fichier XDF_Config_Analyseur_<NomMachine>  */
      sprintf ( pl_PathEtFic, "%s/%s_%s", XZEXC_PATH_CONFIG, XDF_Config_Analyseur, pa_NomMachine );
      
      /* Ouverture du fichier XDF_Config_Analyseur_<NomMachine>  */
      if ( (vl_Fd = open (pl_PathEtFic, O_RDONLY)) <= 0 )
      {
         /* Ecriture trace absence fichier */
         XZST_03EcritureTrace( XZSTC_WARNING, " Manque fichier %s. \n", pl_PathEtFic ); 
         /* Sortie de la fonction */
         return ( XDC_NOK );
      }
      /*A Marquage des structures afin de determiner les eventuelles suppression */
      for ( pl_DonCrt = pg_debutAnaly; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant )
      {
      	 /* Effacement du numero de DAI pour marquage */
         pl_DonCrt->Config.NumeroDAI=0;
      }
 
      /*A Lecture de chaque ligne du fichier */
      while ( ex_LireLigneFich ( vl_Fd, pl_LigneFich, sizeof ( pl_LigneFich ) ) != XZEXC_EOF ) 
      {
          /* Lire ligne suivante si c'est une ligne de commentaire */
          if ( *pl_LigneFich != XDF_COMMENTAIRE )
          {
             /*B Extraire les parametre de l'analyseur */

             vl_NbParam = sscanf ( pl_LigneFich, " %hd %hd %hd %hd %d",  
					&vl_DonneesAnaly.Config.NumeroDAI,
					&vl_DonneesAnaly.Config.NoAnaly,
					&vl_DonneesAnaly.Config.Numero,
					&vl_DonneesAnaly.Config.NbVoiesGerees,
					&vl_site);
             
	     vl_DonneesAnaly.Config.SiteGestion=vl_site;
             /* Si tout les parametre ont ete lu */
             if ( vl_NbParam == 5 )
             {
                 /*B Inserer dans la liste des structures config PICTO */
                 if ( ed_ins_liste_Analy ( &vl_DonneesAnaly, sizeof(vl_DonneesAnaly.Config) ) == XDC_OK )
                 {
                    /* Incrementation du nombre d'analyseur */
                    vl_NbAnaly++;
                 }
             }
         }
      }
      /*A Fermeture du fichier XDF_Presence_Equip_<NomMachine>  */
      XZSS_13FermerFichier( vl_Fd );
      
      /*A Suppression de la liste des donnees des elements ne faisant plus partie de la config */
      for ( pl_DonCrt = pg_debutAnaly; pl_DonCrt != NULL; )
      {
         /* Si le numero de ST est effacer */
         if ( pl_DonCrt->Config.NumeroDAI == 0 )
         {
            /* Suppression de la liste */
            ed_sup_liste_Analy ( pl_DonCrt->Config.Numero );
            pl_DonCrt = pg_debutAnaly;
         }
         else
         {
            /* Passage a l'element suivant */
            pl_DonCrt = pl_DonCrt->Suivant;
         }
      }
     
      /*A Ecriture trace de la config pour debug et init de l'etat de service et de l'activite */
      for ( pl_DonCrt = pg_debutAnaly; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant )
      {
         pl_DonCrt->Etat_Service  = 0;
         pl_DonCrt->Alerte = 0;
         pl_DonCrt->EtatConf = EDAIC_CONF_FIN;
         for ( vl_indtaberr=0;vl_indtaberr<NB_ERR_POSSI;vl_indtaberr++)
         {
            pl_DonCrt->TabErrSyst[vl_indtaberr]=0;
         }
         
	 XZST_03EcritureTrace ( XZSTC_DEBUG3, "Config Analyseur: %d %d %d %d %d",  
               				pl_DonCrt->Config.NumeroDAI,
					pl_DonCrt->Config.NoAnaly,
					pl_DonCrt->Config.Numero,
					pl_DonCrt->Config.NbVoiesGerees,
					pl_DonCrt->Config.SiteGestion);
	 
	 sprintf ( pl_NomDG, "%s_N%d", XDG_EInit_IHM_DAI, pl_DonCrt->Config.Numero );
	 			
   	 TipcSrvSubjectGetSubscribe ( pl_NomDG, &vl_EtatDg );
   	 
   	 if ( ( !vl_EtatDg ) && ( pl_NomDG != NULL ) )
   	 {
   	    /* joint ce datagroup */
   	    if ( !XDG_JoindreDG( pl_NomDG ) )
   	    {
	       /* Affichage Trace */
      	       XZST_03EcritureTrace(XZSTC_WARNING,
	       "Abonnement au Datagroup %s non effectue ", pl_NomDG );
	    }
	 }			
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
   static char *version = "$Id: edai_cfg.c,v 1.22 2021/06/24 09:01:37 pc2dpdy Exp $ : ec_controle" ;

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
         /*A Arreter la tache par appel de ec_sortir */
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
*   Lecture des fichier de configuration des DAi et des Analyseur
*   Remplissage des structures de donnees
*   Connexion socket avec la tache protocole
*
------------------------------------------------------*/
{
   static char *version = "$Id: edai_cfg.c,v 1.22 2021/06/24 09:01:37 pc2dpdy Exp $ : ec_init" ;

   EDAI_DONNEES_DAI *		pl_DonCrt;
   XZEXT_MASK *			pl_Socket;
   XZEXT_ADR_SOCK		pl_TacheTrt;
   XZEXT_ADR_SOCK		pl_TacheProt;
   int				vl_SockProt = 0;
   int				vl_retcode = 0; 

   /*A
    * Lecture des fichiers de configuration et remplissage des structure de donnees 
    */
   /*A Configuration des dai (appel de la fonction ec_lire_config_DAI) */
    if ( ec_lire_config_DAI ( pa_NomMachine ) != XDC_OK) return(XDC_NOK);
   /*A Configuration des Analyseurs (appel de la fonction ec_lire_config_Analy)*/
    if ( ec_lire_config_Analy ( pa_NomMachine ) != XDC_OK) return(XDC_NOK);
   /*A Configuration des Alertes DAI Tunnel (appel de la fonction ec_lire_config_Alerte_Tunnel*/
    ec_lire_config_Alerte_Tunnel ( pa_NomMachine );

   /*A Lecture du password de la machine (user/password) */
   if ( ( vl_retcode = ex_lire_idpassw ( pa_NomMachine, vg_idpassw ) ) != XDC_OK )
   {
   	XZST_03EcritureTrace(XZSTC_WARNING,"%s: ex_lire_idpassw retourne %d", version, vl_retcode);
	return (XDC_NOK);
   }
   XZST_03EcritureTrace( XZSTC_WARNING, "Pwd:<%s>.", vg_idpassw );	
   
   /*A Ouverture du fichier d'alarme */
   ex_OuvertureFichierMsg(vg_NomMachine);
   
   /*A Appel  xzat26_Init_Etats_Eqts pour Initialisation des etats equipement (HS ou Actif) */
   xzat26_Init_Etats_Eqts ( XDC_EQT_DAI, XDC_ETAT_TOUS_EQT, vg_NomMachine );
   xzat26_Init_Etats_Eqts ( XDC_EQT_ANA, XDC_ETAT_TOUS_EQT, vg_NomMachine );

   /*A Demande d'init de la configh des voies */
   XZAH26_InitRadtDai(vg_NomMachine);

   /*A
    * Connexion socket en mode client pour communiquer avec la tache TELEi
    */
   /*A Pour tout les DAI rechercher si la tache protocole associee existe */
   for ( pl_DonCrt = pg_debutDAI; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant )
   {
      /* Construction du nom de la tache protocole telmi*/
      sprintf ( pl_TacheProt, "%s%d", XZEXC_ADR_SOCK_TELE, pl_DonCrt->Config.NoLSEsclave );
      sprintf ( pl_TacheTrt, "%s%d", XZEXC_ADR_SOCK_DAITELE, pl_DonCrt->Config.NoLSEsclave );
      /*A Recherche sur tout le masque des socket si la socket exite */
      for ( pl_Socket = vg_SockMask; pl_Socket != NULL; pl_Socket = pl_Socket->Suivant )
      {
	if (!strcmp(pl_DonCrt->Config.AdresseIPE,XZECC_IP_VIDE)) {
         if ( !strcmp ( pl_Socket->adresse, pl_TacheTrt ) )
         {
            /* Mise a jour du numero de socket */
            pl_DonCrt->SocketEsclave = pl_Socket->desc_sock;
            /*B Arret: la socket avec la tache existe */
            break;
         }
      }
     	}

      /*A Si la socket avec la tache telmi n'existe pas */
	if (!strcmp(pl_DonCrt->Config.AdresseIP,XZECC_IP_VIDE)) {
      if ( pl_Socket == NULL )
      {
         /*A Appel de ex_cnx_client pour etablir la liaison avec la tache protocole */
         if ( ex_cnx_client ( 	pl_TacheTrt,
				pl_TacheProt,
				&vg_SockMask,
				&vl_SockProt ) != XDC_OK )
	 {
	    /*B Ecriture Trace */
            XZST_03EcritureTrace( XZSTC_FATAL, "Impossible de creer la connexion client avec %s .", pl_TacheProt );	
	 }
	 else
	 {
	    /* Memorisation du numero de socket */
	    pl_DonCrt->SocketEsclave = vl_SockProt;
	 }	
      }
      }
   }
   
   /*A
    * Connexion socket en mode client pour communiquer avec la tache TELMi
    */
   /*A Pour tout les DAI rechercher si la tache protocole associee existe */
   for ( pl_DonCrt = pg_debutDAI; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant )
   {
      /* Construction du nom de la tache protocole telmi*/
      sprintf ( pl_TacheProt, "%s%d", XZEXC_ADR_SOCK_TELM, pl_DonCrt->Config.NoLSMaitre );
      sprintf ( pl_TacheTrt, "%s%d", XZEXC_ADR_SOCK_DAITELM, pl_DonCrt->Config.NoLSMaitre );
	 if (strcmp(pl_DonCrt->Config.AdresseIP,XZECC_IP_VIDE)) {
                pl_DonCrt->SocketMaitre = vg_SockTEMIP;
                pl_DonCrt->SocketEsclave = vg_SockTEMIP;

        }

      /*A Recherche sur tout le masque des socket si la socket exite */
      for ( pl_Socket = vg_SockMask; pl_Socket != NULL; pl_Socket = pl_Socket->Suivant )
      {
	if (!strcmp(pl_DonCrt->Config.AdresseIP,XZECC_IP_VIDE)) {
         if ( !strcmp ( pl_Socket->adresse, pl_TacheTrt ) )
         {
            /* Mise a jour du numero de socket */
            pl_DonCrt->SocketMaitre = pl_Socket->desc_sock;
            /*B Arret: la socket avec la tache existe */
            break;
         }
         }
      }
      
      /*A Si la socket avec la tache telmi n'existe pas */
	if (!strcmp(pl_DonCrt->Config.AdresseIP,XZECC_IP_VIDE)) {
      if ( pl_Socket == NULL )
      {
         /*A Appel de ex_cnx_client pour etablir la liaison avec la tache protocole */
         if ( ex_cnx_client ( 	pl_TacheTrt,
				pl_TacheProt,
				&vg_SockMask,
				&vl_SockProt ) != XDC_OK )
	 {
	    /*B Ecriture Trace */
            XZST_03EcritureTrace( XZSTC_FATAL, "Impossible de creer la connexion client avec %s .", pl_TacheProt );	
	 }
	 else
	 {
	    /* Memorisation du numero de socket */
	    pl_DonCrt->SocketMaitre = vl_SockProt;
	 }	
      }
      }
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
   static char *version = "$Id: edai_cfg.c,v 1.22 2021/06/24 09:01:37 pc2dpdy Exp $ : ec_sortir" ;

   EDAI_DONNEES_DAI		*pl_DAI, 	*pl_DAIALiberer;
   EDAI_DONNEES_ANALYSEUR	*pl_Analy, 	*pl_AnalyALiberer;
   XZEXT_MASK *			pl_Socket;
   XZEXT_ADR_SOCK		pl_TacheProt,
   				pl_TacheTrt;
   XZEXT_MSG_SOCKET 		pl_message;
   XDY_Datagroup		pl_NomDG;
   int				vl_resultat;

  /*A
   * Liberation des ressources memoires
   */

   /*A Liberation des ressources memoires des donnees DAI */
   for ( pl_DAI = pg_debutDAI; pl_DAI != NULL; free(pl_DAIALiberer) )
   {
      pl_DAIALiberer = pl_DAI;
      pl_DAI         = pl_DAI->Suivant;
   }  
   /*A Liberation des ressources memoires des donnees des Analy */
   for ( pl_Analy = pg_debutAnaly; pl_Analy != NULL; free(pl_AnalyALiberer) )
   {
      pl_AnalyALiberer = pl_Analy;
      pl_Analy         = pl_Analy->Suivant;
   }  


   /*A
    * Envoie du message d'arret aux taches protocoles 
    */
    
   for ( pl_Socket = vg_SockMask; pl_Socket != NULL; pl_Socket = pl_Socket->Suivant )
   {
      if ( (!strncmp(pl_Socket->adresse,XZEXC_ADR_SOCK_TELM,strlen(XZEXC_ADR_SOCK_TELM))) ||
           (!strncmp(pl_Socket->adresse,XZEXC_ADR_SOCK_TELE,strlen(XZEXC_ADR_SOCK_TELE))) )
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
      /* Fermeture de la socket courante */
      ex_fin_cnx ( pl_Socket->desc_sock, &vg_SockMask );
   }

   /* Fin de la tache */
   exit (0);
}


void checkPasserelle(char *va_ip)
{
  char vl_ligne[50];
  FILE  *pl_fp=NULL;

vm_principal=XDC_NOK;
  sprintf(vl_ligne,"%s %s %u %u",SG_PING,va_ip,
        SG_BYTES_PING, SG_COUNT_PING);
   if ((pl_fp = popen(vl_ligne,"r")) != (FILE *)NULL)
   {
      fgets(vl_ligne,sizeof(vl_ligne)-1,pl_fp) ;
      fgets(vl_ligne,sizeof(vl_ligne)-1,pl_fp) ;
      pclose(pl_fp) ;

      /*tout va bien*/
      if ((strstr(vl_ligne,SG_TOKEN_TST) != NULL)) {
        XZST_03EcritureTrace( XZSTC_DEBUG1,"serveur %s OK", va_ip);
        vm_principal=XDC_OK;
      }

  }
}



