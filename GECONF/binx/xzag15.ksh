#!/bin/ksh
# Fichier : @(#)xzag15.ksh	1.5      Release : 1.5        Date : 02/04/98
#################################################################################
#
# STERIA # PROJET MIGRAZUR
#################################################################################
#
# SOUS-SYSTEME BASEDD
#################################################################################
#
# SCRIPT xzag16.ksh
#################################################################################
#
# DESCRIPTION DU SCRIPT :
#
# Dechencher le demarrage d'un serveur de replication
#
#################################################################################
#
# HISTORIQUE DES MODIFICATIONS :
# C.T.  25/11/94        : Creation non teste (V 1.1)
# C.T.  20/03/95        : Modif le nom du serveur de replication (V 1.2)
# C.T.  02/09/96        : Ajout serveur DC et DS  (V 1.3)
# P.N.  29/01/98	: Ajout DA DY DP 1.4
# P.F.	30/05/05	: Ajout HIS
# P.F.  25/06/10	: Suppression de DN DA DP 
#################################################################################
#

#
#################################################################################
#
# Service rendu
# Dechencher le demarrage d'un serveur de replication (CI ou DM ou DC ou DS ou DP ou HIS)
#
# Sequence d'appel
# xzag16.ksh Site 
#
# Arguments en entree
# Site          : CI ou DM ou DC ou DS ou DP ou HIS
#
# Code retour
# 0             : sauvegarde ok
# 1             : syntaxe d'appel incorrecte
#
# Conditions d'utilisation
#
#################################################################################
#

# tester si le nom du site est correct
if [[ $1 != "CI" && $1 != "DM" && $1 != "DC" && $1 != "DS" && $1 != "DP" && $1 != "HIS" ]]
then
	echo "syntaxe : xzag15.ksh site"
	echo "          site : CI ou DM ou DC ou DS ou DP ou HIS"
	exit 1
fi

unset LANG

export SYBASE=/produits2/sybase15
. $SYBASE/SYBASE.sh

# supprimer l'ancien log et renommer l'actuel
rm /produits/migrazur/appliSD/traces/REP_${1}15.log.old
mv /produits/migrazur/appliSD/traces/REP_${1}15.log /produits/migrazur/appliSD/traces/REP_${1}15.log.old

# lancement du serveur de replication
$SYBASE/REP-15_1/install/RUN_REP_${1} &

export LANG=french

exit 0
