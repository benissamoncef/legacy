/*E*/
/*  Fichier : $Id: eini_pmv.c,v 1.7 2019/01/07 13:42:23 pc2dpdy Exp $      Release : $Revision: 1.7 $        Date : $Date: 2019/01/07 13:42:23 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEINI * FICHIER eini_pmv.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module de creation des fichiers de config pour la tache PMV >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1 	26 Sep 1994	: Creation
* Mismer.D 	version 1.2 	14 Dec 1994 	:
* Mismer.D 	version 1.3 	23 Jan 1995 	:
* Mismer.D 	version 1.4 	25 Jan 1995 	:
* Fontaine.C 	version 1.5 	19 Mai 1995 	: Reprise entete, historique
* JMG   13/11/07        : ajout site de gestion DEM715
* JMG	02/08/19 : zjout picto type pmv suppression SAGA DEM1306 1.7
* GGY   15/06/23    :   Ajout gestion eqt IP (DEM472)
* GGY	02/08/23	:	Ajout TypePnc et FichierPnc a ei_config_type_picto (DEM473)
* GGY	06/12/23	:	Suppression ligne PICTO dans les fichiers Config_Lignes_* (DEM473)
------------------------------------------------------*/

/* fichiers inclus */

#include "eini_cfg.h"

/* definitions de constantes */

/* definitions de types locaux */


/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: eini_pmv.c,v 1.7 2019/01/07 13:42:23 pc2dpdy Exp $ : eini_pmv" ;
  
/* declaration de fonctions internes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_config_PMV (	XZAOT_PMV va_PMV, int va_NumAppel )

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
*   Ecrire les parametres dans le fichier de config Picto
*   Mettre a jour pour la machine concerne la config de la ligne
*
------------------------------------------------------*/
{
XDY_Texte	pl_TexteConf;
int		vl_ValRet = XDC_OK;
int vl_vitesse;

   if ( va_NumAppel == 1)
   {
      /* Ecriture de l'entete dans le fichier de config */
      sprintf ( pl_TexteConf, 	"#\tTypeEqt\tNumPMV\tServeur\tLiaison\tRGS\tType\tSiteGestion\tAdresseIP\tPort\tTypeLCR\tIdLCR\n\0" );
      vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_PMV );
   }
   
   sprintf ( pl_TexteConf, 	" P\t%d\t%s\t%d\t%s\t%d\t%d\t%s\t%d\t%d\t%s\n\0",
   				va_PMV.NumeroPMV,
   				va_PMV.NomServeur,
   				va_PMV.Liaison,
   				va_PMV.RGS,
   				va_PMV.NumeroTypePMV,
				va_PMV.SiteGestion,
				va_PMV.AdresseIP,
				va_PMV.Port,
				va_PMV.TypeLCR,
				va_PMV.IdLCR);
   XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ec_config_PMV %s", pl_TexteConf );
   /* Ecriture dans le fichier de config */
   vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_PMV );
   
   /* mise a jour des liaisons series de la machine */
   if (va_PMV.NumeroTypePMV!=1)
	vl_vitesse = 9600;
   else
	vl_vitesse = 1200;
   if (!strcmp(va_PMV.AdresseIP,XZECC_IP_VIDE))
     ec_mise_a_jour_config_ligne ( va_PMV.Liaison,
                      		 va_PMV.NomServeur,
                      		 XZECC_PROT_LCR,
                      		 XZECC_SENS_MAITRE,
                      		 XZECC_TYPE_EQT_PMV,
                      		 va_PMV.NumeroPMV, vl_vitesse );
   else
     ec_mise_a_jour_config_ligne ( va_PMV.Port,
     					va_PMV.AdresseIP,
					XZECC_PROT_IP,
					XZECC_SENS_MAITRE,
					XZECC_TYPE_EQT_PMV,
					va_PMV.NumeroPMV,vl_vitesse );

   XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ec_config_PMV " );
   return ( vl_ValRet );
   
}  

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_config_Picto (	XZAOT_Picto va_Picto, int va_NumAppel )

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
*   Ecrire les parametres dans le fichier de config Picto
*   Mettre a jour pour la machine concerne la config de la ligne serie
*
------------------------------------------------------*/
{
XDY_Texte	pl_TexteConf;
int		vl_ValRet = XDC_OK;

   if ( va_NumAppel == 1)
   {
      /* Ecriture de l'entete dans le fichier de config */
      sprintf ( pl_TexteConf, 	"#\tTypeEqt\tNumPicto\tServeur\tLiaison\tRGS\tType\tSiteGestion\tPort\tAdresseIP\n\0" );
      vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_PICTO );
   }
   
   sprintf ( pl_TexteConf, 	" I\t%d\t%s\t%d\t%s\t%d\t%d\t%d\t%s\t%d\t%s\n\0",
   				va_Picto.NumeroPicto,
   				va_Picto.NomServeur,
   				va_Picto.Liaison,
   				va_Picto.RGS,
   				va_Picto.NumeroTypePicto,
				va_Picto.SiteGestion,
				va_Picto.Port,
				va_Picto.AdresseIP,
				va_Picto.TypeLCR,
				va_Picto.IdLCR);
   XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ec_config_Picto %s", pl_TexteConf );
   /* Ecriture dans le fichier de config */
   vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_PICTO );
   
   /* mise a jour des liaisons series de la machine */
   if (!strcmp(va_Picto.AdresseIP,XZECC_IP_VIDE))
   	ec_mise_a_jour_config_ligne ( va_Picto.Liaison,
                      		 va_Picto.NomServeur,
                      		 XZECC_PROT_LCR,
                      		 XZECC_SENS_MAITRE,
                      		 XZECC_TYPE_EQT_PICTO,
                      		 va_Picto.NumeroPicto , 0);
//   else
//      ec_mise_a_jour_config_ligne ( va_Picto.Port,
//     					va_Picto.AdresseIP,
//					XZECC_PROT_IP,
//					XZECC_SENS_MAITRE,
//					XZECC_TYPE_EQT_PICTO,
//					va_Picto.NumeroPicto, 0);

   XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ec_config_Picto " );
   return ( vl_ValRet );
} 

 
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_fichier_type_pmv (	XZAOT_TypePMV va_TypePMV, int va_NumAppel )

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
*   Ecrire les parametres dans le fichier de config type PMV 
*
------------------------------------------------------*/
{
XDY_Texte	pl_TexteConf;
int		vl_ValRet = XDC_OK;
      	
   if ( va_NumAppel == 1)
   {
      /* Ecriture de l'entete dans le fichier de config */
      sprintf ( pl_TexteConf, 	"#\tNumType\tNBLigne\tNBCar\tFlash\tTemp\tPicto\t\n\0" );
      vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_Type_PMV );
   }
   
   sprintf ( pl_TexteConf, 	" %d\t%d\t%d\t%d\t%d\t%d\t\n\0",
   				va_TypePMV.NumType,
   				va_TypePMV.NombreLignes,
   				va_TypePMV.NombreCaracteres,
   				va_TypePMV.Flash,
   				va_TypePMV.Temperature,
				va_TypePMV.Picto);
   XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ei_fichier_type_pmv %s", pl_TexteConf );
   /* Ecriture dans le fichier de config */
   vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_Type_PMV );
                         		 
   XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ei_fichier_type_pmv " );
   return ( vl_ValRet );
   
}  



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_fichier_type_picto (	XZAOT_TypPicto va_TypePicto, int va_NumAppel )

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
*   Ecrire les parametres dans le fichier de config type Picto 
*
------------------------------------------------------*/
{
XDY_Texte	pl_TexteConf;
int		vl_ValRet = XDC_OK;
      	
   if ( va_NumAppel == 1)
   {
      /* Ecriture de l'entete dans le fichier de config */
      sprintf ( pl_TexteConf, 	"#\tNumType\tNBCar\tFicPicto\tTypePnc\tFichierPnc\n\0" );
      vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_Type_PICTO );
   }
   
   sprintf ( pl_TexteConf, 	" %d\t%d\t%s\t%d\t%s\n\0",
   				va_TypePicto.NumType,
   				va_TypePicto.NombreCaracteres,
   				va_TypePicto.FichierPanneau,
				va_TypePicto.TypePnc,
				va_TypePicto.FichierPnc);
   XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ei_fichier_type_picto %s", pl_TexteConf );
   /* Ecriture dans le fichier de config */
   vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_Type_PICTO );
                         		 
   XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ei_fichier_type_picto " );
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

int	ei_config_eqt_pmv ( 	EINI_MACHINE * 	pa_Machine )

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
*   Ecrire les parametres dans le fichier de config PMV
*   Mettre a jour pour la machine concerne la presence equipement
*
------------------------------------------------------*/
{
int		vl_ValRet = XDC_OK;
XDY_Entier	vl_NbEqt  = 0;

    /* Lecture en base de donnees des paramatres de configuration */
    vl_ValRet = XZAO78_Lire_Liste_PMV ( 	EINI_NOM_BASE,
    						pa_Machine->NomMachine,
    						ei_config_PMV, 
    						&vl_NbEqt );
    if ( ( vl_ValRet == XDC_OK ) && ( vl_NbEqt != 0 ) )
    {
       /* Memorisation existance PMV sur la machine */
       strcpy ( pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_PMV)].Equipement, XZECC_TACHE_PMV );
       pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_PMV)].Present = XZEXC_EQT_PRESENT;
       return ( XDC_OK );
    }
    else
    {
       /* Memorisation absence PMV sur la machine */
       strcpy ( pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_PMV)].Equipement, XZECC_TACHE_PMV );
       pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_PMV)].Present = XZEXC_EQT_ABSENT;
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

int	ei_config_eqt_picto ( 	EINI_MACHINE * 	pa_Machine )

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
*   Ecrire les parametres dans le fichier de config Picto
*   Mettre a jour pour la machine concerne la presence equipement
*
------------------------------------------------------*/
{
int		vl_ValRet = XDC_OK;
XDY_Entier	vl_NbEqt  = 0;

    /* Lecture en base de donnees des paramatres de configuration */
    vl_ValRet = XZAO79_Lire_Liste_Picto ( 	EINI_NOM_BASE,
    						pa_Machine->NomMachine,
    						ei_config_Picto, 
    						&vl_NbEqt );
    if ( ( vl_ValRet == XDC_OK ) && ( vl_NbEqt != 0 ) )
    {
       /* Memorisation existance Picto sur la machine */
       strcpy ( pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_PICTO)].Equipement, XZECC_TACHE_PMV );
       pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_PICTO)].Present = XZEXC_EQT_PRESENT;
       return ( XDC_OK );
    }
    else
    {
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

int	ei_config_type_pmv ( 	EINI_MACHINE * 	pa_Machine )

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
*   Ecrire les parametres dans le fichier de config type PMV
*
------------------------------------------------------*/
{
int		vl_ValRet = XDC_OK;
XDY_Entier	vl_NbType  = 0;
char 		pl_PathEtFic[XDC_PATH_ABS_SIZE];

   XZST_03EcritureTrace(XZSTC_FONCTION, " IN  : ei_config_type_pmv ");
		  	       
   /* Construction du nom du fichier XDF_Config_Type_PMV */
   sprintf ( pl_PathEtFic, "%s/%s", XZEXC_PATH_DIFFUSION, XDF_Config_Type_PMV );
      
   /* Ouverture du fichier XDF_Config_Type_PMV apres avoir detruit l'ancien*/
   XZSS_06DetruireFichier ( pl_PathEtFic );
   if ( (vg_FdConf[0] = open ( pl_PathEtFic, O_RDWR | O_CREAT , S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) ) < 0 )
   {
      XZST_03EcritureTrace( XZSTC_WARNING, "Impossible d'ouvrir le fichier %s", pl_PathEtFic );
   }


    /* Lecture en base de donnees des paramatres de configuration XZAO74_Lire_Types_PMV */
    vl_ValRet = XZAO74_Lire_Types_PMV ( 	EINI_NOM_BASE,
    						ei_fichier_type_pmv, 
    						&vl_NbType );
   /* Fermeture du fichier XDF_Config_Type_PMV */
   close ( vg_FdConf[0] );
   
   /* Si retour lecture en base OK retourne OK sinon NOK */
   if ( ( vl_ValRet == XDC_OK ) && ( vl_NbType != 0 ) )
   {
      XZST_03EcritureTrace(XZSTC_FONCTION, " OUT : ei_config_type_pmv return OK");
      return ( XDC_OK );
   }
   else
   {
      XZST_03EcritureTrace(XZSTC_FONCTION, " OUT : ei_config_type_pmv return NOK");
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

int	ei_config_type_picto (  )

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
*   Ecrire les parametres dans le fichier de config type Picto
*
------------------------------------------------------*/
{
int		vl_ValRet = XDC_OK;
XDY_Entier	vl_NbType  = 0;
char 		pl_PathEtFic[XDC_PATH_ABS_SIZE];

   XZST_03EcritureTrace(XZSTC_FONCTION, " IN  : ei_config_type_picto ");
		  	       
   /* Construction du nom du fichier XDF_Config_Type_PICTO */
   sprintf ( pl_PathEtFic, "%s/%s", XZEXC_PATH_DIFFUSION, XDF_Config_Type_PICTO );
      
   /* Ouverture du fichier XDF_Config_Type_PICTO apres avoir detruit l'ancien*/
   XZSS_06DetruireFichier ( pl_PathEtFic );
   if ( (vg_FdConf[0] = open ( pl_PathEtFic, O_RDWR | O_CREAT , S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) ) < 0 )
   {
      XZST_03EcritureTrace( XZSTC_WARNING, "Impossible d'ouvrir le fichier %s", pl_PathEtFic );
   }


    /* Lecture en base de donnees des paramatres de configuration XZAO75_Lire_Types_Picto */
    vl_ValRet = XZAO75_Lire_Types_Picto ( 	EINI_NOM_BASE,
    						ei_fichier_type_picto, 
    						&vl_NbType );
   /* Fermeture du fichier XDF_Config_Type_PICTO */
   close ( vg_FdConf[0] );
   
   /* Si retour lecture en base OK retourne OK sinon NOK */
   if ( ( vl_ValRet == XDC_OK ) && ( vl_NbType != 0 ) )
   {
      XZST_03EcritureTrace(XZSTC_FONCTION, " OUT : ei_config_type_picto return OK");
      return ( XDC_OK );
   }
   else
   {
      XZST_03EcritureTrace(XZSTC_FONCTION, " OUT : ei_config_type_picto return NOK");
      return ( XDC_NOK );
   }
                   
}
