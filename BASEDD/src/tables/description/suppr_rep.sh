#!/bin/ksh
# Fichier : @(#)sub_drop.rab	1.1        Release : 1.1        Date : 03/09/98
##################################################################################
# STERIA # PROJET MIGRAZUR
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# SCRIPT sub_create.rab
##################################################################################
# DESCRIPTION DU SCRIPT : creation d'un abonnement sans condition
#
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# P.V.	09/07/96	: Creation
# Hilmarcher 13/11/03   : Modification pour connexion au SiteReplique et non au site pour
#                         arret de la repli
##################################################################################

#echo "     suppr_sub.sh NomBase NomSub NomSql "

# nom de la base
NomBase=$1

# nom de la table 
NomSub=$2

# nom du site
NomSite=`echo $2 | awk -F _ '{ print $2 }'`

# nom du site replique
NomSiteReplique=`echo $2 | awk -F _ '{ print $1 }'`

RepDef=`echo $2 | awk -F _ '{ split($0,a,$2); print $2a[2] }'`

NomSql=$3

# nom du fichier d'erreur
NomFicErr=$6


###########################################################################
# suppression d'un abonnement pour une table 
###########################################################################
echo "drop replication definition ${RepDef} \n\
go\n" \

echo "| isql -S REP_${NomSite} -Usa -Pzigramur\n "

exit 0
