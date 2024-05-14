#!/bin/ksh
# Fichier :  $Id: RS_sub_EXP.sh,v 1.9 2018/04/17 06:34:06 devgfi Exp $      Release : $Revision: 1.9 $        Date : $Date: 2018/04/17 06:34:06 $
##################################################################################
# STERIA # PROJET MIGRAZUR
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# SCRIPT RS_sub_EXP.sh
##################################################################################
# DESCRIPTION DU SCRIPT : creation des abonnements replication de EXP
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# C.T.  09/03/98	: creation
# PN..  23/12/98	: Ajout de ACT_EXP_ADA _TFM_EXP _PMV_ADA + FMC_DAT v1.2
# F.C.  20/03/07	: Ajout de ACT_BAF, ACT_BAD, ACT_PMA, FMC_NAT v1.3
# JBL Jan 08 : DEM 663
# PNI	17/09/12	: ajout FMC_INC
# PNI	07/11/12	: ajout ACT_CCH FMC_MET_CCH
#PNI	12/03/13	: ajout PRG_PMV & PRG_PMA 
# PNI	02/03/17	; ajout de PMV_NTR
# PNI	17/04/17	; ajout PRG_PMV_SAGA & PRG_PMA_SAGA
##################################################################################

if (($# < 3))
then
	echo "Syntaxe : RS_sub_EXP.sh NomSite NomSitePrim TypeSitePrim (CI,DN,DM,DC,DP)"
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
FicErr="sub_EXP.err"
rm $FicErr

##################################################################################
# abonnements symÅtriques
##################################################################################

# fiches main courante

tables/sub_create.rab EXP FMC_GEN $NomSitePrim $NomSite xx $FicErr
 
tables/sub_create.rab EXP FMC_HIS $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create.rab EXP FMC_COM $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create.rab EXP FMC_OPR $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create.rab EXP FMC_TRF $NomSitePrim $NomSite xx $FicErr
 
tables/sub_create.rab EXP FMC_TRH $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create.rab EXP FMC_NAT $NomSitePrim $NomSite Condition $FicErr

tables/sub_create.rab EXP FMC_INC $NomSitePrim $NomSite Condition $FicErr
 
if [[ $TypeSitePrim = "NonSimplifie" ]]
then
tables/sub_create.rab EXP FMC_ACC $NomSitePrim $NomSite xx $FicErr
 
tables/sub_create.rab EXP FMC_BAS $NomSitePrim $NomSite xx $FicErr
 
tables/sub_create.rab EXP FMC_BOU $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create.rab EXP FMC_CNC $NomSitePrim $NomSite xx $FicErr
 
tables/sub_create.rab EXP FMC_DEL $NomSitePrim $NomSite xx $FicErr
 
tables/sub_create.rab EXP FMC_ECH $NomSitePrim $NomSite xx $FicErr
 
tables/sub_create.rab EXP FMC_MET $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create.rab EXP FMC_MET_CCH $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create.rab EXP FMC_MNF $NomSitePrim $NomSite xx $FicErr
 
tables/sub_create.rab EXP FMC_VEH $NomSitePrim $NomSite xx $FicErr
 
tables/sub_create.rab EXP FMC_VLN $NomSitePrim $NomSite Condition $FicErr

if [[ $NomSitePrim != "CI" ]]
then
tables/sub_create.rab EXP FRN_TDP $NomSitePrim $NomSite xx $FicErr
fi

if [[ $NomSitePrim = "CI" && $TypeSitePrim = "Simplifie" ]]
then
tables/sub_create.rab EXP FMC_PUR $NomSitePrim $NomSite xx $FicErr
fi

if [[ $NomSitePrim = "CI" && $TypeSitePrim = "NonSimplifie" ]]
then
tables/sub_create.rab EXP FMC_TRS $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab EXP FMC_DAT $NomSitePrim $NomSite Condition $FicErr
fi

fi
 
# actions

tables/sub_create.rab EXP ACT_GEN $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create.rab EXP ACT_CCH $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create.rab EXP ACT_NAV $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create.rab EXP ACT_PMV $NomSitePrim $NomSite Condition $FicErr

tables/sub_create.rab EXP ACT_PAL $NomSitePrim $NomSite Condition $FicErr

tables/sub_create.rab EXP ACT_BAF $NomSitePrim $NomSite Condition $FicErr

tables/sub_create.rab EXP ACT_BAD $NomSitePrim $NomSite Condition $FicErr

tables/sub_create.rab EXP ACT_PMA $NomSitePrim $NomSite Condition $FicErr

if [[ $TypeSitePrim = "NonSimplifie" ]]
then
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

# GTC renovee
tables/sub_create.rab EXP ACT_GTC_SIG $NomSitePrim $NomSite Condition $FicErr
tables/sub_create.rab EXP ACT_GTC_ECL $NomSitePrim $NomSite Condition $FicErr
tables/sub_create.rab EXP ACT_GTC_VEN $NomSitePrim $NomSite Condition $FicErr
tables/sub_create.rab EXP ACT_GTC_ENR $NomSitePrim $NomSite Condition $FicErr
tables/sub_create.rab EXP ACT_GTC_EXP $NomSitePrim $NomSite Condition $FicErr
tables/sub_create.rab EXP ACT_GTC_INC $NomSitePrim $NomSite Condition $FicErr
# Fin GTC renovee
 
tables/sub_create.rab EXP ACT_TXT_TFM $NomSitePrim $NomSite Condition $FicErr

 
if [[ $NomSitePrim = "CI" && $TypeSitePrim = "NonSimplifie" ]]
then
tables/sub_create.rab EXP ACT_EXP_ADA $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create.rab EXP ACT_TFM_EXP $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create.rab EXP ACT_PMV_ADA $NomSitePrim $NomSite Condition $FicErr
 fi
# autre

tables/sub_create.rab EXP FMC_TRT $NomSitePrim $NomSite xx $FicErr

tables/sub_create.rab EXP FMC_REV $NomSitePrim $NomSite xx $FicErr
fi
 
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

### de PC vers CI sans condition
tables/sub_create.rab EXP EQT_ACV $NomSitePrim $NomSite xx $FicErr
 
if [[ $TypeSitePrim = "NonSimplifie" ]]
then
### de PC vers CI sans condition
tables/sub_create.rab EXP FMC_TFM $NomSitePrim $NomSite xx $FicErr
 
### de PC vers CI sans condition
tables/sub_create.rab EXP EQT_FIL $NomSitePrim $NomSite xx $FicErr
fi
 
tables/sub_create.rab EXP ALM_EQT $NomSitePrim $NomSite Condition $FicErr
 
tables/sub_create.rab EXP ALM_NBR $NomSitePrim $NomSite Condition $FicErr
 
fi

tables/sub_create.rab EXP PRG_PMV $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab EXP PRG_PMA $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab EXP PMV_NTR $NomSitePrim $NomSite xx $FicErr

tables/sub_create.rab EXP PRG_PMV_SAGA $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab EXP PRG_PMA_SAGA $NomSitePrim $NomSite xx $FicErr
