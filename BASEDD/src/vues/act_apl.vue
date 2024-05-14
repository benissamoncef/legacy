/*X  Fichier : @(#)act_apl.vue	0.1      Release : 0.1        Date : 19/08/96
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER act_apl.vue
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* creation de la vue ACT_APL
*
------------------------------------------------------
* HISTORIQUE :
*
* L.V.	19/08/96	: Creation
-----------------------------------------------------*/


/* ============================================================ */
/*   Nom de la base   :  MIGRAZUR                               */
/*   Nom de SGBD      :  SYBASE10                               */
/*   Date de creation :  19/08/96                               */
/* ============================================================ */

/* ============================================================ */
/*   Vue : ACT_APL	                                        */
/* ============================================================ */

declare @nom_base varchar(10)

/* nom de la base courante */
select @nom_base = db_name()

/*
*  Fonctionalite de la vue
*  -------------------------
*  Contient les appels telephoniques,radios,et autres appels sauf fax
*
*  Description des colonnes de la vue
*  ------------------------------------
*        ACT_APL__numero		: numero d'une action
*        ACT_APL__district		: numero du site de l'action
*	 ACT_APL_type_appel		: libelle du type d'appel
*        ACT_APL_fmc_liee_numero	: numero de la fiche main courante cause de l'action
*        ACT_APL_fmc_liee_district	: numero de cle de la fiche main courante cause de l'action
*	 ACT_APL_interlocuteur		: nom de la personne appelee
*	 ACT_APL_telephone		: numero de telephone
*	 ACT_APL_astreinte		: numero du type d'astreinte appele    
*        ACT_APL_heure_succes		: date de succes de l'action
*        ACT_APL_heure_echec		: date d'echec de l'action
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
if exists (select * from sysobjects where name = 'ACT_APL' and type = 'V')
	begin
	drop view ACT_APL
	end
go


create view ACT_APL
(
ACT_APL__numero,
ACT_APL__district,
ACT_APL_type_appel,
ACT_APL_fmc_liee_numero,
ACT_APL_fmc_liee_district,
ACT_APL_interlocuteur,
ACT_APL_telephone,
ACT_APL_astreinte,
ACT_APL_heure_succes,
ACT_APL_heure_echec
)
as select
GEN1.numero,
GEN1.sit,
DICO1.libelle,
GEN1.evenement,
GEN1.cle,
FIC1.nom,
APL1.numero_telephonique,
AST1.nom,
GEN1.heure_succes,
GEN1.heure_echec

from HIS..ACT_GEN GEN1,VUE..DICO DICO1,HIS..ACT_APL APL1,
CFG..ANN_FIC FIC1,CFG..TYP_AST AST1

where
/* jointure entre ACT_GEN et ACT_APL */
/* --------------------------------- */
GEN1.numero=APL1.actio
and GEN1.sit=APL1.sit
/* jointure entre ACT_GEN et DICO */
/* ------------------------------ */
and GEN1.type=DICO1.code
and 9=DICO1.notyp
/* selection de tous les appels sauf appels fax */
/* -------------------------------------------- */
and 12!=DICO1.code
/* jointure entre ANN_FIC et ACT_APL */
/* --------------------------------- */
and FIC1.numero=APL1.interlocuteur
/* jointure entre TYP_AST et ACT_APL */
/* --------------------------------- */
and AST1.numero=APL1.type_d_astreinte

/* ajout des droits a public*/
grant all on ACT_APL to public

go
