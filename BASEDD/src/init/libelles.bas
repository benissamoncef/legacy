/*X  Fichier : $Id: libelles.bas,v 1.3 1996/11/28 10:30:16 gaborit Exp $      Release : $Revision: 1.3 $        Date : $Date: 1996/11/28 10:30:16 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER libelles.bas
------------------------------------------------------
* DESCRIPTION DE LA PROCEDURE :
*
* Initialisation de la table de types de libelles
*
------------------------------------------------------
* HISTORIQUE :
*
* P.V.  19/06/96: Creation
* C.T.  28/11/96: Modif type FMC et ajout nouveaux types
-----------------------------------------------------*/


use CFG
go

insert LIB_TYP (notyp, nom, lgmax, explication, ajposs)
values( 1, "VEHICULE/categories      ", 25, "Catégorie de véhicules : PL, VL, ...", 0)
insert LIB_TYP (notyp, nom, lgmax, explication, ajposs)
values( 2, "VEHICULE/marques         ", 25, "Marque du véhicule", 1)
insert LIB_TYP (notyp, nom, lgmax, explication, ajposs)
values( 3, "VEHICULE/couleurs                 ", 25, "Couleur du véhicule", 1)
insert LIB_TYP (notyp, nom, lgmax, explication, ajposs)
values( 4, "VEHICULE/code matiere    ", 4, "Code de la matiere dangereuse", 1)
insert LIB_TYP (notyp, nom, lgmax, explication, ajposs)
values( 5, "VEHICULE/code danger     ", 4, "Code du niveau de danger", 1)
insert LIB_TYP (notyp, nom, lgmax, explication, ajposs)
values( 6, "Pays                     ", 25, "Pays d'immatriculation du véhicule", 1)
insert LIB_TYP (notyp, nom, lgmax, explication, ajposs)
values( 7, "VEHICULE/natures pannes  ", 25, "Nature de la panne du véhicule", 1)
insert LIB_TYP (notyp, nom, lgmax, explication, ajposs)
values( 8, "VEHICULE/demandeurs enlev", 25, "Personne qui demande l'enlevement du vehicule", 1)
insert LIB_TYP (notyp, nom, lgmax, explication, ajposs)
values( 9, "Type d'action            ", 20, "Liste des differents types d'actions", 0)
insert LIB_TYP (notyp, nom, lgmax, explication, ajposs)
values( 10, "Messages-fourgon        ", 10, "Affichage sur fourgon", 1)
insert LIB_TYP (notyp, nom, lgmax, explication, ajposs)
values( 11, "Type d'intervenant      ", 15, "Liste des intervenants possibles", 0)
insert LIB_TYP (notyp, nom, lgmax, explication, ajposs)
values( 12, "Qualification alerte    ", 20, "Qualification possible des alertes", 0)
insert LIB_TYP (notyp, nom, lgmax, explication, ajposs)
values( 13, "Type de bip             ", 20, "Liste des types de bip", 0)
insert LIB_TYP (notyp, nom, lgmax, explication, ajposs)
values( 14, "Type de neutralisation  ", 25, "Neutralisations possibles", 0)
insert LIB_TYP (notyp, nom, lgmax, explication, ajposs)
values( 15, "Type de bouchon         ", 8, "Types de bouchons possibles", 0)
insert LIB_TYP (notyp, nom, lgmax, explication, ajposs)
values( 16, "Classes de FMC          ", 25, "Differentes classes de FMC", 0)
insert LIB_TYP (notyp, nom, lgmax, explication, ajposs)
values( 17, "Sens pour localisation  ", 20, "Sens pour localisation", 0)
insert LIB_TYP (notyp, nom, lgmax, explication, ajposs)
values( 18, "Type point caracteristiq", 12, "Type de point caracteristique pour localisation", 0)
insert LIB_TYP (notyp, nom, lgmax, explication, ajposs)
values( 19, "Position aire/echangeur ", 45, "Position sur aire ou echangeur", 0)
insert LIB_TYP (notyp, nom, lgmax, explication, ajposs)
values( 20, "Type de perturbation    ", 20, "Types de perturbations possibles", 0)
insert LIB_TYP (notyp, nom, lgmax, explication, ajposs)
values( 21, "Type de travaux         ", 35, "Types de travaux possibles", 0)
insert LIB_TYP (notyp, nom, lgmax, explication, ajposs)
values( 22, "Nature des travaux      ", 20, "Nature de travaux possibles", 1)
insert LIB_TYP (notyp, nom, lgmax, explication, ajposs)
values( 23, "Type d'equipement       ", 30, "Types possibles pour les equipements", 0)
insert LIB_TYP (notyp, nom, lgmax, explication, ajposs)
values( 24, "Classes de Silhouettes  ", 30, "Differentes classes de Silhouettes", 0)
insert LIB_TYP (notyp, nom, lgmax, explication, ajposs)
values( 25, "Classes de poids d'essieu", 30, "Differentes classes de poids d'essieux", 0)
insert LIB_TYP (notyp, nom, lgmax, explication, ajposs)
values( 26, "Classes de Poids        ", 30, "Differentes classes de Poids", 0)
insert LIB_TYP (notyp, nom, lgmax, explication, ajposs)
values( 27, "Motifs d'appels         ", 30, "Types de motifs d'appels recus au CI", 1)
insert LIB_TYP (notyp, nom, lgmax, explication, ajposs)
values( 28, "Type de FMC             ", 25, "Types possibles pour les FMC", 0)
insert LIB_TYP (notyp, nom, lgmax, explication, ajposs)
values( 29, "Pluie                   ", 30, "Code de Pluie", 1)

go
