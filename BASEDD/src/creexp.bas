#!/bin/ksh
# Fichier : $Id: creexp.bas,v 1.14 2007/05/15 17:11:37 gesconf Exp $        Release : $Revision: 1.14 $        Date : $Date: 2007/05/15 17:11:37 $
##################################################################################
# STERIA # PROJET MIGRAZUR
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# SCRIPT creexp.bas
##################################################################################
# DESCRIPTION DU SCRIPT :
# creation des tables, des index, des cles et des triggers de la base EXP
#
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# C.T.	09/08/94	: Creation
# C.T.	24/02/95	: Ajout la creation de la table FMC_SYN
# C.T.	09/09/96	: Ajout la creation de la table FMC_PUR au CI pour RADT
# P.N.	18/11/96	: Ajout MES_QPC KCS et NEC
# P.N.	21/02/97	: Ajout FRN_TDP et ZDP_6MN + LST_CPT
# C.T.	19/12/97	: Ajout FMC_TRS (1531)
# C.T.	16/01/98	: Ajout ACT_PAL (1.11)
# C.T.	25/02/98	: Ajout ADA_SYN, FMC_ADA (1.12)
# PN	22/03/99	: Ajout ADA_SIT,FMC_DAT,ADA_TXT_SYN,ACT_EXP_ADA,
			ACT_TFM_EXP, ACT_PMV_ADA, PMV_ADA,TFM_ADA, MES_ADA,
			MES_NIV_DER, SAD_EXP (1.13)
##################################################################################

# Creation des tables de la base EXP

# controle des parametres d'appels
if ((($#<2)) || (test $2 != "EXP"))
then echo "syntaxe : creexp.bas NomServeur EXP NomFicErreur"
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
else FicErr="creexp.err"
fi
if test -r $FicErr
then rm $FicErr
fi


####################
# Creation des types 
####################

cat exp.use cretyp.bas | isql -U MIGRAZUR  -P MIGRAZUR -S $NomServeur >> $FicErr

# Test si un probleme est survenu lors de la creation de type
grep "Msg" $FicErr | wc -l | read NbLignes ResteInfo
if ((NbLignes!=0))
then exit 0
fi
rm $FicErr


#########################
# Creation des tables et des index 
#########################

echo "FMC_TRT" >> $FicErr
# nom table : A_TRAITER, code table : FMC_TRT
cat exp.use tables/fmc_trt.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACCIDENT, code : FMC_ACC
echo "FMC_ACC" >> $FicErr
cat exp.use tables/fmc_acc.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACTION, code : ACT_GEN
echo "ACT_GEN" >> $FicErr
cat exp.use tables/act_gen.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACTIVATION, code : EQT_ACV
echo "EQT_ACV" >> $FicErr
cat exp.use tables/eqt_acv.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : AFFICHAGE_ECHANGEUR, code : ACT_ECH
echo "ACT_ECH" >> $FicErr
cat exp.use tables/act_ech.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : AFFICHAGE_EN_GARE, code : ACT_GAR
echo "ACT_GAR" >> $FicErr
cat exp.use tables/act_gar.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : AFFICHAGE_TUNNEL, code : ACT_TUB
echo "ACT_TUB" >> $FicErr
cat exp.use tables/act_tub.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ALARME, code : ALM_EQT
echo "ALM_EQT" >> $FicErr
cat exp.use tables/alm_eqt.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ALERTE, code : ALT_EXP
echo "ALT_EXP" >> $FicErr
cat exp.use tables/alt_exp.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : APPEL_TELEPHONIQUE_BIP_FAX, code : ACT_APL
echo "ACT_APL" >> $FicErr
cat exp.use tables/act_apl.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : BASCULEMENT, code : FMC_BAS
echo "FMC_BAS" >> $FicErr
cat exp.use tables/fmc_bas.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : CMPT_ALARME, code : ALM_NBR
echo "ALM_NBR" >> $FicErr
cat exp.use tables/alm_nbr.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : COMMENTAIRES, code : FMC_COM
echo "FMC_COM" >> $FicErr
cat exp.use tables/fmc_com.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : COMPTEUR_ACTION, code : FMC_ACT
echo "FMC_ACT" >> $FicErr
cat exp.use tables/fmc_act.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : CR_FAX, code : APL_FAX
echo "APL_FAX" >> $FicErr
cat exp.use tables/apl_fax.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : DELESTAGE, code : FMC_DEL
echo "FMC_DEL" >> $FicErr
cat exp.use tables/fmc_del.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : DEST_PA_FAX, code : SAD_DST_FAX
echo "SAD_DST_FAX" >> $FicErr
cat exp.use tables/sad_dst_fax.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : E_NAV, code : ACT_NAV
echo "ACT_NAV" >> $FicErr
cat exp.use tables/act_nav.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : E_PAL, code : ACT_PAL
echo "ACT_PAL" >> $FicErr
cat exp.use tables/act_pal.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : E_PMV, code : ACT_PMV
echo "ACT_PMV" >> $FicErr
cat exp.use tables/act_pmv.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : EVENEMENT, code : FMC_GEN
echo "FMC_GEN" >> $FicErr
cat exp.use tables/fmc_gen.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : EVENEMENT_HISTORIQUE, code : FMC_HIS
echo "FMC_HIS" >> $FicErr
cat exp.use tables/fmc_his.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FERMETURE_ECHANGEUR, code : FMC_ECH
echo "FMC_ECH" >> $FicErr
cat exp.use tables/fmc_ech.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FILE_D_ATTENTE_EQUIPEMENT, code : EQT_FIL
echo "EQT_FIL" >> $FicErr
cat exp.use tables/eqt_fil.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FILE_D_ATTENTE_TRAVAIL, code : EQT_FIL_TRV
echo "EQT_FIL_TRV" >> $FicErr
cat exp.use tables/eqt_fil_trv.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : INDISPONIBILITE, code : EQT_DSP
echo "EQT_DSP" >> $FicErr
cat exp.use tables/eqt_dsp.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : INTERVENTION, code : ACT_INT
echo "ACT_INT" >> $FicErr
cat exp.use tables/act_int.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : INTERVENTION_DEPANNEUR, code : ACT_DEP
echo "ACT_DEP" >> $FicErr
cat exp.use tables/act_dep.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : INTERVENTION_PATROUILLE, code ACT_SIS
echo "ACT_SIS" >> $FicErr
cat exp.use tables/act_sis.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : MANIFESTATION, code : FMC_MNF
echo "FMC_MNF" >> $FicErr
cat exp.use tables/fmc_mnf.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : MESSAGE_TRAFIC_FM, code : ACT_TFM
echo "ACT_TFM" >> $FicErr
cat exp.use tables/act_tfm.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : MESURE_TRAFIC_6_MINUTES, code : MES_TRA
echo "MES_TRA" >> $FicErr
cat exp.use tables/mes_tra.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : MESURES_POIDS, code : MES_POI
echo "MES_POI" >> $FicErr
cat exp.use tables/mes_poi.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr
# nom table : , code : MES_KCS
echo "MES_KCS" >> $FicErr
cat exp.use tables/mes_kcs.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr
# nom table : MESURE_TRAFIC_6_MINUTES, code : MES_NEC
echo "MES_NEC" >> $FicErr
cat exp.use tables/mes_nec.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr
# nom table : MESURE_TRAFIC_6_MINUTES, code : MES_QPC
echo "MES_QPC" >> $FicErr
cat exp.use tables/mes_qpc.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : METEO_HISTORIQUE, code : FMC_MET
echo "FMC_MET" >> $FicErr
cat exp.use tables/fmc_met.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : NIVEAU_TRAFIC, code : MES_NIV
echo "MES_NIV" >> $FicErr
cat exp.use tables/mes_niv.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : PA_APPEL, code : SAD_APL
echo "SAD_APL" >> $FicErr
cat exp.use tables/sad_apl.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : PA_ECHANGEUR, code : SAD_ECH
echo "SAD_ECH" >> $FicErr
cat exp.use tables/sad_ech.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : PA_FAX, code : SAD_FAX
echo "SAD_FAX" >> $FicErr
cat exp.use tables/sad_fax.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : PA_MES_TFM, code : SAD_MSG_TFM
echo "SAD_MSG_TFM" >> $FicErr
cat exp.use tables/sad_msg_tfm.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR ->> $FicErr

# nom table : PA_NAV, code : SAD_NAV
echo "SAD_NAV" >> $FicErr
cat exp.use tables/sad_nav.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : PA_PMV, code : SAD_PMV
echo "SAD_PMV" >> $FicErr
cat exp.use tables/sad_pmv.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : PA_TFM, code : SAD_TFM
echo "SAD_TFM" >> $FicErr
cat exp.use tables/sad_tfm.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : PA_TUNNEL, code : SAD_TUB
echo "SAD_TUB" >> $FicErr
cat exp.use tables/sad_tub.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : PLAN_D_ACTION, code : SAD_PLN
echo "SAD_PLN" >> $FicErr
cat exp.use tables/sad_pln.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : PMV_REMORQUE, code : ACT_RMQ
echo "ACT_RMQ" >> $FicErr
cat exp.use tables/act_rmq.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : PMV_SUR_FOURGON, code : ACT_FRG
echo "ACT_FRG" >> $FicErr
cat exp.use tables/act_frg.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : POSTE_OPERATEUR, code :FMC_OPR
echo "FMC_OPR" >> $FicErr
cat exp.use tables/fmc_opr.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : QUEUE_BOUCHON, code : FMC_BOU
echo "FMC_BOU" >> $FicErr
cat exp.use tables/fmc_bou.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : REVEIL, code : FMC_REV, code : FMC_REV
echo "FMC_REV" >> $FicErr
cat exp.use tables/fmc_rev.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SS_CONCESSIONNAIRE, code : FMC_CNC
echo "FMC_CNC" >> $FicErr
cat exp.use tables/fmc_cnc.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

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

# nom table : TEXTE_FAX, code : ACT_FAX
echo "ACT_FAX" >> $FicErr
cat exp.use tables/act_fax.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : TEXTE_PA_FAX, code : SAD_TXT_FAX
echo "SAD_TXT_FAX" >> $FicErr
cat exp.use tables/sad_txt_fax.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : TEXTE_PA_TFM, code : SAD_TXT_TFM
echo "SAD_TXT_TFM" >> $FicErr
cat exp.use tables/sad_txt_tfm.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : TFM_PC, code : FMC_TFM
 echo "FMC_TFM" >> $FicErr
cat exp.use tables/fmc_tfm.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : TRAVAUX, code : FMC_TRF
echo "FMC_TRF" >> $FicErr
cat exp.use tables/fmc_trf.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : TRAVAUX_HISTORIQUE, code : FMC_TRH
echo "FMC_TRH" >> $FicErr
cat exp.use tables/fmc_trh.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : VEHICULE, code : FMC_VEH
echo "FMC_VEH" >> $FicErr
cat exp.use tables/fmc_veh.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : VEHICULES_LENTS_HISTORIQUE, code : FMC_VLN
echo "FMC_VLN" >> $FicErr
cat exp.use tables/fmc_vln.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FMC_A_TRANSFERER, code : FMC_TRS
echo "FMC_TRS" >> $FicErr
cat exp.use tables/fmc_trs.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACT_TXT_TFM, code : ACT_TXT_TFM
echo "ACT_TXT_TFM" >> $FicErr
cat exp.use tables/act_txt_tfm.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : VOI_DAI, code : VOI_DAI
echo "VOI_DAI" >> $FicErr
cat exp.use tables/voi_dai.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : VOI_RAD, code : VOI_RAD
echo "VOI_RAD" >> $FicErr
cat exp.use tables/voi_rad.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FMC_SAT, code : FMC_SAT
echo "FMC_SAT" >> $FicErr
cat exp.use tables/fmc_sat.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FMC_SYN, code : FMC_SYN
echo "FMC_SYN" >> $FicErr
cat exp.use tables/fmc_syn.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : COMPTEUR_ACTION, code : FMC_ACT
echo "FMC_ACT" >> $FicErr
cat exp.use tables/fmc_act.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FRN_TDP, code : FRN_TDP
echo "FRN_TDP" >> $FicErr
cat exp.use tables/frn_tdp.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ZDP_6MN, code : ZDP_6MN
echo "ZDP_6MN" >> $FicErr
cat exp.use tables/zdp_6mn.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : LST_CPT, code : LST_CPT
echo "LST_CPT" >> $FicErr
cat exp.use tables/lst_cpt.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FMC_DAT, code : FMC_DAT
echo "FMC_DAT" >> $FicErr
cat exp.use tables/fmc_dat.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACT_EXP_ADA, code : ACT_EXP_ADA
echo "ACT_EXP_ADA" >> $FicErr
cat exp.use tables/act_exp_ada.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACT_TFM_EXP, code : ACT_TFM_EXP
echo "ACT_TFM_EXP" >> $FicErr
cat exp.use tables/act_tfm_exp.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACT_PMV_ADA, code : ACT_PMV_ADA
echo "ACT_PMV_ADA" >> $FicErr
cat exp.use tables/act_pmv_ada.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr




if [[ $NomServeur = "CI" ]]
then
# nom table : FMC_PUR, code : FMC_PUR
echo "FMC_PUR" >> $FicErr
cat exp.use tables/fmc_pur.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

##### evenement strada

# nom table : FMC_ADA, code : FMC_ADA
echo "FMC_ADA" >> $FicErr
cat exp.use tables/fmc_ada.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ADA_SIT, code : ADA_SIT
echo "ADA_SIT" >> $FicErr
cat exp.use tables/ada_sit.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ADA_TXT_SYN, code : ADA_TXT_SYN
echo "ADA_TXT_SYN" >> $FicErr
cat exp.use tables/ada_txt_syn.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : PMV_ADA, code : PMV_ADA
echo "PMV_ADA" >> $FicErr
cat exp.use tables/pmv_ada.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : TFM_ADA, code : TFM_ADA
echo "TFM_ADA" >> $FicErr
cat exp.use tables/tfm_ada.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SAD_EXP, code : SAD_EXP
echo "SAD_EXP" >> $FicErr
cat exp.use tables/sad_exp.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : MES_ADA, code : MES_ADA
echo "MES_ADA" >> $FicErr
cat exp.use tables/mes_ada.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr


# nom table : MES_NIV_DER, code : MES_NIV_DER
echo "MES_NIV_DER" >> $FicErr
cat exp.use tables/mes_niv_der.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

fi


# Test si un probleme est survenu lors de la creation de table
grep "Msg" $FicErr | wc -l | read NbLignes ResteInfo
if ((NbLignes!=0))
then exit 0
fi
rm $FicErr


###################
# Creation des cles 
###################

# nom table : A_TRAITER, code table : FMC_TRT
echo "FMC_TRT" >> $FicErr
cat exp.use tables/fmc_trt.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACCIDENT, code : FMC_ACC
echo "FMC_ACC" >> $FicErr
cat exp.use tables/fmc_acc.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACTION, code : ACT_GEN
echo "ACT_GEN" >> $FicErr
cat exp.use tables/act_gen.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACTIVATION, code : EQT_ACV
echo "EQT_ACV" >> $FicErr
cat exp.use tables/eqt_acv.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : AFFICHAGE_ECHANGEUR, code : ACT_ECH
echo "ACT_ECH" >> $FicErr
cat exp.use tables/act_ech.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : AFFICHAGE_EN_GARE, code : ACT_GAR
echo "ACT_GAR" >> $FicErr
cat exp.use tables/act_gar.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : AFFICHAGE_TUNNEL, code : ACT_TUB
echo "ACT_TUB" >> $FicErr
cat exp.use tables/act_tub.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ALARME, code : ALM_EQT
echo "ALM_EQT" >> $FicErr
cat exp.use tables/alm_eqt.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ALERTE, code : ALT_EXP
echo "ALT_EXP" >> $FicErr
cat exp.use tables/alt_exp.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : APPEL_TELEPHONIQUE_BIP_FAX, code : ACT_APL
echo "ACT_APL" >> $FicErr
cat exp.use tables/act_apl.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : BASCULEMENT, code : FMC_BAS
echo "FMC_BAS" >> $FicErr
cat exp.use tables/fmc_bas.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : CMPT_ALARME, code : ALM_NBR
echo "ALM_NBR" >> $FicErr
cat exp.use tables/alm_nbr.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : COMMENTAIRES, code : FMC_COM
echo "FMC_COM" >> $FicErr
cat exp.use tables/fmc_com.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr


# nom table : CR_FAX, code : APL_FAX
echo "APL_FAX" >> $FicErr
cat exp.use tables/apl_fax.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : DELESTAGE, code : FMC_DEL
echo "FMC_DEL" >> $FicErr
cat exp.use tables/fmc_del.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : DEST_PA_FAX, code : SAD_DST_FAX
echo "SAD_DST_FAX" >> $FicErr
cat exp.use tables/sad_dst_fax.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : E_NAV, code : ACT_NAV
echo "ACT_NAV" >> $FicErr
cat exp.use tables/act_nav.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : E_PAL, code : ACT_PAL
echo "ACT_PAL" >> $FicErr
cat exp.use tables/act_pal.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : E_PMV, code : ACT_PMV
echo "ACT_PMV" >> $FicErr
cat exp.use tables/act_pmv.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : EVENEMENT, code : FMC_GEN
echo "FMC_GEN" >> $FicErr
cat exp.use tables/fmc_gen.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : EVENEMENT_HISTORIQUE, code : FMC_HIS
echo "FMC_HIS" >> $FicErr
cat exp.use tables/fmc_his.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FERMETURE_ECHANGEUR, code : FMC_ECH
echo "FMC_ECH" >> $FicErr
cat exp.use tables/fmc_ech.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FILE_D_ATTENTE_EQUIPEMENT, code : EQT_FIL
echo "EQT_FIL" >> $FicErr
cat exp.use tables/eqt_fil.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FILE_D_ATTENTE_TRAVAIL, code : EQT_FIL_TRV
echo "EQT_FIL_TRV" >> $FicErr
cat exp.use tables/eqt_fil_trv.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : INDISPONIBILITE, code : EQT_DSP
echo "EQT_DSP" >> $FicErr
cat exp.use tables/eqt_dsp.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : INTERVENTION, code : ACT_INT
echo "ACT_INT" >> $FicErr
cat exp.use tables/act_int.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : INTERVENTION_DEPANNEUR, code : ACT_DEP
echo "ACT_DEP" >> $FicErr
cat exp.use tables/act_dep.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : INTERVENTION_PATROUILLE, code ACT_SIS
echo "ACT_SIS" >> $FicErr
cat exp.use tables/act_sis.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : MANIFESTATION, code : FMC_MNF
echo "FMC_MNF" >> $FicErr
cat exp.use tables/fmc_mnf.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : MESSAGE_TRAFIC_FM, code : ACT_TFM
echo "ACT_TFM" >> $FicErr
cat exp.use tables/act_tfm.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : MESURE_TRAFIC_6_MINUTES, code : MES_TRA
echo "MES_TRA" >> $FicErr
cat exp.use tables/mes_tra.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : MESURES_POIDS, code : MES_POI
echo "MES_POI" >> $FicErr
cat exp.use tables/mes_poi.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : METEO_HISTORIQUE, code : FMC_MET
echo "FMC_MET" >> $FicErr
cat exp.use tables/fmc_met.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : NIVEAU_TRAFIC, code : MES_NIV
echo "MES_NIV" >> $FicErr
cat exp.use tables/mes_niv.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : PA_APPEL, code : SAD_APL
echo "SAD_APL" >> $FicErr
cat exp.use tables/sad_apl.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : PA_ECHANGEUR, code : SAD_ECH
echo "SAD_ECH" >> $FicErr
cat exp.use tables/sad_ech.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : PA_FAX, code : SAD_FAX
echo "SAD_FAX" >> $FicErr
cat exp.use tables/sad_fax.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : PA_MES_TFM, code : SAD_MSG_TFM
echo "SAD_MSG_TFM" >> $FicErr
cat exp.use tables/sad_msg_tfm.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR ->> $FicErr

# nom table : PA_NAV, code : SAD_NAV
echo "SAD_NAV" >> $FicErr
cat exp.use tables/sad_nav.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : PA_PMV, code : SAD_PMV
echo "SAD_PMV" >> $FicErr
cat exp.use tables/sad_pmv.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : PA_TFM, code : SAD_TFM
echo "SAD_TFM" >> $FicErr
cat exp.use tables/sad_tfm.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : PA_TUNNEL, code : SAD_TUB
echo "SAD_TUB" >> $FicErr
cat exp.use tables/sad_tub.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : PLAN_D_ACTION, code : SAD_PLN
echo "SAD_PLN" >> $FicErr
cat exp.use tables/sad_pln.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : PMV_REMORQUE, code : ACT_RMQ
echo "ACT_RMQ" >> $FicErr
cat exp.use tables/act_rmq.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : PMV_SUR_FOURGON, code : ACT_FRG
echo "ACT_FRG" >> $FicErr
cat exp.use tables/act_frg.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : POSTE_OPERATEUR, code :FMC_OPR
echo "FMC_OPR" >> $FicErr
cat exp.use tables/fmc_opr.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : QUEUE_BOUCHON, code : FMC_BOU
echo "FMC_BOU" >> $FicErr
cat exp.use tables/fmc_bou.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : REVEIL, code : FMC_REV, code : FMC_REV
echo "FMC_REV" >> $FicErr
cat exp.use tables/fmc_rev.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SS_CONCESSIONNAIRE, code : FMC_CNC
echo "FMC_CNC" >> $FicErr
cat exp.use tables/fmc_cnc.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

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

# nom table : TEXTE_FAX, code : ACT_FAX
echo "ACT_FAX" >> $FicErr
cat exp.use tables/act_fax.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : TEXTE_PA_FAX, code : SAD_TXT_FAX
echo "SAD_TXT_FAX" >> $FicErr
cat exp.use tables/sad_txt_fax.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : TEXTE_PA_TFM, code : SAD_TXT_TFM
echo "SAD_TXT_TFM" >> $FicErr
cat exp.use tables/sad_txt_tfm.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : TFM_PC, code : FMC_TFM
echo "FMC_TFM" >> $FicErr
cat exp.use tables/fmc_tfm.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : TRAVAUX, code : FMC_TRF
echo "FMC_TRF" >> $FicErr
cat exp.use tables/fmc_trf.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : TRAVAUX_HISTORIQUE, code : FMC_TRH
echo "FMC_TRH" >> $FicErr
cat exp.use tables/fmc_trh.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : VEHICULE, code : FMC_VEH
echo "FMC_VEH" >> $FicErr
cat exp.use tables/fmc_veh.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : VEHICULES_LENTS_HISTORIQUE, code : FMC_VLN
echo "FMC_VLN" >> $FicErr
cat exp.use tables/fmc_vln.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACT_TXT_TFM, code : ACT_TXT_TFM
echo "ACT_TXT_TFM" >> $FicErr
cat exp.use tables/act_txt_tfm.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FRN_TDP, code : FRN_TDP
echo "FRN_TDP" >> $FicErr
cat exp.use tables/frn_tdp.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ZDP_6MN, code : ZDP_6MN
echo "ZDP_6MN" >> $FicErr
cat exp.use tables/zdp_6mn.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr


# nom table : FMC_DAT, code : FMC_DAT
echo "FMC_DAT" >> $FicErr
cat exp.use tables/fmc_dat.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACT_EXP_ADA, code : ACT_EXP_ADA
echo "ACT_EXP_ADA" >> $FicErr
cat exp.use tables/act_exp_ada.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACT_TFM_EXP, code : ACT_TFM_EXP
echo "ACT_TFM_EXP" >> $FicErr
cat exp.use tables/act_tfm_exp.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACT_PMV_ADA, code : ACT_PMV_ADA
echo "ACT_PMV_ADA" >> $FicErr
cat exp.use tables/act_pmv_ada.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr


if [[ $NomServeur = "CI" ]]
then
##### evenement strada

# nom table : FMC_ADA, code : FMC_ADA
echo "FMC_ADA" >> $FicErr
cat exp.use tables/fmc_ada.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ADA_SIT, code : ADA_SIT
echo "ADA_SIT" >> $FicErr
cat exp.use tables/ada_sit.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ADA_TXT_SYN, code : ADA_TXT_SYN
echo "ADA_TXT_SYN" >> $FicErr
cat exp.use tables/ada_txt_syn.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : PMV_ADA, code : PMV_ADA
echo "PMV_ADA" >> $FicErr
cat exp.use tables/pmv_ada.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : TFM_ADA, code : TFM_ADA
echo "TFM_ADA" >> $FicErr
cat exp.use tables/tfm_ada.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SAD_EXP, code : SAD_EXP
echo "SAD_EXP" >> $FicErr
cat exp.use tables/sad_exp.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : MES_ADA, code : MES_ADA
echo "MES_ADA" >> $FicErr
cat exp.use tables/mes_ada.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr


# nom table : MES_NIV_DER, code : MES_NIV_DER
echo "MES_NIV_DER" >> $FicErr
cat exp.use tables/mes_niv_der.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr


fi

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
