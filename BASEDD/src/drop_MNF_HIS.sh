#!/bin/ksh
# Fichier : @(#)RS_sub_HIS.sh	1.1        Release : 1.1        Date : 03/09/98
##################################################################################
# STERIA # PROJET MIGRAZUR
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# SCRIPT RS_sub_HIS.sh
##################################################################################
# DESCRIPTION DU SCRIPT : drop des abonnements replication de HIS sans repliquer
# les donnees existantes
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# C.T.  09/03/98	: creation
##################################################################################


# nom du site
NomSite="HIS"
# nom du site primaire
NomSitePrim=CI
# nom du fichier d'erreur
FicErr="drop_HISHIS.err"
rm xx $FicErr

##################################################################################
# abonnements sym≈triques
##################################################################################

# fiches main courante

tables/sub_drop.rab HIS FMC_MNF $NomSitePrim $NomSite xx $FicErr

