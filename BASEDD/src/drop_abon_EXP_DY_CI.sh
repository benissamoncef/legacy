#!/bin/ksh
# Fichier : @(#)drop_abon_EXP_DY_CI.sh	        Date : 19/03/2007
##################################################################################
# STERIA # PROJET ESCOTA PC N3 N2
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# SCRIPT drop_abon_EXP_DY_CI.sh
##################################################################################
# DESCRIPTION DU SCRIPT : suppression de toutes les souscriptions EXP vers le CI 
# qui ont été récupérées lors du chargement de la base DY_RSSD de l'ancien PODY1
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# F.C.  19/03/07	: creation
##################################################################################

# nom du fichier d'erreur
FicErr="abon_EXP_DY_CI.err"
rm $FicErr

tables/sub_drop.rab EXP ACT_BAF DY CI sp_setreplicate $FicErr
tables/sub_drop.rab EXP ACT_BAD DY CI sp_setreplicate $FicErr
tables/sub_drop.rab EXP ACT_GEN DY CI sp_setreplicate $FicErr
tables/sub_drop.rab EXP ACT_NAV DY CI sp_setreplicate $FicErr
tables/sub_drop.rab EXP ACT_PAL DY CI sp_setreplicate $FicErr
tables/sub_drop.rab EXP ACT_PMV DY CI sp_setreplicate $FicErr
tables/sub_drop.rab EXP ACT_PMA DY CI sp_setreplicate $FicErr
tables/sub_drop.rab EXP ALM_EQT DY CI sp_setreplicate $FicErr
tables/sub_drop.rab EXP ALM_NBR DY CI sp_setreplicate $FicErr
tables/sub_drop.rab EXP ALT_EXP DY CI sp_setreplicate $FicErr
tables/sub_drop.rab EXP EQT_ACV DY CI sp_setreplicate $FicErr
tables/sub_drop.rab EXP EQT_DSP DY CI sp_setreplicate $FicErr
tables/sub_drop.rab EXP FMC_COM DY CI sp_setreplicate $FicErr
tables/sub_drop.rab EXP FMC_GEN DY CI sp_setreplicate $FicErr
tables/sub_drop.rab EXP FMC_HIS DY CI sp_setreplicate $FicErr
tables/sub_drop.rab EXP FMC_OPR DY CI sp_setreplicate $FicErr
tables/sub_drop.rab EXP FMC_TRF DY CI sp_setreplicate $FicErr
tables/sub_drop.rab EXP FMC_TRH DY CI sp_setreplicate $FicErr
tables/sub_drop.rab EXP MES_NIV DY CI sp_setreplicate $FicErr
tables/sub_drop.rab EXP MES_POI DY CI sp_setreplicate $FicErr
tables/sub_drop.rab EXP MES_TRA DY CI sp_setreplicate $FicErr
