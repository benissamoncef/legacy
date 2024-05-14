#!/bin/ksh
# Fichier : @(#)drop_abon_EXP_CI_DY.sh	        Date : 19/03/2007
##################################################################################
# STERIA # PROJET ESCOTA PC N3 N2
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# SCRIPT drop_abon_EXP_CI_DY.sh
##################################################################################
# DESCRIPTION DU SCRIPT : suppression de toutes les souscriptions EXP partant du CI vers DY
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# F.C.  19/03/07	: creation
##################################################################################

# nom du fichier d'erreur
FicErr="abon_EXP_CI_DY.err"
rm $FicErr

tables/sub_drop.rab EXP ACT_BAF CI DY sp_setreplicate $FicErr
tables/sub_drop.rab EXP ACT_BAD CI DY sp_setreplicate $FicErr
tables/sub_drop.rab EXP ACT_GEN CI DY sp_setreplicate $FicErr
tables/sub_drop.rab EXP ACT_NAV CI DY sp_setreplicate $FicErr
tables/sub_drop.rab EXP ACT_PAL CI DY sp_setreplicate $FicErr
tables/sub_drop.rab EXP ACT_PMV CI DY sp_setreplicate $FicErr
tables/sub_drop.rab EXP ACT_PMA CI DY sp_setreplicate $FicErr
tables/sub_drop.rab EXP ALT_EXP CI DY sp_setreplicate $FicErr
tables/sub_drop.rab EXP EQT_DSP CI DY sp_setreplicate $FicErr
tables/sub_drop.rab EXP FMC_COM CI DY sp_setreplicate $FicErr
tables/sub_drop.rab EXP FMC_GEN CI DY sp_setreplicate $FicErr
tables/sub_drop.rab EXP FMC_HIS CI DY sp_setreplicate $FicErr
tables/sub_drop.rab EXP FMC_OPR CI DY sp_setreplicate $FicErr
tables/sub_drop.rab EXP FMC_TRF CI DY sp_setreplicate $FicErr
tables/sub_drop.rab EXP FMC_TRH CI DY sp_setreplicate $FicErr
tables/sub_drop.rab EXP MES_NIV CI DY sp_setreplicate $FicErr
tables/sub_drop.rab EXP MES_POI CI DY sp_setreplicate $FicErr
tables/sub_drop.rab EXP MES_TRA CI DY sp_setreplicate $FicErr
