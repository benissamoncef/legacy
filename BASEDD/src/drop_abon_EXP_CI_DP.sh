#!/bin/ksh
# Fichier : @(#)drop_abon_EXP_CI_DP.sh	        Date : 19/03/2007
##################################################################################
# STERIA # PROJET ESCOTA PC N3 N2
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# SCRIPT drop_abon_EXP_CI_DP.sh
##################################################################################
# DESCRIPTION DU SCRIPT : suppression de toutes les souscriptions EXP partant du CI vers DP
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# F.C.  19/03/07	: creation
##################################################################################

# nom du fichier d'erreur
FicErr="abon_EXP_CI_DP.err"
rm $FicErr

tables/sub_drop.rab EXP ACT_BAF CI DP sp_setreplicate $FicErr
tables/sub_drop.rab EXP ACT_BAD CI DP sp_setreplicate $FicErr
tables/sub_drop.rab EXP ACT_GEN CI DP sp_setreplicate $FicErr
tables/sub_drop.rab EXP ACT_NAV CI DP sp_setreplicate $FicErr
tables/sub_drop.rab EXP ACT_PAL CI DP sp_setreplicate $FicErr
tables/sub_drop.rab EXP ACT_PMV CI DP sp_setreplicate $FicErr
tables/sub_drop.rab EXP ACT_PMA CI DP sp_setreplicate $FicErr
tables/sub_drop.rab EXP ALT_EXP CI DP sp_setreplicate $FicErr
tables/sub_drop.rab EXP EQT_DSP CI DP sp_setreplicate $FicErr
tables/sub_drop.rab EXP FMC_COM CI DP sp_setreplicate $FicErr
tables/sub_drop.rab EXP FMC_GEN CI DP sp_setreplicate $FicErr
tables/sub_drop.rab EXP FMC_HIS CI DP sp_setreplicate $FicErr
tables/sub_drop.rab EXP FMC_OPR CI DP sp_setreplicate $FicErr
tables/sub_drop.rab EXP FMC_TRF CI DP sp_setreplicate $FicErr
tables/sub_drop.rab EXP FMC_TRH CI DP sp_setreplicate $FicErr
tables/sub_drop.rab EXP MES_NIV CI DP sp_setreplicate $FicErr
tables/sub_drop.rab EXP MES_POI CI DP sp_setreplicate $FicErr
tables/sub_drop.rab EXP MES_TRA CI DP sp_setreplicate $FicErr
