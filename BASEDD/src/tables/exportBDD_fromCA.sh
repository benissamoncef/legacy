#!/bin/ksh
#################################################################################
#
# DESCRIPTION DU SCRIPT :
# Exporte les donnes necessaire aux tables en BDD qui doivent etres remplies avant l'appel a tcgcd
# Les donnes sont exportees depuis CA
#
#################################################################################
#
#################################################################################
# HISTORIQUE DES MODIFICATIONS :
# GGY	26/04/24	: Creation
#################################################################################

bcp CFG..LIB_PRE out LIB_PRE.bcp -SSQL_CA -UMIGRAZUR -PMIGRAZUR
cp -p bcp.fmt LIB_PRE.fmt

bcp CFG..LIB_TYP out LIB_TYP.bcp -SSQL_CA -UMIGRAZUR -PMIGRAZUR
cp -p bcp.fmt LIB_TYP.fmt

bcp CFG..LST_FMC_BLC out LST_FMC_BLC.bcp -SSQL_CA -UMIGRAZUR -PMIGRAZUR
cp -p bcp.fmt LST_FMC_BLC.fmt

bcp CFG..LST_FMC_COL out LST_FMC_COL.bcp -SSQL_CA -UMIGRAZUR -PMIGRAZUR
cp -p bcp.fmt LST_FMC_COL.fmt

bcp CFG..OPR_FCT_NOM out OPR_FCT_NOM.bcp -SSQL_CA -UMIGRAZUR -PMIGRAZUR
cp -p bcp.fmt OPR_FCT_NOM.fmt

bcp CFG..OPR_IDE out OPR_IDE.bcp -SSQL_CA -UMIGRAZUR -PMIGRAZUR
cp -p bcp.fmt OPR_IDE.fmt

bcp CFG..OPR_LOG_MDP out OPR_LOG_MDP.bcp -SSQL_CA -UMIGRAZUR -PMIGRAZUR
cp -p bcp.fmt OPR_LOG_MDP.fmt

bcp CFG..OPR_NOM out OPR_NOM.bcp -SSQL_CA -UMIGRAZUR -PMIGRAZUR
cp -p bcp.fmt OPR_NOM.fmt

bcp CFG..OPR_PRF out OPR_PRF.bcp -SSQL_CA -UMIGRAZUR -PMIGRAZUR
cp -p bcp.fmt OPR_PRF.fmt

bcp CFG..OPR_PRF_FCT out OPR_PRF_FCT.bcp -SSQL_CA -UMIGRAZUR -PMIGRAZUR
cp -p bcp.fmt OPR_PRF_FCT.fmt

bcp CFG..TYP_CRB out TYP_CRB.bcp -SSQL_CA -UMIGRAZUR -PMIGRAZUR
cp -p bcp.fmt TYP_CRB.fmt

bcp CFG..TYP_FMC out TYP_FMC.bcp -SSQL_CA -UMIGRAZUR -PMIGRAZUR
cp -p bcp.fmt TYP_FMC.fmt

bcp CFG..TYP_FMC_GRP out TYP_FMC_GRP.bcp -SSQL_CA -UMIGRAZUR -PMIGRAZUR
cp -p bcp.fmt TYP_FMC_GRP.fmt

#################################################################################
# 
# Pour les tables suivantes on ne peut realiser l'export depuis CI car les donnes
# sont specifiques a chaque concessionnaire, il faut generer des scripts manuellement 
#
#################################################################################
#bcp CFG..OPR_FCT_MAC out OPR_FCT_MAC.bcp -SSQL_CA -UMIGRAZUR -PMIGRAZUR
#cp -p bcp.fmt OPR_FCT_MAC.fmt

#bcp CFG..EQT_LOC out EQT_LOC.bcp -SSQL_CA -UMIGRAZUR -PMIGRAZUR
#cp -p bcp.fmt EQT_LOC.fmt

#bcp CFG..LST_FMC_MAC out LST_FMC_MAC.bcp -SSQL_CA -UMIGRAZUR -PMIGRAZUR
#cp -p bcp.fmt LST_FMC_MAC.fmt

#bcp CFG..MGA_PAR_REF out MGA_PAR_REF.bcp -SSQL_CA -UMIGRAZUR -PMIGRAZUR
#cp -p bcp.fmt MGA_PAR_REF.fmt

#bcp CFG..RES_PRA out RES_PRA.bcp -SSQL_CA -UMIGRAZUR -PMIGRAZUR
#cp -p bcp.fmt RES_PRA.fmt

#bcp CFG..SYN_PSE out SYN_PSE.bcp -SSQL_CA -UMIGRAZUR -PMIGRAZUR
#cp -p bcp.fmt SYN_PSE.fmt

#bcp CFG..ZON_RAU out ZON_RAU.bcp -SSQL_CA -UMIGRAZUR -PMIGRAZUR
#cp -p bcp.fmt ZON_RAU.fmt

#bcp EXP..BAS_RAU out BAS_RAU.bcp -SSQL_CA -UMIGRAZUR -PMIGRAZUR
#cp -p bcp.fmt BAS_RAU.fmt
