/*E*/
/*  Fichier : $Id: eini_nav.c,v 1.8 2019/01/29 11:07:35 pc2dpdy Exp $      Release : $Revision: 1.8 $        Date : $Date: 2019/01/29 11:07:35 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEINI * FICHIER eini_nav.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module de creation des fichiers de config pour la tache NAV >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	26 Sep 1994	: Creation
* Mismer.D	version 1.2	14 Dec 1994	:
* Mismer.D	version 1.3	23 Jan 1995	:
* Mismer.D	version 1.4	25 Jan 1995	:
* Fontaine.C	version 1.5	19 Mai 1995	: Reprise entete, historique
* Mismer.D	version 1.6	20 Jan 1998	: Ajout type FAU et FAC (DEM/1544)
* JMG		version 1.7	22/11/07	: ajout site de gestion DEM/715
*  JMG   07/11/18 : ajout vitesse dans ec_mise_a_jour_config_ligne 1.8
------------------------------------------------------*/

/* fichiers inclus */

#include "eini_cfg.h"

/* definitions de constantes */

/* definitions de types locaux */


/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: eini_nav.c,v 1.8 2019/01/29 11:07:35 pc2dpdy Exp $ : eini_nav" ;
  
/* declaration de fonctions internes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_config_NAV (	XZAOT_NAV va_NAV, int va_NumAppel )

/*
* ARGUMENTS EN ENTREE :
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
*   Ecrire les parametres dans le fichier de config NAV
*   Mettre a jour pour la machine concerne la config de la ligne
*
------------------------------------------------------*/
{
XDY_Texte	pl_TexteConf;
int		vl_ValRet = XDC_OK;

   if ( va_NumAppel == 1)
   {
      /* Ecriture de l'entete dans le fichier de config */
      sprintf ( pl_TexteConf, 	"# NumNAV\tServeur\tLiaison\tRGS\tSous Type\tType\tSiteGestion\n\0" );
      vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_NAV );
   }
   
   sprintf ( pl_TexteConf, 	"N %d\t%s\t%d\t%s\t%d\t%d\t%d\n\0",
   				va_NAV.NumeroNAV,
   				va_NAV.NomServeur,
   				va_NAV.Liaison,
   				va_NAV.Rgs,
   				va_NAV.NumeroType,
   				va_NAV.Type,
				va_NAV.SiteGestion);              
   XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ec_config_NAV %s", pl_TexteConf );
   /* Ecriture dans le fichier de config */
   vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_NAV );
   
   /* mise a jour des liaisons series de la machine */
   ec_mise_a_jour_config_ligne ( va_NAV.Liaison,
                      		 va_NAV.NomServeur,
                      		 XZECC_PROT_LCR,
                      		 XZECC_SENS_MAITRE,
                      		 XZECC_TYPE_EQT_NAV,
                      		 va_NAV.NumeroNAV ,0);
                      		 
   XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ec_config_NAV " );
   return ( vl_ValRet );
   
}  

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_config_Module (	XZAOT_Module va_Module, int va_NumAppel )

/*
* ARGUMENTS EN ENTREE :
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
*   Ecrire les parametres dans le fichier de config Module NAV
*
------------------------------------------------------*/
{
XDY_Texte	pl_TexteConf;
int		vl_ValRet = XDC_OK;

   if ( va_NumAppel == 1)
   {
      /* Ecriture de l'entete dans le fichier de config */
      sprintf ( pl_TexteConf, 	"# NumModule\tCode\t\n\0" );
      vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_NAV );
   }
   
   sprintf ( pl_TexteConf, 	"M \t%d\t%c.%c\t\n\0",
   				va_Module.NumeroMod,
   				va_Module.Code[0],
   				va_Module.Code[1] );
   XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ec_config_Module %s", pl_TexteConf );
   /* Ecriture dans le fichier de config */
   vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_NAV );
                      		 
   XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ec_config_Module " );
   return ( vl_ValRet );
}  


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_fichier_type_nav (	XZAOT_TypeNAV va_TypeNAV, int va_NumAppel )

/*
* ARGUMENTS EN ENTREE :
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
*   Ecrire les parametres dans le fichier de config type NAV 
*
------------------------------------------------------*/
{
XDY_Texte	pl_TexteConf;
int		vl_ValRet = XDC_OK;
      	
   if ( va_NumAppel == 1)
   {
      /* Ecriture de l'entete dans le fichier de config */
      sprintf ( pl_TexteConf, 	"#\tNumType\tFlash\tNbBarriere\t\n\0" );
      vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_Type_NAV );
   }
   
   sprintf ( pl_TexteConf, 	"T %d\t%d\t%d\t\n\0",
   				va_TypeNAV.NumeroType,
   				va_TypeNAV.FlashMod4,
   				va_TypeNAV.NbBarrieres );
   XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ei_fichier_type_nav %s", pl_TexteConf );
   /* Ecriture dans le fichier de config */
   vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_Type_NAV );
                         		 
   XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ei_fichier_type_nav " );
   return ( vl_ValRet );
   
}  


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_fichier_scen_nav (	XZAOT_ScenConnu va_Scen, int va_NumAppel )

/*
* ARGUMENTS EN ENTREE :
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
*   Ecrire les parametres dans le fichier de config Scenario
*
------------------------------------------------------*/
{
XDY_Texte	pl_TexteConf;
int		vl_ValRet = XDC_OK;
      	
   if ( va_NumAppel == 1)
   {
      /* Ecriture de l'entete dans le fichier de config */
      sprintf ( pl_TexteConf, 	"#\tNumScen\tFichier Scenario\t\n\0" );
      vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_Type_NAV );
   }
   
   sprintf ( pl_TexteConf, 	"S %d\t%d\t%s\t\n\0",
   				va_Scen.NumeroScen,
   				va_Scen.AutoFichier,
   				va_Scen.Type );
   XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ei_fichier_scen_nav %s", pl_TexteConf );
   /* Ecriture dans le fichier de config */
   vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_Type_NAV );
                         		 
   XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ei_fichier_scen_nav " );
   return ( vl_ValRet );
   
}  

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_config_eqt_nav ( 	EINI_MACHINE * 	pa_Machine )

/*
* ARGUMENTS EN ENTREE :
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
*   Ecrire les parametres dans le fichier de config NAV
*   Mettre a jour pour la machine concerne la presence equipement
*
------------------------------------------------------*/
{
XDY_Texte	pl_TexteConf;
int		vl_ValRet = XDC_OK;
XDY_Entier	vl_NbEqt  = 0;

    /* Lecture en base de donnees des paramatres de configuration */
    vl_ValRet = XZAO163_Liste_NAV_Machine ( 	EINI_NOM_BASE,
    						pa_Machine->NomMachine,
    						ei_config_NAV, 
    						ei_config_Module,
    						&vl_NbEqt );
    /* Si lecture en base ok */
    if ( ( vl_ValRet == XDC_OK ) && ( vl_NbEqt != 0 ) )
    {
       /* Alors memorisation existance NAV sur la machine */
       strcpy ( pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_NAV)].Equipement, XZECC_TACHE_NAV );
       pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_NAV)].Present = XZEXC_EQT_PRESENT;
       return ( XDC_OK );
    }
    else
    {
       /* Sinon memorisation absence NAV sur la machine */
       strcpy ( pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_NAV)].Equipement, XZECC_TACHE_NAV );
       pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_NAV)].Present = XZEXC_EQT_ABSENT;
       return ( XDC_NOK );
    }
                   
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_config_type_nav (  )

/*
* ARGUMENTS EN ENTREE :
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
*   Ecrire les parametres dans le fichier de config type NAV
*
------------------------------------------------------*/
{
int		vl_ValRet = XDC_OK;
XDY_Entier	vl_NbType  = 0;
char 		pl_PathEtFic[XDC_PATH_ABS_SIZE];

   XZST_03EcritureTrace(XZSTC_FONCTION, " IN  : ei_config_type_nav ");
		  	       
   /* Construction du nom du fichier XDF_Config_Type_NAV */
   sprintf ( pl_PathEtFic, "%s/%s", XZEXC_PATH_DIFFUSION, XDF_Config_Type_NAV );
      
   /* Ouverture du fichier XDF_Config_Type_NAV apres avoir detruit l'ancien*/
   XZSS_06DetruireFichier ( pl_PathEtFic );
   if ( (vg_FdConf[0] = open ( pl_PathEtFic, O_RDWR | O_CREAT , S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) ) < 0 )
   {
      XZST_03EcritureTrace( XZSTC_WARNING, "Impossible d'ouvrir le fichier %s", pl_PathEtFic );
   }

    /* Lecture en base de donnees des paramatres de configuration XZAO164_Liste_Types_NAV */
    vl_ValRet = XZAO164_Liste_Types_NAV ( 	EINI_NOM_BASE,
    						ei_fichier_type_nav, 
    						ei_fichier_scen_nav, 
    						&vl_NbType );
   /* Fermeture du fichier XDF_Config_Type_NAV */
   close ( vg_FdConf[0] );
   
   /* Si retour lecture en base OK retourne OK sinon NOK */
   if ( ( vl_ValRet == XDC_OK ) && ( vl_NbType != 0 ) )
   {
      XZST_03EcritureTrace(XZSTC_FONCTION, " OUT : ei_config_type_nav return OK");
      return ( XDC_OK );
   }
   else
   {
      XZST_03EcritureTrace(XZSTC_FONCTION, " OUT : ei_config_type_nav return NOK");
      return ( XDC_NOK );
   }
                   
}
