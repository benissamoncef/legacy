/*E*/
/*  Fichier : $Id: eini_dai.c,v 1.7 2021/05/04 13:25:03 pc2dpdy Exp $      Release : $Revision: 1.7 $        Date : $Date: 2021/05/04 13:25:03 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEINI * FICHIER eini_dai.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module de creation des fichiers de config pour la tache DAI >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1 	26 Sep 1994	: Creation
* Mismer.D 	version 1.2 	14 Dec 1994 	:
* Fontaine.C 	version 1.3 	19 Mai 1995 	: Reprise entete, historique
* JMG   13/11/07        : ajout site de gestion DEM715
*  JMG   07/11/18 : ajout vitesse dans ec_mise_a_jour_config_ligne 1.5
* JMG	01/06/19 : passage IP DEM1333 1.6
* CGR	14/09/20 : Ajout deuxieme adresse IP DEM-SAE175 1.7
------------------------------------------------------*/

/* fichiers inclus */

#include "eini_cfg.h"

/* definitions de constantes */

/* definitions de types locaux */


/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: eini_dai.c,v 1.7 2021/05/04 13:25:03 pc2dpdy Exp $ : eini_dai" ;
  
/* declaration de fonctions internes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_config_DAI (	XZAOT_DAI 	va_DAI,
			int		va_NumAppel )

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
*   Ecrire les parametres dans le fichier de config d'un DAI
*   Mettre a jour pour la machine concerne config de la ligne
*
------------------------------------------------------*/
{
XDY_Texte	pl_TexteConf;
int		vl_ValRet = XDC_OK;

   if ( va_NumAppel == 1)
   {
      /* Ecriture de l'entete dans le fichier de config */
      sprintf ( pl_TexteConf, 	"#NumDAI\tServeur\tLiaisonM\tLiaisonE\tRGS\tSiteGestion\tPort\tAdresseIP\tPortE\tAdresseIPE\tPort2\tAdresse2IP\tPort2E\tAdresse2IPE\n\0" );
      vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_DAI );
   }
   sprintf ( pl_TexteConf, 	" %d\t%s\t%d\t%d\t%s\t%d\t%d\t%s\t%d\t%s\t%d\t%s\t%d\t%s\n\0",
   				va_DAI.NumeroDAI,
   				va_DAI.NomServeur,
   				va_DAI.LiaisonMaitre,
   				va_DAI.LiaisonEsclave,
   				va_DAI.Rgs,
				va_DAI.SiteGestion,
				va_DAI.Port,
				va_DAI.AdresseIP,
				va_DAI.PortE,
				va_DAI.AdresseIPE,
				va_DAI.Port2,
				va_DAI.Adresse2IP,
				va_DAI.Port2E,
				va_DAI.Adresse2IPE);
   XZST_03EcritureTrace ( XZSTC_FONCTION, " XZAO82 Ecriture config : %s\n", pl_TexteConf );
   /* Ecriture dans le fichier de config */
   vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_DAI );

   if (!strcmp(va_DAI.AdresseIP,XZECC_IP_VIDE)) {
   /* mise a jour des liaisons series de la machine */
   ec_mise_a_jour_config_ligne ( va_DAI.LiaisonMaitre,
                      		 va_DAI.NomServeur,
                      		 XZECC_PROT_LCR,
                      		 XZECC_SENS_MAITRE,
                      		 XZECC_TYPE_EQT_DAI,
                      		 va_DAI.NumeroDAI ,0 );
    
   ec_mise_a_jour_config_ligne ( va_DAI.LiaisonEsclave,
                      		 va_DAI.NomServeur,
                      		 XZECC_PROT_LCR,
                      		 XZECC_SENS_ESCLAVE,
                      		 XZECC_TYPE_EQT_DAI,
                      		 va_DAI.NumeroDAI, 0 );
   }
      else {
   ec_mise_a_jour_config_ligne ( va_DAI.Port,
                      		 va_DAI.AdresseIP,
                      		 XZECC_PROT_IP,
                      		 XZECC_SENS_MAITRE,
                      		 XZECC_TYPE_EQT_DAI,
                      		 va_DAI.NumeroDAI ,0 );
    
   ec_mise_a_jour_config_ligne ( va_DAI.PortE,
                      		 va_DAI.AdresseIPE,
                      		 XZECC_PROT_IP,
                      		 XZECC_SENS_ESCLAVE,
                      		 XZECC_TYPE_EQT_DAI,
                      		 va_DAI.NumeroDAI, 0 );

   ec_mise_a_jour_config_ligne ( va_DAI.Port2,
                                 va_DAI.Adresse2IP,
				 XZECC_PROT_IP,
				 XZECC_SENS_MAITRE,
				 XZECC_TYPE_EQT_DAI,
				 va_DAI.NumeroDAI ,0 );

   ec_mise_a_jour_config_ligne ( va_DAI.Port2E,
   				 va_DAI.Adresse2IPE,
				 XZECC_PROT_IP,
				 XZECC_SENS_ESCLAVE,
				 XZECC_TYPE_EQT_DAI,
				 va_DAI.NumeroDAI, 0 );
       }


   XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ec_config_DAI " );
   return ( vl_ValRet );
   
}  

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_config_Analyseur_DAI (	XZAOT_Analyseur va_Analyseur,
					int		va_NumAppel )

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
*   Ecrire les parametres dans le fichier de config d'un Analyseur
*
------------------------------------------------------*/
{
XDY_Texte	pl_TexteConf;
int		vl_ValRet = XDC_OK;

   if ( va_NumAppel == 1)
   {
      /* Ecriture de l'entete dans le fichier de config */
      sprintf ( pl_TexteConf, 	"# NumDAI\tNumAnal\tNumEqtAna\tNbVoie\tSiteGestion\t\n\0" );
      vl_ValRet = ec_write_fic ( vg_FdConf[1], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_Analyseur );
   }
   sprintf ( pl_TexteConf, 	" %d\t%d\t%d\t%d\t%d\t\n\0",
   				va_Analyseur.NumeroDAI,
   				va_Analyseur.NumeroAnalyseur,
   				va_Analyseur.NumeroEqt,
   				va_Analyseur.NombreVoies,
				va_Analyseur.SiteGestion);
   XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ec_config_Analyseur %s", pl_TexteConf );
   
   /* Ecriture dans le fichier de config */
   vl_ValRet = ec_write_fic ( vg_FdConf[1], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_Analyseur );
    
   XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ec_config_Analyseur " );
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

int	ei_config_eqt_dai ( 	EINI_MACHINE * 	pa_Machine )

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
*   Ecrire les parametres dans le fichier de config d'une DAI
*   Mettre a jour pour la machine concerne la presence equipement
*
------------------------------------------------------*/
{
XDY_Texte	pl_TexteConf;
int		vl_ValRet = XDC_OK;
XDY_Entier	vl_NbEqt  = 0;

    /* Lecture en base de donnees des paramatres de configuration */
    vl_ValRet = XZAO82_Lire_Liste_DAI (  EINI_NOM_BASE,
    					  pa_Machine->NomMachine,
    					  ei_config_DAI,
    					  ei_config_Analyseur_DAI,
    					  &vl_NbEqt );
    if ( ( vl_ValRet == XDC_OK ) && ( vl_NbEqt != 0 ) )
    {
       /* Memorisation existance DAI sur la machine */
       strcpy ( pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_DAI)].Equipement, XZECC_TACHE_DAI );
       pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_DAI)].Present = XZEXC_EQT_PRESENT;
       return ( XDC_OK );
    }
    else
    {
       /* Memorisation absence DAI sur la machine */
       strcpy ( pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_DAI)].Equipement, XZECC_TACHE_DAI );
       pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_DAI)].Present = XZEXC_EQT_ABSENT;
       return ( XDC_NOK );
    }
                   
}
