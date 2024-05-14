/*E*/
/* Fichier : $Id: eini_pic.c,v 1.2 2023/08/18 11:07:37 pc2dpdy Exp $        Release : $Revision: 1.2 $        Date : $Date: 2023/08/18 11:07:37 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEINI * FICHIER eini_pic.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module de creation des fichiers de config pour la tache PIC >
*
------------------------------------------------------
* HISTORIQUE :
*
* ABK 	18/08/23		: Creation (DEM/483)
------------------------------------------------------*/

/* fichiers inclus */

#include "eini_cfg.h"
#include "xzao8701.h"

/* definitions de constantes */

/* definitions de types locaux */


/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)eini_pic.c	1.2 1.2 11/20/07: eini_pic" ;
  
/* declaration de fonctions internes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_config_PIC (	XZAOT_PIC va_PIC, int va_NumAppel )

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

XZST_03EcritureTrace( XZSTC_FONCTION, "IN : ei_config_PIC" );

XDY_Texte	pl_TexteConf;
int		vl_ValRet = XDC_OK;

   if ( va_NumAppel == 1)
   {
      /* Ecriture de l'entete dans le fichier de config */
      sprintf ( pl_TexteConf, 	"#\tTypeEqt\tNumPIC\tLiaison\tRGS\tServeur\tSiteGestion\tAdresseIP\tPort\n\0" );
      vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_PIC );
      XZST_03EcritureTrace( XZSTC_INFO, "Ecriture de l'entete dans le fichier de config vl_ValRet=%d.", vl_ValRet);
   }
   else
   {
	   XZST_03EcritureTrace ( XZSTC_WARNING, "OUT ei_config_PIC : Aucune Ecriture de l'entete dans le fichier de config");
   }
  

   sprintf ( pl_TexteConf, 	" P\t%d\t%d\t%s\t%s\t%d\t%s\t%d\n\0",
		   va_PIC.NumeroPIC,
		   va_PIC.Liaison,
		   va_PIC.RGS,
		   va_PIC.NomServeur,
		   va_PIC.SiteGestion,
		   va_PIC.Adresse_IP,
		   va_PIC.Port);
				

   XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ei_config_PIC %s", pl_TexteConf );
   /* Ecriture dans le fichier de config */
   vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_PIC );
   
   /* mise a jour des liaisons series de la machine */

   if (!strcmp(va_PIC.Adresse_IP,XZECC_IP_VIDE)) 
	   ec_mise_a_jour_config_ligne ( va_PIC.Liaison,
	   	   	   	va_PIC.NomServeur,
                      		XZECC_PROT_LCR,
                      		XZECC_SENS_MAITRE,
                      		XZECC_TYPE_EQT_PIC,
				va_PIC.NumeroPIC,0 ); 
   else 
	   ec_mise_a_jour_config_ligne ( va_PIC.Port, 
                                 va_PIC.Adresse_IP, 
                                 XZECC_PROT_IP, 
                                 XZECC_SENS_MAITRE, 
                                 XZECC_TYPE_EQT_PIC, 
                                 va_PIC.NumeroPIC,0 ); 


   XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ei_config_PIC " );
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

int	ei_config_eqt_pic ( 	EINI_MACHINE * 	pa_Machine )

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
*   Ecrire les parametres dans le fichier de config PIC
*   Mettre a jour pour la machine concerne la presence equipement
*
------------------------------------------------------*/
{
	XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_WARNING|XZSTC_INFO|XZSTC_FONCTION|XZSTC_INTERFACE|XZSTC_MESSAGE,XZSTC_NECRAN);

	XZST_03EcritureTrace( XZSTC_FONCTION, "IN : ei_config_eqt_pic" );
int		vl_ValRet = XDC_OK;
XDY_Entier	vl_NbEqt  = 0;

    /* Lecture en base de donnees des paramatres de configuration */
    vl_ValRet = XZAO8701_Lire_Liste_PIC ( 	EINI_NOM_BASE,
    						pa_Machine->NomMachine,
    						ei_config_PIC, 
    						&vl_NbEqt );

    XZST_03EcritureTrace( XZSTC_INFO, "XZAO870_Lire_Liste_PIC : EINI_NOM_BASE : %s, NomMachine : %s ", EINI_NOM_BASE, pa_Machine->NomMachine);

    if ( ( vl_ValRet == XDC_OK ) && ( vl_NbEqt != 0 ) )
    {
       /* Memorisation existance PIC sur la machine */
       strcpy ( pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_PIC)].Equipement, XZECC_TACHE_PIC );
       pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_PIC)].Present = XZEXC_EQT_PRESENT;
       XZST_03EcritureTrace( XZSTC_INFO, "Memorisation existance PIC sur la machine");
       return ( XDC_OK );
    }
    else
    {
       /* Memorisation absence PIC sur la machine */
       strcpy ( pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_PIC)].Equipement, XZECC_TACHE_PIC );
       pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_PIC)].Present = XZEXC_EQT_ABSENT;
       XZST_03EcritureTrace( XZSTC_INFO, "Memorisation absence PIC sur la machine");
       return ( XDC_NOK );
    }

    XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ei_config_eqt_pic " );
                   
}
