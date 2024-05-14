/*E*/
/*  Fichier : $Id: eini_baf.c,v 1.4 2020/05/19 14:18:10 pc2dpdy Exp $     Release : $Revision: 1.4 $       Date : $Date: 2020/05/19 14:18:10 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEINI * FICHIER eini_bad.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module de creation des fichiers de config pour la tache BAF >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	16/09/05	creation
* JMG   13/11/07        : ajout site de gestion DEM715
* JMG	07/11/18/	: ajout vitesse 1.3
* JMG	01/06/19  	: passage IP DEM1332 1.4
------------------------------------------------------*/

/* fichiers inclus */

#include "eini_cfg.h"
#include "xzao452.h"

/* definitions de constantes */

/* definitions de types locaux */


/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: eini_baf.c,v 1.4 2020/05/19 14:18:10 pc2dpdy Exp $ $Revision: 1.4 $ $Date: 2020/05/19 14:18:10 $ : eini_baf" ;
  
/* declaration de fonctions internes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_config_BAF (	XZAOT_BAF va_BAF, int va_NumAppel )

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
*   Ecrire les parametres dans le fichier de config BAF
*   Mettre a jour pour la machine concerne la config de la ligne
*
------------------------------------------------------*/
{
XDY_Texte	pl_TexteConf;
int		vl_ValRet = XDC_OK;

   if ( va_NumAppel == 1)
   {
      /* Ecriture de l'entete dans le fichier de config */
      sprintf ( pl_TexteConf, 	"# NumBAF\tServeur\tLiaison\tRGS\tType\tSiteGestion\tPort\tAdresseIP\n\0" );
      vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_BAF );
   }
   
   sprintf ( pl_TexteConf, 	"N %d\t%s\t%d\t%s\t%d\t%d\t%d\t%s\n\0",
   				va_BAF.NumeroBAF,
   				va_BAF.NomServeur,
   				va_BAF.Liaison,
   				va_BAF.Rgs,
   				va_BAF.Type,
				va_BAF.SiteGestion,
				va_BAF.Port,
				va_BAF.AdresseIP);              
   XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ec_config_BAF %s", pl_TexteConf );
   /* Ecriture dans le fichier de config */
   vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_BAF );
   
   	/* mise a jour des liaisons series de la machine */
   	if (!strcmp(va_BAF.AdresseIP,XZECC_IP_VIDE))
   		ec_mise_a_jour_config_ligne (	va_BAF.Liaison,
										va_BAF.NomServeur,
										XZECC_PROT_LCR,
										XZECC_SENS_MAITRE,
										XZECC_TYPE_EQT_BAF,
										va_BAF.NumeroBAF,0 );
	else
   		ec_mise_a_jour_config_ligne (	va_BAF.Port,
                      					va_BAF.AdresseIP,
                      					XZECC_PROT_IP,
                      					XZECC_SENS_MAITRE,
                      					XZECC_TYPE_EQT_BAF,
                      					va_BAF.NumeroBAF,0 );

   XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ec_config_BAF " );
   return ( vl_ValRet );
   
}  

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_config_ModuleBAF (	XZAOT_Module va_Module, int va_NumAppel )

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
*   Ecrire les parametres dans le fichier de config Module BAF
*
------------------------------------------------------*/
{
XDY_Texte	pl_TexteConf;
int		vl_ValRet = XDC_OK;

   if ( va_NumAppel == 1)
   {
      /* Ecriture de l'entete dans le fichier de config */
      sprintf ( pl_TexteConf, 	"# NumModule\tCode\t\n\0" );
      vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_BAF );
   }
   
   sprintf ( pl_TexteConf, 	"M \t%d\t%c.%c\t\n\0",
   				va_Module.NumeroMod,
   				va_Module.Code[0],
   				va_Module.Code[1] );
   XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ec_config_Module %s", pl_TexteConf );
   /* Ecriture dans le fichier de config */
   vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_BAF );
                      		 
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

int	ei_fichier_scen_baf (	XZAOT_ScenConnu va_Scen, int va_NumAppel )

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
      vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_Type_BAF );
   }
   
   sprintf ( pl_TexteConf, 	"S %d\t%d\t%s\t\n\0",
   				va_Scen.NumeroScen,
   				va_Scen.AutoFichier,
   				va_Scen.Type );
   XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ei_fichier_scen_bad %s", pl_TexteConf );
   vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_Type_BAF );
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

int	ei_config_eqt_baf ( 	EINI_MACHINE * 	pa_Machine )

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
*   Ecrire les parametres dans le fichier de config BAF
*   Mettre a jour pour la machine concerne la presence equipement
*
------------------------------------------------------*/
{
XDY_Texte	pl_TexteConf;
int		vl_ValRet = XDC_OK;
XDY_Entier	vl_NbEqt  = 0;

    /* Lecture en base de donnees des paramatres de configuration */
    vl_ValRet = XZAO452_Liste_BAF_Machine ( 	EINI_NOM_BASE,
    						pa_Machine->NomMachine,
    						ei_config_BAF, 
    						ei_config_ModuleBAF,
    						&vl_NbEqt );
   XZST_03EcritureTrace ( XZSTC_WARNING, "XZAO452_Liste_BAF_Machine OUT : %d", vl_NbEqt);
    /* Si lecture en base ok */
    if ( ( vl_ValRet == XDC_OK ) && ( vl_NbEqt != 0 ) )
    {
       /* Alors memorisation existance BAF sur la machine */
       strcpy ( pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_BAF)].Equipement, XZECC_TACHE_BAF );
       pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_BAF)].Present = XZEXC_EQT_PRESENT;
       return ( XDC_OK );
    }
    else
    {
       /* Sinon memorisation absence BAF sur la machine */
       strcpy ( pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_BAF)].Equipement, XZECC_TACHE_BAF );
       pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_BAF)].Present = XZEXC_EQT_ABSENT;
       return ( XDC_NOK );
    }
                   
}

