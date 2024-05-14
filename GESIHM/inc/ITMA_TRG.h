/*E*/
/* Fichier : $Date: 2012/07/04 15:02:11 $    Release : $Revision: 1.1 $        Date :  $Date: 2012/07/04 15:02:11 $
-------------------------------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
-------------------------------------------------------------------------------
* SOUS-SYSTEME  GESIHM
-------------------------------------------------------------------------------
* MODULE MTMC * FICHIER ITMA_TRG.h
-------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  structure regulation
*
*   
*
-------------------------------------------------------------------------------
* HISTORIQUE :
* JMG	09/04/12	creation DEM/1016
*
*
*
*
-----------------------------------------------------------------------------*/
FORMAT T_zone
        numero,
        nom,
        autoroute,
        sens,
        debut,
        fin,
        vitesse_amont,
        inhibition

FORMAT T_commande
        prv,
        scenario,
        commande,
        prv_critique


FORMAT T_scenario
        numero,
        libelle,
        niveau,
        nominal,
        degrade,
        zone,
        inhibition

FORMAT T_station
        numero,
        autoroute,
        sens,
        PR,
        zone

FORMAT T_FMC
        numero_type,
        qualification

FORMAT T_prv
        numero,
        autoroute,
        PR,
        sens,
        vitesse_scenario,
        critique,
        vitesse_nominale,
        type,
        zone,
	bretelle_entree,
	critique_decrement,
	critique_distance,
	critique_vitesse

FORMAT T_condition
        numero,
        libelle,
        type_horaire,
        jour,
        horodate_debut,
        horodate_fin,
        heure_debut,
        minute_debut,
        heure_fin,
        minute_fin,
        cond_alternative,
        seuil,
        donnee_trafic,
        operateur,
        scenario,
        conjonction1,
        conjonction2,
        conjonction3,
        conjonction4,
        station,
        type_jour,
	sens,
	simple


