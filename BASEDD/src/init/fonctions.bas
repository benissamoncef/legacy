/*X  Fichier : $Id: fonctions.bas,v 1.2 1996/10/31 10:55:32 gaborit Exp $      Release : $Revision: 1.2 $        Date : $Date: 1996/10/31 10:55:32 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER fonctions.bas
------------------------------------------------------
* DESCRIPTION :
* renseigne la table de fonctions operateur de CFG a partir 
* des constantes d≈finies dans XDC (» faire au CI)
------------------------------------------------------
* HISTORIQUE :
*
* B.GABORIT	11/10/94	: Creation
* C.T.     	15/03/95	: Creation sous un autre fichier
* C.T.     	31/10/96	: Ajout fonction pc simplifie (1.2)
----------------------------------------------------*/

use CFG
go


/* cr≈ation des familles de fonction */

delete OPR_FCT_NOM
insert OPR_FCT_NOM (numero,nom)
values (XDC_FAM_ADMINISTRATION,"ADMINISTRATION SYSTEME")
insert OPR_FCT_NOM (numero,nom)
values (XDC_FAM_DEVEL_CONFIG ,"DEVELOPPEMENT CONFIG.")
insert OPR_FCT_NOM (numero,nom)
values (XDC_FAM_EXPLOITATION ,"EXPLOITATION")
insert OPR_FCT_NOM (numero,nom)
values (XDC_FAM_EXPLOITATION_DISTRICT ,"EXPLOITATION DISTRICT")
insert OPR_FCT_NOM (numero,nom)
values (XDC_FAM_EXPLOITATION_CI ,"EXPLOITATION CI")
insert OPR_FCT_NOM (numero,nom)
values (XDC_FAM_RADIO_TRAFIC_FM ,"RADIO TRAFIC FM")
insert OPR_FCT_NOM (numero,nom)
values (XDC_FAM_ADM_ANNUAIRE ,"ADMINISTRER L'ANNUAIRE")
insert OPR_FCT_NOM (numero,nom)
values (XDC_FAM_CONSULT_ANNUAIRE ,"CONSULTER L'ANNUAIRE")
insert OPR_FCT_NOM (numero,nom)
values (XDC_FAM_VISU_SYNOPTIQUES ,"VISUALISER SYNOPTIQUES")
insert OPR_FCT_NOM (numero,nom)
values (XDC_FAM_VISU_COURBES ,"VISUALISER COURBES")
insert OPR_FCT_NOM (numero,nom)
values (XDC_FAM_VISU_STATS ,"VISUALISER STATISTIQUES")
insert OPR_FCT_NOM (numero,nom)
values (XDC_FAM_VISU_EXPLOITATION ,"VISUALISER EXPLOITATION")
insert OPR_FCT_NOM (numero,nom)
values (XDC_FAM_COMM_CONFIG_MAINTENANCE ,"COMM. CONFIG. MAINTENANCE")
insert OPR_FCT_NOM (numero,nom)
values (XDC_FAM_MAINTENANCE ,"MAINTENANCE")
insert OPR_FCT_NOM (numero,nom)
values (XDC_FAM_LCR ,"FENETRE LCR")
insert OPR_FCT_NOM (numero,nom)
values (XDC_FAM_ETUDIANT ,"ETUDIANT")
insert OPR_FCT_NOM (numero,nom)
values (XDC_FAM_ENSEIGNANT ,"ENSEIGNANT")
insert OPR_FCT_NOM (numero,nom)
values (XDC_FAM_PREVISION ,"SAISIE PREVISIONS FMC")
insert OPR_FCT_NOM (numero,nom)
values (XDC_FAM_MODIF_HISTORIQUE ,"MODIF HISTORIQUE FMC")
insert OPR_FCT_NOM (numero,nom)
values (XDC_FAM_PLAN_SECOURS ,"PLANS DE SECOURS")
insert OPR_FCT_NOM (numero,nom)
values (XDC_FAM_OUTILS_TABLEUR ,"OUTILS TABLEUR")
insert OPR_FCT_NOM (numero,nom)
values (XDC_FAM_OUTILS_INFOCENTRE ,"OUTILS INFOCENTRE")
insert OPR_FCT_NOM (numero,nom)
values (XDC_FAM_PILOTAGE_MANUEL_NAV ,"PILOTAGE MANUEL NAV")
insert OPR_FCT_NOM (numero,nom)
values (XDC_FAM_MATRICE_NAV ,"MATRICE NAV")
insert OPR_FCT_NOM (numero,nom)
values (XDC_FAM_CONFIG_CAMERA ,"CONFIGURATION CAMERA")
insert OPR_FCT_NOM (numero,nom)
values (XDC_FAM_DEV_MIGRAZUR ,"STERIA-GTIE-GRANIOU")
insert OPR_FCT_NOM (numero,nom)
values (XDC_FAM_PC_SIMPLIFIE ,"PC SIMPLIFIE")
go
