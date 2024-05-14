/*E*/
/*  Fichier : $Id: eini_pma.c,v 1.3 2019/01/07 16:36:59 pc2dpdy Exp $  Release : $Revision: 1.3 $      Date : $Date: 2019/01/07 16:36:59 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEINI * FICHIER eini_pmv.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module de creation des fichiers de config pour la tache PMVA >
*
------------------------------------------------------
* HISTORIQUE :
*
* jmg	10/06/05	creation
* JMG   13/11/07        : ajout site de gestion DEM715
* JMG	02/10/18	: ajout IP suppression SAGA DEM1306 1.3
* JMG	07/11/18	: ajout vitesse 1.3
------------------------------------------------------*/

/* fichiers inclus */

#include "eini_cfg.h"
#include "xzao780.h"

/* definitions de constantes */

/* definitions de types locaux */


/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: eini_pma.c,v 1.3 2019/01/07 16:36:59 pc2dpdy Exp $ $Revision: 1.3 $ $Date: 2019/01/07 16:36:59 $: eini_pmv" ;
  
/* declaration de fonctions internes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_config_PMVA (	XZAOT_PMVA va_PMV, int va_NumAppel )

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

   if ( va_NumAppel == 1)
   {
      /* Ecriture de l'entete dans le fichier de config */
      sprintf ( pl_TexteConf, 	"#\tTypeEqt\tNumPMVA\tServeur\tLiaison\tRGS\tSiteGestion\tPort\tAdresseIP\tType\tIdLCR\n\0" );
      vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_PMVA );
   }
   

   XZST_03EcritureTrace ( XZSTC_WARNING,"Port %d",va_PMV.Port);
   sprintf ( pl_TexteConf, 	" P\t%d\t%s\t%d\t%s\t%d\t%d\t%s\t%d\t%s\n\0",
   				va_PMV.NumeroPMVA,
   				va_PMV.NomServeur,
   				va_PMV.Liaison,
   				va_PMV.RGS,
				va_PMV.SiteGestion,
				va_PMV.Port,
				va_PMV.AdresseIP,
				va_PMV.NumeroTypePMVA,
				va_PMV.IdLCR);
   XZST_03EcritureTrace ( XZSTC_WARNING, " IN  : ec_config_PMVA %s", pl_TexteConf );
   /* Ecriture dans le fichier de config */
   vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_PMVA );
   
   /* mise a jour des liaisons series de la machine */
   if (!strcmp(va_PMV.AdresseIP,XZECC_IP_VIDE))
   	ec_mise_a_jour_config_ligne ( va_PMV.Liaison,
                      		 va_PMV.NomServeur,
                      		 XZECC_PROT_LCR,
                      		 XZECC_SENS_MAITRE,
                      		 XZECC_TYPE_EQT_PMVA,
                      		 va_PMV.NumeroPMVA,0 );
   else
   	ec_mise_a_jour_config_ligne ( va_PMV.Port,
				va_PMV.AdresseIP,
				XZECC_PROT_IP,
				XZECC_SENS_MAITRE,
				XZECC_TYPE_EQT_PMVA,
				va_PMV.NumeroPMVA,0);
   XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ec_config_PMVA " );
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

int	ei_config_eqt_pmva ( 	EINI_MACHINE * 	pa_Machine )

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
    vl_ValRet = XZAO780_Lire_Liste_PMVA ( 	EINI_NOM_BASE,
    						pa_Machine->NomMachine,
    						ei_config_PMVA, 
    						&vl_NbEqt );
    if ( ( vl_ValRet == XDC_OK ) && ( vl_NbEqt != 0 ) )
    {
       /* Memorisation existance PMV sur la machine */
       strcpy ( pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_PMVA)].Equipement, XZECC_TACHE_PMVA );
       pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_PMVA)].Present = XZEXC_EQT_PRESENT;
       return ( XDC_OK );
    }
    else
    {
       /* Memorisation absence PMV sur la machine */
       strcpy ( pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_PMVA)].Equipement, XZECC_TACHE_PMVA );
       pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_PMVA)].Present = XZEXC_EQT_ABSENT;
       return ( XDC_NOK );
    }
                   
}
