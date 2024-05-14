#!/bin/ksh
#
#	-- Script d'arret de l'application MIGRAZUR
#		sur un SD ou SC ACTIF (SDCI1, SDDN1, SDDM1)
#
set -x

site="XX"

# tester le nom du site
if [[ `hostname` = "SDCI1" ]]
then
site="CI"
fi

if [[ `hostname` = "SDDN1" ]]
then
site="DN"
fi

if [[ `hostname` = "SDDM1" ]]
then
site="DM"
fi

if [[ `hostname` = "PODC1" ]]
then
site="DC"
fi

if [[ `hostname` = "PODS1" ]]
then
site="DS"
fi

if [[ $site = "XX" ]]
then
exit 1
fi

. /produits/migrazur/appliSD/.profile


echo "shutdown
go" | isql -Usa -Pzigramur -SREP_${site}


echo "shutdown
go" | isql -Usa -Pzigramur -S${site}_RSSD_LTM

echo "shutdown
go" | isql -Usa -Pzigramur -S${site}_EXP_LTM

if [[ $site = "CI" ]] || if [[ $site = "CA" ]]
then
echo "shutdown
go" | isql -Usa -Pzigramur -S${site}_CFG_LTM
fi


echo "shutdown
go" | isql -Usa -Pzigramur -SSQL_${site}

