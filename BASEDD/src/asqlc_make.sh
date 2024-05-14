#!/bin/ksh
# Fichier : $Id: asqlc_make.sh,v 1.1 1995/02/15 19:51:51 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/02/15 19:51:51 $
##################################################################################
# STERIA # PROJET MIGRAZUR
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# MODULE ASQLC # FICHIER asqlc_make.sh
##################################################################################
# DESCRIPTION DU MODULE :
#  compile les triggers sur SQL_CI
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# C.T.	15/02/95	: Creation
##################################################################################

# repertoire des includes
includes=$HOME

# initialiser le fichier de trace des erreurs
echo "trace de la creation des triggers de asql" > creprc.err
echo "" >> creprc.err

# cr휿 asql_c01_alm_eqt.trg
echo "asql_c01_res_air.trg" >> creprc.err
cat asql_c01_res_air.trg >> creprc.c
cc -P -C -DCI creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_CI -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cr휿 asql_c02_res_aut.trg
echo "asql_c02_res_aut.trg" >> creprc.err
cat asql_c02_res_aut.trg >> creprc.c
cc -P -C -DCI creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_CI -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cr휿 asql_c03_res_cha.trg
echo "asql_c03_res_cha.trg" >> creprc.err
cat asql_c03_res_cha.trg >> creprc.c
cc -P -C -DCI creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_CI -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cr휿 asql_c04_res_dis.trg
echo "asql_c04_res_dis.trg" >> creprc.err
cat asql_c04_res_dis.trg >> creprc.c
cc -P -C -DCI creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_CI -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cr휿 asql_c05_res_ech.trg
echo "asql_c05_res_ech.trg" >> creprc.err
cat asql_c05_res_ech.trg >> creprc.c
cc -P -C -DCI creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_CI -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cr휿 asql_c06_res_inf.trg
echo "asql_c06_res_inf.trg" >> creprc.err
cat asql_c06_res_inf.trg >> creprc.c
cc -P -C -DCI creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_CI -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cr휿 asql_c07_res_itp.trg
echo "asql_c07_res_itp.trg" >> creprc.err
cat asql_c07_res_itp.trg >> creprc.c
cc -P -C -DCI creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_CI -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cr휿 asql_c08_res_neu.trg
echo "asql_c08_res_neu.trg" >> creprc.err
cat asql_c08_res_neu.trg >> creprc.c
cc -P -C -DCI creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_CI -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cr휿 asql_c09_res_pea.trg
echo "asql_c09_res_pea.trg" >> creprc.err
cat asql_c09_res_pea.trg >> creprc.c
cc -P -C -DCI creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_CI -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cr휿 asql_c10_res_pnt.trg
echo "asql_c10_res_pnt.trg" >> creprc.err
cat asql_c10_res_pnt.trg >> creprc.c
cc -P -C -DCI creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_CI -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cr휿 asql_c11_res_pol.trg
echo "asql_c11_res_pol.trg" >> creprc.err
cat asql_c11_res_pol.trg >> creprc.c
cc -P -C -DCI creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_CI -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cr휿 asql_c12_res_por.trg
echo "asql_c12_res_por.trg" >> creprc.err
cat asql_c12_res_por.trg >> creprc.c
cc -P -C -DCI creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_CI -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cr휿 asql_c13_res_ptl.trg
echo "asql_c13_res_ptl.trg" >> creprc.err
cat asql_c13_res_ptl.trg >> creprc.c
cc -P -C -DCI creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_CI -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cr휿 asql_c14_res_seg.trg
echo "asql_c14_res_seg.trg" >> creprc.err
cat asql_c14_res_seg.trg >> creprc.c
cc -P -C -DCI creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_CI -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cr휿 asql_c15_res_sup.trg
echo "asql_c15_res_sup.trg" >> creprc.err
cat asql_c15_res_sup.trg >> creprc.c
cc -P -C -DCI creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_CI -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cr휿 asql_c16_res_trc.trg
echo "asql_c16_res_trc.trg" >> creprc.err
cat asql_c16_res_trc.trg >> creprc.c
cc -P -C -DCI creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_CI -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cr휿 asql_c17_res_trn.trg
echo "asql_c17_res_trn.trg" >> creprc.err
cat asql_c17_res_trn.trg >> creprc.c
cc -P -C -DCI creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_CI -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cr휿 asql_c18_res_vil.trg
echo "asql_c18_res_vil.trg" >> creprc.err
cat asql_c18_res_vil.trg >> creprc.c
cc -P -C -DCI creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_CI -i creprc.i >> creprc.err
rm creprc.c creprc.i

exit 0
