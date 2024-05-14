/*E*/
/*Fichier : $Id: evid_cfg.c,v 1.15 2017/03/27 12:21:09 devgfi Exp $      Release : $Revision: 1.15 $        Date : $Date: 2017/03/27 12:21:09 $
------------------------------------------------------
* GRANIOU *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEVID * FICHIER evid_cfg.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module de configuration de la tache VIDEO
* il charge toutes les structures a partir des fichiers de config
*
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E	version 1.1	31 Oct 1994	: Creation
* Nagiel.E	version 1.2	29 Nov 1994	:
* Nagiel.E	version 1.3	23 Dec 1994	:
* Nagiel.E	version 1.4	26 Dec 1994	:
* Volcic.F	version 1.5	24 Jan 1995	:
* Mismer.D	version 1.6	26 Jan 1995	:
* Mismer.D	version 1.7	09 Avr 1995	:
* Mismer.D	version 1.8	18 Mai 1995	: Modif gestion compte rendu commande, reprise historique
* Mismer.D	version 1.9	07 Jun 1995	: Modif condition prise en compte nouvelle config
* Hilmarcher    version 1.10    03 fev 2004     : rajout traces pour dem 270
* JPL		25/02/11 : Migration architecture HP ia64 (DEM 975) : adequation format scanf et type d'entier  1.12
* PNI		14/05/11 : Prise en compte des codes moniteur sur 3 chiffres en supprimant le M qui ne sert à rien. DEM984
* JPL		04/11/16 : Suppr. config LTV et magnetos, etat equipements; config, init 2 Systemes video IP (DEM 1189)  1.14
* JPL		24/03/17 : A l'initialisation, message aux operateurs sur connexion correcte a l'un des serveurs video  1.15
------------------------------------------------------*/

/* fichiers inclus */

#include "xzia.h"

#include "ex_mrtf.h"

#include "evid_com.h"
#include "evid_cfg.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: evid_cfg.c,v 1.15 2017/03/27 12:21:09 devgfi Exp $ : evid_cfg" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction effectue la lecture du fichier XDF_Config_VIDEO_<NomMachine>
*  et met a jour le liste des structure de donnees VIDEO;
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ec_lire_config_VIDEO	( XDY_NomMachine		pa_NomMachine )
			  
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
   static char *version = "$Id: evid_cfg.c,v 1.15 2017/03/27 12:21:09 devgfi Exp $ : ec_lire_config_VIDEO" ;

   int				vl_ValRet = XDC_OK;
   int				vl_Fd;
   char 			pl_PathEtFic[XDC_PATH_ABS_SIZE];
   XDY_Texte			pl_LigneFich;
   EVID_DONNEES_CAMERA		*pl_Camera,vl_doncam;
//   EVID_DONNEES_MONITEUR	*pl_Moniteur,vl_donmon;

   int				vl_NbParam  = 0;
   int				vl_Numero_Mnt  = 0;
   char				vl_Code_Mnt[4] = "";
   char				vl_CMnt = '0';
   int				vl_cycle,vl_mobile,vl_posi;

      /*A Ouverture du fichier de configuration des Video */
      /* Construction du nom du fichier XDF_Config_video_<NomMachine>  */
      sprintf ( pl_PathEtFic, "%s/%s_%s", XZEXC_PATH_CONFIG, XDF_Config_VIDEO, pa_NomMachine );
      XZST_03EcritureTrace( XZSTC_WARNING, "Nom de la machine %s. \n", pa_NomMachine);    

      /* Ouverture du fichier XDF_Config_video_<NomMachine>  */
      if ( (vl_Fd = open (pl_PathEtFic, O_RDONLY)) <= 0 )
      {
         /* Ecriture trace absence fichier */
         XZST_03EcritureTrace( XZSTC_WARNING, " Manque fichier %s. \n", pl_PathEtFic ); 
         /* Sortie de la fonction */
         return ( XDC_NOK );
      }

      /*A Marquage des structures afin de determiner les eventuelles suppression de cameras */
      for ( pl_Camera = pg_debutCamera; pl_Camera != NULL; pl_Camera = pl_Camera->Suivant )
      {
      	 /* Effacement Nature pour marquage */
         pl_Camera->Config.Nature=0;
      }
 
      /*A Marquage des structures afin de determiner les eventuelles suppression de moniteurs */
//      for ( pl_Moniteur = pg_debutMoniteur; pl_Moniteur != NULL; pl_Moniteur = pl_Moniteur->Suivant )
//      {
//      	 /* Effacement Code pour marquage */
//	 XZST_03EcritureTrace( XZSTC_WARNING, "config moniteur: %d/%d.\n", pl_Moniteur->Config.Numero, pl_Moniteur->Config.Code);
//	 pl_Moniteur->Config.Code=0;
//      }


      /*A Lecture de chaque ligne du fichier */
      while ( ex_LireLigneFich ( vl_Fd, pl_LigneFich, sizeof ( pl_LigneFich ) ) != XZEXC_EOF ) 
      {
          /* Lire ligne suivante si c'est une ligne de commentaire */
          if ( *pl_LigneFich != XDF_COMMENTAIRE )
          {             if (strstr(pl_LigneFich,"C"))
             {
                /*B Extraire les parametres camera */
                vl_NbParam = sscanf ( pl_LigneFich, 	"C %hd %hd %d %d %d %d",  
                                              &vl_doncam.Config.Numero,
                                              &vl_doncam.Config.NumeroLTV,
                                              &vl_doncam.Config.Code,
                                              &vl_cycle,
                                              &vl_mobile,
                                              &vl_posi);
	        if (vl_mobile) vl_doncam.Config.Nature=XZEXC_MOBILE;
	        if (!vl_mobile) vl_doncam.Config.Nature=XZEXC_FIXE;
	        if (vl_cycle) vl_doncam.Config.Nature=XZEXC_CYCLE;
	        if (vl_posi) vl_doncam.Config.Nature=XZEXC_POSI;
	     
                /* Si tout les parametre ont ete lu */
                if ( vl_NbParam == 6 )
                {
                    /*B Inserer dans la liste des structures config Camera */
                    ed_ins_liste_Camera ( &vl_doncam, sizeof(vl_doncam.Config) ) ;
                }
             }

             
	     /*B Si c'est une ligne de config moniteur */
//             if (strstr(pl_LigneFich,"M"))
//             {
//                /*B Extraire les parametres moniteur */
//                vl_NbParam = sscanf ( pl_LigneFich, 	"M %hd %s %s",  
//                                              &vl_donmon.Config.Numero,
//                                              vl_Code_Mnt,
//                                              vl_donmon.Config.PO );
//	     
//                /* Si tout les parametre ont ete lu */
//                if ( vl_NbParam == 3 )
//                {
//                    vl_Numero_Mnt ++;
//                    vl_donmon.Config.Numero = vl_Numero_Mnt;
//                    if ( vl_Code_Mnt[0]=='M' )
//                      sscanf ( vl_Code_Mnt, "%c%hd", &vl_CMnt, &vl_donmon.Config.Code );
//		    else
//                      sscanf ( vl_Code_Mnt, "%hd", &vl_donmon.Config.Code );                     
//             	   
//                    /*B Inserer dans la liste des structures config Camera */
//                    ed_ins_liste_Moniteur ( &vl_donmon, sizeof(vl_donmon.Config) ) ;
//                }
//             }
         }
      }
      /*A Fermeture du fichier XDF_Config_Video_<NomMachine>  */
      XZSS_13FermerFichier( vl_Fd );


      /*A Suppression de la liste des donnees des elements ne faisant plus partie de la config camera */
      for ( pl_Camera = pg_debutCamera; pl_Camera != NULL; )
      {
         /* Si le numero de ST est effacer */
         if ( pl_Camera->Config.Nature == 0 )
         {
            /* Suppression de la liste */
            ed_sup_liste_Camera ( pl_Camera->Config.Numero );
            pl_Camera = pg_debutCamera;
         }
         else
         {
            /* Passage a l'element suivant */
            pl_Camera = pl_Camera->Suivant;
         }
      }


      /*A Suppression de la liste des donnees des elements ne faisant plus partie de la config moniteur */
//      for ( pl_Moniteur = pg_debutMoniteur; pl_Moniteur != NULL; )
//      {
//         /* Si le code est efface */
//         if ( pl_Moniteur->Config.Code == 0 )
//         {
//            /* Suppression de la liste */
//            ed_sup_liste_Moniteur ( pl_Moniteur->Config.Numero );
//            pl_Moniteur = pg_debutMoniteur;
//         }
//         else
//         {
//            /* Passage a l'element suivant */
//            pl_Moniteur = pl_Moniteur->Suivant;
//         }
//      }


      /* Ecriture trace de la config pour debug */
      for ( pl_Camera = pg_debutCamera; pl_Camera != NULL; pl_Camera = pl_Camera->Suivant )
      {
          pl_Camera->Etat_Service  = XDC_EQT_EN_SRV;
          pl_Camera->Etat.CRCmdCour = XZEXC_PAS_DE_CMD;
          XZST_03EcritureTrace ( XZSTC_DEBUG3, "Config Camera: %d %d %d %d %d",  
					pl_Camera->Config.Numero,
					pl_Camera->Config.Code,
					pl_Camera->Config.NumeroLTV,
					pl_Camera->Config.Nature,
					pl_Camera->Etat_Service );				
      }

//      for ( pl_Moniteur = pg_debutMoniteur; pl_Moniteur != NULL; pl_Moniteur = pl_Moniteur->Suivant )
//      {
//          pl_Moniteur->Etat_Service  = 0;
//          pl_Moniteur->CamAssocie = 0;
//          XZST_03EcritureTrace ( XZSTC_WARNING, "Config Moniteur: %d %d %s %d",  
//					pl_Moniteur->Config.Numero,
//					pl_Moniteur->Config.Code,
//					pl_Moniteur->Config.PO,
//					pl_Moniteur->Etat_Service );				
//      }

      return ( vl_ValRet );
}





/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction effectue la lecture du fichier XDF_Config_SYSVIDEO_<NomMachine>
*  et met a jour le liste des structure de donnees VIDEO;
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ec_lire_config_SYSVIDEO	( XDY_NomMachine		pa_NomMachine )
			  
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
   static char *version = "$Id: evid_cfg.c,v 1.15 2017/03/27 12:21:09 devgfi Exp $ : ec_lire_config_SYSVIDEO" ;

   int				vl_ValRet = XDC_OK;
   int				vl_Fd;
   char 			pl_PathEtFic[XDC_PATH_ABS_SIZE];
   XDY_Texte			pl_LigneFich;

   XDY_Nom			vl_nom_serveur;
   int				vl_no_port;
   int				vl_no_sysv = EVIDC_SYSV1;
   XDY_Eqt			vl_num_eqt_sysv;
   int				vl_NbParam = 0;

      /*A Initialiser les donnees du Systeme Video */
      memset ( (void *) &vg_sysvid[EVIDC_SYSV1], 0, sizeof(vg_sysvid[0]) );
      memset ( (void *) &vg_sysvid[EVIDC_SYSV2], 0, sizeof(vg_sysvid[0]) );

      /*A Ouverture du fichier de configuration du systeme Video */
      /* Construction du nom du fichier XDF_Config_sysvideo_<NomMachine>  */
      sprintf ( pl_PathEtFic, "%s/%s_%s", XZEXC_PATH_CONFIG, XDF_Config_SYSVIDEO, pa_NomMachine );

      /* Ouverture du fichier XDF_Config_sysvideo_<NomMachine>  */
      if ( (vl_Fd = open (pl_PathEtFic, O_RDONLY)) <= 0 )
      {
         XZST_03EcritureTrace( XZSTC_WARNING, " Manque fichier %s. \n", pl_PathEtFic ); 
         return ( XDC_NOK );
      }

 
      /*A Lecture de chaque ligne du fichier */
      while ( ex_LireLigneFich ( vl_Fd, pl_LigneFich, sizeof ( pl_LigneFich ) ) != XZEXC_EOF ) 
      {
          /* Lire ligne suivante si c'est une ligne de commentaire */
          if ( *pl_LigneFich != XDF_COMMENTAIRE )
          {
                /*B Extraire les parametres systeme video */
                vl_num_eqt_sysv = 0;
                vl_NbParam = sscanf ( pl_LigneFich, 	"%hd %s %d",
                                              &vl_num_eqt_sysv,
                                              vl_nom_serveur,
                                              &vl_no_port);
                if ( vl_NbParam != 3 )
                {
                   XZST_03EcritureTrace(XZSTC_WARNING, "Configuration du Systeme Video %d erronee.\n", (int) vl_num_eqt_sysv); 
                   vl_ValRet = XDC_NOK;
                }
                else if (vl_no_sysv > EVIDC_SYSV2)
                {
                   XZST_03EcritureTrace(XZSTC_WARNING, "Configuration de Systeme Video (%d) superflue, ignoree.\n", (int) vl_num_eqt_sysv);
                   vl_ValRet = XDC_NOK;
                }
                else
                {
                   sprintf (vg_sysvid[vl_no_sysv].Nom, "SYSVIDEO%d", vl_num_eqt_sysv);
                   vg_sysvid[vl_no_sysv].Numero = vl_num_eqt_sysv;
                   strcpy (vg_sysvid[vl_no_sysv].Config.Nom_machine, vl_nom_serveur);
                   vg_sysvid[vl_no_sysv].Config.No_port = vl_no_port;
                   vg_sysvid[vl_no_sysv].Masque = NULL;
                }
                vl_no_sysv = vl_no_sysv + 1;
         }
      }
      /*A Fermeture du fichier XDF_Config_sysVideo_<NomMachine>  */
      XZSS_13FermerFichier( vl_Fd );


      /* Ecriture trace de la config pour debug */
      XZST_03EcritureTrace ( XZSTC_DEBUG3, "Config systeme video: %s/%d, %s/%d",  
					vg_sysvid[EVIDC_SYSV1].Config.Nom_machine, vg_sysvid[EVIDC_SYSV1].Config.No_port,		
					vg_sysvid[EVIDC_SYSV2].Config.Nom_machine, vg_sysvid[EVIDC_SYSV2].Config.No_port);				

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
   static char *version = "$Id: evid_cfg.c,v 1.15 2017/03/27 12:21:09 devgfi Exp $ : ec_controle" ;

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
/*------------------------------------------------------------------------------
* SERVICE RENDU : 
*  Effectue l'initialisation de la tache
*
--------------------------------------------------------------------------------
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
*   Lecture des fichier de configuration des DAi et des Analyseur
*   Remplissage des structures de donnees
*   Connexion socket avec la tache protocole
*
------------------------------------------------------------------------------*/
{
   static char *version = "$Id: evid_cfg.c,v 1.15 2017/03/27 12:21:09 devgfi Exp $ : ec_init" ;

   EVID_DONNEES_SYSTVIDEO	vl_systvideo;
   XZEXT_MASK *			pl_Socket;
   int				vl_SockProt = 0;

   /*A
    * Lecture des fichiers de configuration et remplissage des structure de donnees 
    */
   /*A Configuration des cameras moniteurs et magetos (appel de la fonction ec_lire_config_VIDEO) */
    ec_lire_config_VIDEO ( pa_NomMachine );

   /*A Configuration du systeme video et des ltv (appel de la fonction ec_lire_config_SYSVIDEO)*/
    ec_lire_config_SYSVIDEO ( pa_NomMachine );

   /*A Definition du mode de Fonctionnement initial */
   vg_Mode_Fonct = XZEXC_MODE_NORMAL;


   /*A Si aucune configuration de Systeme video n'est trouvee alors abandonner */
   if ( ! estConfigure (EVIDC_SYSV1)  &&  ! estConfigure (EVIDC_SYSV2) )
   {
      XZST_03EcritureTrace( XZSTC_FATAL, "AUCUNE CONFIGURATION DE SYSTEME VIDEO, ABANDON !!!");
   }


   /*A
   ** Tenter d'etablir la liaison avec chacun des serveurs configures
   */

   if ( estConfigure (EVIDC_SYSV1) )
   {
      if ( ec_sysv_connecter ( EVIDC_SYSV1 ) != XDC_OK )
      {
         XZST_03EcritureTrace( XZSTC_WARNING, "Impossible d'etablir la liaison vers %s.", vg_sysvid[EVIDC_SYSV1].Nom );	
         ec_message_operateurs ( "Impossible d'etablir la liaison vers le Systeme video %d.", EVIDC_SYSV1 );	
      } else {
         ec_message_operateurs ( "Liaison etablie avec le Systeme video %d.", EVIDC_SYSV1 );
      }
   }

   if ( estConfigure (EVIDC_SYSV2) )
   {
      if ( ec_sysv_connecter ( EVIDC_SYSV2 ) != XDC_OK )
      {
         XZST_03EcritureTrace( XZSTC_WARNING, "Impossible d'etablir la liaison vers %s.", vg_sysvid[EVIDC_SYSV2].Nom );	
         ec_message_operateurs ( "Impossible d'etablir la liaison vers le Systeme video %d.", EVIDC_SYSV2 );	
      } else {
         ec_message_operateurs ( "Liaison etablie avec le Systeme video %d.", EVIDC_SYSV2 );
      }
   }

   if ( ! estConnecte (EVIDC_SYSV1)  &&  ! estConnecte (EVIDC_SYSV2) )
   {
         ec_message_operateurs ( "AUCUN ACCES AU SYSTEME VIDEO !!!", 0 );	
   }


   /*A Choisir l'equipement courant utilise */
   vg_noSV = EVIDC_SYSV1;
   if ( ! estConnecte (EVIDC_SYSV1)  &&  estConfigure (EVIDC_SYSV2) )
   {
      vg_noSV = EVIDC_SYSV2;
   }
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
*   Envoyer le message aux taches protocoles
*   Attendre x secondes
*   Fermeture des sockets
*   Liberation des ressources memoires
*
------------------------------------------------------*/
{
   static char *version = "$Id: evid_cfg.c,v 1.15 2017/03/27 12:21:09 devgfi Exp $ : ec_sortir" ;

   EVID_DONNEES_CAMERA		*pl_Camera, 	*pl_CameraALiberer;
   XZEXT_MASK *			pl_Socket;

   /*A Attendre */
   /* sleep (XZEXC_ATTENTE_ARRET_TACHE); */

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

   /* Fermer les connexions au Systeme video */
   if ( estConnecte (EVIDC_SYSV1) )
   {
      ex_fin_cnx ( vg_sysvid[EVIDC_SYSV1].Masque->desc_sock, &vg_sysvid[EVIDC_SYSV1].Masque );
   }
   if ( estConnecte (EVIDC_SYSV2) )
   {
      ex_fin_cnx ( vg_sysvid[EVIDC_SYSV2].Masque->desc_sock, &vg_sysvid[EVIDC_SYSV1].Masque );
   }


  /*A
   * Liberation des ressources memoires
   */

   /*A Liberation des ressources memoires des donnees des Camera */
   for ( pl_Camera = pg_debutCamera; pl_Camera != NULL; free(pl_CameraALiberer) )
   {
      pl_CameraALiberer = pl_Camera;
      pl_Camera         = pl_Camera->Suivant;
   }  

   exit (0);
}





/*X*/
/*------------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Envoi d'un message d'avertissement aux operateurs du site local.
*
--------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ec_message_operateurs (
	char		*pa_message,
	int		va_num_sysV)

/*
* ARGUMENTS EN ENTREE :
*   pa_message		: Texte du message.
*   va_num_sysV		: Identifiant de l'exemplaire du Systeme video emetteur.
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   XDC_OK en cas de succes, XDC_NOK sinon.
*
* CONDITION D'UTILISATION
*  Toutes fonctions.
*
* FONCTION 
*  Emet le message indique a destination des postes du site local.
*
------------------------------------------------------------------------------*/
{
	XZIAT_destinataire	vl_postes_oper_site;
	XDY_Texte		vl_texte_message;

	/*A Constituer le texte du message, avec l'identifiant du Systeme video eventuellement */
	sprintf (vl_texte_message, pa_message, va_num_sysV);

	/*A Puis envoyer le message aux postes du site */
	strcpy (vl_postes_oper_site.site, vg_NomSite);
#if defined (TEST)
strcpy (vl_postes_oper_site.site, "POCI3");
XZIA_06AffMsgSyst (vl_postes_oper_site, vl_texte_message);
strcpy (vl_postes_oper_site.site, "PODR");
#endif
	return (XZIA_06AffMsgSyst (vl_postes_oper_site, vl_texte_message));
}
