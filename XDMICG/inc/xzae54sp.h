/*E*/
/*  Fichier : $Id: xzae54sp.h,v 1.2 2009/03/09 14:34:11 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2009/03/09 14:34:11 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae54sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	04/10/94	: Creation	(V 1.1)
* JPL	25/02/09	: Degroupage pour ajout champ 'feu maitrise' (DEM 845) 1.2
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Modifier une fiche de vehicule relative a un evenement existant
* 
* Sequence d'appel
* SP	XZAE54_Ecrire_Fiche_Vehicule
* 
* Arguments en entree
* XDY_Entier	va_NumEvenement_in
* XDY_Octet	va_CleEvenement_in
* XDY_Octet	va_Type_in
* XDY_Octet	va_Passagers_in
* XDY_Booleen	va_Animaux_in
* XDY_Booleen	va_VehEscota_in
* XDY_Octet	va_Chargement_in
* char(6)	va_CodeMat_in
* char(100)	va_Texte_in
* XDY_Mot	va_NumeroVehicule_in
* XDY_Octet	va_site_creation_in
* XDY_Mot	va_nb_bebes_in
* XDY_Mot	va_nb_agees_in
* XDY_Octet	va_feu_maitrise_in
* 
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: evenement non trouve, update impossible,
*                 poste enrichisseur different
* XDC_ARG_INV	: parametres d'entree null
* 
* Conditions d'utilisation
* va_NumeroVehicule_in doit etre positionne a 0 pour une FMC
* de type vehicule
* pour un accident, positionner a null pour inserer un 
* vehicule dans la liste des vehicules impliques dans 
* l'accident ou preciser le numero du vehicule dont on
* desire modifier les valeurs
* 
* Fonction
* Modifier dans la table VEHICULE
---------------------------------------------------------*/

 
#define XZAE54_Ecrire_Fiche_Vehicule	"XZAE54"
