/*E*/
/*Fichier :  $Id: eini_nic.c,v 1.8 2011/04/20 09:27:25 gesconf Exp $      Release : $Revision: 1.8 $        Date : $Date: 2011/04/20 09:27:25 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEINI * FICHIER eini_nic.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module de creation des fichiers de config pour la tache GTC Nice >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1 	10 Nov 1994	: Creation
* Mismer.D 	version 1.2 	23 Jan 1995 	:
* Mismer.D	version 1.3 	25 Jan 1995 	:
* Fontaine.C 	version 1.4 	19 Mai 1995 	: Reprise entete, historique
* Mismer.D      version 1.5     24 Avr 1997     : Ajout colonne alerte (DEM/1421)
* Mismer.D      version 1.6     24 Avr 2002     : Suppression GTC Castor&Pollux (DEM/219)
* JBL							Nov 2007		: DEM 664
* JMG		version 1.7	20 aril 2011	: correction traces inutiles 
------------------------------------------------------*/

/* fichiers inclus */

#include "eini_cfg.h"

/* definitions de constantes */

/* definitions de types locaux */


/* definition de macro locales */

/* declaration de variables locales */

static char *version = "@(#)eini_nic.c	1.6 04/24/02 : eini_nic" ;
  
/* declaration de fonctions internes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_config_Tube (	XZAOT_Tube  	va_Tube, 
				XDY_Entier	va_NumAppel )

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
*   Ecrire les parametres d'un tube dans le fichier de config GTC de Nice
*
------------------------------------------------------*/
{

XDY_Texte	pl_TexteConf;
int		vl_ValRet = XDC_OK;

   if ( va_NumAppel == 1)
   {
      /* Ecriture de l'entete dans le fichier de config */
      sprintf ( pl_TexteConf, 	"# NumTube\t@Etat\t@Cmd\t@Dis\t@Ext\t@Lit\t@OC\t@Tel\t@Gab\t@Vit\t@Niche\t\n\0" );
      vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_GTCNice );
   }
   sprintf ( pl_TexteConf, 	"T\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t\n\0",
   				va_Tube.Numero,
   				va_Tube.AdresseEtat,
   				va_Tube.AdresseCommande,
   				va_Tube.AdresseDiscord,
   				va_Tube.AdresseExtincteur,
   				va_Tube.AdresseLit,
   				va_Tube.AdresseOPCO,
   				va_Tube.AdresseTelealarme,
   				va_Tube.AdresseGabarit,
   				va_Tube.AdresseVitesse,
   				va_Tube.AdresseNiche );
   XZST_03EcritureTrace ( XZSTC_INFO, " IN  : ei_config_Tube %s", pl_TexteConf );
   /* Ecriture dans le fichier de config */
   vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_GTCNice );

   XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ei_config_Tube " );
   return ( vl_ValRet );
}  


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_config_Echangeur (	XZAOT_DemiEchangeur	va_Ech, 
				XDY_Entier	  	va_NumAppel )

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
*   Ecrire les parametres dans le fichier de config echangeur GTC Nice
*
------------------------------------------------------*/
{
XDY_Texte	pl_TexteConf;
int		vl_ValRet = XDC_OK;

   if ( va_NumAppel == 1)
   {
      /* Ecriture de l'entete dans le fichier de config */
      sprintf ( pl_TexteConf, 	"#Ech\tNumEch\t@Eta\t@Cmd\t@Dis\t@Tel\n\0" );
      vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_GTCNice );
   }
   sprintf ( pl_TexteConf, 	"E\t%d\t%d\t%d\t%d\t%d\n\0",
   				va_Ech.NumeroDemiEch,
   				va_Ech.AdresseEtat,
   				va_Ech.AdresseCommande,
   				va_Ech.AdresseDiscordance,
   				va_Ech.AdresseTelealarme );

   XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ei_config_Echangeur %s", pl_TexteConf );
   /* Ecriture dans le fichier de config */
   vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf),XDF_Config_GTCNice  );

   XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ei_config_Echangeur " );
   return ( vl_ValRet );
}  


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_config_SeqTube (	XZAOT_SeqTube		va_SeqTube,
				XDY_Entier	  		va_NumAppel )

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
*   Ecrire les parametres sequence tube dans le fichier de config echangeur GTC Nice
*
------------------------------------------------------*/
{
XDY_Texte	pl_TexteConf;
int		vl_ValRet = XDC_OK;

   if ( va_NumAppel == 1)
   {
      /* Ecriture de l'entete dans le fichier de config */
      sprintf ( pl_TexteConf, 	"\n\n#T\tNumEch\tNumSeq\tAdress\tNumBit\tBitDisc\t\n\0" );
      vl_ValRet = ec_write_fic ( vg_FdConf[1], pl_TexteConf, strlen (pl_TexteConf), XDF_Sequence_GTCNice );
   }

   sprintf ( pl_TexteConf, 	"T\t%d\t%d\t%d\t%d\t%d\t\n\0",
   				va_SeqTube.NumTube,
   				va_SeqTube.NumSequence,
   				va_SeqTube.Adresse,
   				va_SeqTube.BitCommande,
   				va_SeqTube.BitDisc );

   XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ei_config_SeqTube %s", pl_TexteConf );
   /* Ecriture dans le fichier de config */
   vl_ValRet = ec_write_fic ( vg_FdConf[1], pl_TexteConf, strlen (pl_TexteConf), XDF_Sequence_GTCNice );

   XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ei_config_SeqTube " );
   return ( vl_ValRet );
}  



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_config_SeqEchangeur (	XZAOT_SeqEch		va_SeqEch,	 
					XDY_Entier	  		va_NumAppel )

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
*   Ecrire les parametres sequence echangeur dans le fichier de config echangeur GTC Nice
*
------------------------------------------------------*/
{
XDY_Texte	pl_TexteConf;
int		vl_ValRet = XDC_OK;

   if ( va_NumAppel == 1)
   {
      /* Ecriture de l'entete dans le fichier de config */
      sprintf ( pl_TexteConf, 	"\n\n#E\tNumEch\tNumSeq\tAdress\tNumBit\tBitDisc\t\n\0" );
      vl_ValRet = ec_write_fic ( vg_FdConf[1], pl_TexteConf, strlen (pl_TexteConf), XDF_Sequence_GTCNice  );
   }
   
   sprintf ( pl_TexteConf, 	"E\t%d\t%d\t%d\t%d\t%d\t\n\0",
   				va_SeqEch.NumDemiEch,
   				va_SeqEch.NumSequence,
   				va_SeqEch.Adresse,
   				va_SeqEch.BitCommande,
   				va_SeqEch.BitDisc );

   XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ei_config_SeqEchangeur %s", pl_TexteConf );
   /* Ecriture dans le fichier de config */
   vl_ValRet = ec_write_fic ( vg_FdConf[1], pl_TexteConf, strlen (pl_TexteConf), XDF_Sequence_GTCNice );

   XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ei_config_SeqEchangeur " );
   return ( vl_ValRet );
}  


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_config_CapteurTube (	XZAOT_CapteurTube	va_Capteur, 
				XDY_Entier			va_NumAppel )

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
*   Ecrire les parametres capteur GTC Nice dans le fichier de config XDF_Capt_Pan_GTCNice
*
------------------------------------------------------*/
{
XDY_Texte	pl_TexteConf;
int		vl_ValRet = XDC_OK;

   if ( va_NumAppel == 1)
   {
      /* Ecriture de l'entete dans le fichier de config */
      sprintf ( pl_TexteConf, 	"#C\tNumTube\tNumCapt\tType\tNumBit\tNumAlerte\t\n\0" );
      vl_ValRet = ec_write_fic ( vg_FdConf[2], pl_TexteConf, strlen (pl_TexteConf), XDF_Capt_Pan_GTCNice );
   }
   sprintf ( pl_TexteConf, 	"C\t%d\t%d\t%d\t%d\t%d\n\0",
   				va_Capteur.NumeroTube,
				va_Capteur.NumeroCapteur,
   				va_Capteur.Type,
   				va_Capteur.NumBit,
   				va_Capteur.NumAlerte );
   XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ei_config_CapteurTube %s", pl_TexteConf );
   /* Ecriture dans le fichier de config */
   vl_ValRet = ec_write_fic ( vg_FdConf[2], pl_TexteConf, strlen (pl_TexteConf), XDF_Capt_Pan_GTCNice );

   XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ei_config_CapteurTube " );
   return ( vl_ValRet );
}  
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_config_FAV	 (	XZAOT_FAVTube	va_FAV, 
				XDY_Entier		va_NumAppel )

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
*   Ecrire les parametres FAV dans le fichier de config XDF_Capt_Pan_GTCNice
*
------------------------------------------------------*/
{
XDY_Texte	pl_TexteConf;
int		vl_ValRet = XDC_OK;

   if ( va_NumAppel == 1)
   {
      /* Ecriture de l'entete dans le fichier de config */
      sprintf ( pl_TexteConf, 	"#F\tNumTube\tNumPn\tCom1\tCom2\tEta1\tEta2\tDis1\tDis2\t\n\0" );
      vl_ValRet = ec_write_fic ( vg_FdConf[2], pl_TexteConf, strlen (pl_TexteConf), XDF_Capt_Pan_GTCNice );
   }
   sprintf ( pl_TexteConf, 	"F\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n\0",
   				va_FAV.NumeroTube,
   				va_FAV.NumeroFAV,
   				va_FAV.RangComm,
   				va_FAV.RangComm2,
   				va_FAV.RangEtat,
   				va_FAV.RangEtat2,
   				va_FAV.BitDiscordance,
   				va_FAV.BitDiscordance2 );

   XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ei_config_FAV %s", pl_TexteConf );
   /* Ecriture dans le fichier de config */
   vl_ValRet = ec_write_fic ( vg_FdConf[2], pl_TexteConf, strlen (pl_TexteConf), XDF_Capt_Pan_GTCNice );

   XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ei_config_FAV " );
   return ( vl_ValRet );
}  



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_config_Panneau (	XZAOT_PnGTC	va_Panneau, 
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
*   Ecrire les parametres panneau dans le fichier de config XDF_Capt_Pan_GTCNice
*
------------------------------------------------------*/
{
XDY_Texte	pl_TexteConf;
int		vl_ValRet = XDC_OK;

   if ( va_NumAppel == 1)
   {
      /* Ecriture de l'entete dans le fichier de config */
      sprintf ( pl_TexteConf, 	"#P\tNumMait\tTypeMait\tNumPn\tCom1\tCom2\tEta1\tEta2\tDis1\tDis2\t\n\0" );
      vl_ValRet = ec_write_fic ( vg_FdConf[2], pl_TexteConf, strlen (pl_TexteConf), XDF_Capt_Pan_GTCNice );
   }
   sprintf ( pl_TexteConf, 	"P\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n\0",
   				va_Panneau.NumMaitre,
   				va_Panneau.TypeMaitre,
   				va_Panneau.NumeroPnGTC,
   				va_Panneau.RangComm,
   				va_Panneau.RangComm2,
   				va_Panneau.RangEtat,
   				va_Panneau.RangEtat2,
   				va_Panneau.BitDiscordance,
   				va_Panneau.BitDiscordance2 );

   XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ei_config_Panneau %s", pl_TexteConf );
   /* Ecriture dans le fichier de config */
   vl_ValRet = ec_write_fic ( vg_FdConf[2], pl_TexteConf, strlen (pl_TexteConf), XDF_Capt_Pan_GTCNice );

   XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ei_config_Panneau " );
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

int	ei_config_eqt_GTC_NICE ( 	EINI_MACHINE * 	pa_Machine )

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
*   Ecrire les parametres dans le fichier de config d'un systeme video
*   Mettre a jour pour la machine concerne la presence equipement
*
------------------------------------------------------*/
{
XDY_Texte	pl_TexteConf;
XZAOT_GTC_NICE	vl_GTCNice;
int		vl_ValRet = XDC_OK;

		
	XZST_03EcritureTrace( XZSTC_FONCTION, "ei_config_eqt_GTC_NICE : Configuration de la GTC Nice " );
    /* Lecture en base de donnees des paramatres de configuration */
    memset ( pl_TexteConf, ' ', sizeof(XDY_Texte)-2);
    sprintf ( pl_TexteConf+sizeof(XDY_Texte)-4, "\n\0");
    write ( vg_FdConf[0], pl_TexteConf, strlen(pl_TexteConf) );
    vl_ValRet =  XZAO203_Liste_Eqt_GTC_NICE ( EINI_NOM_BASE,
    					    pa_Machine->NomMachine, 
    					    ei_config_Tube,
    					    ei_config_Echangeur,
    					    ei_config_SeqTube,
    					    ei_config_SeqEchangeur,
    					    ei_config_CapteurTube,
     					    ei_config_FAV,
     					    ei_config_Panneau,
   					    &vl_GTCNice );
    if ( ( vl_ValRet == XDC_OK ) && ( vl_GTCNice.Numero != 0 ) )
    {
        sprintf ( pl_TexteConf, "G %d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t\n#\0",
			vl_GTCNice.Numero,
			vl_GTCNice.NomServeur,
			vl_GTCNice.LongueurITOR,
			vl_GTCNice.AdresseITOR,
			vl_GTCNice.LongueurETOR,
			vl_GTCNice.AdresseETOR,
			vl_GTCNice.LongTelealarme,
			vl_GTCNice.AdresseTelealarme,
			vl_GTCNice.LongTelecomEch,
			vl_GTCNice.AdresseTelecomEch,
			vl_GTCNice.LongTelecomTub,
			vl_GTCNice.AdresseTelecomTub,
			vl_GTCNice.LongTelesignEch,
			vl_GTCNice.AdresseTelesignEch,
			vl_GTCNice.LongTelesignTub,
			vl_GTCNice.AdresseTelesignTub,
			vl_GTCNice.LongSeqEch,
			vl_GTCNice.AdresseSeqEch,
			vl_GTCNice.LongSeqTub,
			vl_GTCNice.AdresseSeqTub,
			vl_GTCNice.LongAlerteTub,
			vl_GTCNice.AdresseAlerteTub );
       /* Ecriture dans le fichier de config */
       lseek ( vg_FdConf[0], 0, SEEK_SET);
       write ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf) );
    
       /* Memorisation existance GTC Nice sur la machine */
       strcpy ( pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_NIC)].Equipement, XZECC_TACHE_NIC );
       pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_NIC)].Present = XZEXC_EQT_PRESENT;
              
	   XZST_03EcritureTrace( XZSTC_FONCTION, "ei_config_eqt_GTC_NICE : Fin Configuration de la GTC Nice (OK) " );
       return ( XDC_OK );
    }
    else
    {
       /* Memorisation absence GTC Nice sur la machine */
       strcpy ( pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_NIC)].Equipement, XZECC_TACHE_NIC );
       pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_NIC)].Present = XZEXC_EQT_ABSENT;
	   XZST_03EcritureTrace( XZSTC_WARNING, "ei_config_eqt_GTC_NICE : Fin Configuration de la GTC Nice (erreur)" );
       return ( XDC_NOK );
    }

}

/* AAZ 17/10/2007 */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Generation du fichier de parametres pour les ouvrages
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int    ei_config_OUV ( XZAOT_OUV    va_OUV, 
                       XDY_Entier   va_NumAppel )

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
*   Ecrire les parametres dans le fichier de config ouvrage
*   GTC Nice
*
------------------------------------------------------*/
{
    XDY_Texte    pl_TexteConf;
    int          vl_ValRet = XDC_OK;

	return XDC_OK;
	
}  

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Generation du fichier de parametres pour le domaine Signalisation
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int    ei_config_SIG ( XZAOT_DOM    va_DOM, 
                       XDY_Entier   va_NumAppel )

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
*   Ecrire les parametres dans le fichier de config domaine Signalisation
*   GTC Nice
*
------------------------------------------------------*/
{
    XDY_Texte    pl_TexteConf;
    int          vl_ValRet = XDC_OK;

    if ( va_NumAppel == 1)
    {
        /* Ecriture de l'entete dans le fichier de config */
        sprintf ( pl_TexteConf,     "#SIG\tNumero\tType\tEtat\n\0" );
        vl_ValRet = ec_write_fic ( vg_FdConf[5], pl_TexteConf, strlen (pl_TexteConf), XDF_CONF_SIG );
    }
    sprintf ( pl_TexteConf,     "%s\t%d\t%d\t%d\n\0",
              va_DOM.Domaine,
              va_DOM.Numero,
              va_DOM.Type,
              va_DOM.Etat );

    XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ei_config_SIG %s", pl_TexteConf );
    /* Ecriture dans le fichier de config */
    vl_ValRet = ec_write_fic ( vg_FdConf[5], pl_TexteConf, strlen (pl_TexteConf),XDF_CONF_SIG  );

    XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ei_config_SIG " );
    return ( vl_ValRet );
}  

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Generation du fichier de parametres pour le domaine Eclairage
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int    ei_config_ECL ( XZAOT_DOM    va_DOM, 
                       XDY_Entier   va_NumAppel )

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
*   Ecrire les parametres dans le fichier de config domaine Eclairage
*   GTC Nice
*
------------------------------------------------------*/
{
    XDY_Texte    pl_TexteConf;
    int          vl_ValRet = XDC_OK;

    if ( va_NumAppel == 1)
    {
        /* Ecriture de l'entete dans le fichier de config */
        sprintf ( pl_TexteConf,     "#ECL\tNumero\tType\tEtat\n\0" );
        vl_ValRet = ec_write_fic ( vg_FdConf[5], pl_TexteConf, strlen (pl_TexteConf), XDF_CONF_ECL );
    }
    sprintf ( pl_TexteConf,     "%s\t%d\t%d\t%d\n\0",
              va_DOM.Domaine,
              va_DOM.Numero,
              va_DOM.Type,
              va_DOM.Etat );

   XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ei_config_ECL %s", pl_TexteConf );
   /* Ecriture dans le fichier de config */
   vl_ValRet = ec_write_fic ( vg_FdConf[5], pl_TexteConf, strlen (pl_TexteConf),XDF_CONF_ECL  );

   XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ei_config_ECL " );
   return ( vl_ValRet );
}  

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Generation du fichier de parametres pour le domaine Ventilation
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int    ei_config_VEN ( XZAOT_DOM    va_DOM, 
                       XDY_Entier   va_NumAppel )

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
*   Ecrire les parametres dans le fichier de config domaine Ventilation
*   GTC Nice
*
------------------------------------------------------*/
{
    XDY_Texte    pl_TexteConf;
    int          vl_ValRet = XDC_OK;

    if ( va_NumAppel == 1)
    {
        /* Ecriture de l'entete dans le fichier de config */
        sprintf ( pl_TexteConf,     "#VEN\tNumero\tType\tEtat\n\0" );
        vl_ValRet = ec_write_fic ( vg_FdConf[5], pl_TexteConf, strlen (pl_TexteConf), XDF_CONF_VEN );
    }
    sprintf ( pl_TexteConf,     "%s\t%d\t%d\t%d\n\0",
              va_DOM.Domaine,
              va_DOM.Numero,
              va_DOM.Type,
              va_DOM.Etat );

    XZST_03EcritureTrace ( XZSTC_INTERFACE, " IN  : ei_config_VEN '%s' ", pl_TexteConf );
    /* Ecriture dans le fichier de config */
    vl_ValRet = ec_write_fic ( vg_FdConf[5], pl_TexteConf, strlen (pl_TexteConf),XDF_CONF_VEN  );

    XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ei_config_VEN " );
    return ( vl_ValRet );
}  

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Generation du fichier de parametres pour le domaine Energie
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int    ei_config_ENR ( XZAOT_DOM    va_DOM, 
                       XDY_Entier   va_NumAppel )

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
*   Ecrire les parametres dans le fichier de config domaine Energie
*   GTC Nice
*
------------------------------------------------------*/
{
    XDY_Texte    pl_TexteConf;
    int          vl_ValRet = XDC_OK;

    if ( va_NumAppel == 1)
    {
        /* Ecriture de l'entete dans le fichier de config */
        sprintf ( pl_TexteConf,     "#ENR\tNumero\tType\tEtat\n\0" );
        vl_ValRet = ec_write_fic ( vg_FdConf[5], pl_TexteConf, strlen (pl_TexteConf), XDF_CONF_ENR );
    }
    sprintf ( pl_TexteConf,     "%s\t%d\t%d\t%d\n\0",
              va_DOM.Domaine,
              va_DOM.Numero,
              va_DOM.Type,
              va_DOM.Etat );

   XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ei_config_ENR %s", pl_TexteConf );
   /* Ecriture dans le fichier de config */
   vl_ValRet = ec_write_fic ( vg_FdConf[5], pl_TexteConf, strlen (pl_TexteConf),XDF_CONF_ENR  );

   XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ei_config_ENR " );
   return ( vl_ValRet );
}  

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Generation du fichier de parametres pour le domaine Protection incendie
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int    ei_config_INC ( XZAOT_DOM    va_DOM, 
                       XDY_Entier   va_NumAppel )

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
*   Ecrire les parametres dans le fichier de config domaine Protection incendie
*   GTC Nice
*
------------------------------------------------------*/
{
    XDY_Texte    pl_TexteConf;
    int          vl_ValRet = XDC_OK;

    if ( va_NumAppel == 1)
    {
        /* Ecriture de l'entete dans le fichier de config */
        sprintf ( pl_TexteConf,     "#INC\tNumero\tType\tEtat\n\0" );
        vl_ValRet = ec_write_fic ( vg_FdConf[5], pl_TexteConf, strlen (pl_TexteConf), XDF_CONF_INC );
    }
    sprintf ( pl_TexteConf,     "%s\t%d\t%d\t%d\n\0",
              va_DOM.Domaine,
              va_DOM.Numero,
              va_DOM.Type,
              va_DOM.Etat );

    XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ei_config_INC %s", pl_TexteConf );
    /* Ecriture dans le fichier de config */
    vl_ValRet = ec_write_fic ( vg_FdConf[5], pl_TexteConf, strlen (pl_TexteConf),XDF_CONF_INC  );

    XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ei_config_INC " );
    return ( vl_ValRet );
}  

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Generation du fichier de parametres pour le domaine Exploitation
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int    ei_config_EXP ( XZAOT_DOM    va_DOM, 
                       XDY_Entier   va_NumAppel )

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
*   Ecrire les parametres dans le fichier de config domaine Exploitation
*   GTC Nice
*
------------------------------------------------------*/
{
    XDY_Texte    pl_TexteConf;
    int          vl_ValRet = XDC_OK;

    if ( va_NumAppel == 1)
    {
        /* Ecriture de l'entete dans le fichier de config */
        sprintf ( pl_TexteConf,     "#EXP\tNumero\tType\tEtat\n\0" );
        vl_ValRet = ec_write_fic ( vg_FdConf[5], pl_TexteConf, strlen (pl_TexteConf), XDF_CONF_EXP );
    }
    sprintf ( pl_TexteConf,     "%s\t%d\t%d\t%d\n\0",
              va_DOM.Domaine,
              va_DOM.Numero,
              va_DOM.Type,
              va_DOM.Etat );

    XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ei_config_EXP %s", pl_TexteConf );
    /* Ecriture dans le fichier de config */
    vl_ValRet = ec_write_fic ( vg_FdConf[5], pl_TexteConf, strlen (pl_TexteConf),XDF_CONF_EXP  );

    XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ei_config_EXP " );
    return ( vl_ValRet );
}  

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Generation du fichier de parametres pour le domaine Systeme
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int    ei_config_SYS ( XZAOT_DOM    va_DOM, 
                       XDY_Entier   va_NumAppel )

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
*   Ecrire les parametres dans le fichier de config domaine Systeme
*   GTC Nice
*
------------------------------------------------------*/
{
    XDY_Texte    pl_TexteConf;
    int          vl_ValRet = XDC_OK;

    if ( va_NumAppel == 1)
    {
        /* Ecriture de l'entete dans le fichier de config */
        sprintf ( pl_TexteConf,     "#SYS\tNumero\tType\tEtat\n\0" );
        vl_ValRet = ec_write_fic ( vg_FdConf[5], pl_TexteConf, strlen (pl_TexteConf), XDF_CONF_SYS );
    }
    sprintf ( pl_TexteConf,     "%s\t%d\t%d\t%d\n\0",
              va_DOM.Domaine,
              va_DOM.Numero,
              va_DOM.Type,
              va_DOM.Etat );

    XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ei_config_SYS %s", pl_TexteConf );
    /* Ecriture dans le fichier de config */
    vl_ValRet = ec_write_fic ( vg_FdConf[5], pl_TexteConf, strlen (pl_TexteConf),XDF_CONF_SYS  );

    XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ei_config_SYS " );
    return ( vl_ValRet );
}  

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Generation du fichier de parametres pour le domaine Locaux techniques
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int    ei_config_LTN ( XZAOT_DOM    va_DOM, 
                       XDY_Entier   va_NumAppel )

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
*   Ecrire les parametres dans le fichier de config domaine Locaux techniques
*   GTC Nice
*
------------------------------------------------------*/
{
    XDY_Texte    pl_TexteConf;
    int          vl_ValRet = XDC_OK;

    if ( va_NumAppel == 1)
    {
        /* Ecriture de l'entete dans le fichier de config */
        sprintf ( pl_TexteConf,     "#LTN\tNumero\tType\tEtat\n\0" );
        vl_ValRet = ec_write_fic ( vg_FdConf[5], pl_TexteConf, strlen (pl_TexteConf), XDF_CONF_LTN );
    }
    sprintf ( pl_TexteConf,     "%s\t%d\t%d\t%d\n\0",
              va_DOM.Domaine,
              va_DOM.Numero,
              va_DOM.Type,
              va_DOM.Etat );

    XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ei_config_LTN %s", pl_TexteConf );
    /* Ecriture dans le fichier de config */
    vl_ValRet = ec_write_fic ( vg_FdConf[5], pl_TexteConf, strlen (pl_TexteConf),XDF_CONF_LTN  );

    XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ei_config_LTN " );
    return ( vl_ValRet );
}  


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Generation du fichier de parametres pour les equipements du domaine Signalisation
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int    ei_config_EQTSIG ( XZAOT_EQT    va_EQT, 
                          XDY_Entier   va_NumAppel )

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
*   Ecrire les parametres dans le fichier de config equipements domaine Signalisation
*   GTC Nice
*
------------------------------------------------------*/
{
    XDY_Texte    pl_TexteConf;
    int          vl_ValRet = XDC_OK;

    if ( va_NumAppel == 1)
    {
        /* Ecriture de l'entete dans le fichier de config */
        sprintf ( pl_TexteConf,     "#EQT\tTypeMaitre\tMaitre\tType\tNumero\tDomaine\n\0" );
        vl_ValRet = ec_write_fic ( vg_FdConf[4], pl_TexteConf, strlen (pl_TexteConf), XDF_CONF_EQT_SIG );
    }
    sprintf ( pl_TexteConf,     "%s\t%d\t%d\t%d\t%d\t%s\n\0",
              va_EQT.EQT,
              va_EQT.TypeMaitre,
              va_EQT.Maitre,
              va_EQT.Type,
              va_EQT.Numero,
              va_EQT.Domaine );

    XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ei_config_EQTSIG %s", pl_TexteConf );
    /* Ecriture dans le fichier de config */
    vl_ValRet = ec_write_fic ( vg_FdConf[4], pl_TexteConf, strlen (pl_TexteConf),XDF_CONF_EQT_SIG  );

    XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ei_config_EQTSIG " );
    return ( vl_ValRet );
}  


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Generation du fichier de parametres pour les equipements du domaine Eclairage
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int    ei_config_EQTECL ( XZAOT_EQT    va_EQT, 
                          XDY_Entier   va_NumAppel )

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
*   Ecrire les parametres dans le fichier de config equipements domaine Eclairage
*   GTC Nice
*
------------------------------------------------------*/
{
    XDY_Texte    pl_TexteConf;
    int          vl_ValRet = XDC_OK;

    if ( va_NumAppel == 1)
    {
        /* Ecriture de l'entete dans le fichier de config */
        sprintf ( pl_TexteConf,     "#EQT\tTypeMaitre\tMaitre\tType\tNumero\tDomaine\n\0" );
        vl_ValRet = ec_write_fic ( vg_FdConf[4], pl_TexteConf, strlen (pl_TexteConf), XDF_CONF_EQT_ECL );
    }
    sprintf ( pl_TexteConf,     "%s\t%d\t%d\t%d\t%d\t%s\n\0",
              va_EQT.EQT,
              va_EQT.TypeMaitre,
              va_EQT.Maitre,
              va_EQT.Type,
              va_EQT.Numero,
              va_EQT.Domaine );

    XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ei_config_EQTECL %s", pl_TexteConf );
    /* Ecriture dans le fichier de config */
    vl_ValRet = ec_write_fic ( vg_FdConf[4], pl_TexteConf, strlen (pl_TexteConf),XDF_CONF_EQT_ECL  );

    XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ei_config_EQTECL " );
    return ( vl_ValRet );
}  


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Generation du fichier de parametres pour les equipements du domaine Ventilation
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int    ei_config_EQTVEN ( XZAOT_EQT    va_EQT, 
                          XDY_Entier   va_NumAppel )

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
*   Ecrire les parametres dans le fichier de config equipements domaine Ventilation
*   GTC Nice
*
------------------------------------------------------*/
{
    XDY_Texte    pl_TexteConf;
    int          vl_ValRet = XDC_OK;

    if ( va_NumAppel == 1)
    {
        /* Ecriture de l'entete dans le fichier de config */
        sprintf ( pl_TexteConf,     "#EQT\tTypeMaitre\tMaitre\tType\tNumero\tDomaine\n\0" );
        vl_ValRet = ec_write_fic ( vg_FdConf[4], pl_TexteConf, strlen (pl_TexteConf), XDF_CONF_EQT_VEN );
    }
    sprintf ( pl_TexteConf,     "%s\t%d\t%d\t%d\t%d\t%s\n\0",
              va_EQT.EQT,
              va_EQT.TypeMaitre,
              va_EQT.Maitre,
              va_EQT.Type,
              va_EQT.Numero,
              va_EQT.Domaine );

    XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ei_config_EQTVEN %s", pl_TexteConf );
    /* Ecriture dans le fichier de config */
    vl_ValRet = ec_write_fic ( vg_FdConf[4], pl_TexteConf, strlen (pl_TexteConf),XDF_CONF_EQT_VEN  );

    XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ei_config_EQTVEN " );
    return ( vl_ValRet );
}  


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Generation du fichier de parametres pour les equipements du domaine Energie
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int    ei_config_EQTENR ( XZAOT_EQT    va_EQT, 
                          XDY_Entier   va_NumAppel )

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
*   Ecrire les parametres dans le fichier de config equipements domaine Energie
*   GTC Nice
*
------------------------------------------------------*/
{
    XDY_Texte    pl_TexteConf;
    int          vl_ValRet = XDC_OK;

    if ( va_NumAppel == 1)
    {
        /* Ecriture de l'entete dans le fichier de config */
        sprintf ( pl_TexteConf,     "#EQT\tTypeMaitre\tMaitre\tType\tNumero\tDomaine\n\0" );
        vl_ValRet = ec_write_fic ( vg_FdConf[4], pl_TexteConf, strlen (pl_TexteConf), XDF_CONF_EQT_ENR );
    }
    sprintf ( pl_TexteConf,     "%s\t%d\t%d\t%d\t%d\t%s\n\0",
              va_EQT.EQT,
              va_EQT.TypeMaitre,
              va_EQT.Maitre,
              va_EQT.Type,
              va_EQT.Numero,
              va_EQT.Domaine );

    XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ei_config_EQTENR %s", pl_TexteConf );
    /* Ecriture dans le fichier de config */
    vl_ValRet = ec_write_fic ( vg_FdConf[4], pl_TexteConf, strlen (pl_TexteConf),XDF_CONF_EQT_ENR  );

    XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ei_config_EQTENR " );
    return ( vl_ValRet );
}  


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Generation du fichier de parametres pour les equipements du domaine Protection Incendie
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int    ei_config_EQTINC ( XZAOT_EQT    va_EQT, 
                          XDY_Entier   va_NumAppel )

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
*   Ecrire les parametres dans le fichier de config equipements domaine Protection Incendie
*   GTC Nice
*
------------------------------------------------------*/
{
    XDY_Texte    pl_TexteConf;
    int          vl_ValRet = XDC_OK;

    if ( va_NumAppel == 1)
    {
        /* Ecriture de l'entete dans le fichier de config */
        sprintf ( pl_TexteConf,     "#EQT\tTypeMaitre\tMaitre\tType\tNumero\tDomaine\n\0" );
        vl_ValRet = ec_write_fic ( vg_FdConf[4], pl_TexteConf, strlen (pl_TexteConf), XDF_CONF_EQT_INC );
    }
    sprintf ( pl_TexteConf,     "%s\t%d\t%d\t%d\t%d\t%s\n\0",
              va_EQT.EQT,
              va_EQT.TypeMaitre,
              va_EQT.Maitre,
              va_EQT.Type,
              va_EQT.Numero,
              va_EQT.Domaine );

    XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ei_config_EQTINC %s", pl_TexteConf );
    /* Ecriture dans le fichier de config */
    vl_ValRet = ec_write_fic ( vg_FdConf[4], pl_TexteConf, strlen (pl_TexteConf),XDF_CONF_EQT_INC  );

    XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ei_config_EQTINC " );
    return ( vl_ValRet );
}  


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Generation du fichier de parametres pour les equipements du domaine Exploitation
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int    ei_config_EQTEXP ( XZAOT_EQT    va_EQT, 
                          XDY_Entier   va_NumAppel )

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
*   Ecrire les parametres dans le fichier de config equipements domaine Exploitation
*   GTC Nice
*
------------------------------------------------------*/
{
    XDY_Texte    pl_TexteConf;
    int          vl_ValRet = XDC_OK;

    if ( va_NumAppel == 1)
    {
        /* Ecriture de l'entete dans le fichier de config */
        sprintf ( pl_TexteConf,     "#EQT\tTypeMaitre\tMaitre\tType\tNumero\tDomaine\n\0" );
        vl_ValRet = ec_write_fic ( vg_FdConf[4], pl_TexteConf, strlen (pl_TexteConf), XDF_CONF_EQT_EXP );
    }
    sprintf ( pl_TexteConf,     "%s\t%d\t%d\t%d\t%d\t%s\n\0",
              va_EQT.EQT,
              va_EQT.TypeMaitre,
              va_EQT.Maitre,
              va_EQT.Type,
              va_EQT.Numero,
              va_EQT.Domaine );

    XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ei_config_EQTEXP %s", pl_TexteConf );
    /* Ecriture dans le fichier de config */
    vl_ValRet = ec_write_fic ( vg_FdConf[4], pl_TexteConf, strlen (pl_TexteConf),XDF_CONF_EQT_EXP  );

    XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ei_config_EQTEXP " );
    return ( vl_ValRet );
}  


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Generation du fichier de parametres pour les equipements du domaine Systeme
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int    ei_config_EQTSYS ( XZAOT_EQT    va_EQT, 
                          XDY_Entier   va_NumAppel )

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
*   Ecrire les parametres dans le fichier de config equipements domaine Systeme
*   GTC Nice
*
------------------------------------------------------*/
{
    XDY_Texte    pl_TexteConf;
    int          vl_ValRet = XDC_OK;

    if ( va_NumAppel == 1)
    {
        /* Ecriture de l'entete dans le fichier de config */
        sprintf ( pl_TexteConf,     "#EQT\tTypeMaitre\tMaitre\tType\tNumero\tDomaine\n\0" );
        vl_ValRet = ec_write_fic ( vg_FdConf[4], pl_TexteConf, strlen (pl_TexteConf), XDF_CONF_EQT_SYS );
    }
    sprintf ( pl_TexteConf,     "%s\t%d\t%d\t%d\t%d\t%s\n\0",
              va_EQT.EQT,
              va_EQT.TypeMaitre,
              va_EQT.Maitre,
              va_EQT.Type,
              va_EQT.Numero,
              va_EQT.Domaine );

    XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ei_config_EQTSYS %s", pl_TexteConf );
    /* Ecriture dans le fichier de config */
    vl_ValRet = ec_write_fic ( vg_FdConf[4], pl_TexteConf, strlen (pl_TexteConf),XDF_CONF_EQT_SYS  );

    XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ei_config_EQTSYS " );
    return ( vl_ValRet );
}  


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Generation du fichier de parametres pour les equipements du domaine Locaux Techniques
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int    ei_config_EQTLTN ( XZAOT_EQT    va_EQT, 
                          XDY_Entier   va_NumAppel )

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
*   Ecrire les parametres dans le fichier de config equipements domaine Locaux Techniques
*   GTC Nice
*
------------------------------------------------------*/
{
    XDY_Texte    pl_TexteConf;
    int          vl_ValRet = XDC_OK;

    if ( va_NumAppel == 1)
    {
        /* Ecriture de l'entete dans le fichier de config */
        sprintf ( pl_TexteConf,     "#EQT\tTypeMaitre\tMaitre\tType\tNumero\tDomaine\n\0" );
        vl_ValRet = ec_write_fic ( vg_FdConf[4], pl_TexteConf, strlen (pl_TexteConf), XDF_CONF_EQT_LTN );
    }
    sprintf ( pl_TexteConf,     "%s\t%d\t%d\t%d\t%d\t%s\n\0",
              va_EQT.EQT,
              va_EQT.TypeMaitre,
              va_EQT.Maitre,
              va_EQT.Type,
              va_EQT.Numero,
              va_EQT.Domaine );

    XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ei_config_EQTLTN %s", pl_TexteConf );
    /* Ecriture dans le fichier de config */
    vl_ValRet = ec_write_fic ( vg_FdConf[4], pl_TexteConf, strlen (pl_TexteConf),XDF_CONF_EQT_LTN  );

    XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ei_config_EQTLTN " );
    return ( vl_ValRet );
}

/* AZAZAZAZA */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int    ei_config_SeqSIG ( XZAOT_SEQ    va_SEQ,     
                          XDY_Entier   va_NumAppel )

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
*   Ecrire les parametres sequence dans le fichier de config 
*   sequence du domaine Signalisation GTC Nice Renovee
*
------------------------------------------------------*/
{
    XDY_Texte  pl_TexteConf;
    int        vl_ValRet = XDC_OK;
/*
	XDY_NomEqt	Domaine;
	XDY_Octet   TypeEquipement;
	XDY_Eqt		Numero;
	XDY_Octet	NumSequence;
*/
   XZST_03EcritureTrace ( XZSTC_INTERFACE, " IN  : ei_config_SeqSIG, va_NumAppel = %d, fichier : %s", va_NumAppel, XDF_CONF_SEQ_SIG);
    if ( va_NumAppel == 1)
    {
        /* Ecriture de l'entete dans le fichier de config */
        sprintf ( pl_TexteConf,     "\n\n#Domaine\tTypeEqt\tNumero\tNumSeq\n\0" );
        vl_ValRet = ec_write_fic ( vg_FdConf[3], pl_TexteConf, strlen (pl_TexteConf), XDF_CONF_SEQ_SIG  );
    }
   
   sprintf ( pl_TexteConf,     "%s\t%d\t%d\t%d\n\0",
                   va_SEQ.Domaine,
                   va_SEQ.TypeEquipement,
                   va_SEQ.Numero,
                   va_SEQ.NumSequence );

   XZST_03EcritureTrace ( XZSTC_DEBUG1, " ei_config_SeqSIG Ecriture ligne '%s'", pl_TexteConf );
   /* Ecriture dans le fichier de config */
   vl_ValRet = ec_write_fic ( vg_FdConf[3], pl_TexteConf, strlen (pl_TexteConf), XDF_CONF_SEQ_SIG );

   XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ei_config_SeqSIG " );
   return ( vl_ValRet );
}  

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int    ei_config_SeqECL ( XZAOT_SEQ    va_SEQ,     
                          XDY_Entier   va_NumAppel )

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
*   Ecrire les parametres sequence dans le fichier de config 
*   sequence du domaine Eclairage GTC Nice Renovee
*
------------------------------------------------------*/
{
    XDY_Texte  pl_TexteConf;
    int        vl_ValRet = XDC_OK;
    if ( va_NumAppel == 1)
    {
        /* Ecriture de l'entete dans le fichier de config */
        sprintf ( pl_TexteConf,     "\n\n#Domaine\tTypeEqt\tNumero\tNumSeq\n\0" );
        vl_ValRet = ec_write_fic ( vg_FdConf[3], pl_TexteConf, strlen (pl_TexteConf), XDF_CONF_SEQ_ECL  );
    }
   
   sprintf ( pl_TexteConf,     "%s\t%d\t%d\t%d\n\0",
                   va_SEQ.Domaine,
                   va_SEQ.TypeEquipement,
                   va_SEQ.Numero,
                   va_SEQ.NumSequence );

   XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ei_config_SeqECL %s", pl_TexteConf );
   /* Ecriture dans le fichier de config */
   vl_ValRet = ec_write_fic ( vg_FdConf[3], pl_TexteConf, strlen (pl_TexteConf), XDF_CONF_SEQ_ECL );

   XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ei_config_SeqECL " );
   return ( vl_ValRet );
}  

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int    ei_config_SeqVEN ( XZAOT_SEQ    va_SEQ,     
                          XDY_Entier   va_NumAppel )

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
*   Ecrire les parametres sequence dans le fichier de config 
*   sequence du domaine Ventilation GTC Nice Renovee
*
------------------------------------------------------*/
{
    XDY_Texte  pl_TexteConf;
    int        vl_ValRet = XDC_OK;
    if ( va_NumAppel == 1)
    {
        /* Ecriture de l'entete dans le fichier de config */
        sprintf ( pl_TexteConf,     "\n\n#Domaine\tTypeEqt\tNumero\tNumSeq\n\0" );
        vl_ValRet = ec_write_fic ( vg_FdConf[3], pl_TexteConf, strlen (pl_TexteConf), XDF_CONF_SEQ_VEN  );
    }
   
   sprintf ( pl_TexteConf,     "%s\t%d\t%d\t%d\n\0",
                   va_SEQ.Domaine,
                   va_SEQ.TypeEquipement,
                   va_SEQ.Numero,
                   va_SEQ.NumSequence );

   XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ei_config_SeqVEN %s", pl_TexteConf );
   /* Ecriture dans le fichier de config */
   vl_ValRet = ec_write_fic ( vg_FdConf[3], pl_TexteConf, strlen (pl_TexteConf), XDF_CONF_SEQ_VEN );

   XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ei_config_SeqVEN " );
   return ( vl_ValRet );
}  

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int    ei_config_SeqENR ( XZAOT_SEQ    va_SEQ,     
                          XDY_Entier   va_NumAppel )

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
*   Ecrire les parametres sequence dans le fichier de config 
*   sequence du domaine Energie GTC Nice Renovee
*
------------------------------------------------------*/
{
    XDY_Texte  pl_TexteConf;
    int        vl_ValRet = XDC_OK;
    if ( va_NumAppel == 1)
    {
        /* Ecriture de l'entete dans le fichier de config */
        sprintf ( pl_TexteConf,     "\n\n#Domaine\tTypeEqt\tNumero\tNumSeq\n\0" );
        vl_ValRet = ec_write_fic ( vg_FdConf[3], pl_TexteConf, strlen (pl_TexteConf), XDF_CONF_SEQ_ENR  );
    }
   
   sprintf ( pl_TexteConf,     "%s\t%d\t%d\t%d\n\0",
                   va_SEQ.Domaine,
                   va_SEQ.TypeEquipement,
                   va_SEQ.Numero,
                   va_SEQ.NumSequence );

   XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ei_config_SeqENR %s", pl_TexteConf );
   /* Ecriture dans le fichier de config */
   vl_ValRet = ec_write_fic ( vg_FdConf[3], pl_TexteConf, strlen (pl_TexteConf), XDF_CONF_SEQ_ENR );

   XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ei_config_SeqENR " );
   return ( vl_ValRet );
}  

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int    ei_config_SeqINC ( XZAOT_SEQ    va_SEQ,     
                          XDY_Entier   va_NumAppel )

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
*   Ecrire les parametres sequence dans le fichier de config 
*   sequence du domaine Protection Incendie GTC Nice Renovee
*
------------------------------------------------------*/
{
    XDY_Texte  pl_TexteConf;
    int        vl_ValRet = XDC_OK;
    if ( va_NumAppel == 1)
    {
        /* Ecriture de l'entete dans le fichier de config */
        sprintf ( pl_TexteConf,     "\n\n#Domaine\tTypeEqt\tNumero\tNumSeq\n\0" );
        vl_ValRet = ec_write_fic ( vg_FdConf[3], pl_TexteConf, strlen (pl_TexteConf), XDF_CONF_SEQ_INC  );
    }
   
   sprintf ( pl_TexteConf,     "%s\t%d\t%d\t%d\n\0",
                   va_SEQ.Domaine,
                   va_SEQ.TypeEquipement,
                   va_SEQ.Numero,
                   va_SEQ.NumSequence );

   XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ei_config_SeqINC %s", pl_TexteConf );
   /* Ecriture dans le fichier de config */
   vl_ValRet = ec_write_fic ( vg_FdConf[3], pl_TexteConf, strlen (pl_TexteConf), XDF_CONF_SEQ_INC );

   XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ei_config_SeqINC " );
   return ( vl_ValRet );
}  


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int    ei_config_SeqEXP ( XZAOT_SEQ    va_SEQ,     
                          XDY_Entier   va_NumAppel )

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
*   Ecrire les parametres sequence dans le fichier de config 
*   sequence du domaine Exploitation GTC Nice Renovee
*
------------------------------------------------------*/
{
    XDY_Texte  pl_TexteConf;
    int        vl_ValRet = XDC_OK;
    XZST_03EcritureTrace ( XZSTC_INTERFACE ," IN  : ei_config_SeqEXP, va_NumAppel = %d, fichier : %s", va_NumAppel, XDF_CONF_SEQ_EXP);
    if ( va_NumAppel == 1)
    {
        /* Ecriture de l'entete dans le fichier de config */
        sprintf ( pl_TexteConf,     "\n\n#Domaine\tTypeEqt\tNumero\tNumSeq\n\0" );
        vl_ValRet = ec_write_fic ( vg_FdConf[3], pl_TexteConf, strlen (pl_TexteConf), XDF_CONF_SEQ_EXP  );
    }
   
   sprintf ( pl_TexteConf,     "%s\t%d\t%d\t%d\n\0",
                   va_SEQ.Domaine,
                   va_SEQ.TypeEquipement,
                   va_SEQ.Numero,
                   va_SEQ.NumSequence );

   XZST_03EcritureTrace ( XZSTC_DEBUG1, " ei_config_SeqEXP Ecriture ligne '%s'", pl_TexteConf );
   /* Ecriture dans le fichier de config */
   vl_ValRet = ec_write_fic ( vg_FdConf[3], pl_TexteConf, strlen (pl_TexteConf), XDF_CONF_SEQ_EXP );

   XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ei_config_SeqEXP " );
   return ( vl_ValRet );
}  


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int  ei_config_eqt_GTC_NICE_RENOVEE ( EINI_MACHINE * pa_Machine )

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
*   Ecrire les parametres dans le fichier de config d'un systeme video
*   Mettre a jour pour la machine concerne la presence equipement
*
------------------------------------------------------*/
{
XDY_Texte               pl_TexteConf;
XZAOT_GTC_NICE_RENOVEE  vl_GTCNice;
int                     vl_ValRet = XDC_OK;

/* AZAZAZA */        
	XZST_03EcritureTrace( XZSTC_INTERFACE, "ei_config_eqt_GTC_NICE_RENOVEE : Configuration de la GTC Nice Renovee" );
    /* Lecture en base de donnees des paramatres de configuration */
    memset ( pl_TexteConf, ' ', sizeof(XDY_Texte)-2);
    sprintf ( pl_TexteConf+sizeof(XDY_Texte)-4, "\n\0");
    write ( vg_FdConf[4], pl_TexteConf, strlen(pl_TexteConf) );

    vl_ValRet =  XZAO2031_Liste_Eqt_GTC_NICE_RENOVEE (  EINI_NOM_BASE,
                                                        pa_Machine->NomMachine, 
                                                        ei_config_OUV,
                                                        ei_config_SIG,
                                                        ei_config_ECL,
                                                        ei_config_VEN,
                                                        ei_config_ENR,
                                                        ei_config_INC,
                                                        ei_config_EXP,
                                                        ei_config_SYS,
                                                        ei_config_LTN,
                                                        ei_config_EQTSIG,
                                                        ei_config_EQTECL,
                                                        ei_config_EQTVEN,
                                                        ei_config_EQTENR,
                                                        ei_config_EQTINC,
                                                        ei_config_EQTEXP,
                                                        ei_config_EQTSYS,
                                                        ei_config_EQTLTN,
                                                        ei_config_SeqSIG,
                                                        ei_config_SeqECL,
                                                        ei_config_SeqVEN,
                                                        ei_config_SeqENR,
                                                        ei_config_SeqINC,
														ei_config_SeqEXP,
                                                        &vl_GTCNice );
    if ( ( vl_ValRet == XDC_OK ) && ( vl_GTCNice.Numero != 0 ) )
    {
       /* Memorisation existance GTC Nice sur la machine */
       strcpy ( pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_NIC)].Equipement, XZECC_TACHE_NIC );
       pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_NIC)].Present = XZEXC_EQT_PRESENT;
              
	   XZST_03EcritureTrace( XZSTC_DEBUG1, "ei_config_eqt_GTC_NICE_RENOVEE : Fin Configuration de la GTC Nice Renovee (OK)" );
       return ( XDC_OK );
    }
    else
    {
       /* Memorisation absence GTC Nice sur la machine */
       strcpy ( pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_NIC)].Equipement, XZECC_TACHE_NIC );
       pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_NIC)].Present = XZEXC_EQT_ABSENT;
	   XZST_03EcritureTrace( XZSTC_WARNING, "ei_config_eqt_GTC_NICE_RENOVEE : Fin Configuration de la GTC Nice Renovee (erreur)" );
       return ( XDC_NOK );
    }
                   
}

/* AAZ */
