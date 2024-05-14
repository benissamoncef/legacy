/*E*/
/*  Fichier : $Id: eini_bad.c,v 1.3 2019/01/29 11:07:27 pc2dpdy Exp $     Release : $Revision: 1.3 $       Date : $Date: 2019/01/29 11:07:27 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEINI * FICHIER eini_bad.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module de creation des fichiers de config pour la tache BAD >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	16/09/05	creation
* JMG   13/11/07        : ajout site de gestion DEM715
* JMG	07/11/18	ajout vitesse 1.3
------------------------------------------------------*/

/* fichiers inclus */

#include "eini_cfg.h"
#include "xzao451.h"

/* definitions de constantes */

/* definitions de types locaux */


/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: eini_bad.c,v 1.3 2019/01/29 11:07:27 pc2dpdy Exp $ $Revision: 1.3 $ $Date: 2019/01/29 11:07:27 $ : eini_bad" ;
  
/* declaration de fonctions internes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_config_BAD (	XZAOT_BAD va_BAD, int va_NumAppel )

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
*   Ecrire les parametres dans le fichier de config BAD
*   Mettre a jour pour la machine concerne la config de la ligne
*
------------------------------------------------------*/
{
XDY_Texte	pl_TexteConf;
int		vl_ValRet = XDC_OK;

   if ( va_NumAppel == 1)
   {
      /* Ecriture de l'entete dans le fichier de config */
      sprintf ( pl_TexteConf, 	"# NumBAD\tServeur\tLiaison\tRGS\tType\tSiteGestion\t\n\0" );
      vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_BAD );
   }
   
   sprintf ( pl_TexteConf, 	"N %d\t%s\t%d\t%s\t%d\t%d\t\n\0",
   				va_BAD.NumeroBAD,
   				va_BAD.NomServeur,
   				va_BAD.Liaison,
   				va_BAD.Rgs,
   				va_BAD.Type,
				va_BAD.SiteGestion);              
   XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ec_config_BAD %s", pl_TexteConf );
   /* Ecriture dans le fichier de config */
   vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_BAD );
   
   /* mise a jour des liaisons series de la machine */
   ec_mise_a_jour_config_ligne ( va_BAD.Liaison,
                      		 va_BAD.NomServeur,
                      		 XZECC_PROT_LCR,
                      		 XZECC_SENS_MAITRE,
                      		 XZECC_TYPE_EQT_BAD,
                      		 va_BAD.NumeroBAD,0 );
                      		 
   XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ec_config_BAD " );
   return ( vl_ValRet );
   
}  

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_config_ModuleBAD (	XZAOT_Module va_Module, int va_NumAppel )

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
*   Ecrire les parametres dans le fichier de config Module BAD
*
------------------------------------------------------*/
{
XDY_Texte	pl_TexteConf;
int		vl_ValRet = XDC_OK;

   if ( va_NumAppel == 1)
   {
      /* Ecriture de l'entete dans le fichier de config */
      sprintf ( pl_TexteConf, 	"# NumModule\tCode\t\n\0" );
      vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_BAD );
   }
   
   sprintf ( pl_TexteConf, 	"M \t%d\t%c.%c\t\n\0",
   				va_Module.NumeroMod,
   				va_Module.Code[0],
   				va_Module.Code[1] );
   XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ec_config_Module %s", pl_TexteConf );
   /* Ecriture dans le fichier de config */
   vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_BAD );
                      		 
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

int	ei_fichier_scen_bad (	XZAOT_ScenConnu va_Scen, int va_NumAppel )

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
      	
   /*
   if ( va_NumAppel == 1)
   {
      sprintf ( pl_TexteConf, 	"#\tNumScen\tFichier Scenario\t\n\0" );
      vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_Type_BAD );
   }
   
   sprintf ( pl_TexteConf, 	"S %d\t%d\t%s\t\n\0",
   				va_Scen.NumeroScen,
   				va_Scen.AutoFichier,
   				va_Scen.Type );
   XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ei_fichier_scen_bad %s", pl_TexteConf );
   vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_Type_BAD );
   	*/                      		 
   XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ei_fichier_scen_bad " );
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

int	ei_config_eqt_bad ( 	EINI_MACHINE * 	pa_Machine )

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
*   Ecrire les parametres dans le fichier de config BAD
*   Mettre a jour pour la machine concerne la presence equipement
*
------------------------------------------------------*/
{
XDY_Texte	pl_TexteConf;
int		vl_ValRet = XDC_OK;
XDY_Entier	vl_NbEqt  = 0;

    /* Lecture en base de donnees des paramatres de configuration */
    vl_ValRet = XZAO453_Liste_BAD_Machine ( 	EINI_NOM_BASE,
    						pa_Machine->NomMachine,
    						ei_config_BAD, 
    						ei_config_ModuleBAD,
    						&vl_NbEqt );
    /* Si lecture en base ok */
    if ( ( vl_ValRet == XDC_OK ) && ( vl_NbEqt != 0 ) )
    {
       /* Alors memorisation existance BAD sur la machine */
       strcpy ( pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_BAD)].Equipement, XZECC_TACHE_BAD );
       pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_BAD)].Present = XZEXC_EQT_PRESENT;
       return ( XDC_OK );
    }
    else
    {
       /* Sinon memorisation absence BAD sur la machine */
       strcpy ( pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_BAD)].Equipement, XZECC_TACHE_BAD );
       pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_BAD)].Present = XZEXC_EQT_ABSENT;
       return ( XDC_NOK );
    }
                   
}

