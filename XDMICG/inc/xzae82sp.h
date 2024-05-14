/*E*/
/*  Fichier : $Id: xzae82sp.h,v 1.2 2007/04/18 14:56:05 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2007/04/18 14:56:05 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae82sp.prc
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* retourne la synth…se d'un evt ou operation
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	14/10/94 : Creation	V 1.1 
* JPL	18/04/07 : Rrenommage 1.2
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Ajout des actions dans la synthese d'un evenement
* a une date donnee
* 
* Sequence d'appel
* SP	XZAE82_Synthese_Actions_FMC
* 
* Arguments en entree
* XDY_Entier	va_NumEvt_in
* char(2)	va_CleEvt_in
* char(10)	va_Type_in
* XDY_Autoroute	va_Autoroute_in
* XDC_PR	va_PR_in
* XDC_Sens	va_Sens_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK	
* XDC_NOK	action non trouvee ou informations
*               complementaires
* XDC_ARG_INV
* < 0          : erreur sybase
* 
* Conditions d'utilisation
* Cette procedure est utilis≈e par la procedure XZAE02
* Elle est interne a la base de donn≈e.
* 
* Fonction
* 
-------------------------------------------------------*/

 
#define XZAE82_Synthese_Actions_FMC	"XZAE82"
