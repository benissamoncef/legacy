/*E*/
/*  Fichier : $Id: xzac15sp.h,v 1.1 1994/11/03 14:52:33 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 14:52:33 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac15sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	07/09/94	: Creation
* C.T.	13/09/94	: Modif param entree et insertion ds table
* C.T.	28/09/94	: Modif NomDEpanneur et resultat en " " si null
* B.G.  17/10/94        : Suppression des appels xzah
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Ajouter ou modifier l'intervention d'un depanneur
* 
* Sequence d'appel
* SP	XZAC15_Ecrire_Intervention_Depanneur
* 
* Arguments en entree
* XDY_Entier		va_NumEvt_in
* XDY_Octet		va_CleEvt_in
* XDY_Entier		va_NumAction_in
* XDY_Octet		va_Site_in
* XDY_Nom		va_NomDepanneur_in
* XDY_Nom		va_Resultat_in
* XY_Horodate		va_HoroLancement_in
* XY_Horodate		va_HoroFin_in
* XDY_Operateur		va_Operateur_in
* XY_Horodate		va_HoroSucces_in
* 
* Arguments en sortie
* XDY_Entier		va_NumAction_out
* 
* Code retour
* XDC_OK
* XDC_NOK	: actio ou evenement non trouvee ou insertion impossible ou site incorrect
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Si le numero d'actio et le site sont non values en entree 
* alors l'intervention d'un depanneur est ajoutee sinon elle est modifiee
* 
* Fonction
* Verifier l'existence de l'evenement
* Inserer dans les tables ACTION, INTERVENTION_DEPANNEUR
* Appeler XZAH12_Animer_Objet_Mobile
---------------------------------------------------------*/

 
#define XZAC15_Ecrire_Intervention_Depanneur	"XZAC;15" 
