#!/bin/ksh
# Fichier : @(#)xzag17.ksh      1.3      Release : 1.3        Date : 02/04/98
#################################################################################
#
# STERIA # PROJET MIGRAZUR
#################################################################################
#
# SOUS-SYSTEME BASEDD
#################################################################################
#
# SCRIPT xzag17.ksh
#################################################################################
#
# DESCRIPTION DU SCRIPT :
#
# Dechencher le demarrage d'un backup serveur
#
#################################################################################
#
# HISTORIQUE DES MODIFICATIONS :
# C.T.  25/11/94        : Creation non teste (V 1.1)
# C.T.  02/09/96        : Ajout cas des serveurs DC et DS (V 1.2)
# P.N.  29/01/98        : Ajout DA DY DP 1.3
# P.F.  25/06/10        : Suppression de DN DA DP
# LCL   08/01/20        : Modification $SYBASE --> /opt/sybase
# RGR   13/11/20        : Recuperation de MOVIS : DEM 93
# LCL	11/01/22		: Modification avec variables d'environnement pour migration Sybase 16
#################################################################################
#

#
#################################################################################
#
# Service rendu
# Dechencher le demarrage d'un backup serveur (CI ou DM ou DC ou DS ou DP ou HIS)
#
# Sequence d'appel
# xzag17.ksh Site
#
# Arguments en entree
# Site          : CI ou DM ou DC ou DS ou DP ou HIS
#
# Code retour
# 0             : sauvegarde ok
# 1             : syntaxe d'appel incorrecte
#
# Conditions d'utilisation
# L'execution du script doit etre effectue dans le repertoire
# "/produits/migrazur/appliSD/traces"
#
#################################################################################
#

# tester si le nom du site est correct
if [[ $1 != "CI" && $1 != "DM" && $1 != "DS" && $1 != "HIS" ]]
then
        echo "syntaxe : xzag17.ksh site"
        echo "          site : CI ou DM ou DS ou HIS"
        exit 1
fi

# supprimer l'ancien log et renommer l'actuel
rm /produits/migrazur/appliSD/traces/BACKUP_${1}.log.old
mv /produits/migrazur/appliSD/traces/BACKUP_${1}.log /produits/migrazur/appliSD/traces/BACKUP_${1}.log.old

# lancement
#export SYBASE=/produits/sybase15.7
export SYBASE=/opt/sybase
. $SYBASE/SYBASE.sh
$SYBASE/$SYBASE_ASE/bin/startserver -f $SYBASE/$SYBASE_ASE/install/RUN_BACKUP_${1}

exit 0

