#!/bin/ksh
# Fichier : @(#)RS_abon_EXP.sh	1.8        Release : 1.8        Date : 02/26/99
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
# JMG	31/08/09	creation
##################################################################################

if (($# < 3))
then
	echo "Syntaxe : RS_condi_EXP.sh NomSite NomSitePrim Condition "
	exit 0
fi

# nom du site
NomSite=$1
# nom du site primaire
NomSitePrim=$2
# condition
Condition=$3
# nom du fichier d'erreur
FicErr="condi_EXP.err"
rm $FicErr

##################################################################################
# abonnements symÅtriques
##################################################################################

# fiches main courante

tables/sub_create.rab EXP FMC_HIS $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create.rab EXP FMC_COM $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create.rab EXP FMC_OPR $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create.rab EXP FMC_TRH $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create.rab EXP FMC_BOU $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create.rab EXP FMC_MET $NomSitePrim $NomSite Condition $FicErr
tables/sub_create.rab EXP FMC_MET_CCH $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create.rab EXP FMC_VLN $NomSitePrim $NomSite Condition $FicErr

tables/sub_create.rab EXP FMC_DAT $NomSitePrim $NomSite Condition $FicErr

tables/sub_create.rab EXP FMC_INC $NomSitePrim $NomSite Condition $FicErr

# actions

tables/sub_create.rab EXP ACT_GEN $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create.rab EXP ACT_NAV $NomSitePrim $NomSite Condition $FicErr
tables/sub_create.rab EXP ACT_BAF $NomSitePrim $NomSite Condition $FicErr
tables/sub_create.rab EXP ACT_BAD $NomSitePrim $NomSite Condition $FicErr
tables/sub_create.rab EXP ACT_PMA $NomSitePrim $NomSite Condition $FicErr
tables/sub_create.rab EXP ACT_CCH $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create.rab EXP ACT_PMV $NomSitePrim $NomSite Condition $FicErr

tables/sub_create.rab EXP ACT_PAL $NomSitePrim $NomSite Condition $FicErr

tables/sub_create.rab EXP ACT_APL $NomSitePrim $NomSite Condition $FicErr

tables/sub_create.rab EXP ACT_DEP $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create.rab EXP ACT_ECH $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create.rab EXP ACT_FAX $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create.rab EXP ACT_FRG $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create.rab EXP ACT_GAR $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create.rab EXP ACT_INT $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create.rab EXP ACT_RMQ $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create.rab EXP ACT_SIS $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create.rab EXP ACT_TFM $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create.rab EXP ACT_TUB $NomSitePrim $NomSite Condition $FicErr
tables/sub_create.rab EXP ACT_PRV $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create.rab EXP ACT_TXT_TFM $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create.rab EXP ACT_GTC_SIG $NomSitePrim $NomSite Condition $FicErr
tables/sub_create.rab EXP ACT_GTC_VEN $NomSitePrim $NomSite Condition $FicErr
tables/sub_create.rab EXP ACT_GTC_ECL $NomSitePrim $NomSite Condition $FicErr
tables/sub_create.rab EXP ACT_GTC_EXP $NomSitePrim $NomSite Condition $FicErr
tables/sub_create.rab EXP ACT_GTC_ENR $NomSitePrim $NomSite Condition $FicErr
tables/sub_create.rab EXP ACT_GTC_INC $NomSitePrim $NomSite Condition $FicErr

# autre

tables/sub_create.rab EXP EQT_DSP $NomSitePrim $NomSite Condition $FicErr

### condition particuliere
tables/sub_create.rab EXP ALT_EXP $NomSitePrim $NomSite Condition $FicErr

# mesures terrain

tables/sub_create.rab EXP MES_NIV $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create.rab EXP MES_POI $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create.rab EXP MES_TRA $NomSitePrim $NomSite Condition $FicErr

tables/sub_create.rab EXP ZDP_6MN $NomSitePrim $NomSite Condition $FicErr

##################################################################################
# cas particulier pour CI
##################################################################################

if [[ $1 = "CI" ]]
then

##################################################################################
# abonnement PC(primaire)->CI
##################################################################################

tables/sub_create.rab EXP ALM_EQT $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create.rab EXP ALM_NBR $NomSitePrim $NomSite Condition $FicErr
 
fi
