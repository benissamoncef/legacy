/*E*/
/*  Fichier : $Id: eini_prf.c,v 1.1 1995/05/22 19:32:47 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/05/22 19:32:47 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  EQUEXT
------------------------------------------------------
* MODULE MDIR * FICHIER eini_prf.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module de creation des fichiers de config pour les profils utilisateurs >
*
------------------------------------------------------
* HISTORIQUE :
*
* F. VOLCIC	19 Mai 1995	: Creation
------------------------------------------------------*/

/* fichiers inclus */

#include "eini_cfg.h"

/* definitions de constantes */

/* definitions de types locaux */


/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: eini_prf.c,v 1.1 1995/05/22 19:32:47 volcic Exp $ : eini_prf" ;
  
/* declaration de fonctions internes */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*   Ecrire les parametres dans le fichier de config d'un utilisateur maintenance
*   Mettre a jour pour la machine concerne la presence equipement
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_config_profil_util (	char 	   pa_Profil_MDP[18],
				XDY_Entier va_NumAppel )

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
*   Ecrire les parametres dans le fichier de config d'un utilisateur maintenance
*   Mettre a jour pour la machine concerne la presence equipement
*
------------------------------------------------------*/
{
int		vl_ValRet = XDC_OK;
XDY_Texte	pl_TexteConf = "";

   XZST_03EcritureTrace ( XZSTC_WARNING, " IN  : ec_config_profil_util %s", pl_TexteConf );
   /* Ecriture dans le fichier de config */
   sprintf ( pl_TexteConf, "%s\n", pa_Profil_MDP );
   vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_Utilisateurs );

   XZST_03EcritureTrace ( XZSTC_WARNING, " OUT : ec_config_profil_util " );
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

int	ei_config_liste_profil ( )

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
*   
*	Retourne la liste des utilisateurs avec leur mot de passe crypte
*
------------------------------------------------------*/
{
int		vl_ValRet = XDC_OK;

    /* Lecture en base de donnees des paramatres de configuration */
    vl_ValRet = XZAU16_Liste_Utilisateurs ( ei_config_profil_util );
    if ( vl_ValRet == XDC_OK )
       return ( XDC_OK );
    else
       return ( XDC_NOK );
}
