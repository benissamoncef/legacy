/*E*/
/*  Fichier : $Id: xzac14sp.h,v 1.1 1994/11/03 14:52:32 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 14:52:32 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac13sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	07/09/94	: Creation
* C.T.	13/09/94	: Modif insertion enregistrement 
*                         et parametres entree
* C.T.	20/09/94	: Modif taille message (non reteste)
* C.T.	28/09/94	: Modif message en " " si null
* B.G.  16/10/94        : Suppression des appels xzah
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
Sequence d'appel
* SP	XZAC14_Ecrire_PMV_Mobile
* 
* Arguments en entree
* XDY_Entier		va_NumEvt_in
* XDY_Octet		va_CleEvt_in
* XDY_Entier		va_NumAction_in
* XDY_Octet		va_Site_in
* XY_Horodate		va_HoroDebut_in
* XY_Horodate		va_HoroFin_in
* XDY_PR		va_PR_in
* XDY_Sens		va_Sens_in
* char(48)		va_Message_in
* XDY_Operateur		va_Operateur_in
* 
* Arguments en sortie
* XDY_Entier		va_NumAction_out
* 
* Service rendu
* Ajouter ou modifier une actio sur un PMV mobile
* 
* Code retour
* XDC_OK
* XDC_NOK	: evenement ou actio non trouvee, ou insertion impossible
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* En insertion, fournir NumeroEvt seul, en modification, fournir egalement NumeroAction
* 
* Fonction
* Verifier l'existence de l'evenement
* Inserer ou modifier dans les tables ACTION et PMV_REMORQUE
* Appeler XZAH12_Animer_Objet_Mobile
-------------------------------------------------------*/

 
#define XZAC14_Ecrire_PMV_Mobile	"XZAC;14" 
