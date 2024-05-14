#!/bin/sh
#################################################################################
#
# DESCRIPTION DU SCRIPT :
# Initialise les tables en BDD qui doivent etres remplies avant l'appel a tcgcd
#
#################################################################################
#
#################################################################################
# HISTORIQUE DES MODIFICATIONS :
# GGY	23/11/23	: Creation
#################################################################################

cp -p LIB_PRE.fmt bcp.fmt
bcp CFT..LIB_PRE in LIB_PRE.bcp -SSQL_CA -UMIGRAZUR -PMIGRAZUR
bcp CFG..LIB_PRE in LIB_PRE.bcp -SSQL_CA -UMIGRAZUR -PMIGRAZUR

cp -p LIB_TYP.fmt bcp.fmt
bcp CFT..LIB_TYP in LIB_TYP.bcp -SSQL_CA -UMIGRAZUR -PMIGRAZUR
bcp CFG..LIB_TYP in LIB_TYP.bcp -SSQL_CA -UMIGRAZUR -PMIGRAZUR

cp -p LST_FMC_BLC.fmt bcp.fmt
bcp CFT..LST_FMC_BLC in LST_FMC_BLC.bcp -SSQL_CA -UMIGRAZUR -PMIGRAZUR
bcp CFG..LST_FMC_BLC in LST_FMC_BLC.bcp -SSQL_CA -UMIGRAZUR -PMIGRAZUR

cp -p LST_FMC_COL.fmt bcp.fmt
bcp CFT..LST_FMC_COL in LST_FMC_COL.bcp -SSQL_CA -UMIGRAZUR -PMIGRAZUR
bcp CFG..LST_FMC_COL in LST_FMC_COL.bcp -SSQL_CA -UMIGRAZUR -PMIGRAZUR

cp -p OPR_FCT_NOM.fmt bcp.fmt
bcp CFT..OPR_FCT_NOM in OPR_FCT_NOM.bcp -SSQL_CA -UMIGRAZUR -PMIGRAZUR
bcp CFG..OPR_FCT_NOM in OPR_FCT_NOM.bcp -SSQL_CA -UMIGRAZUR -PMIGRAZUR

cp -p OPR_IDE.fmt bcp.fmt
bcp CFT..OPR_IDE in OPR_IDE.bcp -SSQL_CA -UMIGRAZUR -PMIGRAZUR
bcp CFG..OPR_IDE in OPR_IDE.bcp -SSQL_CA -UMIGRAZUR -PMIGRAZUR

cp -p OPR_LOG_MDP.fmt bcp.fmt
bcp CFT..OPR_LOG_MDP in OPR_LOG_MDP.bcp -SSQL_CA -UMIGRAZUR -PMIGRAZUR
bcp CFG..OPR_LOG_MDP in OPR_LOG_MDP.bcp -SSQL_CA -UMIGRAZUR -PMIGRAZUR

cp -p OPR_NOM.fmt bcp.fmt
bcp CFT..OPR_NOM in OPR_NOM.bcp -SSQL_CA -UMIGRAZUR -PMIGRAZUR
bcp CFG..OPR_NOM in OPR_NOM.bcp -SSQL_CA -UMIGRAZUR -PMIGRAZUR

cp -p OPR_PRF.fmt bcp.fmt
bcp CFT..OPR_PRF in OPR_PRF.bcp -SSQL_CA -UMIGRAZUR -PMIGRAZUR
bcp CFG..OPR_PRF in OPR_PRF.bcp -SSQL_CA -UMIGRAZUR -PMIGRAZUR

cp -p OPR_PRF_FCT.fmt bcp.fmt
bcp CFT..OPR_PRF_FCT in OPR_PRF_FCT.bcp -SSQL_CA -UMIGRAZUR -PMIGRAZUR
bcp CFG..OPR_PRF_FCT in OPR_PRF_FCT.bcp -SSQL_CA -UMIGRAZUR -PMIGRAZUR

cp -p TYP_CRB.fmt bcp.fmt
bcp CFT..TYP_CRB in TYP_CRB.bcp -SSQL_CA -UMIGRAZUR -PMIGRAZUR
bcp CFG..TYP_CRB in TYP_CRB.bcp -SSQL_CA -UMIGRAZUR -PMIGRAZUR

cp -p TYP_FMC.fmt bcp.fmt
bcp CFT..TYP_FMC in TYP_FMC.bcp -SSQL_CA -UMIGRAZUR -PMIGRAZUR
bcp CFG..TYP_FMC in TYP_FMC.bcp -SSQL_CA -UMIGRAZUR -PMIGRAZUR

cp -p TYP_FMC_GRP.fmt bcp.fmt
bcp CFT..TYP_FMC_GRP in TYP_FMC_GRP.bcp -SSQL_CA -UMIGRAZUR -PMIGRAZUR
bcp CFG..TYP_FMC_GRP in TYP_FMC_GRP.bcp -SSQL_CA -UMIGRAZUR -PMIGRAZUR

#################################################################################
# 
# Pour les tables suivantes on ne peut realiser l'import depuis CI car les donnes
# sont specifiques a chaque concessionnaire, il faut utiliser les scripts ecrit manuellement 
#
#################################################################################
./init_OPR_FCT_MAC_CA.sh

./init_EQT_LOC_CA.sh

./init_LST_FMC_MAC_CA.sh

./init_MGA_PAR_REF_CA.sh

./init_RES_PRA_CA.sh

./init_SYN_PSE_CA.sh

./init_ZON_RAU_CA.sh

./init_BAS_RAU_CA.sh
