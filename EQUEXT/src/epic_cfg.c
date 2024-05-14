/*E*/
/*Fichier : $Id: epic_cfg.c,v 1.2 2023/08/18 08:19:06 pc2dpdy Exp $        Release : $Revision: 1.2 $        Date : $Date: 2023/08/18 08:19:06 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEPIC * FICHIER epic_cfg.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module de configuration de la tache TEPICTO
*
------------------------------------------------------
* HISTORIQUE :
*
* ABK	18/08/23		: Creation (DEM/483)
------------------------------------------------------*/

/* fichiers inclus */
#include <xzsem.h>

#include "epic_cfg.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

/* declaration de fonctions internes */

/* definition de fonctions externes */




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
* init ou reinit de la config picto 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ec_lire_config_PIC	( XDY_NomMachine		pa_NomMachine )
			  
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
*  Cette fonction effectue la lecture du fichier XDF_Config_PIC_<NomMachine>
*  et met a jour le liste des structure de donnees PIC;
*   
------------------------------------------------------*/
{
   static char *version = "@(#)epic_cfg.c	1.20 03/10/98 : ec_lire_config_PIC" ;

   int				vl_ValRet = XDC_OK;
   int				vl_Fd;
   char 			pl_PathEtFic[XDC_PATH_ABS_SIZE];
   XDY_Texte			pl_LigneFich;
   int				vl_NbPIC = 0;
   EPIC_DONNEES_PIC		vl_DonneesPIC = {0},
   				*pl_DonCrt;
   char 			vl_Typ_Eqt;
   short			vl_NumeroLS = 0;
   int				vl_NbParam  = 0;
   int				vl_site;
   char				vl_temp;
   int	vl_num_eqt,vl_num_ls, vl_type;
   XDY_PortIP	vl_port;
   char vl_adr[50];
   char pl_message[200];

      /*A Ouverture du fichier de configuration des PIC */
      /* Construction du nom du fichier XDF_Config_PIC_<NomMachine>  */
      sprintf ( pl_PathEtFic, "%s/%s_%s", XZEXC_PATH_CONFIG, XDF_Config_PIC, pa_NomMachine );
      
      /*A Ouverture du fichier XDF_Config_PIC_<NomMachine>  */
      if ( (vl_Fd = open (pl_PathEtFic, O_RDONLY)) <= 0 )
      {
         /* Ecriture trace absence fichier */
         XZST_03EcritureTrace( XZSTC_WARNING, " Manque fichier %s. \n", pl_PathEtFic ); 
         /* Sortie de la fonction */
         return ( XDC_NOK );
      }
      /*A Marquage des structures afin de determiner les eventuelles suppression */
      for ( pl_DonCrt = pg_debutPIC; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant )
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

		/*B Extraire les parametre du PIC */
		XZST_03EcritureTrace ( XZSTC_FONCTION, "ligne %s",pl_LigneFich);
		vl_NbParam = sscanf ( pl_LigneFich, 	" %c %d %d %3c %s %d %d %d %d %s %d",  
						&vl_temp,
						&vl_num_eqt, /*&vl_DonneesPIC.Config.Numero.NoEqt,*/
						&vl_num_ls /*&vl_NumeroLS*/,
						vl_DonneesPIC.Config.AdresseRGS ,
						vl_DonneesPIC.Config.NumeroST,
						&vl_Typ_Eqt,
						&vl_site,
						vl_DonneesPIC.Config.AdresseIP,
						&vl_port);
XZST_03EcritureTrace ( XZSTC_FONCTION, "NOMBRE VALEURS LUES : %d",vl_NbParam);
		vl_DonneesPIC.Config.SiteGestion=vl_site;
		vl_DonneesPIC.Config.Numero.Typ_Eqt  = vl_Typ_Eqt;
		vl_DonneesPIC.Config.NumeroLS = vl_num_ls;
		vl_DonneesPIC.Config.Numero.NoEqt = vl_num_eqt;
		vl_DonneesPIC.Config.Port = vl_port;

		XZST_03EcritureTrace ( XZSTC_FONCTION, "Config PIC: %d %s %d %s %s %s %d",  
							vl_DonneesPIC.Config.Numero.NoEqt,
							vl_DonneesPIC.Config.NumeroST,
							vl_DonneesPIC.Config.NumeroLS,
							vl_DonneesPIC.Config.AdresseRGS,
							vl_DonneesPIC.Config.SiteGestion,
							vl_DonneesPIC.Config.AdresseIP,
							vl_DonneesPIC.Config.Port );

		if (strcmp(vl_DonneesPIC.Config.AdresseIP,XZECC_IP_VIDE)) {
			if (vg_SockTEMIP==-1) {
				sprintf(vl_adr,"%s%d",XZEXC_ADR_SOCK_TEIM,XDC_EQT_PIC);
				ex_cnx_client ("picteim",vl_adr,
						&vg_SockMask,
						&vg_SockTEMIP);
			}

			vl_DonneesPIC.Socket = vg_SockTEMIP;
			sprintf ( pl_message, "%s%s %d %c%s",
					XZEXC_CONFIG_IP,
					vl_DonneesPIC.Config.AdresseIP,
					vl_DonneesPIC.Config.Port,
					XZECC_PROT_LCR,
					XZEXC_FIN_MSG_SOCK );
			XZST_03EcritureTrace(XZSTC_WARNING,"trame %s",pl_message);     
			sem_wait(&vg_semaphore);                                                        
			if ( write ( vg_SockTEMIP, pl_message, strlen (pl_message) ) == XDC_NOK) {          
				sem_post(&vg_semaphore);                                   
				XZST_03EcritureTrace(XZSTC_WARNING, " connexion impossible");                                                 
				return (XDC_NOK);
			}
			sem_post(&vg_semaphore); 
		}

		/* Si tout les parametre ont ete lu */
		if ( vl_NbParam == 12 )
		{
			XZST_03EcritureTrace ( XZSTC_WARNING,"insertion");
			if (pg_debutPIC==NULL) {
				pg_debutPIC = (EPIC_DONNEES_PIC *)malloc(sizeof(EPIC_DONNEES_PIC));
				pg_debutPIC->Config = vl_DonneesPIC.Config;
				pg_debutPIC->Suivant = NULL;
				strcpy(pg_debutPIC->Trame,"");
				pl_DonCrt = pg_debutPIC;		
			}
			else {
				pg_debutPIC->Suivant = (EPIC_DONNEES_PIC *)malloc(sizeof(EPIC_DONNEES_PIC));
				pg_debutPIC = pg_debutPIC->Suivant; 		
				pg_debutPIC->Config = vl_DonneesPIC.Config;
				pg_debutPIC->Suivant = NULL;
				strcpy(pg_debutPIC->Trame,"");
			}

			/*B Inserer dans la liste des structures config PIC */
			if ( ed_ins_liste_PIC ( &vl_DonneesPIC, sizeof(vl_DonneesPIC.Config) ) == XDC_OK )
			{
				XZST_03EcritureTrace ( XZSTC_WARNING,"%d",vl_NbPIC);
				vl_NbPIC++;
			}
		}
	else
		XZST_03EcritureTrace ( XZSTC_WARNING,"pb decodage");
	}
      }
      pg_debutPIC = pl_DonCrt;

      /*A Fermeture du fichier XDF_Presence_Equip_<NomMachine>  */
      XZSS_13FermerFichier( vl_Fd );
      
      /*A Suppression de la liste des donnees des elements ne faisant plus partie de la config */
      /*
      for ( pl_DonCrt = pg_debutPIC; pl_DonCrt != NULL; )
      {
         if ( strlen ( pl_DonCrt->Config.NumeroST ) <= 1 )
         {
            ed_sup_liste_PIC ( pl_DonCrt->Config.Numero.NoEqt );
            pl_DonCrt = pg_debutPIC;
         }
         else
         {
            pl_DonCrt = pl_DonCrt->Suivant;
         }
      }*/
     
      /* Ecriture trace de la config pour debug */
      for ( pl_DonCrt = pg_debutPIC; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant )
      {
          /*pl_DonCrt->Etat_Service  = 0;
          pl_DonCrt->Contraste = XDC_CONTRASTENORMAL;
          pl_DonCrt->Mode = XZEXC_MODE_NORMAL;

	  strcpy ( pl_DonCrt->Etat.Vitesse, "XXXX" );
	  strcpy ( pl_DonCrt->Etat.Bandeau, "" );

	  strcpy ( pl_DonCrt->Etat_Commande.Bandeau, "" );
	  strcpy ( pl_DonCrt->Etat_Commande.Vitesse, "XXXX" );*/

	
		XZST_03EcritureTrace ( XZSTC_FONCTION, "Config PIC2: %d %s %d %s %s %s %d",  
							vl_DonneesPIC.Config.Numero.NoEqt,
							vl_DonneesPIC.Config.NumeroST,
							vl_DonneesPIC.Config.NumeroLS,
							vl_DonneesPIC.Config.AdresseRGS,
							vl_DonneesPIC.Config.SiteGestion,
							vl_DonneesPIC.Config.AdresseIP,
							vl_DonneesPIC.Config.Port );
      }

      /* Retourne compte rendu OK */
      return ( vl_ValRet );
}





/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction effectue la lecture du fichier XDF_Affichage_Def_PIC
*  (affichage par defaut PIC pour mise au neutre )
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ec_lire_Affichage_Def_PIC	( void )

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
*
* FONCTION 
*   
*
------------------------------------------------------*/
{
   static char *version = "@(#)epic_cfg.c	1.20 03/10/98 : ec_lire_Affichage_Def_PIC" ;

   int				vl_Fd;
   char 			pl_PathEtFic[XDC_PATH_ABS_SIZE];
   XDY_Eqt			vl_NumPIC = 0;
   char *			vl_Mot = NULL;
   XDY_Texte			pl_LigneFich;
   XDY_Texte			pl_LigneFichMem;
   EPIC_DONNEES_PIC		*pl_DonCrt;
   XDY_Nom			vl_NomPICFic  = "";			
   XDY_Nom			vl_NomPIC     = "";
   XDY_NomMachine		vl_NomMachine = "";			
   int				vl_index = 0;

      /*A Ouverture du fichier des affichage par defaut des PIC */
      /* Construction du nom du fichier XDF_Affichage_Def_PIC  */
      sprintf ( pl_PathEtFic, "%s/%s", XZEXC_PATH_CONFIG, XDF_Affichage_Def_PIC );
      
      /* Ouverture du fichier XDF_Affichage_Def_PIC  */
      if ( (vl_Fd = open (pl_PathEtFic, O_RDONLY)) <= 0 )
      {
         /* Ecriture trace absence fichier */
         XZST_03EcritureTrace( XZSTC_WARNING, " Manque fichier %s. \n", pl_PathEtFic ); 
         /* Sortie de la fonction */
         return ;
      }
       
      /*A Lecture de chaque ligne du fichier */
      while ( ex_LireLigneFich ( vl_Fd, pl_LigneFich, sizeof ( pl_LigneFich ) ) != XZEXC_EOF ) 
      {
         /* Lire ligne suivante si c'est une ligne de commentaire */
         if ( *pl_LigneFich != XDF_COMMENTAIRE )
         {
            /*B Extraire les parametres  */
            strcpy ( pl_LigneFichMem, pl_LigneFich );
            strcpy ( vl_NomPICFic, ((vl_Mot = strtok(pl_LigneFich, ",")) != NULL) ? vl_Mot : "" );
            if ( strlen(vl_NomPICFic) != 0 )
            {
               for ( pl_DonCrt = pg_debutPIC; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant )
               {
                  ex_nom_equipement ( 	XDC_EQT_PIC, 
                  			pl_DonCrt->Config.Numero.NoEqt, 
                  			NULL,
                  			vl_NomPIC,
                  			vl_NomMachine );
		  if ( strstr(vl_NomPIC, vl_NomPICFic) != NULL )
		     break;
	       }
               strcpy ( pl_LigneFich, pl_LigneFichMem );
               strcpy ( vl_NomPICFic, ((vl_Mot = strtok(pl_LigneFich, ",")) != NULL) ? vl_Mot : "" );
               vl_index = 0;
	       while ( (pl_DonCrt != NULL) && (vl_Mot = strtok ( NULL, "," )) != NULL )
	       {
		  if ( strlen(vl_Mot) < XDC_Lon_Picto_PIC )
		  {
		     strcpy ( pl_DonCrt->AffDefaut[vl_index], vl_Mot );
		  }
		  else
		  {
		     strncpy ( pl_DonCrt->AffDefaut[vl_index], vl_Mot, XDC_Lon_Picto_PIC-1 );
		     pl_DonCrt->AffDefaut[vl_index][XDC_Lon_Picto_PIC-1] = '\0';
		  }
		  if ( (atoi(pl_DonCrt->AffDefaut[vl_index]) != 0) || 
		       (vl_index++ > sizeof(pl_DonCrt->AffDefaut)/XDC_Lon_Picto_PIC) )
		      break;
	       }
            }
         }
      }

      /*A Fermeture du fichier XDF_Presence_Equip_<NomMachine>  */
      XZSS_13FermerFichier( vl_Fd );
      
      /* Ecriture trace de la config pour debug */
      for ( pl_DonCrt = pg_debutPIC; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant )
          XZST_03EcritureTrace ( XZSTC_WARNING, "Affichage par defaut PIC : %hd\t--%s--%s--%s--%s--%s",
               				pl_DonCrt->Config.Numero.NoEqt,
					pl_DonCrt->AffDefaut[0],
					pl_DonCrt->AffDefaut[1],
					pl_DonCrt->AffDefaut[2],
					pl_DonCrt->AffDefaut[3],
					pl_DonCrt->AffDefaut[4] );				

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
   static char *version = "@(#)epic_cfg.c	1.20 03/10/98 : ec_controle" ;

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
  	     ec_init ( pa_NomMachine, &vl_MaxEqtLigne  );
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
*   Lecture des fichier de configuration des PMV et des Picto
*   Remplissage des structures de donnees
*   Connexion socket avec la tache protocole
*
------------------------------------------------------*/
{
   static char *version = "@(#)epic_cfg.c	1.20 03/10/98 : ec_init" ;
   EPIC_DONNEES_PIC *		pl_DonCrtPIC;
   int				vl_Index = 0;
   short			vl_NbEqtLigne[21] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

   XZST_03EcritureTrace( XZSTC_FONCTION, "pa_NomMachine %s",pa_NomMachine);
    ex_lire_idpassw ( pa_NomMachine, vg_idpassw );
   /*A
    * Lecture des fichiers de configuration et remplissage des structure de donnees 
    */
   /*A Configuration des PIC (appel de la fonction ec_lire_config_PIC) */
    if (ec_lire_config_PIC ( pa_NomMachine ) == XDC_NOK) return (XDC_NOK);
   /*A Configuration des clignotements */
    /*ex_lire_cligno ( pa_NomMachine, vg_clignolent, vg_clignorapide );*/
   /*A Lecture user/password */
    
   /*A Lecture fichier affichage par par defaut */
/*    ec_lire_Affichage_Def_PIC();*/
    
   /*A Ouverture du fichier d'alarme */
   ex_OuvertureFichierMsg(vg_NomMachine);
   
   /*A Appel  xzat26_Init_Etats_Eqts pour Initialisation des etats equipement (HS ou Actif) */
   xzat26_Init_Etats_Eqts ( XDC_EQT_PIC, XDC_ETAT_TOUS_EQT, vg_NomMachine );
   
   /*A
    * Connexion socket en mode client par appel de ec_init_cnx
    */
   ec_init_cnx ();

   /*A						
    * Calcul du nombre maxi de PIC par ligne
    */
   for ( pl_DonCrtPIC = pg_debutPIC; pl_DonCrtPIC != NULL; pl_DonCrtPIC = pl_DonCrtPIC->Suivant )
      vl_NbEqtLigne[pl_DonCrtPIC->Config.NumeroLS]++;
   for ( vl_Index = 0; vl_Index < 21; vl_Index ++) 
      if ( *va_MaxEqtLigne < vl_NbEqtLigne[vl_Index] ) *va_MaxEqtLigne = vl_NbEqtLigne[vl_Index];



   return ( XDC_OK );
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Effectue la connection avec avec la tache protocole
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ec_init_cnx	( )

/*
* ARGUMENTS EN ENTREE :
*
*  aucun
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
*   Connexion socket avec la tache protocole
*
------------------------------------------------------*/
{
   static char *version = "@(#)epic_cfg.c	1.20 03/10/98 : ec_init_cnx" ;

   EPIC_DONNEES_PIC *		pl_DonCrtPIC;
   XZEXT_MASK *			pl_Socket;
   XZEXT_ADR_SOCK		pl_TacheTrt;
   XZEXT_ADR_SOCK		pl_TacheProt;
   int				vl_SockProt = 0;
   char 			vl_adr[50];
   char				pl_message[50];

   XZST_03EcritureTrace( XZSTC_FONCTION, "ec_init_cnx : IN" );
   /*A
    * Connexion socket en mode client pour communiquer avec la tache TELMi
    */
   /*A Pour tout les PIC rechercher si la tache protocole associee existe */
   for ( pl_DonCrtPIC = pg_debutPIC; pl_DonCrtPIC != NULL; pl_DonCrtPIC = pl_DonCrtPIC->Suivant )
   								{
      XZST_03EcritureTrace( XZSTC_WARNING,  "Adresse IP :%s Port:%d",pl_DonCrtPIC->Config.AdresseIP, pl_DonCrtPIC->Config.Port);
      /* Construction du nom de la tache protocole telmi*/
      sprintf ( pl_TacheProt, "%s%d", XZEXC_ADR_SOCK_TELM, pl_DonCrtPIC->Config.NumeroLS );
      sprintf ( pl_TacheTrt, "%s%d", XZEXC_ADR_SOCK_PICTELM, pl_DonCrtPIC->Config.NumeroLS );
      /*A Recherche sur tout le masque des socket si la socket exite */
      for ( pl_Socket = vg_SockMask; pl_Socket != NULL; pl_Socket = pl_Socket->Suivant )
      {
	if ((!strcmp(pl_DonCrtPIC->Config.AdresseIP,XZECC_IP_VIDE)))
	{
		if ( !strcmp ( pl_Socket->adresse, pl_TacheProt ) )
		 {
		    /* Mise a jour du numero de socket */
		    pl_DonCrtPIC->Socket = pl_Socket->desc_sock;
	      	    EnvoyerTrame("PS",
				 XZEXC_TRAME_ETAT_AFF_PIC,
				 pl_DonCrtPIC->Config.AdresseRGS,
				 XZEXC_CMD_INIT,
				 XZEXC_PRIORITEFAIBLE,
				 pl_DonCrtPIC->Socket );
		    pl_DonCrtPIC->EtatTrans = EPICC_EQT_OCCUP;
		    /*B Arret: la socket avec la tache existe */
		    break;
		 }
	}
	else/* Envoie trame IP */
	{		
		pl_DonCrtPIC->Socket = vg_SockTEMIP;
		EnvoyerTrameIP(	"PS", 
				XZEXC_TRAME_ETAT_AFF_PIC, 
				pl_DonCrtPIC->Config.AdresseIP, 
				pl_DonCrtPIC->Config.Port, 
				pl_DonCrtPIC->Config.AdresseRGS, 
				XZEXC_NOCMD, 
				XZEXC_PRIORITEFAIBLE, 
				pl_DonCrtPIC->Socket );
		pl_DonCrtPIC->EtatTrans = EPICC_EQT_OCCUP; 
        	break; 
	}
      }
      
	/*A Si la socket avec la tache telmi n'existe pas */
	if ( pl_Socket == NULL )
	{
		if ((!strcmp(pl_DonCrtPIC->Config.AdresseIP,XZECC_IP_VIDE))) /*PIC NON IP*/
		{
			/*A Appel de ex_cnx_client pour etablir la liaison avec la tache protocole */
			if ( ex_cnx_client ( 	pl_TacheTrt,
						pl_TacheProt,
						&vg_SockMask,
						&vl_SockProt ) != XDC_OK )
			{
				/*B Ecriture Trace */
				XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de creer la connexion client (%s)(%s)",pl_TacheTrt, pl_TacheProt );
				pl_DonCrtPIC->EtatTrans = EPICC_EQT_PANNE;
			}
			else
			{
				/* Memorisation du numero de socket */
				pl_DonCrtPIC->Socket = vl_SockProt;						
				EnvoyerTrame(      "PS",
					XZEXC_TRAME_ETAT_AFF_PIC,
					pl_DonCrtPIC->Config.AdresseRGS,
					XZEXC_CMD_INIT,
					XZEXC_PRIORITEFAIBLE,
					pl_DonCrtPIC->Socket );
				pl_DonCrtPIC->EtatTrans = EPICC_EQT_OCCUP;
				XZST_03EcritureTrace( XZSTC_WARNING, "la connexion client effectuee ." );
			}								
		}
		else /* PIC IP */
		{
			if (ex_cnx_client ("picteim", 
						vl_adr, 
						&vg_SockMask, 						
						&vg_SockTEMIP) != XDC_OK ) 				
			{ 
				XZST_03EcritureTrace( XZSTC_WARNING,"impossible de creer la connexion TEMIP"); 
				pl_DonCrtPIC->EtatTrans = EPICC_EQT_PANNE;
			} 
			else 
			{ 
				pl_DonCrtPIC->Socket = vg_SockTEMIP; 
				sprintf ( pl_message, "%s%s %d %c%s", 						
						XZEXC_CONFIG_IP, 
						pl_DonCrtPIC->Config.AdresseIP,
						pl_DonCrtPIC->Config.Port,
						XZECC_PROT_LCR,
						XZEXC_FIN_MSG_SOCK ); 
				sem_wait(&vg_semaphore); 
				write ( vg_SockTEMIP, pl_message, 
				strlen (pl_message)); 
				sem_post(&vg_semaphore); 
				EnvoyerTrameIP(	"PS", 
						XZEXC_TRAME_ETAT_AFF_PIC, 
						pl_DonCrtPIC->Config.AdresseIP, 
						pl_DonCrtPIC->Config.Port, 
						pl_DonCrtPIC->Config.AdresseRGS, 
						XZEXC_NOCMD, 
						XZEXC_PRIORITEFAIBLE, 
						pl_DonCrtPIC->Socket );
				pl_DonCrtPIC->EtatTrans = EPICC_EQT_OCCUP; 
			}								
		}
   	}
   }

   XZST_03EcritureTrace( XZSTC_FONCTION, "ec_init_cnx : OUT" );
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
   static char *version = "@(#)epic_cfg.c	1.20 03/10/98 : ec_sortir" ;

   EPIC_DONNEES_PIC 		*pl_PIC, 	*pl_PICLiberer;
   XZEXT_MASK *			pl_Socket;
   XZEXT_ADR_SOCK		pl_TacheProt,
   				pl_TacheTrt;
   XZEXT_MSG_SOCKET 		pl_message;
   XDY_Datagroup		pl_NomDG;
   int				vl_resultat;

   /*A Desabonnement a JourNuit */
   XDG_EncodeDG2 ( pl_NomDG, "Einit_JourNuit_PIC", vg_NomMachine) ;			
   XZEA29_Abt_Etat_JourNuit(XZEAC_FERMER_ABONNEMENT,pl_NomDG,ep_journuit,vg_NomSite,&vl_resultat);

  /*A
   * Liberation des ressources memoires
   */

   /*A Liberation des ressources memoires des donnees PIC */
   for ( pl_PIC = pg_debutPIC; pl_PIC != NULL; free(pl_PICLiberer) )
   {
      pl_PICLiberer = pl_PIC;
      pl_PIC         = pl_PIC->Suivant;
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
