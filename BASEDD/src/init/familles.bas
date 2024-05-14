/*X  Fichier : $Id: familles.bas,v 1.2 1996/10/31 10:52:43 gaborit Exp $      Release : $Revision: 1.2 $        Date : $Date: 1996/10/31 10:52:43 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER fonctions.bas
------------------------------------------------------
* DESCRIPTION :
* renseigne les fonctions par machine de CFG a partir 
* des constantes d≈finies dans XDC (» faire au CI)
------------------------------------------------------
* HISTORIQUE :
*
* B.GABORIT	11/10/94	: Creation
* C.T.     	15/03/95	: Creation avec entete (V 1.1)
* C.T.     	31/10/96	: Ajout familles PC simplifie pour PODS1 et PODC1 (V1.2)
----------------------------------------------------*/

use CFG
go

delete OPR_FCT_MAC
go

/*       1 ADMINISTRATION SYSTEME tout le monde */
insert OPR_FCT_MAC (machine,fonction)
(select numero,1 from EQT_GEN where type=13)
go

/*      2 DEVELOPPEMENT CONFIG. podfa */
insert OPR_FCT_MAC (machine,fonction)
(select numero,2 from EQT_GEN where type=13 and nom like "PODFA%")
go

/*      4 EXPLOITATION poci, podn ,podm */
insert OPR_FCT_MAC (machine,fonction)
(select numero,4 from EQT_GEN where type=13 and
	(nom like "POCI%" or nom like "PODN%" or nom like "PODM%")
	and not nom like "PODMA%"
)
go

/*      5 EXPLOITATION DISTRICT podn ,podm */
insert OPR_FCT_MAC (machine,fonction)
(select numero,5 from EQT_GEN where type=13 and
	(nom like "PODN%" or nom like "PODM%")
	and not nom like "PODMA%"
)
go

/*      6 EXPLOITATION CI poci */
insert OPR_FCT_MAC (machine,fonction)
(select numero,6 from EQT_GEN where type=13 and nom like "POCI%")
go

/*      7 RADIO TRAFIC FM potfm */
insert OPR_FCT_MAC (machine,fonction)
(select numero,7 from EQT_GEN where type=13 and nom like "POTFM%")
go

/*      8 ADMINISTRER L'ANNUAIRE machines ci */
insert OPR_FCT_MAC (machine,fonction)
(select numero,8 from EQT_GEN where type=13 and
			(  nom like "POCI%"
			or nom like "POGT%"
			or nom like "POTFM%"
			or nom like "PODFA%"
			or nom like "SPCI%"
			or nom like "POCMA%"
			or nom like "PODMA%"
			)
)
go


/*      9 CONSULTER L'ANNUAIRE po... et sp... sauf polt... */
insert OPR_FCT_MAC (machine,fonction)
(select numero,9 from EQT_GEN where type=13 and
	(nom like "PO%" or nom like "SP%")
	and not nom like "POLT%"
)
go

/*     10 VISUALISER SYNOPTIQUES po.. sp.. ss.. */
insert OPR_FCT_MAC (machine,fonction)
(select numero,10 from EQT_GEN where type=13 and
	(nom like "PO%" or nom like "SP%" or nom like "SS%")
)
go

/*     11 VISUALISER COURBES po... et sp... sauf polt... */
insert OPR_FCT_MAC (machine,fonction)
(select numero,11 from EQT_GEN where type=13 and
	(nom like "PO%" or nom like "SP%")
	and not nom like "POLT%"
)
go

/*     12 VISUALISER STATISTIQUES po... et sp... sauf polt... */
insert OPR_FCT_MAC (machine,fonction)
(select numero,12 from EQT_GEN where type=13 and
	(nom like "PO%" or nom like "SP%")
	and not nom like "POLT%"
)
go

/*     13 VISUALISER EXPLOITATION po... et sp... sauf polt... */
insert OPR_FCT_MAC (machine,fonction)
(select numero,13 from EQT_GEN where type=13 and
	(nom like "PO%" or nom like "SP%")
	and not nom like "POLT%"
)
go

/*     14 COMM. CONFIG. MAINTENANCE */
insert OPR_FCT_MAC (machine,fonction)
(select numero,14 from EQT_GEN where type=13 and nom like "POCMA%")
go

/*     15 MAINTENANCE po... et sp... sauf polt... */
insert OPR_FCT_MAC (machine,fonction)
(select numero,15 from EQT_GEN where type=13 and
	(nom like "PO%" or nom like "SP%")
	and not nom like "POLT%"
)
go

/*     16 FENETRE LCR polt.. podma.. pocma.. */
insert OPR_FCT_MAC (machine,fonction)
(select numero,16 from EQT_GEN where type=13 and
	(nom like "POLT%" or nom like "POCMA%" or nom like "PODMA%")
)
go

/*     17 ETUDIANT podfa */
insert OPR_FCT_MAC (machine,fonction)
(select numero,17 from EQT_GEN where type=13 and nom like "PODFA%")
go

/*     19 ENSEIGNANT podfa */
insert OPR_FCT_MAC (machine,fonction)
(select numero,19 from EQT_GEN where type=13 and nom like "PODFA%")
go

/*     20 SAISIE PREVISIONS FMC po... et sp... sauf polt... */
insert OPR_FCT_MAC (machine,fonction)
(select numero,20 from EQT_GEN where type=13 and
	(nom like "PO%" or nom like "SP%")
	and not nom like "POLT%"
)
go

/*     21 MODIF HISTORIQUE FMC machines CI */
insert OPR_FCT_MAC (machine,fonction)
(select numero,21 from EQT_GEN where type=13 and
			(  nom like "POCI%"
			or nom like "POGT%"
			or nom like "POTFM%"
			or nom like "PODFA%"
			or nom like "SPCI%"
			or nom like "POCMA%"
			or nom like "PODMA%"
			)
)
go

/*     22 PLANS DE SECOURS po... et sp... sauf polt... */
insert OPR_FCT_MAC (machine,fonction)
(select numero,22 from EQT_GEN where type=13 and
	(nom like "PO%" or nom like "SP%")
	and not nom like "POLT%"
)
go

/*     23 OUTILS TABLEUR po... et sp... sauf polt... */
insert OPR_FCT_MAC (machine,fonction)
(select numero,23 from EQT_GEN where type=13 and
	(nom like "PO%" or nom like "SP%")
	and not nom like "POLT%"
)
go

/*     24 OUTILS INFOCENTRE po... et sp... sauf polt... */
insert OPR_FCT_MAC (machine,fonction)
(select numero,24 from EQT_GEN where type=13 and
	(nom like "PO%" or nom like "SP%")
	and not nom like "POLT%"
)
go

/*     25 PILOTAGE MANUEL NAV podn ,podm */
insert OPR_FCT_MAC (machine,fonction)
(select numero,25 from EQT_GEN where type=13 and
	(nom like "PODN%" or nom like "PODM%")
	and not nom like "PODMA%"
)
go

/*     26 MATRICE NAV podn ,podm */
insert OPR_FCT_MAC (machine,fonction)
(select numero,26 from EQT_GEN where type=13 and
	(nom like "PODN%" or nom like "PODM%")
	and not nom like "PODMA%"
)
go

/*     27 CONFIGURATION CAMERA podn ,podm */
insert OPR_FCT_MAC (machine,fonction)
(select numero,27 from EQT_GEN where type=13 and
	(nom like "PODN%" or nom like "PODM%")
	and not nom like "PODMA%"
)
go

/*     28 STERIA-GTIE-GRANIOU tout le monde */
insert OPR_FCT_MAC (machine,fonction)
(select numero,28 from EQT_GEN where type=13)
go

/*     29 PC SIMPLIFIE podc1, pods1 */
insert OPR_FCT_MAC (machine,fonction)
(select numero,29 from EQT_GEN where type=13 and
	(nom = "PODC1" or nom = "PODS1"))
go
