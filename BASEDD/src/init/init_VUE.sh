#!/bin/ksh
# Fichier : 
##################################################################################
# STERIA # PROJET MIGRAZUR
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# SCRIPT init_VUE.sh
##################################################################################
# DESCRIPTION DU SCRIPT :
# renseigne les tables de VUE a partir des constantes
# d≈finies dans XDC (» faire au CI)
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# L.V.	22/08/96	: Creation
##################################################################################

# initialisation des differents types de libelles
isql -UMIGRAZUR -PMIGRAZUR -SSQL_CI <dico.bas

exit 0
