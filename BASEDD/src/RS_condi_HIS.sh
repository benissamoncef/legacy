#!/bin/ksh
# Fichier : @(#)RS_abon_HIS.sh	1.8        Release : 1.8        Date : 02/26/99
##################################################################################
# STERIA # PROJET MIGRAZUR
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# SCRIPT RS_abon_HIS.sh
##################################################################################
# DESCRIPTION DU SCRIPT : creation des abonnements replication de HIS
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# JMG	31/08/09	creation
# GGY   15/09/2023  : Adaptation pour CNA (DEM-483)
##################################################################################

if (($# < 3))
then
	echo "Syntaxe : RS_condi_HIS.sh NomSite NomSitePrim Condition "
	exit 0
fi

# nom du site
NomSite=$1
# nom du site primaire
NomSitePrim=$2
# condition
Condition=$3
# nom du fichier d'erreur
FicErr="condi_HIS.err"
rm $FicErr

##################################################################################
# abonnements symetriques
##################################################################################

# fiches main courante

tables/sub_create_his.rab HIS FMC_HIS $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create_his.rab HIS FMC_COM $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create_his.rab HIS FMC_OPR $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create_his.rab HIS FMC_TRH $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create_his.rab HIS FMC_BOU $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create_his.rab HIS FMC_MET $NomSitePrim $NomSite Condition $FicErr
tables/sub_create_his.rab HIS FMC_MET_CCH $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create_his.rab HIS FMC_VLN $NomSitePrim $NomSite Condition $FicErr

tables/sub_create_his.rab HIS FMC_DAT $NomSitePrim $NomSite Condition $FicErr
tables/sub_create_his.rab HIS FMC_INC $NomSitePrim $NomSite Condition $FicErr

# actions

tables/sub_create_his.rab HIS ACT_GEN $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create_his.rab HIS ACT_NAV $NomSitePrim $NomSite Condition $FicErr
tables/sub_create_his.rab HIS ACT_BAF $NomSitePrim $NomSite Condition $FicErr
tables/sub_create_his.rab HIS ACT_BAD $NomSitePrim $NomSite Condition $FicErr
tables/sub_create_his.rab HIS ACT_PMA $NomSitePrim $NomSite Condition $FicErr
tables/sub_create_his.rab HIS ACT_CCH $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create_his.rab HIS ACT_PMV $NomSitePrim $NomSite Condition $FicErr

tables/sub_create_his.rab HIS ACT_PAL $NomSitePrim $NomSite Condition $FicErr

tables/sub_create_his.rab HIS ACT_APL $NomSitePrim $NomSite Condition $FicErr

tables/sub_create_his.rab HIS ACT_DEP $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create_his.rab HIS ACT_ECH $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create_his.rab HIS ACT_FAX $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create_his.rab HIS ACT_FRG $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create_his.rab HIS ACT_GAR $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create_his.rab HIS ACT_INT $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create_his.rab HIS ACT_RMQ $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create_his.rab HIS ACT_SIS $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create_his.rab HIS ACT_TFM $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create_his.rab HIS ACT_TUB $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create_his.rab HIS ACT_TXT_TFM $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create_his.rab HIS ACT_HIS_ADA $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create_his.rab HIS ACT_TFM_HIS $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create_his.rab HIS ACT_GTC_SIG $NomSitePrim $NomSite Condition $FicErr
tables/sub_create_his.rab HIS ACT_GTC_VEN $NomSitePrim $NomSite Condition $FicErr
tables/sub_create_his.rab HIS ACT_GTC_ECL $NomSitePrim $NomSite Condition $FicErr
tables/sub_create_his.rab HIS ACT_GTC_EXP $NomSitePrim $NomSite Condition $FicErr
tables/sub_create_his.rab HIS ACT_GTC_ENR $NomSitePrim $NomSite Condition $FicErr
tables/sub_create_his.rab HIS ACT_GTC_INC $NomSitePrim $NomSite Condition $FicErr
tables/sub_create_his.rab HIS ACT_PRV $NomSitePrim $NomSite Condition $FicErr

# autre

tables/sub_create_his.rab HIS EQT_DSP $NomSitePrim $NomSite Condition $FicErr

### condition particuliere
tables/sub_create_his.rab HIS ALT_HIS $NomSitePrim $NomSite Condition $FicErr

# mesures terrain

tables/sub_create_his.rab HIS MES_POI $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create_his.rab HIS MES_TRA $NomSitePrim $NomSite Condition $FicErr

tables/sub_create_his.rab HIS ZDP_6MN $NomSitePrim $NomSite Condition $FicErr

##################################################################################
# cas particulier pour CI ou CA
##################################################################################

if [[ $1 = "CI" ]] || [[ $1 = "CA" ]]
then

##################################################################################
# abonnement PC(primaire)->CI
##################################################################################

tables/sub_create_his.rab HIS ALM_EQT $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create_his.rab HIS ALM_NBR $NomSitePrim $NomSite Condition $FicErr
 
fi
