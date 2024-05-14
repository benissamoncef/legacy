/*E*/
/*  Fichier : $Id: eini_gtc.c,v 1.6 2019/01/29 11:07:34 pc2dpdy Exp $      Release : $Revision: 1.6 $        Date : $Date: 2019/01/29 11:07:34 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEINI * FICHIER eini_gtc.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module de creation des fichiers de config pour la tache mini GTC >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1 	26 Sep 1994	: Creation
* Mismer.D 	version 1.2 	14 Dec 1994 	:
* Mismer.D 	version 1.3 	28 Jan 1995	:
* Fontaine.C	version 1.4	19 Mai 1995 	: Reprise entete, historique
* Mismer.D 	version 1.5 	08 Aou 1995	: Utilisation constante XDC_NB_ETOR
*  JMG   07/11/18 : ajout vitesse dans ec_mise_a_jour_config_ligne 1.6
------------------------------------------------------*/

/* fichiers inclus */

#include "eini_cfg.h"

/* definitions de constantes */

/* definitions de types locaux */


/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: eini_gtc.c,v 1.6 2019/01/29 11:07:34 pc2dpdy Exp $ : eini_gtc" ;
  
/* declaration de fonctions internes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_config_MiniGTC (	XZAOT_MiniGTC 	va_MiniGTC,
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
*   Ecrire les parametres dans le fichier de config d'une MiniGTC
*   Mettre a jour pour la machine concerne config de la ligne
*
------------------------------------------------------*/
{
XDY_Texte	pl_TexteConf;
int		vl_Index = 0;
int		vl_ValRet = XDC_OK;

   if ( va_NumAppel == 1 )
   {
      /* Ecriture de l'entete dans le fichier de config */
      sprintf ( pl_TexteConf, 	"#NumMiniGTC\tServeur\tLiaison\tRGS\t\n\0" );
      vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_GTC );
   }
   sprintf ( pl_TexteConf, 	"G %d\t%s\t%d\t%s\t\n\0",
   				va_MiniGTC.NumeroMiniGTC,
   				va_MiniGTC.NomServeur,
   				va_MiniGTC.Liaison,
   				va_MiniGTC.Rgs );
   				
   XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ec_config_MiniGTC %s", pl_TexteConf );
                      		 
   
   /* Ecriture dans le fichier de config */
   vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_GTC );
   
   for ( vl_Index = 0; vl_Index < XDC_NB_ETOR; vl_Index++)
   {
      if ( vl_Index == 0)
      {
         /* Ecriture de l'entete dans le fichier */
         sprintf ( pl_TexteConf, "#NumMiniGTC\tNumETOR\tNumEqtETOR\tAlarme\tAlerte\tNomETOR\t\n\0" );
         vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_GTC );
      }
      sprintf ( pl_TexteConf, 	"E\t%d\t%d\t%d\t%d\t%d\t%s\t\n\0",
   				va_MiniGTC.NumeroMiniGTC,
   				vl_Index,
   				va_MiniGTC.ListeETOR[vl_Index].NumeroEqt,
    				va_MiniGTC.ListeETOR[vl_Index].NumeroAlerte,
  				va_MiniGTC.ListeETOR[vl_Index].NumeroAlarme,
   				va_MiniGTC.ListeETOR[vl_Index].NomETOR );   
      /* Ecriture dans le fichier de config */
      vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_GTC );
   }

   /* mise a jour des liaisons series de la machine */
   ec_mise_a_jour_config_ligne ( va_MiniGTC.Liaison,
                      		 va_MiniGTC.NomServeur,
                      		 XZECC_PROT_LCR,
                      		 XZECC_SENS_MAITRE,
                      		 XZECC_TYPE_EQT_GTC,
                      		 va_MiniGTC.NumeroMiniGTC,0 );
        
   XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ec_config_MiniGTC " );
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

int	ei_config_eqt_MiniGTC ( 	EINI_MACHINE * 	pa_Machine )

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
*   Ecrire les parametres dans le fichier de config d'une MiniGTC
*   Mettre a jour pour la machine concerne la presence equipement
*
------------------------------------------------------*/
{
XDY_Texte	pl_TexteConf;
int		vl_ValRet = XDC_OK;
XDY_Entier	vl_NbEqt  = 0;

    /* Lecture en base de donnees des paramatres de configuration */
    vl_ValRet = XZAO77_Lire_Liste_Mini_GTC (  	EINI_NOM_BASE,
    					  	pa_Machine->NomMachine,
    					  	ei_config_MiniGTC,
    					  	&vl_NbEqt );
    if ( ( vl_ValRet == XDC_OK ) && ( vl_NbEqt != 0 ) )
    {
       /* Memorisation existance MiniGTC sur la machine */
       strcpy ( pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_GTC)].Equipement, XZECC_TACHE_GTC );
       pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_GTC)].Present = XZEXC_EQT_PRESENT;
       return ( XDC_OK );
    }
    else
    {
       /* Memorisation absence MiniGTC sur la machine */
       strcpy ( pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_GTC)].Equipement, XZECC_TACHE_GTC );
       pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_GTC)].Present = XZEXC_EQT_ABSENT;
       return ( XDC_NOK );
    }
                   
}
