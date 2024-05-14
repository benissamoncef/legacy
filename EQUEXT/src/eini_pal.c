/*E*/
/* Fichier : $Id: eini_pal.c,v 1.4 2020/05/19 14:39:14 pc2dpdy Exp $     Release : $Revision: 1.4 $      Date :  $Date: 2020/05/19 14:39:14 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEINI * FICHIER eini_pal.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module de creation des fichiers de config pour la tache PAL >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	15 Jan 1998	: Creation (DEM/1539)
* JMG   13/11/07        : ajout site de gestion DEM715
*  JMG   07/11/18 : ajout vitesse dans ec_mise_a_jour_config_ligne 1.3
* JMG	12/06/19 : ajout IP DEM 1337 1.4
------------------------------------------------------*/

/* fichiers inclus */

#include "eini_cfg.h"

/* definitions de constantes */

/* definitions de types locaux */



/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: eini_pal.c,v 1.4 2020/05/19 14:39:14 pc2dpdy Exp $ : eini_pal" ;
  
/* declaration de fonctions internes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_config_PAL (	XZAOT_PAL va_PAL, int va_NumAppel )

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
*   Ecrire les parametres dans le fichier de config PAL
*   Mettre a jour pour la machine concerne la config de la ligne
*
------------------------------------------------------*/
{
XDY_Texte	pl_TexteConf;
int		vl_ValRet = XDC_OK;

   if ( va_NumAppel == 1)
   {
      /* Ecriture de l'entete dans le fichier de config */
      sprintf ( pl_TexteConf, 	"# NumPAL\tServeur\tLiaison\tRGS\tType\tSiteGestion\n\0" );
      vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_PAL );
   }
   
   sprintf ( pl_TexteConf, 	"N\t%d\t%s\t%d\t%s\t%d\t%d\t%s\t%d\n\0",
   				va_PAL.NumeroPAL,
   				va_PAL.NomServeur,
   				va_PAL.Liaison,
   				va_PAL.Rgs,
   				va_PAL.NumeroType,
				va_PAL.SiteGestion,
				va_PAL.AdresseIP,
				va_PAL.Port);
   XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ec_config_PAL %s", pl_TexteConf );
   /* Ecriture dans le fichier de config */
   vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_PAL );
   
   /* mise a jour des liaisons series de la machine */
   if (!strcmp(va_PAL.AdresseIP, XZECC_IP_VIDE))
   	ec_mise_a_jour_config_ligne ( va_PAL.Liaison,
                      		 va_PAL.NomServeur,
                      		 XZECC_PROT_LCR,
                      		 XZECC_SENS_MAITRE,
                      		 XZECC_TYPE_EQT_PAL,
                      		 va_PAL.NumeroPAL,0 );
   else
   	ec_mise_a_jour_config_ligne ( va_PAL.Port,
                      		 va_PAL.AdresseIP,
                      		 XZECC_PROT_IP,
                      		 XZECC_SENS_MAITRE,
                      		 XZECC_TYPE_EQT_PAL,
                      		 va_PAL.NumeroPAL,0 );

   XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ec_config_PAL " );
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

int	ei_config_eqt_pal ( 	EINI_MACHINE * 	pa_Machine )

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
*   Ecrire les parametres dans le fichier de config PAL
*   Mettre a jour pour la machine concerne la presence equipement
*
------------------------------------------------------*/
{
XDY_Texte	pl_TexteConf;
int		vl_ValRet = XDC_OK;
XDY_Entier	vl_NbEqt  = 0;

    /* Lecture en base de donnees des paramatres de configuration */
    vl_ValRet = XZAO422_Liste_PAL_Machine ( 	pa_Machine->NomMachine,
    						ei_config_PAL, 
    						&vl_NbEqt ); 
    /* Si lecture en base ok */
    if ( ( vl_ValRet == XDC_OK ) && ( vl_NbEqt != 0 ) )
    {
       /* Alors memorisation existance PAL sur la machine */
       strcpy ( pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_PAL)].Equipement, XZECC_TACHE_PAL );
       pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_PAL)].Present = XZEXC_EQT_PRESENT;
       return ( XDC_OK );
    }
    else
    {
       /* Sinon memorisation absence PAL sur la machine */
       strcpy ( pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_PAL)].Equipement, XZECC_TACHE_PAL );
       pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_PAL)].Present = XZEXC_EQT_ABSENT;
       return ( XDC_NOK );
    }
                   
}

