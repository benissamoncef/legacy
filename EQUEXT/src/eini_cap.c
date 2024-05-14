/*E*/
/* Fichier : $Id: eini_cap.c,v 1.3 2020/11/03 18:09:37 pc2dpdy Exp $     Release : $Revision: 1.3 $      Date :  $Date: 2020/11/03 18:09:37 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEINI * FICHIER eini_cap.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module de creation des fichiers de config pour la tache CAP >
*
------------------------------------------------------
* HISTORIQUE :
*
* 
* JMG 	04/03/18 : Creation controleurs de feux CTRL_FEUX 1.1 DEM1284
*  JMG   07/11/18 : ajout vitesse dans ec_mise_a_jour_config_ligne 1.2
* CGR	30/07/20 : Corrections (eini_pal -> eini_cap) 1.3 DEM-SAEYYYY
------------------------------------------------------*/

/* fichiers inclus */

#include "eini_cfg.h"

/* definitions de constantes */

/* definitions de types locaux */



/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: eini_cap.c,v 1.3 2020/11/03 18:09:37 pc2dpdy Exp $ : eini_cap" ;
  
/* declaration de fonctions internes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_config_CAP (	XZAOT_CAP va_CAP, int va_NumAppel )

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
*   Ecrire les parametres dans le fichier de config CAP
*   Mettre a jour pour la machine concerne la config de la ligne
*
------------------------------------------------------*/
{
XDY_Texte	pl_TexteConf;
int		vl_ValRet = XDC_OK;

   if ( va_NumAppel == 1)
   {
      /* Ecriture de l'entete dans le fichier de config */
      sprintf ( pl_TexteConf, 	"# NumCAP\tIP\tPortMaitre\tPortEsclave\tRGS\tSiteGestion\n\0" );
      vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_CAP );
   }
   
   sprintf ( pl_TexteConf, 	"N\t%d\t%s\t%d\t%d\t%s\t%d\n\0",
   				va_CAP.NumeroCAP,
   				va_CAP.AdresseIP,
   				va_CAP.PortMaitre,
   				va_CAP.PortEsclave,
				va_CAP.AdresseRgs,
				va_CAP.SiteGestion);
   XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ec_config_CAP %s", pl_TexteConf );
   /* Ecriture dans le fichier de config */
   vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_CAP );
   
   /* mise a jour des liaisons series de la machine */
   ec_mise_a_jour_config_ligne ( va_CAP.PortMaitre,
                      		 va_CAP.AdresseIP,
                      		 XZECC_PROT_IP,
                      		 XZECC_SENS_MAITRE,
                      		 XZECC_TYPE_EQT_CAP,
                      		 va_CAP.NumeroCAP,0 );
                      		 
   XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ec_config_CAP " );
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

int	ei_config_eqt_cap ( 	EINI_MACHINE * 	pa_Machine )

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
*   Ecrire les parametres dans le fichier de config CAP
*   Mettre a jour pour la machine concerne la presence equipement
*
------------------------------------------------------*/
{
XDY_Texte	pl_TexteConf;
int		vl_ValRet = XDC_OK;
XDY_Entier	vl_NbEqt  = 0;

    /* Lecture en base de donnees des paramatres de configuration */
    vl_ValRet = XZAO861_Lire_Liste_CAP  (XDC_BASE_CFG,
						pa_Machine->NomMachine,
						XDC_CAP_TYPE_BOUCHON,
    						ei_config_CAP, 
    						&vl_NbEqt ); 
    /* Si lecture en base ok */
    if ( ( vl_ValRet == XDC_OK ) && ( vl_NbEqt != 0 ) )
    {
       /* Alors memorisation existance CAP sur la machine */
       strcpy ( pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_CAP)].Equipement, XZECC_TACHE_CAP );
       pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_CAP)].Present = XZEXC_EQT_PRESENT;
       return ( XDC_OK );
    }
    else
    {
       /* Sinon memorisation absence CAP sur la machine */
       strcpy ( pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_CAP)].Equipement, XZECC_TACHE_CAP );
       pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_CAP)].Present = XZEXC_EQT_ABSENT;
       return ( XDC_NOK );
    }
                   
}

