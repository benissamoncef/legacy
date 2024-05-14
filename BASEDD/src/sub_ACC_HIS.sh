#!/bin/ksh
# Fichier : %W%        Release : %I%        Date : %G%
##################################################################################
# STERIA # PROJET MIGRAZUR
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# SCRIPT rep.bas
##################################################################################
# DESCRIPTION DU SCRIPT :
# creation des definitions de la replication des tables pour un site
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# C.T.	09/08/94	: Creation
# B.G.  29/09/94        : Ajout des tables manquantes (v1.2)
# C.T.  21/03/95        : Modif chemin d'acces aux fichiers de definition (v 1.5)
# C.T.  30/08/96        : Ajout le cas de PC simplifie (v 1.6)
#                         et fmc_pur pour la purge RADT
##################################################################################

if ((($#<1)) || (test $1 != "CI" && test $1 != "DN" && test $1 != "DM" && test $1 != "DC" && test $1 != "DS" ))
then echo "syntaxe : sub_his.sh NomSitePrimaire NomSite TypeSitePrimaire NomfichierErreur"
     echo "          NomSitePrimaire  : CI ou DN ou DM ou $NomSitePrimaire"
     echo "          TypeSitePrimaire  : Simplifie ou NonSimplifie"
     echo "          NomfichierErreur : fichier d'erreur optionnel"
     echo "                             par defaut rep.err"
     exit 0
fi

#nom du site primaire
NomSite=$2
NomSitePrimaire=$1

#type du site primaire
TypeSite=$3

#nom du fichier d'erreur
if (($#==4))
then NomFicErr="rep.err"
else NomFicErr=$4.err
fi

#creation des definitions des replications de tables de EXP
tables/sub_create.rab HIS FMC_ACC $NomSitePrimaire $NomSite xx rab
