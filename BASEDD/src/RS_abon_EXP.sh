#!/bin/ksh
# Fichier : $Id: RS_abon_EXP.sh,v 1.8 1999/02/26 14:30:03 gaborit Exp $        Release : $Revision: 1.8 $        Date : $Date: 1999/02/26 14:30:03 $
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
# B.G.  05/10/94	: creation
# C.T.  30/08/96	: Ajout cas d'un pc simplifie en site primaire (v 1.2)
#                         et ajout de la replication sur fmc_pur (RADT)
# P.N.  21/02/97	: Ajout ZDP_6MN et FRN_TDP (v 1.2)
# C.T.  18/12/97	: Ajout de FMC_TRS (1531) (v 1.4)
# JMG	08/01/98	: gestion des PC niveau 2 (dem/1532) 1.5
# C.T.  16/01/98	: Ajout de ACT_PAL (v 1.6)
# PN..  23/12/98	: Ajout de ACT_EXP_ADA _TFM_EXP _PMV_ADA +FMC_DAT v1.7
##################################################################################

if (($# < 4))
then
	echo "Syntaxe : RS_abon_EXP.sh NomSite NomSitePrim TypeSitePrim (CI,DN,DM,DC)"
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
FicErr="abon_EXP.err"
rm $FicErr

##################################################################################
# abonnements symÅtriques
##################################################################################

# fiches main courante

tables/absscond.rab EXP FMC_GEN $NomSitePrim $NomSite $FicErr
 
tables/abcond.rab EXP FMC_HIS $NomSitePrim $NomSite $FicErr
 
tables/abcond.rab EXP FMC_COM $NomSitePrim $NomSite $FicErr
 
tables/abcond.rab EXP FMC_OPR $NomSitePrim $NomSite $FicErr
 
tables/absscond.rab EXP FMC_TRF $NomSitePrim $NomSite $FicErr
 
tables/abcond.rab EXP FMC_TRH $NomSitePrim $NomSite $FicErr
 
if [[ $TypeSitePrim = "NonSimplifie" ]]
then
tables/absscond.rab EXP FMC_ACC $NomSitePrim $NomSite $FicErr
 
tables/absscond.rab EXP FMC_BAS $NomSitePrim $NomSite $FicErr
 
tables/abcond.rab EXP FMC_BOU $NomSitePrim $NomSite $FicErr
 
tables/absscond.rab EXP FMC_CNC $NomSitePrim $NomSite $FicErr
 
tables/absscond.rab EXP FMC_DEL $NomSitePrim $NomSite $FicErr
 
tables/absscond.rab EXP FMC_ECH $NomSitePrim $NomSite $FicErr
 
tables/abcond.rab EXP FMC_MET $NomSitePrim $NomSite $FicErr
 
tables/absscond.rab EXP FMC_MNF $NomSitePrim $NomSite $FicErr
 
tables/absscond.rab EXP FMC_VEH $NomSitePrim $NomSite $FicErr
 
tables/abcond.rab EXP FMC_VLN $NomSitePrim $NomSite $FicErr

tables/abcond.rab EXP FMC_DAT $NomSitePrim $NomSite $FicErr

tables/absscond.rab EXP FRN_TDP $NomSitePrim $NomSite $FicErr

if [[ $NomSitePrim = "CI" && $TypeSitePrim = "Simplifie" ]]
then
tables/absscond.rab EXP FMC_PUR $NomSitePrim $NomSite $FicErr
fi

if [[ $NomSitePrim = "CI" && $TypeSitePrim = "NonSimplifie" ]]
then
tables/absscond.rab EXP FMC_TRS $NomSitePrim $NomSite $FicErr
fi

fi
 
# actions

tables/abcond.rab EXP ACT_GEN $NomSitePrim $NomSite $FicErr
 
tables/abcond.rab EXP ACT_NAV $NomSitePrim $NomSite $FicErr
 
tables/abcond.rab EXP ACT_PMV $NomSitePrim $NomSite $FicErr

tables/abcond.rab EXP ACT_PAL $NomSitePrim $NomSite $FicErr

if [[ $TypeSitePrim = "NonSimplifie" ]]
then
tables/abcond.rab EXP ACT_APL $NomSitePrim $NomSite $FicErr

tables/abcond.rab EXP ACT_DEP $NomSitePrim $NomSite $FicErr
 
tables/abcond.rab EXP ACT_ECH $NomSitePrim $NomSite $FicErr
 
tables/abcond.rab EXP ACT_FAX $NomSitePrim $NomSite $FicErr
 
tables/abcond.rab EXP ACT_FRG $NomSitePrim $NomSite $FicErr
 
tables/abcond.rab EXP ACT_GAR $NomSitePrim $NomSite $FicErr
 
tables/abcond.rab EXP ACT_INT $NomSitePrim $NomSite $FicErr
 
tables/abcond.rab EXP ACT_RMQ $NomSitePrim $NomSite $FicErr
 
tables/abcond.rab EXP ACT_SIS $NomSitePrim $NomSite $FicErr
 
tables/abcond.rab EXP ACT_TFM $NomSitePrim $NomSite $FicErr
 
tables/abcond.rab EXP ACT_TUB $NomSitePrim $NomSite $FicErr
 
tables/abcond.rab EXP ACT_TXT_TFM $NomSitePrim $NomSite $FicErr
 
tables/abcond.rab EXP ACT_EXP_ADA $NomSitePrim $NomSite $FicErr
 
tables/abcond.rab EXP ACT_TFM_EXP $NomSitePrim $NomSite $FicErr
 
tables/abcond.rab EXP ACT_PMV_ADA $NomSitePrim $NomSite $FicErr
# autre

tables/absscond.rab EXP FMC_TRT $NomSitePrim $NomSite $FicErr

tables/absscond.rab EXP FMC_REV $NomSitePrim $NomSite $FicErr
fi
 
tables/abcond.rab EXP EQT_DSP $NomSitePrim $NomSite $FicErr

### condition particuliere
tables/alt_exp.rab EXP ALT_EXP $NomSitePrim $NomSite $FicErr

# mesures terrain

tables/abcond.rab EXP MES_NIV $NomSitePrim $NomSite $FicErr
 
tables/abcond.rab EXP MES_POI $NomSitePrim $NomSite $FicErr
 
tables/abcond.rab EXP MES_TRA $NomSitePrim $NomSite $FicErr

tables/abcond.rab EXP ZDP_6MN $NomSitePrim $NomSite $FicErr

##################################################################################
# cas particulier pour CI
##################################################################################

if [[ $1 = "CI" ]]
then

##################################################################################
# abonnement PC(primaire)->CI
##################################################################################

### de PC vers CI sans condition
tables/absscond.rab EXP EQT_ACV $NomSitePrim $NomSite $FicErr
 
if [[ $TypeSitePrim = "NonSimplifie" ]]
then
### de PC vers CI sans condition
tables/absscond.rab EXP FMC_TFM $NomSitePrim $NomSite $FicErr
 
### de PC vers CI sans condition
tables/absscond.rab EXP EQT_FIL $NomSitePrim $NomSite $FicErr
fi
 
tables/abcond.rab EXP ALM_EQT $NomSitePrim $NomSite $FicErr
 
tables/abcond.rab EXP ALM_NBR $NomSitePrim $NomSite $FicErr
 
fi
