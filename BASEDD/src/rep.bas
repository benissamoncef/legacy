#!/bin/ksh
# Fichier : @(#)rep.bas	1.14        Release : 1.14        Date :  20/03/2007
##################################################################################
# STERIA # PROJET MIGRAZUR
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# SCRIPT rep.bas
##################################################################################
# DESCRIPTION DU SCRIPT :
# creation des definitions de la replication des tables pour un site
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# C.T.	09/08/94	: Creation
# B.G.  29/09/94        : Ajout des tables manquantes (v1.2)
# C.T.  21/03/95        : Modif chemin d'acces aux fichiers de definition (v 1.5)
# C.T.  30/08/96        : Ajout le cas de PC simplifie (v 1.6)
#                         et fmc_pur pour la purge RADT
# C.T.  30/10/96        : Ajout mes_kcs, mes_qpc, mes_nec (V1.7)
# C.T.  08/12/97        : Ajout frn_tdp, zdp_6mn (V1.8)
# C.T.  19/12/97        : Ajout fmc_trs (V1.9)
# JMG	08/01/98	: ajout alt_exp sur tous les PC (dem/1536) 1.10
# C.T.  20/01/98        : Ajout act_pal (1538) (V1.11)
# P.N.  29/01/98	: Ajout DA DY DP v1.12
# C.T.  19/02/98	: Ajout DS v1.13
# F.C.  20/03/07	: Ajout act_baf, act_bad, act_pma, fmc_nat, et dans CI, ajout de act_exp_ada, act_pmv_ada v1.14
" JMG	09/07/07	: suppression DY
##################################################################################

if ((($#<1)) || (test $1 != "CI" && test $1 != "DN" && test $1 != "DM" && test $1 != "DC" && test $1 != "DA" && test $1 != "DP" && test $1 != "DS" ))
then echo "syntaxe : rep.bas NomSitePrimaire TypeSitePrimaire NomfichierErreur"
     echo "          NomSitePrimaire  : CI ou DN ou DM ou DC ou DA ou DP ou DS"
     echo "          TypeSitePrimaire  : Simplifie ou NonSimplifie"
     echo "          NomfichierErreur : fichier d'erreur optionnel"
     echo "                             par defaut rep.err"
     exit 0
fi

#nom du site primaire
NomSite=$1

#type du site primaire
TypeSite=$2

#nom du fichier d'erreur
if (($#==2))
then NomFicErr="rep.err"
else NomFicErr=$3.err
fi

#creation des definitions des replications de tables de EXP
tables/act_gen.rep EXP $NomSite $NomFicErr
tables/act_nav.rep EXP $NomSite $NomFicErr
tables/act_pmv.rep EXP $NomSite $NomFicErr
tables/act_pal.rep EXP $NomSite $NomFicErr
tables/act_baf.rep EXP $NomSite $NomFicErr
tables/act_bad.rep EXP $NomSite $NomFicErr
tables/act_pma.rep EXP $NomSite $NomFicErr

tables/alm_eqt.rep EXP $NomSite $NomFicErr
tables/alm_nbr.rep EXP $NomSite $NomFicErr
tables/eqt_acv.rep EXP $NomSite $NomFicErr
tables/eqt_dsp.rep EXP $NomSite $NomFicErr

tables/alt_exp.rep EXP $NomSite $NomFicErr

tables/fmc_com.rep EXP $NomSite $NomFicErr
tables/fmc_gen.rep EXP $NomSite $NomFicErr
tables/fmc_his.rep EXP $NomSite $NomFicErr
tables/fmc_opr.rep EXP $NomSite $NomFicErr
tables/fmc_trf.rep EXP $NomSite $NomFicErr
tables/fmc_trh.rep EXP $NomSite $NomFicErr
tables/fmc_nat.rep EXP $NomSite $NomFicErr

tables/mes_niv.rep EXP $NomSite $NomFicErr
tables/mes_poi.rep EXP $NomSite $NomFicErr
tables/mes_tra.rep EXP $NomSite $NomFicErr
tables/mes_kcs.rep EXP $NomSite $NomFicErr
tables/mes_qpc.rep EXP $NomSite $NomFicErr
tables/mes_nec.rep EXP $NomSite $NomFicErr
tables/zdp_6mn.rep EXP $NomSite $NomFicErr

if [[ $TypeSite = "NonSimplifie" ]]
then
tables/act_apl.rep EXP $NomSite $NomFicErr
tables/act_dep.rep EXP $NomSite $NomFicErr
tables/act_ech.rep EXP $NomSite $NomFicErr
tables/act_fax.rep EXP $NomSite $NomFicErr
tables/act_frg.rep EXP $NomSite $NomFicErr
tables/act_gar.rep EXP $NomSite $NomFicErr
tables/act_int.rep EXP $NomSite $NomFicErr
tables/act_rmq.rep EXP $NomSite $NomFicErr
tables/act_sis.rep EXP $NomSite $NomFicErr
tables/act_tfm.rep EXP $NomSite $NomFicErr
tables/act_tub.rep EXP $NomSite $NomFicErr
tables/act_txt_tfm.rep EXP $NomSite $NomFicErr
tables/eqt_fil.rep EXP $NomSite $NomFicErr
tables/fmc_acc.rep EXP $NomSite $NomFicErr
tables/fmc_bas.rep EXP $NomSite $NomFicErr
tables/fmc_bou.rep EXP $NomSite $NomFicErr
tables/fmc_cnc.rep EXP $NomSite $NomFicErr
tables/fmc_del.rep EXP $NomSite $NomFicErr
tables/fmc_ech.rep EXP $NomSite $NomFicErr
tables/fmc_met.rep EXP $NomSite $NomFicErr
tables/fmc_mnf.rep EXP $NomSite $NomFicErr
tables/fmc_rev.rep EXP $NomSite $NomFicErr
tables/fmc_tfm.rep EXP $NomSite $NomFicErr
tables/fmc_trt.rep EXP $NomSite $NomFicErr
tables/fmc_veh.rep EXP $NomSite $NomFicErr
tables/fmc_vln.rep EXP $NomSite $NomFicErr
tables/frn_tdp.rep EXP $NomSite $NomFicErr
fi

if [[ $NomSite = "CI" ]]
then
tables/fmc_pur.rep EXP $NomSite $NomFicErr
tables/fmc_trs.rep EXP $NomSite $NomFicErr
tables/act_exp_ada.rep EXP $NomSite $NomFicErr
tables/act_pmv_ada.rep EXP $NomSite $NomFicErr
fi
