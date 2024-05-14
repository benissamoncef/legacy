#!/bin/ksh
# Fichier : %W%      Release : %I%        Date : %G%
#################################################################################
#
# STERIA # PROJET MIGRAZUR
#################################################################################
#
# SOUS-SYSTEME BASEDD
#################################################################################
#
# SCRIPT xzag22.ksh
#################################################################################
#
# DESCRIPTION DU SCRIPT :
#
# Dechencher le demarrage de la replication
#
#################################################################################
#
# HISTORIQUE DES MODIFICATIONS :
# B.G.  20/01/95	: creation (1.1)
# C.T.  10/08/95	: ajout d'un resume connection sur exploitation et
#                         l'historique (1.2)
# C.T.  08/09/95	: modif test de presence (1.3)
# P.N.  29/01/98	: Ajout DA DY DP 1.4
# PNI	01/10/12	: transformation des noms secteurs en noms districts pour lancement REP + BACKUP v1.5
#################################################################################
#

#
#################################################################################
#
# Service rendu
# Dechenche le demarrage de la replication apres avoir verifie qu'elle ne tourne pas
#
# Sequence d'appel
# xzag22.ksh Site 
#
# Arguments en entree
# Site          : CI ou DN ou DM ou DA ou DP ou DY
#
# Code retour
# 0             : sauvegarde ok
# 1             : syntaxe d'appel incorrecte
#
# Conditions d'utilisation
#
#################################################################################
#

site_d=$1

if [[ $1 = "CA" ]]
then
site_d="DM"
fi
if [[ $1 = "VE" ]]
then
site_d="DC"
fi
if [[ $1 = "PR" ]]
then
site_d="DS"
fi
if [[ $1 = "VD" ]]
then
site_d="DP"
fi

#echo $site_d

# tester si le nom du site est correct
if [[ $site_d != "CI" && $site_d != "DN" && $site_d != "DM" && $site_d != "DC" && $site_d != "DS" && $site_d != "DA" && $site_d != "DP" && $site_d != "DY" ]]
then
	echo "syntaxe : xzag22.ksh site"
	echo "          site : CI ou DN ou DM ou DC ou DS ou DA ou DP ou DY "
	exit 1
fi

#echo "site =" $site_d
#exit

# regarde s'il existe un replication server (ps = 2) sinon le lance

if [[ `ps -ef | grep "repser" | grep -c "epserver"` = 0 ]]
then

/produits/migrazur/appliSD/exec/xzag15.ksh $site_d

sleep 20


fi

# regarde s'il existe un backup server (ps = 2) sinon le lance

if [[ `ps -ef | grep "backupser" | grep -c "ackupserver"` = 0 ]]
then
/produits/migrazur/appliSD/exec/xzag17.ksh $site_d
fi

sleep 30
# faire un resume des connexions de replications pour parer au pb de 
# reconnexion de tasrv
#/produits/migrazur/appliSD/exec/xzag23.ksh $site_d

exit 0
