/*E*/
/* Fichier : $Id: gmot_com.c,v 1.3 2010/11/19 17:42:42 gesconf Exp $		        Release : $Revision: 1.3 $              Date : $Date: 2010/11/19 17:42:42 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_commun * FICHIER gmot_commun.c
******************************************************
* DESCRIPTION DU MODULE :
*
* Ce module contient les fonctions communes a tous
* les modules utilisant MOTIF:
* grisage de widgets, affichage de listes, ...
*
******************************************************
* HISTORIQUE :
*
* T;Milleville	15 Nov 94	: Creation
* P.Niepceron	28 Aou 96	: Chargement uid specifiques pour pc simplifie v1.2
* JPL		19/11/10	: Suppression warnings mineurs pour compilation sur architecture HP ia64 (DEM 961)  1.3
*******************************************************/

/* fichiers inclus */
#include <errno.h>
#include <X11/cursorfont.h>
#include <X11/Intrinsic.h>

#include "gmot_ser.h"
#include "gmot_var.h"
 
#include "gmot_com.h"

/* definitions de types locaux */

typedef char tl_Format[128];

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: gmot_com.c,v 1.3 2010/11/19 17:42:42 gesconf Exp $ : gmot_commun" ;

static Cursor wait_cursor;

/* declaration de fonctions internes */


/* fonction callback sur un bouton "Annuler" */

done(  Widget   dialog )
{
   XtDestroyWidget( dialog );
}


extern int vg_TypeMachine;


/* definition de fonctions externes */

/* Fichier : $Id: gmot_com.c,v 1.3 2010/11/19 17:42:42 gesconf Exp $	      Release : 1.3        Date : $Date: 2010/11/19 17:42:42 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_commun
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* ouvre une hierarchie Mrm ( contenant les objets MOTIF )
* apres avoir eventuellement ferme l'ancienne hierarchie
* Ce changement est fait a tout changement de langue
*
******************************************************
* SEQUENCE D'APPEL :
*
*
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
******************************************************/
extern int ouvre_hierarchie	( tg_ouverture 	va_FlagClose)

{
  register int 		valeurRetour = XDC_OK;
  static char*		version = "$Id: gmot_com.c,v 1.3 2010/11/19 17:42:42 gesconf Exp $ : ouvre_hierarchie";

  /* si flag fermeture hierarchie courante ... */
  if (va_FlagClose == cg_FLAG_FERME)
    {
      MrmCloseHierarchy( vg_mrm_hierarchy );
    }	
  
  /* ouverture de la hierarchie utile */ 
      if ( vg_TypeMachine == XZSCC_TYPEM_PCS )
         {
         if ( MrmOpenHierarchy (cg_NB_HIERARCHIES,  
			     vg_mrm_filename_list_frpcs,
			     &vg_mrm_open_param,
			     &vg_mrm_hierarchy ) != MrmSUCCESS ) 
		{	
	  	XZST_03EcritureTrace (XZSTC_FATAL,
				"ouvre_hierarchie: pb a l'appel de MrmOpenHierarchy" );
	  	valeurRetour = XDC_NOK;
		}
         }
      else
         {
         if ( MrmOpenHierarchy (cg_NB_HIERARCHIES,  
			     vg_mrm_filename_list_fr,
			     &vg_mrm_open_param,
			     &vg_mrm_hierarchy ) != MrmSUCCESS ) 
		{	
	  	XZST_03EcritureTrace (XZSTC_FATAL,
				"ouvre_hierarchie: pb a l'appel de MrmOpenHierarchy" );
	  	valeurRetour = XDC_NOK;
		}	
         }
  return (valeurRetour);
}




/*X*/
/* Fichier : $Id: gmot_com.c,v 1.3 2010/11/19 17:42:42 gesconf Exp $	      Release : 1.3        Date : $Date: 2010/11/19 17:42:42 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_commun
******************************************************
* DESCRIPTION DE LA FONCTION : 	busy_cursor
*	
* Cette fonction change la forme du curseur (curseur
* d'attente) sur la fenetre a laquelle appartient
* le widget passe en entree 
*
******************************************************
* SEQUENCE D'APPEL :
*
*
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
******************************************************/
extern void busy_cursor	( Widget  va_widget )
{
   int 		i;
   Widget  	vl_parent;
   Display 	*pl_display  = XtDisplay( va_widget );

   if ( wait_cursor == 0 )
     wait_cursor = XCreateFontCursor( pl_display, XC_watch );

   XDefineCursor( pl_display, XtWindow( va_widget ), wait_cursor );

   if ( (vl_parent = XtParent( va_widget )) != NULL )
      XDefineCursor( pl_display, XtWindow( vl_parent ), wait_cursor );

   for ( i=0; i<va_widget->core.num_popups; i++ )
       if ( XtIsRealized( va_widget->core.popup_list[i] ))
	  XDefineCursor( pl_display, XtWindow( va_widget->core.popup_list[i] ), wait_cursor );

   XFlush( XtDisplay ( va_widget ) );
   
}



/* Fichier : $Id: gmot_com.c,v 1.3 2010/11/19 17:42:42 gesconf Exp $	      Release : 1.3        Date : $Date: 2010/11/19 17:42:42 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_commun
******************************************************
* DESCRIPTION DE LA FONCTION : 	unbusy_cursor
*	
* Cette fonction change la forme du curseur (curseur
* normal) sur la fenetre a laquelle appartient
* le widget passe en entree 
*
******************************************************
* SEQUENCE D'APPEL :
*
*
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
******************************************************/
extern void unbusy_cursor	( Widget  va_widget )
{
   int 		i;
   Widget	vl_parent;
   Display 	*pl_display  = XtDisplay( va_widget );

   XUndefineCursor( pl_display, XtWindow( va_widget ) );

   if ( (vl_parent = XtParent( va_widget )) != NULL )
      XUndefineCursor( pl_display, XtWindow( vl_parent ) );

   for ( i=0; i<va_widget->core.num_popups; i++ )
       if ( XtIsRealized( va_widget->core.popup_list[i] ))
	  XUndefineCursor( pl_display, XtWindow( va_widget->core.popup_list[i] ) );

}



/*X*/
/* Fichier : $Id: gmot_com.c,v 1.3 2010/11/19 17:42:42 gesconf Exp $	      Release : 1.3        Date : $Date: 2010/11/19 17:42:42 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_commun
******************************************************
* DESCRIPTION DE LA FONCTION : 	ConstruitMessage
*	
*	Cette fonction construit une chaine de carac.
* a partir du numero du message et des parametres 
* passes sous formes d'arguments variables en faisant 
* appel a XDL_01 pour recuperer le format propre a la
* langue utilise.
******************************************************
* SEQUENCE D'APPEL :
*
*
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
******************************************************/
extern int construit_message	(Widget va_Parent,
				 int va_NumeroMessage,
				 ...)
{
  /* Variables Locales */
  register int 		valeurRetour = XDC_OK;
  static char*		version = "$Id: gmot_com.c,v 1.3 2010/11/19 17:42:42 gesconf Exp $ : ConstruitMessage";

  char        		vl_format [255];
  XDLT_SystemeAlarme   	vl_SysAlarme;
  char                 	vl_messageAffichage [255];
  va_list              	vl_pointeurArgument;
   
  /* Positionnement du pointeur sur la liste d'argument */
  va_start (vl_pointeurArgument, va_NumeroMessage);
 
  /* recuperation du format d'ecriture a partir de va_NumTexte   */
  if (XDL_01SystemeAlarme (va_NumeroMessage, &vl_SysAlarme) != XDC_OK)
    {
      XZST_03EcritureTrace (XZSTC_WARNING,
			    "GMOT_COMMUN Construit Message: pb a l'appel de XDL_01SystemeAlarme");
    }
  else
    {
	  sprintf (vl_format, "%s", vl_SysAlarme.FormatFranc);

      if (!strcmp (vl_format, ""))
        {
	  XZST_03EcritureTrace( XZSTC_WARNING,
			       "GMOT_COMMUN Construit Message: format de message vide");
        }
      /* Ecriture avec le format precedent des arguments dans le tableau */
      else if (vsprintf (vl_messageAffichage, vl_format, vl_pointeurArgument) < 0)
        {
	  XZST_03EcritureTrace( XZSTC_WARNING,
			       "GMOT_COMMUN Erreur Systeme Vfprintf: %d", errno, NULL);
	  valeurRetour = XDC_NOK;
	}
      /* Affichage dans une boite de dialogue */
      else if (affiche_message (va_Parent, vl_messageAffichage) != XDC_OK)
	{
	  XZST_03EcritureTrace( XZSTC_WARNING,
			       "GMOT_COMMUN AfficheMessage: echec l'affichage");
	  valeurRetour = XDC_NOK;
	}
    }

  /* Liberation de la liste variable d'argument */
  va_end( vl_pointeurArgument );

  return (valeurRetour);
}
  


/* Fichier : $Id: gmot_com.c,v 1.3 2010/11/19 17:42:42 gesconf Exp $	      Release : 1.3        Date : $Date: 2010/11/19 17:42:42 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_commun
******************************************************
* DESCRIPTION DE LA FONCTION : 	AfficheMessage
*	
*	Cette fonction affiche le message construit par
* la primitive ConstruitMessage dans la Boite de Dialogue
* qu'elle cree.
******************************************************
* SEQUENCE D'APPEL :
*
*
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
******************************************************/
extern int affiche_message	(Widget 	va_Parent,
				 char* 		va_Message)

{
  /* Variables Locales */
  register int 		valeurRetour = XDC_OK;
  static char*		version = "$Id: gmot_com.c,v 1.3 2010/11/19 17:42:42 gesconf Exp $ : AfficheMessage";
  XmString		vl_MessageString, vl_OKstring;
  Widget 		vl_BoiteDialogue,
  			vl_Label;
  Arg			vl_Arguments [10];
  int			i = 0;

  /* Conversion du message en Message Motif */
  vl_MessageString = XmStringCreateSimple (va_Message);

  vl_OKstring = XmStringCreateSimple ("OK");

  /* Inialisation des composants de la message box */
  XtSetArg (vl_Arguments [i], XmNcancelLabelString, vl_OKstring);i++;
  XtSetArg (vl_Arguments [i], XmNmessageString, vl_MessageString);i++;
  XtSetArg (vl_Arguments [i], XmNmessageAlignment, XmALIGNMENT_CENTER);i++;
  XtSetArg (vl_Arguments [i], XmNdeleteResponse, XmDESTROY);i++;
  XtSetArg (vl_Arguments [i], XmNdialogStyle, XmDIALOG_FULL_APPLICATION_MODAL);i++;

  /* Creation de la boite */
  vl_BoiteDialogue = XmCreateMessageDialog (va_Parent, "Message/Meldung", vl_Arguments, i );

  /* Abandon du bouton  HELP et OK */
  XtUnmanageChild (XmMessageBoxGetChild (vl_BoiteDialogue, XmDIALOG_OK_BUTTON));
  XtUnmanageChild (XmMessageBoxGetChild (vl_BoiteDialogue, XmDIALOG_HELP_BUTTON));

  /* Ajout de callback sur le dernier bouton (cancel) "ok" */
  XtAddCallback (vl_BoiteDialogue, XmNcancelCallback, (XtCallbackProc) done, NULL);

  /* Affichage de la boite de dialogue */
  XtManageChild (vl_BoiteDialogue);
  XtPopup (XtParent (vl_BoiteDialogue), XtGrabNone);

  /* liberation memoire */
  XmStringFree (vl_MessageString);
  XmStringFree (vl_OKstring);

  return (valeurRetour);
}
  
  
  
/* Fichier : $Id: gmot_com.c,v 1.3 2010/11/19 17:42:42 gesconf Exp $	      Release : 1.3        Date : $Date: 2010/11/19 17:42:42 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_commun
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* Cette fonction recupere l'adresse de la fenetre mere
* et rensigne la structure tg_InfosFenetres qui carac-
* terise cette fenetre.
******************************************************
* SEQUENCE D'APPEL :
*
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
* *
* CONDITIONS LIMITES :
*
* *
******************************************************/

extern int get_fenetre_mere (Widget			va_Widget, 
			     tg_InfosFenetres**		pa_Infos)
{
   /* Variables Locales */
   static char*		version = "$Id: gmot_com.c,v 1.3 2010/11/19 17:42:42 gesconf Exp $ : GetFenetreMere";
   register int 	valeurRetour = XDC_OK;
  

   /* Recuperation de la fenetre */
   XtVaGetValues (va_Widget, XmNuserData, pa_Infos, NULL);

   if (!*pa_Infos) valeurRetour = XDC_NOK;

   return valeurRetour;
}

  
  
/* Fichier : $Id: gmot_com.c,v 1.3 2010/11/19 17:42:42 gesconf Exp $	      Release : 1.3        Date : $Date: 2010/11/19 17:42:42 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_commun
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* Cette fonction rend le display Unix de l'ecran ou 
* se  trouve le widget passe en parametre d'entree
* qui est la concatenetion du hostname avec :0. et
* le numero d'ecran
******************************************************
* SEQUENCE D'APPEL :
*
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
* *
* CONDITIONS LIMITES :
*
* *
******************************************************/

extern void get_display_unix (Widget			va_Widget, 
			     XDY_DisplayUnix		pa_Display)
{
   /* Variables Locales */
   static char*		version = "$Id: gmot_com.c,v 1.3 2010/11/19 17:42:42 gesconf Exp $ : GetFenetreMere";
   register int 	valeurRetour = XDC_OK;
  
   XZSCT_NomMachine      vl_machineLocale;
   int                   vl_numScreen;

   /* recuperation du nom de la machine */
   XZSC_07NomMachine( vl_machineLocale );

   /* on recupere l'ecran du menu appelant */
   vl_numScreen = XScreenNumberOfScreen( XtScreen( va_Widget ) );

   /* concatenation du nom de la machine avec le numero d'ecran */
   sprintf( pa_Display, "%s:0.%d", vl_machineLocale, vl_numScreen );
}

  
/* Fichier : $Id: gmot_com.c,v 1.3 2010/11/19 17:42:42 gesconf Exp $	      Release : 1.3        Date : $Date: 2010/11/19 17:42:42 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_commun
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* Cette fonction positionne un popup menu dans un widget
* a l'endroit ou on clique (bouton droit de la souris)
*
******************************************************
* SEQUENCE D'APPEL :
*
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
* *
* CONDITIONS LIMITES :
*
* *
******************************************************/

extern void PostPopup (	Widget		va_Widget, 
			XtPointer       va_client_data,
			XButtonEvent    *va_event )

{
	Widget vl_popup = (Widget) va_client_data;

	/* bouton droit de la souris ? */
	if (va_event->button != 3 )
	   return;

	/*  Positionne le Popup menu au clic de la souris */
	XmMenuPosition( vl_popup, va_event );
	XtManageChild( vl_popup );
}


