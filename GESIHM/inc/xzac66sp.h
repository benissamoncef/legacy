/*E*/
/*  Fichier : $Id: xzac66sp.h,v 1.1 1998/01/20 14:27:24 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1998/01/20 14:27:24 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac66.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	16/09/94	: Creation
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Action d'affichage PAL+picto.
* 
* Sequence d'appel
* SP	XZAC66_Commande_PAL
* 
* Arguments en entree
* XDY_Horodate	va_Horodate_in
* XDY_Entier	va_NumEvt_in
* XDY_Octet	va_CleEvt_in
* XDY_Operateur	va_Operateur_in
* XDY_Eqt	va_NumEqt_in
* XDY_Site	va_Site_in
* XDY_LignePAL	va_LibelleMod1_in
* XDY_LignePAL	va_LibelleMod2_in
* XDY_LignePAL	va_LibelleMod3_in
* XDY_LignePAL	va_LibelleMod4_in
* XDY_LignePAL	va_LibelleMod5_in
* XDY_Octet	va_Priorite_in
* XDY_Entier	va_DistanceEvt_in
* XDY_Mot	va_Destination_in
* 
* Arguments en sortie
* XDY_Entier	va_NumeroAction_out
* 
* Code retour
* XDC_OK
* XDC_NOK	: evenement non trouve
* XDC_ARG_INV	: parametres d'entree null
* XDC_REV_PB_DCL: probleme pour declencher le reveil
* XDC_PRC_INC	: ARRP03 inexistante
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Si l'action a ete proposee par le plan d'actions, positionner 
* Priorite et DistanceEvt, sinon, mettre DistanceEvt a XDC_DISTANCE_INCONNUE
* (Priorite sera mis automatiquement a null)
* 
* Fonction
* Verifier l'existence de l'evenement
* Verifier l'existence d'un equipement non indisponible 
* dans le district ou l'actio est executee.
* 
* Inserer dans la table ACTION et la table E_PAL.
* 
* Chercher l'action precedente sur l'equipement a commander :
* 	SELECT numero FROM ACTION 
* 	WHERE equipement, type, heure_lancement_fin = null, 
* 		heure_lancement = all
* 		(SELECT max(heure_lancement) FROM ACTION WHERE equipement, type)
* 	
* 	Positionner heure_lancement_fin a Horodate pour l'action precedente
* 	Si l'action precedente trouvee est liee a un autre 
* 	evenement alors l'autre evenement doit 	etre reveille 
* 	pour l'avertir du vol (appeler XZAR03).
* 
* 
-----------------------------------------------------*/

 
#define XZAC66_Commande_PAL	"XZAC;66" 
