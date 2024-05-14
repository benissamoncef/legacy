#!/bin/ksh
# Fichier : $Id: crehis.bas,v 1.9 2007/07/02 11:56:11 gesconf Exp $        Release : $Revision: 1.9 $        Date : $Date: 2007/07/02 11:56:11 $
##################################################################################
# STERIA # PROJET MIGRAZUR
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# SCRIPT crehis.bas
##################################################################################
# DESCRIPTION DU SCRIPT :
# creation des tables, des index, des cles et des triggers de la base HIS ou SVG
#
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# C.T.	09/08/94	: Creation			 (V 1.1)
# C.T.	02/11/94	: Ajout creation de act_txt_tfm  (V 1.2)
# T.F.	27/12/94	: Ajout creation de sys_inq  (V 1.3)
# C.T.	03/05/96	: Supprimer la table MES_NIV (V 1.4)
# P.N   25/11/96	: Ajout de mes_met,mes_kcs,mes_qpc,mes_nec (v1.5)
# P.N   21/02/97	: Ajout de zdp_heu (v1.6)
# C.T.	25/02/98	: Ajout ADA_FMC (1.7)
* PN	30/09/98	: correction 1.8
# PN	22/03/99	: Ajout FMC_DAT,ACT_EXP_ADA,
			ACT_TFM_EXP, ACT_PMV_ADA (1.9)
##################################################################################

#Creation des tables de la base HIS ou SVG

# controle des parametres d'appels
if ((($#<2)) || (test $2 != "HIS" && test $2 != "SVG"))
then echo "syntaxe : crehis.bas NomServeur NomBase NomFicErreur"
     echo "          NomBase : HIS ou SVG"
     echo "          NomFicErreur optionnel"
     exit 0
fi

# serveur a utiliser
NomServeur=$1

# nom de la base
NomBase=$2
if test $NomBase = "HIS"
then Use="his.use"
else Use="svg.use"
fi

# fichier de compte rendu d'erreur
if (($#==3))
then FicErr=${3}.err
else FicErr="crehis.err"
fi


################
# Creation des types 
################

cat $Use cretyp.bas | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# Test si un probleme est survenu lors de la creation de type
grep "Msg" $FicErr | wc -l | read NbLignes ResteInfo
if ((NbLignes!=0))
then exit 0
fi
rm $FicErr


#############################################
# Creation des tables et des index de la base
#############################################

# nom table : ACCIDENT, code : FMC_ACC
echo "FMC_ACC" >> $FicErr
cat $Use tables/fmc_acc.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACTION, code : ACT_GEN
echo "ACT_GEN" >> $FicErr
cat $Use tables/act_gen.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : AFFICHAGE_ECHANGEUR, code : ACT_ECH
echo "ACT_ECH" >> $FicErr
cat $Use tables/act_ech.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : AFFICHAGE_EN_GARE, code : ACT_GAR
echo "ACT_GAR" >> $FicErr
cat $Use tables/act_gar.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : AFFICHAGE_TUNNEL, code : ACT_TUB
echo "ACT_TUB" >> $FicErr
cat $Use tables/act_tub.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ALERTE, code : ALT_EXP
echo "ALT_EXP" >> $FicErr
cat $Use tables/alt_exp.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : APPEL_HORAIRE, code : APL_HOR
echo "APL_HOR" >> $FicErr
cat $Use tables/apl_hor.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : APPEL_TELEPHONIQUE_BIP_FAX, code : ACT_APL
echo "ACT_APL" >> $FicErr
cat $Use tables/act_apl.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : BASCULEMENT, code : FMC_BAS
echo "FMC_BAS" >> $FicErr
cat $Use tables/fmc_bas.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : COMMENTAIRES, code : FMC_COM
echo "FMC_COM" >> $FicErr
cat $Use tables/fmc_com.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : DELESTAGE, code : FMC_DEL
echo "FMC_DEL" >> $FicErr
cat $Use tables/fmc_del.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : E_NAV, code : ACT_NAV
echo "ACT_NAV" >> $FicErr
cat $Use tables/act_nav.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : E_PMV, code : ACT_PMV
echo "ACT_PMV" >> $FicErr
cat $Use tables/act_pmv.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : EVENEMENT, code : FMC_GEN
echo "FMC_GEN" >> $FicErr
cat $Use tables/fmc_gen.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : EVENEMENT_HISTORIQUE, code : FMC_HIS
echo "FMC_HIS" >> $FicErr
cat $Use tables/fmc_his.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FERMETURE_ECHANGEUR, code : FMC_ECH
echo "FMC_ECH" >> $FicErr
cat $Use tables/fmc_ech.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : INDISPONIBILITE, code : EQT_DSP
echo "EQT_DSP" >> $FicErr
cat $Use tables/eqt_dsp.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : INTERVENTION, code : ACT_INT
echo "ACT_INT" >> $FicErr
cat $Use tables/act_int.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : INTERVENTION_DEPANNEUR, code : ACT_DEP
echo "ACT_DEP" >> $FicErr
cat $Use tables/act_dep.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : INTERVENTION_PATROUILLE, code ACT_SIS
echo "ACT_SIS" >> $FicErr
cat $Use tables/act_sis.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : MANIFESTATION, code : FMC_MNF
echo "FMC_MNF" >> $FicErr
cat $Use tables/fmc_mnf.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : MESSAGE_TRAFIC_FM, code : ACT_TFM
echo "ACT_TFM" >> $FicErr
cat $Use tables/act_tfm.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : TEXTE_TRAFIC_FM, code : ACT_TXT_TFM
echo "ACT_TXT_TFM" >> $FicErr
cat $Use tables/act_txt_tfm.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : MESURE_TRAFIC_6_MINUTES, code : MES_TRA
echo "MES_TRA" >> $FicErr
cat $Use tables/mes_tra.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : MESURES_POIDS, code : MES_POI
echo "MES_POI" >> $FicErr
cat $Use tables/mes_poi.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : MES_KCS
echo "MES_KCS" >> $FicErr
cat $Use tables/mes_kcs.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : MES_QPC
echo "MES_QPC" >> $FicErr
cat $Use tables/mes_qpc.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : MES_NEC
echo "MES_NEC" >> $FicErr
cat $Use tables/mes_nec.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : MES_MET
echo "MES_MET" >> $FicErr
cat $Use tables/mes_met.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : METEO_HISTORIQUE, code : FMC_MET
echo "FMC_MET" >> $FicErr
cat $Use tables/fmc_met.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : PMV_REMORQUE, code : ACT_RMQ
echo "ACT_RMQ" >> $FicErr
cat $Use tables/act_rmq.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : PMV_SUR_FOURGON, code : ACT_FRG
echo "ACT_FRG" >> $FicErr
cat $Use tables/act_frg.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : POSTE_OPERATEUR, code :FMC_OPR
echo "FMC_OPR" >> $FicErr
cat $Use tables/fmc_opr.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : QUEUE_BOUCHON, code : FMC_BOU
echo "FMC_BOU" >> $FicErr
cat $Use tables/fmc_bou.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SS_CONCESSIONNAIRE, code : FMC_CNC
echo "FMC_CNC" >> $FicErr
cat $Use tables/fmc_cnc.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : TEXTE_FAX, code : ACT_FAX
echo "ACT_FAX" >> $FicErr
cat $Use tables/act_fax.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : TRAVAUX, code : FMC_TRF
echo "FMC_TRF" >> $FicErr
cat $Use tables/fmc_trf.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : TRAVAUX_HISTORIQUE, code : FMC_TRH
echo "FMC_TRH" >> $FicErr
cat $Use tables/fmc_trh.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : VEHICULE, code : FMC_VEH
echo "FMC_VEH" >> $FicErr
cat $Use tables/fmc_veh.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : VEHICULES_LENTS_HISTORIQUE, code : FMC_VLN
echo "FMC_VLN" >> $FicErr
cat $Use tables/fmc_vln.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : INDICE QUALITE SYSTEME, code : SYS_INQ
echo "SYS_INQ" >> $FicErr
cat $Use tables/sys_inq.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ZDP_HEU, code : ZDP_HEU
echo "ZDP_HEU" >> $FicErr
cat $Use tables/zdp_heu.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

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


#############################################
# Test si un probleme est survenu lors de la creation de table
#############################################
grep "Msg" $FicErr | wc -l | read NbLignes ResteInfo
if ((NbLignes!=0))
then exit 0
fi
rm $FicErr


##############
# Creation des cles 
##############

# nom table : ACCIDENT, code : FMC_ACC
echo "FMC_ACC" >> $FicErr
cat $Use tables/fmc_acc.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACTION, code : ACT_GEN
echo "ACT_GEN" >> $FicErr
cat $Use tables/act_gen.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : AFFICHAGE_ECHANGEUR, code : ACT_ECH
echo "ACT_ECH" >> $FicErr
cat $Use tables/act_ech.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : AFFICHAGE_EN_GARE, code : ACT_GAR
echo "ACT_GAR" >> $FicErr
cat $Use tables/act_gar.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : AFFICHAGE_TUNNEL, code : ACT_TUB
echo "ACT_TUB" >> $FicErr
cat $Use tables/act_tub.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ALERTE, code : ALT_EXP
echo "ALT_EXP" >> $FicErr
cat $Use tables/alt_exp.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : APPEL_HORAIRE, code : APL_HOR
echo "APL_HOR" >> $FicErr
cat $Use tables/apl_hor.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : APPEL_TELEPHONIQUE_BIP_FAX, code : ACT_APL
echo "ACT_APL" >> $FicErr
cat $Use tables/act_apl.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : BASCULEMENT, code : FMC_BAS
echo "FMC_BAS" >> $FicErr
cat $Use tables/fmc_bas.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : COMMENTAIRES, code : FMC_COM
echo "FMC_COM" >> $FicErr
cat $Use tables/fmc_com.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : DELESTAGE, code : FMC_DEL
echo "FMC_DEL" >> $FicErr
cat $Use tables/fmc_del.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : E_NAV, code : ACT_NAV
echo "ACT_NAV" >> $FicErr
cat $Use tables/act_nav.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : E_PMV, code : ACT_PMV
echo "ACT_PMV" >> $FicErr
cat $Use tables/act_pmv.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : EVENEMENT, code : FMC_GEN
echo "FMC_GEN" >> $FicErr
cat $Use tables/fmc_gen.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : EVENEMENT_HISTORIQUE, code : FMC_HIS
echo "FMC_HIS" >> $FicErr
cat $Use tables/fmc_his.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FERMETURE_ECHANGEUR, code : FMC_ECH
echo "FMC_ECH" >> $FicErr
cat $Use tables/fmc_ech.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : INDISPONIBILITE, code : EQT_DSP
echo "EQT_DSP" >> $FicErr
cat $Use tables/eqt_dsp.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : INTERVENTION, code : ACT_INT
echo "ACT_INT" >> $FicErr
cat $Use tables/act_int.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : INTERVENTION_DEPANNEUR, code : ACT_DEP
echo "ACT_DEP" >> $FicErr
cat $Use tables/act_dep.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : INTERVENTION_PATROUILLE, code ACT_SIS
echo "ACT_SIS" >> $FicErr
cat $Use tables/act_sis.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : MANIFESTATION, code : FMC_MNF
echo "FMC_MNF" >> $FicErr
cat $Use tables/fmc_mnf.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : MESSAGE_TRAFIC_FM, code : ACT_TFM
echo "ACT_TFM" >> $FicErr
cat $Use tables/act_tfm.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : TEXTE_TRAFIC_FM, code : ACT_TXT_TFM
echo "ACT_TXT_TFM" >> $FicErr
cat $Use tables/act_txt_tfm.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : MESURE_TRAFIC_6_MINUTES, code : MES_TRA
echo "MES_TRA" >> $FicErr
cat $Use tables/mes_tra.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : MESURES_POIDS, code : MES_POI
echo "MES_POI" >> $FicErr
cat $Use tables/mes_poi.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : METEO_HISTORIQUE, code : FMC_MET
echo "FMC_MET" >> $FicErr
cat $Use tables/fmc_met.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : PMV_REMORQUE, code : ACT_RMQ
echo "ACT_RMQ" >> $FicErr
cat $Use tables/act_rmq.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : PMV_SUR_FOURGON, code : ACT_FRG
echo "ACT_FRG" >> $FicErr
cat $Use tables/act_frg.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : POSTE_OPERATEUR, code :FMC_OPR
echo "FMC_OPR" >> $FicErr
cat $Use tables/fmc_opr.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : QUEUE_BOUCHON, code : FMC_BOU
echo "FMC_BOU" >> $FicErr
cat $Use tables/fmc_bou.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : SS_CONCESSIONNAIRE, code : FMC_CNC
echo "FMC_CNC" >> $FicErr
cat $Use tables/fmc_cnc.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : TEXTE_FAX, code : ACT_FAX
echo "ACT_FAX" >> $FicErr
cat $Use tables/act_fax.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : TRAVAUX, code : FMC_TRF
echo "FMC_TRF" >> $FicErr
cat $Use tables/fmc_trf.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : TRAVAUX_HISTORIQUE, code : FMC_TRH
echo "FMC_TRH" >> $FicErr
cat $Use tables/fmc_trh.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : VEHICULE, code : FMC_VEH
echo "FMC_VEH" >> $FicErr
cat $Use tables/fmc_veh.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : VEHICULES_LENTS_HISTORIQUE, code : FMC_VLN
echo "FMC_VLN" >> $FicErr
cat $Use tables/fmc_vln.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ZDP_HEU, code : ZDP_HEU
echo "ZDP_HEU" >> $FicErr
cat $Use tables/zdp_heu.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr


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


#############################################
# Test si un probleme est survenu lors de la creation de table
#############################################
grep "Msg" $FicErr | wc -l | read NbLignes ResteInfo
if ((NbLignes!=0))
then exit 0
fi
rm $FicErr

#################
# Creation des triggers
#################


exit 1
