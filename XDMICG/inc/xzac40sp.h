/*E*/
/*  Fichier : $Id: xzac40sp.h,v 1.1 1994/11/03 14:52:59 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 14:52:59 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac40sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	09/09/94	: Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Lecture de l'actio d'affichage PMV+picto.
* 
* Sequence d'appel
* SP	XZAC40_Lire_Commande_PMV
* 
* Arguments en entree
* XDY_Entier	va_NumAction_in
* XDY_Octet	va_Site_in
* 
* Arguments en sortie
* XDY_Eqt	va_NumEqt_out
* XDY_LignePMV	va_Ligne1_out
* XDY_LignePMV	va_Ligne2_out
* XDY_LignePMV	va_Ligne3_out
* XDY_LignePMV	va_Alternat1_out
* XDY_LignePMV	va_Alternat2_out
* XDY_LignePMV	va_Alternat3_out
* XDY_Booleen	va_Clignotement1_out
* XDY_Booleen	va_Clignotement2_out
* XDY_Booleen	va_Clignotement3_out
* char[XDC_LGR_NOM_PICTO]	va_TypePicto_out
* char[XDC_LGR_TXT_PICTO]	va_TextePicto_out
* XDY_Booleen	va_ClignotementPicto_out
* XDY_Horodate	va_HeureLancement_out
* XDY_Horodate	va_HeureLancementFin_out
* XDY_Horodate	va_HeureSucces_out
* XDY_Horodate	va_HeureEchec_out
* XDY_Horodate	va_HeureFin_out
* 
* Code retour
* XDC_OK
* XDC_NOK	: actio non trouvee
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* 
* Fonction
* Select dans les tables ACTION et E_PMV
-----------------------------------------------------------------------*/

 
#define XZAC40_Lire_Commande_PMV	"XZAC;40" 
