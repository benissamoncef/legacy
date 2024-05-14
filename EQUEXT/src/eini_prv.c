/*E*/
/* Fichier : $Id: eini_prv.c,v 1.2 2019/01/29 11:07:37 pc2dpdy Exp $        Release : $Revision: 1.2 $        Date : $Date: 2019/01/29 11:07:37 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEINI * FICHIER eini_prv.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module de creation des fichiers de config pour la tache PRV >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR	20/12/11		: Creation (DEM/1016)
*  JMG   07/11/18 : ajout vitesse dans ec_mise_a_jour_config_ligne 1.2
* ABE	31/05/21	: PRV suur IP DEM-SAE283 1.3
------------------------------------------------------*/

/* fichiers inclus */

#include "eini_cfg.h"
#include "xzao870.h"

/* definitions de constantes */

/* definitions de types locaux */


/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)eini_prv.c	1.2 1.2 11/20/07: eini_prv" ;
  
/* declaration de fonctions internes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_config_PRV (	XZAOT_PRV va_PRV, int va_NumAppel )

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

XZST_03EcritureTrace( XZSTC_FONCTION, "IN : ei_config_PRV" );
XDY_Texte	pl_TexteConf;
int		vl_ValRet = XDC_OK;

   if ( va_NumAppel == 1)
   {
      /* Ecriture de l'entete dans le fichier de config */
      sprintf ( pl_TexteConf, 	"#\tTypeEqt\tNumPRV\tLiaison\tRGS\tServeur\tType\tVitesseNominale\tZone\tCritique\tSiteGestion\tAdresseIP\tPort\n\0" );
      vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_PRV );
      XZST_03EcritureTrace( XZSTC_INFO, "Ecriture de l'entete dans le fichier de config");
   }
   else
   {
	   XZST_03EcritureTrace ( XZSTC_WARNING, "OUT ei_config_PRV : Aucune Ecriture de l'entete dans le fichier de config");
   }
  

   sprintf ( pl_TexteConf, 	" P\t%d\t%d\t%s\t%s\t%d\t%d\t%d\t%d\t%d\t%s\t%d\n\0",
		   va_PRV.NumeroPRV,
		   va_PRV.Liaison,
		   va_PRV.RGS,
		   va_PRV.NomServeur,
		   va_PRV.Type,
		   va_PRV.Vitesse_nominale,
		   va_PRV.Zone,
		   va_PRV.Critique,
		   va_PRV.SiteGestion,
		   va_PRV.Adresse_IP,
		   va_PRV.Port);
				

   XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ei_config_PRV %s", pl_TexteConf );
   /* Ecriture dans le fichier de config */
   vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_PRV );
   
   /* mise a jour des liaisons series de la machine */

   if (!strcmp(va_PRV.Adresse_IP,XZECC_IP_VIDE)) 
	   ec_mise_a_jour_config_ligne ( va_PRV.Liaison,
	   	   	   	va_PRV.NomServeur,
                      		XZECC_PROT_LCR,
                      		XZECC_SENS_MAITRE,
                      		XZECC_TYPE_EQT_PRV,
				va_PRV.NumeroPRV,0 ); 
   else 
	   ec_mise_a_jour_config_ligne ( va_PRV.Port, 
                                 va_PRV.Adresse_IP, 
                                 XZECC_PROT_IP, 
                                 XZECC_SENS_MAITRE, 
                                 XZECC_TYPE_EQT_PRV, 
                                 va_PRV.NumeroPRV,0 ); 


   XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ei_config_PRV " );
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

int	ei_config_eqt_prv ( 	EINI_MACHINE * 	pa_Machine )

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
*   Ecrire les parametres dans le fichier de config PRV
*   Mettre a jour pour la machine concerne la presence equipement
*
------------------------------------------------------*/
{
	XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_WARNING|XZSTC_INFO|XZSTC_FONCTION|XZSTC_INTERFACE|XZSTC_MESSAGE,XZSTC_NECRAN);

	XZST_03EcritureTrace( XZSTC_FONCTION, "IN : ei_config_eqt_prv" );
int		vl_ValRet = XDC_OK;
XDY_Entier	vl_NbEqt  = 0;

    /* Lecture en base de donnees des paramatres de configuration */
    vl_ValRet = XZAO870_Lire_Liste_PRV ( 	EINI_NOM_BASE,
    						pa_Machine->NomMachine,
    						ei_config_PRV, 
    						&vl_NbEqt );

    XZST_03EcritureTrace( XZSTC_INFO, "XZAO870_Lire_Liste_PRV : EINI_NOM_BASE : %s, NomMachine : %s ", EINI_NOM_BASE, pa_Machine->NomMachine);

    if ( ( vl_ValRet == XDC_OK ) && ( vl_NbEqt != 0 ) )
    {
       /* Memorisation existance PRV sur la machine */
       strcpy ( pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_PRV)].Equipement, XZECC_TACHE_PRV );
       pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_PRV)].Present = XZEXC_EQT_PRESENT;
       XZST_03EcritureTrace( XZSTC_INFO, "Memorisation existance PRV sur la machine");
       return ( XDC_OK );
    }
    else
    {
       /* Memorisation absence PRV sur la machine */
       strcpy ( pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_PRV)].Equipement, XZECC_TACHE_PRV );
       pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_PRV)].Present = XZEXC_EQT_ABSENT;
       XZST_03EcritureTrace( XZSTC_INFO, "Memorisation absence PRV sur la machine");
       return ( XDC_NOK );
    }

    XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ei_config_eqt_prv " );
                   
}
