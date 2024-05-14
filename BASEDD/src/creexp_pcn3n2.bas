#!/bin/ksh
# Fichier : $Id: creexp_pcn3n2.bas,v 1.1 2007/06/22 10:49:52 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2007/06/22 10:49:52 $
##################################################################################
# STERIA # PROJET MIGRAZUR
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# SCRIPT creexp_pcn3n2.bas
##################################################################################
# DESCRIPTION DU SCRIPT :
# creation des tables, des index, des cles et des triggers de la base EXP
# pour les nouvelles tables pour la migration PC N3 N2
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# F.C.	10/04/07	: Creation
##################################################################################

# Creation des tables de la base EXP

# controle des parametres d'appels
if ((($#<2)) || (test $2 != "EXP"))
then echo "syntaxe : creexp_pcn3n2.bas NomServeur EXP NomFicErreur"
     echo "          NomFicErreur optionnel"
     exit 0
fi

# serveur a utiliser
NomServeur=$1

# nom de la base 
NomBase=$2

# fichier de compte rendu d'erreur
if (($#==3))
then FicErr=${3}.err
else FicErr="creexp_pcn3n2.err"
fi
if test -r $FicErr
then rm $FicErr
fi

#########################
# Creation des tables et des index 
#########################

# nom table : ACT_BAF, code : ACT_BAF
echo "ACT_BAF" >> $FicErr
cat exp.use tables/act_baf.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACT_BAD, code : ACT_BAD
echo "ACT_BAD" >> $FicErr
cat exp.use tables/act_bad.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACT_PMA, code : ACT_PMA
echo "ACT_PMA" >> $FicErr
cat exp.use tables/act_pma.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FMC_NAT, code : FMC_NAT
echo "FMC_NAT" >> $FicErr
cat exp.use tables/fmc_nat.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACT_ADA, code : ACT_ADA
echo "ACT_ADA" >> $FicErr
cat exp.use tables/act_ada.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACT_APL, code : ACT_APL
echo "ACT_APL" >> $FicErr
cat exp.use tables/act_apl.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACT_DEP, code : ACT_DEP
echo "ACT_DEP" >> $FicErr
cat exp.use tables/act_dep.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACT_ECH, code : ACT_ECH
echo "ACT_ECH" >> $FicErr
cat exp.use tables/act_ech.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACT_EXP_ADA, code : ACT_EXP_ADA
echo "ACT_EXP_ADA" >> $FicErr
cat exp.use tables/act_exp_ada.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACT_FAX, code : ACT_FAX
echo "ACT_FAX" >> $FicErr
cat exp.use tables/act_fax.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACT_FRG, code : ACT_FRG
echo "ACT_FRG" >> $FicErr
cat exp.use tables/act_frg.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACT_GAR, code : ACT_GAR
echo "ACT_GAR" >> $FicErr
cat exp.use tables/act_gar.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACT_INT, code : ACT_INT
echo "ACT_INT" >> $FicErr
cat exp.use tables/act_int.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACT_PMV_ADA, code : ACT_PMV_ADA
echo "ACT_PMV_ADA" >> $FicErr
cat exp.use tables/act_pmv_ada.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACT_RMQ, code : ACT_RMQ
echo "ACT_RMQ" >> $FicErr
cat exp.use tables/act_rmq.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACT_SIS, code : ACT_SIS
echo "ACT_SIS" >> $FicErr
cat exp.use tables/act_sis.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACT_TFM, code : ACT_TFM
echo "ACT_TFM" >> $FicErr
cat exp.use tables/act_tfm.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACT_TFM_EXP, code : ACT_TFM_EXP
echo "ACT_TFM_EXP" >> $FicErr
cat exp.use tables/act_tfm_exp.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACT_TUB, code : ACT_TUB
echo "ACT_TUB" >> $FicErr
cat exp.use tables/act_tub.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACT_TXT_TFM, code : ACT_TXT_TFM
echo "ACT_TXT_TFM" >> $FicErr
cat exp.use tables/act_txt_tfm.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ADA_ACT, code : ADA_ACT
echo "ADA_ACT" >> $FicErr
cat exp.use tables/ada_act.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ADA_FMC, code : ADA_FMC
echo "ADA_FMC" >> $FicErr
cat exp.use tables/ada_fmc.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ADA_SIT, code : ADA_SIT
echo "ADA_SIT" >> $FicErr
cat exp.use tables/ada_sit.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ADA_SYN, code : ADA_SYN
echo "ADA_SYN" >> $FicErr
cat exp.use tables/ada_syn.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ADA_TXT_SYN, code : ADA_TXT_SYN
echo "ADA_TXT_SYN" >> $FicErr
cat exp.use tables/ada_txt_syn.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : APL_FAX, code : APL_FAX
echo "APL_FAX" >> $FicErr
cat exp.use tables/apl_fax.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : EQT_FIL, code : EQT_FIL
echo "EQT_FIL" >> $FicErr
cat exp.use tables/eqt_fil.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : EQT_FIL_TRV, code : EQT_FIL_TRV
echo "EQT_FIL_TRV" >> $FicErr
cat exp.use tables/eqt_fil_trv.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FMC_ACC, code : FMC_ACC
echo "FMC_ACC" >> $FicErr
cat exp.use tables/fmc_acc.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FMC_ADA, code : FMC_ADA
echo "FMC_ADA" >> $FicErr
cat exp.use tables/fmc_ada.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FMC_BAS, code : FMC_BAS
echo "FMC_BAS" >> $FicErr
cat exp.use tables/fmc_bas.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FMC_BOU, code : FMC_BOU
echo "FMC_BOU" >> $FicErr
cat exp.use tables/fmc_bou.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FMC_CNC, code : FMC_CNC
echo "FMC_CNC" >> $FicErr
cat exp.use tables/fmc_cnc.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FMC_DAT, code : FMC_DAT
echo "FMC_DAT" >> $FicErr
cat exp.use tables/fmc_dat.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FMC_DEL, code : FMC_DEL
echo "FMC_DEL" >> $FicErr
cat exp.use tables/fmc_del.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FMC_ECH, code : FMC_ECH
echo "FMC_ECH" >> $FicErr
cat exp.use tables/fmc_ech.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FMC_HIS_DER, code : FMC_HIS_DER
echo "FMC_HIS_DER" >> $FicErr
cat exp.use tables/fmc_his_der.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FMC_LIS, code : FMC_LIS
echo "FMC_LIS" >> $FicErr
cat exp.use tables/fmc_lis.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FMC_MET, code : FMC_MET
echo "FMC_MET" >> $FicErr
cat exp.use tables/fmc_met.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FMC_MNF, code : FMC_MNF
echo "FMC_MNF" >> $FicErr
cat exp.use tables/fmc_mnf.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FMC_REV, code : FMC_REV
echo "FMC_REV" >> $FicErr
cat exp.use tables/fmc_rev.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FMC_SAT, code : FMC_SAT
echo "FMC_SAT" >> $FicErr
cat exp.use tables/fmc_sat.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FMC_SYN, code : FMC_SYN
echo "FMC_SYN" >> $FicErr
cat exp.use tables/fmc_syn.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FMC_TFM, code : FMC_TFM
echo "FMC_TFM" >> $FicErr
cat exp.use tables/fmc_tfm.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FMC_TRS, code : FMC_TRS
echo "FMC_TRS" >> $FicErr
cat exp.use tables/fmc_trs.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FMC_TRT, code : FMC_TRT
echo "FMC_TRT" >> $FicErr
cat exp.use tables/fmc_trt.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FMC_VEH, code : FMC_VEH
echo "FMC_VEH" >> $FicErr
cat exp.use tables/fmc_veh.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FMC_VLN, code : FMC_VLN
echo "FMC_VLN" >> $FicErr
cat exp.use tables/fmc_vln.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FRN_TDP, code : FRN_TDP
echo "FRN_TDP" >> $FicErr
cat exp.use tables/frn_tdp.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : LST_CPT, code : LST_CPT
echo "LST_CPT" >> $FicErr
cat exp.use tables/lst_cpt.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : MES_ADA, code : MES_ADA
echo "MES_ADA" >> $FicErr
cat exp.use tables/mes_ada.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : MES_NIV_DER, code : MES_NIV_DER
echo "MES_NIV_DER" >> $FicErr
cat exp.use tables/mes_niv_der.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : PMV_ADA, code : PMV_ADA
echo "PMV_ADA" >> $FicErr
cat exp.use tables/pmv_ada.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SAD_APL, code : SAD_APL
echo "SAD_APL" >> $FicErr
cat exp.use tables/sad_apl.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SAD_BAD, code : SAD_BAD
echo "SAD_BAD" >> $FicErr
cat exp.use tables/sad_bad.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SAD_BAF, code : SAD_BAF
echo "SAD_BAF" >> $FicErr
cat exp.use tables/sad_baf.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SAD_DST_FAX, code : SAD_DST_FAX
echo "SAD_DST_FAX" >> $FicErr
cat exp.use tables/sad_dst_fax.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SAD_ECH, code : SAD_ECH
echo "SAD_ECH" >> $FicErr
cat exp.use tables/sad_ech.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SAD_EXP, code : SAD_EXP
echo "SAD_EXP" >> $FicErr
cat exp.use tables/sad_exp.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SAD_FAX, code : SAD_FAX
echo "SAD_FAX" >> $FicErr
cat exp.use tables/sad_fax.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SAD_MSG_TFM, code : SAD_MSG_TFM
echo "SAD_MSG_TFM" >> $FicErr
cat exp.use tables/sad_msg_tfm.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SAD_NAV, code : SAD_NAV
echo "SAD_NAV" >> $FicErr
cat exp.use tables/sad_nav.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SAD_PAL, code : SAD_PAL
echo "SAD_PAL" >> $FicErr
cat exp.use tables/sad_pal.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SAD_PLN, code : SAD_PLN
echo "SAD_PLN" >> $FicErr
cat exp.use tables/sad_pln.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SAD_PMA, code : SAD_PMA
echo "SAD_PMA" >> $FicErr
cat exp.use tables/sad_pma.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SAD_PMV, code : SAD_PMV
echo "SAD_PMV" >> $FicErr
cat exp.use tables/sad_pmv.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SAD_TFM, code : SAD_TFM
echo "SAD_TFM" >> $FicErr
cat exp.use tables/sad_tfm.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SAD_TUB, code : SAD_TUB
echo "SAD_TUB" >> $FicErr
cat exp.use tables/sad_tub.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SAD_TXT_FAX, code : SAD_TXT_FAX
echo "SAD_TXT_FAX" >> $FicErr
cat exp.use tables/sad_txt_fax.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SAD_TXT_TFM, code : SAD_TXT_TFM
echo "SAD_TXT_TFM" >> $FicErr
cat exp.use tables/sad_txt_tfm.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SYN_BAS_SEG, code : SYN_BAS_SEG
echo "SYN_BAS_SEG" >> $FicErr
cat exp.use tables/syn_bas_seg.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SYN_BOU_POR, code : SYN_BOU_POR
echo "SYN_BOU_POR" >> $FicErr
cat exp.use tables/syn_bou_por.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SYN_BOU_SEG, code : SYN_BOU_SEG
echo "SYN_BOU_SEG" >> $FicErr
cat exp.use tables/syn_bou_seg.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SYN_INV_SEG, code : SYN_INV_SEG
echo "SYN_INV_SEG" >> $FicErr
cat exp.use tables/syn_inv_seg.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SYN_NEU_SEG, code : SYN_NEU_SEG
echo "SYN_NEU_SEG" >> $FicErr
cat exp.use tables/syn_neu_seg.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SYN_OBJ_POR, code : SYN_OBJ_POR
echo "SYN_OBJ_POR" >> $FicErr
cat exp.use tables/syn_obj_por.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SYN_OBJ_SEG, code : SYN_OBJ_SEG
echo "SYN_OBJ_SEG" >> $FicErr
cat exp.use tables/syn_obj_seg.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SYN_RAB_SEG, code : SYN_RAB_SEG
echo "SYN_RAB_SEG" >> $FicErr
cat exp.use tables/syn_rab_seg.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : TFM_ADA, code : TFM_ADA
echo "TFM_ADA" >> $FicErr
cat exp.use tables/tfm_ada.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : TFM_DTX, code : TFM_DTX
echo "TFM_DTX" >> $FicErr
cat exp.use tables/tfm_dtx.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : TFM_TDP, code : TFM_TDP
echo "TFM_TDP" >> $FicErr
cat exp.use tables/tfm_tdp.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : VOI_DAI, code : VOI_DAI
echo "VOI_DAI" >> $FicErr
cat exp.use tables/voi_dai.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : VOI_RAD, code : VOI_RAD
echo "VOI_RAD" >> $FicErr
cat exp.use tables/voi_rad.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : WEB_CRI, code : WEB_CRI
echo "WEB_CRI" >> $FicErr
cat exp.use tables/web_cri.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : WEB_EVT, code : WEB_EVT
echo "WEB_EVT" >> $FicErr
cat exp.use tables/web_evt.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : WEB_TXT, code : WEB_TXT
echo "WEB_TXT" >> $FicErr
cat exp.use tables/web_txt.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ZDP_6MN, code : ZDP_6MN
echo "ZDP_6MN" >> $FicErr
cat exp.use tables/zdp_6mn.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# Test si un probleme est survenu lors de la creation de table
grep "Msg" $FicErr | wc -l | read NbLignes ResteInfo
if ((NbLignes!=0))
then exit 0
fi
rm $FicErr


###################
# Creation des cles 
###################

# nom table : ACT_BAF, code : ACT_BAF
echo "ACT_BAF" >> $FicErr
cat exp.use tables/act_baf.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACT_BAD, code : ACT_BAD
echo "ACT_BAD" >> $FicErr
cat exp.use tables/act_bad.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACT_PMA, code : ACT_PMA
echo "ACT_PMA" >> $FicErr
cat exp.use tables/act_pma.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FMC_NAT, code : FMC_NAT
echo "FMC_NAT" >> $FicErr
cat exp.use tables/fmc_nat.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACT_ADA, code : ACT_ADA
echo "ACT_ADA" >> $FicErr
cat exp.use tables/act_ada.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACT_APL, code : ACT_APL
echo "ACT_APL" >> $FicErr
cat exp.use tables/act_apl.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACT_DEP, code : ACT_DEP
echo "ACT_DEP" >> $FicErr
cat exp.use tables/act_dep.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACT_ECH, code : ACT_ECH
echo "ACT_ECH" >> $FicErr
cat exp.use tables/act_ech.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACT_EXP_ADA, code : ACT_EXP_ADA
echo "ACT_EXP_ADA" >> $FicErr
cat exp.use tables/act_exp_ada.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACT_FAX, code : ACT_FAX
echo "ACT_FAX" >> $FicErr
cat exp.use tables/act_fax.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACT_FRG, code : ACT_FRG
echo "ACT_FRG" >> $FicErr
cat exp.use tables/act_frg.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACT_GAR, code : ACT_GAR
echo "ACT_GAR" >> $FicErr
cat exp.use tables/act_gar.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACT_INT, code : ACT_INT
echo "ACT_INT" >> $FicErr
cat exp.use tables/act_int.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACT_PMV_ADA, code : ACT_PMV_ADA
echo "ACT_PMV_ADA" >> $FicErr
cat exp.use tables/act_pmv_ada.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACT_RMQ, code : ACT_RMQ
echo "ACT_RMQ" >> $FicErr
cat exp.use tables/act_rmq.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACT_SIS, code : ACT_SIS
echo "ACT_SIS" >> $FicErr
cat exp.use tables/act_sis.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACT_TFM, code : ACT_TFM
echo "ACT_TFM" >> $FicErr
cat exp.use tables/act_tfm.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACT_TFM_EXP, code : ACT_TFM_EXP
echo "ACT_TFM_EXP" >> $FicErr
cat exp.use tables/act_tfm_exp.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACT_TUB, code : ACT_TUB
echo "ACT_TUB" >> $FicErr
cat exp.use tables/act_tub.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACT_TXT_TFM, code : ACT_TXT_TFM
echo "ACT_TXT_TFM" >> $FicErr
cat exp.use tables/act_txt_tfm.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ADA_ACT, code : ADA_ACT
echo "ADA_ACT" >> $FicErr
cat exp.use tables/ada_act.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ADA_FMC, code : ADA_FMC
echo "ADA_FMC" >> $FicErr
cat exp.use tables/ada_fmc.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ADA_SIT, code : ADA_SIT
echo "ADA_SIT" >> $FicErr
cat exp.use tables/ada_sit.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ADA_SYN, code : ADA_SYN
echo "ADA_SYN" >> $FicErr
cat exp.use tables/ada_syn.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ADA_TXT_SYN, code : ADA_TXT_SYN
echo "ADA_TXT_SYN" >> $FicErr
cat exp.use tables/ada_txt_syn.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : APL_FAX, code : APL_FAX
echo "APL_FAX" >> $FicErr
cat exp.use tables/apl_fax.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : EQT_FIL, code : EQT_FIL
echo "EQT_FIL" >> $FicErr
cat exp.use tables/eqt_fil.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : EQT_FIL_TRV, code : EQT_FIL_TRV
echo "EQT_FIL_TRV" >> $FicErr
cat exp.use tables/eqt_fil_trv.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FMC_ACC, code : FMC_ACC
echo "FMC_ACC" >> $FicErr
cat exp.use tables/fmc_acc.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FMC_ADA, code : FMC_ADA
echo "FMC_ADA" >> $FicErr
cat exp.use tables/fmc_ada.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FMC_BAS, code : FMC_BAS
echo "FMC_BAS" >> $FicErr
cat exp.use tables/fmc_bas.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FMC_BOU, code : FMC_BOU
echo "FMC_BOU" >> $FicErr
cat exp.use tables/fmc_bou.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FMC_CNC, code : FMC_CNC
echo "FMC_CNC" >> $FicErr
cat exp.use tables/fmc_cnc.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FMC_DAT, code : FMC_DAT
echo "FMC_DAT" >> $FicErr
cat exp.use tables/fmc_dat.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FMC_DEL, code : FMC_DEL
echo "FMC_DEL" >> $FicErr
cat exp.use tables/fmc_del.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FMC_ECH, code : FMC_ECH
echo "FMC_ECH" >> $FicErr
cat exp.use tables/fmc_ech.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FMC_HIS_DER, code : FMC_HIS_DER
echo "FMC_HIS_DER" >> $FicErr
cat exp.use tables/fmc_his_der.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FMC_LIS, code : FMC_LIS
echo "FMC_LIS" >> $FicErr
cat exp.use tables/fmc_lis.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FMC_MET, code : FMC_MET
echo "FMC_MET" >> $FicErr
cat exp.use tables/fmc_met.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FMC_MNF, code : FMC_MNF
echo "FMC_MNF" >> $FicErr
cat exp.use tables/fmc_mnf.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FMC_REV, code : FMC_REV
echo "FMC_REV" >> $FicErr
cat exp.use tables/fmc_rev.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FMC_SAT, code : FMC_SAT
echo "FMC_SAT" >> $FicErr
cat exp.use tables/fmc_sat.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FMC_SYN, code : FMC_SYN
echo "FMC_SYN" >> $FicErr
cat exp.use tables/fmc_syn.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FMC_TFM, code : FMC_TFM
echo "FMC_TFM" >> $FicErr
cat exp.use tables/fmc_tfm.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FMC_TRS, code : FMC_TRS
echo "FMC_TRS" >> $FicErr
cat exp.use tables/fmc_trs.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FMC_TRT, code : FMC_TRT
echo "FMC_TRT" >> $FicErr
cat exp.use tables/fmc_trt.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FMC_VEH, code : FMC_VEH
echo "FMC_VEH" >> $FicErr
cat exp.use tables/fmc_veh.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FMC_VLN, code : FMC_VLN
echo "FMC_VLN" >> $FicErr
cat exp.use tables/fmc_vln.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FRN_TDP, code : FRN_TDP
echo "FRN_TDP" >> $FicErr
cat exp.use tables/frn_tdp.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : LST_CPT, code : LST_CPT
echo "LST_CPT" >> $FicErr
cat exp.use tables/lst_cpt.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : MES_ADA, code : MES_ADA
echo "MES_ADA" >> $FicErr
cat exp.use tables/mes_ada.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : MES_NIV_DER, code : MES_NIV_DER
echo "MES_NIV_DER" >> $FicErr
cat exp.use tables/mes_niv_der.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : PMV_ADA, code : PMV_ADA
echo "PMV_ADA" >> $FicErr
cat exp.use tables/pmv_ada.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SAD_APL, code : SAD_APL
echo "SAD_APL" >> $FicErr
cat exp.use tables/sad_apl.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SAD_BAD, code : SAD_BAD
echo "SAD_BAD" >> $FicErr
cat exp.use tables/sad_bad.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SAD_BAF, code : SAD_BAF
echo "SAD_BAF" >> $FicErr
cat exp.use tables/sad_baf.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SAD_DST_FAX, code : SAD_DST_FAX
echo "SAD_DST_FAX" >> $FicErr
cat exp.use tables/sad_dst_fax.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SAD_ECH, code : SAD_ECH
echo "SAD_ECH" >> $FicErr
cat exp.use tables/sad_ech.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SAD_EXP, code : SAD_EXP
echo "SAD_EXP" >> $FicErr
cat exp.use tables/sad_exp.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SAD_FAX, code : SAD_FAX
echo "SAD_FAX" >> $FicErr
cat exp.use tables/sad_fax.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SAD_MSG_TFM, code : SAD_MSG_TFM
echo "SAD_MSG_TFM" >> $FicErr
cat exp.use tables/sad_msg_tfm.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SAD_NAV, code : SAD_NAV
echo "SAD_NAV" >> $FicErr
cat exp.use tables/sad_nav.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SAD_PAL, code : SAD_PAL
echo "SAD_PAL" >> $FicErr
cat exp.use tables/sad_pal.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SAD_PLN, code : SAD_PLN
echo "SAD_PLN" >> $FicErr
cat exp.use tables/sad_pln.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SAD_PMA, code : SAD_PMA
echo "SAD_PMA" >> $FicErr
cat exp.use tables/sad_pma.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SAD_PMV, code : SAD_PMV
echo "SAD_PMV" >> $FicErr
cat exp.use tables/sad_pmv.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SAD_TFM, code : SAD_TFM
echo "SAD_TFM" >> $FicErr
cat exp.use tables/sad_tfm.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SAD_TUB, code : SAD_TUB
echo "SAD_TUB" >> $FicErr
cat exp.use tables/sad_tub.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SAD_TXT_FAX, code : SAD_TXT_FAX
echo "SAD_TXT_FAX" >> $FicErr
cat exp.use tables/sad_txt_fax.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SAD_TXT_TFM, code : SAD_TXT_TFM
echo "SAD_TXT_TFM" >> $FicErr
cat exp.use tables/sad_txt_tfm.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SYN_BAS_SEG, code : SYN_BAS_SEG
echo "SYN_BAS_SEG" >> $FicErr
cat exp.use tables/syn_bas_seg.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SYN_BOU_POR, code : SYN_BOU_POR
echo "SYN_BOU_POR" >> $FicErr
cat exp.use tables/syn_bou_por.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SYN_BOU_SEG, code : SYN_BOU_SEG
echo "SYN_BOU_SEG" >> $FicErr
cat exp.use tables/syn_bou_seg.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SYN_INV_SEG, code : SYN_INV_SEG
echo "SYN_INV_SEG" >> $FicErr
cat exp.use tables/syn_inv_seg.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SYN_NEU_SEG, code : SYN_NEU_SEG
echo "SYN_NEU_SEG" >> $FicErr
cat exp.use tables/syn_neu_seg.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SYN_OBJ_POR, code : SYN_OBJ_POR
echo "SYN_OBJ_POR" >> $FicErr
cat exp.use tables/syn_obj_por.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SYN_OBJ_SEG, code : SYN_OBJ_SEG
echo "SYN_OBJ_SEG" >> $FicErr
cat exp.use tables/syn_obj_seg.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SYN_RAB_SEG, code : SYN_RAB_SEG
echo "SYN_RAB_SEG" >> $FicErr
cat exp.use tables/syn_rab_seg.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : TFM_ADA, code : TFM_ADA
echo "TFM_ADA" >> $FicErr
cat exp.use tables/tfm_ada.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : TFM_DTX, code : TFM_DTX
echo "TFM_DTX" >> $FicErr
cat exp.use tables/tfm_dtx.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : TFM_TDP, code : TFM_TDP
echo "TFM_TDP" >> $FicErr
cat exp.use tables/tfm_tdp.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : VOI_DAI, code : VOI_DAI
echo "VOI_DAI" >> $FicErr
cat exp.use tables/voi_dai.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : VOI_RAD, code : VOI_RAD
echo "VOI_RAD" >> $FicErr
cat exp.use tables/voi_rad.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : WEB_CRI, code : WEB_CRI
echo "WEB_CRI" >> $FicErr
cat exp.use tables/web_cri.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : WEB_EVT, code : WEB_EVT
echo "WEB_EVT" >> $FicErr
cat exp.use tables/web_evt.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : WEB_TXT, code : WEB_TXT
echo "WEB_TXT" >> $FicErr
cat exp.use tables/web_txt.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ZDP_6MN, code : ZDP_6MN
echo "ZDP_6MN" >> $FicErr
cat exp.use tables/zdp_6mn.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# Test si un probleme est survenu lors de la creation de cles
grep "Msg" $FicErr | wc -l | read NbLignes ResteInfo
if ((NbLignes!=0))
then exit 0
fi
rm $FicErr


#################
# Creation des triggers
#################


exit 1
