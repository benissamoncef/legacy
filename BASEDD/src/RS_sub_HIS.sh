#!/bin/ksh
# Fichier :  $Id: RS_sub_HIS.sh,v 1.13 2018/09/04 13:47:25 devgfi Exp $      Release : $Revision: 1.13 $        Date : $Date: 2018/09/04 13:47:25 $
##################################################################################
# STERIA # PROJET MIGRAZUR
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# SCRIPT RS_sub_HIS.sh
##################################################################################
# DESCRIPTION DU SCRIPT : creation des abonnements replication de HIS sans repliquer
# les donnees existantes
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# C.T.  09/03/98	: creation
# P.N	30/09/98	: separation EXP/HIS ajout de ADA_FMC
# PN..  23/12/98	: Ajout de ACT_EXP_ADA _TFM_EXP _PMV_ADA + FMC_DAT v1.4
# ESCOTA 03/05/99	: Ajout de ZDP_6MN
# F.C.  20/03/07	: Ajout de ACT_BAF, ACT_BAD, ACT_PMA, FMC_NAT
# JBL 	Jan 08 		: DEM 663
# PNI	17/09/12	: ajout FMC_INC
# PNI	07/11/12	: ajout ACT_CCH FMC_MET_CCH
# PNI	02/03/17	; ajout de PMV_NTR
# JMG	24/01/18	: ajout aCT_SAGA
# PNI	04/09/18	: ajout FMC_TRA
# GGY   15/09/2023  : Adaptation pour CNA (DEM-483)
##################################################################################

if (($# < 1))
then
	echo "Syntaxe : RS_sub_HIS.sh NomSitePrim TypeSitePrim"
	echo "          NomSitePrim : (CI,DN,DM, ...)"
	echo "          TypeSitePrim : Simplifie / NonSimplifie (CI,DN,DM, ...)"
	exit 0
fi

# nom du site
NomSite="HIS"
# nom du site primaire
NomSitePrim=$1
# type du site primaire
TypeSitePrim=$2
# nom du fichier d'erreur
FicErr="abon_HIS.err"
rm xx $FicErr

##################################################################################
# abonnements symetriques
##################################################################################

# fiches main courante

tables/sub_create.rab HIS FMC_GEN $NomSitePrim $NomSite xx $FicErr
 
tables/sub_create.rab HIS FMC_HIS $NomSitePrim $NomSite xx $FicErr
 
tables/sub_create.rab HIS FMC_COM $NomSitePrim $NomSite xx $FicErr
 
tables/sub_create.rab HIS FMC_OPR $NomSitePrim $NomSite xx $FicErr
 
tables/sub_create.rab HIS FMC_TRF $NomSitePrim $NomSite xx $FicErr
 
tables/sub_create.rab HIS FMC_TRH $NomSitePrim $NomSite xx $FicErr
 
tables/sub_create.rab HIS FMC_NAT $NomSitePrim $NomSite xx $FicErr
 
tables/sub_create.rab HIS FMC_INC $NomSitePrim $NomSite xx $FicErr
 
if [[ $TypeSitePrim = "NonSimplifie" ]]
then
tables/sub_create.rab HIS FMC_ACC $NomSitePrim $NomSite xx $FicErr
 
tables/sub_create.rab HIS FMC_BAS $NomSitePrim $NomSite xx $FicErr
 
tables/sub_create.rab HIS FMC_BOU $NomSitePrim $NomSite xx $FicErr
 
tables/sub_create.rab HIS FMC_CNC $NomSitePrim $NomSite xx $FicErr
 
tables/sub_create.rab HIS FMC_DEL $NomSitePrim $NomSite xx $FicErr
 
tables/sub_create.rab HIS FMC_ECH $NomSitePrim $NomSite xx $FicErr
 
tables/sub_create.rab HIS FMC_MET $NomSitePrim $NomSite xx $FicErr
 
tables/sub_create.rab HIS FMC_MET_CCH $NomSitePrim $NomSite xx $FicErr
 
tables/sub_create.rab HIS FMC_MNF $NomSitePrim $NomSite xx $FicErr
 
tables/sub_create.rab HIS FMC_VEH $NomSitePrim $NomSite xx $FicErr
 
tables/sub_create.rab HIS FMC_VLN $NomSitePrim $NomSite xx $FicErr

tables/sub_create.rab HIS FMC_DAT $NomSitePrim $NomSite xx $FicErr

fi
 
# actions

tables/sub_create.rab HIS ACT_GEN $NomSitePrim $NomSite xx $FicErr
 
tables/sub_create.rab HIS ACT_NAV $NomSitePrim $NomSite xx $FicErr
 
tables/sub_create.rab HIS ACT_CCH $NomSitePrim $NomSite xx $FicErr
 
tables/sub_create.rab HIS ACT_PMV $NomSitePrim $NomSite xx $FicErr

tables/sub_create.rab HIS ACT_PAL $NomSitePrim $NomSite xx $FicErr

tables/sub_create.rab HIS ACT_BAF $NomSitePrim $NomSite xx $FicErr

tables/sub_create.rab HIS ACT_BAD $NomSitePrim $NomSite xx $FicErr

tables/sub_create.rab HIS ACT_PMA $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab HIS ACT_SAGA $NomSitePrim $NomSite xx $FicErr


if [[ $TypeSitePrim = "NonSimplifie" ]]
then
tables/sub_create.rab HIS ACT_APL $NomSitePrim $NomSite xx $FicErr

tables/sub_create.rab HIS ACT_DEP $NomSitePrim $NomSite xx $FicErr
 
tables/sub_create.rab HIS ACT_ECH $NomSitePrim $NomSite xx $FicErr
 
tables/sub_create.rab HIS ACT_FAX $NomSitePrim $NomSite xx $FicErr
 
tables/sub_create.rab HIS ACT_FRG $NomSitePrim $NomSite xx $FicErr
 
tables/sub_create.rab HIS ACT_GAR $NomSitePrim $NomSite xx $FicErr
 
tables/sub_create.rab HIS ACT_INT $NomSitePrim $NomSite xx $FicErr
 
tables/sub_create.rab HIS ACT_RMQ $NomSitePrim $NomSite xx $FicErr
 
tables/sub_create.rab HIS ACT_SIS $NomSitePrim $NomSite xx $FicErr
 
tables/sub_create.rab HIS ACT_TFM $NomSitePrim $NomSite xx $FicErr
 
tables/sub_create.rab HIS ACT_TUB $NomSitePrim $NomSite xx $FicErr

# GTC renovee
tables/sub_create.rab HIS ACT_GTC_SIG $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab HIS ACT_GTC_ECL $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab HIS ACT_GTC_VEN $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab HIS ACT_GTC_EXP $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab HIS ACT_GTC_ENR $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab HIS ACT_GTC_INC $NomSitePrim $NomSite xx $FicErr
# Fin GTC renovee
 
tables/sub_create.rab HIS ACT_TXT_TFM $NomSitePrim $NomSite xx $FicErr
 
tables/sub_create.rab HIS ACT_EXP_ADA $NomSitePrim $NomSite xx $FicErr
 
tables/sub_create.rab HIS ACT_TFM_EXP $NomSitePrim $NomSite xx $FicErr
 
tables/sub_create.rab HIS ACT_PMV_ADA $NomSitePrim $NomSite xx $FicErr
fi
 
# autre

### condition particuliere
#tables/alt_exp.rab HIS ALT_EXP $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab HIS ALT_EXP $NomSitePrim $NomSite xx $FicErr

tables/sub_create.rab HIS EQT_DSP $NomSitePrim $NomSite xx $FicErr
 
# mesures terrain

tables/sub_create.rab HIS MES_POI $NomSitePrim $NomSite xx $FicErr
 
tables/sub_create.rab HIS MES_TRA $NomSitePrim $NomSite xx $FicErr

tables/sub_create.rab HIS MES_KCS $NomSitePrim $NomSite xx $FicErr

tables/sub_create.rab HIS MES_NEC $NomSitePrim $NomSite xx $FicErr

tables/sub_create.rab HIS MES_QPC $NomSitePrim $NomSite xx $FicErr

tables/sub_create.rab HIS ZDP_6MN $NomSitePrim $NomSite xx $FicErr

if [[ $NomSitePrim = "CI" ]] || [[ $NomSitePrim = "CA" ]]
then
tables/sub_create.rab HIS ADA_FMC $NomSitePrim $NomSite xx $FicErr
fi

tables/sub_create.rab HIS PMV_NTR $NomSitePrim $NomSite xx $FicErr
tables/sub_create.rab HIS FMC_TRA $NomSitePrim $NomSite xx $FicErr
