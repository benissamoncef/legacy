#!/bin/ksh
# Fichier : crevue.bas
##################################################################################
# STERIA # PROJET MIGRAZUR
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# SCRIPT crevue.bas
##################################################################################
# DESCRIPTION DU SCRIPT :
# creation des tables, des index, des cles et des triggers de la base VUE
#
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# L.V.	21/08/96	: Creation
##################################################################################

# Creation des tables de la base VUE

# controle des parametres d'appels
if ((($#<2)) || (test $2 != "VUE"))
then echo "syntaxe : crevue.bas NomServeur VUE NomFicErreur"
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
else FicErr="crevue.err"
fi
if test -r $FicErr
then rm $FicErr
fi


####################
# Creation des types 
####################

cat vue.use cretyp.bas | isql -U MIGRAZUR  -P MIGRAZUR -S $NomServeur >> $FicErr

# Test si un probleme est survenu lors de la creation de type
grep "Msg" $FicErr | wc -l | read NbLignes ResteInfo
if ((NbLignes!=0))
then exit 0
fi
rm $FicErr


#########################
# Creation des tables et des index et des vues
#########################

# nom table : DICO, code : DICO
echo "DICO" >> $FicErr
cat vue.use tables/dico.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom vue : fmc 
echo "FMC" >> $FicErr
cat vue.use vues/fmc.vue | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom vue : fmc_acc 
echo "FMC_ACC" >> $FicErr
cat vue.use vues/fmc_acc.vue | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom vue : fmc_veh 
echo "FMC_VEH" >> $FicErr
cat vue.use vues/fmc_veh.vue | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom vue : fmc_com 
echo "FMC_COM" >> $FicErr
cat vue.use vues/fmc_com.vue | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom vue : fmc_opr 
echo "FMC_OPR" >> $FicErr
cat vue.use vues/fmc_opr.vue | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom vue : act 
echo "ACT" >> $FicErr
cat vue.use vues/act.vue | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom vue : act_apl 
echo "ACT_APL" >> $FicErr
cat vue.use vues/act_apl.vue | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom vue : act_bra 
echo "ACT_BRA" >> $FicErr
cat vue.use vues/act_bra.vue | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom vue : act_depan 
echo "ACT_DEPAN" >> $FicErr
cat vue.use vues/act_depan.vue | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom vue : act_ech 
echo "ACT_ECH" >> $FicErr
cat vue.use vues/act_ech.vue | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom vue : act_fax 
echo "ACT_FAX" >> $FicErr
cat vue.use vues/act_fax.vue | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom vue : act_fourgon 
echo "ACT_FOURGON" >> $FicErr
cat vue.use vues/act_fourgon.vue | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom vue : act_gare 
echo "ACT_GARE" >> $FicErr
cat vue.use vues/act_gare.vue | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom vue : act_interv 
echo "ACT_INTERV" >> $FicErr
cat vue.use vues/act_interv.vue | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom vue : act_pmv 
echo "ACT_PMV" >> $FicErr
cat vue.use vues/act_pmv.vue | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom vue : act_remorq 
echo "ACT_REMORQ" >> $FicErr
cat vue.use vues/act_remorq.vue | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom vue : act_sis 
echo "ACT_SIS" >> $FicErr
cat vue.use vues/act_sis.vue | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom vue : act_tfm 
echo "ACT_TFM" >> $FicErr
cat vue.use vues/act_tfm.vue | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom vue : act_tun 
echo "ACT_TUN" >> $FicErr
cat vue.use vues/act_tun.vue | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom vue : eqt 
echo "EQT" >> $FicErr
cat vue.use vues/eqt.vue | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom vue : eqt_bra 
echo "EQT_BRA" >> $FicErr
cat vue.use vues/eqt_bra.vue | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom vue : eqt_camera 
echo "EQT_CAMERA" >> $FicErr
cat vue.use vues/eqt_camera.vue | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom vue : eqt_camera_dai 
echo "EQT_CAMERA_DAI" >> $FicErr
cat vue.use vues/eqt_camera_dai.vue | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom vue : eqt_pmv 
echo "EQT_PMV" >> $FicErr
cat vue.use vues/eqt_pmv.vue | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom vue : eqt_tunnel 
echo "EQT_TUNNEL" >> $FicErr
cat vue.use vues/eqt_tunnel.vue | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom vue : inf_aire 
echo "INF_AIRE" >> $FicErr
cat vue.use vues/inf_aire.vue | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom vue : inf_ech 
echo "INF_ECH" >> $FicErr
cat vue.use vues/inf_ech.vue | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom vue : inf_itpc 
echo "INF_ITPC" >> $FicErr
cat vue.use vues/inf_itpc.vue | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom vue : inf_passinf 
echo "INF_PASSINF" >> $FicErr
cat vue.use vues/inf_passinf.vue | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom vue : inf_peage 
echo "INF_PEAGE" >> $FicErr
cat vue.use vues/inf_peage.vue | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom vue : inf_pente 
echo "INF_PENTE" >> $FicErr
cat vue.use vues/inf_pente.vue | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom vue : inf_pont 
echo "INF_PONT" >> $FicErr
cat vue.use vues/inf_pont.vue | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom vue : inf_portail 
echo "INF_PORTAIL" >> $FicErr
cat vue.use vues/inf_portail.vue | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom vue : inf_reseau 
echo "INF_RESEAU" >> $FicErr
cat vue.use vues/inf_reseau.vue | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom vue : inf_seg 
echo "INF_SEG" >> $FicErr
cat vue.use vues/inf_seg.vue | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom vue : inf_vitesse 
echo "INF_VITESSE" >> $FicErr
cat vue.use vues/inf_vitesse.vue | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom vue : lim_vit
echo "LIM_VIT" >> $FicErr
cat vue.use vues/lim_vit.vue | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr


# Test si un probleme est survenu lors de la creation de la table ou des vues 
grep "Msg" $FicErr | wc -l | read NbLignes ResteInfo
if ((NbLignes!=0))
then exit 0
fi
rm $FicErr



###################
# Creation des cles 
###################

# nom table : DICO, code : DICO
echo "DICO" >> $FicErr
cat vue.use tables/dico.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr


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
