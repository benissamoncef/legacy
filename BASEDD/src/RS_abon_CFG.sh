#!/bin/ksh
# Fichier : $Id: RS_abon_CFG.sh,v 1.14 2007/07/02 11:22:44 gesconf Exp $        Release : $Revision: 1.14 $        Date : $Date: 2007/07/02 11:22:44 $
##################################################################################
# STERIA # PROJET MIGRAZUR
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# SCRIPT RS_abon_CFG.sh
##################################################################################
# DESCRIPTION DU SCRIPT : creation des abonnements replication de CFG
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# B.G.  11/10/94	: creation
# C.T.  29/06/95	: ajout de RES_NUM_ECH
# C.T.  10/08/95	: ajout de EQT_ISI
# C.T.  06/11/95	: ajout de RES_AUT_INV
# C.T.  04/12/95	: ajout de EQT_FAX (V 1.5)
# P.V.  19/06/96	: ajout de LIB_TYP et LIB_PRE (V1.6)
# C.T.  05/09/96	: ajout de DS et DC (V1.7)
# D.M.  05/11/96	: ajout de EQT_LCR (DEM/1232) (V1.8)
# D.M.  07/02/97	: ajout de EQT_TDP, EQT_ZDP, RES_DTP (DEM/1395) (V1.9)
# JMG	08/01/98	: ajout de ZON_PMV (dem/1534) 1.10
# JMG	08/01/98	: ajout des PC niveau 2 (dem/1536) 1.11
# C.T.	16/01/98	: ajout de EQT_PAL 1.12
# PNI	02/07/07	: ajout de RES_CAN DEM657 1.14
##################################################################################

if (test $1 != "DM" && test $1 != "DN" && test $1 != "DC" && test $1 != "DS" && test $1 != "DA" && test $1 != "DP" && test $1 != "DY")
then
	echo "Syntaxe : RS_abon_CFG.sh NomSite (DN,DM,DS,DC,DA,DY,DP)"
	exit 0
fi

# nom du site
NomSite=$1
# nom du site primaire
NomSitePrim="CI"
# nom du fichier d'erreur
FicErr="abon_CFG.err"
rm $FicErr

tables/absscond.rab CFG ANN_AST_CAL $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG ANN_AST_CAT $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG ANN_AST_CMP $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG ANN_CMP $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG ANN_FIC $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG ANN_GEO $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG EQT_ALM $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG EQT_ANA $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG EQT_CAM $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG EQT_CAM_POS $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG EQT_CPT $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG EQT_DAI $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG EQT_ECH $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG EQT_FAV $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG EQT_FAX $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG EQT_GEN $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG EQT_GTC $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG EQT_ISI $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG EQT_LTV $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG EQT_MAA $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG EQT_MAC $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG EQT_MAG $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG EQT_MNT $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG EQT_MOD $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG EQT_NAV $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG EQT_NIC $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG EQT_PAL $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG EQT_PAN $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG EQT_PAU $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG EQT_PCT $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG EQT_PMV $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG EQT_PNT_MES $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG EQT_RAD $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG EQT_RAD_VTL $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG EQT_RAU $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG EQT_LCR $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG EQT_TDP $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG EQT_ZDP $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG EQT_SCN_CNN $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG EQT_SCN_NAV $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG EQT_SEQ_ECH $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG EQT_SEQ_TUB $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG EQT_SQA_ECH $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG EQT_SQA_TUB $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG EQT_SYV $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG EQT_TOR $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG EQT_TUB $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG LIB_TYP $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG LIB_PRE $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG OPR_FCT_MAC $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG OPR_FCT_NOM $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG OPR_LOG_MDP $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG OPR_NOM $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG OPR_PRF $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG OPR_PRF_FCT $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG PLN_SCR $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG RES_AIR $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG RES_AUT $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG RES_AUT_INV $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG RES_CAN $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG RES_CHA $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG RES_DIS $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG RES_ECH $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG RES_INF $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG RES_ITP $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG RES_NEU $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG RES_NUM_ECH $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG RES_PEA $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG RES_PNT $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG RES_POL $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG RES_POR $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG RES_PTL $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG RES_SEG $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG RES_SUP $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG RES_TRC $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG RES_TRN $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG RES_VIL $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG RES_DTP $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG TYP_ALM $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG TYP_ALT $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG TYP_AST $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG TYP_CRB $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG TYP_FMC $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG TYP_NAV $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG TYP_PCT $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG TYP_PMV $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG ZON_PMV $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG ZON_PMA $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG ZON_BAF $NomSitePrim $NomSite $FicErr
tables/absscond.rab CFG ZON_BAD $NomSitePrim $NomSite $FicErr
