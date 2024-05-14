/*E*/
/*  Fichier : $Id: eini_vid.c,v 1.8 2019/02/12 16:54:49 pc2dpdy Exp $      Release : $Revision: 1.8 $        Date : $Date: 2019/02/12 16:54:49 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEINI * FICHIER eini_vid.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module de creation des fichiers de config pour la tache VIDEO >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1 	26 Sep 1994	: Creation
* Mismer.D 	version 1.2 	14 Dec 1994 	:
* Mismer.D 	version 1.3 	23 Jan 1995 	:
* Mismer.D 	version 1.4 	28 Jan 1995 	:
* Fontaine.C 	version 1.5 	19 Mai 1995 	: Reprise entete, historique
* JMG	07/11/18 ajout vitesse 1.6
* RGR	12/02/19 : suppression generation fichier Config_SYSVIDEO
------------------------------------------------------*/

/* fichiers inclus */

#include "eini_cfg.h"

/* definitions de constantes */

/* definitions de types locaux */


/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: eini_vid.c,v 1.8 2019/02/12 16:54:49 pc2dpdy Exp $ : eini_vid" ;
  
/* declaration de fonctions internes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_config_LTVideo (	XZAOT_LTVideo 	va_LTVideo, 
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
*   Ecrire les parametres dans le fichier de config LTVideo
*   Mettre a jour pour la machine concerne la presence equipement
*
------------------------------------------------------*/
{
XDY_Texte	pl_TexteConf;
int		vl_ValRet = XDC_OK;

   if ( va_NumAppel == 1)
   {
      /* Ecriture de l'entete dans le fichier de config */
      sprintf ( pl_TexteConf, 	"# NumLTVid\tLFixe\tLCycle\t\n\0" );
      vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_SYSVIDEO );
   }
   sprintf ( pl_TexteConf, 	" %d\t%d\t%d\n\0",
   				va_LTVideo.Numero,
   				va_LTVideo.LiaisonsFixes,
   				va_LTVideo.LiaisonsCycles );
   XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ei_config_LTVideo %s", pl_TexteConf );
   /* Ecriture dans le fichier de config */
   vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_SYSVIDEO );

   XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ei_config_LTVideo " );
   XZST_03EcritureTrace ( XZSTC_WARNING, " OUT : ei_config_LTVideo " );
   return ( vl_ValRet );
}  


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_config_Camera (	XZAOT_Camera 	va_Camera, 
				int	  	va_NumAppel )

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
*   Ecrire les parametres dans le fichier de config Camera
*   Mettre a jour pour la machine concerne la presence equipement
*
------------------------------------------------------*/
{
XDY_Texte	pl_TexteConf;
int		vl_ValRet = XDC_OK;

   if ( va_NumAppel == 1)
   {
      /* Ecriture de l'entete dans le fichier de config */
      sprintf ( pl_TexteConf, 	"#C\tNumCamera\tNumLTVid\tCode\tCycle\tMobile\tPositionnable\n\0" );
      vl_ValRet = ec_write_fic ( vg_FdConf[1], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_VIDEO );
   }
   sprintf ( pl_TexteConf, 	"C\t%d\t%d\t%s\t%d\t%d\t%d\n\0",
   				va_Camera.Numero,
   				va_Camera.NumeroLTVideo,
   				va_Camera.Code,
   				va_Camera.Cycle,
   				va_Camera.Mobile,
   				va_Camera.Positionnable );
   XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ei_config_Camera %s", pl_TexteConf );
   /* Ecriture dans le fichier de config */
   vl_ValRet = ec_write_fic ( vg_FdConf[1], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_VIDEO );

   XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ei_config_Camera " );
   return ( vl_ValRet );
}  


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_config_Moniteur (	XZAOT_Moniteur 	va_Moniteur, 
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
*   Ecrire les parametres dans le fichier de config Moniteur
*   Mettre a jour pour la machine concerne la presence equipement
*
------------------------------------------------------*/
{
XDY_Texte	pl_TexteConf;
int		vl_ValRet = XDC_OK;

   if ( va_NumAppel == 1)
   {
      /* Ecriture de l'entete dans le fichier de config */
      sprintf ( pl_TexteConf, 	"#M\tNumMonit\tCode\tMachineOperateur\t\n\0" );
      vl_ValRet = ec_write_fic ( vg_FdConf[1], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_VIDEO );
   }
   sprintf ( pl_TexteConf, 	"M\t%d\t%s\t%s\n\0",
   				va_Moniteur.Numero,
   				va_Moniteur.Code,
   				va_Moniteur.MachineOperateur );
   XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ei_config_Moniteur %s", pl_TexteConf );
   /* Ecriture dans le fichier de config */
   vl_ValRet = ec_write_fic ( vg_FdConf[1], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_VIDEO );

   XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ei_config_Moniteur " );
   return ( vl_ValRet );
}  


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_config_Magneto (	XZAOT_Magneto 	va_Magneto, 
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
*   Ecrire les parametres dans le fichier de config Magneto
*   Mettre a jour pour la machine concerne la presence equipement
*
------------------------------------------------------*/
{
XDY_Texte	pl_TexteConf;
int		vl_ValRet = XDC_OK;

   if ( va_NumAppel == 1)
   {
      /* Ecriture de l'entete dans le fichier de config */
      sprintf ( pl_TexteConf, 	"#G\tNumMagneto\tCode\t\n\0" );
      vl_ValRet = ec_write_fic ( vg_FdConf[1], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_VIDEO );
   }
   sprintf ( pl_TexteConf, 	"G\t%d\t%s\n\0",
   				va_Magneto.Numero,
   				va_Magneto.Code );
   XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ei_config_Magneto %s", pl_TexteConf );
   /* Ecriture dans le fichier de config */
   vl_ValRet = ec_write_fic ( vg_FdConf[1], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_VIDEO );

   XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ei_config_Magneto " );
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

int	ei_config_eqt_video ( 	EINI_MACHINE * 	pa_Machine )

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
XZAOT_SysVideo	vl_SysVideo = {0};
int		vl_ValRet = XDC_OK;

	XZST_03EcritureTrace ( XZSTC_WARNING, "ei_config_eqt_video : TypeMachine=%d",pa_Machine->TypeMachine);
     /* si on est sur un SP** alors presence de tevid.x */
     if (pa_Machine->TypeMachine == XZSCC_TYPEM_SP)
    {
    	/* Memorisation existance VID sur la machine */
	strcpy ( pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_VID)].Equipement, XZECC_TACHE_VID );
	pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_VID)].Present = XZEXC_EQT_PRESENT;

	/* mise a jour des liaisons series de la machine */
	ec_mise_a_jour_config_ligne ( vl_SysVideo.Liaison,
	                              vl_SysVideo.NomServeur,
				      XZECC_PROT_LCR,
				      XZECC_SENS_MAITRE,
			       	      XZECC_TYPE_EQT_VID,
				      20,0);


	return ( XDC_OK );
    }
    else
    {
       /* Memorisation absence VID sur la machine */
       strcpy ( pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_VID)].Equipement, XZECC_TACHE_VID );
       pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_VID)].Present = XZEXC_EQT_ABSENT;


       return ( XDC_NOK );
    }

	XZST_03EcritureTrace ( XZSTC_WARNING, "ei_config_eqt_video : OUT");
}
