#!/bin/ksh
# Fichier : $Id: rep_cfg.bas,v 1.10 2007/07/02 14:13:19 gesconf Exp $        Release : $Revision: 1.10 $        Date : $Date: 2007/07/02 14:13:19 $
##################################################################################
# STERIA # PROJET MIGRAZUR
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# SCRIPT rep_cfg.bas
##################################################################################
# DESCRIPTION DU SCRIPT :
# creation des definitions de la replication des tables pour CFG
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# B.G.	11/10/94	: Creation
# C.T.  21/03/95	: Modif chemin d'acces aux fichiers de replication
# C.T.  29/06/95	: Ajout de res_num_ech
# C.T.  10/08/95	: Ajout de eqt_isi
# C.T.  18/10/95	: Ajout de res_aut_inv
# C.T.  07/12/95	: Ajout de eqt_fax
# P.V.  18/06/96	: Ajout de lib_typ et lib_pre
# C.T.  08/12/97	: Ajout de eqt_tdp eqt_zdp res_dtp
# JMG	08/01/99	: ajout de zon_pmv (dem/1536) 1.8
# C.T.  16/01/98	: Ajout de eqt_pal 1.9
# JPL	02/07/07	: Ajout de res_can (DEM 657) 1.10
##################################################################################

if ((($#<0)))
then echo "syntaxe : rep_cfg.bas NomfichierErreur"
     echo "          NomfichierErreur : fichier d'erreur optionnel"
     echo "                             par defaut rep_cfg.err"
     exit 0
fi

#nom du site primaire
NomSite=$1

#nom du fichier d'erreur
if (($#==0))
then NomFicErr="rep_cfg.err"
else NomFicErr=$1.err
fi

#creation des definitions des replications de tables de CFG
tables/ann_ast_cal.rep $NomFicErr
tables/ann_ast_cat.rep $NomFicErr
tables/ann_ast_cmp.rep $NomFicErr
tables/ann_cmp.rep $NomFicErr
tables/ann_fic.rep $NomFicErr
tables/ann_geo.rep $NomFicErr
tables/eqt_alm.rep $NomFicErr
tables/eqt_ana.rep $NomFicErr
tables/eqt_cam.rep $NomFicErr
tables/eqt_cam_pos.rep $NomFicErr
tables/eqt_cpt.rep $NomFicErr
tables/eqt_dai.rep $NomFicErr
tables/eqt_ech.rep $NomFicErr
tables/eqt_fax.rep $NomFicErr
tables/eqt_fav.rep $NomFicErr
tables/eqt_gen.rep $NomFicErr
tables/eqt_gtc.rep $NomFicErr
tables/eqt_isi.rep $NomFicErr
tables/eqt_lcr.rep $NomFicErr
tables/eqt_ltv.rep $NomFicErr
tables/eqt_maa.rep $NomFicErr
tables/eqt_mac.rep $NomFicErr
tables/eqt_mag.rep $NomFicErr
tables/eqt_mnt.rep $NomFicErr
tables/eqt_mod.rep $NomFicErr
tables/eqt_nav.rep $NomFicErr
tables/eqt_nic.rep $NomFicErr
tables/eqt_pal.rep $NomFicErr
tables/eqt_pan.rep $NomFicErr
tables/eqt_pau.rep $NomFicErr
tables/eqt_pct.rep $NomFicErr
tables/eqt_pmv.rep $NomFicErr
tables/eqt_pnt_mes.rep $NomFicErr
tables/eqt_rad.rep $NomFicErr
tables/eqt_rad_vtl.rep $NomFicErr
tables/eqt_rau.rep $NomFicErr
tables/eqt_scn_cnn.rep $NomFicErr
tables/eqt_scn_nav.rep $NomFicErr
tables/eqt_seq_ech.rep $NomFicErr
tables/eqt_seq_tub.rep $NomFicErr
tables/eqt_sqa_ech.rep $NomFicErr
tables/eqt_sqa_tub.rep $NomFicErr
tables/eqt_syv.rep $NomFicErr
tables/eqt_tdp.rep $NomFicErr
tables/eqt_tor.rep $NomFicErr
tables/eqt_tub.rep $NomFicErr
tables/eqt_zdp.rep $NomFicErr
tables/lib_typ.rep $NomFicErr
tables/lib_pre.rep $NomFicErr
tables/opr_fct_mac.rep $NomFicErr
tables/opr_fct_nom.rep $NomFicErr
tables/opr_log_mdp.rep $NomFicErr
tables/opr_nom.rep $NomFicErr
tables/opr_prf.rep $NomFicErr
tables/opr_prf_fct.rep $NomFicErr
tables/pln_scr.rep $NomFicErr
tables/res_air.rep $NomFicErr
tables/res_aut.rep $NomFicErr
tables/res_aut_inv.rep $NomFicErr
tables/res_can.rep $NomFicErr
tables/res_cha.rep $NomFicErr
tables/res_dis.rep $NomFicErr
tables/res_dtp.rep $NomFicErr
tables/res_ech.rep $NomFicErr
tables/res_inf.rep $NomFicErr
tables/res_itp.rep $NomFicErr
tables/res_neu.rep $NomFicErr
tables/res_num_ech.rep $NomFicErr
tables/res_pea.rep $NomFicErr
tables/res_pnt.rep $NomFicErr
tables/res_pol.rep $NomFicErr
tables/res_por.rep $NomFicErr
tables/res_ptl.rep $NomFicErr
tables/res_seg.rep $NomFicErr
tables/res_sup.rep $NomFicErr
tables/res_trc.rep $NomFicErr
tables/res_trn.rep $NomFicErr
tables/res_vil.rep $NomFicErr
tables/typ_alm.rep $NomFicErr
tables/typ_alt.rep $NomFicErr
tables/typ_ast.rep $NomFicErr
tables/typ_crb.rep $NomFicErr
tables/typ_fmc.rep $NomFicErr
tables/typ_nav.rep $NomFicErr
tables/typ_pct.rep $NomFicErr
tables/typ_pmv.rep $NomFicErr
tables/zon_pmv.rep $NomFicErr
