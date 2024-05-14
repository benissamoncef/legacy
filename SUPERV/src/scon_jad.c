/*E*/
/* Fichier : $Id: scon_jad.c,v 1.7 1996/02/20 09:21:52 milleville Exp $	      Release : $Revision: 1.7 $         Date : $Date: 1996/02/20 09:21:52 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_jad * FICHIER scon_jad.c
******************************************************
* DESCRIPTION DU MODULE :
*
* ce module contient les fonctions appelees a partir
* du sous-menu "Journal d'Administration"
*
* 1) Fonction de creation de journal:
*
*    CreJou_CB puis 
*    BDCJA_VAL;
* 
*
******************************************************
* HISTORIQUE :
*
* T.Milleville  30 Aou 1994	: Creation
* T.Milleville  25 Jul 1995	: Mise a jour entete V1.6
* P.Niepceron  19 Fev  1996        : Remplacement de XDC_PATHREF_FICADM
*       par XDC_PATHTRAVFICADM  V1.7
*******************************************************/

/* fichiers inclus */

#include "scon_ser.h"
#include "scon_var.h"

#include "scon_jad.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: scon_jad.c,v 1.7 1996/02/20 09:21:52 milleville Exp $ : scon_jad" ;


/* declaration de fonctions internes */

/* definition de fonctions externes */



/*X*/
/* Fichier : $Id: scon_jad.c,v 1.7 1996/02/20 09:21:52 milleville Exp $	      Release : $Revision: 1.7 $         Date : $Date: 1996/02/20 09:21:52 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_jad
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* callback appele par le sous-menu "Creation du
* Journal" de "Journal d'Administration"
*
******************************************************
* SEQUENCE D'APPEL :
*/

void
CreJou_CB (Widget w, XtPointer client_data, XtPointer xt_call_data )

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
	static char *version = "$Id: scon_jad.c,v 1.7 1996/02/20 09:21:52 milleville Exp $ : CreJou_CB" ;

  long		vl_taille	= 0;
  char 		vl_TailleTexte[20]	= "";



	/*A Recuperation de la Taille du Journal d'administartion */
	if (XZSX_29JournalLecture(&vl_taille,XDC_PATHTRAVFICADM) 
					== XDC_OK)
	{
		sprintf(vl_TailleTexte,"%d",vl_taille);	
  		EcritChampsCJA( vl_TailleTexte );
	}
	else
	{
		EcritChampsCJA( "" );
	}

  /* ouverture de BDCJA permettant de preciser la taille */
  /* du journal d'administration                         */
  XtManageChild( BDCJA_FM );
}


/*X*/
/* Fichier : $Id: scon_jad.c,v 1.7 1996/02/20 09:21:52 milleville Exp $	      Release : $Revision: 1.7 $         Date : $Date: 1996/02/20 09:21:52 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_jad
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* callback de validation de BDCJA, creant le journal
* d'administration
*
******************************************************
* SEQUENCE D'APPEL :
*/

void
BDCJA_VAL (Widget w, XtPointer client_data, XtPointer xt_call_data )

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
	static char *version = "$Id: scon_jad.c,v 1.7 1996/02/20 09:21:52 milleville Exp $ : BDCJA_VAL" ;
  long                 vl_taille;
  int                 vl_ValRet;

  XZST_03EcritureTrace( XZSTC_INFO, "IN: BDCJA_VAL");

  /* lecture du champ texte de BDCJA                 */
  vl_ValRet = LitChampsCJA ( &vl_taille);

  if ( vl_ValRet == CG_VIDE )
  {
     AfficheErreur( "Entrez une taille maximale de fichier, SVP" );
     return;
  }

  /* appel a XZSX_30Jouradm_Ecrire qui ecrit cette taille */
  /* dans le fichier d'administration                     */
  vl_ValRet = XZSX_30JournalEcriture( vl_taille, XDC_PATHTRAVFICADM );

  XtUnmanageChild ( BDCJA_FM );
}

