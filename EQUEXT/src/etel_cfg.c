/*E*/
/*Fichier : @(#)etel_cfg.c	1.7      Release : 1.7        Date : 12/13/95
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  EQUEXT
------------------------------------------------------
* MODULE TETEL * FICHIER etel_cfg.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Fonctions de initialisation et configuration de la tache TETEL
*
*
*
------------------------------------------------------
* HISTORIQUE :
*
* Nouaille.M	Version 1.1	30 Nov 1994	: Creation
* Mismer.D      Version 1.2     17 Mar 1995     : RAS
* Fontaine.C    Version 1.3     18 Avr 1995     : RAS
* Fontaine.C    Version 1.4     03 Mai 1995     : Reprise entete et commentaire
* Fontaine.C    Version 1.5     17 Mai 1995     : Reprise historique
* Mismer.D	version 1.6	07 Jun 1995	: Modif condition prise en compte nouvelle config
* Mismer.D	version 1.7	04 Dec 1995	: Ajout appel xzat26_Init_Etats_Eqts
* Diaz.L  	version 1.8     09 Oct 1998     : Suppresion de traces du PABX
*
------------------------------------------------------*/

/* fichiers inclus */
#include <xzsem.h>

#include "etel_cfg.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)etel_cfg.c	1.7 12/13/95 : etel_cfg" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction effectue la lecture du fichier XDF_Config_Cstes_AUTOTEL_<NomMachine>
*  et met a jour le liste des structure de donnees AUTOTEL;
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ec_lire_config_AUTOTEL	( XDY_NomMachine		pa_NomMachine )
			  
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
   static char *version = "@(#)etel_cfg.c	1.7 12/13/95 : ec_lire_config_AUTOTEL" ;

   int				vl_ValRet = XDC_OK;
   int				vl_Fd;
   char 			pl_PathEtFic[XDC_PATH_ABS_SIZE];
   XDY_Texte			pl_LigneFich;
   int				vl_NbAUTOTEL = 0;
   ETEL_DONNEES_AUTOTEL		vl_DonneesAUTOTEL = {0},
   				*pl_DonCrt;
   int				vl_NbParam  = 0;
  
   
      /*A Ouverture du fichier de configuration des AUTOTEL */
      /* Construction du nom du fichier XDF_Config_Cstes_AUTOTEL_<NomMachine>  */
      sprintf ( pl_PathEtFic, "%s/%s_%s", XZEXC_PATH_CONFIG, XDF_Config_Cstes_AUTOTEL, pa_NomMachine );
      
      /* Ouverture du fichier XDF_Config_Cstes_AUTOTEL_<NomMachine>  */
      if ( (vl_Fd = open (pl_PathEtFic, O_RDONLY)) <= 0 )
      {
         /* Ecriture trace absence fichier */
         XZST_03EcritureTrace( XZSTC_WARNING, " Manque fichier %s. \n", pl_PathEtFic ); 
         /* Sortie de la fonction */
         return ( XDC_NOK );
      }
      /*A Marquage des structures afin de determiner les eventuelles suppression */
      for ( pl_DonCrt = pg_debutAUTOTEL; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant )
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
             /*B Extraire les parametre du AUTOTEL */
             vl_NbParam = sscanf ( pl_LigneFich, "%s %s %d %d %s %s %d %d %d %d",
               				vl_DonneesAUTOTEL.Config.Adr,
               				vl_DonneesAUTOTEL.Config.NumeroST,
					&vl_DonneesAUTOTEL.Config.MaxNumTel,
					&vl_DonneesAUTOTEL.Config.TaillePrefixe,
					vl_DonneesAUTOTEL.Config.CodePrefixe,
					vl_DonneesAUTOTEL.Config.CodeRenvoi,
					&vl_DonneesAUTOTEL.Config.TpsAttente,
					&vl_DonneesAUTOTEL.Config.ModeEcho,
					&vl_DonneesAUTOTEL.Config.CodeRetour,
					&vl_DonneesAUTOTEL.Config.FormatCodeRetour
					);
             
             /* Si tout les parametre ont ete lu */
             if ( vl_NbParam == 10 )
             {
                
                 /*B Inserer dans la liste des structures config AUTOTEL */
                 if ( ed_ins_liste_AUTOTEL ( &vl_DonneesAUTOTEL, sizeof(vl_DonneesAUTOTEL.Config) ) == XDC_OK )
                 {
                    /* Incrementation du nombre de AUTOTEL */
                    vl_NbAUTOTEL++;
                    vl_DonneesAUTOTEL.Config.Numero = vl_NbAUTOTEL;
                 }
             }
         }
      }
      /*A Fermeture du fichier XDF_Config_Cstes_AUTOTEL_<NomMachine>  */
      XZSS_13FermerFichier( vl_Fd );
      
      /*A Suppression de la liste des donnees des elements ne faisant plus partie de la config */
      for ( pl_DonCrt = pg_debutAUTOTEL; pl_DonCrt != NULL; )
      {
         /* Si le numero de ST est effacer */
         if ( strlen ( pl_DonCrt->Config.NumeroST ) <= 1 )
         {
            /* Suppression de la liste */
            ed_sup_liste_AUTOTEL ( pl_DonCrt->Config.Numero );
            pl_DonCrt = pg_debutAUTOTEL;
         }
         else
         {
            /* Passage a l'element suivant */
            pl_DonCrt = pl_DonCrt->Suivant;
         }
      }
     
      /* Ecriture trace de la config pour debug */
      for ( pl_DonCrt = pg_debutAUTOTEL; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant )
      {
         
          XZST_03EcritureTrace ( XZSTC_DEBUG3, "Config TEL: %s %s %d %d %s %s %d %d %d %d",  
					pl_DonCrt->Config.Adr,
					pl_DonCrt->Config.NumeroST,
					pl_DonCrt->Config.MaxNumTel,
					pl_DonCrt->Config.TaillePrefixe,
					pl_DonCrt->Config.CodePrefixe,
					pl_DonCrt->Config.CodeRenvoi,
					pl_DonCrt->Config.TpsAttente,
					pl_DonCrt->Config.ModeEcho,
					pl_DonCrt->Config.CodeRetour,
					pl_DonCrt->Config.FormatCodeRetour );
	 				
      }
      /* Retourne compte rendu OK */
      return ( vl_ValRet );
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction effectue la lecture du fichier XDF_Config_AUTOTEL_<NomMachine>
*  et met a jour le liste des structure de donnees MAA;
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ec_lire_config_MAA	( XDY_NomMachine		pa_NomMachine )
			  
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
   static char *version = "@(#)etel_cfg.c	1.7 12/13/95 : ec_lire_config_MAA" ;

   int				vl_ValRet = XDC_OK;
   int				vl_Fd;
   char 			pl_PathEtFic[XDC_PATH_ABS_SIZE];
   XDY_Texte			pl_LigneFich;
   int				vl_NbMAA = 0;
   ETEL_DONNEES_MAA		vl_DonneesMAA = {0},
   				*pl_DonCrt;
   int				vl_NbParam  = 0;
   int				vl_indtaberr;
   int				vl_int1, vl_int2, vl_int3 ;
   
  
      /*A Ouverture du fichier de configuration des MAA */
      /* Construction du nom du fichier XDF_Config_AUTOTEL_<NomMachine>  */
      sprintf ( pl_PathEtFic, "%s/%s_%s", XZEXC_PATH_CONFIG, XDF_Config_AUTOTEL, pa_NomMachine );
      
      /* Ouverture du fichier XDF_Config_AUTOTEL_<NomMachine>  */
      if ( (vl_Fd = open (pl_PathEtFic, O_RDONLY)) <= 0 )
      {
         /* Ecriture trace absence fichier */
         XZST_03EcritureTrace( XZSTC_WARNING, " Manque fichier %s. \n", pl_PathEtFic ); 
         /* Sortie de la fonction */
         return ( XDC_NOK );
      }
      /*A Marquage des structures afin de determiner les eventuelles suppression */
      for ( pl_DonCrt = pg_debutMAA; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant )
      {
      	 /* Effacement du numero de Serveur de terminal pour marquage */
         strcpy(pl_DonCrt->Config.NumeroST, " ");
      }
 
      /*A Lecture de chaque ligne du fichier */
      while ( ex_LireLigneFich ( vl_Fd, pl_LigneFich, sizeof ( pl_LigneFich ) ) != XZEXC_EOF ) 
      {
          /* Lire ligne suivante si c'est une ligne de commentaire */
          if ( *pl_LigneFich != XDF_COMMENTAIRE )
          {
             /*B Extraire les parametre du MAA */

             vl_NbParam = sscanf ( pl_LigneFich, "%d %s %d %s %d %s",
              				&vl_int1,
              				vl_DonneesMAA.Config.NumeroST,
					&vl_int2,
					vl_DonneesMAA.Config.AdrAUTOTEL ,
					&vl_int3,
					vl_DonneesMAA.Config.RepAssoc);
             
             /*B Si tout les parametre ont ete lu et donnees de type MAA (pas OPERATOR) */
             if ( vl_NbParam == 6 )
             {
             	 if (strstr(vl_DonneesMAA.Config.AdrAUTOTEL, "OPE") == NULL) 
             	 { 
             	  vl_DonneesMAA.Config.Numero = vl_int1 ;
             	  vl_DonneesMAA.Config.NoLSMaitre = vl_int2 ;
             	  vl_DonneesMAA.Config.NumeroPO = vl_int3 ;

                 /*B Inserer dans la liste des structures config PICTO */
                  if ( ed_ins_liste_MAA ( &vl_DonneesMAA, sizeof(vl_DonneesMAA.Config) ) == XDC_OK )
                  {
                    /* Incrementation du nombre d'analyseur */
                    vl_NbMAA++;
                  }
                 }
             }
         }
      }
      /*A Fermeture du fichier XDF_Presence_Equip_<NomMachine>  */
      XZSS_13FermerFichier( vl_Fd );
      
      /*A Suppression de la liste des donnees des elements ne faisant plus partie de la config */
      for ( pl_DonCrt = pg_debutMAA; pl_DonCrt != NULL; )
      {
         /* Si le numero de ST est effacer */
         if ( strlen ( pl_DonCrt->Config.NumeroST ) <= 1 )
         {
            /* Suppression de la liste */
            ed_sup_liste_MAA ( pl_DonCrt->Config.Numero );
            pl_DonCrt = pg_debutMAA;
         }
         else
         {
            /* Passage a l'element suivant */
            pl_DonCrt = pl_DonCrt->Suivant;
         }
      }
     
      /*A Ecriture trace de la config pour debug et init de l'etat de service et de l'activite */
      for ( pl_DonCrt = pg_debutMAA; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant )
      {
         pl_DonCrt->EtatLiaison = XDC_OK ;
         pl_DonCrt->Etat_Service  = 0;
         pl_DonCrt->CptRenduCde = XDC_OK ;
         pl_DonCrt->NoCdeCourante  = 0;
         strcpy(pl_DonCrt->NoTelAppele, "" );
         pl_DonCrt->RenvoiSurRepondeur = XDC_FAUX ;
         
	 XZST_03EcritureTrace ( XZSTC_WARNING, "Config MAA: %d %s %d %s %d %s",
	 				pl_DonCrt->Config.Numero,
					pl_DonCrt->Config.NumeroST,
					pl_DonCrt->Config.NoLSMaitre,
					pl_DonCrt->Config.AdrAUTOTEL,
					pl_DonCrt->Config.NumeroPO,
					pl_DonCrt->Config.RepAssoc );
	 
	 			
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
   static char *version = "@(#)etel_cfg.c	1.7 12/13/95 : ec_controle" ;

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

void	ec_init	( XDY_NomMachine  	pa_NomMachine )

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
*   Lecture des fichier de configuration des AUTOTEL et des MAA
*   Remplissage des structures de donnees
*   Connexion socket avec la tache protocole
*
------------------------------------------------------*/
{
   static char *version = "@(#)etel_cfg.c	1.7 12/13/95 : ec_init" ;

   ETEL_DONNEES_MAA *		pl_DonCrt;
   XZEXT_MASK *			pl_Socket;
   XZEXT_ADR_SOCK		pl_TacheTrt;
   XZEXT_ADR_SOCK		pl_TacheProt;
   int				vl_SockProt = 0;

   /*A
    * Lecture des fichiers de configuration et remplissage des structure de donnees 
    */
   /*A Configuration des AUTOTEL (appel de la fonction ec_lire_config_AUTOTEL) */
    ec_lire_config_AUTOTEL ( pa_NomMachine );
   /*A Configuration des MAA (appel de la fonction ec_lire_config_MAA)*/
    ec_lire_config_MAA ( pa_NomMachine );

   /*A Appel  xzat26_Init_Etats_Eqts pour Initialisation des etats equipement (HS ou Actif) */
   xzat26_Init_Etats_Eqts ( XDC_EQT_MAA, XDC_ETAT_TOUS_EQT, vg_NomMachine );
   
   /*A
    * Connexion socket en mode client pour communiquer avec la tache TETMi
    */
   /*A Pour tout les MAA rechercher si la tache protocole associee existe */
   for ( pl_DonCrt = pg_debutMAA; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant )
   {
      /* Construction du nom de la tache protocole tetmi*/
      sprintf ( pl_TacheProt, "%s%d", XZEXC_ADR_SOCK_TETM, pl_DonCrt->Config.NoLSMaitre );
      sprintf ( pl_TacheTrt, "%s%d", XZEXC_ADR_SOCK_MAATETM, pl_DonCrt->Config.NoLSMaitre );
      /*A Recherche sur tout le masque des socket si la socket exite */
      for ( pl_Socket = vg_SockMask; pl_Socket != NULL; pl_Socket = pl_Socket->Suivant )
      {
         if ( !strcmp ( pl_Socket->adresse, pl_TacheTrt ) )
         {
            /* Mise a jour du numero de socket */
            pl_DonCrt->SocketMaitre = pl_Socket->desc_sock;
            /*B Arret: la socket avec la tache existe */
            break;
         }
      }
      
      /*A Si la socket avec la tache tetmi n'existe pas */
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
	    pl_DonCrt->SocketMaitre = vl_SockProt;
	 }	
      }
   }
   
    /*A Envoi commande initialisation a l'automate telephonique : module ed_init_MAA */
    ed_init_MAA() ;
 
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
   static char *version = "@(#)etel_cfg.c	1.7 12/13/95 : ec_sortir" ;

   ETEL_DONNEES_AUTOTEL		*pl_AUTOTEL, 	*pl_AUTOTELALiberer;
   ETEL_DONNEES_MAA		*pl_MAA, 	*pl_MAAALiberer;
   XZEXT_MASK *			pl_Socket;
   XZEXT_ADR_SOCK		pl_TacheProt,
   				pl_TacheTrt;
   XZEXT_MSG_SOCKET 		pl_message;
   XDY_Datagroup		pl_NomDG;
   int				vl_resultat;

  /*A
   * Liberation des ressources memoires
   */

   /*A Liberation des ressources memoires des donnees AUTOTEL */
   for ( pl_AUTOTEL = pg_debutAUTOTEL; pl_AUTOTEL != NULL; free(pl_AUTOTELALiberer) )
   {
      pl_AUTOTELALiberer = pl_AUTOTEL;
      pl_AUTOTEL         = pl_AUTOTEL->Suivant;
   }  
   /*A Liberation des ressources memoires des donnees des MAA */
   for ( pl_MAA = pg_debutMAA; pl_MAA != NULL; free(pl_MAAALiberer) )
   {
      pl_MAAALiberer = pl_MAA;
      pl_MAA         = pl_MAA->Suivant;
   }  


   /*A
    * Envoie du message d'arret aux taches protocoles 
    */
    
   for ( pl_Socket = vg_SockMask; pl_Socket != NULL; pl_Socket = pl_Socket->Suivant )
   {
      if ( !strncmp ( pl_Socket->adresse, XZEXC_ADR_SOCK_TETM, strlen(XZEXC_ADR_SOCK_TETM) ) )
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

   /* Fin de la tache */
   exit (0);
}
