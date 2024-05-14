#!/bin/ksh
# Fichier : $Id: RS_abon_HIS.sh,v 1.10 2007/04/11 09:59:00 gesconf Exp $        Release : $Revision: 1.10 $        Date : $Date: 2007/04/11 09:59:00 $
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
# B.G.  15/11/94	: creation
# C.T.  03/05/96	: suppression de la souscription sur MES_NIV (1.2)
# C.T.  30/08/96	: ajout le cas de creation d'abonnements a partir d'un PC simplifie (1.3)
# P.N.  04/10/96	: ajout de MES_KCS MES_NEC MES_QPC (eqt10) (1.4)
# JMG	08/01/98	: ajout de ALT_EXP sur les PC simplifies (dem/1532) 1.6
# C.T.  16/01/98	: Ajout de ACT_PAL (v 1.7)
# PN	30/09/98	: separation EXP/HIS ajout ADA_FMC 1.8
# PN..  23/12/98	: Ajout de ACT_EXP_ADA _TFM_EXP _PMV_ADA + FMC_DAT v1.9
# ESCOTA 03/05/99	: Ajout de ZDP_6MN sur HIS v1.10
# GGY   15/09/2023  : Adaptation pour CNA (DEM-483)
##################################################################################

if (($# < 1))
then
	echo "Syntaxe : RS_abon_HIS.sh NomSitePrim TypeSitePrim"
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
rm $FicErr

##################################################################################
# abonnements sym�triques
##################################################################################

# fiches main courante

tables/absscond.rab HIS FMC_GEN $NomSitePrim $NomSite $FicErr
 
tables/abcond.rab HIS FMC_HIS $NomSitePrim $NomSite $FicErr
 
tables/abcond.rab HIS FMC_COM $NomSitePrim $NomSite $FicErr
 
tables/abcond.rab HIS FMC_OPR $NomSitePrim $NomSite $FicErr
 
tables/absscond.rab HIS FMC_TRF $NomSitePrim $NomSite $FicErr
 
tables/abcond.rab HIS FMC_TRH $NomSitePrim $NomSite $FicErr
 
if [[ $TypeSitePrim = "NonSimplifie" ]]
then
tables/absscond.rab HIS FMC_ACC $NomSitePrim $NomSite $FicErr
 
tables/absscond.rab HIS FMC_BAS $NomSitePrim $NomSite $FicErr
 
tables/abcond.rab HIS FMC_BOU $NomSitePrim $NomSite $FicErr
 
tables/absscond.rab HIS FMC_CNC $NomSitePrim $NomSite $FicErr
 
tables/absscond.rab HIS FMC_DEL $NomSitePrim $NomSite $FicErr
 
tables/absscond.rab HIS FMC_ECH $NomSitePrim $NomSite $FicErr
 
tables/abcond.rab HIS FMC_MET $NomSitePrim $NomSite $FicErr
 
tables/absscond.rab HIS FMC_MNF $NomSitePrim $NomSite $FicErr
 
tables/absscond.rab HIS FMC_VEH $NomSitePrim $NomSite $FicErr
 
tables/abcond.rab HIS FMC_VLN $NomSitePrim $NomSite $FicErr

tables/abcond.rab HIS FMC_DAT $NomSitePrim $NomSite $FicErr

tables/abcond.rab HIS ZDP_6MN $NomSitePrim $NomSite $FicErr

fi
 
# actions

tables/abcond.rab HIS ACT_GEN $NomSitePrim $NomSite $FicErr
 
tables/abcond.rab HIS ACT_NAV $NomSitePrim $NomSite $FicErr
 
tables/abcond.rab HIS ACT_PMV $NomSitePrim $NomSite $FicErr

tables/abcond.rab HIS ACT_PAL $NomSitePrim $NomSite $FicErr


if [[ $TypeSitePrim = "NonSimplifie" ]]
then
tables/abcond.rab HIS ACT_APL $NomSitePrim $NomSite $FicErr

tables/abcond.rab HIS ACT_DEP $NomSitePrim $NomSite $FicErr
 
tables/abcond.rab HIS ACT_ECH $NomSitePrim $NomSite $FicErr
 
tables/abcond.rab HIS ACT_FAX $NomSitePrim $NomSite $FicErr
 
tables/abcond.rab HIS ACT_FRG $NomSitePrim $NomSite $FicErr
 
tables/abcond.rab HIS ACT_GAR $NomSitePrim $NomSite $FicErr
 
tables/abcond.rab HIS ACT_INT $NomSitePrim $NomSite $FicErr
 
tables/abcond.rab HIS ACT_RMQ $NomSitePrim $NomSite $FicErr
 
tables/abcond.rab HIS ACT_SIS $NomSitePrim $NomSite $FicErr
 
tables/abcond.rab HIS ACT_TFM $NomSitePrim $NomSite $FicErr
 
tables/abcond.rab HIS ACT_TUB $NomSitePrim $NomSite $FicErr
 
tables/abcond.rab HIS ACT_TXT_TFM $NomSitePrim $NomSite $FicErr
 
tables/abcond.rab HIS ACT_EXP_ADA $NomSitePrim $NomSite $FicErr
 
tables/abcond.rab HIS ACT_TFM_EXP $NomSitePrim $NomSite $FicErr
 
tables/abcond.rab HIS ACT_PMV_ADA $NomSitePrim $NomSite $FicErr

fi
 
# autre

### condition particuliere
tables/alt_exp.rab HIS ALT_EXP $NomSitePrim $NomSite $FicErr
 
tables/abcond.rab HIS EQT_DSP $NomSitePrim $NomSite $FicErr
 
# mesures terrain

tables/abcond.rab HIS MES_POI $NomSitePrim $NomSite $FicErr
 
tables/abcond.rab HIS MES_TRA $NomSitePrim $NomSite $FicErr

tables/abcond.rab HIS MES_KCS $NomSitePrim $NomSite $FicErr

tables/abcond.rab HIS MES_NEC $NomSitePrim $NomSite $FicErr

tables/abcond.rab HIS MES_QPC $NomSitePrim $NomSite $FicErr

if [[ $NomSitePrim = "CI" ]] || [[ $NomSitePrim = "CA" ]]
then
tables/absscond.rab HIS ADA_FMC $NomSitePrim $NomSite $FicErr
fi
