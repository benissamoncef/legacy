/*E*/
/*Fichier :  $Id: ex_malt.c,v 1.25 2020/11/03 18:12:42 pc2dpdy Exp $      Release : $Revision: 1.25 $        Date : $Date: 2020/11/03 18:12:42 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  : EQUEXT
------------------------------------------------------
* MODULE COMMUN * FICHIER ex_malt.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module commun pour envoi des alertes >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	20 Oct 1994	: Creation
* Mismer.D	version 1.2	23 Nov 1994	:
* Mismer.D	version 1.3	06 Jan 1995	:
* Mismer.D	version 1.4	28 Jan 1995	:
* Fontaine.C	version 1.5	23 Mai 1995	: Reprise historique
* Mismer.D	version 1.6	21 Mai 1996	: Ajout raz ligne lecture fichier texte des alertes (DEM/1136)
* Mismer.D	version 1.7	23 Mai 1996	: Mise a jour historique avec numero de DEM pour v1.6
* Mismer.D  	version 1.8	02 Sep 1996	: Ajout type machine (RADT_W) 
* Mismer.D  	version 1.9	02 Dec 1996	: Modif pour equipement generique LCR (DEM/1232) 
* Mismer.D	version 1.10	10 Fev 1997	: Modif TDP (DEM/1395)
* Mismer.D	version 1.11	15 Jan 1998	: Modif FAC et FAU (DEM/1544)
* JMG           version 1.12    21/11/07        : ajout site de gestion DEM/715
* JMG		version 1.14	22/11/07	: correction ex_env_alerte DEM/715
* JMG		version 1.15	22/11/07	: correction ex_env_alerte DEM/715
* JMG		version 1.16	02/10/09	: secto DEM887
* JMG		version 1.17	12/12/10	: portage linux
* JPL		17/12/10 : Migration architecture HP ia64 (DEM 961) : suppression erreurs mineures  1.18
* JPL		23/02/11 : Migration architecture HP ia64 (DEM 975) : suppression warnings mineurs  1.19
* JMG		21/03/11 : ajout ex_env_alerte_localisee 1.20
* JPL		29/11/11 : Reprise construction textes alertes (cas sans modele et args. variables)  1.21
* JPL		01/12/11 : Support de parametres pour les textes des alertes localisees  1.22
* JPL		04/04/12 : Ajout de la trace des parametres de localisation des alertes localisees  1.23
* JMG   09/03/17        : REGIONALISATION - DEM1220 1.24
* JMG	07/03/2 : EMS 1.25
------------------------------------------------------*/

/* fichiers inclus */

#include "stdarg.h"

#include "xzaa.h"
#include "xzsa.h"

#include "ex_malt.h"
#include "ex_mrtf.h"

/* definitions de constantes */

/* definitions de types locaux */

typedef	struct	
{
	int	Numero;
	char	Texte[XDC_Lon_TexteAlerte];
	void *	Suivant;
}	EXT_DONNEES_ALERTE;

/* definition de macro locales */

/* declaration de variables locales */

EXT_DONNEES_ALERTE *	pg_DebutAlerte = NULL;

static char *version = "$Id: ex_malt.c,v 1.25 2020/11/03 18:12:42 pc2dpdy Exp $      : ex_malt" ;

/* declaration de fonctions internes */

/* Fonction interne pour insertion dans la liste des textes d'alerte */
EX_FONCT_INSERER   ( EXT_DONNEES_ALERTE, ex_ins_liste_alerte, pg_DebutAlerte, Numero, Suivant, Suivant)

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Sauvegarde d'un message RTworks dans un fichier ASCII.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ex_lecture_fichier_alerte (	 )

/*
* ARGUMENTS EN ENTREE :
*
*  aucun
*
* ARGUMENTS EN SORTIE :
*  aucun
*
* CODE RETOUR :
*  XDC_OK / XDC_NOK
*
* CONDITION D'UTILISATION
* 
* FONCTION 
*  Construction du message et envoi du message
*
------------------------------------------------------*/
{
   static char *version = "$Id: ex_malt.c,v 1.25 2020/11/03 18:12:42 pc2dpdy Exp $      :  ex_lecture_fichier_alerte";

   int				vl_Fd;
   char 			pl_PathEtFic[XDC_PATH_ABS_SIZE];
   XDY_Texte			pl_LigneFich;
   EXT_DONNEES_ALERTE		vl_DonneesAlerte = {0},
   				*pl_DonCrt;
   int				vl_Debut    = XDC_NOK;
   int				vl_index    = 0;
   XZSCT_NomTache  		vl_NomTache;
   
      /* Lecture du nom de la tache */
      XZSC_06NomTache( vl_NomTache );
      for ( vl_index = 0; vl_NomTache[vl_index] != '\0'; vl_index++ )
         if ( vl_NomTache[vl_index] == '.' ) vl_NomTache[vl_index] = '\0';
      
      /*A Ouverture du fichier des textes d'alerte */
      /* Construction du nom du fichier des textes d'alerte */
      sprintf ( pl_PathEtFic, "%s/%s", XZEXC_PATH_FIC_TEXTE_ALERTE, XDF_Texte_Alerte );
      sprintf ( pl_PathEtFic, "%s/%s", XZEXC_PATH_CONFIG1, XDF_Texte_Alerte );
      
      /*A Ouverture du fichier des textes d'alerte */
      if ( XZSS_12OuvrirFichier( O_RDONLY , pl_PathEtFic, &vl_Fd ) != XDC_OK )
      {
         /* Ecriture trace absence fichier */
         XZST_03EcritureTrace( XZSTC_WARNING, " Manque fichier %s. \n", pl_PathEtFic ); 
         /* Sortie de la fonction */
         return ( XDC_NOK );
      }
 
      /*A Lecture de chaque ligne du fichier */
      while ( ex_LireLigneFich ( vl_Fd, pl_LigneFich, sizeof ( pl_LigneFich ) ) != XZEXC_EOF ) 
      {
      	  /* Si debut de la liste des d'alerte de la tache concernee */
          if ( !strncmp(&pl_LigneFich[1], vl_NomTache, strlen(vl_NomTache)) )
          {
              /* Memorisation du debut de la liste */
              vl_Debut = XDC_OK;
          }
          /* Si fin de la liste des d'alerte de la tache concernee */
          if ( ( vl_Debut == XDC_OK ) && ( !strncmp(pl_LigneFich, "#fin", 4) ) )
          {
              /* fin de la lecture du fichier */
              break;
          }
          /* Lire ligne suivante si c'est une ligne de commentaire */
          if ( ( vl_Debut == XDC_OK ) && ( *pl_LigneFich != XDF_COMMENTAIRE ) )
          {
             /*B Extraire le texte de l'alerte */
             memset ( vl_DonneesAlerte.Texte, '\0', sizeof(vl_DonneesAlerte.Texte) );
             if ( sscanf ( pl_LigneFich, "%d %256c",  
					&vl_DonneesAlerte.Numero,
					vl_DonneesAlerte.Texte ) == 2 )
             {
                 /*B Inserer dans la liste des structures texte d'alerte */
                 ex_ins_liste_alerte ( &vl_DonneesAlerte, 0 );
             }
         }
      }
      /*A Fermeture du fichier des textes d'alerte  */
      XZSS_13FermerFichier( vl_Fd );
      
     
      /* Ecriture trace de la config pour debug */
      for ( pl_DonCrt = pg_DebutAlerte; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant )
      {
          XZST_03EcritureTrace ( XZSTC_DEBUG1, "Alerte: %d %s",  
               			 pl_DonCrt->Numero,
				 pl_DonCrt->Texte );				
      }
      /* Retourne compte rendu OK */
      return ( XDC_OK );

}



/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Envoi d'une alerte vers le sous-systeme BASEDD
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int ex_env_alerte  ( 	XDY_Horodate	va_Horodate,
			XDY_TypeEqt	va_TypeEqt,
			XDY_Eqt		va_NumEqt,
			XDY_TypeAlerte	va_Type,
			int		va_Mode,
			XDY_District	va_SiteGestion,
			... )


/*
* ARGUMENTS EN ENTREE :
*
*  
*
* ARGUMENTS EN SORTIE :
*  aucun
*
* CODE RETOUR :
*  XDC_OK / XDC_NOK
*
* CONDITION D'UTILISATION
* 
* FONCTION 
*  Recherche du texte de l'alerte et mise a jour de celui-ci
*  Construction du message et envoi du message si pas mode isole
*  Si mode isole ou erreur envoi msg alors ecriture du message dans fichier
*
------------------------------------------------------*/
{
EXT_DONNEES_ALERTE	*pl_DonCrt = NULL;
XDY_TexteAlerte		vl_TexteAlerte;
va_list			vl_listeArgument;
char			*vl_arg0;
XDY_Datagroup		pl_NomDg;
XDY_NomSite		pl_NomSite;
int			vl_RetCreerAlerte = XDC_OK;
XDY_NomMachine  pl_NomMachine;
static int  vl_TypeMac       = 0;

   /* Sil s'agit d'un PC simplifie alors return */
   if ( !vl_TypeMac )
   {
      /*A Recuperation du nom de la machine */
      if ( XZSC_07NomMachine(pl_NomMachine) != XDC_OK)
      {
         /* Ecriture Trace */
         XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom de la machine impossible . " );
      } 
      if ( XZSA_17TypeMachine ( pl_NomMachine, &vl_TypeMac ) != XDC_OK )
      {
         XZSA_20InitAdmin();
         if ( XZSA_17TypeMachine ( pl_NomMachine, &vl_TypeMac ) != XDC_OK )
            XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de trouve le type de la machine" );
      }
   }
   if ( (vl_TypeMac == XZSCC_TYPEM_PCS) && (va_TypeEqt != XDC_EQT_MET) ) 
   	return ( XDC_OK );

   XZST_03EcritureTrace( XZSTC_FONCTION, "IN : env_alerte :H:%lf T:%d E:%d N:%d M:%d S:%d",va_Horodate,va_TypeEqt,va_NumEqt,va_Type , va_Mode, va_SiteGestion);

   /* Si la liste des textes d'alerte est vide */
   if ( pg_DebutAlerte == NULL )
   {
      /* Lecture de la liste des textes d'alerte */
      ex_lecture_fichier_alerte ();
   }
   
   /* Recherche de l'alerte concernee */
   for ( pl_DonCrt = pg_DebutAlerte; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant )
   {
       /* Si Num alerte trouve */
      if  ( pl_DonCrt->Numero == va_Type )
      {
         /* Fin de la recherche */
         break;
      }
   }
   XZST_03EcritureTrace( XZSTC_DEBUG3, "Libelle alerte: %s", (pl_DonCrt == NULL) ? "NULL" : pl_DonCrt->Texte);


   /* Construction du texte d'alerte */
#ifdef _HPUX_SOURCE
   memset ( vl_TexteAlerte, '\0', sizeof(XDY_TexteAlerte)-1 );
#else
   strcpy(vl_TexteAlerte,"");
#endif

   /* Initialisation du Pointeur d'argument sur le premier argument non nomme */
   va_start (vl_listeArgument, va_SiteGestion );

   if ( pl_DonCrt == NULL )
   {
      /* JPL 29/11/11 utiliser le premier argument comme motif de texte d'alerte en son absence */
      vl_arg0 = va_arg (vl_listeArgument, char *);
      if ( vl_arg0 != NULL )
      {
         vsprintf (  vl_TexteAlerte, vl_arg0, vl_listeArgument );
      }
   }
   else
   {
      /* construire le texte de l'alerte avec le modele et les eventuels arguments fournis */
      vsprintf (  vl_TexteAlerte, pl_DonCrt->Texte, vl_listeArgument );
   }

   va_end (vl_listeArgument);

    XZST_03EcritureTrace( XZSTC_WARNING, "Mode = %d",va_Mode);

   /* Si pas en mode isole */
   if ( va_Mode != XZEXC_MODE_ISOLE )
   {
      /* Envoi de l'alerte par xzaa01_Creer_Alerte */
      XZST_03EcritureTrace( XZSTC_WARNING, "Envoi alerte avec %lf TypeEqt:%d NumEqt:%d Type:%d Texte:%s",
         					   va_Horodate,
						   va_TypeEqt,
						   va_NumEqt,
						   va_Type,
						   vl_TexteAlerte );
					   
      vl_RetCreerAlerte = xzaa01_Creer_Alerte ( va_Horodate,
						   va_TypeEqt,
						   va_NumEqt,
						   va_Type,
						   vl_TexteAlerte,
						   va_SiteGestion);
   }
      
   /* Si mode isole  ou erreur sur xzaa01_Creer_Alerte */
   if  ( ( va_Mode == XZEXC_MODE_ISOLE ) || ( vl_RetCreerAlerte != XDC_OK ) )
   {
      /* Ecriture d'une alerte dans le fichier des message RTworks */
      XZST_03EcritureTrace( XZSTC_DEBUG1, "Ecriture dans File Msg XDM_AA_alerte %lf TypeEqt:%d NumEqt:%d Type:%d Texte:%s",
         					   va_Horodate,
						   va_TypeEqt,
						   va_NumEqt,
						   va_Type,
						   vl_TexteAlerte );
						   
	 
      /* Recuperation du nom du site */
      XZSC_04NomSite ( pl_NomSite );
      /* Concatenation du DataGroup */
      XDG_EncodeDG2( pl_NomDg, XDG_AA, pl_NomSite );

      switch (va_SiteGestion) {
       case (XDC_CI) : XDG_EncodeDG2( pl_NomDg, XDG_AA,  XDC_NOM_SITE_CI);
			break;
       case (XDC_VC) : XDG_EncodeDG2( pl_NomDg, XDG_AA,  XDC_NOM_SITE_VC);
			break;
       case (XDC_DP) : XDG_EncodeDG2( pl_NomDg, XDG_AA,  XDC_NOM_SITE_DP);
			break;
       case (XDC_CA) : XDG_EncodeDG2( pl_NomDg, XDG_AA,  XDC_NOM_SITE_CA);
			break;
       default : XDG_EncodeDG2( pl_NomDg, XDG_AA, pl_NomSite );
		break;
      }

   }
   
   return ( XDC_OK );

}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Liberation memoire texte alerte.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void ex_liberation_memoire_alerte (	 )

/*
* ARGUMENTS EN ENTREE :
*
*  aucun
*
* ARGUMENTS EN SORTIE :
*  aucun
*
* CODE RETOUR :
*  aucun
*
* CONDITION D'UTILISATION
* 
* FONCTION 
*  Liberation memoire texte alerte.
*
------------------------------------------------------*/
{
static char *version = "$Id: ex_malt.c,v 1.25 2020/11/03 18:12:42 pc2dpdy Exp $      :  ex_liberation_memoire_alerte ";
EXT_DONNEES_ALERTE	*pl_DonCrt,
			*pl_DonSup;

      /* Liberation memoire texte alerte */
      for ( pl_DonCrt = pl_DonSup = pg_DebutAlerte;
            pl_DonCrt != NULL; 
            pl_DonSup = pl_DonCrt )
      {
          pl_DonCrt = pl_DonCrt->Suivant;				
          free ( pl_DonSup );
      }
      
      pg_DebutAlerte = NULL;
      

}

	

/*X*/
/*------------------------------------------------------
*
* IPHONE
* SERVICE RENDU :
*  Envoi d'une alerte localisee vers le sous-systeme BASEDD
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int ex_env_alerte_localisee  (  XDY_Horodate    va_Horodate,
                                XDY_TypeEqt     va_TypeEqt,
                                XDY_Eqt         va_NumEqt,
                                XDY_TypeAlerte  va_Type,
                                int             va_Mode,
                                XDY_District    va_SiteGestion,
                                char           *va_autoroute,
                                XDY_PR          va_PR,
                                XDY_Sens        va_sens,
                                ...
                             )
/*
* ARGUMENTS EN ENTREE :
*
*
*
* ARGUMENTS EN SORTIE :
*  aucun
*
* CODE RETOUR :
*  XDC_OK / XDC_NOK
*
* CONDITION D'UTILISATION
*
* FONCTION
*  Recherche du texte de l'alerte et mise a jour de celui-ci
*  Construction du message et envoi du message si pas mode isole
*  Si mode isole ou erreur envoi msg alors ecriture du message dans fichier
*
------------------------------------------------------*/
{
EXT_DONNEES_ALERTE      *pl_DonCrt = NULL;
XDY_TexteAlerte         vl_TexteAlerte;
va_list			vl_listeArgument;
char			*vl_arg0;
XDY_Datagroup           pl_NomDg;
XDY_NomSite             pl_NomSite;
int                     vl_RetCreerAlerte = XDC_OK;
XDY_NomMachine  pl_NomMachine;
static int  vl_TypeMac       = 0;

   XZST_03EcritureTrace( XZSTC_FONCTION, "IN : env_alerte_localisee :H:%lf T:%d E:%d N:%d M:%d S:%d LOC:%s-%d-%d",va_Horodate,va_TypeEqt,va_NumEqt,va_Type , va_Mode, va_SiteGestion, va_autoroute, va_sens, va_PR);

   /* Si la liste des textes d'alerte est vide */
   if ( pg_DebutAlerte == NULL )
   {
      /* Lecture de la liste des textes d'alerte */
      ex_lecture_fichier_alerte ();
   }

   /* Recherche de l'alerte concernee */
   for ( pl_DonCrt = pg_DebutAlerte; pl_DonCrt != NULL; pl_DonCrt = pl_DonCrt->Suivant )
   {
       /* Si Num alerte trouve */
      if  ( pl_DonCrt->Numero == va_Type )
      {
         /* Fin de la recherche */
         break;
      }
   }
   XZST_03EcritureTrace( XZSTC_DEBUG3, "Libelle alerte: %s", (pl_DonCrt == NULL) ? "NULL" : pl_DonCrt->Texte);


   /* Construction du texte d'alerte */
#ifdef _HPUX_SOURCE
   memset ( vl_TexteAlerte, '\0', sizeof(XDY_TexteAlerte)-1 );
#else
   strcpy(vl_TexteAlerte,"");
#endif

   /* Initialisation du Pointeur d'argument sur le premier argument non nomme */
   va_start (vl_listeArgument, va_sens );

   if ( pl_DonCrt == NULL )
   {
      /* JPL 29/11/11 utiliser le premier argument comme motif de texte d'alerte en son absence */
      vl_arg0 = va_arg (vl_listeArgument, char *);
      if ( vl_arg0 != NULL )
      {
         vsprintf (  vl_TexteAlerte, vl_arg0, vl_listeArgument );
      }
   }
   else
   {
         vsprintf (  vl_TexteAlerte, pl_DonCrt->Texte, vl_listeArgument );
   }

   va_end (vl_listeArgument);

   /* Si pas en mode isole */
   if ( va_Mode != XZEXC_MODE_ISOLE )
   {
      /* Envoi de l'alerte par xzaa01_Creer_Alerte */
      XZST_03EcritureTrace( XZSTC_WARNING, "Envoi alerte localisee avec %lf TypeEqt:%d NumEqt:%d Type:%d Texte:%s AUT=%s PR=%d SENS=%d",
                                                   va_Horodate,
                                                   va_TypeEqt,
                                                   va_NumEqt,
                                                   va_Type,
                                                   vl_TexteAlerte ,
                                                   va_autoroute,
                                                   va_PR,
                                                   va_sens);

      vl_RetCreerAlerte = xzaa01_Creer_Alerte_Localisee ( va_Horodate,
                                                   va_TypeEqt,
                                                   va_NumEqt,
                                                   va_Type,
                                                   vl_TexteAlerte,
                                                   va_SiteGestion,
                                                   va_autoroute,
                                                   va_PR,
                                                   va_sens);
   }
   /* Si mode isole  ou erreur sur xzaa01_Creer_Alerte */
   if  ( ( va_Mode == XZEXC_MODE_ISOLE ) || ( vl_RetCreerAlerte != XDC_OK ) )
   {
      /* Ecriture d'une alerte dans le fichier des message RTworks */
      XZST_03EcritureTrace( XZSTC_DEBUG1, "Ecriture dans File Msg XDM_AA_alerte %lf TypeEqt:%d NumEqt:%d Type:%d Texte:%s",
                                                   va_Horodate,
                                                   va_TypeEqt,
                                                   va_NumEqt,
                                                   va_Type,
                                                   vl_TexteAlerte );


      /* Recuperation du nom du site */
      XZSC_04NomSite ( pl_NomSite );
      /* Concatenation du DataGroup */
      XDG_EncodeDG2( pl_NomDg, XDG_AA, pl_NomSite );

      switch (va_SiteGestion) {
       case (XDC_CI) : XDG_EncodeDG2( pl_NomDg, XDG_AA,  XDC_NOM_SITE_CI);
                        break;
       case (XDC_VC) : XDG_EncodeDG2( pl_NomDg, XDG_AA,  XDC_NOM_SITE_VC);
                        break;
       case (XDC_DP) : XDG_EncodeDG2( pl_NomDg, XDG_AA,  XDC_NOM_SITE_DP);
                        break;
       case (XDC_CA) : XDG_EncodeDG2( pl_NomDg, XDG_AA,  XDC_NOM_SITE_CA);
                        break;
       default : XDG_EncodeDG2( pl_NomDg, XDG_AA, pl_NomSite );
                break;
      }
   }

   return ( XDC_OK );

}

