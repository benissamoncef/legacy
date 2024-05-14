#!/bin/ksh
# Fichier : $Id: crecfg.bas,v 1.20 2007/07/02 13:56:39 gesconf Exp $        Release : $Revision: 1.20 $        Date : $Date: 2007/07/02 13:56:39 $
##################################################################################
# STERIA # PROJET MIGRAZUR
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# SCRIPT crecfg.bas
##################################################################################
# DESCRIPTION DU SCRIPT :
# creation des tables, des index, des cles et des triggers de la base CFG ou CFT
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# C.T.	09/08/94	: Creation
# T.F.	27/12/94	: Ajout table eqt_rad_vtl (1.3)
# C.T.	16/03/95	: Ajout table EQT_LTV_CAN (1.4)
# C.T.	10/08/95	: Ajout table EQT_ISI (1.5)
# C.T.	18/10/95	: Ajout table RES_AUT_INV (1.6)
# C.T.	18/10/95	: Ajout table EQT_FAX (1.7)
# C.T.	26/04/96	: Ajout table RES_NUM_ECH  (1.8)
# P.V.  18/06/96 	: Ajout tables LIB_TYP et LIB_PRE (1.9 et 1.10 et 1.11)
# D.M.  05/11/96 	: Ajout table EQT_LCR (DEM/1232) (1.12)
# D.M.  07/02/97	: Ajout de EQT_TDP, EQT_ZDP, RES_DTP (DEM/1395) (V1.13)
# C.T.	16/01/98	: Ajout table EQT_PAL (V1.14)
# C.T.	25/02/98	: Ajout table ADA_EVT, ADA_STA (V1.15)
# C.T.	25/02/98	: Ajout table NOM_RDS (V1.16)
# P.N	30/09/98	: Correction v1.18
# P.N	22/03/99	: Ajout de ADA_PMV NUM_RDS NOM_RDS PR_RDS RDS v1.19
# JPL	02/07/07	: Ajout de RES_CAN (DEM 657) v1.20
##################################################################################

# controle des parametres d'appels
if ((($#<2)) || (test $2 != "CFG" && test $2 != "CFT"))
then    echo "syntaxe : crecfg.bas NomServeur NomBase NomFicErreur"
	echo "          NomBase : CFG ou CFT"
	echo "          NomFicErreur optionnel"
	exit 0
fi
# serveur a utiliser
NomServeur=$1

# fichier de compte rendu d'erreur
if (($#==3))
then    FicErr=${3}.err
else    FicErr="crecfg.err"
fi
if test -r $FicErr
then rm $FicErr
fi

# fichier pour le positionnement de la base È utiliser
if test $2 = "CFG"
then Use="cfg.use"
else Use="cft.use"
fi


####################
# Creation des types 
####################

cat $Use cretyp.bas | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# Test si un probleme est survenu lors de la creation de type
grep "Msg" $FicErr | wc -l | read NbLignes ResteInfo
if ((NbLignes!=0))
then exit 0
fi
rm $FicErr


#####################
# Creation des tables
#####################

# nom table : AIRE, code : RES_AIR
echo "RES_AIR" >> $FicErr
cat $Use tables/res_air.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : ALARME_POSSIBLE, code : EQT_ALM
echo "EQT_ALM" >> $FicErr
cat $Use tables/eqt_alm.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : ANALYSEUR, code : EQT_ANA
echo "EQT_ANA" >> $FicErr
cat $Use tables/eqt_ana.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : AUTOROUTE, code : RES_AUT
echo "RES_AUT" >> $FicErr
cat $Use tables/res_aut.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : SENS_AUTOROUTE, code : RES_AUT_INV
echo "RES_AUT_INV" >> $FicErr
cat $Use tables/res_aut_inv.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : C_FAX, code : EQT_FAX
echo "EQT_FAX" >> $FicErr
cat $Use tables/eqt_fax.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : C_FAV, code : EQT_FAV
echo "EQT_FAV" >> $FicErr
cat $Use tables/eqt_fav.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : C_MODULE, code : EQT_MOD
echo "EQT_MOD" >> $FicErr
cat $Use tables/eqt_mod.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur>> $FicErr

# nom table : C_NAV, code : EQT_NAV
echo "EQT_NAV" >> $FicErr
cat $Use tables/eqt_nav.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : C_PANNEAU_GTC, code : EQT_PAN
echo "EQT_PAN" >> $FicErr
cat $Use tables/eqt_pan.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : C_PICTO, code : EQT_PCT
echo "EQT_PCT" >> $FicErr
cat $Use tables/eqt_pct.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : C_PAL, code : EQT_PAL
echo "EQT_PAL" >> $FicErr
cat $Use tables/eqt_pal.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : C_PMV, code : EQT_PMV
echo "EQT_PMV" >> $FicErr
cat $Use tables/eqt_pmv.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : CALENDRIER_D_ASTREINTE, code : ANN_AST_CAL
echo "ANN_AST_CAL" >> $FicErr
cat $Use tables/ann_ast_cal.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : CANTON, code : RES_CAN
echo "RES_CAN" >> $FicErr
cat $Use tables/res_can.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : CAMERA, code : EQT_CAM
echo "EQT_CAM" >> $FicErr
cat $Use tables/eqt_cam.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : CAPTEUR_TUNNEL, code : EQT_CPT
echo "EQT_CPT" >> $FicErr
cat $Use tables/eqt_cpt.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : CARBURANT, code : TYP_CRB
echo "TYP_CRB" >> $FicErr
cat $Use tables/typ_crb.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : CATEGORIE_D_ASTREINTE, code : ANN_AST_CAT
echo "ANN_AST_CAT" >> $FicErr
cat $Use tables/ann_ast_cat.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : CHAUSSEE, code : RES_CHA
echo "RES_CHA" >> $FicErr
cat $Use tables/res_cha.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : COMPETENCE, code : ANN_AST_CMP
echo "ANN_AST_CMP" >> $FicErr
cat $Use tables/ann_ast_cmp.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : DAI, code : EQT_DAI
echo "EQT_DAI" >> $FicErr
cat $Use tables/eqt_dai.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : DEMI_ECHANGEUR, code : EQT_ECH
echo "EQT_ECH" >> $FicErr
cat $Use tables/eqt_ech.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : DISTRICT, code : RES_DIS
echo "RES_DIS" >> $FicErr
cat $Use tables/res_dis.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : ECHANGEUR, code : RES_ECH
echo "RES_ECH" >> $FicErr
cat $Use tables/res_ech.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : ENTREES_TOR, code : EQT_TOR
echo "EQT_TOR" >> $FicErr
cat $Use tables/eqt_tor.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : EQUIPEMENT, code : EQT_GEN
echo "EQT_GEN" >> $FicErr
cat $Use tables/eqt_gen.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : EQUIPEMENT_EASYCAM, code : EQT_ISI
echo "EQT_ISI" >> $FicErr
cat $Use tables/eqt_isi.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : FICHE_ANNUAIRE, code : ANN_FIC
echo "ANN_FIC" >> $FicErr
cat $Use tables/ann_fic.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur  >> $FicErr

# nom table : FONCTION, code : OPR_FCT_NOM
echo "OPR_FCT_NOM" >> $FicErr
cat $Use tables/opr_fct_nom.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : FONCTION_MACHINE, code : OPR_FCT_MAC
echo "OPR_FCT_MAC" >> $FicErr
cat $Use tables/opr_fct_mac.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : GTC, code : EQT_GTC
echo "EQT_GTC" >> $FicErr
cat $Use tables/eqt_gtc.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : GTC_NICE, code : EQT_NIC
echo "EQT_NIC" >> $FicErr
cat $Use tables/eqt_nic.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : IDENTIFICATION, code : OPR_LOG_MDP
echo "OPR_LOG_MDP" >> $FicErr
cat $Use tables/opr_log_mdp.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : ITPC, code : RES_ITP
echo "RES_ITP" >> $FicErr
cat $Use tables/res_itp.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : LIBELLE_PREDEFINI, code : LIB_PRE
echo "LIB_PRE" >> $FicErr
cat $Use tables/lib_pre.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : LIBELLE_TYPE, code : LIB_TYP
echo "LIB_TYP" >> $FicErr
cat $Use tables/lib_typ.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : LT_VIDEO, code : EQT_LTV
echo "EQT_LTV" >> $FicErr
cat $Use tables/eqt_ltv.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : LT_VIDEO_CAN, code : EQT_LTV_CAN
echo "EQT_LTV_CAN" >> $FicErr
cat $Use tables/eqt_ltv_can.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : MAA, code : EQT_MAA
echo "EQT_MAA" >> $FicErr
cat $Use tables/eqt_maa.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : MACHINE, code : EQT_MAC
echo "EQT_MAC" >> $FicErr
cat $Use tables/eqt_mac.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : MAGNETOSCOPE, code : EQT_MAG
echo "EQT_MAG" >> $FicErr
cat $Use tables/eqt_mag.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : MONITEUR, code : EQT_MNT
echo "EQT_MNT" >> $FicErr
cat $Use tables/eqt_mnt.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : MOTIF_APPEL, code : APL_MTF
echo "APL_MTF" >> $FicErr
cat $Use tables/apl_mtf.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : NOEUD, code : RES_NEU
echo "RES_NEU" >> $FicErr
cat $Use tables/res_neu.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : OPERATEUR, code : OPR_NOM
echo "OPR_NOM" >> $FicErr
cat $Use tables/opr_nom.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : PANNEAU_POLICE, code : RES_POL
echo "RES_POL" >> $FicErr
cat $Use tables/res_pol.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : PASSAGE_SUPERIEUR, code : RES_SUP
echo "RES_SUP" >> $FicErr
cat $Use tables/res_sup.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : PAU, code : EQT_PAU
echo "EQT_PAU" >> $FicErr
cat $Use tables/eqt_pau.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : PEAGE, code : RES_PEA
echo "RES_PEA" >> $FicErr
cat $Use tables/res_pea.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : PENTE, code : RES_INT
echo "RES_PNT" >> $FicErr
cat $Use tables/res_pnt.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : PI_RAU, code : EQT_RAU
echo "EQT_RAU" >> $FicErr
cat $Use tables/eqt_rau.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : GENERIQUE_LCR, code : EQT_LCR
echo "EQT_LCR" >> $FicErr
cat $Use tables/eqt_lcr.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : UGTP_TDP, code : EQT_TDP
echo "EQT_TDP" >> $FicErr
cat $Use tables/eqt_tdp.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : ZONE_TDP, code : EQT_ZDP
echo "EQT_ZDP" >> $FicErr
cat $Use tables/eqt_zdp.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : PLAN_SECOURS, code : PLN_SCR
echo "PLN_SCR" >> $FicErr
cat $Use tables/pln_scr.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : POINT_DE_MESURE, code : EQT_PNT_MES
echo "EQT_PNT_MES" >> $FicErr
cat $Use tables/eqt_pnt_mes.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : PORTAIL, code : RES_PTL
echo "RES_PTL" >> $FicErr
cat $Use tables/res_ptl.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : PORTION_D_AUTOROUTE, code : RES_POR
echo "RES_POR" >> $FicErr
cat $Use tables/res_por.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : POSITION, code : EQT_CAM_POS
echo "EQT_CAM_POS" >> $FicErr
cat $Use tables/eqt_cam_pos.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : PROFIL, code : OPR_PRF
echo "OPR_PRF" >> $FicErr
cat $Use tables/opr_prf.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : PROFIL_FONCTIONS, code : OPR_PRF_FCT
echo "OPR_PRF_FCT" >> $FicErr
cat $Use tables/opr_prf_fct.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : RADT, code : EQT_RAD
echo "EQT_RAD" >> $FicErr
cat $Use tables/eqt_rad.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : RES_NUM_ECH, code : RES_NUM_ECH
echo "RES_NUM_ECH" >> $FicErr
cat $Use tables/res_num_ech.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : RESEAU_TRANVERSAL, code : RES_TRN
echo "RES_TRN" >> $FicErr
cat $Use tables/res_trn.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : SCENARIO_NAV, code : EQT_SCN_NAV
echo "EQT_SCN_NAV" >> $FicErr
cat $Use tables/eqt_scn_nav.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : SCENARIOS_CONNUS, code : EQT_SCN_CNN
echo "EQT_SCN_CNN" >> $FicErr
cat $Use tables/eqt_scn_cnn.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : SEGMENT_HOMOGENE, code : RES_SEG
echo "RES_SEG" >> $FicErr
cat $Use tables/res_seg.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : SEQ_AUTORISEES_ECH, code : EQT_SQA_ECH
echo "EQT_SQA_ECH" >> $FicErr
cat $Use tables/eqt_sqa_ech.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : SEQUENCE_ECH, code : EQT_SEQ_ECH
echo "EQT_SEQ_ECH" >> $FicErr
cat $Use tables/eqt_seq_ech.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : SEQUENCE_TUBE, code : EQT_SEQ_TUB
echo "EQT_SEQ_TUB" >> $FicErr
cat $Use tables/eqt_seq_tub.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : SEQUENCE_AUTORISEES_TUBE, code : EQT_SQA_TUB
echo "EQT_SQA_TUB" >> $FicErr
cat $Use tables/eqt_sqa_tub.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : SYSTEME_VIDEO, code : EQT_SYV
echo "EQT_SYV" >> $FicErr
cat $Use tables/eqt_syv.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : TABLE_DE_COMPETENCE, code : ANN_CMP
echo "ANN_CMP" >> $FicErr
cat $Use tables/ann_cmp.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : TUBE, code : EQT_TUB
echo "EQT_TUB" >> $FicErr
cat $Use tables/eqt_tub.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : TYPE_D_ALARME, code : TYP_ALM
echo "TYP_ALM" >> $FicErr
cat $Use tables/typ_alm.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : TYPE_D_ALERTE, code : TYP_ALT
echo "TYP_ALT" >> $FicErr
cat $Use tables/typ_alt.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : TYPE_D_ASTREINTE, code : TYP_AST
echo "TYP_AST" >> $FicErr
cat $Use tables/typ_ast.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : TYPE_D_EVENEMENT, code : TYP_FMC
echo "TYP_FMC" >> $FicErr
cat $Use tables/typ_fmc.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : TYPE_NAV, code : TYP_NAV
echo "TYP_NAV" >> $FicErr
cat $Use tables/typ_nav.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : TYPE_PICTO, code : TYP_PCT
echo "TYP_PCT" >> $FicErr
cat $Use tables/typ_pct.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : TYPE_PMV, code : TYP_PMV
echo "TYP_PMV" >> $FicErr
cat $Use tables/typ_pmv.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : VILLE, code : RES_VIL
echo "RES_VIL" >> $FicErr
cat $Use tables/res_vil.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : POINT_REMARQUABLE_TDP, code : RES_DTP
echo "RES_DTP" >> $FicErr
cat $Use tables/res_dtp.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : VUE_TRONCON, code : RES_TRC
echo "RES_TRC" >> $FicErr
cat $Use tables/res_trc.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : ZONE_D_INFLUENCE_STATION, code : RES_INF
echo "RES_INF" >> $FicErr
cat $Use tables/res_inf.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : ZONE_GEOGRAPHIQUE, code : ANN_GEO
echo "ANN_GEO" >> $FicErr
cat $Use tables/ann_geo.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : VITESSE LIBRE, code : EQT_RAD_VTL
echo "EQT_RAD_VTL" >> $FicErr
cat $Use tables/eqt_rad_vtl.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr


# nom table : ZONE PMV, code ZON_PMV
echo "ZON_PMV" >> $FicErr
cat $Use tables/zon_pmv.tab | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

if [[ $NomServeur = "CI" ]]
then
##### configuration strada

# nom table : ADA_STA, code : ADA_STA
echo "ADA_STA" >> $FicErr
cat $Use tables/ada_sta.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ADA_PMV, code : ADA_PMV
echo "ADA_PMV" >> $FicErr
cat $Use tables/ada_evt.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : NOM_RDS, code : NOM_RDS
echo "NOM_RDS" >> $FicErr
cat $Use tables/nom_rds.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : NUM_RDS, code : NUM_RDS
echo "NUM_RDS" >> $FicErr
cat $Use tables/num_rds.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : PR_RDS, code : PR_RDS
echo "PR_RDS" >> $FicErr
cat $Use tables/pr_rds.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : RDS, code : RDS
echo "RDS" >> $FicErr
cat $Use tables/rds.tab | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr
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

# nom table : AIRE, code : RES_AIR
echo "RES_AIR" >> $FicErr
cat $Use tables/res_air.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : ALARME_POSSIBLE, code : EQT_ALM
echo "EQT_ALM" >> $FicErr
cat $Use tables/eqt_alm.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : ANALYSEUR, code : EQT_ANA
echo "EQT_ANA" >> $FicErr
cat $Use tables/eqt_ana.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : AUTOROUTE, code : RES_AUT
echo "RES_AUT" >> $FicErr
cat $Use tables/res_aut.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : C_FAX, code : EQT_FAX
echo "EQT_FAX" >> $FicErr
cat $Use tables/eqt_fax.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : C_FAV, code : EQT_FAV
echo "EQT_FAV" >> $FicErr
cat $Use tables/eqt_fav.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : C_MODULE, code : EQT_MOD
echo "EQT_MOD" >> $FicErr
cat $Use tables/eqt_mod.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur>> $FicErr

# nom table : C_NAV, code : EQT_NAV
echo "EQT_NAV" >> $FicErr
cat $Use tables/eqt_nav.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : C_PANNEAU_GTC, code : EQT_PAN
echo "EQT_PAN" >> $FicErr
cat $Use tables/eqt_pan.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : C_PICTO, code : EQT_PCT
echo "EQT_PCT" >> $FicErr
cat $Use tables/eqt_pct.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : C_PAL, code : EQT_PAL
echo "EQT_PAL" >> $FicErr
cat $Use tables/eqt_pal.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : C_PMV, code : EQT_PMV
echo "EQT_PMV" >> $FicErr
cat $Use tables/eqt_pmv.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : CALENDRIER_D_ASTREINTE, code : ANN_AST_CAL
echo "ANN_AST_CAL" >> $FicErr
cat $Use tables/ann_ast_cal.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : CAMERA, code : EQT_CAM
echo "EQT_CAM" >> $FicErr
cat $Use tables/eqt_cam.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : CAPTEUR_TUNNEL, code : EQT_CPT
echo "EQT_CPT" >> $FicErr
cat $Use tables/eqt_cpt.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : GTC_NICE, code : EQT_NIC
echo "EQT_NIC" >> $FicErr
cat $Use tables/eqt_nic.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : CANTON, code : RES_CAN
echo "RES_CAN" >> $FicErr
cat $Use tables/res_can.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : CARBURANT, code : TYP_CRB
echo "TYP_CRB" >> $FicErr
cat $Use tables/typ_crb.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : CATEGORIE_D_ASTREINTE, code : ANN_AST_CAT
echo "ANN_AST_CAT" >> $FicErr
cat $Use tables/ann_ast_cat.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : CHAUSSEE, code : RES_CHA
echo "RES_CHA" >> $FicErr
cat $Use tables/res_cha.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : COMPETENCE, code : ANN_AST_CMP
echo "ANN_AST_CMP" >> $FicErr
cat $Use tables/ann_ast_cmp.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : DAI, code : EQT_DAI
echo "EQT_DAI" >> $FicErr
cat $Use tables/eqt_dai.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : DEMI_ECHANGEUR, code : EQT_ECH
echo "EQT_ECH" >> $FicErr
cat $Use tables/eqt_ech.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : DISTRICT, code : RES_DIS
echo "RES_DIS" >> $FicErr
cat $Use tables/res_dis.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : ECHANGEUR, code : RES_ECH
echo "RES_ECH" >> $FicErr
cat $Use tables/res_ech.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : ENTREES_TOR, code : EQT_TOR
echo "EQT_TOR" >> $FicErr
cat $Use tables/eqt_tor.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : EQUIPEMENT, code : EQT_GEN
echo "EQT_GEN" >> $FicErr
cat $Use tables/eqt_gen.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : FICHE_ANNUAIRE, code : ANN_FIC
echo "ANN_FIC" >> $FicErr
cat $Use tables/ann_fic.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur  >> $FicErr

# nom table : FONCTION, code : OPR_FCT_NOM
echo "OPR_FCT_NOM" >> $FicErr
cat $Use tables/opr_fct_nom.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : FONCTION_MACHINE, code : OPR_FCT_MAC
echo "OPR_FCT_MAC" >> $FicErr
cat $Use tables/opr_fct_mac.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : GTC, code : EQT_GTC
echo "EQT_GTC" >> $FicErr
cat $Use tables/eqt_gtc.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : IDENTIFICATION, code : OPR_LOG_MDP
echo "OPR_LOG_MDP" >> $FicErr
cat $Use tables/opr_log_mdp.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : ITPC, code : RES_ITP
echo "RES_ITP" >> $FicErr
cat $Use tables/res_itp.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : LIBELLE_PREDEFINI, code : LIB_PRE
echo "LIB_PRE" >> $FicErr
cat $Use tables/lib_pre.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : LIBELLE_TYPE, code : LIB_TYP
echo "LIB_TYP" >> $FicErr
cat $Use tables/lib_typ.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : LT_VIDEO, code : EQT_LTV
echo "EQT_LTV" >> $FicErr
cat $Use tables/eqt_ltv.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : LT_VIDEO_CAN, code : EQT_LTV_CAN
echo "EQT_LTV_CAN" >> $FicErr
cat $Use tables/eqt_ltv_can.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : MAA, code : EQT_MAA
echo "EQT_MAA" >> $FicErr
cat $Use tables/eqt_maa.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : MACHINE, code : EQT_MAC
echo "EQT_MAC" >> $FicErr
cat $Use tables/eqt_mac.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : MAGNETOSCOPE, code : EQT_MAG
echo "EQT_MAG" >> $FicErr
cat $Use tables/eqt_mag.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : MONITEUR, code : EQT_MNT
echo "EQT_MNT" >> $FicErr
cat $Use tables/eqt_mnt.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : MOTIF_APPEL, code : APL_MTF
echo "APL_MTF" >> $FicErr
cat $Use tables/apl_mtf.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : NOEUD, code : RES_NEU
echo "RES_NEU" >> $FicErr
cat $Use tables/res_neu.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : OPERATEUR, code : OPR_NOM
echo "OPR_NOM" >> $FicErr
cat $Use tables/opr_nom.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : PANNEAU_POLICE, code : RES_POL
echo "RES_POL" >> $FicErr
cat $Use tables/res_pol.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : PASSAGE_SUPERIEUR, code : RES_SUP
echo "RES_SUP" >> $FicErr
cat $Use tables/res_sup.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : PAU, code : EQT_PAU
echo "EQT_PAU" >> $FicErr
cat $Use tables/eqt_pau.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : PEAGE, code : RES_PEA
echo "RES_PEA" >> $FicErr
cat $Use tables/res_pea.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : PENTE, code : RES_INT
echo "RES_INT" >> $FicErr
cat $Use tables/res_pnt.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : PI_RAU, code : EQT_RAU
echo "EQT_RAU" >> $FicErr
cat $Use tables/eqt_rau.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : GENERIQUE_LCR, code : EQT_LCR
echo "EQT_LCR" >> $FicErr
cat $Use tables/eqt_lcr.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : UGTP_TDP, code : EQT_TDP
echo "EQT_TDP" >> $FicErr
cat $Use tables/eqt_tdp.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : ZONE_TDP, code : EQT_ZDP
echo "EQT_ZDP" >> $FicErr
cat $Use tables/eqt_zdp.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : PLAN_SECOURS, code : PLN_SCR
echo "PLN_SCR" >> $FicErr
cat $Use tables/pln_scr.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : POINT_DE_MESURE, code : EQT_PNT_MES
echo "EQT_PNT_MES" >> $FicErr
cat $Use tables/eqt_pnt_mes.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur  >> $FicErr

# nom table : PORTAIL, code : RES_PTL
echo "RES_PTL" >> $FicErr
cat $Use tables/res_ptl.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : PORTION_D_AUTOROUTE, code : RES_POR
echo "RES_POR" >> $FicErr
cat $Use tables/res_por.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : POSITION, code : EQT_CAM_POS
echo "EQT_CAM_POS" >> $FicErr
cat $Use tables/eqt_cam_pos.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : PROFIL, code : OPR_PRF
echo "OPR_PRF" >> $FicErr
cat $Use tables/opr_prf.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : PROFIL_FONCTIONS, code : OPR_PRF_FCT
echo "OPR_PRF_FCT" >> $FicErr
cat $Use tables/opr_prf_fct.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : RADT, code : EQT_RAD
echo "EQT_RAD" >> $FicErr
cat $Use tables/eqt_rad.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : RES_NUM_ECH, code : RES_NUM_ECH
echo "RES_NUM_ECH" >> $FicErr
cat $Use tables/res_num_ech.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : RESEAU_TRANVERSAL, code : RES_TRN
echo "RES_TRN" >> $FicErr
cat $Use tables/res_trn.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : SCENARIO_NAV, code : EQT_SCN_NAV
echo "EQT_SCN_NAV" >> $FicErr
cat $Use tables/eqt_scn_nav.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : SCENARIOS_CONNUS, code : EQT_SCN_CNN
echo "EQT_SCN_CNN" >> $FicErr
cat $Use tables/eqt_scn_cnn.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : SEGMENT_HOMOGENE, code : RES_SEG
echo "RES_SEG" >> $FicErr
cat $Use tables/res_seg.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : SEQ_AUTORISEES_ECH, code : EQT_SQA_ECH
echo "EQT_SQA_ECH" >> $FicErr
cat $Use tables/eqt_sqa_ech.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : SEQUENCE_ECH, code : EQT_SEQ_ECH
echo "EQT_SEQ_ECH" >> $FicErr
cat $Use tables/eqt_seq_ech.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : SEQUENCE_TUBE, code : EQT_SEQ_TUB
echo "EQT_SEQ_TUB" >> $FicErr
cat $Use tables/eqt_seq_tub.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : SEQUENCE_AUTORISEES_TUBE, code : EQT_SQA_TUB
echo "EQT_SQA_TUB" >> $FicErr
cat $Use tables/eqt_sqa_tub.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : SYSTEME_VIDEO, code : EQT_SYV
echo "EQT_SYV" >> $FicErr
cat $Use tables/eqt_syv.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : TABLE_DE_COMPETENCE, code : ANN_CMP
echo "ANN_CMP" >> $FicErr
cat $Use tables/ann_cmp.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : TUBE, code : EQT_TUB
echo "EQT_TUB" >> $FicErr
cat $Use tables/eqt_tub.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : TYPE_D_ALARME, code : TYP_ALM
echo "TYP_ALM" >> $FicErr
cat $Use tables/typ_alm.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : TYPE_D_ALERTE, code : TYP_ALT
echo "TYP_ALT" >> $FicErr
cat $Use tables/typ_alt.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : TYPE_D_ASTREINTE, code : TYP_AST
echo "TYP_AST" >> $FicErr
cat $Use tables/typ_ast.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : TYPE_D_EVENEMENT, code : TYP_FMC
echo "TYP_FMC" >> $FicErr
cat $Use tables/typ_fmc.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : TYPE_NAV, code : TYP_NAV
echo "TYP_NAV" >> $FicErr
cat $Use tables/typ_nav.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : TYPE_PICTO, code : TYP_PCT
echo "TYP_PCT" >> $FicErr
cat $Use tables/typ_pct.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : TYPE_PMV, code : TYP_PMV
echo "TYP_PMV" >> $FicErr
cat $Use tables/typ_pmv.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : VILLE, code : RES_VIL
echo "RES_VIL" >> $FicErr
cat $Use tables/res_vil.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : POINT_REMARQUABLE_TDP, code : RES_DTP
echo "RES_DTP" >> $FicErr
cat $Use tables/res_dtp.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : VUE_TRONCON, code : RES_TRC
echo "RES_TRC" >> $FicErr
cat $Use tables/res_trc.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : ZONE_D_INFLUENCE_STATION, code : RES_INF
echo "RES_INF" >> $FicErr
cat $Use tables/res_inf.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : ZONE_GEOGRAPHIQUE, code : ANN_GEO
echo "ANN_GEO" >> $FicErr
cat $Use tables/ann_geo.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# nom table : ZONE PMV, code ZON_PMV
echo "ZON_PMV" >> $FicErr
cat $Use tables/zon_pmv.cle | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

if [[ $NomServeur = "CI" ]]
then
##### configuration strada

# nom table : ADA_STA, code : ADA_STA
echo "ADA_STA" >> $FicErr
cat $Use tables/ada_sta.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : ADA_PMV, code : ADA_PMV
echo "ADA_PMV" >> $FicErr
cat $Use tables/ada_evt.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : NOM_RDS, code : NOM_RDS
echo "NOM_RDS" >> $FicErr
cat $Use tables/nom_rds.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : NUM_RDS, code : NUM_RDS
echo "NUM_RDS" >> $FicErr
cat $Use tables/num_rds.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : PR_RDS, code : PR_RDS
echo "PR_RDS" >> $FicErr
cat $Use tables/pr_rds.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

# nom table : RDS, code : RDS
echo "RDS" >> $FicErr
cat $Use tables/rds.cle | isql -S $NomServeur -U MIGRAZUR -P MIGRAZUR >> $FicErr

fi

# Test si un probleme est survenu lors de la creation des cles etrangeres des tables
grep "Msg" $FicErr | wc -l | read NbLignes ResteInfo
if ((NbLignes!=0))
then exit 0
fi
rm $FicErr


#######################
# Creation des triggers
#######################


exit 1
