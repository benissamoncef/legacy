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
insert into LST_FMC_MAC (sit, machine, operateur, liste, colonne) VALUES(15, NULL, NULL, 1, 11)
insert into LST_FMC_MAC (sit, machine, operateur, liste, colonne) VALUES(15, NULL, NULL, 2, 12)
insert into LST_FMC_MAC (sit, machine, operateur, liste, colonne) VALUES(15, NULL, NULL, 3, 13)
go" | isql -Usa -Pzigramur -SSQL_CA

echo "
use CFG
go
insert into LST_FMC_MAC (sit, machine, operateur, liste, colonne) VALUES(15, NULL, NULL, 1, 11)
insert into LST_FMC_MAC (sit, machine, operateur, liste, colonne) VALUES(15, NULL, NULL, 2, 12)
insert into LST_FMC_MAC (sit, machine, operateur, liste, colonne) VALUES(15, NULL, NULL, 3, 13)
go" | isql -Usa -Pzigramur -SSQL_CA
