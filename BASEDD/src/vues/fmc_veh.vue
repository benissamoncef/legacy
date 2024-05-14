/*X Fichier fmc_veh.vue 1.0		Date : 12/08/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER fmc_veh.vue
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* creation de la vue FMC_VEH
*
------------------------------------------------------
* HISTORIQUE :
*
* V-L   12/08/96        : creation
-----------------------------------------------------*/


/* ============================================================ */
/*   Nom de la base   :  MIGRAZUR                               */
/*   Nom de SGBD      :  SYBASE10                               */
/*   Date de creation :  12/08/96                               */
/* ============================================================ */

/* ============================================================ */
/*  VUE : FMC_VEH                                           */
/* ============================================================ */

declare @nom_base varchar(10)

/* nom de la base courante */
select @nom_base = db_name()


/*
*  Fonctionalite de la vue
*  -------------------------
*  Contient les complements de fiche main courante de type vehicule
*
*  Description des colonnes de la vue
*  ------------------------------------
*        FMC_VEH__numero		: numero de la fiche main courante
*        FMC_VEH__district		: numero du district de la fiche main courante
*	 FMC_VEH__numero_vehicule	: numero de vehicule implique dans l'accident
*	 FMC_VEH_type			: nom du type de fmc lu dans FMC_TYP.nom
*	 FMC_VEH_heure_debut		: no comment
*	 FMC_VEH_heure_fin_prevue	: no comment
*	 FMC_VEH_heure_fin		: no comment
*        FMC_VEH_fmc_cause_numero	: numero de la fiche main courante cause de cette fmc
*        FMC_VEH_fmc_cause_district	: numero du district de la fiche main courante cause de cette fmc
*        FMC_VEH_loc_autoroute		: nom de l'autoroute de la fmc (deniere localisation) lue ds RES_AUT
*	 FMC_VEH_loc_pr			: pr de la fmc en metres (derniere localisation)
*	 FMC_VEH_loc_sens		: sens de la fmc (derniere localisation)
*	 FMC_VEH_origine_creation	: commentaire sur l'origine de la fmc
*	 FMC_VEH_alerte_numero		: numero de l'alerte qui a occasionne la creation de la fmc
*	 FMC_VEH_alerte_site		: code du site de l'alerte
*	 FMC_VEH_alerte_type		: code indiquant le type d'alerte
*	 FMC_VEH_alerte_type_equipement	: code indiquant le type d'equipement generant l'alerte
*	 FMC_VEH_alerte_equipement	: numero de l'equipement generant l'alerte
*	 FMC_VEH_alerte_commentaire	: commentaire sur l'alerte
*	 FMC_VEH_type_vehicule		: libelle du type de vehicule
*	 FMC_VEH_nombre_passagers	: nombre de passagers
*	 FMC_VEH_presence_bebe		: un bebe est-il dans le vehicule oui(1) ou non(0)
*	 FMC_VEH_presence_personne_agee	: presence oui(1) ou non(0) d'une personne agee
*	 FMC_VEH_presence_animaux	: presence oui(1) ou non(0) d'animaux vivants
*	 FMC_VEH_vehicule_ESCOTA	: le vehicule est ESCOTA (1) ou non (0)
*	 FMC_VEH_volume_chargement	: nombre de m3 ou tonnes
*	 FMC_VEH_matieres_dangereuses	: code de la matiere dangereuse
*	 FMC_VEH_commentaire		: texte complementaire de commentaire
*	 FMC_VEH_marque			: libelle de la marque du vehicule
*	 FMC_VEH_couleur		: libelle de la couleur du vehicule
*	 FMC_VEH_immatriculation	: numero d'immatriculation du vehicule
*	 FMC_VEH_pays			: libelle du pays d'origine du vehicule
*	 FMC_VEH_proprietaire		: nom du proprietaire du vehicule
*	 FMC_VEH_adresse		: adresse du proprietaire
*	 FMC_VEH_ville			: ville du proprietaire
*	 FMC_VEH_nature_panne		: libelle de la nature de la panne
*	 FMC_VEH_positionnement		: positionnement du vehicule
*	 FMC_VEH_code_danger		: code du niveau de danger
*
*  Remarque
*  --------
*  Pour sélectionner une fmc, il faut préciser son numéro et sa clé car seul le couple
*  (numéro, clé) est unique sur tous les sites.
*  Un equipement generant une alerte est clairement identifie par son numero et le numero
*  du type d'equipement. En effet le numero d'equipement est unique par type d'equipement.
*  le champ numero de vehicule permet de referencer les vehicules (dans le cas ou il y en
*  aurait plusieurs) implques dans un accident. Pour un vehicule en panne le champ aura la
*  valeur zero.
*/


/* Suppression de la vue si elle existe */
/* ===========================*/
if exists (select * from sysobjects where name = "FMC_VEH" and type = 'V')
	begin
	drop view FMC_VEH
	end
go


create view FMC_VEH
(
FMC_VEH__numero,
FMC_VEH__district,
FMC_VEH__numero_vehicule,
FMC_VEH_type,
FMC_VEH_heure_debut,
FMC_VEH_heure_fin_prevue,
FMC_VEH_heure_fin,
FMC_VEH_fmc_cause_numero,
FMC_VEH_fmc_cause_district,
FMC_VEH_loc_autoroute,
FMC_VEH_loc_pr,
FMC_VEH_loc_sens,
FMC_VEH_origine_creation,
FMC_VEH_alerte_numero,
FMC_VEH_alerte_site,
FMC_VEH_alerte_type,
FMC_VEH_alerte_type_equipement,
FMC_VEH_alerte_equipement,
FMC_VEH_alerte_commentaire,
FMC_VEH_type_vehicule,
FMC_VEH_nombre_passagers,
FMC_VEH_presence_bebe,
FMC_VEH_presence_personne_agee,
FMC_VEH_presence_animaux,
FMC_VEH_vehicule_ESCOTA,
FMC_VEH_volume_chargement,
FMC_VEH_matieres_dangereuses,
FMC_VEH_commentaire,
FMC_VEH_marque,
FMC_VEH_couleur,
FMC_VEH_immatriculation,
FMC_VEH_pays,
FMC_VEH_proprietaire,
FMC_VEH_adresse,
FMC_VEH_ville,
FMC_VEH_nature_panne,
FMC_VEH_positionnement,
FMC_VEH_code_danger
)
as select
GEN1.numero,
GEN1.cle,
VEH1.numero_vehicule,
TYP1.nom,
GEN1.debut,
GEN1.fin_prevue,
GEN1.fin,
GEN1.cause,
GEN1.cle_cause,
RES1.nom,
HIS1.PR,
HIS1.sens,
GEN1.origine_creation,
GEN1.alerte,
GEN1.sit,
ALT1.type,
ALT1.type_equipement,
ALT1.equipement,
ALT1.texte,
DICO1.libelle,
VEH1.nombre_passagers,
VEH1.bebe,
VEH1.personne_agee,
VEH1.animaux_vivants,
VEH1.vehicule_ESCOTA,
VEH1.volume_de_chargement,
VEH1.matieres_dangereuses,
VEH1.commentaire,
VEH1.marque,
VEH1.couleur,
VEH1.immatriculation,
VEH1.pays,
VEH1.nom,
VEH1.adresse,
VEH1.ville,
VEH1.nature_panne,
VEH1.positionnement,
VEH1.code_danger

from HIS..FMC_GEN GEN1,HIS..FMC_HIS HIS1,HIS..FMC_VEH VEH1,
CFG..RES_AUT RES1,HIS..ALT_EXP ALT1,CFG..TYP_FMC TYP1,VUE..DICO DICO1

where
/* realisation de la jointure entre FMC_GEN et FMC_HIS */
/* --------------------------------------------------- */
GEN1.numero=HIS1.numero
and GEN1.cle=HIS1.cle
/* realisation de la jointure entre FMC_GEN et FMC_VEH */
/* --------------------------------------------------- */
and GEN1.numero=VEH1.numero
and GEN1.cle=VEH1.cle
/* realisation de la jointure entre FMC_VEH et DICO */
/* --------------------------------------------------- */
and VEH1.type_de_vehicule*=DICO1.code
and DICO1.notyp=1
/* realisation de la jointure entre FMC_GEN et TYP_FMC */
/* --------------------------------------------------- */
and GEN1.type=TYP1.numero
/* realisation de la jointure entre FMC_HIS et RES_AUT */
/* --------------------------------------------------- */
and HIS1.autoroute=RES1.numero
/* realisation de la jointure externe entre FMC_GEN et ALT_EXP */
/* On veut avoir toutes les fmc meme celles qui n'ont pas d'alerte */
/* --------------------------------------------------------------- */
and GEN1.alerte*=ALT1.numero
and GEN1.sit*=ALT1.sit
/* selection de l'historique pour la derniere validation */
/* ----------------------------------------------------- */
and HIS1.horodate_validation in
(
select max(HIS2.horodate_validation)
from HIS..FMC_HIS HIS2
where
HIS2.numero=HIS1.numero
and HIS2.cle=HIS1.cle
group by HIS2.numero,HIS2.cle
)

grant all on FMC_VEH to public

go
