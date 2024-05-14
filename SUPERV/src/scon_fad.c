/*E*/
/* Fichier : $Id: scon_fad.c,v 1.6 1996/02/20 09:21:49 milleville Exp $      Release : $Revision: 1.6 $        Date : $Date: 1996/02/20 09:21:49 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_fad * FICHIER scon_fad.c
******************************************************
* DESCRIPTION DU MODULE :
*
* ce module contient les fonctions appelees a partir
* du sous-menu "Environnement de la machine"
*
* 1) Fonction "Ajout d'un site" 
*    CreSitFad_CB puis 
*    BDSIT_VAL 
* 
* 2) Fonction "Ajout d'un calculateur"
*    CreMacFad_CB puis
*    BDCAL_VAL 
*
* 3) Fonction "Ajout d'un sous-systeme"
*    CreSsyFad_CB puis
*    BDSSY_VAL
*
* 4) Fonction "Ajout d'une tache"
*    CreTacFad_CB puis
*    BDTAS_VAL
*
* 5) Fonction "Ajout d'une entite"
*    CreEntFad_CB puis
*    BDENT_VAL
*
* 6) Fonction "Suppression d'un site" 
*    DesSitFad_CB puis 
*    BDSIT_VAL puis
*    BDCON2_VAL
*
* 7) Fonction "Suppression d'un calculateur"
*    CreMacFad_CB puis
*    BDCAL_VAL puis
*    BDCON2_VAL
*
* 8) Fonction "Suppression d'un sous-systeme"
*    CreSsyFad_CB puis
*    BDSSY_VAL puis
*    BDCON2_VAL
*
* 9) Fonction "Suppression d'une tache"
*    CreTacFad_CB puis
*    BDTAS_VAL puis
*    BDCON2_VAL
*
* 10) Fonction "Suppression d'une entite"
*    CreEntFad_CB puis
*    BDENT_VAL puis
*    BDCON2_VAL
*
******************************************************
* HISTORIQUE :
*
* T.Milleville 30 Aou 1994	: Creation
* T.Milleville 20 Mar 1995	: Remplacement de  XDC_PATHREF_FICADM
*		par XDC_PATHREF_FICADM V1.4
* T.Milleville 25 Jul 1995	: Mise a jour enetet V1.5
* P.Niepceron  19 Fev  1996        : Remplacement de XDC_PATHREF_FICADM
*       par XDC_PATHTRAVFICADM  V1.6
******************************************************/

/* fichiers inclus */

#include "scon_fad.h"
#include "scon_ser.h"
#include "scon_var.h"

/* definitions de constantes */


/* definitions de types locaux */


/* definition de macro locales */


/* declaration de variables locales */

static XZSCT_NomMachine    vs_nomMachine;
static XZSXT_ListeSSysteme vs_listeSSysteme;
static XZSXT_ListeSite     vs_listeSite; 
static XZSCT_NomEntite     vs_nomEntite; 
static int                 vs_typeEntite; 
static XZSCT_NomSSysteme   vs_nomSSysteme; 
static int                 vs_typeMachine;
static int                 vs_temporaire;
static XZSCT_NomSite       vs_nomSite;
static XZSCT_NomTache      vs_nomTache;

static char *version = "$Id: scon_fad.c,v 1.6 1996/02/20 09:21:49 milleville Exp $ : scon_fad" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/* Fichier : $Id: scon_fad.c,v 1.6 1996/02/20 09:21:49 milleville Exp $      Release : $Revision: 1.6 $        Date : $Date: 1996/02/20 09:21:49 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_fad
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* callback de "Creation de calculateur" 
* du sous-menu "Fichier d'administration"
*
******************************************************
* SEQUENCE D'APPEL :
*/

void
CreMacFad_CB( Widget w, XtPointer client_data, XtPointer xt_call_data )

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
	static char *version = "$Id: scon_fad.c,v 1.6 1996/02/20 09:21:49 milleville Exp $ : CreMacFad_CB" ;
  /* valorisation de la variable mode operatoire */
  vg_operation = CG_CRECAL;

  EcritChampsCAL( "" );

  XtManageChild( BDCAL_FM );
}


/*X*/
/* Fichier : $Id: scon_fad.c,v 1.6 1996/02/20 09:21:49 milleville Exp $      Release : $Revision: 1.6 $        Date : $Date: 1996/02/20 09:21:49 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_fad
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* callback de "Creation d'un sous-systeme"
* du sous-menu "Fichier d'administration"
*
******************************************************
* SEQUENCE D'APPEL :
*/

void
CreSsyFad_CB( Widget w, XtPointer client_data, XtPointer xt_call_data )

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
	static char *version = "$Id: scon_fad.c,v 1.6 1996/02/20 09:21:49 milleville Exp $ : CreSsyFad_CB" ;
  /* valorisation de la variable mode operatoire */ 
  vg_operation = CG_CRESSY;

  EcritChampsSSY( "" );

  XtManageChild( BDSSY_FM );
}


/* Fichier : $Id: scon_fad.c,v 1.6 1996/02/20 09:21:49 milleville Exp $      Release : $Revision: 1.6 $        Date : $Date: 1996/02/20 09:21:49 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_fad
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* callback de "Creation d'une tache"
* du sous-menu "Fichier d'administration"
*
******************************************************
* SEQUENCE D'APPEL :
*/

void CreTacFad_CB( 
	Widget w, 
	XtPointer client_data, 
	XtPointer xt_call_data )

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
	static char *version = "$Id: scon_fad.c,v 1.6 1996/02/20 09:21:49 milleville Exp $ : CreTacFad_CB" ;
  /* valorisation de la variable mode operatoire */ 
  /* valorisation de la variable mode operatoire */
  vg_operation = CG_CRETAS;

  EcritChampsTAS( "", "" );

  XtManageChild( BDTAS_FM );
}


/* Fichier : $Id: scon_fad.c,v 1.6 1996/02/20 09:21:49 milleville Exp $      Release : $Revision: 1.6 $        Date : $Date: 1996/02/20 09:21:49 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_fad
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* callback de "Creation d'un site" 
* du sous-menu "Fichier d'administration"
*
******************************************************
* SEQUENCE D'APPEL :
*/

void
CreSitFad_CB( Widget w, XtPointer client_data, XtPointer xt_call_data )

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
	static char *version = "$Id: scon_fad.c,v 1.6 1996/02/20 09:21:49 milleville Exp $ : CreSitFad_CB" ;
  /* valorisation de la variable mode operatoire */
  vg_operation = CG_CRESIT;

  EcritChampsSIT( "" );

  XtManageChild( BDSIT_FM );
}


/* Fichier : $Id: scon_fad.c,v 1.6 1996/02/20 09:21:49 milleville Exp $      Release : $Revision: 1.6 $        Date : $Date: 1996/02/20 09:21:49 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_fad
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* callback de "Creation d'une entite"
* du sous-menu "Fichier d'administration"
*
******************************************************
* SEQUENCE D'APPEL :
*/

void
CreEntFad_CB( Widget w, XtPointer client_data, XtPointer xt_call_data )

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
	static char *version = "$Id: scon_fad.c,v 1.6 1996/02/20 09:21:49 milleville Exp $ : CreEntFad_CB" ;
  /* valorisation de la variable mode operatoire */
  vg_operation = CG_CREENT;

  /* effacement des champs texte de BDENT */
  EcritChampsENT( "", "", "", "" );

  XtManageChild( BDENT_FM );
}

/* Fichier : $Id: scon_fad.c,v 1.6 1996/02/20 09:21:49 milleville Exp $      Release : $Revision: 1.6 $        Date : $Date: 1996/02/20 09:21:49 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_fad
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* callback de "Creation de calculateur" 
* du sous-menu "Fichier d'administration"
*
******************************************************
* SEQUENCE D'APPEL :
*/


void
DesMacFad_CB( Widget w, XtPointer client_data, XtPointer xt_call_data )

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
	static char *version = "$Id: scon_fad.c,v 1.6 1996/02/20 09:21:49 milleville Exp $ : DesMacFad_CB";
  /* valorisation de la variable mode operatoire */
  vg_operation = CG_DESCAL;

  EcritChampsCAL( "" );

  XtManageChild( BDCAL_FM );
}


/* Fichier : $Id: scon_fad.c,v 1.6 1996/02/20 09:21:49 milleville Exp $      Release : $Revision: 1.6 $        Date : $Date: 1996/02/20 09:21:49 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_fad
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* callback de "Suppression d'un sous-systeme"
* du sous-menu "Fichier d'administration"
*
******************************************************
* SEQUENCE D'APPEL :
*/

void
DesSsyFad_CB( Widget w, XtPointer client_data, XtPointer xt_call_data )

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
	static char *version = "$Id: scon_fad.c,v 1.6 1996/02/20 09:21:49 milleville Exp $ : DesSsyFad_CB";
  /* valorisation de la variable mode operatoire */
  vg_operation = CG_DESSSY;

  EcritChampsSSY( "" );

  XtManageChild( BDSSY_FM );
}


/* Fichier : $Id: scon_fad.c,v 1.6 1996/02/20 09:21:49 milleville Exp $      Release : $Revision: 1.6 $        Date : $Date: 1996/02/20 09:21:49 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_fad
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* callback de "Suppression d'une tache"
* du sous-menu "Fichier d'administration"
*
******************************************************
* SEQUENCE D'APPEL :
*/

void
DesTacFad_CB( Widget w, XtPointer client_data, XtPointer xt_call_data )

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
	static char *version = "$Id: scon_fad.c,v 1.6 1996/02/20 09:21:49 milleville Exp $ : DesTacFad_CB";
  /* valorisation de la variable mode operatoire */
  vg_operation = CG_DESTAS;

  EcritChampsTAS( "", "" );

  XtManageChild( BDTAS_FM );
}


/* Fichier : $Id: scon_fad.c,v 1.6 1996/02/20 09:21:49 milleville Exp $      Release : $Revision: 1.6 $        Date : $Date: 1996/02/20 09:21:49 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_fad
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* callback de "Suppression d'un site" 
* du sous-menu "Fichier d'administration"
*
******************************************************
* SEQUENCE D'APPEL :
*/

void
DesSitFad_CB( Widget w, XtPointer client_data, XtPointer xt_call_data )

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
	static char *version = "$Id: scon_fad.c,v 1.6 1996/02/20 09:21:49 milleville Exp $ : DesSitFad_CB";
  /* valorisation de la variable mode operatoire */
  vg_operation = CG_DESSIT;

  EcritChampsSIT( "" );

  XtManageChild( BDSIT_FM );
}


/* Fichier : $Id: scon_fad.c,v 1.6 1996/02/20 09:21:49 milleville Exp $      Release : $Revision: 1.6 $        Date : $Date: 1996/02/20 09:21:49 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_fad
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* callback de "Suppression d'une entite"
* du sous-menu "Fichier d'administration"
*
******************************************************
* SEQUENCE D'APPEL :
*/

void
DesEntFad_CB( Widget w, XtPointer client_data, XtPointer xt_call_data )

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
	static char *version = "$Id: scon_fad.c,v 1.6 1996/02/20 09:21:49 milleville Exp $ : DesEntFad_CB";
  /* valorisation de la variable mode operatoire */
  vg_operation = CG_DESENT;

  EcritChampsENT( "", "", "", "" );

  XtManageChild( BDENT_FM );
}


/* Fichier : $Id: scon_fad.c,v 1.6 1996/02/20 09:21:49 milleville Exp $      Release : $Revision: 1.6 $        Date : $Date: 1996/02/20 09:21:49 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_fad
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* callback de validation de la boite de dialogue BDCAL
* - lecture du nom de la machine 
* - selon le mode operatoire ( CREATION ou DESTRUCTION )
*   appel de XZSX_07Cmachine ou demande de 
*   confirmation pour appel a XZSX_08Dmachine 
*
******************************************************
* SEQUENCE D'APPEL :
*/
 
void BDCAL_VAL (
	Widget w, 
	XtPointer client_data, 
	XtPointer xt_call_data )

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
	static char *version = "$Id: scon_fad.c,v 1.6 1996/02/20 09:21:49 milleville Exp $ : BDCAL_VAL";
  XZSXT_Machine     vl_strucMac;
  int               vl_cr;
	 
  /* Recuperation des champs de BDCAL */
  vl_cr = LitChampsCAL( vs_nomMachine );

  if ( vl_cr == CG_VIDE )
  {
     AfficheErreur( "Saisissez le nom du calculateur, SVP" );
     return;
  }

/* Cas de CREATION de machine    */
/* ------------------------------*/
if (vg_operation == CG_CRECAL)
{
  /* appel de XZSX_01MachineLecture qui rend la structure */
  /* vl_strucMac si la machine existe                     */
  vl_cr = XZSX_01MachineLecture( vs_nomMachine, 
				 XDC_PATHTRAVFICADM, 
				 &vl_strucMac ) ;
	       
  /* si la machine existe, appel de XZSX_07Cmachine */ 
  if ( vl_cr == XDC_OK )
  {
    vl_cr = XZSX_07Cmachine ( vl_strucMac, XDC_PATHTRAVFICADM ); 

    /* on efface la boite de dialogue BDCAL */
    XtUnmanageChild ( BDCAL_FM );
  }
  else
  {
      if ( vl_cr == XZSXC_FIC_INV )
	{
	  AfficheErreur("Erreur de lecture fichier");
	  return;
	}
      else
	{
	  /* la machine n'existe pas: erreur */
	  AfficheErreur( "La machine n'existe pas..");
	  return;
	}
  }
}
	
/* Cas de DESTRUCTION de machine */
/* ------------------------------*/
else
 if (vg_operation == CG_DESCAL)
 {
	    
   /* demande de confirmation - affichage de BDCON2   */
   /* ( apres confirmation, appel a XZSX_08Dmachine ) */
   DemandeConf2( "Confirmez-vous la destruction du calculateur ?");
 }
}


/* Fichier : $Id: scon_fad.c,v 1.6 1996/02/20 09:21:49 milleville Exp $      Release : $Revision: 1.6 $        Date : $Date: 1996/02/20 09:21:49 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_fad
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* callback de validation de la boite de dialogue BDSSY
* - lecture du nom du sous-systeme
* - selon le mode operatoire ( CREATION ou DESTRUCTION )
*   appel de XZSX_09Cssysteme ou demande de confirmation
*   pour appel a XZSX_10Dssysteme 
*
******************************************************
* SEQUENCE D'APPEL :
*/
 
void BDSSY_VAL (
	Widget w, 
	XtPointer client_data, 
	XtPointer xt_call_data )

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
	static char *version = "$Id: scon_fad.c,v 1.6 1996/02/20 09:21:49 milleville Exp $ : BDSSY_VAL";
  int               vl_cr;
	 
  /* Recuperation des champs de BDSSY */
  vl_cr = LitChampsSSY( vs_listeSSysteme.NomSSysteme, 
			&(vs_listeSSysteme.TypeMachine) );

  if ( vl_cr == CG_VIDE )
  {
     AfficheErreur( "Saisissez tous les champs, SVP" );
     return;
  }

/* Cas de CREATION de sous-systeme    */
/* -----------------------------------*/
if (vg_operation == CG_CRESSY)
{
  /* appel a XZSX_09Cssysteme avec vs_listeSSysteme */
  vl_cr = XZSX_09Cssysteme( vs_listeSSysteme, XDC_PATHTRAVFICADM );

  /* on efface la boite de dialogue BDSSY */
  XtUnmanageChild ( BDSSY_FM );
}
	
/* Cas de DESTRUCTION de sous-systeme */
/* -----------------------------------*/
else
 if (vg_operation == CG_DESSSY)
 {
	    
   /* demande de confirmation - affichage de BDCON2   */
   /* ( apres confirmation, appel a XZSX_10Dssysteme ) */
   DemandeConf2( "Confirmez-vous la destruction du sous-systeme ?");
 }
}



/* Fichier : $Id: scon_fad.c,v 1.6 1996/02/20 09:21:49 milleville Exp $      Release : $Revision: 1.6 $        Date : $Date: 1996/02/20 09:21:49 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_fad
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* callback de validation de la boite de dialogue BDSIT
* - lecture du nom du site
* - selon le mode operatoire ( CREATION ou DESTRUCTION )
*   appel de XZSX_25Csite ou demande de confirmation
*   pour appel a XZSX_26Dsite 
*
******************************************************
* SEQUENCE D'APPEL :
*/
 
void
BDSIT_VAL (Widget w, XtPointer client_data, XtPointer xt_call_data )

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
	static char *version = "$Id: scon_fad.c,v 1.6 1996/02/20 09:21:49 milleville Exp $ : BDSIT_VAL";
  int               vl_cr;
	 
  /* Recuperation des champs de BDSIT */
  vl_cr = LitChampsSIT( vs_listeSite.NomSite );

  if ( vl_cr == CG_VIDE )
  {
     AfficheErreur( "Saisissez le nom du site, SVP" );
     return;
  }

/* Cas de CREATION de site    */
/* -----------------------------------*/
if (vg_operation == CG_CRESIT)
{
  /* creation du site par XZSX_25Csite */ 
  vl_cr = XZSX_25Csite( vs_listeSite.NomSite, XDC_PATHTRAVFICADM );

  /* on efface la boite de dialogue BDSIT */
  XtUnmanageChild ( BDSIT_FM );
}
	
/* Cas de DESTRUCTION de site */
/* -----------------------------------*/
else
 if (vg_operation == CG_DESSIT)
 {
	    
   /* demande de confirmation - affichage de BDCON2   */
   /* ( apres confirmation, appel a XZSX_26Dsite )    */
   DemandeConf2( "Confirmez-vous la destruction du site ?");
 }
}


/* Fichier : $Id: scon_fad.c,v 1.6 1996/02/20 09:21:49 milleville Exp $      Release : $Revision: 1.6 $        Date : $Date: 1996/02/20 09:21:49 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_fad
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* callback de validation de la boite de dialogue BDTAS
* - lecture du nom de la tache, du sous-systeme et du
*   type de machine
* - selon le mode operatoire ( CREATION ou DESTRUCTION )
*   appel de XZSX_12Ctache ou demande de confirmation
*   pour appel a XZSX_13Dtache 
*
******************************************************
* SEQUENCE D'APPEL :
*/
 
void BDTAS_VAL (
	Widget w, 
	XtPointer client_data, 
	XtPointer xt_call_data )

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
	static char *version = "$Id: scon_fad.c,v 1.6 1996/02/20 09:21:49 milleville Exp $ : BDTAS_VAL";
  int               vl_cr;
	 
  /* Recuperation des champs de BDTAS             */
  vl_cr = LitChampsTAS( vs_nomSSysteme, 
		       &vs_typeMachine, 
			vs_nomTache, 
			&vs_temporaire);

  if ( vl_cr == CG_VIDE )
  {
     AfficheErreur( "Saisissez tous les champs, SVP" );
     return;
  }

/* Cas de CREATION de tache    */
/* ----------------------------*/
if (vg_operation == CG_CRETAS)
{
  /* appel a XZSX_12Ctache */ 
  vl_cr = XZSX_12Ctache( vs_nomSSysteme, 
			 vs_typeMachine, 
			 vs_nomTache,
			 vs_temporaire, 
			 XDC_PATHTRAVFICADM );

  /* on efface la boite de dialogue BDTAS */
  XtUnmanageChild ( BDTAS_FM );
}
	
/* Cas de DESTRUCTION de tache */
/* ----------------------------*/
else
 if (vg_operation == CG_DESTAS)
 {
	    
   /* demande de confirmation - affichage de BDCON2   */
   /* ( apres confirmation, appel a XZSX_13Dtache )    */
   DemandeConf2( "Confirmez-vous la destruction de la tache ?");
 }
}


/* Fichier : $Id: scon_fad.c,v 1.6 1996/02/20 09:21:49 milleville Exp $      Release : $Revision: 1.6 $        Date : $Date: 1996/02/20 09:21:49 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_fad
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* callback de validation de la boite de dialogue BDENT
* - lecture du type d'entite, et selon le type des noms
*   de machine, d'entite, tache ou site;
* - selon le mode operatoire ( CREATION ou DESTRUCTION )
*   appel de XZSX_14Centite ou demande de confirmation
*   pour appel a XZSX_15Dentite 
*
******************************************************
* SEQUENCE D'APPEL :
*/
 
void
BDENT_VAL (Widget w, XtPointer client_data, XtPointer xt_call_data )

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
	static char *version = "$Id: scon_fad.c,v 1.6 1996/02/20 09:21:49 milleville Exp $ : BDENT_VAL";
  int               vl_cr;
	 
  /* Recuperation des champs de BDENT   */
  vl_cr = LitChampsENT( &vs_typeEntite, vs_nomSite, vs_nomEntite );

  if ( vl_cr == CG_VIDE )
  {
     AfficheErreur( "Saisissez tous les champs, SVP" );
     return;
  }

/* Cas de CREATION d'entite    */
/* ----------------------------*/
if (vg_operation == CG_CREENT)
{
  /* creation de l'entite par XZSX_14Centite */ 
  vl_cr = XZSX_14Centite( vs_nomEntite, 
			  vs_typeEntite, 
			  vs_nomSite, 
			  XDC_PATHTRAVFICADM );

  /* on efface la boite de dialogue BDENT */
  XtUnmanageChild ( BDENT_FM );
}
	
/* Cas de DESTRUCTION d'entite */
/* ----------------------------*/
else
 if (vg_operation == CG_DESENT)
 {
	    
   /* demande de confirmation - affichage de BDCON2   */
   /* ( apres confirmation, appel a XZSX_15Dentite )    */
   DemandeConf2( "Confirmez-vous la destruction de l'entite ?");
 }
}


/* Fichier : $Id: scon_fad.c,v 1.6 1996/02/20 09:21:49 milleville Exp $      Release : $Revision: 1.6 $        Date : $Date: 1996/02/20 09:21:49 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_fad
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* callback de validation de la boite de dialogue BDCON2
* de confirmation de destruction 
* vg_operation permet de connaitre l'entite que l'on 
* desire detruire
*
******************************************************
* SEQUENCE D'APPEL :
*/
 
void
BDCON2_VAL (Widget w, XtPointer client_data, XtPointer xt_call_data )

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
	static char *version = "$Id: scon_fad.c,v 1.6 1996/02/20 09:21:49 milleville Exp $ : BDCON2_VAL";
  int vl_cr;

  switch ( vg_operation )
  {
  case CG_DESCAL:
 	  /* appel a XZSX_08Dmachine avec vs_nomMachine */  
	  vl_cr = XZSX_08Dmachine( vs_nomMachine, 
				   XDC_PATHTRAVFICADM );

	  /* disparition de BDCAL */
	  XtUnmanageChild ( BDCAL_FM );
	  break;

  case CG_DESSSY:
 	  /* destruction du sous-systeme par XZSX_10Dssysteme */  
          vl_cr = XZSX_10Dssysteme( vs_listeSSysteme, 
				    XDC_PATHTRAVFICADM );

	  /* disparition de BDSSY */
	  XtUnmanageChild ( BDSSY_FM );
	  break;

  case CG_DESSIT:
 	  /* destruction du site par appel a XZSX_26Dsite */
	  vl_cr = XZSX_26Dsite( vs_listeSite.NomSite, 
			        XDC_PATHTRAVFICADM );

	  /* disparition de BDSIT */
	  XtUnmanageChild ( BDSIT_FM );
	  break;

  case CG_DESENT:
 	  /* destruction de l'entite par XZSX_15Dentite */
	  vl_cr = XZSX_15Dentite( vs_nomEntite, 
				  vs_typeEntite, 
				  XDC_PATHTRAVFICADM );

	  /* disparition de BDENT */
	  XtUnmanageChild ( BDENT_FM );
	  break;

  case CG_DESTAS:
 	  /* destruction de la tache par XZSX_13Dtache */
	  vl_cr = XZSX_13Dtache( vs_nomSSysteme, 
			    	 vs_typeMachine, 
				 vs_nomTache, 
			    	 vs_temporaire, 
				 XDC_PATHTRAVFICADM );

	  /* disparition de BDTAS */
	  XtUnmanageChild ( BDTAS_FM );
	  break;

  default:
	  /* erreur */
	  break;
  }

XtUnmanageChild( BDCON2_FM );

}
