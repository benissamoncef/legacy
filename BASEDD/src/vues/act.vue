/*X  Fichier : @(#)act.vue	0.1      Release : 0.1        Date : 12/08/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER act.vue
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* creation de la vue ACT
*
------------------------------------------------------
* HISTORIQUE :
*
* L.V.	12/08/96	: Creation
-----------------------------------------------------*/


/* ============================================================ */
/*   Nom de la base   :  MIGRAZUR                               */
/*   Nom de SGBD      :  SYBASE10                               */
/*   Date de creation :  12/08/96                               */
/* ============================================================ */

/* ============================================================ */
/*   Vue : ACT	                                                */
/* ============================================================ */

declare @nom_base varchar(10)

/* nom de la base courante */
select @nom_base = db_name()

/*
*  Fonctionalite de la vue
*  -------------------------
*  Contient les caracteristiques generales des actions
*
*  Description des colonnes de la vue
*  ------------------------------------
*        ACT__numero		: numero d'une action
*        ACT__district		: numero du site de l'action
*        ACT_fmc_cause_numero	: numero de la fiche main courante cause de l'action
*        ACT_fmc_cause_district	: numero de cle de la fiche main courante cause de l'action
*        ACT_type		: libelle du type d'action    
*        ACT_heure_lancement	: date de lancement de l'action
*        ACT_heure_succes	: date de succes de l'action
*        ACT_heure_echec	: date d'echec de l'action
*        ACT_heure_lancement_fin: date de lancement de fin de l'action
*        ACT_heure_fin		: date de fin de l'action
*
*  Remarque
*  --------
*  Le numero d'action est unique par site (district). Pour selectionner une
*  action, il faut preciser son numero et son site.
*
*  Voici la liste exhaustive des types d'action :
*      type :            lib≈ll≈                                : compl≈ment
*      ---------------------------------------------------------------------
*        1  : action sur des biseaux de rabattement automatique : ACT_NAV
*        2  : action sur un panneau » message variable          : ACT_PMV
*        3  : action sur un ≈quipement de tunnel                : ACT_TUB
*        4  : action de d≈pannage                               : ACT_DEP
*        5  : action d'affichage en gare                        : ACT_GAR
*        6  : action d'intervention de patrouille SIS           : ACT_SIS
*        7  : action d'intervention de patrouille               : ACT_INT
*        8  : action sur un panneau » message var. sur remorque : ACT_RMQ
*        9  : action sur un panneau » message var. sur fourgon  : ACT_FRG
*        10 : action trafic FM                                  : ACT_TFM + ACT_TXT_TFM
*        11 : action sur un ≈changeur                           : ACT_ECH
*        12 : action d'appel par fax                            : ACT_APL + ACT_FAX
*        13 : action d'appel t≈l≈phonique                       : ACT_APL
*        14 : action d'appel par bip                            : ACT_APL
*/

/* Suppression de la vue si elle existe */
/* ===========================*/
if exists (select * from sysobjects where name = 'ACT' and type = 'V')
	begin
	drop view ACT
	end
go


create view ACT
(
ACT__numero,
ACT__district,
ACT_fmc_cause_numero,
ACT_fmc_cause_district,
ACT_type,
ACT_heure_lancement,
ACT_heure_succes,
ACT_heure_echec,
ACT_heure_lancement_fin,
ACT_heure_fin
)
as select
GEN1.numero,
GEN1.sit,
GEN1.evenement,
GEN1.cle,
DICO1.libelle,
GEN1.heure_lancement,
GEN1.heure_succes,
GEN1.heure_echec,
GEN1.heure_lancement_fin,
GEN1.heure_fin

from HIS..ACT_GEN GEN1,VUE..DICO DICO1

where
GEN1.type=DICO1.code
and DICO1.notyp=9

/* ajout des droits a public*/
grant all on ACT to public

go
