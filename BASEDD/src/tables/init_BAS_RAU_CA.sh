#!/bin/sh
#################################################################################
#
# DESCRIPTION DU SCRIPT :
# Initialise la talbe BAS_RAU sur EXP pour CNA
#
#################################################################################
#
#################################################################################
# HISTORIQUE DES MODIFICATIONS :
# GGY	23/11/23	: Creation
#################################################################################

echo "
use EXP
go
insert into BAS_RAU values ('Z_CNA_A11', 'A11', 'LOC', 15)
go" | isql -Usa -Pzigramur -SSQL_CA
