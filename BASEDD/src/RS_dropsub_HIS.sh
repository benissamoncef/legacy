#!/bin/ksh
# Fichier : $Id: RS_dropsub_HIS.sh,v 1.1 1998/09/21 09:30:52 gesconf Exp $        Release : $Revision: 1.1 $        Date : $Date: 1998/09/21 09:30:52 $
##################################################################################
# STERIA # PROJET MIGRAZUR
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# SCRIPT RS_dropsub_HIS.sh
##################################################################################
# DESCRIPTION DU SCRIPT : creation des abonnements replication de HIS sans repliquer
# les donnees existantes
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# C.T.  09/03/98        : creation
##################################################################################

if (($# < 1))
then
        echo "Syntaxe : RS_dropsub_HIS.sh NomSitePrim TypeSitePrim"
        echo "          NomSitePrim : (CI,DN,DM, ...)"
        echo "          TypeSitePrim : Simplifie / NonSimplifie (CI,DN,DM, ...)"
        exit 0
fi

# nom du site
NomSite="CI"
# nom du site primaire
NomSitePrim=$1
# type du site primaire
TypeSitePrim=$2
# nom du fichier d'erreur
FicErr="abon_HIS.err"
rm xxx $FicErr

##################################################################################
# abonnements symEtriques
##################################################################################

# fiches main courante

tables/sub_drop.rab HIS FMC_GEN $NomSitePrim $NomSite xxx $FicErr
 
tables/sub_drop.rab HIS FMC_HIS $NomSitePrim $NomSite xxx $FicErr
 
tables/sub_drop.rab HIS FMC_COM $NomSitePrim $NomSite xxx $FicErr
 
tables/sub_drop.rab HIS FMC_OPR $NomSitePrim $NomSite xxx $FicErr
 
tables/sub_drop.rab HIS FMC_TRF $NomSitePrim $NomSite xxx $FicErr
 
tables/sub_drop.rab HIS FMC_TRH $NomSitePrim $NomSite xxx $FicErr
 
if [[ $TypeSitePrim = "NonSimplifie" ]]
then
tables/sub_drop.rab HIS FMC_ACC $NomSitePrim $NomSite xxx $FicErr
 
tables/sub_drop.rab HIS FMC_BAS $NomSitePrim $NomSite xxx $FicErr
 
tables/sub_drop.rab HIS FMC_BOU $NomSitePrim $NomSite xxx $FicErr
 
tables/sub_drop.rab HIS FMC_CNC $NomSitePrim $NomSite xxx $FicErr
 
tables/sub_drop.rab HIS FMC_DEL $NomSitePrim $NomSite xxx $FicErr
 
tables/sub_drop.rab HIS FMC_ECH $NomSitePrim $NomSite xxx $FicErr
 
tables/sub_drop.rab HIS FMC_MET $NomSitePrim $NomSite xxx $FicErr
 
tables/sub_drop.rab HIS FMC_MNF $NomSitePrim $NomSite xxx $FicErr
 
tables/sub_drop.rab HIS FMC_VEH $NomSitePrim $NomSite xxx $FicErr
 
tables/sub_drop.rab HIS FMC_VLN $NomSitePrim $NomSite xxx $FicErr
fi
 
# actions

tables/sub_drop.rab HIS ACT_GEN $NomSitePrim $NomSite xxx $FicErr
 
tables/sub_drop.rab HIS ACT_NAV $NomSitePrim $NomSite xxx $FicErr
 
tables/sub_drop.rab HIS ACT_PMV $NomSitePrim $NomSite xxx $FicErr

tables/sub_drop.rab HIS ACT_PAL $NomSitePrim $NomSite xxx $FicErr


if [[ $TypeSitePrim = "NonSimplifie" ]]
then
tables/sub_drop.rab HIS ACT_APL $NomSitePrim $NomSite xxx $FicErr

tables/sub_drop.rab HIS ACT_DEP $NomSitePrim $NomSite xxx $FicErr
 
tables/sub_drop.rab HIS ACT_ECH $NomSitePrim $NomSite xxx $FicErr
 
tables/sub_drop.rab HIS ACT_FAX $NomSitePrim $NomSite xxx $FicErr
 
tables/sub_drop.rab HIS ACT_FRG $NomSitePrim $NomSite xxx $FicErr
 
tables/sub_drop.rab HIS ACT_GAR $NomSitePrim $NomSite xxx $FicErr
 
tables/sub_drop.rab HIS ACT_INT $NomSitePrim $NomSite xxx $FicErr
 
tables/sub_drop.rab HIS ACT_RMQ $NomSitePrim $NomSite xxx $FicErr
 
tables/sub_drop.rab HIS ACT_SIS $NomSitePrim $NomSite xxx $FicErr
 
tables/sub_drop.rab HIS ACT_TFM $NomSitePrim $NomSite xxx $FicErr
 
tables/sub_drop.rab HIS ACT_TUB $NomSitePrim $NomSite xxx $FicErr
 
tables/sub_drop.rab HIS ACT_TXT_TFM $NomSitePrim $NomSite xxx $FicErr
fi
 
# autre

### condition particuliere
tables/sub_drop.rab HIS ALT_EXP $NomSitePrim $NomSite xxx $FicErr
 
tables/sub_drop.rab HIS EQT_DSP $NomSitePrim $NomSite xxx $FicErr
 
# mesures terrain

tables/sub_drop.rab HIS MES_POI $NomSitePrim $NomSite xxx $FicErr
 
tables/sub_drop.rab HIS MES_TRA $NomSitePrim $NomSite xxx $FicErr

tables/sub_drop.rab HIS MES_KCS $NomSitePrim $NomSite xxx $FicErr

tables/sub_drop.rab HIS MES_NEC $NomSitePrim $NomSite xxx $FicErr

tables/sub_drop.rab HIS MES_QPC $NomSitePrim $NomSite xxx $FicErr

