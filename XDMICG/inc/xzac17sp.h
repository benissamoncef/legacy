/*E*/
/*  Fichier : $Id: xzac17sp.h,v 1.1 1994/11/03 14:52:36 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 14:52:36 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac17sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	07/09/94	: Creation
* C.T.	13/09/94	: Modif du parametre en entree
*                         HoroLancFin par HoroFin
* C.T.	28/09/94	: Modif NomGare en " " si null
* B.G.  17/10/94        : Suppression des appels xzah
-----------------------------------------------------*/

/*-----------------------------------------------------
* Service rendu
* Inserer ou modifier l'actio d'afficher dans une gare de peage
* 
* Sequence d'appel
* SP	XZAC17_Ecrire_Affichage_Gare
* 
* Arguments en entree
* XDY_Entier		va_NumEvt_in
* XDY_Octet		va_CleEvt_in
* XDY_Entier		va_NumeroAction_in
* XDY_Octet		va_Site_in
* XDY_Nom		va_NomGare_in
* XDY_Horodate		va_HoroAffDebut_in
* XDY_Horodate		va_HoroAffFin_in
* char(100)		va_Affichette_in
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
* XDC_NOK	: actio ou evnement non trouvee ou insertion impossible ou site incorrect
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Si le numero d'actio et le site sont non values en entree 
* alors l'actio est ajoutee sinon elle est modifiee
* 
* Fonction
* Verifier l'existence de l'evenement et de la gare de peage
* Inserer dans les tables ACTION, AFFICHAGE_EN_GARE
* Appeler XZAH12_Animer_Objet_Mobile
----------------------------------------------------*/

 
#define XZAC17_Ecrire_Affichage_Gare	"XZAC;17" 
