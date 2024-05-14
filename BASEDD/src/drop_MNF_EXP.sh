#!/bin/ksh
# Fichier : @(#)RS_abon_EXP.sh	1.3        Release : 1.3        Date : 05/14/97
##################################################################################
# STERIA # PROJET MIGRAZUR
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# SCRIPT RS_abon_EXP.sh
##################################################################################
# DESCRIPTION DU SCRIPT : creation des abonnements replication de EXP
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# B.G.  05/10/94	: creation
# C.T.  30/08/96	: Ajout cas d'un pc simplifie en site primaire (v 1.2)
#                         et ajout de la replication sur fmc_pur (RADT)
# P.N.  21/02/97	: Ajout ZDP_6MN et FRN_TDP (v 1.2)
##################################################################################

if (($# < 4))
then
	echo "Syntaxe : RS_abon_EXP.sh NomSite NomSitePrim TypeSitePrim (CI,DN,DM,DC)"
	echo "TypeSitePrim : Simplifie ou NonSimplifie"
	exit 0
fi

# nom du site
NomSite=$1
# nom du site primaire
NomSitePrim=$2
# type du site primaire
TypeSitePrim=$3
# nom du fichier d'erreur
FicErr="abon_EXP.err"
rm $FicErr

##################################################################################
# abonnements symÅtriques
##################################################################################

# fiches main courante

tables/sub_drop.rab EXP FMC_MNF $NomSitePrim $NomSite xxx $FicErr
 
