/*E*/
/* Fichier : $Id: eini_cfe.c,v 1.2 2019/01/29 11:07:30 pc2dpdy Exp $     Release : $Revision: 1.2 $      Date :  $Date: 2019/01/29 11:07:30 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEINI * FICHIER eini_pal.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module de creation des fichiers de config pour la tache CFE >
*
------------------------------------------------------
* HISTORIQUE :
*
* 
* JMG 	04/03/18 : Creation controleurs de feux CTRL_FEUX 1.1 DEM1284
*  JMG   07/11/18 : ajout vitesse dans ec_mise_a_jour_config_ligne 1.2
------------------------------------------------------*/

/* fichiers inclus */

#include "eini_cfg.h"

/* definitions de constantes */

/* definitions de types locaux */



/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: eini_cfe.c,v 1.2 2019/01/29 11:07:30 pc2dpdy Exp $ : eini_pal" ;
  
/* declaration de fonctions internes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_config_CFE (	XZAOT_CFE va_CFE, int va_NumAppel )

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
*   Ecrire les parametres dans le fichier de config CFE
*   Mettre a jour pour la machine concerne la config de la ligne
*
------------------------------------------------------*/
{
XDY_Texte	pl_TexteConf;
int		vl_ValRet = XDC_OK;

   if ( va_NumAppel == 1)
   {
      /* Ecriture de l'entete dans le fichier de config */
      sprintf ( pl_TexteConf, 	"# NumCFE\tIP\tPort\tSiteGestion\n\0" );
      vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_CFE );
   }
   
   sprintf ( pl_TexteConf, 	"N\t%d\t%s\t%d\t%d\n\0",
   				va_CFE.NumeroCFE,
   				va_CFE.AdresseIP,
   				va_CFE.Port,
				va_CFE.SiteGestion);
   XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ec_config_CFE %s", pl_TexteConf );
   /* Ecriture dans le fichier de config */
   vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_CFE );
   
   /* mise a jour des liaisons series de la machine */
   ec_mise_a_jour_config_ligne ( va_CFE.Port,
                      		 va_CFE.AdresseIP,
                      		 XZECC_PROT_DIASER,
                      		 XZECC_SENS_MAITRE,
                      		 XZECC_TYPE_EQT_CFE,
                      		 va_CFE.NumeroCFE,0 );
                      		 
   XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ec_config_CFE " );
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

int	ei_config_eqt_cfe ( 	EINI_MACHINE * 	pa_Machine )

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
*   Ecrire les parametres dans le fichier de config CFE
*   Mettre a jour pour la machine concerne la presence equipement
*
------------------------------------------------------*/
{
XDY_Texte	pl_TexteConf;
int		vl_ValRet = XDC_OK;
XDY_Entier	vl_NbEqt  = 0;

    /* Lecture en base de donnees des paramatres de configuration */
    vl_ValRet = XZAO807_Liste_CFE_Machine ( 	pa_Machine->NomMachine,
    						ei_config_CFE, 
    						&vl_NbEqt ); 
    /* Si lecture en base ok */
    if ( ( vl_ValRet == XDC_OK ) && ( vl_NbEqt != 0 ) )
    {
       /* Alors memorisation existance CFE sur la machine */
       strcpy ( pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_CFE)].Equipement, XZECC_TACHE_CFE );
       pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_CFE)].Present = XZEXC_EQT_PRESENT;
       return ( XDC_OK );
    }
    else
    {
       /* Sinon memorisation absence CFE sur la machine */
       strcpy ( pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_CFE)].Equipement, XZECC_TACHE_CFE );
       pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_CFE)].Present = XZEXC_EQT_ABSENT;
       return ( XDC_NOK );
    }
                   
}

