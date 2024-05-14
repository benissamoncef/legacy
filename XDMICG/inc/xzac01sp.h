/*E*/
/*  Fichier : $Id: xzac01sp.h,v 1.1 1994/11/03 14:52:15 torregrossa Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/03 14:52:15 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac01.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	16/09/94	: Creation
* C.T.	23/09/94	: Modification du traitement de
*                         recherche du nom de la machine
*                         pilotant
* C.T.	28/09/94	: Modification interface ajout flash
*                         et mise a " " des chaines nulles
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Action d'affichage PMV+picto.
* 
* Sequence d'appel
* SP	XZAC01_Commande_PMV
* 
* Arguments en entree
* XDY_Horodate	va_Horodate_in
* XDY_Entier	va_NumEvt_in
* XDY_Octet	va_CleEvt_in
* XDY_Operateur	va_Operateur_in
* XDY_Eqt	va_NumEqt_in
* XDY_LignePMV	va_Ligne1_in
* XDY_LignePMV	va_Ligne2_in
* XDY_LignePMV	va_Ligne3_in
* XDY_LignePMV	va_Alternat1_in
* XDY_LignePMV	va_Alternat2_in
* XDY_LignePMV	va_Alternat3_in
* XDY_Octet	va_Clignotement1_in
* XDY_Octet	va_Clignotement2_in
* XDY_Octet	va_Clignotement3_in
* XDY_Octet	va_Flash_in
* char[XDC_LGR_NOM_PICTO]	va_TypePicto_in
* char[XDC_LGR_TXT_PICTO]	va_TextePicto_in
* XDY_Octet	va_ClignotementPicto_in
* XDY_Octet	va_Priorite_in
* XDY_Entier	va_DistanceEvt_in
* 
* Arguments en sortie
* XDY_Entier	va_NumeroAction_out
* 
* Code retour
* XDC_OK
* XDC_NOK	: evenement non trouve
* XDC_ARG_INV	: parametres d'entree null
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Si l'actio a ete proposee par le plan d'actions, positionner 
* Priorite et DistanceEvt, sinon, les mettre a 0
* 
* Fonction
* Verifier l'existence de l'evenement
* Verifier l'existence d'un equipement non indisponible 
* dans le district ou l'actio est executee.
* 
* Inserer dans la table ACTION et la table E_PMV.
* 
* Chercher l'actio precedente sur l'equipement a commander :
* 	SELECT numero FROM ACTION 
* 	WHERE equipement, type, heure_lancement_fin = null, 
* 		heure_lancement = all
* 		(SELECT max(heure_lancement) FROM ACTION WHERE equipement, type)
* 	
* 	Positionner heure_lancement_fin a Horodate pour l'actio precedente
* 	Si l'actio precedente trouvee est liee a un autre 
* 	evenement alors l'autre evenement doit 	etre reveille 
* 	pour l'avertir du vol (appeler XZAR03).
* 
* ACRP01 :	Appeler XZEP01 pour afficher sur l'equipement PMV
* 		Appeler XZEP02 pour afficher le picto
* 
-----------------------------------------------------*/

 
#define XZAC01_Commande_PMV	"XZAC;01" 
