#!/bin/sh
#################################################################################
#
# DESCRIPTION DU SCRIPT :
# Initialise la talbe RES_PRA sur CFT et CFG pour CNA
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
insert into RES_PRA (numero, code, serveur_pra) VALUES(15, 'CA', 'SQL_CA')
go" | isql -Usa -Pzigramur -SSQL_CA

echo "
use CFG
go
insert into RES_PRA (numero, code, serveur_pra) VALUES(15, 'CA', 'SQL_CA')
go" | isql -Usa -Pzigramur -SSQL_CA
