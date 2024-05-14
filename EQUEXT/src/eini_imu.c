/*E*/
/* Fichier : $Id: eini_imu.c,v 1.3 2021/06/07 10:23:30 pc2dpdy Exp $     Release : $Revision: 1.3 $      Date :  $Date: 2021/06/07 10:23:30 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEINI * FICHIER eini_imu.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module de creation des fichiers de config pour la tache IMU >
*
------------------------------------------------------
* HISTORIQUE :
*
* 
* CGR 	29/07/20 : Creation IMU 1.1 DEM-SAE155
* ABE	09/02/21 : Ajout SousType et NomMachine DEM-SAE155 1.2
* ABE	07/06/21 : Fix var string pour recup ligne IMU, ajout site GEstion DEM-SAE155 1.3												
------------------------------------------------------*/

/* fichiers inclus */

#include "eini_cfg.h"

/* definitions de constantes */

/* definitions de types locaux */



/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: eini_imu.c,v 1.3 2021/06/07 10:23:30 pc2dpdy Exp $ : eini_imu" ;
  
/* declaration de fonctions internes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_config_IMU (	XZAOT_IMU_491 va_IMU, int va_NumAppel )

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
*   Ecrire les parametres dans le fichier de config IMU
*   Mettre a jour pour la machine concerne la config de la ligne
*
------------------------------------------------------*/
{
char pl_TexteConf[2048];
int		vl_ValRet = XDC_OK;

   XZST_03EcritureTrace ( XZSTC_FONCTION, " IN : ec_config_IMU " );

   if ( va_NumAppel == 1)
   {
      /* Ecriture de l'entete dans le fichier de config */
      sprintf ( pl_TexteConf, 	"# NumIMU\tEndPointOPCUA\tUserOPCUA\tPasswordOPCUA\tNomBICE\tPortMaitre\tSousType\tNomMachine\tSiteGestion\n\0" );
      vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_IMU );
   }
   
   sprintf ( pl_TexteConf, 	"N\t%d\t%s\t%s\t%s\t%s\t%d\t%d\t%s\t%d\n\0",
   				va_IMU.NumeroIMU,
   				va_IMU.AdresseEndPointOPCUA,
   				va_IMU.UserOPCUA,
   				va_IMU.PasswordOPCUA,
				va_IMU.NomBICE,
				va_IMU.PortMaitre,
				va_IMU.SousType,
				va_IMU.NomMachine,
				va_IMU.SiteGestion);

   XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ec_config_IMU %s", pl_TexteConf );
   /* Ecriture dans le fichier de config */
   vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_IMU );
   
   /* mise a jour des liaisons series de la machine */
   ec_mise_a_jour_config_ligne ( va_IMU.PortMaitre,
                      		 va_IMU.AdresseEndPointOPCUA,
                      		 XZECC_PROT_OPC,
                      		 XZECC_SENS_MAITRE,
                      		 XZECC_TYPE_EQT_IMU,
                      		 va_IMU.NumeroIMU,0 );
   
   XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ec_config_IMU " );
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

int	ei_config_eqt_imu ( 	EINI_MACHINE * 	pa_Machine )

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
*   Ecrire les parametres dans le fichier de config IMU
*   Mettre a jour pour la machine concerne la presence equipement
*
------------------------------------------------------*/
{
XDY_Texte	pl_TexteConf;
int		vl_ValRet = XDC_OK;
XDY_Entier	vl_NbEqt  = 0;

    /* Lecture en base de donnees des paramatres de configuration */
    vl_ValRet = XZAO491_Lire_Liste_IMU  (XDC_BASE_CFG,
						pa_Machine->NomMachine,
    						ei_config_IMU, 
    						&vl_NbEqt ); 
    /* Si lecture en base ok */
    if ( ( vl_ValRet == XDC_OK ) && ( vl_NbEqt != 0 ) )
    {
       /* Alors memorisation existance IMU sur la machine */
       strcpy ( pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_IMU)].Equipement, XZECC_TACHE_IMU );
       pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_IMU)].Present = XZEXC_EQT_PRESENT;
       return ( XDC_OK );
    }
    else
    {
       /* Sinon memorisation absence IMU sur la machine */
       strcpy ( pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_IMU)].Equipement, XZECC_TACHE_IMU );
       pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_IMU)].Present = XZEXC_EQT_ABSENT;
       return ( XDC_NOK );
    }
                   
}

