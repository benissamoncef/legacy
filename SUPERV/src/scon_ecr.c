/*E*/
/* Fichier : $Id: scon_ecr.c,v 1.11 1998/09/22 13:59:11 gaborit Exp $      Release : $Revision: 1.11 $        Date : $Date: 1998/09/22 13:59:11 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_ecrit * FICHIER scon_ecrit.c
******************************************************
* DESCRIPTION DU MODULE :
*
* Module contenat les fonctions d'ecriture des 
*	differentes boites de dialogue
*
******************************************************
* HISTORIQUE :
*
* T.Milleville  30 Aou 1994	: Creation
* T.Milleville  09 Avr  1995	: Ajout Tache Shell SCript V1.5
* T.Milleville  25 Jul 1995	: Mise a jour entetet V1.6
* T.Milleville  25 Mar 1996	: Suppression Type Profile, et Priorite V1.7
* P.Niepceron   02 Avr 1996	: Suppression Chemin de trace V1.8
* P.Niepceron   22 Aou 1996	: Ajout du type de machine PCS V1.9
* P.Niepceron   22 Dec 1997	: Ajout du type de machine PC2 + STA V1.10
* P.Niepceron   01 Jul 1998	: Ajout du type de machine Historique v1.11
******************************************************/

/* fichiers inclus */

#include "scon_ser.h"
#include "scon_var.h"
#include "scon_ecr.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: scon_ecr.c,v 1.11 1998/09/22 13:59:11 gaborit Exp $ : scon_ecrit" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/* Fichier : $Id: scon_ecr.c,v 1.11 1998/09/22 13:59:11 gaborit Exp $      Release : $Revision: 1.11 $        Date : $Date: 1998/09/22 13:59:11 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_ecrit
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* cette fonction remplit le champs texte de BDCMA
*
******************************************************
* SEQUENCE D'APPEL :
*/
 
void EcritChampsCMA( char *pa_champ )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
	static char *version = "$Id: scon_ecr.c,v 1.11 1998/09/22 13:59:11 gaborit Exp $ : EcritChampsCMA" ;

  XmTextSetString( BDCMA_CH_CAL , pa_champ );
}

/*X*/
/* Fichier : $Id: scon_ecr.c,v 1.11 1998/09/22 13:59:11 gaborit Exp $      Release : $Revision: 1.11 $        Date : $Date: 1998/09/22 13:59:11 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_ecrit
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* cette fonction remplit le champs texte de BDCOMA
*
******************************************************
* SEQUENCE D'APPEL :
*/
 
void EcritChampsCOMA( char *pa_champ_dest, char *pa_champ_exped )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
	static char *version = "$Id: scon_ecr.c,v 1.11 1998/09/22 13:59:11 gaborit Exp $ : EcritChampsCOMA" ;

	/*A Renseigne les champs Destinataire et Expediteur */
  	XmTextSetString( BDCOMA_CH_CAL_DEST , pa_champ_dest );
  	XmTextSetString( BDCOMA_CH_CAL_EXPED , pa_champ_exped );
}


/* Fichier : $Id: scon_ecr.c,v 1.11 1998/09/22 13:59:11 gaborit Exp $      Release : $Revision: 1.11 $        Date : $Date: 1998/09/22 13:59:11 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_ecrit
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* cette fonction remplit :                            
* - tous les champs texte de BDMAC a partir des       
* champs de va_strucMac si mode est CG_NONVIDE        
* - seulement le champ NomMachine si mode est CG_VIDE 
*
******************************************************
* SEQUENCE D'APPEL :
*/

void EcritChampsMAC( XZSXT_Machine va_strucMac, int va_mode )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
	static char *version = "$Id: scon_ecr.c,v 1.11 1998/09/22 13:59:11 gaborit Exp $ : EcritChampsMAC" ;
  char  vl_chaine[13];

  XmTextSetString( BDMAC_CH_MAC, va_strucMac.NomMachine );

  if ( va_mode == CG_NONVIDE )
  {
    XmTextSetString( BDMAC_CH_IMPSEC, 
	va_strucMac.NomImprimanteSec[0] == CG_CHAR_DEFAUT ? 
	"" : va_strucMac.NomImprimanteSec );
    XmTextSetString( BDMAC_CH_SIT, 
	va_strucMac.NomSite[0] == CG_CHAR_DEFAUT ? 
	"" : va_strucMac.NomSite );
/*251196
    XmTextSetString( BDMAC_CH_CHE, 
	va_strucMac.PathTrace[0] == CG_CHAR_DEFAUT ? 
	"" : va_strucMac.PathTrace );
*/

    sprintf( vl_chaine, "%d", va_strucMac.MaxTailleFicTrace );
    XmTextSetString( BDMAC_CH_TAI, vl_chaine );

    sprintf( vl_chaine, "%d", va_strucMac.PeriodeTestReseau );
    XmTextSetString( BDMAC_CH_PER, vl_chaine );

/*251196
    XmTextSetString( BDMAC_CH_PSR, 
	va_strucMac.NomPeriphSR[0] == CG_CHAR_DEFAUT ? 
	"" : va_strucMac.NomPeriphSR );
*/
/*251196
    XmTextSetString( BDMAC_CH_MIM, 
	va_strucMac.NomMachineImp[0] == CG_CHAR_DEFAUT ? 
	"" : va_strucMac.NomMachineImp );
*/

    XmTextSetString( BDMAC_CH_IMP, 
	va_strucMac.NomImprimante[0] == CG_CHAR_DEFAUT ? 
	"" : va_strucMac.NomImprimante );
    sprintf( vl_chaine, "%d", va_strucMac.DureeAbsenceCourte );
    XmTextSetString( BDMAC_CH_DAB, vl_chaine );


/*251196 -- Modificqtion T.MIleville*/ 
/*    switch ( va_strucMac.Priorite )
    {
    case XZSCC_MAITRE: 
		    XmToggleButtonSetState( BDMAC_TO_OUI, True, False );
		    break;
    case XZSCC_ESCLAVE:
		    XmToggleButtonSetState( BDMAC_TO_NON, True, False );
		    break;
    default:
		    break;
    }*/

    switch ( va_strucMac.TypeMachine )
    {
    case XZSAC_TYPEM_ST: 
		    XmToggleButtonSetState( BDMAC_TO_ST, True, False );
		    break;
    case XZSAC_TYPEM_SD:
		    XmToggleButtonSetState( BDMAC_TO_SD, True, False );
		    break;
    case XZSAC_TYPEM_SC:
		    XmToggleButtonSetState( BDMAC_TO_SC, True, False );
		    break;
    case XZSAC_TYPEM_LT:
		    XmToggleButtonSetState( BDMAC_TO_LT, True, False );
		    break;
    case XZSAC_TYPEM_SP:
		    XmToggleButtonSetState( BDMAC_TO_SP, True, False );
		    break;
    case XZSAC_TYPEM_PCS:
		    XmToggleButtonSetState( BDMAC_TO_PCS, True, False );
		    break;
    case XZSAC_TYPEM_PC2:
		    XmToggleButtonSetState( BDMAC_TO_PC2, True, False );
		    break;
    case XZSAC_TYPEM_PC2LT:
		    XmToggleButtonSetState( BDMAC_TO_PC2LT, True, False );
		    break;
    case XZSAC_TYPEM_STA:
		    XmToggleButtonSetState( BDMAC_TO_STA, True, False );
		    break;
    case XZSAC_TYPEM_HIS:
		    XmToggleButtonSetState( BDMAC_TO_HIS, True, False );
		    break;

    default:
 		    /* erreur */
		    break;
    }

/*#251196 -- Modificqtion T.Milleville
#    switch ( va_strucMac.TypeProfil )
#    {
#    case XZSCC_TYPEP_NUL:
#		    XmToggleButtonSetState( T_PRO_NUL, True, False );
#		    break;
#    case XZSCC_TYPEP_ADM:
#		    XmToggleButtonSetState( T_PRO_ADM, True, False );
#		    break;
#    case XZSCC_TYPEP_GIT:
#		    XmToggleButtonSetState( T_PRO_GIT, True, False );
#		    break;
#    case XZSCC_TYPEP_OTR:
#		    XmToggleButtonSetState( T_PRO_OTR, True, False );
#		    break;
#    default:
#		    break;
#    } */

  }
  else
  {

    XmTextSetString( BDMAC_CH_SIT, "");
    XmTextSetString( BDMAC_CH_TAI, "");
    XmTextSetString( BDMAC_CH_PER, "");
    XmTextSetString( BDMAC_CH_DAB, "");
/*251196
    XmTextSetString( BDMAC_CH_PSR, "");
*/

/* 251196 
    XmTextSetString( BDMAC_CH_CHE, "");
    XmTextSetString( BDMAC_CH_MIM, "");*/

    XmTextSetString( BDMAC_CH_IMP, "");
    XmTextSetString( BDMAC_CH_IMPSEC, "");

/*#251196 -- Mofification T.Milleville
#    XmToggleButtonSetState( BDMAC_TO_NON, True, False );
#    XmToggleButtonSetState( BDMAC_TO_OUI, False, False );

    XmToggleButtonSetState( BDMAC_TO_ST, True, False );
    XmToggleButtonSetState( BDMAC_TO_SD , False, False );
    XmToggleButtonSetState( BDMAC_TO_SC , False, False );
    XmToggleButtonSetState( BDMAC_TO_LT , False, False );
    XmToggleButtonSetState( BDMAC_TO_SP , False, False );
    XmToggleButtonSetState( BDMAC_TO_PCS , False, False );
    XmToggleButtonSetState( BDMAC_TO_PC2 , False, False );
    XmToggleButtonSetState( BDMAC_TO_PC2LT , False, False );
    XmToggleButtonSetState( BDMAC_TO_STA , False, False );
    XmToggleButtonSetState( BDMAC_TO_HIS, False, False );

#251196 -- Mofification T.Milleville
#    XmToggleButtonSetState( T_PRO_NUL , True, False );
#    XmToggleButtonSetState( T_PRO_ADM , False, False );
#    XmToggleButtonSetState( T_PRO_GIT , False, False );
#    XmToggleButtonSetState( T_PRO_OTR , False, False );*/
  } 
}



/* Fichier : $Id: scon_ecr.c,v 1.11 1998/09/22 13:59:11 gaborit Exp $      Release : $Revision: 1.11 $        Date : $Date: 1998/09/22 13:59:11 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_ecrit
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* cette fonction remplit le champs texte de BDCTM
*
******************************************************
* SEQUENCE D'APPEL :
*/
 
void EcritChampsCTM( char *pa_champ1, char *pa_champ2 )
  
/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
	static char *version = "$Id: scon_ecr.c,v 1.11 1998/09/22 13:59:11 gaborit Exp $ : EcritChampsCTM" ;

   XmTextSetString( BDCTM_CH_CAL , pa_champ1 );
   XmTextSetString( BDCTM_CH_TAC , pa_champ2 );
}



/* Fichier : $Id: scon_ecr.c,v 1.11 1998/09/22 13:59:11 gaborit Exp $      Release : $Revision: 1.11 $        Date : $Date: 1998/09/22 13:59:11 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_ecrit
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* cette fonction remplit :
* - tous les champs texte de BDTAC a partir des
* champs de va_strucTac si mode est CG_NONVIDE
* - seulement le champ NomTache si mode est CG_VIDE
*
******************************************************
* SEQUENCE D'APPEL :
*/
 
void EcritChampsTAC( XZSXT_Tache va_strucTac, int va_mode )
  
/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
	static char *version = "$Id: scon_ecr.c,v 1.11 1998/09/22 13:59:11 gaborit Exp $ : EcritChampsTAC" ;
  char  vl_chaine[4];
   
  XmTextSetString( BDTAC_CH_TAC, va_strucTac.NomTache );
      
  if ( va_mode == CG_NONVIDE )
  {
    /*XmTextSetString( BDTAC_CH_LRT, va_strucTac.ServerNode[0] == CG_CHAR_DEFAUT ? "" : va_strucTac.ServerNode );*/
    XmTextSetString( BDTAC_CH_LRT, va_strucTac.ListeServer[0] == CG_CHAR_DEFAUT ? "" : va_strucTac.ListeServer );
    XmTextSetString( BDTAC_CH_LRL, va_strucTac.ListeLicense[0] == CG_CHAR_DEFAUT ? CG_RTLICENSE_DEFAUT : va_strucTac.ListeLicense );
		       
    sprintf( vl_chaine, "%d", va_strucTac.TailleFicTrace );
    XmTextSetString( BDTAC_CH_TAI, vl_chaine );
  }
  else
  {
    XmTextSetString( BDTAC_CH_LRT, "");
    XmTextSetString( BDTAC_CH_LRL, CG_RTLICENSE_DEFAUT);
    XmTextSetString( BDTAC_CH_TAI, "");
  }
}
 

 
/* Fichier : $Id: scon_ecr.c,v 1.11 1998/09/22 13:59:11 gaborit Exp $      Release : $Revision: 1.11 $        Date : $Date: 1998/09/22 13:59:11 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_ecrit
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* cette fonction remplit le champs texte de BDSIT
*
******************************************************
* SEQUENCE D'APPEL :
*/
 
void EcritChampsSIT( char *pa_champ1 )
  
/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
	static char *version = "$Id: scon_ecr.c,v 1.11 1998/09/22 13:59:11 gaborit Exp $ : EcritChampsSIT" ;


    XmTextSetString( BDSIT_CH_SIT , pa_champ1 );
}



/* Fichier : $Id: scon_ecr.c,v 1.11 1998/09/22 13:59:11 gaborit Exp $      Release : $Revision: 1.11 $        Date : $Date: 1998/09/22 13:59:11 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_ecrit
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* cette fonction remplit le champs texte de BDSSY
*
******************************************************
* SEQUENCE D'APPEL :
*/
 
void EcritChampsSSY( char *pa_champ1 )
  
/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
	static char *version = "$Id: scon_ecr.c,v 1.11 1998/09/22 13:59:11 gaborit Exp $ : EcritChampsSSY" ;

   XmTextSetString( BDSSY_CH_SSY , pa_champ1 );
   XmToggleButtonSetState( BDSSY_TO_ST , True, False );
   XmToggleButtonSetState( BDSSY_TO_SC , False, False );
   XmToggleButtonSetState( BDSSY_TO_SD , False, False );

}



/* Fichier : $Id: scon_ecr.c,v 1.11 1998/09/22 13:59:11 gaborit Exp $      Release : $Revision: 1.11 $        Date : $Date: 1998/09/22 13:59:11 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_ecrit
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* cette fonction remplit le champs texte de BDCAL
*
******************************************************
* SEQUENCE D'APPEL :
*/
 
void EcritChampsCAL( char *pa_champ1 )
   
/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
*****************************************************/
{
	static char *version = "$Id: scon_ecr.c,v 1.11 1998/09/22 13:59:11 gaborit Exp $ : EcritChampsCAL" ;

   XmTextSetString( BDCAL_CH_CAL , pa_champ1 );
}




/* Fichier : $Id: scon_ecr.c,v 1.11 1998/09/22 13:59:11 gaborit Exp $      Release : $Revision: 1.11 $        Date : $Date: 1998/09/22 13:59:11 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_ecrit
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* cette fonction remplit les champs texte de BDENT
*
******************************************************
* SEQUENCE D'APPEL :
*/

void EcritChampsENT( char *pa_champ1, char *pa_champ2, 
		char *pa_champ3, char *pa_champ4 )
/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
*****************************************************/
{
	static char *version = "$Id: scon_ecr.c,v 1.11 1998/09/22 13:59:11 gaborit Exp $ : EcritChampsENT" ;

   XmTextSetString( BDENT_CH_SI , pa_champ1 );
   XmTextSetString( BDENT_CH_EN , pa_champ2 );
   XmTextSetString( BDENT_CH_TA , pa_champ3 );
   XmTextSetString( BDENT_CH_CA , pa_champ4 );

   XmToggleButtonSetState( BDENT_TO_PER , True, False );
   XmToggleButtonSetState( BDENT_TO_INT , False, False );
   /*XmToggleButtonSetState( BDENT_TO_CON , False, False );*/
   XmToggleButtonSetState( BDENT_TO_TAC , False, False );

   /* champs et labels "Entite" actifs */
   XtSetSensitive( BDENT_LA_EN, True );
   XtSetSensitive( BDENT_CH_EN, True );
       
   /* champs et labels "Tache" grises */
   XtSetSensitive( BDENT_LA_TA, False );
   XtSetSensitive( BDENT_CH_TA, False );
	      
   /* champs et labels "Calculateur" grises */
   XtSetSensitive( BDENT_LA_CA, False );
   XtSetSensitive( BDENT_CH_CA, False );

   if ( vg_operation == CG_CREENT )
   {
     /* champs et labels "Site" actifs */
     XtSetSensitive( BDENT_LA_SI, True );
     XtSetSensitive( BDENT_CH_SI, True );
   }
   else
   {
	/*  -- Modification  ***/
     /* champs et labels "Site" grises */
     XtSetSensitive( BDENT_LA_SI, False );
     XtSetSensitive( BDENT_CH_SI, False );
	/* -----------------*/
     /* champs et labels "Site" grises */
     XtSetSensitive( BDENT_LA_SI, True );
     XtSetSensitive( BDENT_CH_SI, True );
   }

}




/* Fichier : $Id: scon_ecr.c,v 1.11 1998/09/22 13:59:11 gaborit Exp $      Release : $Revision: 1.11 $        Date : $Date: 1998/09/22 13:59:11 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_ecrit
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* cette fonction remplit les champs texte de BDTAS
*
******************************************************
* SEQUENCE D'APPEL :
*/

void EcritChampsTAS( char *pa_champ1, char *pa_champ2 )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
*****************************************************/
{
	static char *version = "$Id: scon_ecr.c,v 1.11 1998/09/22 13:59:11 gaborit Exp $ : EcritChampsTAS" ;

    XmTextSetString( BDTAS_CH_SSY , pa_champ1 );
    XmTextSetString( BDTAS_CH_TAC , pa_champ2 );
    XmToggleButtonSetState( BDTAS_TO_ST , True, False );
    XmToggleButtonSetState( BDTAS_TO_SC , False, False );
    XmToggleButtonSetState( BDTAS_TO_SD , False, False );
    XmToggleButtonSetState( BDTAS_TO_LT , False, False );
    XmToggleButtonSetState( BDTAS_TO_SP , False, False );
    XmToggleButtonSetState( BDTAS_TO_PCS , False, False );
    XmToggleButtonSetState( BDTAS_TO_PC2 , False, False );
    XmToggleButtonSetState( BDTAS_TO_PC2LT , False, False );
    XmToggleButtonSetState( BDTAS_TO_STA , False, False );
    XmToggleButtonSetState( BDTAS_TO_HIS, False, False );
    XmToggleButtonSetState( Tache_Permanente , True, False );
    XmToggleButtonSetState( Tache_Temporaire , False, False );
    XmToggleButtonSetState( Tache_ShellScript , False, False );
}




/* Fichier : $Id: scon_ecr.c,v 1.11 1998/09/22 13:59:11 gaborit Exp $      Release : $Revision: 1.11 $        Date : $Date: 1998/09/22 13:59:11 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_ecrit
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* cette fonction remplit le champs texte de BDCJA
*
******************************************************
* SEQUENCE D'APPEL :
*/
 
void EcritChampsCJA( char *pa_champ )

/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : < commentaire sur le resultat >
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
*
******************************************************/
{
	static char *version = "$Id: scon_ecr.c,v 1.11 1998/09/22 13:59:11 gaborit Exp $ : EcritChampsCJA" ;

  XmTextSetString( BDCJA_CH_TAI , pa_champ );
}



