#!/bin/ksh
# Fichier : @(#)drop_abon_EXP_DP_CI.sh	        Date : 19/03/2007
##################################################################################
# STERIA # PROJET ESCOTA PC N3 N2
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# SCRIPT drop_abon_EXP_DP_CI.sh
##################################################################################
# DESCRIPTION DU SCRIPT : suppression de toutes les souscriptions EXP vers le CI 
# qui ont été récupérées lors du chargement de la base DP_RSSD de l'ancien PODP1
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# F.C.  19/03/07	: creation
##################################################################################

# nom du fichier d'erreur
FicErr="abon_EXP_DP_CI.err"
rm $FicErr

tables/sub_drop.rab EXP ACT_BAF DP CI sp_setreplicate $FicErr
tables/sub_drop.rab EXP ACT_BAD DP CI sp_setreplicate $FicErr
tables/sub_drop.rab EXP ACT_GEN DP CI sp_setreplicate $FicErr
tables/sub_drop.rab EXP ACT_NAV DP CI sp_setreplicate $FicErr
tables/sub_drop.rab EXP ACT_PAL DP CI sp_setreplicate $FicErr
tables/sub_drop.rab EXP ACT_PMV DP CI sp_setreplicate $FicErr
tables/sub_drop.rab EXP ACT_PMA DP CI sp_setreplicate $FicErr
tables/sub_drop.rab EXP ALM_EQT DP CI sp_setreplicate $FicErr
tables/sub_drop.rab EXP ALM_NBR DP CI sp_setreplicate $FicErr
tables/sub_drop.rab EXP ALT_EXP DP CI sp_setreplicate $FicErr
tables/sub_drop.rab EXP EQT_ACV DP CI sp_setreplicate $FicErr
tables/sub_drop.rab EXP EQT_DSP DP CI sp_setreplicate $FicErr
tables/sub_drop.rab EXP FMC_COM DP CI sp_setreplicate $FicErr
tables/sub_drop.rab EXP FMC_GEN DP CI sp_setreplicate $FicErr
tables/sub_drop.rab EXP FMC_HIS DP CI sp_setreplicate $FicErr
tables/sub_drop.rab EXP FMC_OPR DP CI sp_setreplicate $FicErr
tables/sub_drop.rab EXP FMC_TRF DP CI sp_setreplicate $FicErr
tables/sub_drop.rab EXP FMC_TRH DP CI sp_setreplicate $FicErr
tables/sub_drop.rab EXP MES_NIV DP CI sp_setreplicate $FicErr
tables/sub_drop.rab EXP MES_POI DP CI sp_setreplicate $FicErr
tables/sub_drop.rab EXP MES_TRA DP CI sp_setreplicate $FicErr
