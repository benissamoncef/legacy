
/*E*/
/*  Fichier : $Id: eini_tel.c,v 1.7 2019/01/29 11:07:42 pc2dpdy Exp $      Release : $Revision: 1.7 $        Date : $Date: 2019/01/29 11:07:42 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEINI * FICHIER eini_tel.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module de creation des fichiers de config pour la tache TEL >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1 	26 Sep 1994	: Creation
* Mismer.D 	version 1.2 	14 Dec 1994 	:
* Mismer.D 	version 1.3 	25 Jan 1995 	:
* Mismer.D 	version 1.4 	25 Jan 1995 	: RAS
* Fontaine.C 	version 1.5 	19 Mai 1995 	: Reprise entete, historique
* Mismer.D	version 1.6	10 Fev 1997	: Modif TDP (DEM/1395)
* JMG		04/11/18 	ajoute vitesse 1.7 
------------------------------------------------------*/

/* fichiers inclus */

#include "eini_cfg.h"

/* definitions de constantes */

/* definitions de types locaux */


/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: eini_tel.c,v 1.7 2019/01/29 11:07:42 pc2dpdy Exp $ : eini_tel" ;
  
/* declaration de fonctions internes */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_config_MAA (	XZAOT_MAA 	va_MAA, 
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
*   Ecrire les parametres dans le fichier de config d'un MAA
*   Mettre a jour pour la machine concerne la presence equipement
*
------------------------------------------------------*/
{
XDY_Texte	pl_TexteConf;
int		vl_ValRet = XDC_OK;

   if ( va_NumAppel == 1)
   {
      /* Ecriture de l'entete dans le fichier de config */
      sprintf ( pl_TexteConf, 	"#NumMAA\tServeur\tLiaison\tRGS\tMachOP\tNumRep\t\n\0" );
      vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_AUTOTEL );
   }
   
   if ( va_MAA.Liaison != 0 )
   {
      sprintf ( pl_TexteConf, 	" %d\t%s\t%d\t%s\t%d\t%s\n\0",
   				va_MAA.Numero,
   				va_MAA.NomServeur,
   				va_MAA.Liaison,
   				va_MAA.RGS,
   				va_MAA.MachineOperateur,
   				va_MAA.NumeroRepondeur );
      XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ec_config_MAA %s", pl_TexteConf );
      /* Ecriture dans le fichier de config */
      vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_AUTOTEL );

      /* mise a jour des liaisons series de la machine */
      if ( va_MAA.RGS[0] == 'A' )
      {
         ec_mise_a_jour_config_ligne ( va_MAA.Liaison,
                      		    va_MAA.NomServeur,
                      		    XZECC_PROT_TEL,
                      		    XZECC_SENS_MAITRE,
                      		    XZECC_TYPE_EQT_TEL,
                      		    va_MAA.Numero , 0);
      }
      else
      {
          /* Memorisation existance OPERATOR sur la machine */
          strcpy ( pg_MachineCrt->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_OPE)].Equipement, XZECC_TACHE_OPE );
          pg_MachineCrt->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_OPE)].Present = XZEXC_EQT_PRESENT;
          ec_mise_a_jour_config_ligne ( va_MAA.Liaison,
                      		    va_MAA.NomServeur,
                      		    XZECC_PROT_OPE,
                      		    XZECC_SENS_MAITRE,
                      		    XZECC_TYPE_EQT_OPE,
                      		    va_MAA.Numero,0 );
      }
   }
    
   XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ec_config_MAA " );
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

int	ei_config_eqt_maa ( 	EINI_MACHINE * 	pa_Machine )

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
*   Ecrire les parametres dans le fichier de config
*   Mettre a jour pour la machine concerne la presence equipement
*
------------------------------------------------------*/
{
XDY_Texte	pl_TexteConf;
int		vl_ValRet = XDC_OK;
XDY_Entier	vl_NbEqt  = 0;

    /* Lecture en base de donnees des paramatres de configuration */
    vl_ValRet = XZAO116_Lire_MAA ( 	EINI_NOM_BASE,
    					pa_Machine->NomMachine, 
    					ei_config_MAA, 
    					&vl_NbEqt );
    if ( ( vl_ValRet == XDC_OK ) && ( vl_NbEqt != 0 ) )
    {
       /* Memorisation existance MAA sur la machine */
       strcpy ( pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_TEL)].Equipement, XZECC_TACHE_TEL );
       pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_TEL)].Present = XZEXC_EQT_PRESENT;
       return ( XDC_OK );
    }
    else
    {
       /* Memorisation absence MAA sur la machine */
       strcpy ( pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_TEL)].Equipement, XZECC_TACHE_TEL );
       pa_Machine->Eqt[ec_rang_eqt(XZECC_TYPE_EQT_TEL)].Present = XZEXC_EQT_ABSENT;
       return ( XDC_NOK );
    }
                   
}
