/*E*/
/*Fichier :  $Id: eini_syn.c,v 1.40 2017/03/31 17:38:35 pc2dpdy Exp $      Release : $Revision: 1.40 $        Date : $Date: 2017/03/31 17:38:35 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEINI * FICHIER eini_syn.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module de creation des fichiers de config pour la tache SYN >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D 	version 1.1 	26 Sep 1994 	: Creation
* Mismer.D 	version 1.2 	14 Dec 1994 	:
* Mismer.D 	version 1.3 	21 Jan 1995 	:
* Mismer.D 	version 1.4 	23 Jan 1995 	:
* Mismer.D 	version 1.5 	25 Jan 1995 	:
* Mismer.D 	version 1.6 	25 Jan 1995 	: RAS
* Mismer.D 	version 1.7 	25 Jan 1995 	: RAS
* Mismer.D 	version 1.8 	26 Jan 1995 	: 
* Mismer.D 	version 1.9 	26 Jan 1995 	: RAS
* Mismer.D 	version 1.10 	28 Jan 1995 	:
* Mismer.D 	version 1.11 	28 Jan 1995 	: RAS
* Mismer.D 	version 1.12 	31 Jan 1995 	:
* Mismer.D 	version 1.13 	09 Fev 1995 	:
* Mismer.D 	version 1.14 	13 Avr 1995 	:
* Fontaine.C 	version 1.15 	19 Mai 1995 	: Reprise entete, historique
* Volcic.F 	version 1.16 	19 Mai 1995 	: Ajout fichier utilisateurs
* Volcic.F 	version 1.16 	22 Mai 1995 	: Modif fichier utilisateurs
* Mismer.D	version 1.17 	22 Mai 1995 	: Ajout fichier affichage PMV par defaut
* Mismer.D	version 1.18 	23 Jun 1995 	: Utilisation ex_presence_machine au lieu de ei_test_machine
* Mismer.D	version 1.19 	27 Jun 1995 	: Modif diffusion supleant
* Mismer.D	version 1.20 	29 Jun 1995 	: Modif diffusion supleant v2
* Mismer.D	version 1.21 	03 Sep 1995 	: Ajout fichier Nom Serveur
* Mismer.D	version 1.22 	16 Nov 1995 	: Suppression des recopies de fichier pour le tesur du serveur
* Mismer.D	version 1.23 	13 Dec 1995 	: Correction diffusion fichier LCR vers PO
* Mismer.D	version 1.24 	15 Dec 1995 	: Correction diffusion fichier commun LT
* Mismer.D	version 1.25 	15 Mar 1996 	: Correction compte rendu  (DEM/1100)
* Mismer.D      version 1.26    06 Sep 1996     : Ajout XZEXC_PATH_REFERENCE   (RADT_W)
* Mismer.D	version 1.27	12 Nov 1996	: Modif METEO (DEM/1232)
* Mismer.D	version 1.28	10 Dec 1996	: Suite modif METEO (DEM/1232)
* Mismer.D	version 1.29	10 Fev 1997	: Modif TDP (DEM/1395)
* Mismer.D	version 1.30	24 Mar 1997	: Modif TDP suite (DEM/1396)
* JMG		version 1.32	11/10/05	: ajout PMVA /BAF/BAD 1.32
* Niepceron	version 1.33	15/11/05	: Prise en compte du fichier Param_LS_Est pour Nice et Mandelieu (ligne radt a 9600 au lieu de 1200) DEM537
* AAZ       version         17/10/2007  : GTC Renovee prise en compte Alerte DEM 664
* JMG	02/10/09 	secto DEM887
* JMG	20/04/11	ajout pclose pour eviter pb process zombi sur ia64 1.36
* JPL	17/02/12	Migration architecture HP ia64 (DEM 975) : suppression warnings mineurs  1.37
* VR	20/12/11		: Ajout PRV (DEM/1016) 1.38
* JMG	24/06/16	ajout XDF_ST40 1.39
* JMG   09/03/17        : REGIONALISATION - DEM1220  1.40
* LCL   26/09/21        : Adaptation CNA DEM-483
* GGY	12/10/23		: Adaptaion CNA PODFA ou SDCA1 (DEM483)
------------------------------------------------------*/

/* fichiers inclus */

#include "ex_msok.h"

#include "eini_cfg.h"
#include "eini_syn.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)eini_syn.c	1.33 03/26/07      : eini_syn" ;
  
/* declaration de variables globales */
extern XDY_Mot		vg_SitePrincipal;

/* declaration de fonctions internes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ei_recopie_fichier ( 	XDY_NomMachine	pa_NomMachine,
				XDY_NomMachine	pa_NomMachineFic,
				char *		pa_NomFic )

/*
* ARGUMENTS EN ENTREE :
*   XDY_NomMachine	pa_NomMachine	nom machine destinatrice
*   char *		pa_NomFic 	nom du fichier a recopier
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   Diffussion des fichiers de configuration vers les machines concernees
*
------------------------------------------------------*/
{
XDY_NomMachine	pl_NomMachine;
char 		pl_NomFichier[XDC_PATH_ABS_SIZE];

   /* Recuperation du nom de la machine */
   if ( XZSC_07NomMachine(pl_NomMachine) != XDC_OK)
   {
      /* Ecriture Trace */
      XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom de la machine impossible . " );
   } 
   else
   {
      if ( !strcmp ( pl_NomMachine, pa_NomMachine ) )
      {
         sleep ( 2 );
      }
      else
      {
         /* Recopie du fichier de config vers la machine concernee */
         if ( strlen(pa_NomFic) > 0 )
         {
            sprintf ( pl_NomFichier, "%s_%s", 
               		             pa_NomFic,
               		             pa_NomMachineFic );
            if ( XZSS_07TransfertFichier ( pl_NomMachine,
               				   pl_NomFichier,
               				   XZEXC_PATH_DIFF_DEST,
               				   pa_NomMachine,
               				   "/",
               				   XZEXC_PATH_DIFF_DEST ) != XDC_OK )
            {
                XZST_03EcritureTrace( XZSTC_WARNING, "Transfert fichier %s de %s vers %s impossible",
               					      pl_NomFichier,
               					      pl_NomMachine,
               					      pa_NomMachine );
            } 
            else
            {
                XZST_03EcritureTrace( XZSTC_INFO, "Transfert fichier %s de %s vers %s OK",
               					   pl_NomFichier,
               					   pl_NomMachine,
               					   pa_NomMachine );
            }
         } 
      }
   }
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ei_recopie_fichier_standard ( 	char *		pa_NomFic )

/*
* ARGUMENTS EN ENTREE :
*   char *		pa_NomFic 	nom du fichier a recopier
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   Diffussion des fichiers de configuration vers les machines concernees
*
------------------------------------------------------*/
{
XDY_NomMachine	pl_NomMachine;
XDY_NomMachine	pl_NomMachineSrc;
char 		pl_RepReference[XDC_PATH_ABS_SIZE] = "";


   /* Recuperation du nom de la machine */
   if ( XZSC_07NomMachine(pl_NomMachine) != XDC_OK )
   {
      /* Ecriture Trace */
      XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom de la machine impossible . " );
   } 
   else
   {
      /* Recopie du fichier de config vers la machine concernee */
	  if (vg_SitePrincipal == XDC_CI)
	  {
      	strcpy ( pl_NomMachineSrc, "PODFA" );
	  }
	  else
	  {
		strcpy ( pl_NomMachineSrc, "SDCA1" );
	  }
      sprintf ( pl_RepReference, "%s/deq", XDC_PATHREFFIC );

      if ( strlen(pa_NomFic) > 0 )
      {
         if ( XZSS_07TransfertFichier (    pl_NomMachineSrc,
               				   pa_NomFic,
               				   pl_RepReference,
               				   pl_NomMachine,
               				   "/",
               				   XZEXC_PATH_DIFF_DEST ) != XDC_OK )
         {
             XZST_03EcritureTrace( XZSTC_WARNING, "Copie fichier %s de %s vers %s impossible",
                                                   pl_NomMachineSrc,                  
               					   pa_NomFic,
               					   pl_NomMachine );
         } 
         else
         {
             XZST_03EcritureTrace( XZSTC_INFO, "Copie fichier %s de %s/%s vers %s OK",
                                                   pl_NomMachineSrc,                  
               					   pl_RepReference,
               					   pa_NomFic,
               					   pl_NomMachine );
         } 
      }
    }
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ei_recopie_fichier_standard_LS ( 	char *		pa_NomFic, char *		pa_NomFicDest )

/*
* ARGUMENTS EN ENTREE :
*   char *		pa_NomFic 	nom du fichier a recopier
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   Diffussion des fichiers de configuration vers les machines concernees
*
------------------------------------------------------*/
{
XDY_NomMachine	pl_NomMachine;
XDY_NomMachine	pl_NomMachineSrc;
char 		pl_RepReference[XDC_PATH_ABS_SIZE] = "";
char 		pl_FicDest[XDC_PATH_ABS_SIZE] = "";

  XZST_03EcritureTrace( XZSTC_WARNING, "ei_recopie_fichier_standard_LS : source = %s, dest = %s . ", pa_NomFic, pa_NomFicDest );

   /* Recuperation du nom de la machine */
   if ( XZSC_07NomMachine(pl_NomMachine) != XDC_OK )
   {
      /* Ecriture Trace */
      XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom de la machine impossible . " );
   } 
   else
   {
		/* Recopie du fichier de config vers la machine concernee */
	  if (vg_SitePrincipal == XDC_CI)
	  {
      	strcpy ( pl_NomMachineSrc, "PODFA" );
	  }
	  else
	  {
		strcpy ( pl_NomMachineSrc, "SDCA1" );
	  }
      sprintf ( pl_RepReference, "%s/deq", XDC_PATHREFFIC );
      pa_NomFicDest;
      if ( strlen(pa_NomFic) > 0 )
      {
         if ( XZSS_07TransfertFichier (    pl_NomMachineSrc,
               				   pa_NomFic,
               				   pl_RepReference,
               				   pl_NomMachine,
               				   pa_NomFicDest,
               				   XZEXC_PATH_DIFF_DEST ) != XDC_OK )
         {
             XZST_03EcritureTrace( XZSTC_WARNING, "Copie fichier %s de %s vers %s impossible",
                                                   pl_NomMachineSrc,                  
               					   pa_NomFic,
               					   pl_NomMachine );
         } 
         else
         {
             //XZST_03EcritureTrace( XZSTC_INFO, "Copie fichier %s de %s/%s vers %s OK",
             XZST_03EcritureTrace( XZSTC_WARNING, "Copie fichier %s de %s/%s vers %s OK",
                                                   pl_NomMachineSrc,                  
               					   pl_RepReference,
               					   pa_NomFic,
               					   pl_NomMachine );
         } 
      }
    }
}







/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_test_version ( 	XDY_NomMachine	pa_NomMachine )

/*
* ARGUMENTS EN ENTREE :
*   XDY_NomMachine	pa_NomMachine	nom machine test
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   Test si version a jour sur la machine destinatrice
*
------------------------------------------------------*/
{
        char    pl_Cmd[512]              = "";
        char    pl_ResCmd[512]           = "";
        FILE    *pl_P                    = NULL;

      /* construit la commande test version de la machine demandee */
      sprintf(pl_Cmd,"%s/LCR_%s", XZEXC_PATH_DIFF_DEST, pa_NomMachine );
      if ( access(pl_Cmd, F_OK) )
      {
         /* construit la commande test version de la machine demandee */
         if ( ex_presence_machine(pa_NomMachine) ==  XDC_OK)
         {
            sprintf(pl_Cmd,"cat %s/*LCR>%s/LCR_%s", XZEXC_PATH_DIFF_DEST, XZEXC_PATH_DIFF_DEST, pa_NomMachine );
            if ((pl_P = popen(pl_Cmd,"r")) != (FILE *)NULL)
               pclose(pl_P) ;
            else
               XZST_03EcritureTrace(XZSTC_WARNING, "Erreur execution de %s",pl_Cmd);
            return (XDC_NOK);
         }
         else return (XDC_OK);
      }

      /* construit la commande test version de la machine demandee */
      sprintf(pl_Cmd,"cat %s/*LCR>%s/test", XZEXC_PATH_DIFF_DEST, XZEXC_PATH_DIFF_DEST );
      if ((pl_P = popen(pl_Cmd,"r")) != (FILE *)NULL)
         pclose(pl_P) ;
      else
         XZST_03EcritureTrace(XZSTC_WARNING, "Erreur execution de %s",pl_Cmd);

      /* construit la commande test version de la machine demandee */
      sprintf(pl_Cmd,"diff %s/LCR_%s %s/test | wc -l", XZEXC_PATH_DIFF_DEST, pa_NomMachine, XZEXC_PATH_DIFF_DEST );
      /* execute la commande, */
      if ((pl_P = popen(pl_Cmd,"r")) != (FILE *)NULL)
      {
         /* recupere le resultat de cette commande */
         fgets(pl_ResCmd,sizeof(pl_ResCmd)-1,pl_P) ;

         if (atoi(pl_ResCmd) == 0)
         {
            pclose(pl_P) ;
            return (XDC_OK);
         }
         else
         {
            if ( ex_presence_machine(pa_NomMachine) ==  XDC_NOK) {
	       pclose(pl_P) ;
               return (XDC_OK);
	       }
               
            sprintf ( pl_Cmd, "%s/LCR_%s", XZEXC_PATH_DIFF_DEST, pa_NomMachine );
            sprintf ( pl_ResCmd, "%s/test", XZEXC_PATH_DIFF_DEST );
            unlink ( pl_Cmd );
            rename ( pl_ResCmd, pl_Cmd );
            pclose(pl_P) ;
            return (XDC_NOK);
         }
      }
      else
         XZST_03EcritureTrace(XZSTC_WARNING, "Erreur execution de %s",pl_Cmd);

      if ( ex_presence_machine(pa_NomMachine) ==  XDC_NOK)
         return (XDC_OK);
      else
         return (XDC_NOK);

}       






/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ei_diffusion_fichier_lcr ( void )

/*
* ARGUMENTS EN ENTREE :
*   aucun
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   Diffussion des fichiers de configuration vers les machines concernees
*
------------------------------------------------------*/
{
XDY_NomMachine	pl_NomMachine;
char 		pl_NomFichierSrce[XDC_PATH_ABS_SIZE];
int		vl_indice = 0;
EINI_DONNEES_FEN_LCR 	*pl_EqtLCR;

   /* Recuperation du nom de la machine */
   if ( XZSC_07NomMachine(pl_NomMachine) != XDC_OK )
   {
      /* Ecriture Trace */
      XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom de la machine impossible . " );
   } 
   else
   {
      
		/*A Recherche de la position d'insertion dans la liste */
      for ( pl_EqtLCR = pg_DebMacLCR; pl_EqtLCR != NULL; pl_EqtLCR = pl_EqtLCR->Suivant )
      {
		
         /* Recherche si la machine cible est accessible */
         if ( ei_test_version(pl_EqtLCR->NomEqt) != XDC_OK )
         {
               /* Recopie du fichier de config vers la machine concernee */
                   if ( XZSS_07TransfertFichier (    pl_NomMachine,
               				   "*_LCR",
               				   XZEXC_PATH_DIFF_DEST,
               				   pl_EqtLCR->NomEqt,
               				   "/",
               				   XZEXC_PATH_DIFF_DEST ) != XDC_OK )
                   {
                       XZST_03EcritureTrace( XZSTC_WARNING, "Transfert fichier LCR de %s vers %s impossible",
               					   pl_NomMachine,
               					   pl_EqtLCR->NomEqt );
                   } 
                   else
                   {
                       XZST_03EcritureTrace( XZSTC_INFO, "Transfert fichier LCR de %s vers %s OK",
               					   pl_NomMachine,
                  			           pl_EqtLCR->NomEqt );
                   } 
             XZST_03EcritureTrace( XZSTC_WARNING, "Transfert des fichiers LCR vers %s termine . ", pl_EqtLCR->NomEqt );
          }
       }
    }
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ei_diffusion_fichier_standard ( void )

/*
* ARGUMENTS EN ENTREE :
*   aucun
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   Recuperation des fichiers standard de configuration 
*
------------------------------------------------------*/
{
char 		pl_NomFichier[XDC_PATH_ABS_SIZE] = "";
XZSCT_NomSite    vl_NomSite="DS";

   if ( XZSC_04NomSite(vl_NomSite) != XDC_OK )
   {
      /* Ecriture Trace */
      XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom du Site impossible . " );
   } 
   /* Recopie du fichier XDF_ParamLS */
   /* si on est sur le site de DN ou DM alors on prend le fichier specific zone Est DEM537*/
   if (!strcmp(vl_NomSite,XDC_NOM_SITE_VC))
   {
	sprintf(pl_NomFichier,"%s_Est",XDF_ParamLS );
	XZST_03EcritureTrace( XZSTC_WARNING, "Site %s : Prise en compte du fichier %s.",vl_NomSite,pl_NomFichier);
        ei_recopie_fichier_standard_LS (pl_NomFichier,XDF_ParamLS);
   }
   else
   	ei_recopie_fichier_standard ( XDF_ParamLS );

   /* Recopie du fichier XDF_Config_Cligno */
   ei_recopie_fichier_standard ( XDF_Config_Cligno );
   /* Recopie du fichier XDF_Config_IDPASSW */
   ei_recopie_fichier_standard ( XDF_Config_IDPASSW );
   /* Recopie du fichier XDF_Config_Scenar */
   ei_recopie_fichier_standard ( XDF_Config_Scenar );
   /* Recopie du fichier XDF_Texte_Alerte */
   ei_recopie_fichier_standard ( XDF_Texte_Alerte );
   /* Recopie du fichier XDF_CONF_ALARME */
   ei_recopie_fichier_standard ( XDF_CONF_ALARME );
   /* Recopie du fichier XDF_Param_Status */
   ei_recopie_fichier_standard ( XDF_Param_Status );
   /* Recopie du fichier XDF_Affichage_Def_PMV */
   ei_recopie_fichier_standard ( XDF_Affichage_Def_PMV );
   /* Recopie du fichier XDF_Config_Cstes_AUTOTEL */
   ei_recopie_fichier_standard ( XDF_Config_Cstes_AUTOTEL );
   /* Recopie du fichier XDF_CONF_ALARME_ALERTE pour equipement generique LCR */
   sprintf ( pl_NomFichier, "%s_*\0", XDF_CONF_ALARME_ALERTE );
   ei_recopie_fichier_standard ( pl_NomFichier );
   /* Recopie du fichier XDF_CONF_ALERTE_DAI_TUNNEL pour alerte DAI Tunnel */
   sprintf ( pl_NomFichier, "%s_*\0", XDF_CONF_ALERTE_DAI_TUNNEL );
   ei_recopie_fichier_standard ( pl_NomFichier );
   /* Recopie du fichier XDF_CONF_ALERTE_PAU_HS pour alerte PAU HS */
   ei_recopie_fichier_standard ( XDF_CONF_ALERTE_PAU_HS );
   /* Recopie du fichier XDF_CONF_RADT_VIRTUELLE pour calcul stations RADT vituelle */
   ei_recopie_fichier_standard ( XDF_CONF_RADT_VIRTUELLE );
   /* Recopie du fichier XDF_CONF_PARAMS_TERDT pour calcul des equipements RDT  */
   ei_recopie_fichier_standard ( XDF_CONF_PARAMS_TERDT );
   /* Recopie du fichier XDF_CONF_ZONE_UGTP pour config zone UGTP */
   ei_recopie_fichier_standard ( XDF_CONF_ZONE_UGTP );
   /* Recopie du fichier XDF_PARAM_UGTP pour parametre UGTP */
   ei_recopie_fichier_standard ( XDF_PARAM_UGTP );
   /* Recopie du fichier XDF_Affichage_Def_PMVA*/
   ei_recopie_fichier_standard ( XDF_Affichage_Def_PMVA);
   /* Recopie du fichier XDF_Affichage_Def_PRV*/
   ei_recopie_fichier_standard ( XDF_Affichage_Def_PRV);
   /* AAZ 17/10/2007 */
   /* Recopie du fichier XDF_CONF_ALERTE */
   ei_recopie_fichier_standard ( XDF_CONF_ALERTE );
   /* AAZ */
   ei_recopie_fichier_standard (XDF_ST40);
}






/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ei_recopie_fichier_type ( 	XDY_NomMachine	pa_NomMachine,
					XDY_NomMachine	pa_NomMachineFic,
					char *		pa_NomFic )

/*
* ARGUMENTS EN ENTREE :
*   XDY_NomMachine	pa_NomMachine	nom machine destinatrice
*   char *		pa_NomFic 	nom du fichier a recopier
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   Diffussion des fichiers de configuration vers les machines concernees
*
------------------------------------------------------*/
{
XDY_NomMachine	pl_NomMachine;
char 		pl_NomFichier[XDC_PATH_ABS_SIZE];

   /* Recuperation du nom de la machine */
   if ( XZSC_07NomMachine(pl_NomMachine) != XDC_OK)
   {
      /* Ecriture Trace */
      XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom de la machine impossible . " );
   } 
   else
   {
      /* Recopie du fichier de config vers la machine concernee */
      if ( strlen(pa_NomFic) > 0 )
      {
         sprintf ( pl_NomFichier, "%s_%s", 
               		          pa_NomFic,
               		          pa_NomMachineFic );
         if ( XZSS_07TransfertFichier (    pl_NomMachine,
               				   pa_NomFic,
               				   XZEXC_PATH_DIFF_DEST,
               				   pa_NomMachine,
               				   pl_NomFichier,
               				   XZEXC_PATH_DIFF_DEST ) != XDC_OK )
         {
             XZST_03EcritureTrace( XZSTC_WARNING, "Transfert fichier %s de %s vers %s impossible",
               					   pa_NomFic,
               					   pl_NomMachine,
               					   pa_NomMachine );
         } 
         else
         {
             XZST_03EcritureTrace( XZSTC_INFO, "Transfert fichier %s de %s vers %s OK",
               					   pa_NomFic,
               					   pl_NomMachine,
               					   pa_NomMachine );
         } 
      }
    }
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ei_recopie_fichier_commun ( 	XDY_NomMachine	pa_NomMachine,
					char *		pa_NomFic )

/*
* ARGUMENTS EN ENTREE :
*   XDY_NomMachine	pa_NomMachine	nom machine destinatrice
*   char *		pa_NomFic 	nom du fichier a recopier
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   Diffussion des fichiers de configuration vers les machines concernees
*
------------------------------------------------------*/
{
XDY_NomMachine	pl_NomMachine;

   /* Recuperation du nom de la machine */
   if ( XZSC_07NomMachine(pl_NomMachine) != XDC_OK)
   {
      /* Ecriture Trace */
      XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom de la machine impossible . " );
   } 
   else
   {

      /* Recopie du fichier de config vers la machine concernee */
      if ( !strcmp ( pl_NomMachine, pa_NomMachine ) )
      {
         sleep ( 2 );
      }
      else
      {
         if ( strlen(pa_NomFic) > 0 )
         {
            if ( XZSS_07TransfertFichier ( pl_NomMachine,
               			           pa_NomFic,
               				   XZEXC_PATH_DIFF_DEST,
               				   pa_NomMachine,
               				   "/",
               				   XZEXC_PATH_DIFF_DEST ) != XDC_OK )
            {
                XZST_03EcritureTrace( XZSTC_WARNING, "Transfert fichier %s de %s vers %s impossible",
               					      pa_NomFic, pl_NomMachine, pa_NomMachine );
            } 
            else
            {
                XZST_03EcritureTrace( XZSTC_INFO, "Transfert fichier %s de %s vers %s OK",
                  			           pa_NomFic, pl_NomMachine, pa_NomMachine );
            } 
         }
      }
    }
}


/* definition de fonctions externes */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	ec_diffusion_fichier ( 	XDY_NomMachine	pa_NomMachine,
				int		va_TypeMachine,
				int		va_TypeEqt )

/*
* ARGUMENTS EN ENTREE :
*   XDY_NomMachine	pa_NomMachine	nom machine a configurer
*   int			va_TypeMachine	Type de la machine (principale ou supleante)
*   int			va_TypeEqt 	type d'equipement a configurer
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   Recopie d'un fichier de config
*
------------------------------------------------------*/
{
EINI_MACHINE *	pl_Machine;
EINI_MACHINE *	pl_MachineSur;
XDY_NomMachine	vl_NomMachine;
XDY_NomMachine	vl_NomMachineDiff;
XDY_Datagroup	pl_NomDG;
int		vl_IndexFd = 0,
		vl_indice = 0,
		vl_MemLivCom = XDC_FAUX;
char            pl_PathEtFic[XDC_PATH_ABS_SIZE];
XDY_Texte       pl_TexteConf;
int             vl_Fd = 0;
XDY_NomMachine	pl_NomServeur;

   /* Recuperation du nom de la machine */
   if ( XZSC_07NomMachine(pl_NomServeur) != XDC_OK)
   {
      /* Ecriture Trace */
      XZST_03EcritureTrace( XZSTC_WARNING, "Recuperation du Nom de la machine impossible . " );
   } 

   XZST_03EcritureTrace( XZSTC_FONCTION, " IN  : ec_diffusion_fichier");
   /* Creation du nouveau fichier de XDF_Nom_Serveur */
   /* Construction du nom du fichier presence equipement */
   sprintf ( pl_PathEtFic, "%s/%s", XZEXC_PATH_DIFFUSION, XDF_Nom_Serveur );

   /* Ouverture du fichier presence equipement apres avoir detruit l'ancien*/
   XZSS_06DetruireFichier ( pl_PathEtFic );
   if ( (vl_Fd = open ( pl_PathEtFic, O_RDWR | O_CREAT , S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) ) >= 0 )
   {
       sprintf ( pl_TexteConf, "#Nom du serveur \n%s\t\n", pl_NomServeur );
       write ( vl_Fd, pl_TexteConf, strlen (pl_TexteConf) );
       /* Fermeture du fichier XDF_Nom_Serveur */
       close ( vl_Fd );
   }

//XZST_03EcritureTrace( XZSTC_WARNING, "ec_diffusion_fichier : pa_NomMachine = %s va_TypeMachine = %s",pa_NomMachine,va_TypeMachine==XZECC_MACHINE_PRINC?"XZECC_MACHINE_PRINC":"NON XZECC_MACHINE_PRINC");
   /* Recherche de la machine concerne */
   for ( pl_Machine = pg_ListeMachine; pl_Machine != NULL; pl_Machine = pl_Machine->Suivant )
   {
//XZST_03EcritureTrace( XZSTC_WARNING, "ec_diffusion_fichier : pl_Machine->NomMachine = [%s] pl_Machine->NomMachineSup = [%s]",pl_Machine->NomMachine,pl_Machine->NomMachineSup);
      /* S'il s'agit de la machine concernee */
      if ( (!strcmp (pl_Machine->NomMachine,    pa_NomMachine) && (va_TypeMachine == XZECC_MACHINE_PRINC)) ||
           (!strcmp (pl_Machine->NomMachineSup, pa_NomMachine) && (va_TypeMachine != XZECC_MACHINE_PRINC)) ||
           (!strcmp (pa_NomMachine, XZECC_TOUTES_MACHINES)) )
      {
         XZST_03EcritureTrace( XZSTC_WARNING, "ec_diffusion_fichier : va_TypeMachine = %d",va_TypeMachine);
         vl_MemLivCom = XDC_FAUX;
         if ( va_TypeMachine == XZECC_MACHINE_PRINC )
         {
            strcpy ( vl_NomMachineDiff, pl_Machine->NomMachine );
         }
         else
         {
            strcpy ( vl_NomMachineDiff, pa_NomMachine );
            for ( pl_MachineSur = pg_ListeMachine; pl_MachineSur != NULL; pl_MachineSur = pl_MachineSur->Suivant )
            {
               if ( !strcmp (pl_MachineSur->NomMachine,  pa_NomMachine) ) break;
            }
         }			
         
         XZST_03EcritureTrace( XZSTC_WARNING, "ec_diffusion_fichier : va_TypeEqt = %d",va_TypeEqt);
         /* Recherche de l'equipement concerne */
         for ( vl_IndexFd = 0, vl_indice = 0; vl_indice < pg_SizeTabConf; vl_indice++)
         {
            /* S'il s'agit de l'equipement concerne */
            if ( ( ( pg_TabConf[vl_indice].TypeEqt == va_TypeEqt ) || ( va_TypeEqt == XZECC_TYPE_EQT_ALL ) || 
                     ((va_TypeEqt == XDC_EQT_PMV) && (pg_TabConf[vl_indice].TypeEqt == XDC_EQT_PCT))) && 
                   ( ((va_TypeMachine == XZECC_MACHINE_PRINC) && (pl_Machine->Eqt[ec_rang_eqt(pg_TabConf[vl_indice].TypeEqt)].Present)) ||
                     ((va_TypeMachine != XZECC_MACHINE_PRINC) && (pl_MachineSur->Eqt[ec_rang_eqt(pg_TabConf[vl_indice].TypeEqt)].Present)) ) )
            {
               if ( ex_presence_machine(pl_Machine->NomMachine) !=  XDC_OK )
               {
                  XZST_03EcritureTrace(XZSTC_WARNING, "Impossible de diffuser vers %s.", pl_Machine->NomMachine );
                  continue;
               }  
               if ( vl_MemLivCom == XDC_FAUX )
               {
//XZST_03EcritureTrace( XZSTC_WARNING, "ec_diffusion_fichier : 1");
                   vl_MemLivCom = XDC_VRAI;
                   if ( va_TypeMachine == XZECC_MACHINE_PRINC )
                   {
                      /* Diffusion du fichier texte alerte */
                      ei_recopie_fichier_commun ( pl_Machine->NomMachine, XDF_Texte_Alerte );
                      /* Diffusion du fichier de config des alarmes */
                      ei_recopie_fichier_commun ( pl_Machine->NomMachine, XDF_CONF_ALARME );
                      /* Diffusion du fichier parametre status */
                      ei_recopie_fichier_commun ( pl_Machine->NomMachine, XDF_Param_Status );
                      /* Diffusion du fichier config scenario */
                      ei_recopie_fichier_commun ( pl_Machine->NomMachine, XDF_ScenNAV_Connu );
                      /* Diffusion du fichier login LT */
                      ei_recopie_fichier_commun ( pl_Machine->NomMachine, XDF_Config_Utilisateurs );
                      /* Diffusion du fichier nom serveur */
                      ei_recopie_fichier_commun ( pl_Machine->NomMachine, XDF_Nom_Serveur );
                      /* AAZ 17/10/2007 */
                      ei_recopie_fichier_commun ( pl_Machine->NomMachine, XDF_CONF_ALERTE );
                      /* AAZ */
                      strcpy ( vl_NomMachineDiff, pl_Machine->NomMachine );
                   }
//XZST_03EcritureTrace( XZSTC_WARNING, "ec_diffusion_fichier : 2");
                   
                   if ( va_TypeEqt != XZECC_TYPE_EQT_ALL )
                   {
//XZST_03EcritureTrace( XZSTC_WARNING, "ec_diffusion_fichier : 2-1");
                      /* Diffusion du fichier de config presence equipement vers la machine concernee */
                      ei_recopie_fichier ( pl_Machine->NomMachine, vl_NomMachineDiff, XDF_Presence_Equip );
         
                      /* Diffusion du fichier de config ligne vers la machine concernee */
                      ei_recopie_fichier ( pl_Machine->NomMachine, vl_NomMachineDiff, XDF_Config_Lignes );
         
                      /* Diffusion du fichier de config nom machine supleante concernee */
                      ei_recopie_fichier ( pl_Machine->NomMachine, vl_NomMachineDiff, XDF_Machine_Sup );
                   }
                   else
                   {
//XZST_03EcritureTrace( XZSTC_WARNING, "ec_diffusion_fichier : 2-2");
                      sprintf ( pl_PathEtFic, "*_%s\0", vl_NomMachineDiff );
                      ei_recopie_fichier_commun ( pl_Machine->NomMachine, pl_PathEtFic );
                   }
         
                   /* Diffusion du fichier de password */
                   ei_recopie_fichier_type ( pl_Machine->NomMachine, vl_NomMachineDiff, XDF_Config_IDPASSW );
         
                   /* Diffusion du fichier clignottement */
                   ei_recopie_fichier_type ( pl_Machine->NomMachine, vl_NomMachineDiff, XDF_Config_Cligno );
               }
//XZST_03EcritureTrace( XZSTC_WARNING, "ec_diffusion_fichier : 3");
               
               if ( va_TypeEqt != XZECC_TYPE_EQT_ALL )
               {
//XZST_03EcritureTrace( XZSTC_WARNING, "ec_diffusion_fichier : 3-1");
                  /* Diffusion du 1er fichier de config vers la machine concernee */
                  ei_recopie_fichier ( pl_Machine->NomMachine, vl_NomMachineDiff, pg_TabConf[vl_indice].NomFichier1 );
                
                  /* Diffusion du 2eme fichier de config vers la machine concernee */
                  ei_recopie_fichier ( pl_Machine->NomMachine, vl_NomMachineDiff, pg_TabConf[vl_indice].NomFichier2 );
               
                  /* Diffusion du 3eme fichier de config vers la machine concernee */
                  ei_recopie_fichier ( pl_Machine->NomMachine, vl_NomMachineDiff, pg_TabConf[vl_indice].NomFichier3 );

                  /* Diffusion du 4eme fichier de config vers la machine concernee */
                  ei_recopie_fichier ( pl_Machine->NomMachine, vl_NomMachineDiff, pg_TabConf[vl_indice].NomFichier4 );

                  /* Diffusion du 5eme fichier de config vers la machine concernee */
                  ei_recopie_fichier ( pl_Machine->NomMachine, vl_NomMachineDiff, pg_TabConf[vl_indice].NomFichier5 );

                  /* Diffusion du 6eme fichier de config vers la machine concernee */
                  ei_recopie_fichier ( pl_Machine->NomMachine, vl_NomMachineDiff, pg_TabConf[vl_indice].NomFichier6 );
               }

               /* Diffussion des fichiers de type d'equipement */
               switch ( pg_TabConf[vl_indice].TypeEqt )
               {
                  case XZECC_TYPE_EQT_PMV : 
//XZST_03EcritureTrace( XZSTC_WARNING, "ec_diffusion_fichier : 4- PMV");
                     /* Diffusion du fichier de config Type PMV vers la machine concernee */
                     ei_recopie_fichier_type ( pl_Machine->NomMachine, vl_NomMachineDiff, XDF_Config_Type_PMV );
                     ei_recopie_fichier_commun ( pl_Machine->NomMachine, XDF_Affichage_Def_PMV );
                     break;
                     
                  case XZECC_TYPE_EQT_PICTO : 
//XZST_03EcritureTrace( XZSTC_WARNING, "ec_diffusion_fichier : 4- PICTO");
                     /* Diffusion du fichier de config Type PICTO vers la machine concernee */
                     ei_recopie_fichier_type ( pl_Machine->NomMachine, vl_NomMachineDiff,  XDF_Config_Type_PICTO );
                     break;
                     
                  case XZECC_TYPE_EQT_NAV : 
//XZST_03EcritureTrace( XZSTC_WARNING, "ec_diffusion_fichier : 4- NAV");
                     /* Diffusion du fichier de config Type NAV vers la machine concernee */
                     ei_recopie_fichier_type ( pl_Machine->NomMachine, vl_NomMachineDiff, XDF_Config_Type_NAV );
                     /* Diffusion du fichier de config Scenario  vers la machine concernee */
                     ei_recopie_fichier_commun ( pl_Machine->NomMachine, XDF_Config_Scenar );
                     break;

                  case XZECC_TYPE_EQT_TEL : 
//XZST_03EcritureTrace( XZSTC_WARNING, "ec_diffusion_fichier : 4- TEL");
                     /* Diffusion du fichier de constantes AUTOTEL  vers la machine concernee */
                     ei_recopie_fichier_type ( pl_Machine->NomMachine, vl_NomMachineDiff, XDF_Config_Cstes_AUTOTEL );
                     break;
                     
                  case XDC_EQT_GEN_LCR : 
//XZST_03EcritureTrace( XZSTC_WARNING, "ec_diffusion_fichier : 4- GEN_LCR");
                     /* Diffusion du fichier de config alarme et alerte eqt LCR XDF_CONF_ALARME_ALERTE vers la machine concernee */
                     sprintf ( pl_PathEtFic, "%s_*\0", XDF_CONF_ALARME_ALERTE );
                     ei_recopie_fichier_commun ( pl_Machine->NomMachine, pl_PathEtFic );
                     break;

                  case XDC_EQT_TDP : 
//XZST_03EcritureTrace( XZSTC_WARNING, "ec_diffusion_fichier : 4- TDP");
   		     /* Diffusion du fichier XDF_CONF_ZONE_UGTP pour config zone UGTP vers la machine concernee */
                     ei_recopie_fichier_commun ( pl_Machine->NomMachine, XDF_CONF_ZONE_UGTP );
   		     /* Diffusion du fichier XDF_PARAM_UGTP pour parametre UGTP vers la machine concernee */
                     ei_recopie_fichier_commun ( pl_Machine->NomMachine, XDF_PARAM_UGTP );
                     break;

                  case XZECC_TYPE_EQT_RAU : 
                     /* Diffusion du fichier de config des alertes PAU HS vers la machine concernee */
//XZST_03EcritureTrace( XZSTC_WARNING, "ec_diffusion_fichier : 4- RAU");
                     ei_recopie_fichier_commun ( pl_Machine->NomMachine, XDF_CONF_ALERTE_PAU_HS );
                     break;

                  case XZECC_TYPE_EQT_RDT : 
                     /* Diffusion du fichier de config des station RADT virtuelles vers la machine concernee */
//XZST_03EcritureTrace( XZSTC_WARNING, "ec_diffusion_fichier : 4- RDT");
                     ei_recopie_fichier_commun ( pl_Machine->NomMachine, XDF_CONF_RADT_VIRTUELLE );
					 /* Diffusion du fichier de config des equipements RDT vers la machine concernee */
					 ei_recopie_fichier_commun ( pl_Machine->NomMachine, XDF_CONF_PARAMS_TERDT );
                     break;

                  case XZECC_TYPE_EQT_DAI : 
                     /* Diffusion du fichier de config des alertes DAI Tunnel vers la machine concernee */
//XZST_03EcritureTrace( XZSTC_WARNING, "ec_diffusion_fichier : 4- DAI");
                     sprintf ( pl_PathEtFic, "%s_%s\0", XDF_CONF_ALERTE_DAI_TUNNEL, vl_NomMachineDiff );
                     ei_recopie_fichier_commun ( pl_Machine->NomMachine, pl_PathEtFic );
                     break;
                     
               }  /* fin switch fichier type d'equipement */
                     
            }  /* fin si equipt concerne */
         } 
         
         
         
         if ( va_TypeMachine == XZECC_MACHINE_PRINC )
         {
            /* Emission du message XDM_EFIN_CONFIG sur le DataGroup XDG_EFIN_CONFIG_<NomMachine> */

            XDG_EncodeDG2 ( pl_NomDG, XDG_EFIN_CONFIG, pl_Machine->NomMachine );			
            if (!TipcSrvMsgWrite (  pl_NomDG,
      	      	      	            XDM_IdentMsg(XDM_EFIN_CONFIG),
                          	    XDM_FLG_SRVMSGWRITE,
               			    T_IPC_FT_INT4, va_TypeEqt,
               			    T_IPC_FT_INT4, XDC_OK,
                          	    NULL))
           {
               XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_EFIN_CONFIG non effectue." );
           }
            else
               XZST_03EcritureTrace( XZSTC_INFO, "ec_diffusion_fichier : Envoi message XDM_EFIN_CONFIG effectue.");
        }
        else
        {
            /* Emission du message XDM_EFIN_CONFIG_SUR sur le DataGroup XDG_EFIN_CONFIG_SUR_<NomMachine> */

            XDG_EncodeDG2 ( pl_NomDG, XDG_EFIN_CONFIG_SUR, pa_NomMachine );			
            if (!TipcSrvMsgWrite (  pl_NomDG,
      	      	      	            XDM_IdentMsg(XDM_EFIN_CONFIG_SUR),
                          	    XDM_FLG_SRVMSGWRITE,
               			    T_IPC_FT_INT4, va_TypeEqt,
               			    T_IPC_FT_INT4, XDC_OK,
                          	    NULL))
           {
               XZST_03EcritureTrace(XZSTC_WARNING, "Envoi message XDM_EFIN_CONFIG_SUR non effectue." );
           }
            else
               XZST_03EcritureTrace( XZSTC_INFO, "ec_diffusion_fichier : Envoi message XDM_EFIN_CONFIG_SUR effectue.");

        }
           

      }  /* fin si machine concernee */
   }

}


