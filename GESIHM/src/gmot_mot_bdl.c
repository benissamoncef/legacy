/*E*/
/* Fichier : @(#)gmot_mot_bdl.c	1.6	        Release : 1.6              Date : 03/03/95
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_mot_bdl * FICHIER gmot_mot_bdl.c
******************************************************
* DESCRIPTION DU MODULE :
*
*
******************************************************
* HISTORIQUE :
*
* T.Milleville	14 Nov 94	: Creation
* JPL		19/11/10	: Suppression warnings mineurs pour compilation sur architecture HP ia64 (DEM 961)  1.2
*******************************************************/

/******************************************************
* TESTS UNITAIRES  : RELECTURE CROISEE
*
*******************************************************/

/* fichiers inclus */

#include "gmot_ser.h"
#include "gmot_var.h"
#include "gmot_mot_bdl.h"
#include "gmot_com.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version   = "@(#)gmot_mot_bdl.c	1.6	 03/03/95 :  gmot_mot_bdl.c";

/* declaration de fonctions internes */
void create_GRMACH_SYNT (
	Widget parent, 
	MrmHierarchy hierarchy_id, 
	MrmCode *mrm_class);



/*X*/
/* Fichier : @(#)gmot_mot_bdl.c	1.6	      Release : 1.6        Date : 03/03/95
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_mot_bdl
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* fonction du popup menu sur GRSYN 
* revient a l'architecture generale 
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
void syngen_cb (	Widget 			va_Widget, 
			XtPointer 		va_ClientData, 
			XmAnyCallbackStruct 	va_Call_data)
{
    Widget  	vl_wid_drawingArea;
    int		vl_cr;

    vl_wid_drawingArea =  XtParent( XtParent( XtParent( va_Widget)));

    /* retour au synoptique d'architecture generale */
    vl_cr = AfficheVueAdm( vl_wid_drawingArea, cg_TYP_ARC, NULL );

}
 

  
/*X*/
/* Fichier : @(#)gmot_mot_bdl.c	1.6	      Release : 1.6        Date : 03/03/95
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_mot_bdl
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* fonction du popup menu sur GRSYN 
* ferme GRSYN
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

void quit_GRSYN_cb (	Widget 			va_Widget, 
			XtPointer 		va_ClientData, 
			XmAnyCallbackStruct 	va_Call_data)
{
    XtUnmanageChild(XtParent(XtParent(XtParent(XtParent(XtParent(XtParent(va_Widget)))))));

    /* on ferme une vue ARCHITECTURE generale ? */
    if ( vg_nb_vues_arch_ecran > 0 )
    	vg_nb_vues_arch_ecran = vg_nb_vues_arch_ecran - 1 ;
}
 

  
/*X*/
/* Fichier : @(#)gmot_mot_bdl.c	1.6	      Release : 1.6        Date : 03/03/95
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_mot_bdl
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* cree un popup menu pour l'afficher dans GRSYN 
* ce popup contiendra les fonctions suivantes:
* - Revenir a l'architecture generale
* - Fermer GRSYN
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
*
******************************************************/

void MetPopupGRSYN( Widget  va_widget )
{
   Arg    	al[64];     
   int    	ac = 0;      
   Widget 	children[2];  
   Widget 	vl_popupGRSYN;
   Widget 	vl_quit_wid;
   Widget 	vl_syngen_wid;
   XmString     vl_string1;
   XmString     vl_string2;

   vl_popupGRSYN= XmCreatePopupMenu( va_widget, "popupGRSYN", NULL, 0);
	 
   vl_syngen_wid = XmCreatePushButton ( 
			vl_popupGRSYN, 
			"Archi", 
			al, ac );

   XtAddCallback ( 	vl_syngen_wid, 
			XmNactivateCallback, 
			(XtCallbackProc) syngen_cb, NULL);

   vl_quit_wid = XmCreatePushButton ( 
			vl_popupGRSYN, 
			"Quitter", 
			al, ac );

   XtAddCallback (	vl_quit_wid, 
			XmNactivateCallback, 
			(XtCallbackProc) quit_GRSYN_cb, NULL);

   vl_string1 = XmStringCreateSimple( "Architecture generale" );
   vl_string2 = XmStringCreateSimple( "Quitter" );

   XtVaSetValues( vl_syngen_wid, XmNlabelString, vl_string1, NULL);
   XtVaSetValues( vl_quit_wid, XmNlabelString, vl_string2, NULL);

   XmStringFree( vl_string1 );
   XmStringFree( vl_string2 );

   ac = 0;
   children[ac++] = vl_syngen_wid;
   children[ac++] = vl_quit_wid;
   XtManageChildren(children, ac);


   /* PostPopup positionne le popup au clic souris */
   XtAddEventHandler( va_widget, ButtonPressMask, 
			False, (XtEventHandler) PostPopup, vl_popupGRSYN );
}




/* definition de fonctions externes */


  
/*X*/
/* Fichier : @(#)gmot_mot_bdl.c	1.6	      Release : 1.6        Date : 03/03/95
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_mot_bdl
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* fonction creant la fenetre MOTIF BDASU 
* a partir de la hierarchie Mrm couramment ouverte
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
* CONDITIONS LIMITES :
*
******************************************************/

extern void create_BDASU (Widget parent, MrmHierarchy hierarchy_id, MrmCode *mrm_class)
{
	Arg al[64];                    /* Arg List */
	register int ac = 0;           /* Arg Count */


	XZST_03EcritureTrace(XZSTC_FONCTION,"create_BDASU : IN \n");


	XtSetArg(al[ac], XmNtitle, "ARRET/LANCEMENT DE MIGRAZUR"); ac++;

	XtSetArg(al[ac], XmNallowShellResize, TRUE ); ac++;
	BDASU = XmCreateDialogShell ( parent, "BDASU", al, ac );

	(void) MrmFetchWidget ( hierarchy_id, "BDASU_FM", BDASU, &BDASU_FM, mrm_class );

	BDASU_LI_MAC = XtNameToWidget ( XtNameToWidget ( BDASU_FM, "BDASU_LI_MACSW" ), "BDASU_LI_MAC" );

	BDASU_TO_AR = XtNameToWidget ( XtNameToWidget ( BDASU_FM, "BDASU_RA_BOX" ), "BDASU_TO_AR" );
	BDASU_TO_LA = XtNameToWidget ( XtNameToWidget ( BDASU_FM, "BDASU_RA_BOX" ), "BDASU_TO_LA" );

	XZST_03EcritureTrace(XZSTC_FONCTION,"create_BDASU : OUT \n");

}	/*Fin create_BDASU */


  
/*X*/
/* Fichier : @(#)gmot_mot_bdl.c	1.6	      Release : 1.6        Date : 03/03/95
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODUL  gmot_mot_bdl
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* fonction creant la fenetre MOTIF LIJA 
* a partir de la hierarchie Mrm couramment ouverte
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
* CONDITIONS LIMITES :
*
******************************************************/

extern void create_LIJA (
	Widget parent, 
	MrmHierarchy hierarchy_id, 
	MrmCode *mrm_class)
{
	Arg al[64];                    /* Arg List */
	register int ac = 0;           /* Arg Count */

	XtSetArg(al[ac], XmNallowShellResize, TRUE ); ac++;
	LIJA = XmCreateDialogShell ( parent, "LIJA", al, ac );
	
	(void) MrmFetchWidget ( hierarchy_id, "LIJA_FM", LIJA, &LIJA_FM, mrm_class );
	LIJA_LI = XtNameToWidget ( XtNameToWidget ( LIJA_FM, "LIJA_LISW" ), "LIJA_LI" );
}


  
/*X*/
/* Fichier : @(#)gmot_mot_bdl.c	1.6	      Release : 1.6        Date : 03/03/95
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_mot_bdl
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* fonction creant la fenetre MOTIF LIVL 
* a partir de la hierarchie Mrm couramment ouverte
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
* CONDITIONS LIMITES :
*
******************************************************/

extern void create_LIVL (Widget parent, MrmHierarchy hierarchy_id, MrmCode *mrm_class)
{
	Arg al[64];                    /* Arg List */
	register int ac = 0;           /* Arg Count */

	XtSetArg(al[ac], XmNallowShellResize, TRUE ); ac++;
	LIVL = XmCreateDialogShell ( parent, "LIVL", al, ac );

	(void) MrmFetchWidget ( hierarchy_id, "LIVL_FM", LIVL, &LIVL_FM, mrm_class );
	LIVL_LI = XtNameToWidget ( XtNameToWidget ( LIVL_FM, "LIVL_LISW" ), "LIVL_LI" );
	LIVL_CH_MAC = XtNameToWidget ( LIVL_FM, "LIVL_CH_MAC" );
}


  
/* Fichier : @(#)gmot_mot_bdl.c	1.6	      Release : 1.6        Date : 03/03/95
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_mot_bdl
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* fonction creant la fenetre MOTIF BDERR 
* a partir de la hierarchie Mrm couramment ouverte
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
* CONDITIONS LIMITES :
*
******************************************************/

extern void create_BDERR (Widget parent, MrmHierarchy hierarchy_id, MrmCode *mrm_class)
{
	Arg al[64];                    /* Arg List */
	register int ac = 0;           /* Arg Count */

	XtSetArg(al[ac], XmNallowShellResize, TRUE ); ac++;
	BDERR = XmCreateDialogShell ( parent, "BDERR", al, ac );
	
	(void) MrmFetchWidget ( hierarchy_id, "BDERR_FM", BDERR, &BDERR_FM, mrm_class );
	BDERR_LA = XtNameToWidget ( BDERR_FM, "BDERR_LA" );
}


  
/* Fichier : @(#)gmot_mot_bdl.c	1.6	      Release : 1.6        Date : 03/03/95
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_mot_bdl
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* fonction creant la fenetre MOTIF BDACA 
* a partir de la hierarchie Mrm couramment ouverte
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
* CONDITIONS LIMITES :
*
******************************************************/

extern void create_BDACA (Widget parent, MrmHierarchy hierarchy_id, MrmCode *mrm_class)
{
	Arg al[64];                    /* Arg List */
	register int ac = 0;           /* Arg Count */

	XtSetArg(al[ac], XmNtitle, "ARRET CALCULATEUR"); ac++;
	XtSetArg(al[ac], XmNallowShellResize, TRUE ); ac++;
	BDACA = XmCreateDialogShell ( parent, "BDACA", al, ac );
	ac = 0;
	(void) MrmFetchWidget ( hierarchy_id, "BDACA_FM", BDACA, &BDACA_FM, mrm_class );
	BDACA_LI_MAC = XtNameToWidget ( XtNameToWidget ( BDACA_FM, "BDACA_LI_MACSW" ), "BDACA_LI_MAC" );

	BDACA_TO_AVE = XtNameToWidget ( XtNameToWidget ( BDACA_FM, "BDACA_RA_BOX" ), "BDACA_TO_AVE" );
	BDACA_TO_SAN = XtNameToWidget ( XtNameToWidget ( BDACA_FM, "BDACA_RA_BOX" ), "BDACA_TO_SAN" );
}


  
  
/* Fichier : @(#)gmot_mot_bdl.c	1.6	      Release : 1.6        Date : 03/03/95
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_mot_bdl
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* fonction creant la fenetre MOTIF BDBSC 
* a partir de la hierarchie Mrm couramment ouverte
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
* CONDITIONS LIMITES :
*
******************************************************/

extern void create_BDBSC (Widget parent, MrmHierarchy hierarchy_id, MrmCode *mrm_class)
{
	Arg al[64];                    /* Arg List */
	register int ac = 0;           /* Arg Count */

	XtSetArg(al[ac], XmNtitle, "BASCULEMENT D'UN SERVEUR DE COMMUNICATION"); ac++;
	XtSetArg(al[ac], XmNallowShellResize, TRUE ); ac++;
	BDBSC = XmCreateDialogShell ( parent, "BDBSC", al, ac );
	ac = 0;
	(void) MrmFetchWidget ( hierarchy_id, "BDBSC_FM", 
					BDBSC, &BDBSC_FM, mrm_class );

	BDBSC_LI_SIT = XtNameToWidget ( 
		XtNameToWidget (BDBSC_FM, "BDBSC_LI_SITSW" ), "BDBSC_LI_SIT" );


}	/* Fin create_BDBSC */


  
/* Fichier : @(#)gmot_mot_bdl.c	1.6	      Release : 1.6        Date : 03/03/95
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_mot_bdl
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* fonction creant la fenetre MOTIF BDASS 
* a partir de la hierarchie Mrm couramment ouverte
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
* CONDITIONS LIMITES :
*
******************************************************/

extern void create_BDASS (Widget parent, MrmHierarchy hierarchy_id, MrmCode *mrm_class)
{
	Arg al[64];                    /* Arg List */
	register int ac = 0;           /* Arg Count */

	XtSetArg(al[ac], XmNtitle, "ARRET/LANCEMENT D'UN SOUS-SYSTEME"); ac++;
	XtSetArg(al[ac], XmNallowShellResize, TRUE ); ac++;
	BDASS = XmCreateDialogShell ( parent, "BDASS", al, ac );
	
	(void) MrmFetchWidget ( hierarchy_id, "BDASS_FM", BDASS, &BDASS_FM, mrm_class );
	BDASS_LI_MAC = XtNameToWidget ( XtNameToWidget ( BDASS_FM, "BDASS_LI_MACSW" ), "BDASS_LI_MAC" );

	BDASS_LI_SSS = XtNameToWidget ( XtNameToWidget ( BDASS_FM, "BDASS_LI_SSSSW" ), "BDASS_LI_SSS" );

	BDASS_TO_AR = XtNameToWidget ( XtNameToWidget ( BDASS_FM, "BDASS_RA_BOX" ), "BDASS_TO_AR" );
	BDASS_TO_LA = XtNameToWidget ( XtNameToWidget ( BDASS_FM, "BDASS_RA_BOX" ), "BDASS_TO_LA" );
}


  
/*X*/
/* Fichier : @(#)gmot_mot_bdl.c	1.6	      Release : 1.6        Date : 03/03/95
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_mot_bdl
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* fonction creant la fenetre MOTIF GRTRA 
* a partir de la hierarchie Mrm couramment ouverte
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
* CONDITIONS LIMITES :
*
******************************************************/

extern void create_GRTRA (Widget parent, MrmHierarchy hierarchy_id, MrmCode *mrm_class)
{
	Arg al[64];                    /* Arg List */
	register int ac = 0;           /* Arg Count */
	Widget widgetRC = (Widget)NULL;
	Widget widgetLISW = (Widget)NULL;


	XZST_03EcritureTrace(XZSTC_FONCTION,
		"create_GRTRA  : IN \n");

	XtSetArg(al[ac], XmNtitle, "SYNOPTIQUE DES TRACES"); ac++;
	XtSetArg(al[ac], XmNallowShellResize, TRUE ); ac++;
	GRTRA = XmCreateDialogShell ( parent, "GRTRA", al, ac );
	ac = 0;
	GRTRA_FM=NULL;
	(void) MrmFetchWidget ( hierarchy_id, "GRTRA_FM", GRTRA, &GRTRA_FM, mrm_class );

	/* Management de la Fenetre */
	XtManageChild(GRTRA_FM);

	/* Definition des divers Widgets de la Fenetre */
	widgetRC = XtNameToWidget ( XtNameToWidget ( GRTRA_FM, "GRTRA_FM_2" ), "GRTRA_RC_1" );
	GRTRA_TO_FO = XtNameToWidget ( widgetRC, "GRTRA_TO_FO" );
	GRTRA_TO_ME = XtNameToWidget ( widgetRC, "GRTRA_TO_ME" );
	GRTRA_TO_IN = XtNameToWidget ( widgetRC, "GRTRA_TO_IN" );
	GRTRA_TO_BD = XtNameToWidget ( widgetRC, "GRTRA_TO_BD" );
	GRTRA_TO_D1 = XtNameToWidget ( widgetRC, "GRTRA_TO_D1" );
	GRTRA_TO_D2 = XtNameToWidget ( widgetRC, "GRTRA_TO_D2" );
	GRTRA_TO_D3 = XtNameToWidget ( widgetRC, "GRTRA_TO_D3" );
	GRTRA_TO_EC = XtNameToWidget ( widgetRC, "GRTRA_TO_EC" );
	GRTRA_TO_INTER = XtNameToWidget ( widgetRC, "GRTRA_TO_INTER" );
	GRTRA_TO_REGLE = XtNameToWidget ( widgetRC, "GRTRA_TO_REGLE" );


	widgetLISW = XtNameToWidget ( XtNameToWidget ( GRTRA_FM, "GRTRA_FM_2" ), "GRTRA_LISW" );
	GRTRA_LI = XtNameToWidget ( widgetLISW, "GRTRA_LI" );


	GRTRA_SCROLL = XtNameToWidget ( GRTRA_FM, "GRTRA_SCROLL" );
	GRTRA_DA = XmCreateDrawingArea( GRTRA_SCROLL, "GRTRA_DA",
		al,ac);


	XZST_03EcritureTrace(XZSTC_FONCTION,
		"create_GRTRA  : OUT \n");

}	/* Fin create_GRTRA */


  
/*X*/
/* Fichier : @(#)gmot_mot_bdl.c	1.6	      Release : 1.6        Date : 03/03/95
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_mot_bdl
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* fonction creant la fenetre MOTIF BDSTA 
* a partir de la hierarchie Mrm couramment ouverte
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
* CONDITIONS LIMITES :
*
******************************************************/

extern void create_BDSTA (      Widget          parent,  
				MrmHierarchy    hierarchy_id, 
				MrmCode         *mrm_class)
{
	Arg al[64];                    /* Arg List */
	register int ac = 0;           /* Arg Count */

	XtSetArg(al[ac], XmNallowShellResize, TRUE ); ac++;
	BDSTA = XmCreateDialogShell ( parent, "BDSTA", al, ac );

	(void) MrmFetchWidget ( hierarchy_id, "BDSTA_FM", BDSTA, &BDSTA_FM, mrm_class );
	BDSTA_LI_MAC = XtNameToWidget ( XtNameToWidget ( BDSTA_FM, "BDSTA_LI_MACSW" ), "BDSTA_LI_MAC" );
}


  
/* Fichier : @(#)gmot_mot_bdl.c	1.6	      Release : 1.6        Date : 03/03/95
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_mot_bdl
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* fonction creant la fenetre MOTIF BDSIT 
* a partir de la hierarchie Mrm couramment ouverte
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
* CONDITIONS LIMITES :
*
******************************************************/

extern void create_BDSIT (      Widget          parent, 
				MrmHierarchy    hierarchy_id, 
				MrmCode         *mrm_class)
{
	Arg al[64];                    /* Arg List */
	register int ac = 0;           /* Arg Count */

	XtSetArg(al[ac], XmNallowShellResize, TRUE ); ac++;
	BDSIT = XmCreateDialogShell ( parent, "BDSIT", al, ac );

	(void) MrmFetchWidget ( hierarchy_id, "BDSIT_FM", BDSIT, &BDSIT_FM, mrm_class );
	BDSIT_LI_SIT = XtNameToWidget ( XtNameToWidget ( BDSIT_FM, "BDSIT_LI_SITSW" ), "BDSIT_LI_SIT" );
}


  
/* Fichier : @(#)gmot_mot_bdl.c	1.6	      Release : 1.6        Date : 03/03/95
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_mot_bdl
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* fonction creant la fenetre MOTIF BDATA 
* a partir de la hierarchie Mrm couramment ouverte
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
* CONDITIONS LIMITES :
*
******************************************************/

extern void create_BDATA (	Widget 		parent, 
				MrmHierarchy 	hierarchy_id, 
				MrmCode 	*mrm_class)
{
	Arg al[64];                    /* Arg List */
	register int ac = 0;           /* Arg Count */

	XtSetArg(al[ac], XmNtitle, "ARRET/LANCEMENT D'UNE TACHE"); ac++;
	XtSetArg(al[ac], XmNallowShellResize, TRUE ); ac++;
	BDATA = XmCreateDialogShell ( parent, "BDATA", al, ac );
	
	(void) MrmFetchWidget ( hierarchy_id, "BDATA_FM", BDATA, &BDATA_FM, mrm_class );
	BDATA_LI_MAC = XtNameToWidget ( XtNameToWidget ( BDATA_FM, "BDATA_LI_MACSW" ), "BDATA_LI_MAC" );

	BDATA_LI_TAC = XtNameToWidget ( XtNameToWidget ( BDATA_FM, "BDATA_LI_TACSW" ), "BDATA_LI_TAC" );
	BDATA_TO_AR = XtNameToWidget ( XtNameToWidget ( BDATA_FM, "BDATA_RA_BOX" ), "BDATA_TO_AR" );
	BDATA_TO_LA = XtNameToWidget ( XtNameToWidget ( BDATA_FM, "BDATA_RA_BOX" ), "BDATA_TO_LA" );
}


  
/*X*/
/* Fichier : @(#)gmot_mot_bdl.c	1.6	      Release : 1.6        Date : 03/03/95
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_mot_bdl
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* fonction creant la fenetre MOTIF GRSYN 
* a partir de la hierarchie Mrm couramment ouverte
* la fonction positionne egalement une callback
* pour ouvrir un popup menu sur clic souris
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
* CONDITIONS LIMITES :
*
******************************************************/

extern void create_GRSYN (	Widget parent, 
				MrmHierarchy hierarchy_id, 
				MrmCode *mrm_class)
{
	Arg al[64];                    /* Arg List */
	register int ac = 0;           /* Arg Count */

	XZST_03EcritureTrace (XZSTC_FONCTION, "create_GRSYN : IN \
		-- Parent %d \n",parent);


	XtSetArg(al[ac], XmNallowShellResize, TRUE ); ac++;
	XtSetArg(al[ac], XmNx, 0); ac++;
	XtSetArg(al[ac], XmNy, 200); ac++;

	GRSYN_FM=NULL;
	GRSYN = XmCreateDialogShell ( parent, "GRSYN", al, ac );
	(void) MrmFetchWidget ( 	hierarchy_id, 
					"GRSYN_FM", 
					GRSYN, 
					&GRSYN_FM, 
					mrm_class );

	XtManageChild(GRSYN_FM);

	GRSYN_SCROLL = XtNameToWidget ( GRSYN_FM, "GRSYN_SCROLL" );
	/*ac = 0;*/
	GRSYN_DA = XmCreateDrawingArea( GRSYN_SCROLL, "GRSYN_DA",
		al,ac );


	MetPopupGRSYN( GRSYN_DA );

	XZST_03EcritureTrace (XZSTC_FONCTION, "create_GRSYN : OUT \
		-- Scroll %d -- Draw %d \n",GRSYN_SCROLL,GRSYN_DA);

}	/* create_GRSYN */


  
/*X*/
/* Fichier : @(#)gmot_mot_bdl.c	1.6	      Release : 1.6        Date : 03/03/95
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_mot_bdl
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* fonction creant la fenetre MOTIF MENU 
* ( menu principal MIGRAZUR )
* a partir de la hierarchie Mrm couramment ouverte
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
* CONDITIONS LIMITES :
*
******************************************************/

extern void create_MENU (	Display 	*display, 
				Screen 		*screen, 
				char 		*app_name, 
				int 		app_argc, 
				char 		**app_argv, 
				MrmHierarchy 	hierarchy_id, 
				MrmCode 	*mrm_class)
{
	Arg al[64];
	register int ac	= 0;
	int cr;


	XtSetArg(al[ac],XmNallowShellResize, TRUE);ac++;
	XtSetArg(al[ac],XmNargc, app_argc);ac++;
	XtSetArg(al[ac],XmNargv, app_argv);ac++;
	/*XtSetArg(al[ac],XmNx, 0);ac++;
	XtSetArg(al[ac],XmNy, 0);ac++;*/
	XtSetArg(al[ac],XmNscreen, screen);ac++;
	/*XtSetArg(al[ac],XmNmwmDecorations, 0);ac++;*/


	MENU = XtAppCreateShell ( app_name, "XMigrazur",
				applicationShellWidgetClass,
				display, al, ac);
	ac = 0;
	XtSetArg(al[ac],XmNallowShellResize, TRUE);ac++;
	XtSetArg(al[ac],XmNx, 0);ac++;
	XtSetArg(al[ac],XmNy, 0);ac++;
	XtSetValues(MENU,al, ac);

	if ((cr = MrmFetchWidget (      hierarchy_id,
					"GRMACH_SYNT_FM",
					MENU,
					&GRMACH_SYNT_FM,
					mrm_class )) != MrmSUCCESS)
	{ 
		XZST_03EcritureTrace (XZSTC_FATAL,
		   "gmot_  : Erreur de fetch de la hierarchie");
	}
	else
	{
		XtManageChild(GRMACH_SYNT_FM);

	  	vg_PtrSurMenuACreer->w_menu_bar = MENU_MB;
	    	vg_PtrSurMenuACreer->w_menu_fen = MENU;
	}

}	/* Fin create_MENU */

/*X*/
/* Fichier : @(#)gmot_mot_bdl.c	1.6	      Release : 1.6        Date : 03/03/95
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE gmot_mot_bdl
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* fonction creant la fenetre MOTIF GRMACH_SYNT 
* a partir de la hierarchie Mrm couramment ouverte
* la fonction positionne egalement une callback
* pour ouvrir un popup menu sur clic souris
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
* CONDITIONS LIMITES :
*
******************************************************/

void create_GRMACH_SYNT (
	Widget parent, 
	MrmHierarchy hierarchy_id, 
	MrmCode *mrm_class)
{
	Arg al[64];                    /* Arg List */
	register int ac = 0;           /* Arg Count */


	XZST_03EcritureTrace(XZSTC_FONCTION," create_GRMACH_SYNT : IN \n");

	MENU_MB = XtNameToWidget(GRMACH_SYNT_FM,"MENU_MB");

	GRMACH_SYNT_SCROLL = 
		XtNameToWidget(GRMACH_SYNT_FM,"GRMACH_SYNT_SCROLL");
	ac = 0;
	GRMACH_SYNT_DA = 
		XmCreateDrawingArea( GRMACH_SYNT_SCROLL,"GRMACH_SYNT_DA",al,ac);


	XZST_03EcritureTrace(XZSTC_FONCTION,
		" create_GRMACH_SYNT : OUT -- Menu Bar %d -- \
		Widegt SCROLL %d -- Widget Draw %d \n",
		MENU_MB,GRMACH_SYNT_SCROLL,GRMACH_SYNT_DA);


}	/* create_GRMACH_SYNT */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de creer le Widget d'affichage Bilan Config
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void create_LIST_BC(
	Widget parent, 
	MrmHierarchy hierarchy_id, 
	MrmCode *mrm_class)

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
	static char *version="@(#)gmot_mot_bdl.c	1.6 1.6 03/03/95 : create_LIST_BC " ;
	Arg al[64];                    /* Arg List */
	register int ac = 0;           /* Arg Count */

	XtSetArg(al[ac], XmNallowShellResize, TRUE ); ac++;
	LIST_BC = XmCreateDialogShell ( parent, "LIST_BC", al, ac );
	
	(void) MrmFetchWidget ( hierarchy_id, "LIST_BC_FM", LIST_BC, &LIST_BC_FM, mrm_class );
	XtManageChild(LIST_BC_FM);


	LIST_BC_LI = XtNameToWidget ( XtNameToWidget(LIST_BC_FM,"LIST_BC_LISW"), "LIST_BC_LI" );


}	/* Fin create_LIST_BC */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de creer le Widget d'affichage Bon de Livraison
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void create_LIST_BL(
	Widget parent, 
	MrmHierarchy hierarchy_id, 
	MrmCode *mrm_class)

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
	static char *version="@(#)gmot_mot_bdl.c	1.6 1.6 03/03/95 : create_LIST_BL " ;
	Arg al[64];                    /* Arg List */
	register int ac = 0;           /* Arg Count */

	XtSetArg(al[ac], XmNallowShellResize, TRUE ); ac++;
	LIST_BL = XmCreateDialogShell ( parent, "LIST_BL", al, ac );
	
	(void) MrmFetchWidget ( hierarchy_id, "LIST_BL_FM", LIST_BL, &LIST_BL_FM, mrm_class );
	LIST_BL_LI = XtNameToWidget ( XtNameToWidget(LIST_BL_FM,"LIST_BL_LISW"), "LIST_BL_LI" );

}	/* Fin create_LIST_BL */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de creer le Widget d'affichage CAtalogue Archive
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void create_LIST_ARC(
	Widget parent, 
	MrmHierarchy hierarchy_id, 
	MrmCode *mrm_class)

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
	static char *version="@(#)gmot_mot_bdl.c	1.6 1.6 03/03/95 : create_LIST_ARC " ;
	Arg al[64];                    /* Arg List */
	register int ac = 0;           /* Arg Count */

	XtSetArg(al[ac], XmNallowShellResize, TRUE ); ac++;
	LIST_ARC = XmCreateDialogShell ( parent, "LIST_ARC", al, ac );
	
	(void) MrmFetchWidget ( hierarchy_id, "LIST_ARC_FM", LIST_ARC, &LIST_ARC_FM, mrm_class );
	LIST_ARC_LI = XtNameToWidget ( XtNameToWidget(LIST_ARC_FM,"LIST_ARC_LISW"), "LIST_ARC_LI" );

}	/* Fin create_LIST_ARC */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de creer le Widget d'affichage Bilan Rest BAse Hist
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void create_LIST_BRBH(
	Widget parent, 
	MrmHierarchy hierarchy_id, 
	MrmCode *mrm_class)

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
	static char *version="@(#)gmot_mot_bdl.c	1.6 1.6 03/03/95 : create_LIST_BRBH " ;
	Arg al[64];                    /* Arg List */
	register int ac = 0;           /* Arg Count */

	XtSetArg(al[ac], XmNallowShellResize, TRUE ); ac++;
	LIST_BRBH = XmCreateDialogShell ( parent, "LIST_BRBH", al, ac );
	
	(void) MrmFetchWidget ( hierarchy_id, "LIST_BRBH_FM", LIST_BRBH, &LIST_BRBH_FM, mrm_class );
	LIST_BRBH_LI = XtNameToWidget ( XtNameToWidget(LIST_BRBH_FM,"LIST_BRBH_LISW"), "LIST_BRBH_LI" );

}	/* Fin create_LIST_BRBH */




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de creer le Widget d'affichage Bilan Svg BAse Hist
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void create_LIST_BSBH(
	Widget parent, 
	MrmHierarchy hierarchy_id, 
	MrmCode *mrm_class)

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
	static char *version="@(#)gmot_mot_bdl.c	1.6 1.6 03/03/95 : create_LIST_BSBH " ;
	Arg al[64];                    /* Arg List */
	register int ac = 0;           /* Arg Count */

	XtSetArg(al[ac], XmNallowShellResize, TRUE ); ac++;
	LIST_BSBH = XmCreateDialogShell ( parent, "LIST_BSBH", al, ac );
	
	(void) MrmFetchWidget ( hierarchy_id, "LIST_BSBH_FM", LIST_BSBH, &LIST_BSBH_FM, mrm_class );
	LIST_BSBH_LI = XtNameToWidget ( XtNameToWidget(LIST_BSBH_FM,"LIST_BSBH_LISW"), "LIST_BSBH_LI" );

}	/* Fin create_LIST_BSBH */




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de creer le Widget d'affichage Bilan Svg Hebdo
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void create_LIST_HEBDO(
	Widget parent, 
	MrmHierarchy hierarchy_id, 
	MrmCode *mrm_class)

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{
	static char *version="@(#)gmot_mot_bdl.c	1.6 1.6 03/03/95 : create_LIST_HEBDO " ;
	Arg al[64];                    /* Arg List */
	register int ac = 0;           /* Arg Count */

	XtSetArg(al[ac], XmNallowShellResize, TRUE ); ac++;
	LIST_HEBDO = XmCreateDialogShell ( parent, "LIST_HEBDO", al, ac );
	
	(void) MrmFetchWidget ( hierarchy_id, "LIST_HEBDO_FM", LIST_HEBDO, &LIST_HEBDO_FM, mrm_class );
	LIST_HEBDO_LI = XtNameToWidget ( XtNameToWidget(LIST_HEBDO_FM,"LIST_HEBDO_LISW"), "LIST_HEBDO_LI" );

}	/* Fin create_LIST_HEBDO */
