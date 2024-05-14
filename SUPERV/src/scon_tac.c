/*E*/
/* Fichier : $Id: scon_tac.c,v 1.8 1996/02/20 09:21:53 milleville Exp $      Release : $Revision: 1.8 $        Date : $Date: 1996/02/20 09:21:53 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_tac * FICHIER scon_tac.c
******************************************************
* DESCRIPTION DU MODULE :
*
* module contenant les callbacks appelees a partir du
* sous-menu "Caracteristiques de la machine" de scon
*
******************************************************
* HISTORIQUE :
*
* T.Milleville 30 Aou 1994	: Creation
* T.Milleville 20 Mar 1995	: Remplacement de XDC_PATHREF_FICADM
*	par XDC_PATHREF_FICADM V1.6
* T.Milleville 25 Jul  1995	: Mise a jour entete V1.7
* P.Niepceron  19 Fev  1996	: Remplacement de XDC_PATHREF_FICADM
*	par XDC_PATHTRAVFICADM  V1.8
*******************************************************/

/* fichiers inclus */

#include "scon_ser.h"
#include "scon_var.h"

#include "scon_tac.h"

/* definitions de constantes */


/* definitions de types locaux */


/* definition de macro locales */


/* declaration de variables locales */

static XZSCT_NomMachine  vs_nomMac		= "";
static XZSCT_NomMachine  vs_nomMac_dest		= "";
static XZSCT_NomMachine  vs_nomMac_exped	= "";
static XZSCT_NomTache    vs_nomTac		= "";

static Widget                    vs_menu_parent;

static char *version = "$Id: scon_tac.c,v 1.8 1996/02/20 09:21:53 milleville Exp $ : scon_tac" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */

/*X*/
/* Fichier : $Id: scon_tac.c,v 1.8 1996/02/20 09:21:53 milleville Exp $      Release : $Revision: 1.8 $        Date : $Date: 1996/02/20 09:21:53 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_tac
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* - callback appele par le PushButton "Creation d'une tache" 
* du sous-menu "Caracteristiques de la machine"
*
* - a la fin de l'execution de ce callback, la boite de dialogue
* BDCTM s'affiche
* - la suite des traitements se trouve donc dans le 
* callback de validation de BDCTM: BDCTM_VAL
*
******************************************************
* SEQUENCE D'APPEL :
*/
 
void
CreTac_CB (Widget w, XtPointer client_data, XtPointer xt_call_data )
 
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
	static char *version = "$Id: scon_tac.c,v 1.8 1996/02/20 09:21:53 milleville Exp $ : CreTac_CB" ;

/* on stocke dans une variable statique le mode operatoire */
vg_operation = CG_CRE_TAC;

/* affichage de BDCTM */
EcritChampsCTM( "", "" );
XtManageChild ( BDCTM_FM );
}

/*X*/
/* Fichier : $Id: scon_tac.c,v 1.8 1996/02/20 09:21:53 milleville Exp $      Release : $Revision: 1.8 $        Date : $Date: 1996/02/20 09:21:53 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_tac
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* - callback appele par le PushButton "Destruction d'une tache" 
* du sous-menu "Caracteristiques de la machine"
*
* - a la fin de l'execution de ce callback, la boite de dialogue
* BDCTM s'affiche
* - la suite des traitements se trouve donc dans le 
* callback de validation de BDCTM: BDCTM_VAL
*
******************************************************
* SEQUENCE D'APPEL :
*/
 
void
DesTac_CB (Widget w, XtPointer client_data, XtPointer xt_call_data )
 
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
	static char *version = "$Id: scon_tac.c,v 1.8 1996/02/20 09:21:53 milleville Exp $ : DesTac_CB" ;

/* on stocke dans une variable statique le mode operatoire  */
vg_operation = CG_DES_TAC;
 
/* affichage de BDCTM */
EcritChampsCTM( "", "" );
XtManageChild ( BDCTM_FM );
}

/*X*/
/* Fichier : $Id: scon_tac.c,v 1.8 1996/02/20 09:21:53 milleville Exp $      Release : $Revision: 1.8 $        Date : $Date: 1996/02/20 09:21:53 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_tac
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* callback de validation de la boite de dialogue BDCTM
* - lecture du nom de la machine et de la tache
* - selon le mode operatoire ( CREATION ou DESTRUCTION )
*   appel de XZSX_02TacheLecture ou 
*   demande de confirmation avant appel a XZSX_05Tache_Detruire
*
******************************************************
* SEQUENCE D'APPEL :
*/
 
void
BDCTM_VAL (Widget w, XtPointer client_data, XtPointer xt_call_data )

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
	static char *version = "$Id: scon_tac.c,v 1.8 1996/02/20 09:21:53 milleville Exp $ : BDCTM_VAL" ;

  XZSXT_Tache   vl_strucTac;
  int           vl_cr;
  char		vl_msg[80];
	 
  /* Recuperation des champs de BDCTM */
  vl_cr = LitChampsCTM( vs_nomMac, vs_nomTac );

  if ( vl_cr == CG_VIDE )
  {
     AfficheErreur( "Saisissez tous les champs, SVP" );
     return;
  }

/* Cas de CREATION de tache      */
/* ------------------------------*/
if (vg_operation == CG_CRE_TAC)
  {
   
  /* appel de XZSX_02TacheLecture qui rend la structure SXT_Tache */
  vl_cr = XZSX_02TacheLecture(  vs_nomMac, 
				vs_nomTac, 
			        XDC_PATHTRAVFICADM, 
				&vl_strucTac ) ;
	       
  /* si la machine et la tache existait, mise des champs */
  /* de la structure SXT_Tache dans les champs de BDTAC */
		    
  if ( vl_cr == XDC_OK )
    {
      EcritChampsTAC( vl_strucTac, CG_NONVIDE );
    }
  else
    if ( vl_cr == XZSXC_NOM_INV )
      {
	strcpy( vl_strucTac.NomTache, vs_nomTac );
	EcritChampsTAC( vl_strucTac, CG_VIDE);
      }
    else
      {
        /* la machine n'existe pas: erreur */
        AfficheErreur( "La machine n'existe pas..");
        return;
      }

  /* affichage de BDTAC (la suite des operations dans */
  /* BDTAC_VAL, callback de validation de BDTAC )     */
  XtManageChild ( BDTAC_FM );

  }
	
/* Cas de DESTRUCTION de tache   */
/* ------------------------------*/
else
 if (vg_operation == CG_DES_TAC)
   {
   /* demande de confirmation (affichage de BDCON1)     */
   sprintf( vl_msg, "Confirmez-vous la destruction de la tache %s ?", vs_nomTac );
   DemandeConf1( vl_msg );
   }
		      
XtUnmanageChild ( BDCTM_FM );

}


/*X*/
/* Fichier : $Id: scon_tac.c,v 1.8 1996/02/20 09:21:53 milleville Exp $      Release : $Revision: 1.8 $        Date : $Date: 1996/02/20 09:21:53 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_tac
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*
******************************************************
* SEQUENCE D'APPEL :
*/

BDTAC_VAL(Widget w, XtPointer client_data, XtPointer xt_call_data )

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
	static char *version = "$Id: scon_tac.c,v 1.8 1996/02/20 09:21:53 milleville Exp $ : BDTAC_VAL" ;
  int         vl_cr;
  XZSXT_Tache vl_strucTac;

  /* lecture des champs de BDTAC   */
  vl_cr = LitChampsTAC( &vl_strucTac );

  if ( vl_cr == CG_VIDE )
  {
    AfficheErreur( "Entrez un nom de tache, SVP" );
    return;
  }

  /* appel de XZSX_04Tache_Ecriture   */
  vl_cr = XZSX_04Tache_Ecriture( vs_nomMac, vl_strucTac, XDC_PATHTRAVFICADM ); 

  XtUnmanageChild ( BDTAC_FM );
}


/*X*/
/* Fichier : $Id: scon_tac.c,v 1.8 1996/02/20 09:21:53 milleville Exp $      Release : $Revision: 1.8 $        Date : $Date: 1996/02/20 09:21:53 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_tac
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* callback appele par le PushButton "Creation d'une 
* machine" du sous-menu "Caracteristiques de la Machine"

* - a la fin de l'execution de ce callback, la boite de 
*   dialogue BDCMA s'affiche
* - la suite des traitements se trouve donc dans le 
*   callback de validation de BDCMA (BDCMA_VAL)
*
******************************************************
* SEQUENCE D'APPEL :
*/

void CreMac_CB (
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
	static char *version = "$Id: scon_tac.c,v 1.8 1996/02/20 09:21:53 milleville Exp $ : CreMac_CB" ;

/* on stocke dans une variable statique le mode operatoire */ 
vg_operation = CG_CRE_MAC;

/* affichage de BDCMA */
EcritChampsCMA( "" );

XtManageChild ( BDCMA_FM ); 
}
 
/*X*/
/* Fichier : $Id: scon_tac.c,v 1.8 1996/02/20 09:21:53 milleville Exp $      Release : $Revision: 1.8 $        Date : $Date: 1996/02/20 09:21:53 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_tac
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* callback appele par le PushButton "Copie d'une 
* machine" du sous-menu "Caracteristiques de la Machine"

* - a la fin de l'execution de ce callback, la boite de 
*   dialogue BDCOMA s'affiche
* - la suite des traitements se trouve donc dans le 
*   callback de validation de BDCOMA (BDCOMA_VAL)
*
******************************************************
* SEQUENCE D'APPEL :
*/

void CoMac_CB (
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
	static char *version = "$Id: scon_tac.c,v 1.8 1996/02/20 09:21:53 milleville Exp $ : CoMac_CB" ;

/* on stocke dans une variable statique le mode operatoire */ 
vg_operation = CG_CO_MAC;

/* affichage de BDCOMA */
EcritChampsCOMA( "", "" );

XtManageChild ( BDCOMA_FM ); 
}

/*X*/
/* Fichier : $Id: scon_tac.c,v 1.8 1996/02/20 09:21:53 milleville Exp $      Release : $Revision: 1.8 $        Date : $Date: 1996/02/20 09:21:53 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_tac
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* callback appele par le PushButton "Creation" du 
* sous-menu "Machine"
*
* - a la fin de l'execution de ce callback, la boite de 
*   dialogue BDCMA s'affiche
* - la suite des traitements se trouve donc dans le 
*   callback de validation de BDCMA (BDCMA_VAL)
*
******************************************************
* SEQUENCE D'APPEL :
*/
void
DesMac_CB (Widget w, XtPointer client_data, XtPointer xt_call_data )

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
	static char *version = "$Id: scon_tac.c,v 1.8 1996/02/20 09:21:53 milleville Exp $ : DesMac_CB" ;

/* on stocke dans une variable statique le mode operatoire */
vg_operation = CG_DES_MAC;

/* affichage de BDCMA */
EcritChampsCMA( "" );

XtManageChild ( BDCMA_FM );
}



/*X*/
/* Fichier : $Id: scon_tac.c,v 1.8 1996/02/20 09:21:53 milleville Exp $      Release : $Revision: 1.8 $        Date : $Date: 1996/02/20 09:21:53 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_tac
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* callback de validation de la boite de dialogue BDCMA
* - lecture du nom de la machine
* - selon le mode operatoire ( CREATION ou DESTRUCTION )
*   appel de XZSX_01MachineLecture ou 
*   demande de confirmation avant appel a XZSS_06 pour
*   detruire le fichier XDC_NSUP_NomMachine
*
******************************************************
* SEQUENCE D'APPEL :
*/

void BDCMA_VAL (
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
	static char *version = "$Id: scon_tac.c,v 1.8 1996/02/20 09:21:53 milleville Exp $ : BDCMA_VAL" ;

  XZSXT_Machine vl_strucMac;
  int           vl_cr;
  char		vl_msg[80];

  /* Recuperation du nom de machine dans le champ de BDCMA */
  vl_cr = LitChampsCMA( vs_nomMac );

  if ( vl_cr == CG_VIDE )
  {
    AfficheErreur( "Entrez un nom de machine, SVP" );
    return;
  }

  XtUnmanageChild ( BDCMA_FM );

/* Cas de CREATION de machine    */
/* ------------------------------*/
if (vg_operation == CG_CRE_MAC)
{

  /* appel a XZSX_01MachineLecture qui retourne la structure vl_strucMac */
  vl_cr = XZSX_01MachineLecture( vs_nomMac, XDC_PATHTRAVFICADM, &vl_strucMac );
  /* si la machine existait, mise des champs de la structure  */
  /* vl_strucMac dans les champs texte de BDMAC               */

  if ( vl_cr == XDC_OK )
  {
	/*Creation de la boite de Dialogue */
	create_BDMAC ( CONFM, vg_mrm_hierarchy, &vg_class);
	
	/* Affichage des donnees */
      	EcritChampsMAC( vl_strucMac, CG_NONVIDE );

	/* affichage de BDMAC */
  	XtManageChild ( BDMAC_FM );
  }
  else
  {
    if ( vl_cr == XZSXC_FIC_INV )
    {
	/*Creation de la boite de Dialogue */
	create_BDMAC ( CONFM, vg_mrm_hierarchy, &vg_class);

      strcpy( vl_strucMac.NomMachine, vs_nomMac );
      EcritChampsMAC( vl_strucMac, CG_VIDE );
 
	/* affichage de BDMAC */
  	XtManageChild ( BDMAC_FM );
    }
    else
    {
      XZST_03EcritureTrace(XZSTC_WARNING,
		"Impossibilite ouverture fichier %s \n",
		vl_strucMac);
     	AfficheErreur( 
		"Impossible d'ouvrir le fichier associe a cette machine ");
     	return;
    }
  }

}

/* Cas de DESTRUCTION de machine */
/* ------------------------------*/
if (vg_operation == CG_DES_MAC)
 {
   /* demande de confirmation - affichage de BDCON1 */ 
   sprintf( vl_msg, 
	"Confirmez-vous la destruction de la machine %s ?", vs_nomMac );
   DemandeConf1( vl_msg );
 }


}
	 
/*X*/
/* Fichier : $Id: scon_tac.c,v 1.8 1996/02/20 09:21:53 milleville Exp $      Release : $Revision: 1.8 $        Date : $Date: 1996/02/20 09:21:53 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_tac
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* callback de validation de la boite de dialogue BDCOMA
* - lecture du nom des machines
*   Copie de fichiers 
*
******************************************************
* SEQUENCE D'APPEL :
*/

void BDCOMA_VAL (
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
	static char *version = "$Id: scon_tac.c,v 1.8 1996/02/20 09:21:53 milleville Exp $ : BDCOMA_VAL" ;

  XZSXT_Machine vl_strucMac	;
  int           vl_cr		= XDC_OK;
  char		vl_msg[80]	= "";
  char 		vl_Commande[80] = "";
  char 		vl_Fichier_Dest[80]	= "";
  char 		vl_Fichier_Exped[80]	= "";
  char		vl_Copie[3]		= "cp";


  /* Recuperation du nom des machines dans les  champs de BDCOMA */
  vl_cr = LitChampsCOMA( vs_nomMac_dest, vs_nomMac_exped );

  if ( vl_cr == CG_VIDE )
  {
    AfficheErreur( "Entrez les Noms des Machines, SVP" );
    return;
  }

  XtUnmanageChild ( BDCOMA_FM );


	/*A verification que le fichier a creer n'existe pas */
	vl_cr = XZSX_01MachineLecture(vs_nomMac_dest,
				XDC_PATHTRAVFICADM, &vl_strucMac );
	if (vl_cr != XDC_OK)
	{	
  		/* appel a XZSX_01MachineLecture qui retourne 
				la structure vl_strucMac */
  		vl_cr = XZSX_01MachineLecture( vs_nomMac_exped, 
				XDC_PATHTRAVFICADM, &vl_strucMac );
  
		/* si la machine existait, mise des champs 
		*				de la structure  */
  		/* vl_strucMac dans les champs texte de BDMAC      */

  		if ( vl_cr == XDC_OK )
  		{
			/*Copie  du Fichier nsup_NomMachineDest vers 
			*	le fichier nsup_NomMachineExped */
			sprintf(vl_Fichier_Dest,"%s/%s_%s",
				XDC_PATHTRAVFICADM,XDF_NSUP,vs_nomMac_dest);
			sprintf(vl_Fichier_Exped,"%s/%s_%s",
				XDC_PATHTRAVFICADM,XDF_NSUP,vs_nomMac_exped);
			sprintf(vl_Commande,"%s %s %s",vl_Copie,
				vl_Fichier_Exped,vl_Fichier_Dest); 
			system(vl_Commande);

			/*Creation de la boite de Dialogue */
			create_BDMAC ( CONFM, vg_mrm_hierarchy, &vg_class);

			/*Modification du Nom de la Machine */
			strcpy(vl_strucMac.NomMachine,vs_nomMac_dest);
			/* Ecriture des Champs */
    			EcritChampsMAC( vl_strucMac, CG_NONVIDE );

			/* affichage de BDMAC */
  			XtManageChild ( BDMAC_FM );
  		}
  		else
  		{
    			if ( vl_cr == XZSXC_FIC_INV )
    			{
				/*Creation de la boite de Dialogue */
				create_BDMAC ( CONFM, vg_mrm_hierarchy, &vg_class);
      				strcpy( vl_strucMac.NomMachine, 
							vs_nomMac_exped );
      				EcritChampsMAC( vl_strucMac, CG_VIDE );
 				 
				/* affichage de BDMAC */
  				XtManageChild ( BDMAC_FM );
    			}
    			else
    			{
      				XZST_03EcritureTrace(XZSTC_WARNING,
				"Impossibilite ouverture fichier %s \n",
					vl_strucMac);
     				AfficheErreur( 
	    "Impossible d'ouvrir le fichier associe a cette machine ");
     				return;
    			}
  		}
	}
	else
	{
		/*A On considere l'operation comme une Cration */
		vg_operation = CG_CRE_MAC;

		/*Creation de la boite de Dialogue */
		create_BDMAC ( CONFM, vg_mrm_hierarchy, &vg_class);

		/* Ecriture des Champs */
		EcritChampsMAC( vl_strucMac, CG_NONVIDE );

		/* affichage de BDMAC */
		XtManageChild ( BDMAC_FM );
	}
}



/*X*/
/* Fichier : $Id: scon_tac.c,v 1.8 1996/02/20 09:21:53 milleville Exp $      Release : $Revision: 1.8 $        Date : $Date: 1996/02/20 09:21:53 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_tac
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* callback de validation de la boite de dialogue BDCTM
* - lecture des champs de la structure machine
* - appel de XZSX_01MachineEcriture
*
******************************************************
* SEQUENCE D'APPEL :
*/

void 
BDMAC_VAL (Widget w, XtPointer client_data, XtPointer xt_call_data )

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
	static char *version = "$Id: scon_tac.c,v 1.8 1996/02/20 09:21:53 milleville Exp $ : BDMAC_VAL" ;

  XZSXT_Machine     vl_strucMac;
  int               vl_cr;

  /* recuperation des champs texte de BDMAC */
  /* dans une structure vl_strucMac         */
  vl_cr = LitChampsMAC( &vl_strucMac );

  /* interruption de la fonction si le champ "nom de */
  /* la machine " n'est pas renseigne                */
  if ( vl_cr == CG_VIDE )
  {
     AfficheErreur( "Entrez un nom de machine, SVP" );
     return;
  }
  else if ( vl_cr == CG_SITE)
  {
        AfficheErreur( "Attention Nom Site incorrecte");
   	return;
  }
  else
  {
  	/* appel de XZSX_03Machine_Ecriture */
  	vl_cr = XZSX_03Machine_Ecriture(XDC_PATHTRAVFICADM,vl_strucMac );
 
  	XtUnmanageChild ( BDMAC_FM );
  	XtDestroyWidget ( BDMAC_FM );
  }

}

/*X*/
/* Fichier : $Id: scon_tac.c,v 1.8 1996/02/20 09:21:53 milleville Exp $      Release : $Revision: 1.8 $        Date : $Date: 1996/02/20 09:21:53 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_tac
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* callback de validation de la boite de dialogue BDCTM
* - lecture des champs de la structure machine
* - appel de XZSX_01MachineEcriture
*
******************************************************
* SEQUENCE D'APPEL :
*/

void 
BDMAC_QUI (Widget w, XtPointer client_data, XtPointer xt_call_data )

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
	static char *version = "$Id: scon_tac.c,v 1.8 1996/02/20 09:21:53 milleville Exp $ : BDMAC_QUI" ;

  XZSXT_Machine     vl_strucMac;
  int               vl_cr;
  char 		vl_Fichier_Dest[80]	= "";


	/* Cas d'une Copie */
        if (vg_operation == CG_CO_MAC)
        {
		/* Destruction du Fichier */
                sprintf(vl_Fichier_Dest,"%s/%s_%s" ,
		     XDC_PATHTRAVFICADM,XDF_NSUP,vs_nomMac_dest);
	       	vl_cr = XZSS_06DetruireFichier( vl_Fichier_Dest );
	}

  	XtUnmanageChild ( BDMAC_FM );
	XtDestroyWidget ( BDMAC_FM );
}

/*X*/
/* Fichier : $Id: scon_tac.c,v 1.8 1996/02/20 09:21:53 milleville Exp $      Release : $Revision: 1.8 $        Date : $Date: 1996/02/20 09:21:53 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_tac
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* procedure de sortie de la tache tsconf a partir du
* menu "Quitter"
*
******************************************************
* SEQUENCE D'APPEL :
*/

void 
CONFM_QUI (Widget w, XtPointer client_data, XtPointer xt_call_data )

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
	static char *version = "$Id: scon_tac.c,v 1.8 1996/02/20 09:21:53 milleville Exp $ : CONFM_QUI" ;

  XZST_03EcritureTrace( XZSTC_INFO, 
	       "Sortie de scon par sous-menu Quitter ");
  exit(-1);
}


/*X*/
/* Fichier : $Id: scon_tac.c,v 1.8 1996/02/20 09:21:53 milleville Exp $      Release : $Revision: 1.8 $        Date : $Date: 1996/02/20 09:21:53 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_tac
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* callback de validation de BDCON1, qui est appele en
* cas de destruction de machine ou tache
*
* - recuperation du mode operatoire
******************************************************
* SEQUENCE D'APPEL :
*/

BDCON1_VAL( Widget w, XtPointer client_data, XtPointer xt_call_data )

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
	static char *version = "$Id: scon_tac.c,v 1.8 1996/02/20 09:21:53 milleville Exp $ : BDCON1_VAL" ;

  int vl_cr;
  char pl_PathName[256];

  switch ( vg_operation )
  {
    case CG_DES_TAC:
	/* appel a XZSX_05Tache_Detruire */
	vl_cr = XZSX_05Tache_Detruire( vs_nomMac, 
			   		vs_nomTac, 
		   			XDC_PATHTRAVFICADM );
        break;

    case CG_DES_MAC:

	/* construction du nom de fichier XDC_NSUP_NomMachine */
  	sprintf(pl_PathName,"%s/%s_%s", XDC_PATHTRAVFICADM,
				    XDF_NSUP, vs_nomMac );
	/* destruction de ce fichier */
        vl_cr = XZSS_06DetruireFichier( pl_PathName );
        break;

    default:
	/* erreur */
	break;
  } 

  XtUnmanageChild( BDCON1_FM );
}


