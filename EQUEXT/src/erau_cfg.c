/*E*/
/*Fichier :  $Id: erau_cfg.c,v 1.14 2020/11/03 18:12:22 pc2dpdy Exp $      Release : $Revision: 1.14 $        Date : $Date: 2020/11/03 18:12:22 $ ------------------------------------------------------
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TERAU * FICHIER erau_cfg.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module d'init et de reinit, de controle et d'exit de la tache TERAU
*
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E		version 1.1	30 Nov 1994		: Creation
* Nagiel.E		version 1.2	24 Jan 1995		:
* Mismer.D		version 1.3	26 Jan 1995		:
* Mismer.D		version 1.4	26 Jan 1995		:
* Fontaine.C	version 1.5	22 Mai 1995		: Reprise entete, historique
* Mismer.D		version 1.6	06 Nov 1995		: Suppression NumAction
* Mismer.D		version 1.7	24 Oct 1996		: Modif pour Alerte PAU HS (DEM/12..)
* Diaz.L		version 1.8	09 Oct 1998     : Modif du au PC Niv.2
* Niepceron		version 1.11	08 juil 08	: RAU au CI suppression de la bidouille pour pc niveau 2 DEM716 (liaison bascul)
* JMG			version 1.12	29/04/16	: connection base de donnees (basculement RAU) 
* JMG			version 1.13 	06/05/19	: ajout type DEM1334
* GR            version 1.14    08/07/2020	: EMS
* GGY							07/09/23	: Passage des RAU en IP : ajout de adresse_ip_esclave, port_ip_esclave, adresse_ip_maitre, port_ip_maitre (DEM483)
* GGY							07/09/23	: Connexion sur teim12 pour les RAU IP (DEM483)
------------------------------------------------------*/

/* fichiers inclus */
#include <xzsem.h>

#include "erau_cfg.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */
T_Liste_Zones *pg_Zones = NULL;

/* declaration de variables locales */

static char *version = "$Id: erau_cfg.c,v 1.14 2020/11/03 18:12:22 pc2dpdy Exp $      : erau_cfg" ;

/* declaration de fonctions internes */
int ec_ajouter_zone( char *, char *, char *, XDY_District);

/* definition de fonctions externes */

int ec_ajouter_zone (char *va_zone, char *va_libelle, char *va_etat, XDY_District va_site){
  T_Liste_Zones 	*pl_NewZone;
  T_Liste_Zones		*pl_Zones;

  XZST_03EcritureTrace( XZSTC_WARNING,"ec_ajouter_zone : zone %s, libelle %s, etat %s, sit %d",
  				va_zone,va_libelle,va_etat,va_site);

  /*A Allocation memoire pour la nouvelle structure */
  if (( pl_NewZone = (T_Liste_Zones *) malloc ( sizeof(T_Liste_Zones) )) == NULL)
  {
         /* Ecriture trace */
         XZST_03EcritureTrace ( XZSTC_WARNING, " OUT: creer_liste_zones Allocation memoire impossible");
         return XDC_NOK;
 }

  strcpy(pl_NewZone->zone.zone,va_zone);
  strcpy(pl_NewZone->zone.libelle,va_libelle);
  strcpy(pl_NewZone->zone.etat,va_etat);
  pl_NewZone->zone.site = va_site;
  pl_NewZone->zone.compteur = 0;
  pl_NewZone->suivant = NULL;
  pl_NewZone->suivant = pg_Zones;
  pg_Zones = pl_NewZone;

  XZST_03EcritureTrace( XZSTC_WARNING,"ec_ajouter_zone : out");
  return (XDC_OK);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction effectue la lecture du fichier XDF_Config_RAU_<NomMachine>
*  et met a jour le liste des structure de donnees RAU;
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ec_lire_config_RAU	( XDY_NomMachine		pa_NomMachine )
			  
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
static char *version = "$Id: erau_cfg.c,v 1.14 2020/11/03 18:12:22 pc2dpdy Exp $      : ec_lire_config_RAU" ;

   XDY_Entier				vl_ValRet = XDC_OK;
   int				vl_Fd;
   FILE                  	*vl_Fic = NULL;
   char 			pl_PathEtFic[XDC_PATH_ABS_SIZE];
   XDY_Texte			pl_LigneFich;
   XDY_Mot			vl_TimeOut = 0;
   XDY_Mot			vl_TimeOutRetour = 0;
   XDY_Mot			vl_NbQuart = 0;
   
   ERAU_DONNEES_PAU		*pl_ListePAU, vl_ListePAU;
   XDY_NomSite			vl_NomSite = "";
   int				vl_NbParam  = 0;
   
      if ( ( vl_ValRet = XZAG02_Connexion_Base(vg_NomSite) ) != XDC_OK )
	{
	       XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de se connecter a la base erreur : %d.", vl_ValRet );
	     ec_sortir();
	     return (XDC_NOK);
	}

      /*recuperation des zones de basculement RAU*/
      if (XZAO910_Liste_Zones_RAU((XDY_FonctionInt)ec_ajouter_zone,XDC_BASE_CFG,XZSC_22NumeroSiteLocal(),&vl_ValRet)!= XDC_OK) {
	XZST_03EcritureTrace( XZSTC_WARNING, "ec_lire_config_rau : pb appel a XZAO910_Liste_Zones_RAU");
	ec_sortir();
	return (XDC_NOK);
      }

      /*A Ouverture du fichier de configuration des Video */
      /* Construction du nom du fichier XDF_Config_video_<NomMachine>  */
      sprintf ( pl_PathEtFic, "%s/%s_%s", XZEXC_PATH_CONFIG, XDF_Config_RAU, pa_NomMachine );
      
      /* Ouverture du fichier XDF_Config_RAU_<NomMachine>  */
      if ( (vl_Fd = open (pl_PathEtFic, O_RDONLY)) <= 0 )
      {
         /* Ecriture trace absence fichier */
         XZST_03EcritureTrace( XZSTC_WARNING, " Manque fichier %s. \n", pl_PathEtFic ); 
         /* Sortie de la fonction */
         return ( XDC_NOK );
      }
      /*A Marquage des structures afin de determiner les eventuelles suppression de cameras */
      for ( pl_ListePAU = pg_debutPAU; pl_ListePAU != NULL; pl_ListePAU = pl_ListePAU->Suivant )
      {
      	 /* Effacement Nature pour marquage */
         pl_ListePAU->Config.CodeAntenne = 0;
      }

      /*A Lecture de chaque ligne du fichier */
      while ( ex_LireLigneFich ( vl_Fd, pl_LigneFich, sizeof ( pl_LigneFich ) ) != XZEXC_EOF ) 
      {
          /* Lire ligne suivante si c'est une ligne de commentaire */
          if ( *pl_LigneFich != XDF_COMMENTAIRE )
          {  

	     /*B Si c'est une ligne de config RAU */
             if (strstr(pl_LigneFich,"R"))
             {
                /*B Extraire les parametres RAU */
                vl_NbParam = sscanf ( pl_LigneFich, "R %hd %s %hd %hd %3c %s %d %s %d" 
                                            		, &vg_donsysrau.Config.Numero
                                            		, vg_donsysrau.Config.NumeroST
                                            		, &vg_donsysrau.Config.NoLSMaitre
                                            		, &vg_donsysrau.Config.NoLSEsclave
                                            		, vg_donsysrau.Config.AdresseRGS
													, vg_donsysrau.Config.AdresseIpEsclave
													, &vg_donsysrau.Config.PortIpEsclave
													, vg_donsysrau.Config.AdresseIpMaitre
													, &vg_donsysrau.Config.PortIpMaitre);
             }

             
	     /*B Si c'est une ligne de config pau */
             if (strstr(pl_LigneFich,"P"))
             {
                /*B Extraire les parametres pau */
                vl_NbParam = sscanf ( pl_LigneFich, 	"P %hd %d %d %d",  
                                              &vl_ListePAU.Config.Numero,
                                              &vl_ListePAU.Config.CodePAU,
                                              &vl_ListePAU.Config.CodeAntenne,
                                              &vl_ListePAU.Config.Type);
	     
                /* Si tout les parametre ont ete lu */
                if ( vl_NbParam == 4 )
                {
                    /*B Inserer dans la liste des structures config pau */
                    ed_ins_liste_PAU ( &vl_ListePAU, sizeof(vl_ListePAU.Config) ) ;
                }
             }
             
         }
      }
      /*A Fermeture du fichier XDF_Config_RAU_<NomMachine>  */
      XZSS_13FermerFichier( vl_Fd );
      
      /*A Suppression de la liste des donnees des elements ne faisant plus partie de la config pau */
      for ( pl_ListePAU = pg_debutPAU; pl_ListePAU != NULL; )
      {
         /* Si le numero de ST est effacer */
         if ( pl_ListePAU->Config.CodeAntenne == 0 )
         {
            /* Suppression de la liste */
            ed_sup_liste_PAU ( pl_ListePAU->Config.Numero );
            pl_ListePAU = pg_debutPAU;
         }
         else
         {
            /* Passage a l'element suivant */
            pl_ListePAU = pl_ListePAU->Suivant;
         }
      }
    
      /* Ecriture trace de la config pour debug */
      vg_donsysrau.Etat_Service = 0;
      vg_donsysrau.Config.NoLSBascul = vg_donsysrau.Config.NoLSMaitre - 1;
      XZST_03EcritureTrace ( XZSTC_WARNING, "Config RAU: %d %s %d %d %s %s %d %s %d %d",  
					vg_donsysrau.Config.Numero,
					vg_donsysrau.Config.NumeroST,
					vg_donsysrau.Config.NoLSMaitre,
					vg_donsysrau.Config.NoLSEsclave,
					vg_donsysrau.Config.AdresseRGS,
					vg_donsysrau.Config.AdresseIpEsclave,
					vg_donsysrau.Config.PortIpEsclave,
					vg_donsysrau.Config.AdresseIpMaitre,
					vg_donsysrau.Config.PortIpMaitre,
					vg_donsysrau.Etat_Service);				
                                            	  
      for ( pl_ListePAU = pg_debutPAU; pl_ListePAU != NULL; pl_ListePAU = pl_ListePAU->Suivant )
      {
          pl_ListePAU->Etat_Service  = 0;
          pl_ListePAU->Etat.NoDerCmd = 0;
          pl_ListePAU->Etat.CRCmd = XZEXC_PAS_DE_CMD;
          pl_ListePAU->Etat.Gardepau = XDC_FAUX;
 /*!!!         vg_donsysrau.Basculement[pl_ListePAU->Config.CodeAntenne] = XDC_GN_PREND_RAU;  */
          XZST_03EcritureTrace ( XZSTC_WARNING, "Config PAU: %d %d %d %d",  
					pl_ListePAU->Config.Numero,
					pl_ListePAU->Config.CodePAU,
					pl_ListePAU->Config.CodeAntenne,
					pl_ListePAU->Etat_Service);				
      }

      /* Lecture du fichier de config des timeout basculement */
      sprintf ( pl_PathEtFic, "%s/%s", XZEXC_PATH_CONFIG, XDF_CONF_SYST_RAU );
      if ( ( vl_Fic = fopen ( pl_PathEtFic, "r" ) ) == NULL )
      {
          XZST_03EcritureTrace( XZSTC_WARNING, "Impossible d'ouvrir le fichier %s", pl_PathEtFic ); 
          return ( vl_ValRet );
      }
   
      /*A Lecture du fichier integralement */
      while ( fgets ( pl_LigneFich, sizeof(pl_LigneFich), vl_Fic ) != NULL )
         if ( pl_LigneFich[0] != XDF_COMMENTAIRE ) 
         {
       	    vl_NbParam = sscanf ( pl_LigneFich,"%s\t%hd\t%hd\t%hd\n",
    					vl_NomSite,
    					&vl_TimeOut,
    					&vl_TimeOutRetour,
    					&vl_NbQuart );
            if ( (vl_NbParam == 4) && (!strcmp(vl_NomSite, vg_NomSite)) )
            {
               vg_donsysrau.Config.TimeOut = vl_TimeOut * 60;
               vg_donsysrau.Config.TimeOutRetour = vl_TimeOutRetour * 60;
               vg_donsysrau.Config.NbQuart = vl_NbQuart;
               XZST_03EcritureTrace ( XZSTC_WARNING, "Lecture config Basculement RAU OK : timeout=%d %d %d", 
    					vg_donsysrau.Config.TimeOut, vg_donsysrau.Config.TimeOutRetour, vg_donsysrau.Config.NbQuart );
               break;
            }
         }
      fclose ( vl_Fic );   
       
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
static char *version = "$Id: erau_cfg.c,v 1.14 2020/11/03 18:12:22 pc2dpdy Exp $      : ec_controle" ;

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
	static char *version = "$Id: erau_cfg.c,v 1.14 2020/11/03 18:12:22 pc2dpdy Exp $      : ec_init" ;

	XZEXT_MASK *		pl_Socket;
	XZEXT_ADR_SOCK		pl_TacheTrt;
	XZEXT_ADR_SOCK		pl_TacheProt;
	int					vl_SockProt = 0;
	char				vl_adr[30];

   	/* Lecture des fichiers de configuration et remplissage des structure de donnees */
   	/* Configuration du rau */
   	if ( ec_lire_config_RAU ( pa_NomMachine ) != XDC_OK) return (XDC_NOK);
	
   	/* Lecture de la config des PAU pour le traitement des alerte PAU HS */
   	ec_lire_fichier_info_PAU ();

	/* Ouverture du fichier d'alarme */
	ex_OuvertureFichierMsg(vg_NomMachine);

	/* Appel  xzat26_Init_Etats_Eqts pour Initialisation des etats equipement (HS ou Actif) */
	xzat26_Init_Etats_Eqts ( XDC_EQT_RAU, XDC_ETAT_TOUS_EQT, vg_NomMachine );
	xzat26_Init_Etats_Eqts ( XDC_EQT_PAU, XDC_ETAT_TOUS_EQT, vg_NomMachine );
	
	/* Connexion socket en mode client pour communiquer avec la tache TELEi */
	/* Pour le systeme video rechercher si la tache protocole associee existe */
	/* Construction du nom de la tache protocole telmi*/
	sprintf ( pl_TacheProt, "%s%d", XZEXC_ADR_SOCK_TELE, vg_donsysrau.Config.NoLSEsclave );
	sprintf ( pl_TacheTrt, "%s%d", XZEXC_ADR_SOCK_RAUTELE, vg_donsysrau.Config.NoLSEsclave );
	/*A Recherche sur tout le masque des socket si la socket exite */
	for ( pl_Socket = vg_SockMask; pl_Socket != NULL; pl_Socket = pl_Socket->Suivant )
	{
		if ( !strcmp ( pl_Socket->adresse, pl_TacheTrt ) )
		{
			/* Mise a jour du numero de socket */
			vg_donsysrau.SocketEsclave = pl_Socket->desc_sock;
			/*B Arret: la socket avec la tache existe */
			break;
		}
	}
	   
	/*A Si la socket avec la tache telmi n'existe pas */
	if ( pl_Socket == NULL )
	{
		if (!strcmp(vg_donsysrau.Config.AdresseIpEsclave, XZECC_IP_VIDE))
		{	
			/* RAU non IP */
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
				vg_donsysrau.SocketEsclave = vl_SockProt;
			}
		}
		else
		{
			/* RAU IP */
            sprintf(vl_adr, "%s%d", XZEXC_ADR_SOCK_TEIM, XDC_EQT_RAU);
			/*A Appel de ex_cnx_client pour etablir la liaison avec la tache protocole */
			if ( ex_cnx_client ( 	XZEXC_ADR_SOCK_TERAU,
									vl_adr,
									&vg_SockMask,
									&vl_SockProt ) != XDC_OK )
			{
				/*B Ecriture Trace */
				XZST_03EcritureTrace( XZSTC_FATAL, "Impossible de creer la connexion client avec %s .", vl_adr );	
			}
			else
			{
				/* Memorisation du numero de socket */
				vg_donsysrau.SocketEsclave = vl_SockProt;
			}
		}
	}
	
	/*A
	 * Connexion socket en mode client pour communiquer avec la tache TELMi
	 */
	/*A Pour le systeme video rechercher si la tache protocole associee existe */
	
	/* Construction du nom de la tache protocole telmi*/
	sprintf ( pl_TacheProt, "%s%d", XZEXC_ADR_SOCK_TELM, vg_donsysrau.Config.NoLSMaitre );
	sprintf ( pl_TacheTrt, "%s%d", XZEXC_ADR_SOCK_RAUTELM, vg_donsysrau.Config.NoLSMaitre );
	/*A Recherche sur tout le masque des socket si la socket exite */
	for ( pl_Socket = vg_SockMask; pl_Socket != NULL; pl_Socket = pl_Socket->Suivant )
	{
		if ( !strcmp ( pl_Socket->adresse, pl_TacheTrt ) )
		{
			/* Mise a jour du numero de socket */
			vg_donsysrau.SocketMaitre = pl_Socket->desc_sock;
			/*B Arret: la socket avec la tache existe */
			break;
		}
	}
	   
	/*A Si la socket avec la tache telmi n'existe pas */
	if ( pl_Socket == NULL )
	{
		if (!strcmp(vg_donsysrau.Config.AdresseIpMaitre, XZECC_IP_VIDE))
		{	
			/* RAU non IP */
		
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
				vg_donsysrau.SocketMaitre = vl_SockProt;
			}
		}
		else
		{
			/* RAU IP */
            sprintf(vl_adr, "%s%d", XZEXC_ADR_SOCK_TEIM, XDC_EQT_RAU);
			/*A Appel de ex_cnx_client pour etablir la liaison avec la tache protocole */
			if ( ex_cnx_client ( 	XZEXC_ADR_SOCK_TERAU,
									vl_adr,
									&vg_SockMask,
									&vl_SockProt ) != XDC_OK )
			{
				/*B Ecriture Trace */
				XZST_03EcritureTrace( XZSTC_FATAL, "Impossible de creer la connexion client avec %s .", vl_adr );	
			}
			else
			{
				/* Memorisation du numero de socket */
				vg_donsysrau.SocketMaitre = vl_SockProt;
			}
		}
	}
   /*A
    * Connexion socket en mode client pour communiquer avec la tache TELMi pour le basculement
    */
   /*A Pour le systeme video rechercher si la tache protocole associee existe */
   /* ***************************************************** */
   /* La ligne bascul n'existe plus depuis le bascult du PIRAU au CI DEM716 */

 /*if ( vg_donsysrau.PC2 == XDC_VRAI )
 {
    Construction du nom de la tache protocole telmi
   sprintf ( pl_TacheProt, "%s%d", XZEXC_ADR_SOCK_TELM, vg_donsysrau.Config.NoLSBascul );
   sprintf ( pl_TacheTrt, "%s%d", XZEXC_ADR_SOCK_RAUTELM, vg_donsysrau.Config.NoLSBascul );
   Recherche sur tout le masque des socket si la socket exite 
   for ( pl_Socket = vg_SockMask; pl_Socket != NULL; pl_Socket = pl_Socket->Suivant )
   {
      if ( !strcmp ( pl_Socket->adresse, pl_TacheTrt ) )
      {
          Mise a jour du numero de socket 
         vg_donsysrau.SocketMaitre = pl_Socket->desc_sock;
          Arret: la socket avec la tache existe 
         break;
      }
   }

      
    Si la socket avec la tache telmi n'existe pas 
   if ( pl_Socket == NULL )
   {
       Appel de ex_cnx_client pour etablir la liaison avec la tache protocole 
      if ( ex_cnx_client ( 	pl_TacheTrt,
				pl_TacheProt,
				&vg_SockMask,
				&vl_SockProt ) != XDC_OK )
      {
	 / Ecriture Trace 
         XZST_03EcritureTrace( XZSTC_FATAL, "Impossible de creer la connexion client avec %s .", pl_TacheProt );	
      }
      else
      {
	  Memorisation du numero de socket 
	 vg_donsysrau.SocketBascul = vl_SockProt;
      }	
   }
 }
*/  
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
static char *version = "$Id: erau_cfg.c,v 1.14 2020/11/03 18:12:22 pc2dpdy Exp $      : ec_sortir" ;

   ERAU_DONNEES_PAU		*pl_PAU, 	*pl_PAUALiberer;
   XZEXT_MASK *			pl_Socket;
   XZEXT_ADR_SOCK		pl_TacheProt,
   				pl_TacheTrt;
   XZEXT_MSG_SOCKET 		pl_message;
   XDY_Datagroup		pl_NomDG;
   int				vl_resultat;

  /*A
   * Liberation des ressources memoires
   */

   /*A Liberation des ressources memoires des donnees PAU */
   for ( pl_PAU = pg_debutPAU; pl_PAU != NULL; free(pl_PAUALiberer) )
   {
      pl_PAUALiberer = pl_PAU;
      pl_PAU         = pl_PAU->Suivant;
   }  

   /*A
    * Envoie du message d'arret aux taches protocoles 
    */
    
   for ( pl_Socket = vg_SockMask; pl_Socket != NULL; pl_Socket = pl_Socket->Suivant )
   {
      if ( !strncmp ( pl_Socket->adresse, XZEXC_ADR_SOCK_TELM, strlen(XZEXC_ADR_SOCK_TELM) ) ||
           !strncmp ( pl_Socket->adresse, XZEXC_ADR_SOCK_TELE, strlen(XZEXC_ADR_SOCK_TELE) ) )
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
   /* Fin de la tache */
   exit (0);
}



void erau_basc_rau_cb (	T_IPC_CONN va_Cnx,
						T_IPC_CONN_PROCESS_CB_DATA pa_Data,
						T_CB_ARG pa_ARG)
{
#ifndef _TIBCO_EMS 
	T_STR vl_zone;
#else
	XDY_NomZone vl_zone;
#endif

#ifndef _TIBCO_EMS 
	T_STR vl_etat;
#else
	XDY_Etat_Module vl_etat;
#endif

	T_Liste_Zones *pl_Zones;
	int vl_trouve;
	char vl_trame[20];

	/* recuperation du nouvel etat de basculement demande*/
	if (!TipcMsgRead(	pa_Data -> msg,
						T_IPC_FT_STR,&vl_zone,
						T_IPC_FT_STR,&vl_etat,
						NULL)) 
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"erau_basc_rau : message BASC_RAU non lu");
		return;
	}

	XZST_03EcritureTrace(XZSTC_INFO,"erau_basc_rau : zone '%s'",vl_zone);
	vl_trouve=0;

	/* recherche de la zone */
	for (pl_Zones=pg_Zones; pl_Zones->suivant!=NULL; pl_Zones = pl_Zones->suivant) 
	{
		XZST_03EcritureTrace(XZSTC_INFO,"zone %s",pl_Zones->zone.zone);
		if (!strcmp(pl_Zones->zone.zone,vl_zone)) 
		{
			XZST_03EcritureTrace(XZSTC_INFO,"zone trouvee");
			if (!strcmp(pl_Zones->zone.etat,"LOC"))
				strcpy(vl_etat,"CI");
			else
				strcpy(vl_etat,"LOC");
			vl_trouve=1;
			break;
		}
	}

	if (pl_Zones!=NULL) 
	{
		if (!strcmp(pl_Zones->zone.zone,vl_zone)) 
		{
			XZST_03EcritureTrace(XZSTC_INFO,"zone trouvee");
			if (!strcmp(pl_Zones->zone.etat,"LOC"))
				strcpy(vl_etat,"CI");
			else
				strcpy(vl_etat,"LOC");
			vl_trouve=1;
		}
	}

	/*envoi au PIRAU des etats de basculement pour ls zones de mon site local*/
	if ((vl_trouve==1) && ((!(vg_donsysrau.Etat_Service & XDC_EQT_HORS_SRV))))
	{
		pl_Zones->zone.compteur++;
		XZST_03EcritureTrace(XZSTC_INFO,"erau_basc_rau: envoi au PIRAU");
		sprintf( vl_trame, "ACT XT MODE=%s ZONE=%s", 
					vl_etat, vl_zone);

		if (!strcmp(vg_donsysrau.Config.AdresseIpMaitre, XZECC_IP_VIDE))
			EnvoyerTrame(	vl_trame,
							XZEXC_TRAME_RAU_TRONCON,
							vg_donsysrau.Config.AdresseRGS,
							XZEXC_NOCMD,
							XZEXC_PRIORITEFAIBLE,
							vg_donsysrau.SocketMaitre);
		else
			EnvoyerTrameIP(	vl_trame,
							XZEXC_TRAME_RAU_TRONCON,
							vg_donsysrau.Config.AdresseIpMaitre,
							vg_donsysrau.Config.PortIpMaitre,
							vg_donsysrau.Config.AdresseRGS,
							XZEXC_NOCMD,
							XZEXC_PRIORITEFAIBLE,
							vg_donsysrau.SocketMaitre);
	}
}



