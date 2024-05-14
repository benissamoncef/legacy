/*E*/
/*  Fichier : $Id: xzac16sp.h,v 1.1 1994/11/03 14:52:34 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 14:52:34 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac16sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	07/09/94	: Creation
* C.T.	13/09/94	: Modif nom param entree 
*                         (HoroFin au lieu de HoroLancFin)
* C.T.	28/09/94	: Modif NomIntervention en " " si null
* B.G.  17/10/94        : Suppression des appels xzah
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAC16_Ecrire_Autre_Intervention
* 
* Arguments en entrEe
* XDY_Entier		va_NumEvt_in
* XDY_Octet		va_CleEvt_in
* XDY_Entier		va_NumeroAction_in
* XDY_Octet		va_Site_in
* XDY_Nom		va_NomIntervention_in
* XDY_Octet		va_TypeIntervention_in
* XY_Horodate		va_HoroLancement_in
* XY_Horodate		va_HoroFin_in
* XDY_Operateur		va_Operateur_in
* XY_Horodate		va_HoroSucces_in
* 
* Arguments en sortie
* XDY_Entier		va_NumeroAction_out
* 
* Service rendu
* Inserer ou modifier une intervention de type pompiers, gendarmes, ...
* 
* Code retour
* XDC_OK
* XDC_NOK	: actio ou evnement non trouvee ou insertion impossible ou site incorrect
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Si le numero d'actio et le site sont non values en entree 
* alors l'intervention d'un depanneur est ajoutee sinon elle est modifiee
* 
* Fonction
* Verifier l'existence de l'evenement
* Inserer dans les tables ACTION, INTERVENTION
* Appeler XZAH12_Animer_Objet_Mobile
------------------------------------------------------*/

 
#define XZAC16_Ecrire_Autre_Intervention	"XZAC;16" 
