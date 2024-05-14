/*E*/
/* Fichier : $Id: ebaf_cfg.c,v 1.7 2020/05/19 14:17:28 pc2dpdy Exp $     Release : $Revision: 1.7 $       Date : $Date: 2020/05/19 14:17:28 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEBAF * FICHIER ebaf_cfg.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module d'initialisation des structures en memoires
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	15/09/05	creation
* JMG   13/11/07        : ajout site de gestion DEM715
* PNI	03/02/09	: ajout champ EDF DEL788 v1.3
* JPL	17/11/09        : Correction numero module (lie a DEM 617) 1.4
* JMG	21/06/10	: portage Linux DEM 934 1.5
* PNI	17/07/12	: nb eqt ligne passe � 21 (au lieu de 19) V1.6
* JMG	01/06/19	: passgae DEM1332 IP 1.7
------------------------------------------------------ */

/* fichiers inclus */

#include "ebaf_cfg.h"

#include <xzsem.h>²

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

/* declaration de fonctions internes */
int ec_lire_config_Module	( EBAF_LISTE_MODULE * , XDY_TypeEqt );
extern int vg_SockTEMIP;
/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction effectue la lecture du fichier XDF_Config_BAF_<NomMachine>
*  et met a jour le liste des structure de donnees BAF;
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ec_lire_config_BAF	( XDY_NomMachine		pa_NomMachine )
			  
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
   static char *version = "@(#)ebaf_cfg.c	1.19 01/10/05      : ec_lire_config_BAF" ;

	int					vl_ValRet = XDC_OK;
	int					vl_Fd,i,vl_NumEqt = 0;
	char 				pl_PathEtFic[XDC_PATH_ABS_SIZE];
	char 				vl_adr[50];
	XDY_Texte			pl_LigneFich;
	char       			pl_message[200];
	int					vl_NbPICTO = 0;
	EBAF_DONNEES_BAF	vl_DonneesBAF = {0},
						*pl_DonCrt;
	short				vl_NumeroLS = 0;
	int					vl_NbParam  = 0;
	XDY_NomST			vl_NomST;
	int					vl_site;
	static char			vl_tabref[XZEXC_nb_module_max_BAF+1][4] = 
	{
		"1.1","1.2","1.5","2.1","2.2","2.5"
	};
	/*   static char			vl_tabref[XZEXC_nb_module_max+1][4] = 
   	{
		"1.1","1.2","1.3","1.4","2.1","2.2","2.3","2.4","3.1","3.2","4.1","5.1","5.5",
			"6.1","6.5","7.1","7.5","8.1","8.5","9.1","9.5","3.3",".9"
   	};*/
   	char				vl_mod[4];
  
	/*A Ouverture du fichier de configuration des PICTO */
	/* Construction du nom du fichier XDF_Config_BAF_<NomMachine>  */
	sprintf ( pl_PathEtFic, "%s/%s_%s", XZEXC_PATH_CONFIG, XDF_Config_BAF, pa_NomMachine );

	/* Ouverture du fichier XDF_Config_BAF_<NomMachine>  */
	if ( (vl_Fd = open (pl_PathEtFic, O_RDONLY)) <= 0 )
	{
		/* Ecriture trace absence fichier */
		XZST_03EcritureTrace( XZSTC_WARNING, " Manque fichier %s. \n", pl_PathEtFic ); 
		/* Sortie de la fonction */
		return ( XDC_NOK );
	}
	/*A Marquage des structures afin de determiner les eventuelles suppression */
	for ( pl_DonCrt = pg_debutBAF; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant )
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
	    	/*B Extraire la config BAF */
	    	if ( *pl_LigneFich == 'N' )
	    	{	       
				/*B Inserer dans la liste des structures config BAF */
				ed_ins_liste_BAF ( &vl_DonneesBAF, sizeof(vl_DonneesBAF.Config) );

				/* Mise en memoire BAF */
				for(i=0;i!=XZEXC_nb_module_max_BAF;i++)
				{  
			  		vl_DonneesBAF.Config.Liste_Module[i].NoModule = 0;
        	        vl_DonneesBAF.Config.Liste_Module[i].Etat_Service  = 0;
        	    }

        	    /*B Extraire les parametre du BAF */
        	    vl_NbParam = sscanf ( pl_LigneFich, 	"N %hd %s %hd %3c %hd %d %d %s",  
										&vl_DonneesBAF.Config.Numero,
										vl_DonneesBAF.Config.NumeroST,
										&vl_NumeroLS,
										vl_DonneesBAF.Config.AdresseRGS,
										&vl_DonneesBAF.Config.Type,
										&vl_site,
										&vl_DonneesBAF.Config.Port,
										vl_DonneesBAF.Config.AdresseIP);
        	    vl_DonneesBAF.Config.NumeroLS = vl_NumeroLS;
	    	   	vl_DonneesBAF.Config.SiteGestion=vl_site;

				if (strcmp(vl_DonneesBAF.Config.AdresseIP,XZECC_IP_VIDE)) 
				{
        	    	if (vg_SockTEMIP==-1) 
					{
        	        	sprintf(vl_adr,"%s%d",XZEXC_ADR_SOCK_TEIM, XDC_EQT_BAF);
        	            ex_cnx_client (	"bafteim",
        	            				vl_adr,
        	            				&vg_SockMask,
        	            				&vg_SockTEMIP);
        	        }
					vl_DonneesBAF.Socket = vg_SockTEMIP;
	
					sprintf (	pl_message, "%s%s %d %c%s",
        	                    XZEXC_CONFIG_IP,
        	                    vl_DonneesBAF.Config.AdresseIP,
        	                    vl_DonneesBAF.Config.Port,
        	                    XZECC_PROT_LCR,
        	                    XZEXC_FIN_MSG_SOCK );
        	        XZST_03EcritureTrace(XZSTC_WARNING,"trame %s",pl_message);
        	        sem_wait(&vg_semaphore); 
        	        if ( write ( vg_SockTEMIP, pl_message, strlen (pl_message) ) == XDC_NOK) 
					{
        	            sem_post(&vg_semaphore); 
        	            XZST_03EcritureTrace(XZSTC_WARNING, " connexion impossible");
        	            return (XDC_NOK);
        	        }
        	        sem_post(&vg_semaphore); 
        	    }

        	    /* Lecture des nom de module des fichiers SBAF */
        	    if ( ec_lire_config_Module ( vl_DonneesBAF.Config.Liste_Module, vl_DonneesBAF.Config.Type ) != XDC_OK )
        	    {
        	    	for(i=0;i!=XZEXC_nb_module_max_BAF;i++)
        	        {  
			     		strcpy(vl_DonneesBAF.Config.Liste_Module[i].Module,vl_tabref[i]);
        	        }
        	    }
        	}
    
	    	/*B Extraire la config module */
	    	if ( *pl_LigneFich == 'M')
	    	{
        	    /*B Extraire les parametre des modules de la BAF */
        	    vl_NbParam = sscanf ( pl_LigneFich, 	"M %d %s",&vl_NumEqt,vl_mod );  

        	    for(i=0;i!=XZEXC_nb_module_max_BAF;i++)
        	    {  
#ifdef _HPUX_SOURCE
                	if ( strcmp (vl_DonneesBAF.Config.Liste_Module[i].Module, vl_mod) == 0 )
#else
					if ( vl_DonneesBAF.Config.Liste_Module[i].Module[0] == vl_mod[0] )
#endif
                	{
	            		vl_DonneesBAF.Config.Liste_Module[i].NoModule = vl_NumEqt;
	                }
	            }
	        }
	    }
	}

    /*B Inserer dans la liste des structures config BAF */
    ed_ins_liste_BAF ( &vl_DonneesBAF, sizeof(vl_DonneesBAF.Config) );
    /*A Fermeture du fichier XDF_Config_BAF_<NomMachine>  */
    XZSS_13FermerFichier( vl_Fd );


    /*A Suppression de la liste des donnees des elements ne faisant plus partie de la config */
    for ( pl_DonCrt = pg_debutBAF; pl_DonCrt != NULL; )
    {
        /* Si le numero de ST est effacer */
        if ( strlen ( pl_DonCrt->Config.NumeroST ) <= 1 )
        {
            /* Suppression de la liste */
            ed_sup_liste_BAF ( pl_DonCrt->Config.Numero );
            pl_DonCrt = pg_debutBAF;
        }
        else
        {
            /* Passage a l'element suivant */
            pl_DonCrt = pl_DonCrt->Suivant;
        }
    }

	/* Ecriture trace de la config pour debug et init de l'etat de service et de l'activite */
	for ( pl_DonCrt = pg_debutBAF; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant )
	{
	    pl_DonCrt->Etat_Service  = 0;
	    pl_DonCrt->Colision  = XDC_FAUX;
	    pl_DonCrt->EDF  = XDC_FAUX;
	    pl_DonCrt->Contraste = XDC_CONTRASTENORMAL;
	    pl_DonCrt->ContrasteCmd = XZEXC_NOCMD;
	    for ( i = 0 ;i < 4; i++ ) pl_DonCrt->Mode[i] = XZEXC_MODE_NORMAL;
	    strcpy(pl_DonCrt->Etat.Scenario,"");
	    XZST_03EcritureTrace ( XZSTC_WARNING, "Config BAF: %d %s %d %s %d %d %d",  
								pl_DonCrt->Config.Numero,
								pl_DonCrt->Config.NumeroST,
								pl_DonCrt->Config.NumeroLS,
								pl_DonCrt->Config.AdresseRGS,
								pl_DonCrt->Config.Type,
								pl_DonCrt->Etat_Service,
								pl_DonCrt->Config.SiteGestion);

	   	for(i=0;i!=XZEXC_nb_module_max_BAF;i++)
	   	{  
	    	strcpy(pl_DonCrt->Etat.Module[i],"0");
	    	pl_DonCrt->EtatCmd.Module[i][0] = '\0';
	      	*pl_DonCrt->Etat.Scenario = '\0';
	      	if (pl_DonCrt->Config.Liste_Module[i].NoModule!=0)
	      	{
	         	XZST_03EcritureTrace ( XZSTC_DEBUG3, "Config liste modules: %d %s ",  
										pl_DonCrt->Config.Liste_Module[i].NoModule,
										pl_DonCrt->Config.Liste_Module[i].Module);
	  		}
	   	}
	}			

    /* Retourne compte rendu OK */
    return ( vl_ValRet );
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction effectue la lecture du fichier XDF_Config_Module_BAF_<NomMachine>
*  et met a jour le liste des structure de donnees type BAF;
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ec_lire_config_Module	( EBAF_LISTE_MODULE * va_Tab_Module, XDY_TypeEqt va_NumType )
			  
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
   static char *version = "@(#)ebaf_cfg.c	1.19 01/10/05      : ec_lire_config_Module" ;

   int				vl_ValRet = XDC_OK;
   int				vl_Fd;
   char 			pl_PathEtFic[XDC_PATH_ABS_SIZE];
   XDY_Texte			pl_LigneFich;
   int				vl_NbParam  = 0;
   XDY_Nom			vl_NomModule = "";
   XDY_Entier			vl_Numero = 0;
  
      /*A Ouverture du fichier de configuration des PICTO */
      /* Construction du nom du fichier XDF_Config_Module_BAF_<Numero de type>  */
      sprintf ( pl_PathEtFic, "%s/%s_%d.cfg", EBAFC_PATH_CONFIG_BAF, XDF_Config_BAF, va_NumType );
      
      /* Ouverture du fichier XDF_Config_Module_BAF_<NomMachine>  */
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
            /*B Extraire les parametre du type BAF */

            vl_NbParam = sscanf ( 	pl_LigneFich, "%d,%[^,],",  
					&vl_Numero,
					vl_NomModule );
	    if ( vl_NbParam == 2 )
	    {
	        if ( (vl_Numero >= 0) && (vl_Numero < XZEXC_nb_module_max_BAF) )
	        {
	           strcpy ( va_Tab_Module[vl_Numero].Module,vl_NomModule );
	           XZST_03EcritureTrace ( XZSTC_DEBUG1, " Module %d : <%s>",vl_Numero, vl_NomModule );
	        }
	        else
	        {
	           vl_ValRet = XDC_NOK;
	           break;
	        }
	    }
					
         }
      }
         
      /*A Fermeture du fichier XDF_Config_Module_BAF_<NomMachine>  */
      XZSS_13FermerFichier( vl_Fd );
      
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
   static char *version = "@(#)ebaf_cfg.c	1.19 01/10/05      : ec_controle" ;

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
*   Lecture des fichier de configuration des BAF et des Picto
*   Remplissage des structures de donnees
*   Connexion socket avec la tache protocole
*
------------------------------------------------------*/
{
   static char *version = "@(#)ebaf_cfg.c	1.19 01/10/05      : ec_init" ;

   EBAF_DONNEES_BAF 		*pl_DonCrt;
   XZEXT_MASK *			pl_Socket;
   XZEXT_ADR_SOCK		pl_TacheTrt;
   XZEXT_ADR_SOCK		pl_TacheProt;
   int				vl_SockProt = 0;
   int				vl_Index = 0;
   short                        vl_NbEqtLigne[21] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
   char                        vl_adr[21];
   char                       pl_message[200];

   /*A
    * Lecture des fichiers de configuration et remplissage des structure de donnees 
    */
   /*A Configuration des BAF (appel de la fonction ec_lire_config_BAF)*/
   if ( ec_lire_config_BAF ( pa_NomMachine ) != XDC_OK) return (XDC_NOK);
   /*A Configuration des scenarios  */
  /* if ( ec_lire_config_Scenar () != XDC_OK) return (XDC_NOK);*/
   /*A Configuration des clignotements */
    ex_lire_cligno ( pa_NomMachine, vg_clignolent, vg_clignorapide );
   /*A Lecture user/password */
    ex_lire_idpassw ( pa_NomMachine, vg_idpassw );
   
   /*A Ouverture du fichier d'alarme */
   ex_OuvertureFichierMsg(vg_NomMachine);
   
   /*A Appel  xzat26_Init_Etats_Eqts pour Initialisation des etats equipement (HS ou Actif) */
   xzat26_Init_Etats_Eqts ( XDC_EQT_BAF, XDC_ETAT_TOUS_EQT, pa_NomMachine );
   xzat26_Init_Etats_Eqts ( XDC_EQT_MODBAF, XDC_ETAT_TOUS_EQT, pa_NomMachine );

   /*A
    * Connexion socket en mode client pour communiquer avec la tache TELMi
    */
   /*A Pour tout les BAF rechercher si la tache protocole associee existe */
   for ( pl_DonCrt = pg_debutBAF; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant )
   {
      XZST_03EcritureTrace( XZSTC_WARNING,"IP %s",pl_DonCrt->Config.AdresseIP);
      /* Construction du nom de la tache protocole telmi*/
      sprintf ( pl_TacheProt, "%s%d", XZEXC_ADR_SOCK_TELM, pl_DonCrt->Config.NumeroLS );
      sprintf ( pl_TacheTrt, "%s%d", XZEXC_ADR_SOCK_BAFTELM, pl_DonCrt->Config.NumeroLS );
      vl_NbEqtLigne[pl_DonCrt->Config.NumeroLS]++;
      /*A Recherche sur tout le masque des socket si la socket exite */
      for ( pl_Socket = vg_SockMask; pl_Socket != NULL; pl_Socket = pl_Socket->Suivant )
      {
	 if ((!strcmp(pl_DonCrt->Config.AdresseIP,XZECC_IP_VIDE))) {
         if ( !strcmp ( pl_Socket->adresse, pl_TacheProt )  )
         {
            /* Mise a jour du numero de socket */
            pl_DonCrt->Socket = pl_Socket->desc_sock;
            /*B Arret: la socket avec la tache existe */
            break;
         }
	 }
	 else
	 {
		pl_DonCrt->Socket = vg_SockTEMIP;
		break;
      }
      }
      
      /*A Si la socket avec la tache telmi n'existe pas */
      if ( pl_Socket == NULL )
      {
	if (!strcmp(pl_DonCrt->Config.AdresseIP,XZECC_IP_VIDE))  { /*BAF NON IP*/

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
	    pl_DonCrt->Socket = vl_SockProt;
	 }	
	 }
	else { /*BAF IP*/
               sprintf(vl_adr,"%s%d",XZEXC_ADR_SOCK_TEIM,XDC_EQT_BAF);
                if (ex_cnx_client (    "bafteim",
                                vl_adr,
                                &vg_SockMask,
                                &vg_SockTEMIP) != XDC_OK )
                {
                        XZST_03EcritureTrace( XZSTC_WARNING,
                                "impossible de creer la connexion TEMIP");
                }
                else {
                        pl_DonCrt->Socket = vg_SockTEMIP;
                        sprintf ( pl_message, "%s%s %d %c%s",
                                      XZEXC_CONFIG_IP,
                                      pl_DonCrt->Config.AdresseIP,
                                      pl_DonCrt->Config.Port,
                                      XZECC_PROT_LCR,
                                      XZEXC_FIN_MSG_SOCK );
                        sem_wait(&vg_semaphore); 
                        write ( vg_SockTEMIP, pl_message,
                                        strlen (pl_message));
                        sem_post(&vg_semaphore); 
		
      		}
      	}
      }
   }
   /*A Initialisation memoire */
   /*A Pour tous les pmv et les picto -> demande d'etat d'affichage */
   for ( pl_DonCrt = pg_debutBAF; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant )
   {
       if (!strcmp(pl_DonCrt->Config.AdresseIP,XZECC_IP_VIDE))
       		EnvoyerTrame(     "PS",
                         XZEXC_TRAME_ETAT_POS_BAF,
                         pl_DonCrt->Config.AdresseRGS,
                         XZEXC_NOCMD,
	                 XZEXC_PRIORITEFAIBLE,
                         pl_DonCrt->Socket );
	else
       		EnvoyerTrameIP(     "PS",
                         XZEXC_TRAME_ETAT_POS_BAF,
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
   static char *version = "@(#)ebaf_cfg.c	1.19 01/10/05      : ec_sortir" ;

   EBAF_DONNEES_BAF 		*pl_BAF, 	*pl_BAFALiberer;
   XZEXT_MASK *			pl_Socket;
   XZEXT_ADR_SOCK		pl_TacheProt,
   				pl_TacheTrt;
   XZEXT_MSG_SOCKET 		pl_message;
   XDY_Datagroup		pl_NomDG;
   int				vl_resultat;

   /*A Desabonnement a JourNuit */
   XDG_EncodeDG2 ( pl_NomDG, "Einit_JourNuit_BAF", vg_NomMachine) ;			
   /*XZEA29_Abt_Etat_JourNuit(XZEAC_FERMER_ABONNEMENT,pl_NomDG,ep_journuit,vg_NomSite,&vl_resultat);*/

  /*A
   * Liberation des ressources memoires
   */

   /*A Liberation des ressources memoires des donnees des BAF */
   for ( pl_BAF = pg_debutBAF; pl_BAF != NULL; free(pl_BAFALiberer) )
   {
      pl_BAFALiberer = pl_BAF;
      pl_BAF         = pl_BAF->Suivant;
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
