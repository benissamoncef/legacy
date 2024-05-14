/*E*/
/* Fichier : $Id: scon_div.c,v 1.5 1995/07/26 13:44:15 milleville Exp $      Release : $Revision: 1.5 $        Date : $Date: 1995/07/26 13:44:15 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_div * FICHIER scon_div.c
******************************************************
* DESCRIPTION DU MODULE :
*
* Module gerant les boites de dialogue d'affichage d'erreurs
*	et de confirmation.
*
******************************************************
* HISTORIQUE :
*
* T.Milleville  30 Aou 1994	: Creation
* T.Milleville  25 Jul 1995	: Mise a jour entete  V1.4
*******************************************************/

/* fichiers inclus */

#include "scon_div.h"
#include "scon_ser.h"
#include "scon_var.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: scon_div.c,v 1.5 1995/07/26 13:44:15 milleville Exp $ : scon_div" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/* Fichier : $Id: scon_div.c,v 1.5 1995/07/26 13:44:15 milleville Exp $      Release : $Revision: 1.5 $        Date : $Date: 1995/07/26 13:44:15 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_mac
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* cette fonction affiche un message d'erreur dans 
* une boite de dialogue
*
******************************************************
* SEQUENCE D'APPEL :
*/

void AfficheErreur ( TG_MessageType pa_message )
 
/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
******************************************************/
{
	static char *version = "$Id: scon_div.c,v 1.5 1995/07/26 13:44:15 milleville Exp $ : AfficheErreur" ;
   XmString vl_XmString;

   /* le label de la boite de dialogue BDERR recoit la          */
   /* chaine pa_message                                         */
   vl_XmString = XmStringCreateSimple( pa_message );
   XtVaSetValues( BDERR_LA, XmNlabelString, vl_XmString, NULL );
  
   /* liberation de la memoire allouee par XmStringCreateSimple */
   XmStringFree( vl_XmString );

   XtManageChild( BDERR_FM );
}



/*X*/
/* Fichier : $Id: scon_div.c,v 1.5 1995/07/26 13:44:15 milleville Exp $      Release : $Revision: 1.5 $        Date : $Date: 1995/07/26 13:44:15 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_mac
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* cette fonction affiche un message de demande de 
* confirmation dans la boite de dialogue BDCON1
*
******************************************************
* SEQUENCE D'APPEL :
*/

void DemandeConf1 ( TG_MessageType pa_message )
 
/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
******************************************************/
{
	static char *version = "$Id: scon_div.c,v 1.5 1995/07/26 13:44:15 milleville Exp $ : DemandeConf1" ;
   XmString vl_XmString;

   /* le label de la boite de dialogue BDCON1 recoit la          */
   /* chaine pa_message                                         */
   vl_XmString = XmStringCreateSimple( pa_message );
   XtVaSetValues( BDCON1_LA, XmNlabelString, vl_XmString, NULL );
  
   /* liberation de la memoire allouee par XmStringCreateSimple */
   XmStringFree( vl_XmString );

   XtManageChild( BDCON1_FM );
}



/*X*/
/* Fichier : $Id: scon_div.c,v 1.5 1995/07/26 13:44:15 milleville Exp $      Release : $Revision: 1.5 $        Date : $Date: 1995/07/26 13:44:15 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_mac
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* cette fonction affiche un message de demande de 
* confirmation dans la boite de dialogue BDCON2
*
******************************************************
* SEQUENCE D'APPEL :
*/

void DemandeConf2 ( TG_MessageType pa_message )
 
/*
* PARAMETRES EN ENTREE :
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
******************************************************/
{
	static char *version = "$Id: scon_div.c,v 1.5 1995/07/26 13:44:15 milleville Exp $ : DemandeConf2" ;
   XmString vl_XmString;

   /* le label de la boite de dialogue BDCON2 recoit la          */
   /* chaine pa_message                                         */
   vl_XmString = XmStringCreateSimple( pa_message );
   XtVaSetValues( BDCON2_LA, XmNlabelString, vl_XmString, NULL );
  
   /* liberation de la memoire allouee par XmStringCreateSimple */
   XmStringFree( vl_XmString );

   XtManageChild( BDCON2_FM );
}

