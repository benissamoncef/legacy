/*E*/
/* Fichier : $Id: scon_gri.c,v 1.4 1995/07/26 14:11:50 milleville Exp $      Release : $Revision: 1.4 $        Date : $Date: 1995/07/26 14:11:50 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_grise * FICHIER scon_grise.c
******************************************************
* DESCRIPTION DU MODULE :
*
* Ce module contient la gestion des boites de
*	dialogue en terme d'interdiction d'acces 
*	de certains champs
*
******************************************************
* HISTORIQUE :
*
* T.Milleville  30 Aou 1994	: Creation
* T.Milleville  25 Jul 1995	: Mise ajour entete V1.4
*******************************************************/

/* fichiers inclus */

#include "scon_ser.h"
#include "scon_var.h"
#include "scon_gri.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: scon_gri.c,v 1.4 1995/07/26 14:11:50 milleville Exp $ : scon_gri" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/* Fichier : $Id: scon_gri.c,v 1.4 1995/07/26 14:11:50 milleville Exp $      Release : $Revision: 1.4 $        Date : $Date: 1995/07/26 14:11:50 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_grise
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* callback appele lorsque le toggle "Tache"
* est arme
* - champs "Tache", "Calculateur" actifs
* - champs "Site", "Entite" grises
*
******************************************************
* SEQUENCE D'APPEL :
*/

void
TACH_ARM_CB( Widget w, XtPointer client_data, XtPointer xt_call_data )

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
	static char *version = "$Id: scon_gri.c,v 1.4 1995/07/26 14:11:50 milleville Exp $ : TACH_ARM_CB" ;
  XmPushButtonCallbackStruct *call_data = (XmPushButtonCallbackStruct *) xt_call_data;
   
  /* champs et labels "Entite" grises */
  XtSetSensitive( BDENT_LA_EN, False );
  XtSetSensitive( BDENT_CH_EN, False );

  /* champs et labels "Tache" actifs */
  XtSetSensitive( BDENT_LA_TA, True );
  XtSetSensitive( BDENT_CH_TA, True );

  /* champs et labels "Calculateur" actifs */
  XtSetSensitive( BDENT_LA_CA, True );
  XtSetSensitive( BDENT_CH_CA, True );

  /* champs et labels "Site" grises */
  XtSetSensitive( BDENT_LA_SI, False );
  XtSetSensitive( BDENT_CH_SI, False );
}

/*X*/
/* Fichier : $Id: scon_gri.c,v 1.4 1995/07/26 14:11:50 milleville Exp $      Release : $Revision: 1.4 $        Date : $Date: 1995/07/26 14:11:50 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_grise
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* callback appele lorsque le toggle "Peripherique"
* est arme
* - champs "Site" actif pour la CREATION, grise 
*   pour la DESTRUCTION
* - champs "Entite" actif
* - champs "Tache", "Calculateur" grises
*
******************************************************
* SEQUENCE D'APPEL :
*/

void
PERI_ARM_CB( Widget w, XtPointer client_data, XtPointer xt_call_data )

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
	static char *version = "$Id: scon_gri.c,v 1.4 1995/07/26 14:11:50 milleville Exp $ : PERI_ARM_CB" ;
  XmPushButtonCallbackStruct *call_data = (XmPushButtonCallbackStruct *) xt_call_data;
     
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
    /* champs et labels "Site" grises */
    XtSetSensitive( BDENT_LA_SI, False );
    XtSetSensitive( BDENT_CH_SI, False );
  }

}

/*X*/
/* Fichier : $Id: scon_gri.c,v 1.4 1995/07/26 14:11:50 milleville Exp $      Release : $Revision: 1.4 $        Date : $Date: 1995/07/26 14:11:50 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_grise
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* callback appele lorsque le toggle "Interface"
* ou "Concentrateur" est arme
* - champs "Entite" actif
* - champs "Tache", "Calculateur" et "Site" grises
*
******************************************************
* SEQUENCE D'APPEL :
*/

void
AUTR_ARM_CB( Widget w, XtPointer client_data, XtPointer xt_call_data )

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
	static char *version = "$Id: scon_gri.c,v 1.4 1995/07/26 14:11:50 milleville Exp $ : AUTR_ARM_CB" ;
  XmPushButtonCallbackStruct *call_data = (XmPushButtonCallbackStruct *) xt_call_data;

  /* champs et labels "Entite" actifs */
  XtSetSensitive( BDENT_LA_EN, True );
  XtSetSensitive( BDENT_CH_EN, True );

  /* champs et labels "Tache" grises */
  XtSetSensitive( BDENT_LA_TA, False );
  XtSetSensitive( BDENT_CH_TA, False );

  /* champs et labels "Calculateur" grises */
  XtSetSensitive( BDENT_LA_CA, False );
  XtSetSensitive( BDENT_CH_CA, False );

  /* champs et labels "Site" grises */
  XtSetSensitive( BDENT_LA_SI, False );
  XtSetSensitive( BDENT_CH_SI, False );

}
