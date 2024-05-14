#!/bin/ksh
# Fichier :  $Id: RS_sub_CFG.sh,v 1.8 2019/11/22 17:28:29 gesconf Exp $      Release : $Revision: 1.8 $        Date : $Date: 2019/11/22 17:28:29 $
##################################################################################
# STERIA # PROJET MIGRAZUR
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# SCRIPT RS_sub_CFG.sh
##################################################################################
# DESCRIPTION DU SCRIPT : creation des abonnements replication de CFG sans replication
# des donnees existantes en base
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# C.T.  09/03/98	: creation
# PNI	02/07/07	: ajout RES_CAN (DEM 657) 1.4
# JBL  	Jan 08 : DEM 663
# PNI	07/11/12	: ajout CCH_TRO
# PNI	27/11/18	: ajout CAL_VAC
##################################################################################

if (test $1 != "DM" && test $1 != "DN" && test $1 != "DC" && test $1 != "DS" && test $1 != "DA" && test $1 != "DP" && test $1 != "DY")
then
	echo "Syntaxe : RS_sub_CFG.sh NomSite (DN,DM,DS,DC,DA,DY,DP)"
	exit 0
fi

# nom du site
NomSite=$1
# nom du site primaire
NomSitePrim="CI"
# nom du fichier d'erreur
FicErr="sub_CFG.err"
rm $FicErr

tables/sub_create.rab CFG CAL_VAC $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG ANN_AST_CAL $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG ANN_AST_CAT $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG ANN_AST_CMP $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG ANN_CMP $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG ANN_FIC $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG ANN_GEO $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG CCH_TRO $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_ALM $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_ANA $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_CAM $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_CAM_POS $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_CPT $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_DAI $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_ECH $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_FAV $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_FAX $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_GEN $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_GTC $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_ISI $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_LTV $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_MAA $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_MAC $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_MAG $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_MNT $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_MOD $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_NAV $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_NIC $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_PAL $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_PAN $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_PAU $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_PCT $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_PMV $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_PNT_MES $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_RAD $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_RAD_VTL $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_RAU $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_LCR $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_TDP $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_ZDP $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_SCN_CNN $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_SCN_NAV $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_SEQ_ECH $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_SEQ_TUB $NomSitePrim $NomSite xx $FicErr
# GTC renovee
tables/sub_create.rab CFG EQT_SEQ_GTC_ECL $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_SEQ_GTC_ENR $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_SEQ_GTC_EXP $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_SEQ_GTC_INC $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_SEQ_GTC_SIG $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_SEQ_GTC_VEN $NomSitePrim $NomSite xx $FicErr

tables/sub_create.rab CFG EQT_SQA_GTC_ECL $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_SQA_GTC_ENR $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_SQA_GTC_EXP $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_SQA_GTC_INC $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_SQA_GTC_SIG $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_SQA_GTC_VEN $NomSitePrim $NomSite xx $FicErr
# Fin GTC renovee
tables/sub_create.rab CFG EQT_SQA_ECH $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_SQA_TUB $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_SYV $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_TOR $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_TUB $NomSitePrim $NomSite xx $FicErr
# GTC renovee
tables/sub_create.rab CFG EQT_LIT $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_LTN $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_SYS $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_EHT $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_GTC_ECL $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_GTC_ENR $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_GTC_EXP $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_GTC_INC $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_GTC_LTN $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_GTC_SIG $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_GTC_SYS $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG EQT_GTC_VEN $NomSitePrim $NomSite xx $FicErr
# Fin GTC renovee

tables/sub_create.rab CFG LIB_TYP $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG LIB_PRE $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG OPR_FCT_MAC $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG OPR_FCT_NOM $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG OPR_LOG_MDP $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG OPR_NOM $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG OPR_PRF $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG OPR_PRF_FCT $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG PLN_SCR $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG RES_AIR $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG RES_AUT $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG RES_AUT_INV $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG RES_CAN $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG RES_CHA $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG RES_DIS $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG RES_ECH $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG RES_INF $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG RES_ITP $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG RES_NEU $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG RES_NUM_ECH $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG RES_PEA $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG RES_PNT $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG RES_POL $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG RES_POR $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG RES_PTL $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG RES_SEG $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG RES_SUP $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG RES_TRC $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG RES_TRN $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG RES_VIL $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG RES_DTP $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG TYP_ALM $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG TYP_ALT $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG TYP_AST $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG TYP_CRB $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG TYP_FMC $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG TYP_NAV $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG TYP_PCT $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG TYP_PMV $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG ZON_PMV $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG ZON_PMA $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG ZON_BAF $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG ZON_BAD $NomSitePrim $NomSite xx $FicErr

#LOt 26 / listes FMC
tables/sub_create.rab CFG TYP_FMC_GRP $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG LST_FMC_COL $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG LST_FMC_BLC $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab CFG LST_FMC_MAC $NomSitePrim $NomSite xx $FicErr

