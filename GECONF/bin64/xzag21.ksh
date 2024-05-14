#!/bin/ksh
# Fichier : @(#)xzag21.ksh	1.9      Release : 1.9        Date : 02/04/98
#################################################################################
#
# STERIA # PROJET MIGRAZUR
#################################################################################
#
# SOUS-SYSTEME BASEDD
#################################################################################
#
# SCRIPT xzag21.ksh
#################################################################################
#
# DESCRIPTION DU SCRIPT :
#
# Dechencher le demarrage d'un sql server
#
#################################################################################
#
# HISTORIQUE DES MODIFICATIONS :
# C.T.  25/11/94        : Creation non teste (V 1.1)
# B.G.  20/01/95	: modif rend la main quand le sql est ok (1.2)
# B.G.  20/01/95	: determine le site en fonction du nom machine (1.4)
# C.T.  21/03/95	: modif le nom des fichier log (1.5)
# T.M  19/04/95		: Ajout du Time Zone (1.6)
# C.T.  15/10/96	: Ajout saint maximin (1.8)
# P.N.  29/01/98	: Ajout DA DY DP 1.4
# P.F.  25/06/10        : Suppression de DN DA DP
# LCL	08/01/20	: Modification $SYBASE --> /opt/sybase
# RGR	13/11/20	: Recuperation de MOVIS DEM 93
#################################################################################
#

#
#################################################################################
#
# Service rendu
# Dechencher le demarrage d'un server (CI ou DM ou DC ou DS ou DP ou HIS)
#
# Sequence d'appel
# xzag21.ksh Site 
#
# Arguments en entree
#
# Code retour
# 0             : sauvegarde ok
# 1             : syntaxe d'appel incorrecte
#
# Conditions d'utilisation
#
#################################################################################
#

site="XX"

# tester le nom du site
if [[ `hostname` = "SDCI1" ]]
then
site="CI"
fi

if [[ `hostname` = "SDVC1" ]]
then
site="DM"
fi

if [[ `hostname` = "SDDP1" ]]
then
site="DS"
fi

if [[ `hostname` = "SDHIS" ]]
then
site="HIS"
fi

if [[ $site = "XX" ]]
then
exit 1
fi


# lancement du sql serveur
rm /produits/migrazur/appliSD/traces/SQL_${site}.log.old
mv /produits/migrazur/appliSD/traces/SQL_${site}.log /produits/migrazur/appliSD/traces/SQL_${site}.log.old

# Time Zone
TZ=CET;export TZ;

export LANG=""

#export SYBASE=/produits/sybase15.7;cd /produits/migrazur/appliSD/traces;$SYBASE/ASE-15_0/install/startserver -f $SYBASE/ASE-15_0/install/RUN_SQL_$site
export SYBASE=/opt/sybase;cd /produits/migrazur/appliSD/traces;$SYBASE/ASE-15_0/install/startserver -f $SYBASE/ASE-15_0/install/RUN_SQL_$site

fich="/produits/migrazur/appliSD/traces/SQL_${site}.log"

# attendre la creation du fichier de log

while [[ ! -f $fich ]]
do
sleep 5
done

# attendre que le SQL_SERVER soit pret

while [[ `grep -c "online" $fich` = 0 ]]
do
sleep 5
done

exit 0
