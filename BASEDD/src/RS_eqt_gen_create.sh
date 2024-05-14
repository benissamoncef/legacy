#!/bin/ksh
# Fichier : $Id: RS_eqt_gen_create.sh,v 1.1 2007/11/19 17:19:57 pc2dpdy Exp $        Release : $Revision: 1.1 $       Date : $Date: 2007/11/19 17:19:57 $
##################################################################################
# STERIA # PROJET MIGRAZUR
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# SCRIPT RS_sub_CFG.sh
##################################################################################
# DESCRIPTION DU SCRIPT : 
# creation des souscriptions sur EQT_GEN 
# 
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
#* JMG   13/11/07        : ajout site de gestion DEM715
##################################################################################

FicErr="RS_eqt_gen.err"

tables/sub_create.rab CFG EQT_GEN CI DA xx $FicErr
tables/sub_create.rab CFG EQT_GEN CI DM xx $FicErr
tables/sub_create.rab CFG EQT_GEN CI DN xx $FicErr
tables/sub_create.rab CFG EQT_GEN CI DC xx $FicErr
tables/sub_create.rab CFG EQT_GEN CI DS xx $FicErr
tables/sub_create.rab CFG EQT_GEN CI DP xx $FicErr
tables/sub_create.rab CFG EQT_GEN CI HIS xx $FicErr


