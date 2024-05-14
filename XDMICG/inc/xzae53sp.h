/*E*/
/*  Fichier : $Id: xzae53sp.h,v 1.2 2011/11/14 11:41:14 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2011/11/14 11:41:14 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae53sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	04/10/94	: Creation	(V 1.1)
* JPL	14/11/11	: Ajout champs de condition d'accident pour EVEREST (DEM 1006)  1.2
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAE53_Ecrire_Fiche_Accident
* 
* Arguments en entree
* XDY_Entier	va_NumEvenement_in
* XDY_Octet	va_CleEvenement_in
* XDY_Octet	va_VL_in
* XDY_Octet	va_PL_in
* XDY_Octet	va_Remorque_in
* XDY_Octet	va_Car_in
* XDY_Octet	va_Moto_in
* XDY_Octet	va_BlesseLeg_in
* XDY_Octet	va_BlesseGra_in
* XDY_Octet	va_Mort_in
* XDY_Booleen	va_VehEscota_in
* XDY_Booleen	va_MatDang_in
* XDY_Booleen	va_HomEscota_in
* XDY_Booleen	va_Personalite_in
* XDY_Booleen	va_DegatsDom_in			Pour compatibilite
* XDY_Eqt	va_PosteOperateur_in
* XDY_Horodate	va_Validation_in
* XDY_Mot	va_Cond_Meteo_in
* XDY_Mot	va_Cond_Visibilite_in
* XDY_Mot	va_Cond_Etat_Chau_in
* XDY_Mot	va_Cond_Chantier_in
* ****** XDY_Mot	va_Cond_Obstacle_in	ANNULE
* ****** XDY_Mot	va_Cond_Animal_in	ANNULE
* 
* Arguments en sortie
* 
* Service rendu
* Modifier une fiche d'accident relative a un evenement existant
* 
* Code retour
* XDC_OK
* XDC_NOK	: evenement non trouve, update impossible,
*                 poste enrichisseur different
* XDC_ARG_INV	: parametres d'entree null
* 
* Conditions d'utilisation
* 
* Fonction
* Modifier dans la table ACCIDENT
---------------------------------------------------*/

 
#define XZAE53_Ecrire_Fiche_Accident	"XZAE53"
