#!/bin/ksh

# Extrait les table CFG..PLN_SCR et RES_ECH
# les copie sur bande avec les fichier plan_*.scr 


# extraction de PLN_SCR
bcp CFG..PLN_SCR out /produits/migrazur/appliSD/modif/fichiers/ign/iti.lis -UMIGRAZUR -PMIGRAZUR -SSQL_CI -c
# extraction de RES_ECH
bcp CFG..RES_ECH out /tmp/ech.lis -UMIGRAZUR -PMIGRAZUR -SSQL_CI -c

# On ne garde que les 2 premieres colonnes
cat /tmp/ech.lis | awk -F \t '{print $1 "\t" $2}' > /produits/migrazur/appliSD/modif/fichiers/ign/ech.lis

cd /produits/migrazur/appliSD/modif/fichiers/ign

tar cv iti.lis ech.lis plan_*.scr

echo 
echo "RETURN pour Quitter"

read car
