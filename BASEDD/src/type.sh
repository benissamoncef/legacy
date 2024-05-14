#!/bin/sh
#################################################################################
# PROJET MIGRAZUR
#################################################################################
#
# SOUS-SYSTEME BASEDD
#################################################################################
#
# DESCRIPTION DU SCRIPT :
#  Compile les procedures stockées de XZAO sur le serveur d'un site donné.
#
#############################################################################
#
# Service rendu
#################################################################################
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# ABK   18/08/2023  : Creation 
# GGY   10/10/2023  : Adaptation usage (DEM-483)
##################################################################################

if (($#!=2))
then
	echo "Syntaxe : type.sh NomBase Site"
	echo "          NomBase en minuscule"
	echo "          Site : CI DN DM CA HIS"
	echo "Exemple : type.sh prc HIS"
	exit 0
fi

if [ -e ~/migrazur/BASEDD/src/$1.use ]
then
	cat ~/migrazur/BASEDD/src/$1.use  ~/migrazur/BASEDD/src/cretyp.bas | isql -SSQL_$2 -UMIGRAZUR -PMIGRAZUR
	exit 1
else
	echo "Erreur : Fichier ~/migrazur/BASEDD/src/$1.use n'existe pas."
	exit 0
fi

