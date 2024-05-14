#!/bin/ksh
# Fichier : @(#)drop_abon_EXP_DP_HIS.sh	        Date : 19/03/2007
##################################################################################
# STERIA # PROJET ESCOTA PC N3 N2
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# SCRIPT drop_abon_EXP_DP_HIS.sh
##################################################################################
# DESCRIPTION DU SCRIPT : suppression de toutes les souscriptions EXP vers HIS 
# qui ont été récupérées lors du chargement de la base DP_RSSD de l'ancien PODP1
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# F.C.  19/03/07	: creation
##################################################################################

# nom du fichier d'erreur
FicErr="abon_EXP_DP_HIS.err"
rm $FicErr

tables/sub_drop.rab HIS ACT_BAF DP HIS sp_setreplicate $FicErr
tables/sub_drop.rab HIS ACT_BAD DP HIS sp_setreplicate $FicErr
tables/sub_drop.rab HIS ACT_GEN DP HIS sp_setreplicate $FicErr
tables/sub_drop.rab HIS ACT_NAV DP HIS sp_setreplicate $FicErr
tables/sub_drop.rab HIS ACT_PAL DP HIS sp_setreplicate $FicErr
tables/sub_drop.rab HIS ACT_PMV DP HIS sp_setreplicate $FicErr
tables/sub_drop.rab HIS ACT_PMA DP HIS sp_setreplicate $FicErr
tables/sub_drop.rab HIS ALM_EQT DP HIS sp_setreplicate $FicErr
tables/sub_drop.rab HIS ALM_NBR DP HIS sp_setreplicate $FicErr
tables/sub_drop.rab HIS ALT_EXP DP HIS sp_setreplicate $FicErr
tables/sub_drop.rab HIS EQT_ACV DP HIS sp_setreplicate $FicErr
tables/sub_drop.rab HIS EQT_DSP DP HIS sp_setreplicate $FicErr
tables/sub_drop.rab HIS FMC_COM DP HIS sp_setreplicate $FicErr
tables/sub_drop.rab HIS FMC_GEN DP HIS sp_setreplicate $FicErr
tables/sub_drop.rab HIS FMC_HIS DP HIS sp_setreplicate $FicErr
tables/sub_drop.rab HIS FMC_OPR DP HIS sp_setreplicate $FicErr
tables/sub_drop.rab HIS FMC_TRF DP HIS sp_setreplicate $FicErr
tables/sub_drop.rab HIS FMC_TRH DP HIS sp_setreplicate $FicErr
tables/sub_drop.rab HIS MES_NIV DP HIS sp_setreplicate $FicErr
tables/sub_drop.rab HIS MES_POI DP HIS sp_setreplicate $FicErr
tables/sub_drop.rab HIS MES_TRA DP HIS sp_setreplicate $FicErr
