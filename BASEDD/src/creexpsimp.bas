#!/bin/ksh
# Fichier : $Id: creexpsimp.bas,v 1.5 1998/01/20 17:36:31 gaborit Exp $      Release : $Revision: 1.5 $        Date : $Date: 1998/01/20 17:36:31 $
##################################################################################
# STERIA # PROJET MIGRAZUR
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# SCRIPT creexpsimp.bas
##################################################################################
# DESCRIPTION DU SCRIPT :
# creation des tables, des index, des cles et des triggers de la base EXP simplifiee
#
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# C.T.	27/08/96	: Creation
# C.T.	09/09/96	: Ajout FMC_PUR (RADT V1.2)
# P.N   28/10/96	: Ajout MES_KCS QPC et NEC v1.3
# JMG	08/01/98	: ajout ALT_EXP (dem/1532) 1.4
# C.T.	16/01/98	: Ajout ACT_PAL (V1.5)
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

# nom table : ACTION, code : ACT_GEN
echo "ACT_GEN" >> $FicErr
cat exp.use tables/act_gen.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACTIVATION, code : EQT_ACV
echo "EQT_ACV" >> $FicErr
cat exp.use tables/eqt_acv.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ALARME, code : ALM_EQT
echo "ALM_EQT" >> $FicErr
cat exp.use tables/alm_eqt.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : CMPT_ALARME, code : ALM_NBR
echo "ALM_NBR" >> $FicErr
cat exp.use tables/alm_nbr.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : COMMENTAIRES, code : FMC_COM
echo "FMC_COM" >> $FicErr
cat exp.use tables/fmc_com.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : COMPTEUR_ACTION, code : FMC_ACT
echo "FMC_ACT" >> $FicErr
cat exp.use tables/fmc_act.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

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

# nom table : INDISPONIBILITE, code : EQT_DSP
echo "EQT_DSP" >> $FicErr
cat exp.use tables/eqt_dsp.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : MESURE_TRAFIC_6_MINUTES, code : MES_TRA
echo "MES_TRA" >> $FicErr
cat exp.use tables/mes_tra.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : MESURES_POIDS, code : MES_POI
echo "MES_POI" >> $FicErr
cat exp.use tables/mes_poi.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : NIVEAU_TRAFIC, code : MES_NIV
echo "MES_NIV" >> $FicErr
cat exp.use tables/mes_niv.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : KCS, code : MES_KCS
echo "MES_KCS" >> $FicErr
cat exp.use tables/mes_kcs.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : QPC, code : MES_QPC
echo "MES_QPC" >> $FicErr
cat exp.use tables/mes_qpc.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : NEC, code : MES_NEC
echo "MES_NEC" >> $FicErr
cat exp.use tables/mes_nec.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : POSTE_OPERATEUR, code :FMC_OPR
echo "FMC_OPR" >> $FicErr
cat exp.use tables/fmc_opr.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : TRAVAUX, code : FMC_TRF
echo "FMC_TRF" >> $FicErr
cat exp.use tables/fmc_trf.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : TRAVAUX_HISTORIQUE, code : FMC_TRH
echo "FMC_TRH" >> $FicErr
cat exp.use tables/fmc_trh.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : FMC_PUR, code : FMC_PUR
echo "FMC_PUR" >> $FicErr
cat exp.use tables/fmc_pur.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ALT_EXP, code : ALT_EXP
echo "ALT_EXP" >> $FicErr
cat exp.use tables/alt_exp.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# Test si un probleme est survenu lors de la creation de table
grep "Msg" $FicErr | wc -l | read NbLignes ResteInfo
if ((NbLignes!=0))
then exit 0
fi
rm $FicErr


###################
# Creation des cles 
###################

# nom table : ACTION, code : ACT_GEN
echo "ACT_GEN" >> $FicErr
cat exp.use tables/act_gen.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ACTIVATION, code : EQT_ACV
echo "EQT_ACV" >> $FicErr
cat exp.use tables/eqt_acv.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ALARME, code : ALM_EQT
echo "ALM_EQT" >> $FicErr
cat exp.use tables/alm_eqt.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : CMPT_ALARME, code : ALM_NBR
echo "ALM_NBR" >> $FicErr
cat exp.use tables/alm_nbr.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : COMMENTAIRES, code : FMC_COM
echo "FMC_COM" >> $FicErr
cat exp.use tables/fmc_com.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : COMPTEUR_ACTION, code : FMC_ACT
echo "FMC_ACT" >> $FicErr
cat exp.use tables/fmc_act.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

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

# nom table : INDISPONIBILITE, code : EQT_DSP
echo "EQT_DSP" >> $FicErr
cat exp.use tables/eqt_dsp.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : MESURE_TRAFIC_6_MINUTES, code : MES_TRA
echo "MES_TRA" >> $FicErr
cat exp.use tables/mes_tra.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : MESURES_POIDS, code : MES_POI
echo "MES_POI" >> $FicErr
cat exp.use tables/mes_poi.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : NIVEAU_TRAFIC, code : MES_NIV
echo "MES_NIV" >> $FicErr
cat exp.use tables/mes_niv.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : KCS , code : MES_KCS
echo "MES_KCS" >> $FicErr
cat exp.use tables/mes_kcs.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : QPC , code : MES_QPC
echo "MES_QPC" >> $FicErr
cat exp.use tables/mes_qpc.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : NEC , code : MES_NEC
echo "MES_NEC" >> $FicErr
cat exp.use tables/mes_nec.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : POSTE_OPERATEUR, code :FMC_OPR
echo "FMC_OPR" >> $FicErr
cat exp.use tables/fmc_opr.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : TRAVAUX, code : FMC_TRF
echo "FMC_TRF" >> $FicErr
cat exp.use tables/fmc_trf.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : TRAVAUX_HISTORIQUE, code : FMC_TRH
echo "FMC_TRH" >> $FicErr
cat exp.use tables/fmc_trh.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table :  ALT_EXP, code : ALT_EXP
echo "ALT_EXP" >> $FicErr
cat exp.use tables/alt_exp.cle  | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

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
