/*E*/
/*Fichier :  $Id: eini_rdt.c,v 1.11 2019/01/07 11:35:41 pc2dpdy Exp $      Release : $Revision: 1.11 $        Date : $Date: 2019/01/07 11:35:41 $ ------------------------------------------------------
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEINI * FICHIER eini_rdt.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module de creation des fichiers de config pour la tache RADT >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1 	26 Sep 1994	: Creation
* Mismer.D 	version 1.2 	14 Dec 1994	:
* Fontaine.C 	version 1.3 	19 Mai 1995 	: Reprise entete, historique
* Volcic.F      version 1.4	18 Sep 1994     : Modification Structure RADT 
* Mismer.D 	version 1.5 	07 Jun 1996	: Suppression champs seuils et alerte (DEM/725)
* Mismer.D 	version 1.6 	21 Oct 1996	: Traitement des stations virtuelles (DEM/1252)
*               version 1.7     ??
* Hilmarcher    version 1.8     10 Mai 2005     : Rajout du champ alerte DEM465 v1.8
* JMG   13/11/07        : ajout site de gestion DEM715
* VR	20/02/12		: ajout terdt (DEM/1016)
* JMG	31/10/18	: IP DEM1306 1.11
------------------------------------------------------*/

/* fichiers inclus */

#include "eini_cfg.h"

/* definitions de constantes */

/* definitions de types locaux */


/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: eini_rdt.c,v 1.11 2019/01/07 11:35:41 pc2dpdy Exp $      : eini_rdt" ;
  
/* declaration de fonctions internes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_config_RADT (	XZAOT_RADT 	va_RADT,
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
*   Ecrire les parametres dans le fichier de config d'un RADT
*   Mettre a jour pour la machine concerne config de la ligne
*
------------------------------------------------------*/
{
XDY_Texte	pl_TexteConf;
int		vl_ValRet = XDC_OK;

   if ( va_NumAppel == 1)
   {
      /* Ecriture de l'entete dans le fichier de config */
      sprintf ( pl_TexteConf, 	"#NumStat\tServeur\tLiaison\tRGS\tAlertes\tDonIndiv\tStatAmontS1\tStatAvalS1\tStatAmontS2\tStatAvalS2\tSiteGestion\tPort\tAdresseIP\tTypeLCR\tIdLCR\n\0" );
      vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_RADT );
   }
   sprintf ( pl_TexteConf, 	" %d\t%s\t%d\t%s\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%s\t%d\t%s\n\0",
   				va_RADT.NumeroStation,
   				va_RADT.NomServeur,
   				va_RADT.Liaison,
   				va_RADT.RGS,
   				va_RADT.Alertes,
				va_RADT.DonneesIndividuelles,
   				va_RADT.NumStationAmontS1,
   				va_RADT.NumStationAvalS1,
   				va_RADT.NumStationAmontS2,
   				va_RADT.NumStationAvalS2,
				va_RADT.SiteGestion,
				va_RADT.Port,
				va_RADT.AdresseIP,
				va_RADT.Type,
				va_RADT.IdLCR);
   XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ec_config_RADT %s", pl_TexteConf );
   /* Ecriture dans le fichier de config */
   vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_RADT );

   /* mise a jour des liaisons series de la machine si pas une station virtuelle */
   if (!strcmp(va_RADT.AdresseIP,XZECC_IP_VIDE)) {
     if ( va_RADT.Liaison != 0 )
        ec_mise_a_jour_config_ligne ( va_RADT.Liaison,
                      		 va_RADT.NomServeur,
                      		 XZECC_PROT_LCR,
                      		 XZECC_SENS_MAITRE,
                      		 XZECC_TYPE_EQT_RDT,
                      		 va_RADT.NumeroStation,0 );
   }
   else {
	ec_mise_a_jour_config_ligne (va_RADT.Port,
				va_RADT.AdresseIP,
				XZECC_PROT_IP,
				XZECC_SENS_MAITRE,
				XZECC_TYPE_EQT_RDT,
				va_RADT.NumeroStation,0 );
   }
   XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ec_config_RADT " );
   return ( vl_ValRet );
   
}  

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_config_PtMesure_RADT (	XZAOT_PtMes 	va_PtMesure,
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
*   Ecrire les parametres dans le fichier de config d'un PtMesure
*
------------------------------------------------------*/
{
XDY_Texte	pl_TexteConf;
int		vl_ValRet = XDC_OK;

   if ( va_NumAppel == 1)
   {
      /* Ecriture de l'entete dans le fichier de config */
      sprintf ( pl_TexteConf, 	"# NumStat\tSens\tBoucle\tBoucleInv\tVoie\tPoids\t\n\0" );
      vl_ValRet = ec_write_fic ( vg_FdConf[1], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_Point_Mesure );
   }
   sprintf ( pl_TexteConf, 	" %d\t%d\t%d\t%d\t%d\t%d\n\0",
   				va_PtMesure.NumeroStation,
   				va_PtMesure.Sens,
   				va_PtMesure.Boucle,
   				va_PtMesure.BoucleInv,
   				va_PtMesure.Voie,
   				va_PtMesure.Poids );
   XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ec_config_PtMesure %s", pl_TexteConf );
   
   /* Ecriture dans le fichier de config */
   vl_ValRet = ec_write_fic ( vg_FdConf[1], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_Point_Mesure );
    
   XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ec_config_PtMesure " );
   return ( vl_ValRet );
   
}  

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_config_Chaussee_RADT (	XZAOT_Chaussee  va_Chaussee,
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
*   Ecrire les parametres dans le fichier de config chaussee RADT
*
------------------------------------------------------*/
{
XDY_Texte	pl_TexteConf;
int		vl_ValRet = XDC_OK;

   if ( va_NumAppel == 1)
   {
      /* Ecriture de l'entete dans le fichier de config */
      sprintf ( pl_TexteConf, 	"# NumStat\tSens\tVoie1\tVoie2\tVoie3\tVoie4\tVoie5\n\0" );
      vl_ValRet = ec_write_fic ( vg_FdConf[2], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_Chaussee );
   }
   sprintf ( pl_TexteConf, 	" %d\t%d\t%d\t%d\t%d\t%d\t%d\n\0",
   				va_Chaussee.NumeroStation,
   				va_Chaussee.Sens,
   				va_Chaussee.SensVoie[0],
   				va_Chaussee.SensVoie[1],
   				va_Chaussee.SensVoie[2],
   				va_Chaussee.SensVoie[3],
   				va_Chaussee.SensVoie[4] );
   XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ei_config_Chaussee_RADT %s", pl_TexteConf );
   
   /* Ecriture dans le fichier de config */
   vl_ValRet = ec_write_fic ( vg_FdConf[2], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_Chaussee );
    
   XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ei_config_Chaussee_RADT " );
   return ( vl_ValRet );
   
}  

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_config_params_terdt ( XZAOT_Terdt  va_terdt,
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
*   Ecrire les parametres dans le fichier de config de TERDT
*  
*
------------------------------------------------------*/
{
XDY_Texte	pl_TexteConf;
int		vl_ValRet = XDC_OK;

   XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ei_config_params_terdt - handle fichier : %d", vg_FdConf[2]  );

   if ( va_NumAppel == 1)
   {
      /* Ecriturni_rdt.ce de l'entete dans le fichier de config */
      sprintf ( pl_TexteConf, 	"# tNum\tType\tNom\tValeur\n\0" );

      XZST_03EcritureTrace ( XZSTC_FONCTION, " ei_config_params_terdt : Creation fichier %s", XDF_Config_Params_TERDT );
      XZST_03EcritureTrace ( XZSTC_FONCTION, " ei_config_params_terdt : Ecriture dans fichier : %s", pl_TexteConf );

      vl_ValRet = ec_write_fic ( vg_FdConf[2], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_Params_TERDT );
   }
   sprintf ( pl_TexteConf, 	" %d\t%d\t%d\t%s\t\n\0",
   				va_terdt.Numero,
   				va_terdt.Type,
   				va_terdt.Valeur,
   				va_terdt.Nom);
   
   /* Ecriture dans le fichier de config */
   vl_ValRet = ec_write_fic ( vg_FdConf[2], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_Params_TERDT );
   
   XZST_03EcritureTrace ( XZSTC_FONCTION, " ei_config_params_terdt : Ecriture dans fichier : %s - vl_ValRet = %d", pl_TexteConf, vl_ValRet );

   XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ei_config_params_terdt " );
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

int	ei_config_eqt_radt ( 	EINI_MACHINE * 	pa_Machine )

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
*   Ecrire les parametres dans le fichier de config d'une station RADT
*   Mettre a jour pour la machine concerne la presence equipement
*
------------------------------------------------------*/
{
XDY_Texte	pl_TexteConf;
XDY_Entier	vl_ValRet = XDC_OK;
XDY_Entier	vl_ValRetReg = XDC_OK;
XDY_Entier	vl_NbEqt  = 0;
XDY_NomMachine 	pl_NomMachine;

    /* Lecture en base de donnees des paramatres de configuration */
    vl_ValRet = XZAO76_Lire_Liste_RADT ( EINI_NOM_BASE,
    					  pa_Machine->NomMachine,
    					  ei_config_RADT,
    					  ei_config_PtMesure_RADT,
    					/*  ei_config_Chaussee_RADT,*/
    					  &vl_NbEqt );

    if ( ( vl_ValRet == XDC_OK ) && ( vl_NbEqt != 0 ) )
    {
       /* Memorisation existance RADT sur la machine */
       strcpy ( pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_RDT)].Equipement, XZECC_TACHE_RDT );
       pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_RDT)].Present = XZEXC_EQT_PRESENT;
    }
    else
    {
       /* Memorisation absence RADT sur la machine */
       strcpy ( pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_RDT)].Equipement, XZECC_TACHE_RDT );
       pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_RDT)].Present = XZEXC_EQT_ABSENT;
	   vl_ValRet = XDC_NOK;
       return ( XDC_NOK );
    }
        XZSC_07NomMachine( pl_NomMachine);
/*	if (!strcmp(pl_NomMachine, pa_Machine->NomMachine)) */
		vl_ValRetReg = XZAO570_Params_Regulation ( XDC_REG_PARAM_TERDT,
					0,
    					  ei_config_params_terdt,
    					  EINI_NOM_BASE);
						  
    return (XDC_OK);
	
}

