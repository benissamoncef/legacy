#!/bin/sh
#################################################################################
#
# DESCRIPTION DU SCRIPT :
# Initialise la talbe LST_FMC_MAC sur CFT et CFG pour CNA
#
#################################################################################
#
#################################################################################
# HISTORIQUE DES MODIFICATIONS :
# GGY	23/11/23	: Creation
#################################################################################

echo "
use CFT
go
insert into EQT_LOC (type_synoptique, autoroute, sens, equipement, type_equipement, echangeur, x, y, pr_debut, pr_fin) VALUES ('GENERALE', 101, 2, 7, 2, null, 675, 256, null, null)
insert into EQT_LOC (type_synoptique, autoroute, sens, equipement, type_equipement, echangeur, x, y, pr_debut, pr_fin) VALUES ('GENERALE', 101, 2, 6, 2, null, 878, 350, null, null)
insert into EQT_LOC (type_synoptique, autoroute, sens, equipement, type_equipement, echangeur, x, y, pr_debut, pr_fin) VALUES ('GENERALE', 101, 2, 5, 2, null, 1140, 384, null, null)
insert into EQT_LOC (type_synoptique, autoroute, sens, equipement, type_equipement, echangeur, x, y, pr_debut, pr_fin) VALUES ('GENERALE', 101, 1, 4, 2, null, 1041, 235, null, null)
insert into EQT_LOC (type_synoptique, autoroute, sens, equipement, type_equipement, echangeur, x, y, pr_debut, pr_fin) VALUES ('GENERALE', 101, 1, 3, 2, null, 1279, 143, null, null)
insert into EQT_LOC (type_synoptique, autoroute, sens, equipement, type_equipement, echangeur, x, y, pr_debut, pr_fin) VALUES ('GENERALE', 101, 1, 2, 2, null, 1408, 223, null, null)
insert into EQT_LOC (type_synoptique, autoroute, sens, equipement, type_equipement, echangeur, x, y, pr_debut, pr_fin) VALUES ('GENERALE', 101, 1, 1, 2, null, 1605, 321, null, null)
insert into EQT_LOC (type_synoptique, autoroute, sens, equipement, type_equipement, echangeur, x, y, pr_debut, pr_fin) VALUES ('GENERALE', 101, 2, 8, 2, null, 443, 702, null, null)
go" | isql -Usa -Pzigramur -SSQL_CA

echo "
use CFG
go
insert into EQT_LOC (type_synoptique, autoroute, sens, equipement, type_equipement, echangeur, x, y, pr_debut, pr_fin) VALUES ('GENERALE', 101, 2, 7, 2, null, 675, 256, null, null)
insert into EQT_LOC (type_synoptique, autoroute, sens, equipement, type_equipement, echangeur, x, y, pr_debut, pr_fin) VALUES ('GENERALE', 101, 2, 6, 2, null, 878, 350, null, null)
insert into EQT_LOC (type_synoptique, autoroute, sens, equipement, type_equipement, echangeur, x, y, pr_debut, pr_fin) VALUES ('GENERALE', 101, 2, 5, 2, null, 1140, 384, null, null)
insert into EQT_LOC (type_synoptique, autoroute, sens, equipement, type_equipement, echangeur, x, y, pr_debut, pr_fin) VALUES ('GENERALE', 101, 1, 4, 2, null, 1041, 235, null, null)
insert into EQT_LOC (type_synoptique, autoroute, sens, equipement, type_equipement, echangeur, x, y, pr_debut, pr_fin) VALUES ('GENERALE', 101, 1, 3, 2, null, 1279, 143, null, null)
insert into EQT_LOC (type_synoptique, autoroute, sens, equipement, type_equipement, echangeur, x, y, pr_debut, pr_fin) VALUES ('GENERALE', 101, 1, 2, 2, null, 1408, 223, null, null)
insert into EQT_LOC (type_synoptique, autoroute, sens, equipement, type_equipement, echangeur, x, y, pr_debut, pr_fin) VALUES ('GENERALE', 101, 1, 1, 2, null, 1605, 321, null, null)
insert into EQT_LOC (type_synoptique, autoroute, sens, equipement, type_equipement, echangeur, x, y, pr_debut, pr_fin) VALUES ('GENERALE', 101, 2, 8, 2, null, 443, 702, null, null)
go" | isql -Usa -Pzigramur -SSQL_CA
