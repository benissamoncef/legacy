/*E*/
/*  Fichier : $Id: xzac12sp.h,v 1.1 1994/11/03 14:52:31 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 14:52:31 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac12sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	06/09/94	: Creation
* C.T.	13/09/94	: Modif insertion de l'enregistrement
*                         et appel aux XZAH
* C.T.	28/09/94	: Modif NomPatrouille en " " si null
* B.G.  17/10/94        : Suppression des appels xzah
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Inserer ou modifier une intervention de la patrouille Escota
* 
* Sequence d'appel
* SP	XZAC12_Ecrire_Intervention_SIS
* 
* Arguments en entree
* XDY_Entier		va_NumEvt_in
* XDY_Octet		va_CleEvt_in
* XDY_Entier		va_NumeroAction_in
* XDY_Octet		va_Site_in
* XDY_Nom		va_NomPatrouille_in
* XDY_Booleen		va_PoseBalisage_in
* XY_Horodate		va_DebutBalisage_in
* XY_Horodate		va_FinBalisage_in
* XY_Horodate		va_HoroLancement_in
* XY_Horodate		va_HoroFin_in
* XDY_Operateur		va_Operateur_in
* XY_Horodate		va_HoroSucces_in
* 
* Arguments en sortie
* XDY_Entier		va_NumeroAction_out
* 
* Code retour
* XDC_OK
* XDC_NOK	: evenement ou actio non trouve ou site incorrect
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* En insertion, fournir NumeroEvt seul, en modification, fournir 
* egalement NumeroAction
* 
* Fonction
* Verifier l'existence de l'evenement
* Inserer ou modifier dans les tables ACTION, SISERVENTION_PATROUILLE
* Appeler XZAH12_Animer_Objet_Mobile
* Si un balisage est pose, rechercher dans EVENEMENT_HISTORIQUE quelles 
* sont les voies neutralisees et appeler XZAH13_Animer_Neut_Voies.
---------------------------------------------------*/

 
#define XZAC12_Ecrire_Intervention_SIS	"XZAC;12" 
