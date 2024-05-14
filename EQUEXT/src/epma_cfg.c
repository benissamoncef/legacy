/* Fichier : epma_cfg.c
------------------------------------------------------
* GTIE * PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEPMA * FICHIER epma_cfg.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module de configuration de la tache TEPMA
*
------------------------------------------------------
* HISTORIQUE :
*
* jmg	10/06/05	creation
* JMG	17/10/05 : correction decodage Config_PMVA_NomMachine 1.2
* JMG   13/11/07 : ajout site de gestion DEM715  1.3
* JPL	18/03/11 : Migration architecture HP ia64 (DEM 975) : suppression warnings mineurs  1.4
* PNI   17/07/12        : nb eqt ligne passe � 21 (au lieu de 16) V1.5
* NDE	03/08/2015	: type d'affichage par defaut 3 POINTS DEM1135 1.6
* JPL	14/02/18 : Migration Linux 64 bits (DEM 1274) : Init. donnees PMVA; lecture affichage defaut sans debordement  1.7
* JMG	03/10/18 : ajout IP suppression SAGA DEM1306 1.8
* JPL	10/02/23 : Forcage du mode Normal du contraste pour tous les PMVA a l'initialisation (SAE-429)
------------------------------------------------------*/

/* fichiers inclus */
#include <xzems.h>

#include "xzat.h"

#include "ex_mrtf.h"

#include "epma_cfg.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

/* declaration de fonctions internes */

/* definition de fonctions externes */
extern int   vg_SockTEMIP;



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
* init ou reinit de la config pmv 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ec_lire_config_PMVA	( XDY_NomMachine		pa_NomMachine )
			  
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
*  Cette fonction effectue la lecture du fichier XDF_Config_PMV_<NomMachine>
*  et met a jour le liste des structure de donnees PMV;
*   
------------------------------------------------------*/
{
   static char *version = "@(#)epma_cfg.c	1.20 03/10/98 : ec_lire_config_PMV" ;

   int				vl_ValRet = XDC_OK;
   int				vl_Fd;
   char 			pl_PathEtFic[XDC_PATH_ABS_SIZE];
   XDY_Texte			pl_LigneFich;
   int				vl_NbPMVA = 0;
   EPMVA_DONNEES_PMVA		vl_DonneesPMVA,
   				*pl_DonCrt;
   char 			vl_Typ_Eqt;
   short			vl_NumeroLS = 0;
   int				vl_NbParam  = 0;
   int				vl_site;
  XZEXT_MSG_SOCKET             pl_message;
char          vl_adr[30];

      /*A Ouverture du fichier de configuration des PMV */
      /* Construction du nom du fichier XDF_Config_PMV_<NomMachine>  */
      sprintf ( pl_PathEtFic, "%s/%s_%s", XZEXC_PATH_CONFIG, XDF_Config_PMVA, pa_NomMachine );
      
      /*A Ouverture du fichier XDF_Config_PMVA_<NomMachine>  */
      if ( (vl_Fd = open (pl_PathEtFic, O_RDONLY)) <= 0 )
      {
         /* Ecriture trace absence fichier */
         XZST_03EcritureTrace( XZSTC_WARNING, " Manque fichier %s. \n", pl_PathEtFic ); 
         /* Sortie de la fonction */
         return ( XDC_NOK );
      }
      /*A Marquage des structures afin de determiner les eventuelles suppression */
      for ( pl_DonCrt = pg_debutPMVA; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant )
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
             /*B Extraire les parametre du PMVA */
             memset ( (void *) &vl_DonneesPMVA, 0, sizeof (vl_DonneesPMVA) );
	     XZST_03EcritureTrace( XZSTC_WARNING, "ligne %s",pl_LigneFich);
             vl_NbParam = sscanf ( pl_LigneFich, 	" %c %hd %s %hd %3c %d %d %s %d %s",  
               				&vl_Typ_Eqt,
					&vl_DonneesPMVA.Config.Numero.NoEqt,
					vl_DonneesPMVA.Config.NumeroST,
					&vl_NumeroLS,
					vl_DonneesPMVA.Config.AdresseRGS ,
					&vl_site,
					&vl_DonneesPMVA.Config.Port,
					vl_DonneesPMVA.Config.AdresseIP,
					&vl_DonneesPMVA.Config.Type,
					vl_DonneesPMVA.Config.IdLCR);
		if (vl_NbParam !=10) 
             vl_NbParam = sscanf ( pl_LigneFich, 	" %c %hd %s %hd %3c %d %d %s %d",  
               				&vl_Typ_Eqt,
					&vl_DonneesPMVA.Config.Numero.NoEqt,
					vl_DonneesPMVA.Config.NumeroST,
					&vl_NumeroLS,
					vl_DonneesPMVA.Config.AdresseRGS ,
					&vl_site,
					&vl_DonneesPMVA.Config.Port,
					vl_DonneesPMVA.Config.AdresseIP,
					&vl_DonneesPMVA.Config.Type);

	     if (vl_NbParam==9) {
		vl_NbParam=10;
		strcpy(vl_DonneesPMVA.Config.IdLCR,"");
	     }
             /* Si tout les parametre ont ete lu */
             if ( vl_NbParam == 10 )
             {
	     XZST_03EcritureTrace( XZSTC_FONCTION, "adresse RGS %s adresse IP %s port %d type %d idlcr %s",vl_DonneesPMVA.Config.AdresseRGS, vl_DonneesPMVA.Config.AdresseIP, vl_DonneesPMVA.Config.Port, vl_DonneesPMVA.Config.Type,vl_DonneesPMVA.Config.IdLCR);
	     vl_DonneesPMVA.Config.SiteGestion=vl_site;
             vl_DonneesPMVA.Config.Numero.Typ_Eqt  = vl_Typ_Eqt;
             vl_DonneesPMVA.Config.NumeroLS        = vl_NumeroLS;
                 /*B Inserer dans la liste des structures config PMV */
                 if ( ed_ins_liste_PMVA ( &vl_DonneesPMVA, sizeof(vl_DonneesPMVA.Config) ) == XDC_OK )
                 {
                    /* Incrementation du nombre de PMV */
                    vl_NbPMVA++;

			/*connexion eventuelle a temip*/

			/*
			if (strcmp(vl_DonneesPMVA.Config.AdresseIP,XZECC_IP_VIDE)) {
				if (vg_SockTEMIP==-1) {
					sprintf(vl_adr,"%s%d",XZEXC_ADR_SOCK_TEIM,
					XDC_EQT_PMVA);
					ex_cnx_client (    XZEXC_ADR_SOCK_TEPMVA,
						vl_adr,
						&vg_SockMask,
						&vg_SockTEMIP);
				}
                        sprintf ( pl_message, "%s%s %d %c%s",
                                XZEXC_CONFIG_IP,
                                vl_DonneesPMVA.Config.AdresseIP,
                                vl_DonneesPMVA.Config.Port,
                                XZECC_PROT_LCR,
                                XZEXC_FIN_MSG_SOCK );
			XZST_03EcritureTrace(XZSTC_WARNING,"trame %s",pl_message);
			if ( write ( vg_SockTEMIP, pl_message, strlen (pl_message) ) == XDC_NOK) {
                                XZST_03EcritureTrace(XZSTC_WARNING, " connexion impossible");
                                return (XDC_NOK);
                        }
			}
			*/

                 }
             }
         }
      }
      /*A Fermeture du fichier XDF_Presence_Equip_<NomMachine>  */
      XZSS_13FermerFichier( vl_Fd );
      
      /*A Suppression de la liste des donnees des elements ne faisant plus partie de la config */
      for ( pl_DonCrt = pg_debutPMVA; pl_DonCrt != NULL; )
      {
         /* Si le numero de ST est effacer */
         if ( strlen ( pl_DonCrt->Config.NumeroST ) <= 1 )
         {
            /* Suppression de la liste */
            ed_sup_liste_PMVA ( pl_DonCrt->Config.Numero.NoEqt );
            pl_DonCrt = pg_debutPMVA;
         }
         else
         {
            /* Passage a l'element suivant */
            pl_DonCrt = pl_DonCrt->Suivant;
         }
      }
     
      /* Ecriture trace de la config pour debug */
      for ( pl_DonCrt = pg_debutPMVA; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant )
      {
          pl_DonCrt->Etat_Service  = 0;
          pl_DonCrt->Contraste = XDC_CONTRASTENORMAL;
          pl_DonCrt->Mode = XZEXC_MODE_NORMAL;

	  strcpy ( pl_DonCrt->Etat.Texte1, "?" );
	  strcpy ( pl_DonCrt->Etat.Texte2, "?" );
	  strcpy ( pl_DonCrt->Etat.Texte3, "?" );
	  strcpy ( pl_DonCrt->Etat.Texte4, "?" );
	  strcpy ( pl_DonCrt->Etat.Texte5, "?" );
	  strcpy ( pl_DonCrt->Etat.Alternat1, "" );
	  strcpy ( pl_DonCrt->Etat.Alternat2, "" );
	  strcpy ( pl_DonCrt->Etat.Alternat3, "" );
	  strcpy ( pl_DonCrt->Etat.Alternat5, "" );
	  strcpy ( pl_DonCrt->Etat.Alternat4, "" );
	  pl_DonCrt->Etat.Clign1 = XDC_PMVA_PASCLIGNO;
	  pl_DonCrt->Etat.Clign2 = XDC_PMVA_PASCLIGNO;
	  pl_DonCrt->Etat.Clign3 = XDC_PMVA_PASCLIGNO;
	  pl_DonCrt->Etat.Clign4 = XDC_PMVA_PASCLIGNO;
	  pl_DonCrt->Etat.Clign5 = XDC_PMVA_PASCLIGNO;
	  pl_DonCrt->Etat.Affichage1 = XDC_PMVA_TEXTE;
	  pl_DonCrt->Etat.Affichage2 = XDC_PMVA_TEXTE;
	  pl_DonCrt->Etat.Affichage3 = XDC_PMVA_TEXTE;
	  pl_DonCrt->Etat.Affichage4 = XDC_PMVA_TEXTE;
	  pl_DonCrt->Etat.Affichage5 = XDC_PMVA_TEXTE;
	  pl_DonCrt->Etat.Flash = XDC_PMVA_PASFLASH;

	  strcpy ( pl_DonCrt->Etat_Commande.Texte1, "?" );
	  strcpy ( pl_DonCrt->Etat_Commande.Texte2, "?" );
	  strcpy ( pl_DonCrt->Etat_Commande.Texte3, "?" );
	  strcpy ( pl_DonCrt->Etat_Commande.Texte4, "?" );
	  strcpy ( pl_DonCrt->Etat_Commande.Texte5, "?" );
	  strcpy ( pl_DonCrt->Etat_Commande.Alternat1, "" );
	  strcpy ( pl_DonCrt->Etat_Commande.Alternat2, "" );
	  strcpy ( pl_DonCrt->Etat_Commande.Alternat3, "" );
	  strcpy ( pl_DonCrt->Etat_Commande.Alternat4, "" );
	  strcpy ( pl_DonCrt->Etat_Commande.Alternat5, "" );
	  sprintf ( pl_DonCrt->AffDefaut[0], "%d\0", XDC_PMVA_3POINTS );
	  pl_DonCrt->AffDefaut[1][0] = '\0';
	  pl_DonCrt->AffDefaut[2][0] = '\0';
	  pl_DonCrt->AffDefaut[3][0] = '\0';
	  pl_DonCrt->AffDefaut[4][0] = '\0';
	  pl_DonCrt->Etat_Commande.Clign1 = XDC_PMVA_PASCLIGNO;
	  pl_DonCrt->Etat_Commande.Clign2= XDC_PMVA_PASCLIGNO;
	  pl_DonCrt->Etat_Commande.Clign3 = XDC_PMVA_PASCLIGNO;
	  pl_DonCrt->Etat_Commande.Clign4 = XDC_PMVA_PASCLIGNO;
	  pl_DonCrt->Etat_Commande.Clign5 = XDC_PMVA_PASCLIGNO;
	  pl_DonCrt->Etat_Commande.Affichage1 = XDC_PMVA_TEXTE;
	  pl_DonCrt->Etat_Commande.Affichage2 = XDC_PMVA_TEXTE;
	  pl_DonCrt->Etat_Commande.Affichage3 = XDC_PMVA_TEXTE;
	  pl_DonCrt->Etat_Commande.Affichage4 = XDC_PMVA_TEXTE;
	  pl_DonCrt->Etat_Commande.Affichage5 = XDC_PMVA_TEXTE;
	  pl_DonCrt->Etat_Commande.Flash = XDC_PMVA_PASFLASH;

          XZST_03EcritureTrace ( XZSTC_DEBUG3, "Config PMV: %d %d %s %d %s %d",  
               				pl_DonCrt->Config.Numero.Typ_Eqt,
					pl_DonCrt->Config.Numero.NoEqt,
					pl_DonCrt->Config.NumeroST,
					pl_DonCrt->Config.NumeroLS,
					pl_DonCrt->Config.AdresseRGS,
					pl_DonCrt->Etat_Service );				
      }
      /* Retourne compte rendu OK */
      return ( vl_ValRet );
}





/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction effectue la lecture du fichier XDF_Affichage_Def_PMV
*  (affichage par defaut PMV pour mise au neutre )
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ec_lire_Affichage_Def_PMVA	( void )

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
   static char *version = "@(#)epma_cfg.c	1.20 03/10/98 : ec_lire_Affichage_Def_PMV" ;

   int				vl_Fd;
   char 			pl_PathEtFic[XDC_PATH_ABS_SIZE];
   XDY_Eqt			vl_NumPMV = 0;
   char *			vl_Mot = NULL;
   XDY_Texte			pl_LigneFich;
   XDY_Texte			pl_LigneFichMem;
   EPMVA_DONNEES_PMVA		*pl_DonCrt;
   XDY_Nom			vl_NomPMVFic  = "";			
   XDY_Nom			vl_NomPMV     = "";
   XDY_NomMachine		vl_NomMachine = "";			
   int				vl_index = 0;

      /*A Ouverture du fichier des affichage par defaut des PMV */
      /* Construction du nom du fichier XDF_Affichage_Def_PMV  */
      sprintf ( pl_PathEtFic, "%s/%s", XZEXC_PATH_CONFIG, XDF_Affichage_Def_PMVA );
      
      /* Ouverture du fichier XDF_Affichage_Def_PMV  */
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
            strcpy ( vl_NomPMVFic, ((vl_Mot = strtok(pl_LigneFich, ",")) != NULL) ? vl_Mot : "" );
            if ( strlen(vl_NomPMVFic) != 0 )
            {
               for ( pl_DonCrt = pg_debutPMVA; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant )
               {
                  ex_nom_equipement ( 	XDC_EQT_PMVA, 
                  			pl_DonCrt->Config.Numero.NoEqt, 
                  			NULL,
                  			vl_NomPMV,
                  			vl_NomMachine );
		  if ( strstr(vl_NomPMV, vl_NomPMVFic) != NULL )
		     break;
	       }
               strcpy ( pl_LigneFich, pl_LigneFichMem );
               strcpy ( vl_NomPMVFic, ((vl_Mot = strtok(pl_LigneFich, ",")) != NULL) ? vl_Mot : "" );
               vl_index = 0;
	       while ( (pl_DonCrt != NULL) && (vl_Mot = strtok ( NULL, "," )) != NULL )
	       {
		  if ( strlen(vl_Mot) < XDC_Lon_Ligne_PMVA )
		  {
		     strcpy ( pl_DonCrt->AffDefaut[vl_index], vl_Mot );
		  }
		  else
		  {
		     strncpy ( pl_DonCrt->AffDefaut[vl_index], vl_Mot, XDC_Lon_Ligne_PMVA-1 );
		     pl_DonCrt->AffDefaut[vl_index][XDC_Lon_Ligne_PMVA-1] = '\0';
		  }
		  if ( (atoi(pl_DonCrt->AffDefaut[vl_index]) != 0) || 
		       (++vl_index >= sizeof(pl_DonCrt->AffDefaut) / sizeof(pl_DonCrt->AffDefaut[0])) )
		      break;
	       }
            }
         }
      }

      /*A Fermeture du fichier XDF_Presence_Equip_<NomMachine>  */
      XZSS_13FermerFichier( vl_Fd );
      
      /* Ecriture trace de la config pour debug */
      for ( pl_DonCrt = pg_debutPMVA; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant )
          XZST_03EcritureTrace ( XZSTC_WARNING, "Affichage par defaut PMVA : %hd\t--%s--%s--%s--%s--%s",
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
   static char *version = "@(#)epma_cfg.c	1.20 03/10/98 : ec_controle" ;

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
   static char *version = "@(#)epma_cfg.c	1.20 03/10/98 : ec_init" ;
   EPMVA_DONNEES_PMVA *		pl_DonCrtPMVA;
   int				vl_Index = 0;
   short			vl_NbEqtLigne[21] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

   /*A
    * Lecture des fichiers de configuration et remplissage des structure de donnees 
    */
   /*A Configuration des PMV (appel de la fonction ec_lire_config_PMV) */
    if (ec_lire_config_PMVA ( pa_NomMachine ) == XDC_NOK) return (XDC_NOK);
   /*A Configuration des clignotements */
    ex_lire_cligno ( pa_NomMachine, vg_clignolent, vg_clignorapide );
   /*A Lecture user/password */
    ex_lire_idpassw ( pa_NomMachine, vg_idpassw );
   
   /*A Lecture fichier affichage par par defaut */
    ec_lire_Affichage_Def_PMVA();
    
   /*A Ouverture du fichier d'alarme */
   ex_OuvertureFichierMsg(vg_NomMachine);
   
   /*A Appel  xzat26_Init_Etats_Eqts pour Initialisation des etats equipement (HS ou Actif) */
   xzat26_Init_Etats_Eqts ( XDC_EQT_PMVA, XDC_ETAT_TOUS_EQT, vg_NomMachine );
   
   /*A
    * Connexion socket en mode client par appel de ec_init_cnx
    */
   ec_init_cnx ();

   ex_init_ip();
   /*A
    * Calcul du nombre maxi de PMV par ligne
    */
   for ( pl_DonCrtPMVA = pg_debutPMVA; pl_DonCrtPMVA != NULL; pl_DonCrtPMVA = pl_DonCrtPMVA->Suivant )
      vl_NbEqtLigne[pl_DonCrtPMVA->Config.NumeroLS]++;
   for ( vl_Index = 0; vl_Index < 21; vl_Index ++) 
      if ( *va_MaxEqtLigne < vl_NbEqtLigne[vl_Index] ) *va_MaxEqtLigne = vl_NbEqtLigne[vl_Index];


   /* Forcage du mode normal du contraste pour tous les PMVA */
   ep_maj_contraste_tous ( XDC_CONTRASTENORMAL );

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
   static char *version = "@(#)epma_cfg.c	1.20 03/10/98 : ec_init_cnx" ;

   EPMVA_DONNEES_PMVA *		pl_DonCrtPMVA;
   XZEXT_MASK *			pl_Socket;
   XZEXT_ADR_SOCK		pl_TacheTrt;
   XZEXT_ADR_SOCK		pl_TacheProt;
   int				vl_SockProt = 0;
char          vl_adr[30];
  XZEXT_MSG_SOCKET             pl_message;

   
   /*A
    * Connexion socket en mode client pour communiquer avec la tache TELMi
    */
   /*A Pour tout les PMV rechercher si la tache protocole associee existe */
   for ( pl_DonCrtPMVA = pg_debutPMVA; pl_DonCrtPMVA != NULL; pl_DonCrtPMVA = pl_DonCrtPMVA->Suivant )
   {
      /* Construction du nom de la tache protocole telmi*/
      sprintf ( pl_TacheProt, "%s%d", XZEXC_ADR_SOCK_TELM, pl_DonCrtPMVA->Config.NumeroLS );
      sprintf ( pl_TacheTrt, "%s%d", XZEXC_ADR_SOCK_PMVATELM, pl_DonCrtPMVA->Config.NumeroLS );
      /*A Recherche sur tout le masque des socket si la socket exite */
      for ( pl_Socket = vg_SockMask; pl_Socket != NULL; pl_Socket = pl_Socket->Suivant )
      {
	 /*si PMV non IP*/
         if ( !strcmp ( pl_Socket->adresse, pl_TacheProt ) && 
	      (!strcmp(pl_DonCrtPMVA->Config.AdresseIP,XZECC_IP_VIDE)))
         {
            /* Mise a jour du numero de socket */
            pl_DonCrtPMVA->Socket = pl_Socket->desc_sock;
      	    EnvoyerTrame(      "PS",
                         XZEXC_TRAME_ETAT_AFF_PMVA,
                         pl_DonCrtPMVA->Config.AdresseRGS,
                         XZEXC_CMD_INIT,
	                 XZEXC_PRIORITEFAIBLE,
                         pl_DonCrtPMVA->Socket );
            pl_DonCrtPMVA->EtatTrans = EPMVAC_EQT_OCCUP;
            /*B Arret: la socket avec la tache existe */
            break;
         }
	 else {
		pl_DonCrtPMVA->Socket = vg_SockTEMIP;
	 	/*EnvoyerTrameIP(      "PS",
			XZEXC_TRAME_ETAT_AFF_PMVA,
			pl_DonCrtPMVA->Config.AdresseIP,
			pl_DonCrtPMVA->Config.Port,
			pl_DonCrtPMVA->Config.AdresseRGS,
			XZEXC_CMD_INIT,
			XZEXC_PRIORITEFAIBLE,
			pl_DonCrtPMVA->Socket );
      		break;*/
        }
      }
      
      /*A Si la socket avec la tache telmi n'existe pas */
      if ( pl_Socket == NULL )
      {
	 if (!strcmp(pl_DonCrtPMVA->Config.AdresseIP,XZECC_IP_VIDE))  { /*PMV NON IP*/

         /*A Appel de ex_cnx_client pour etablir la liaison avec la tache protocole */
         if ( ex_cnx_client ( 	pl_TacheTrt,
				pl_TacheProt,
				&vg_SockMask,
				&vl_SockProt ) != XDC_OK )
	 {
	    /*B Ecriture Trace */
            XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de creer la connexion client  ." );	
            pl_DonCrtPMVA->EtatTrans = EPMVAC_EQT_PANNE;
	 }
	 else
	 {
	    /* Memorisation du numero de socket */
	    pl_DonCrtPMVA->Socket = vl_SockProt;
      	    EnvoyerTrame(      "PS",
                         XZEXC_TRAME_ETAT_AFF_PMVA,
                         pl_DonCrtPMVA->Config.AdresseRGS,
                         XZEXC_CMD_INIT,
	                 XZEXC_PRIORITEFAIBLE,
                         pl_DonCrtPMVA->Socket );
            pl_DonCrtPMVA->EtatTrans = EPMVAC_EQT_OCCUP;
	 }	
        }
	else { /*PMVA IP*/
               sprintf(vl_adr,"%s%d",XZEXC_ADR_SOCK_TEIM,XDC_EQT_PMVA);
                if (ex_cnx_client (    XZEXC_ADR_SOCK_TEPMVA,
                                vl_adr,
                                &vg_SockMask,
                                &vg_SockTEMIP) != XDC_OK )
                {
                        XZST_03EcritureTrace( XZSTC_WARNING,
                                "impossible de creer la connexion TEMIP");
                        pl_DonCrtPMVA->EtatTrans = EPMVAC_EQT_PANNE;
                }
                else {
                        pl_DonCrtPMVA->Socket = vg_SockTEMIP;
                        sprintf ( pl_message, "%s%s %d %c%s",
                                      XZEXC_CONFIG_IP,
                                      pl_DonCrtPMVA->Config.AdresseIP,
                                      pl_DonCrtPMVA->Config.Port,
                                      XZECC_PROT_LCR,
                                      XZEXC_FIN_MSG_SOCK );
                        sem_wait(&vg_semaphore); 
                        write ( vg_SockTEMIP, pl_message,
                                        strlen (pl_message));
                        sem_post(&vg_semaphore); 

                        EnvoyerTrameIP("PS",
                                            XZEXC_TRAME_ETAT_AFF_PMVA,
                                            pl_DonCrtPMVA->Config.AdresseIP,
                                            pl_DonCrtPMVA->Config.Port,
                                            pl_DonCrtPMVA->Config.AdresseRGS,
                                            XZEXC_CMD_INIT,
                                            XZEXC_PRIORITEFAIBLE,
                                            pl_DonCrtPMVA->Socket );
                }

	}
      }
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
*   Liberation des ressources memoires
*   Envoyer le message aux taches protocoles
*   Attendre x secondes
*   Fermeture des sockets
*
------------------------------------------------------*/
{
   static char *version = "@(#)epma_cfg.c	1.20 03/10/98 : ec_sortir" ;

   EPMVA_DONNEES_PMVA 		*pl_PMV, 	*pl_PMVALiberer;
   XZEXT_MASK *			pl_Socket;
   XZEXT_ADR_SOCK		pl_TacheProt,
   				pl_TacheTrt;
   XZEXT_MSG_SOCKET 		pl_message;
   XDY_Datagroup		pl_NomDG;
   int				vl_resultat;

   /*A Desabonnement a JourNuit */
   XDG_EncodeDG2 ( pl_NomDG, "Einit_JourNuit_PMVA", vg_NomMachine) ;			
   XZEA29_Abt_Etat_JourNuit(XZEAC_FERMER_ABONNEMENT,pl_NomDG,ep_journuit,vg_NomSite,&vl_resultat);

  /*A
   * Liberation des ressources memoires
   */

   /*A Liberation des ressources memoires des donnees PMV */
   for ( pl_PMV = pg_debutPMVA; pl_PMV != NULL; free(pl_PMVALiberer) )
   {
      pl_PMVALiberer = pl_PMV;
      pl_PMV         = pl_PMV->Suivant;
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
