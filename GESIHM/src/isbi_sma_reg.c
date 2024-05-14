/*E*/
/*Fichier : $Id: isbi_sma_reg.c,v 1.3 2017/04/02 16:02:31 pc2dpdy Exp $    Release : $Revision: 1.3 $        Date : $Date: 2017/04/02 16:02:31 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  GESIHM
------------------------------------------------------
* MODULE MSMA * FICHIER isyn_sma_reg.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale
*   ou reference au fichier isyn_sma_reg.h >
*
------------------------------------------------------
* HISTORIQUE :
*
* Niepceron	23 Nov 1994	: Creation
*  JMG           03/03/17 : regio DEM1220
------------------------------------------------------*/

/* fichiers inclus */

#include "isbi_sma_reg.h"

/* definitions de constantes */

#define CM_NOM_FIC_REG "/produits/migrazur/appliSD/fichiers/syn/reglages"
/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)isbi_sma_reg.c	1.5 10/05/09 : isbi_sma_reg" ;


float vm_coeff_predictif=0.0;
XDY_Entier vm_long_boucl=0;
XDY_Entier vm_long_veh=0;

extern XDY_Nom vm_NomMachineSS;



/* declaration de fonctions internes */

/* definition de fonctions externes */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction d'init des variables de reglages par lecture du fichier "reglages"
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
MSMA_init_reg( pa_Site )

	char *pa_Site;

/*
* ARGUMENTS EN ENTREE :
*   aucun
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   - Ouverture fichier
*	- lecture coeff predictif
*	- lecture longueur de boucle
*	- lecture longueur vehicule
*   - Fermeture fichier
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sma_reg.c	1.5 10/05/09 : MSMA_init_reg";
   FILE *pl_fic_reg;
   char vl_chaine[256];

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSMA_init_reg" );

   /*Test d'existance du fichier contenant la liste des vues */
   if ( ( pl_fic_reg = fopen( CM_NOM_FIC_REG , "r")) == NULL ) 
      {
      XZST_03EcritureTrace(   XZSTC_WARNING, "Fichier contenant les reglages introuvable : %s", CM_NOM_FIC_REG );
      return XDC_NOK;
      }
   if ( fgets( vl_chaine, 256 ,pl_fic_reg)  != NULL )
      {
      XZST_03EcritureTrace(   XZSTC_INFO, "vm_coeff_predictif: %s ", vl_chaine);     
      vm_coeff_predictif = atof(vl_chaine);  
      }
   else
      {
      XZST_03EcritureTrace(   XZSTC_WARNING, " Fin de fichier inattendu dans la lecture du fichier de reglage .\n");
      return XDC_NOK;
      }
   if ( fgets( vl_chaine, 256 ,pl_fic_reg) != NULL )
      {
      XZST_03EcritureTrace(   XZSTC_INFO, "long_boucl: %s ", vl_chaine);     
      vm_long_boucl = (XDY_Entier) atoi(vl_chaine);
      }
   else
      {
      XZST_03EcritureTrace(   XZSTC_WARNING, " Fin de fichier inattendu dans la lecture du fichier de reglage .\n");
      return XDC_NOK;
      }
   
   if ( fgets( vl_chaine, 256 ,pl_fic_reg) != NULL )
      {
      XZST_03EcritureTrace(   XZSTC_INFO, "long_veh: %s ", vl_chaine);     
      vm_long_veh = (XDY_Entier) atoi(vl_chaine);
      }
   else
      {
      XZST_03EcritureTrace(   XZSTC_WARNING, " Fin de fichier inattendu dans la lecture du fichier de reglage .\n");
      return XDC_NOK;
      }
  
   if ( fgets( vl_chaine, 256 ,pl_fic_reg) != NULL )
      {
      XZST_03EcritureTrace(   XZSTC_INFO, "Nom mach. Serveur Syn. CI: %s ", vl_chaine);
      if ( strcmp( pa_Site, XDC_NOM_SITE_CI ) == NULL )
         strcpy( vm_NomMachineSS , vl_chaine );
      }
   else
      {
      XZST_03EcritureTrace(   XZSTC_WARNING, " Fin de fichier inattendu dans la lecture du fichier de reglage .\n");
      return XDC_NOK;
      }

   if ( fgets( vl_chaine, 256 ,pl_fic_reg) != NULL )
      {
	XZST_03EcritureTrace(   XZSTC_INFO, "Nom mach. Serveur Syn. DM: %s ", vl_chaine);
	if ( strcmp( pa_Site, XDC_NOM_SITE_VC ) == NULL )
         strcpy( vm_NomMachineSS , vl_chaine );
      }
   else
      {
      XZST_03EcritureTrace(   XZSTC_WARNING, " Fin de fichier inattendu dans la lecture du fichier de reglage .\n");
      return XDC_NOK;
      }

   if ( fgets( vl_chaine, 256 ,pl_fic_reg) != NULL )
      {
       XZST_03EcritureTrace(   XZSTC_INFO, "Nom mach. Serveur Syn. DP: %s ", vl_chaine);
     if ( strcmp( pa_Site, XDC_NOM_SITE_DP ) == NULL )
         strcpy( vm_NomMachineSS , vl_chaine );
     }
   else
      {
      XZST_03EcritureTrace(   XZSTC_WARNING, " Fin de fichier inattendu dans la lecture du fichier de reglage .\n");
      return XDC_NOK;
      }

   if ( fgets( vl_chaine, 256 ,pl_fic_reg) != NULL )
      {
      XZST_03EcritureTrace(   XZSTC_WARNING, " Fin de fichier attendu et non rencontree dans la lecture du fichier de reglage .\n");
      XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSMA_init_reg" );
      return XDC_OK;
      }

   fclose( pl_fic_reg );
   vm_NomMachineSS[strlen(vm_NomMachineSS)-1]='\000';
   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSMA_init_reg" );
   return XDC_OK;
}


