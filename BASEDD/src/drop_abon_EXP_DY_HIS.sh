#!/bin/ksh
# Fichier : @(#)drop_abon_EXP_DY_HIS.sh	        Date : 19/03/2007
##################################################################################
# STERIA # PROJET ESCOTA PC N3 N2
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# SCRIPT drop_abon_EXP_DY_HIS.sh
##################################################################################
# DESCRIPTION DU SCRIPT : suppression de toutes les souscriptions EXP vers HIS 
# qui ont été récupérées lors du chargement de la base DY_RSSD de l'ancien PODY1
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# F.C.  19/03/07	: creation
##################################################################################

# nom du fichier d'erreur
FicErr="abon_EXP_DY_HIS.err"
rm $FicErr

tables/sub_drop.rab HIS ACT_BAF DY HIS sp_setreplicate $FicErr
tables/sub_drop.rab HIS ACT_BAD DY HIS sp_setreplicate $FicErr
tables/sub_drop.rab HIS ACT_GEN DY HIS sp_setreplicate $FicErr
tables/sub_drop.rab HIS ACT_NAV DY HIS sp_setreplicate $FicErr
tables/sub_drop.rab HIS ACT_PAL DY HIS sp_setreplicate $FicErr
tables/sub_drop.rab HIS ACT_PMV DY HIS sp_setreplicate $FicErr
tables/sub_drop.rab HIS ACT_PMA DY HIS sp_setreplicate $FicErr
tables/sub_drop.rab HIS ALM_EQT DY HIS sp_setreplicate $FicErr
tables/sub_drop.rab HIS ALM_NBR DY HIS sp_setreplicate $FicErr
tables/sub_drop.rab HIS ALT_EXP DY HIS sp_setreplicate $FicErr
tables/sub_drop.rab HIS EQT_ACV DY HIS sp_setreplicate $FicErr
tables/sub_drop.rab HIS EQT_DSP DY HIS sp_setreplicate $FicErr
tables/sub_drop.rab HIS FMC_COM DY HIS sp_setreplicate $FicErr
tables/sub_drop.rab HIS FMC_GEN DY HIS sp_setreplicate $FicErr
tables/sub_drop.rab HIS FMC_HIS DY HIS sp_setreplicate $FicErr
tables/sub_drop.rab HIS FMC_OPR DY HIS sp_setreplicate $FicErr
tables/sub_drop.rab HIS FMC_TRF DY HIS sp_setreplicate $FicErr
tables/sub_drop.rab HIS FMC_TRH DY HIS sp_setreplicate $FicErr
tables/sub_drop.rab HIS MES_NIV DY HIS sp_setreplicate $FicErr
tables/sub_drop.rab HIS MES_POI DY HIS sp_setreplicate $FicErr
tables/sub_drop.rab HIS MES_TRA DY HIS sp_setreplicate $FicErr
tables/sub_drop.rab HIS MES_KCS DY HIS sp_setreplicate $FicErr
tables/sub_drop.rab HIS MES_NEC DY HIS sp_setreplicate $FicErr
tables/sub_drop.rab HIS MES_QPC DY HIS sp_setreplicate $FicErr
