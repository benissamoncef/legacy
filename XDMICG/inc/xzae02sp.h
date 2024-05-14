/*E*/
/*  Fichier : $Id: xzae02sp.h,v 1.3 2007/04/18 17:24:10 gesconf Exp $      Release : $Revision: 1.3 $        Date : $Date: 2007/04/18 17:24:10 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae02sp.prc
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* retourne la synthese d'un evt ou operation
*
------------------------------------------------------
* HISTORIQUE :
*
* B.GABORIT	28/09/94	: Creation d'un bouchon
* JPL		18/04/07	: Renommage 1.3
-----------------------------------------------------*/
/*X*/
/*------------------------------------------------------------------------
Sequence d'appel
SP	XZAE02_Synthese_FMC
Arguments en entree
XDY_Entier	va_NumEvt_in
XDY_Octet	va_CleEvt_in
Arguments en sortie
XDY_Octet	va_Etat_out : a traiter, prevision ou clos (XDC_FMC_ETAT_xxx)
char(80)	va_ligne_out : colonne 1 du select
XDY_Horodate	va_heure_out : colonne 2 du select (null si la ligne n'est pas selectionnable)

Service rendu
Retourne la synthese d'une fiche main courante sous la forme d'une liste de lignes
Code retour
XDC_OK
XDC_NOK
Conditions d'utilisation

Fonction
Select dans les tables EVENEMENT, EVENEMENT_HISTORIQUE, ALERTE, ACTION, etc
a preciser fonction de l'IHM
-----------------------------------------------------------------------*/

 
#define XZAE02_Synthese_FMC	"XZAE02"
