/*E*/
/*  Fichier : $Id: xzae95sp.h,v 1.1 2019/05/24 10:02:27 devgfi Exp $        $Revision: 1.1 $        $Date: 2019/05/24 10:02:27 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae95sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* P.V.  19/07/96   : Creation
* JPL	10/02/09 : Degroupage (renommee sans ";") (lie a DEM845) 1.2
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Retourne les champs d'une fiche main courante vehicule
* implique dans un accident
------------------------------------------------------- 
* Sequence d'appel
* SP	XZAE95_Lire_Liste_Vehicule
* 
* Arguments en entree
* XDY_Horodate		va_Horodate_in : par defaut heure courante(non utilisee)
* XDY_Entier		va_NumEvt_in
* XDY_Octet		va_CleEvt_in
* 
* Arguments en sortie
* XDY_Octet		va_Type_out
* XDY_Octet		va_Passagers_out
* XDY_Booleen		va_Bebe_out
* XDY_Booleen		va_PersAgee_out
* XDY_Booleen		va_Animaux_out
* XDY_Booleen		va_VehEscota_out
* XDY_Octet		va_Chargement_out
* char(6)		va_CodeMat_out
* XDY_Entier		va_Depannage_out
* XDY_Site		va_SiteDepannage_out
* char(100)		va_Texte_out
* char(25)		va_marque_out
* char(25)		va_type_out
* char(25)		va_couleur_out
* char(25)		va_immatriculation_out
* XDY_Mot		va_code_pays_out
* char(25)		va_pays_out
* char(25)		va_nom_out
* char(25)		va_ville_out
* char(75)		va_adresse_out
* char(4)		va_code_danger_out
* XDY_Mot		va_code_nature_panne_out
* char(25)		va_nature_panne_out
* char(25)		va_positionnement_out
* XDY_Octet		va_vehicule_roulant_out
* char(25)		va_enlev_dem_par_out
* XDY_Entier		va_numero_fiche_appel_out
* XDY_Octet		va_pres_mat_dangereuse_out
* XDY_Octet		va_fax_envoye_out
* XDY_Mot		va_code_marque_out
* XDY_Mot		va_code_couleur_out
* XDY_Mot		va_code_enlev_demande_par_out
* XDY_Mot		va_num_veh_out
* XDY_Octet		va_sit_out
* XDY_Octet 		va_site_creation_out
* XDY_Mot		va_nb_bebes_out
* XDY_Mot		va_nb_agees_out
* XDY_Octet		va_feu_maitrise
* 
* Code retour
* XDC_OK
* XDC_NOK : pas de complement de fiche main courante
* XDC_ARG_INV
* <0 erreur sybase 
* 
* Conditions d'utilisation
* En formation, preciser l'horodate
* 
* La procedure stockee au CI recherche les informations 
* dans la base HIS si la date donnee est anterieure de plus d'un 
* mois tandis qu'au niveau des districts la recherche s'effectue 
* dans la base EXP.
* Le code de creation de la procedure au CI et aux districts 
* doit etre identique a l'exception de la base de recherche des infos
* et du test de la date en entree (< 1 mois)
* (toute modif dans la creation de la procedure doit etre 
*  effectuee pour le CI et les districts)
*
* Suite a un ajout de colonnes dans ACT_DEP, les champs vehicule_roulant,
* presence_matiere_dangereuse et fax_envoye n'ont pu etre codes en booleens.
* Des controles sont donc necessaires dans le code appelant cette procedure pour que les
* valeurs de ces champs soient 0 ou 1 uniquement.
* 
* Fonction
* Select dans la table VEHICULE
* where numero evenement and horodate
-------------------------------------------------------*/

 
#define XZAE95_Lire_Liste_Vehicule	"XZAE95" 
