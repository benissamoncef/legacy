/*E*/
/*  Fichier : @(#)xzae96sp.prc	1.7      Release : 1.7        Date : 02/20/95
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae96sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* P.V.  01/08/96	: Creation (DEM76)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Modifier une fiche de vehicule relative a un evenement existant
* 
* Sequence d'appel
* SP	XZAE96_Ecrire_Fiche_Vehicule_Site
* 
* Arguments en entree
* XDY_Entier		va_NumEvenement_in
* XDY_Octet		va_CleEvenement_in
* XDY_Octet		va_Type_in
* XDY_Octet		va_Passagers_in
* XDY_Booleen		va_Bebe_in
* XDY_Booleen		va_PersAgee_in
* XDY_Booleen		va_Animaux_in
* XDY_Booleen		va_VehEscota_in
* XDY_Octet		va_Chargement_in
* char(6)		va_CodeMat_in
* XDY_Entier		va_Depannage_in
* XDY_Site		va_SiteDepannage_in
* CHAR(100)		va_Texte_in
* XDY_Mot		va_NumeroVehicule_in
* XDY_Eqt		va_PosteOperateur_in
* char(25)              va_marque_in
* char(25)              va_type_in
* char(25)              va_couleur_in
* char(25)              va_immatriculation_in
* XDY_Mot               va_code_pays_in
* char(25)              va_pays_in
* char(25)              va_nom_in
* char(25)              va_ville_in
* char(75)              va_adresse_in
* char(4)               va_code_danger_in
* XDY_Mot               va_code_nature_panne_in
* char(25)              va_nature_panne_in
* char(25)              va_positionnement_in
* XDY_Octet             va_vehicule_roulant_in
* char(25)              va_enlev_dem_par_in
* XDY_Entier            va_numero_fiche_appel_in
* XDY_Octet             va_pres_mat_dangereuse_in
* XDY_Octet             va_fax_envoye_in
* XDY_Mot               va_code_marque_in
* XDY_Mot               va_code_couleur_in
* XDY_Mot               va_code_enlev_demande_par_in
* XDY_Entier            va_sit_in
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
* Suite a un ajout de colonnes dans ACT_DEP, les champs vehicule_roulant,
* presence_matiere_dangereuse et fax_envoye n'ont pu etre codes en booleens.
* Des controles sont donc necessaires dans le code appelant cette procedure pour que les
* valeurs de ces champs soient 0 ou 1 uniquement.
*
* Fonction
* Modifier dans la table VEHICULE
---------------------------------------------------------*/

 
#define XZAE96_Ecrire_Fiche_Vehicule_Site	"XZAE;96" 
