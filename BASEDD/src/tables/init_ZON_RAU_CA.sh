#!/bin/sh
#################################################################################
#
# DESCRIPTION DU SCRIPT :
# Initialise la talbe ZON_RAU sur CFT et CFG pour CNA
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
insert into ZON_RAU (zone, autoroute, debut, fin) VALUES('Z_CNA_A11', 101, 258700, 277454)
go" | isql -Usa -Pzigramur -SSQL_CA

echo "
use CFG
go
insert into ZON_RAU (zone, autoroute, debut, fin) VALUES('Z_CNA_A11', 101, 258700, 277454)
go" | isql -Usa -Pzigramur -SSQL_CA
