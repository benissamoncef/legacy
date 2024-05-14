/*E*/
/*Fichier :  $Id: enav_cfg.c,v 1.22 2012/07/18 08:19:04 pc2dpdy Exp $      Release : $Revision: 1.22 $        Date : $Date: 2012/07/18 08:19:04 $ ------------------------------------------------------
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TENAV * FICHIER enav_cfg.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module permetant d'initialiser les structures en memoire
*
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E	version 1.1	18 Nov 1994	: Creation
* Nagiel.E	version 1.2	29 Nov 1994	:
* Mismer.D	version 1.3	30 Nov 1994	: Modif sortie sur deconnexion de TESRV
* Nagiel.E	version 1.4	21 Dec 1994	:
* Nagiel.E	version 1.5	16 Jan 1995	: RAS
* Mismer.D	version 1.6	23 Jan 1995	:
* Mismer.D	version 1.7	26 Jan 1995	:
* Mismer.D	version 1.8	21 Fev 1995	:
* Mismer.D	version 1.9	08 Mar 1995	:
* Mismer.D	version 1.10	17 Mar 1995	: 
* Fontaine.C	version 1.11	18 Mai 1995	: Reprise historique
* Mismer.D	version 1.12	24 Mai 1995	: Init memoristaion colision biseaux
* Mismer.D	version 1.13	05 Fev 1996	: Init memoristaion etat cmd module
* Mismer.D	version 1.14	25 Mai 1996	: Remplacement lecture passwrd par ex_lire_Idpassw (DEM/1135)
* Mismer.D	version 1.15	25 Mai 1996	: Modif parametre status CLE CLL CLI (DEM/1136)
* Mismer.D	version 1.16	11 Sep 1996	: Ajout periode variable en fonction NB eqt (RADT_W)
* Mismer.D	version 1.17	10 Jan 1997	: Modif pour matrice BRA (DEM/1419)
* Mismer.D	version 1.18	15 Jan 1998	: Modif FAC et FAU  (DEM/1544)
* CLuchague     version 1.19	15 Nov 2004	: Pour BRA NG vl_tableref ajout numeros de modules 
* JMG   13/11/07        : ajout site de gestion DEM715
* PNI	03/02/09	: ajout champ EDF DEL788 v1.21
* PNI   17/07/12        : nb eqt ligne passe � 21 (au lieu de 16) V1.22
*
------------------------------------------------------*/

/* fichiers inclus */
#include <xzsem.h>

#include "enav_cfg.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

/* declaration de fonctions internes */
int ec_lire_config_Module	( ENAV_LISTE_MODULE * , XDY_TypeEqt );

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction effectue la lecture du fichier XDF_Config_NAV_<NomMachine>
*  et met a jour le liste des structure de donnees NAV;
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ec_lire_config_NAV	( XDY_NomMachine		pa_NomMachine )
			  
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
   static char *version = "@(#)enav_cfg.c	1.19 01/10/05      : ec_lire_config_NAV" ;

   int				vl_ValRet = XDC_OK;
   int				vl_Fd,i,vl_NumEqt = 0;
   char 			pl_PathEtFic[XDC_PATH_ABS_SIZE];
   XDY_Texte			pl_LigneFich;
   int				vl_NbPICTO = 0;
   ENAV_DONNEES_NAV		vl_DonneesNAV = {0},
   				*pl_DonCrt;
   short			vl_NumeroLS = 0;
   int				vl_NbParam  = 0;
   XDY_NomST			vl_NomST;
   int				vl_site;

    static char			vl_tabref[XZEXC_nb_module_max+1][4] = 
   {
      "1.1","1.2","1.3","1.4","2.1","2.2","2.3","2.4","3.1","3.2","4.1","5.1","5.5",
            "6.1","6.5","7.1","7.5","8.1","8.5","9.1","9.5","3.3","3.4","4.2","4.3",
            "1.5","1.6","2.6","2.7","3.5","3.6","3.7","3.8","4.6","4.7",".9"
   };
/*   static char			vl_tabref[XZEXC_nb_module_max+1][4] = 
   {
      "1.1","1.2","1.3","1.4","2.1","2.2","2.3","2.4","3.1","3.2","4.1","5.1","5.5",
            "6.1","6.5","7.1","7.5","8.1","8.5","9.1","9.5","3.3",".9"
   };*/
   char				vl_mod[4];
  
      /*A Ouverture du fichier de configuration des PICTO */
      /* Construction du nom du fichier XDF_Config_NAV_<NomMachine>  */
      sprintf ( pl_PathEtFic, "%s/%s_%s", XZEXC_PATH_CONFIG, XDF_Config_NAV, pa_NomMachine );
      
      /* Ouverture du fichier XDF_Config_NAV_<NomMachine>  */
      if ( (vl_Fd = open (pl_PathEtFic, O_RDONLY)) <= 0 )
      {
         /* Ecriture trace absence fichier */
         XZST_03EcritureTrace( XZSTC_WARNING, " Manque fichier %s. \n", pl_PathEtFic ); 
         /* Sortie de la fonction */
         return ( XDC_NOK );
      }
      /*A Marquage des structures afin de determiner les eventuelles suppression */
      for ( pl_DonCrt = pg_debutNAV; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant )
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
	    /*B Extraire la config NAV */
	    if ( *pl_LigneFich == 'N' )
	    {	       
               
               /*B Inserer dans la liste des structures config NAV */
               ed_ins_liste_NAV ( &vl_DonneesNAV, sizeof(vl_DonneesNAV.Config) );

               /* Mise en memoire NAV */
               for(i=0;i!=XZEXC_nb_module_max;i++)
               {  
		  vl_DonneesNAV.Config.Liste_Module[i].NoModule = 0;
                  vl_DonneesNAV.Config.Liste_Module[i].Etat_Service  = 0;
               }

               /*B Extraire les parametre du NAV */

               vl_NbParam = sscanf ( pl_LigneFich, 	"N %hd %s %hd %3c %hd %hd %d",  
					&vl_DonneesNAV.Config.Numero,
					vl_DonneesNAV.Config.NumeroST,
					&vl_NumeroLS,
					vl_DonneesNAV.Config.AdresseRGS,
					&vl_DonneesNAV.Config.TypeNAV,
					&vl_DonneesNAV.Config.Type,
					&vl_site);
               vl_DonneesNAV.Config.NumeroLS = vl_NumeroLS;
	       vl_DonneesNAV.Config.SiteGestion=vl_site;

               /* Lecture des nom de module des fichiers SNAV */
               if ( ec_lire_config_Module ( vl_DonneesNAV.Config.Liste_Module, vl_DonneesNAV.Config.TypeNAV ) != XDC_OK )
               {
                  for(i=0;i!=XZEXC_nb_module_max;i++)
                  {  
		     strcpy(vl_DonneesNAV.Config.Liste_Module[i].Module,vl_tabref[i]);
                  }
               }
               
            }
            
	    /*B Extraire la config module */
	    if ( *pl_LigneFich == 'M')
	    {
               /*B Extraire les parametre des modules de la NAV */
               vl_NbParam = sscanf ( pl_LigneFich, 	"M %d %s",&vl_NumEqt,vl_mod );               
               for(i=0;i!=XZEXC_nb_module_max;i++)
               {  
                  if ( vl_DonneesNAV.Config.Liste_Module[i].Module[0] == vl_mod[0] )
                  {
	             vl_DonneesNAV.Config.Liste_Module[i].NoModule = vl_NumEqt;
                  }
               }
            }
         }
      }
         
      /*B Inserer dans la liste des structures config NAV */
      ed_ins_liste_NAV ( &vl_DonneesNAV, sizeof(vl_DonneesNAV.Config) );
      
      /*A Fermeture du fichier XDF_Config_NAV_<NomMachine>  */
      XZSS_13FermerFichier( vl_Fd );
      
      /*A Suppression de la liste des donnees des elements ne faisant plus partie de la config */
      for ( pl_DonCrt = pg_debutNAV; pl_DonCrt != NULL; )
      {
         /* Si le numero de ST est effacer */
         if ( strlen ( pl_DonCrt->Config.NumeroST ) <= 1 )
         {
            /* Suppression de la liste */
            ed_sup_liste_NAV ( pl_DonCrt->Config.Numero );
            pl_DonCrt = pg_debutNAV;
         }
         else
         {
            /* Passage a l'element suivant */
            pl_DonCrt = pl_DonCrt->Suivant;
         }
      }
     
      /* Ecriture trace de la config pour debug et init de l'etat de service et de l'activite */
      for ( pl_DonCrt = pg_debutNAV; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant )
      {
          pl_DonCrt->Etat_Service  = 0;
          pl_DonCrt->Colision  = XDC_FAUX;
          pl_DonCrt->EDF  = XDC_FAUX;
          pl_DonCrt->Contraste = XDC_CONTRASTENORMAL;
          pl_DonCrt->ContrasteCmd = XZEXC_NOCMD;
          for ( i = 0 ;i < 4; i++ ) pl_DonCrt->Mode[i] = XZEXC_MODE_NORMAL;
          strcpy(pl_DonCrt->Etat.Scenario,"");
          XZST_03EcritureTrace ( XZSTC_DEBUG3, "Config NAV: %d %s %d %s %d %d %d %d",  
					pl_DonCrt->Config.Numero,
					pl_DonCrt->Config.NumeroST,
					pl_DonCrt->Config.NumeroLS,
					pl_DonCrt->Config.AdresseRGS,
					pl_DonCrt->Config.TypeNAV,
					pl_DonCrt->Config.Type,
					pl_DonCrt->Etat_Service,
					pl_DonCrt->Config.SiteGestion);
					
         for(i=0;i!=XZEXC_nb_module_max;i++)
         {  
            strcpy(pl_DonCrt->Etat.Module[i],"0");
            pl_DonCrt->EtatCmd.Module[i][0] = '\0';
            *pl_DonCrt->Etat.Scenario = '\0';
            if (pl_DonCrt->Config.Liste_Module[i].NoModule!=0)
            {
               XZST_03EcritureTrace ( XZSTC_DEBUG3, "Config listemodules: %d %s ",  
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
*  Cette fonction effectue la lecture du fichier XDF_Config_Module_NAV_<NomMachine>
*  et met a jour le liste des structure de donnees type NAV;
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ec_lire_config_Module	( ENAV_LISTE_MODULE * va_Tab_Module, XDY_TypeEqt va_NumType )
			  
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
   static char *version = "@(#)enav_cfg.c	1.19 01/10/05      : ec_lire_config_Module" ;

   int				vl_ValRet = XDC_OK;
   int				vl_Fd;
   char 			pl_PathEtFic[XDC_PATH_ABS_SIZE];
   XDY_Texte			pl_LigneFich;
   int				vl_NbParam  = 0;
   XDY_Nom			vl_NomModule = "";
   XDY_Entier			vl_Numero = 0;
  
      /*A Ouverture du fichier de configuration des PICTO */
      /* Construction du nom du fichier XDF_Config_Module_NAV_<Numero de type>  */
      sprintf ( pl_PathEtFic, "%s/%s_%d.cfg", ENAVC_PATH_CONFIG_NAV, XDF_Config_NAV, va_NumType );
      
      /* Ouverture du fichier XDF_Config_Module_NAV_<NomMachine>  */
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
            /*B Extraire les parametre du type NAV */

            vl_NbParam = sscanf ( 	pl_LigneFich, "%d,%[^,],",  
					&vl_Numero,
					vl_NomModule );
	    if ( vl_NbParam == 2 )
	    {
	        if ( (vl_Numero >= 0) && (vl_Numero < XZEXC_nb_module_max) )
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
         
      /*A Fermeture du fichier XDF_Config_Module_NAV_<NomMachine>  */
      XZSS_13FermerFichier( vl_Fd );
      
      /* Retourne compte rendu OK */
      return ( vl_ValRet );
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction effectue la lecture du fichier XDF_Config_Type_NAV_<NomMachine>
*  et met a jour le liste des structure de donnees type NAV;
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ec_lire_config_TypeNAV	( XDY_NomMachine		pa_NomMachine )
			  
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
   static char *version = "@(#)enav_cfg.c	1.19 01/10/05      : ec_lire_config_TypeNAV" ;

   int				vl_ValRet = XDC_OK;
   int				vl_Fd,i,vl_premier,vl_first;
   char 			pl_PathEtFic[XDC_PATH_ABS_SIZE];
   XDY_Texte			pl_LigneFich;
   int				vl_NbPICTO = 0;
   ENAV_DONNEES_TYPE_NAV	vl_DonneesTypeNAV = {0},
   				*pl_DonCrt;
   ENAV_LISTE_SCEN		*pl_DonneesScen,*pl_DonneesScen2;
   short			vl_NumeroLS = 0;
   int				vl_NbParam  = 0;
   XDY_NomST			vl_NomST;
  
      /*A Ouverture du fichier de configuration des PICTO */
      /* Construction du nom du fichier XDF_Config_Type_NAV_<NomMachine>  */
      sprintf ( pl_PathEtFic, "%s/%s_%s", XZEXC_PATH_CONFIG, XDF_Config_Type_NAV, pa_NomMachine );
      
      /* Ouverture du fichier XDF_Config_NAV_<NomMachine>  */
      if ( (vl_Fd = open (pl_PathEtFic, O_RDONLY)) <= 0 )
      {
         /* Ecriture trace absence fichier */
         XZST_03EcritureTrace( XZSTC_WARNING, " Manque fichier %s. \n", pl_PathEtFic ); 
         /* Sortie de la fonction */
         return ( XDC_NOK );
      }
      /*A Marquage des structures afin de determiner les eventuelles suppression */
      for ( pl_DonCrt = pg_debutTypeNAV; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant )
      {
      	 /* Effacement du nb de barierres pour marquage */
         pl_DonCrt->NbBarrieres = -1;
      }
 
      vl_premier = XDC_VRAI;
      
      /*A Lecture de chaque ligne du fichier */
      while ( ex_LireLigneFich ( vl_Fd, pl_LigneFich, sizeof ( pl_LigneFich ) ) != XZEXC_EOF ) 
      {
         /* Lire ligne suivante si c'est une ligne de commentaire */
         if ( *pl_LigneFich != XDF_COMMENTAIRE )
         {
	    /*B Extraire la config NAV */
	    if (!strncmp(pl_LigneFich,"T",1))
	    {	       
               /* Mise en memoire NAV precedente */
               if ( vl_premier == XDC_FAUX )
               {
                  /*B Inserer dans la liste des structures config type NAV */
                  ed_ins_liste_type_NAV ( &vl_DonneesTypeNAV, 0 );
               }
               else
               {
      		  vl_premier = XDC_FAUX;
               }
               
               vl_first=XDC_VRAI;
               
               /*B Extraire les parametre du type NAV */

               vl_NbParam = sscanf ( pl_LigneFich, 	"T %hd %hd %hd",  
					&vl_DonneesTypeNAV.Numero,
					&vl_DonneesTypeNAV.Flash,
					&vl_DonneesTypeNAV.NbBarrieres );
					
               vl_DonneesTypeNAV.Liste=NULL;
            }
	    /*B Extraire la config Scenario */
	    if (!strncmp(pl_LigneFich,"S",1))
	    {
               pl_DonneesScen2=(ENAV_LISTE_SCEN *) malloc (sizeof(ENAV_LISTE_SCEN));
               pl_DonneesScen2->Suivant=NULL;
               
               if (vl_first==XDC_VRAI)
               {
                  vl_DonneesTypeNAV.Liste = pl_DonneesScen2;
                  vl_first=XDC_FAUX;
               }
               else
               {
                  pl_DonneesScen->Suivant= pl_DonneesScen2;
               }
               
               /*B Extraire les parametre des modules de la NAV */

               vl_NbParam = sscanf ( pl_LigneFich, 	"S %hd %hd %s",  
					&pl_DonneesScen2->NoSCEN,
					&pl_DonneesScen2->AutoFic,
					pl_DonneesScen2->Nom_SCEN);
					
               pl_DonneesScen = pl_DonneesScen2;
            }
         }
      }
      /*B Inserer dans la liste des structures config NAV */
      ed_ins_liste_type_NAV ( &vl_DonneesTypeNAV, 0 );
         
      /*A Fermeture du fichier XDF_Config_NAV_<NomMachine>  */
      XZSS_13FermerFichier( vl_Fd );
      
      /*A Suppression de la liste des donnees des elements ne faisant plus partie de la config */
      for ( pl_DonCrt = pg_debutTypeNAV; pl_DonCrt != NULL; )
      {
         /* Si le numero de ST est effacer */
         if (pl_DonCrt->NbBarrieres == -1 )
         {
            /* Suppression de la liste */
            ed_sup_liste_type_NAV ( pl_DonCrt->Numero );
            pl_DonCrt = pg_debutTypeNAV;
         }
         else
         {
            /* Passage a l'element suivant */
            pl_DonCrt = pl_DonCrt->Suivant;
         }
      }
     
      /* Ecriture trace de la config pour debug et init de l'etat de service et de l'activite */
      for ( pl_DonCrt = pg_debutTypeNAV; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant )
      {
          XZST_03EcritureTrace ( XZSTC_DEBUG3, "Config Type NAV: %d %d %d",  
					pl_DonCrt->Numero,
					pl_DonCrt->Flash,
					pl_DonCrt->NbBarrieres);
	
        
        for(pl_DonneesScen=pl_DonCrt->Liste;pl_DonneesScen!=NULL;pl_DonneesScen = pl_DonneesScen->Suivant)
         {  
            XZST_03EcritureTrace ( XZSTC_DEBUG3, "Config liste senarios: %d %d %s ",  
					pl_DonneesScen->NoSCEN,
					pl_DonneesScen->AutoFic,
					pl_DonneesScen->Nom_SCEN);
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
   static char *version = "@(#)enav_cfg.c	1.19 01/10/05      : ec_controle" ;

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
* 
* Lit le fichier de correspondance scenario
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ec_lire_config_Scenar ( )
			  
/*
* ARGUMENTS EN ENTREE :
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
* Ouvre le fichier ConfScenar et le charge en memoire  
*
------------------------------------------------------*/
{
   static char *version = "@(#)enav_cfg.c	1.19 01/10/05      : ec_lire_config_Scenar" ;
   
   int				vl_ValRet = XDC_OK;
   int				vl_Fd,i,vl_poub;
   char 			pl_PathEtFic[XDC_PATH_ABS_SIZE];
   XDY_Texte			pl_LigneFich;
   int				vl_NbParam  = 0;
   int				vl_index;
  
      /*A Ouverture du fichier de Scenario */
      /* Construction du nom du fichier XDF_Config_Scenar  */
      sprintf ( pl_PathEtFic, "%s/%s", XZEXC_PATH_CONFIG, XDF_Config_Scenar );
      
      /* Ouverture du fichier XDF_Config_Scenar */
      if ( (vl_Fd = open (pl_PathEtFic, O_RDONLY)) <= 0 )
      {
         /* Ecriture trace absence fichier */
         XZST_03EcritureTrace( XZSTC_WARNING, " Manque fichier %s. \n", pl_PathEtFic ); 
         /* Sortie de la fonction */
         return ( XDC_NOK );
      }
      
      vl_index = 0;
      /*A Lecture de chaque ligne du fichier */
      while ( ex_LireLigneFich ( vl_Fd, pl_LigneFich, sizeof ( pl_LigneFich ) ) != XZEXC_EOF ) 
      {
         /* Lire ligne suivante si c'est une ligne de commentaire */
         if ( *pl_LigneFich != XDF_COMMENTAIRE )
         {
	    if ((!strncmp(pl_LigneFich,"S",1)) && (vl_index != XZEXC_NB_SCENAR_PREDEF))
	    {	       
               /*B Extraire les Scenarios */

               vl_NbParam = sscanf ( pl_LigneFich, 	"S %s %s",  
					 vg_CompScenar[vl_index].PseudoScenar,
					 vg_CompScenar[vl_index].NomScenar);
	       if (vl_NbParam == 2)
	       {
	          vl_index++;
	       }
            }
         }
      }
         
      /*A Fermeture du fichier XDF_Config_Scenar */
      XZSS_13FermerFichier( vl_Fd );
      
      /* Ecriture trace de la config pour debug et init de l'etat de service et de l'activite */
      for ( i = 0; i != vl_index; i++ )
      {
          XZST_03EcritureTrace ( XZSTC_INFO, "Config Scenar Predef: %s %s",  
					 vg_CompScenar[i].PseudoScenar,
					 vg_CompScenar[i].NomScenar);
      }			
      /* Retourne compte rendu OK */
      return ( XDC_OK );
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
*   Lecture des fichier de configuration des NAV et des Picto
*   Remplissage des structures de donnees
*   Connexion socket avec la tache protocole
*
------------------------------------------------------*/
{
   static char *version = "@(#)enav_cfg.c	1.19 01/10/05      : ec_init" ;

   ENAV_DONNEES_NAV 		*pl_DonCrt;
   XZEXT_MASK *			pl_Socket;
   XZEXT_ADR_SOCK		pl_TacheTrt;
   XZEXT_ADR_SOCK		pl_TacheProt;
   int				vl_SockProt = 0;
   int				vl_Index = 0;
   short			vl_NbEqtLigne[21] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

   /*A
    * Lecture des fichiers de configuration et remplissage des structure de donnees 
    */
   /*A Configuration des NAV (appel de la fonction ec_lire_config_NAV)*/
   if ( ec_lire_config_NAV ( pa_NomMachine ) != XDC_OK) return (XDC_NOK);
   /*A Configuration des Type de NAV (appel de la fonction ec_lire_config_TypeNAV) */
   if ( ec_lire_config_TypeNAV ( pa_NomMachine ) != XDC_OK) return (XDC_NOK);
   /*A Configuration des scenarios  */
   if ( ec_lire_config_Scenar () != XDC_OK) return (XDC_NOK);
   /*A Configuration des clignotements */
    ex_lire_cligno ( pa_NomMachine, vg_clignolent, vg_clignorapide );
   /*A Lecture user/password */
    ex_lire_idpassw ( pa_NomMachine, vg_idpassw );
   
   /*A Ouverture du fichier d'alarme */
   ex_OuvertureFichierMsg(vg_NomMachine);
   
   /*A Appel  xzat26_Init_Etats_Eqts pour Initialisation des etats equipement (HS ou Actif) */
   xzat26_Init_Etats_Eqts ( XDC_EQT_NAV, XDC_ETAT_TOUS_EQT, pa_NomMachine );
   xzat26_Init_Etats_Eqts ( XDC_EQT_MOD, XDC_ETAT_TOUS_EQT, pa_NomMachine );

   /*A
    * Connexion socket en mode client pour communiquer avec la tache TELMi
    */
   /*A Pour tout les NAV rechercher si la tache protocole associee existe */
   for ( pl_DonCrt = pg_debutNAV; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant )
   {
      /* Construction du nom de la tache protocole telmi*/
      sprintf ( pl_TacheProt, "%s%d", XZEXC_ADR_SOCK_TELM, pl_DonCrt->Config.NumeroLS );
      sprintf ( pl_TacheTrt, "%s%d", XZEXC_ADR_SOCK_NAVTELM, pl_DonCrt->Config.NumeroLS );
      vl_NbEqtLigne[pl_DonCrt->Config.NumeroLS]++;
      /*A Recherche sur tout le masque des socket si la socket exite */
      for ( pl_Socket = vg_SockMask; pl_Socket != NULL; pl_Socket = pl_Socket->Suivant )
      {
         if ( !strcmp ( pl_Socket->adresse, pl_TacheProt ) )
         {
            /* Mise a jour du numero de socket */
            pl_DonCrt->Socket = pl_Socket->desc_sock;
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
	    pl_DonCrt->Socket = vl_SockProt;
	 }	
      }
   }
   /*A Initialisation memoire */
   /*A Pour tous les pmv et les picto -> demande d'etat d'affichage */
   for ( pl_DonCrt = pg_debutNAV; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant )
   {
       EnvoyerTrame(     "PS",
                         XZEXC_TRAME_ETAT_POS_NAV,
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
   static char *version = "@(#)enav_cfg.c	1.19 01/10/05      : ec_sortir" ;

   ENAV_DONNEES_NAV 		*pl_NAV, 	*pl_NAVALiberer;
   ENAV_DONNEES_TYPE_NAV	*pl_TypeNAV,	*pl_TypeNAVALiberer;
   ENAV_LISTE_SCEN		*pl_ScenAuto,	*pl_ScenAutoALiberer;
   XZEXT_MASK *			pl_Socket;
   XZEXT_ADR_SOCK		pl_TacheProt,
   				pl_TacheTrt;
   XZEXT_MSG_SOCKET 		pl_message;
   XDY_Datagroup		pl_NomDG;
   int				vl_resultat;

   /*A Desabonnement a JourNuit */
   XDG_EncodeDG2 ( pl_NomDG, "Einit_JourNuit_NAV", vg_NomMachine) ;			
   /*XZEA29_Abt_Etat_JourNuit(XZEAC_FERMER_ABONNEMENT,pl_NomDG,ep_journuit,vg_NomSite,&vl_resultat);*/

  /*A
   * Liberation des ressources memoires
   */

   /*A Liberation des ressources memoires des donnees des NAV */
   for ( pl_NAV = pg_debutNAV; pl_NAV != NULL; free(pl_NAVALiberer) )
   {
      pl_NAVALiberer = pl_NAV;
      pl_NAV         = pl_NAV->Suivant;
   }  
   /*A Liberation des ressources memoires des donnees Type de NAV */
   for ( pl_TypeNAV = pg_debutTypeNAV; pl_TypeNAV != NULL; free(pl_TypeNAVALiberer) )
   {
      pl_TypeNAVALiberer = pl_TypeNAV;
      
      /*A Liberation des ressources memoires des donnees NAV autorises */
      for ( pl_ScenAuto = pl_TypeNAV->Liste ; pl_ScenAuto != NULL; free(pl_ScenAutoALiberer) )
      {
         pl_ScenAutoALiberer = pl_ScenAuto;
         pl_ScenAuto         = pl_ScenAuto->Suivant;
      }
      
      pl_TypeNAV         = pl_TypeNAV->Suivant;
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
