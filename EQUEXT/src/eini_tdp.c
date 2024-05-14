/*E*/
/*  Fichier : $Id: eini_tdp.c,v 1.4 2019/01/29 11:07:40 pc2dpdy Exp $      Release : $Revision: 1.4 $        Date : $Date: 2019/01/29 11:07:40 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEINI * FICHIER eini_tdp.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module de creation des fichiers de config pour la tache equipement temps de parcours >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	05 Fev 1997	: Creation (DEM/1395)
* Mismer.D	version 1.2	25 Mar 1997	: Modif pour TDP (DEM/1396)
* JPL	17/02/12 : Migration architecture HP ia64 : format champs de 'printf'  1.3
* JMG	07/11/18 ajout vitesse 1.4
------------------------------------------------------*/

/* fichiers inclus */

#include "eini_cfg.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: eini_tdp.c,v 1.4 2019/01/29 11:07:40 pc2dpdy Exp $ : eini_tdp" ;
extern EINI_DONNEES_FEN_LCR *	        pg_DebListeTDP;

/* declaration de fonctions internes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_ajouter_eqt_TDP (	XZAOT_EQT_TDP 	va_EqtLCR,
				int 		va_NumAppel )

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
*   Ecrire les parametres dans le fichier de config des equipements TDP
*
------------------------------------------------------*/
{
XDY_Texte	pl_TexteConf = "";
int		vl_ValRet = XDC_OK;

   if ( va_NumAppel == 1)
   {
      /* Ecriture de l'entete dans le fichier de config */
      sprintf ( pl_TexteConf, 	"# NumEqt\tNomServeur\tRGS\tLiaison\n\0" );
      vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_TDP );
   }
   sprintf ( pl_TexteConf, 	XZEXC_FORMAT_CONF_TDP,
   				va_EqtLCR.Numero,
   				va_EqtLCR.NomServeur,
   				va_EqtLCR.RGS,
   				va_EqtLCR.Liaison );
   XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ei_ajouter_eqt_TDP %s", pl_TexteConf );

   /* Ecriture dans le fichier de config */
   if ( (vl_ValRet = ec_write_fic ( vg_FdConf[0], pl_TexteConf, strlen (pl_TexteConf), XDF_Config_TDP )) != XDC_NOK )
   {
       ec_mise_a_jour_config_ligne ( 	va_EqtLCR.Liaison,
                      			va_EqtLCR.NomServeur,
                      			XZECC_PROT_LCR,
                      			XZECC_SENS_MAITRE,
                      			XDC_EQT_TDP,
                      			va_EqtLCR.Numero,0 );
   }
   XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ei_ajouter_eqt_TDP" );
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

int	ei_config_eqt_TDP ( 	EINI_MACHINE * 	pa_Machine )

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
int		vl_ValRet = XDC_OK;
XDY_Entier	vl_NbEqt = 0;

   /* Lecture en base de donnees des paramatres de configuration */
   XZST_03EcritureTrace ( XZSTC_INTERFACE, 
   			" Appel XZAO402_Lire_Liste_Eqt_TDP avec EINI_NOM_BASE, %s, ei_config_eqt_TDP",
   			 pa_Machine->NomMachine );
   vl_ValRet = XZAO402_Lire_Liste_Eqt_TDP ( 	EINI_NOM_BASE,
   					 	pa_Machine->NomMachine,
   					 	ei_ajouter_eqt_TDP,
   					 	&vl_NbEqt );
   					 	      
   if ( ( vl_ValRet == XDC_OK ) && ( vl_NbEqt != 0 ) )
   {
       /* Memorisation existance temps de parcours sur la machine et config ligne serie */
       strcpy ( pa_Machine->Eqt[ec_rang_eqt(XDC_EQT_TDP)].Equipement, XZECC_TACHE_TDP );
       pa_Machine->Eqt[ec_rang_eqt(XDC_EQT_TDP)].Present = XZEXC_EQT_PRESENT;
       return ( XDC_OK );
   }
   else
   {
       /* Memorisation absence temps de parcours sur la machine */
       strcpy ( pa_Machine->Eqt[ec_rang_eqt(XDC_EQT_TDP)].Equipement, XZECC_TACHE_TDP );
       pa_Machine->Eqt[ec_rang_eqt(XDC_EQT_TDP)].Present = XZEXC_EQT_ABSENT;
       return ( XDC_NOK );
   }
                   
}

   
    
   
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_ajout_Eqt_TDP_Fentre_LCR (	XZAOT_EQT_TDP 	va_EqtLCR,
					int 		va_NumAppel )

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
*   Creer la liste des equipements TDP
*
------------------------------------------------------*/
{
   EINI_DONNEES_FEN_LCR 	*pl_NewEqtLCR, *pl_EqtLCR, *pl_ExEqtLCR;
   XDY_Nom			vl_NomPR   = "",
    				vl_TmpPR   = "";
   int				vl_ValRet = XDC_OK;

   XZST_03EcritureTrace ( XZSTC_FONCTION, " IN  : ei_config_TDP_Fenetre_LCR %d", va_NumAppel );
 
   if ( va_EqtLCR.Liaison > 0 )
   {
      if ( (va_EqtLCR.PR > 0) && (va_EqtLCR.Sens > 0) )
      {
          sprintf ( vl_TmpPR, "%ld", (XDY_LongEntier) va_EqtLCR.PR );
          switch ( strlen(vl_TmpPR) )
          {
             case 4 :
                sprintf ( vl_NomPR, "%c.%c%c%c\0", vl_TmpPR[0],vl_TmpPR[1],vl_TmpPR[2],vl_TmpPR[3]);
                break;
             case 5 :
                sprintf ( vl_NomPR, "%c%c.%c%c%c\0", vl_TmpPR[0],vl_TmpPR[1],vl_TmpPR[2],vl_TmpPR[3],vl_TmpPR[4]);
                break;
             default :
                sprintf ( vl_NomPR, "%c%c%c.%c%c%c\0", vl_TmpPR[0],vl_TmpPR[1],vl_TmpPR[2],vl_TmpPR[3],vl_TmpPR[4],vl_TmpPR[5]);
                break;
           }
      }
   
      /*A Allocation memoire pour la nouvelle structure */
      if ( ( pl_NewEqtLCR = (EINI_DONNEES_FEN_LCR *) malloc ( sizeof(EINI_DONNEES_FEN_LCR) ) ) == NULL)
      {
         /* Ecriture trace */ 
         XZST_03EcritureTrace ( XZSTC_WARNING, " OUT: ei_config_TDP_Fenetre_LCR Allocation memoire impossible");
         return XDC_NOK;
      }

      /*A Mise a jour de la structure a inserer */
      memset ( pl_NewEqtLCR, 0, sizeof(EINI_DONNEES_FEN_LCR) );
      pl_NewEqtLCR->NumEqt  = va_EqtLCR.Numero;
      pl_NewEqtLCR->TypeEqt = XDC_EQT_TDP;
      sprintf ( pl_NewEqtLCR->NomEqt, "%s %s-%d-%s\0", va_EqtLCR.LibType, va_EqtLCR.NomAutoroute, va_EqtLCR.Sens, vl_NomPR );
      strcpy ( pl_NewEqtLCR->NomLT, va_EqtLCR.NomMachine );
   
   
      /*A Ajout de la structure dans la liste */
      pl_NewEqtLCR->Suivant = pg_DebListeTDP;
      pg_DebListeTDP        = pl_NewEqtLCR;
   }


   XZST_03EcritureTrace ( XZSTC_FONCTION, " OUT : ei_ajouter_eqt_TDP" );
   return ( vl_ValRet );
}  
    

                  
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ei_config_TDP_Fenetre_LCR ( 	XDY_NomMachine pa_NomMachine )

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
*   Appel de XZAO402_Lire_Liste_Eqt_TDP pour mise a jour de la liste des equipement TDP
*
------------------------------------------------------*/
{
int			vl_ValRet = XDC_OK;
XDY_Entier		vl_NbEqt = 0;
EINI_DONNEES_FEN_LCR 	*pl_EqtLCR, *pl_ExEqtLCR;
XDY_NomMachine  	vl_NomMachine = " ";

   /* Lecture en base de donnees des paramatres de configuration */
   XZST_03EcritureTrace (  XZSTC_INTERFACE,
   			" Appel XZAO402_Lire_Liste_Eqt_TDP avec EINI_NOM_BASE, %s, ei_config_TDP_Fenetre_LCR ",
   			 pa_NomMachine );
   if ( strcmp (pa_NomMachine, XZECC_TOUTES_MACHINES) )
      strcpy ( vl_NomMachine, pa_NomMachine );
   else
   {
      /*A liberation de la liste des equipement TDP  */
      for ( pl_EqtLCR = pl_ExEqtLCR = pg_DebListeTDP;
            pl_EqtLCR != NULL;
            pl_ExEqtLCR = pl_EqtLCR  )
      {
         /*A Liberation memoire */
         pl_EqtLCR = pl_EqtLCR->Suivant;
         free ( pl_ExEqtLCR );
      }
      pg_DebListeTDP = NULL;
   }
      
   vl_ValRet = XZAO402_Lire_Liste_Eqt_TDP ( 	EINI_NOM_BASE,
   					 	vl_NomMachine,
   					 	ei_ajout_Eqt_TDP_Fentre_LCR,
   					 	&vl_NbEqt );
   					 	      

   XZST_03EcritureTrace ( XZSTC_INFO, 
   			" Appel XZAO402_Lire_Liste_Eqt_TDP retourne %d EQT ",
   			vl_NbEqt  );

   return (vl_ValRet);
}
