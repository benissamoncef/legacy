#!/bin/ksh
# Fichier : $Id: asql_make.sh,v 1.12 1999/02/26 14:30:18 gaborit Exp $      Release : $Revision: 1.12 $        Date : $Date: 1999/02/26 14:30:18 $
##################################################################################
# STERIA # PROJET MIGRAZUR
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# MODULE ASQL # FICHIER asql_make.sh
##################################################################################
# DESCRIPTION DU MODULE :
#  compile les triggers sur SQL_CI ou SQL_DN ou SQL_DM
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# B.GABORIT	04/11/94		: Creation
# F.VOLCIC      11/11/94    : ajout asql_t01 et 03
# B.GABORIT     11/11/94    : ajout asql_t04
# B.GABORIT     15/11/94    : ajout asql_t05 a 14
# B.GABORIT     25/01/95    : ajout asql_t15 a 19
# C.TORREGROSSA 25/01/95    : ajout cas de compilation sur DC et DS (RADT V1.6)
# C.TORREGROSSA 06/01/98    : ajout asql_t_22 sauf pour PC niveau 3 (1531) 
# C.TORREGROSSA 12/01/98    : ajout asql_t_23 pour CI (1532) 
# C.TORREGROSSA 06/01/98    : modif asql_t_22 sauf pour PC niveau 3  et CI (1585) 
# P.NIEPCERON	10/02/99	: asql_t24_mes_niv.trg dem/1723 v1.12
# GGY   		15/09/2023  : Adaptation pour CNA (DEM-483)
##################################################################################

# repertoire des includes
includes=$HOME

if (test $# = 0 || ( test $1 != "CI" && test $1 != "DN" && test $1 != "DM" && test $1 != "DC" && test $1 != "DS"  && test $1 != "DA"  && test $1 != "DP"  && test $1 != "DY" ))
then
	echo "syntaxe : asql_make.sh NomSite type"
	echo "          NomSite : CI ou DN ou DM ou DC ou DS ou DA ou DY ou DP"
	exit 1
fi

if [[ $# = 1 ]]
then
        type="xx"
else
        type=$2
fi

# code de compilation conditionnellle
if [[ $type != "PC3" ]]
then
	Compilation=$1
else
	Compilation="PC_SIMPL"
fi

# initialiser le fichier de trace des erreurs
echo "trace de la creation des triggers de asql" > creprc.err
echo "" >> creprc.err

# cree asql_t01_alm_eqt.trg
# echo "asql_t01_alm_eqt.trg" >> creprc.err
# echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
# echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
# echo "#include \"$includes/migrazur/XDMICG/inc/xzahc.h\"" >> creprc.c
# cat asql_t01_alm_eqt.trg >> creprc.c
# cc -P -C -D${1} creprc.c
# isql -UMIGRAZUR -PMIGRAZUR -SSQL_${1} -i creprc.i >> creprc.err
# rm creprc.c creprc.i

# cree asql_t04_eqt_gen.trg
echo "asql_t04_eqt_gen.trg" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzahc.h\"" >> creprc.c
cat asql_t04_eqt_gen.trg >> creprc.c
cc -P -C -D${1} creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${1} -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cree asql_t05_fmc_gen.trg
echo "asql_t05_fmc_gen.trg" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
cat asql_t05_fmc_gen.trg >> creprc.c
cc -P -C -D${1} creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${1} -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cree asql_t12_fmc_trf.trg
echo "asql_t12_fmc_trf.trg" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
cat asql_t12_fmc_trf.trg >> creprc.c
cc -P -C -D${1} creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${1} -i creprc.i >> creprc.err
rm creprc.c creprc.i

if [[ $type != "PC_SIMPL" ]]
then
# cree asql_t02_fmc_his.trg
echo "asql_t02_fmc_his.trg" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzahc.h\"" >> creprc.c
cat asql_t02_fmc_his.trg >> creprc.c
cc -P -C -D${1} creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${1} -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cree asql_t03_fmc_rev.trg
echo "asql_t03_fmc_rev.trg" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzahc.h\"" >> creprc.c
cat asql_t03_fmc_rev.trg >> creprc.c
cc -P -C -D${1} creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${1} -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cree asql_t06_fmc_acc.trg
echo "asql_t06_fmc_acc.trg" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
cat asql_t06_fmc_acc.trg >> creprc.c
cc -P -C -D${1} creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${1} -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cree asql_t07_fmc_bas.trg
echo "asql_t07_fmc_bas.trg" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
cat asql_t07_fmc_bas.trg >> creprc.c
cc -P -C -D${1} creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${1} -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cree asql_t08_fmc_cnc.trg
echo "asql_t08_fmc_cnc.trg" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
cat asql_t08_fmc_cnc.trg >> creprc.c
cc -P -C -D${1} creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${1} -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cree asql_t09_fmc_del.trg
echo "asql_t09_fmc_del.trg" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
cat asql_t09_fmc_del.trg >> creprc.c
cc -P -C -D${1} creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${1} -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cree asql_t10_fmc_ech.trg
echo "asql_t10_fmc_ech.trg" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
cat asql_t10_fmc_ech.trg >> creprc.c
cc -P -C -D${1} creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${1} -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cree asql_t11_fmc_mnf.trg
echo "asql_t11_fmc_mnf.trg" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
cat asql_t11_fmc_mnf.trg >> creprc.c
cc -P -C -D${1} creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${1} -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cree asql_t13_fmc_veh.trg
echo "asql_t13_fmc_veh.trg" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
cat asql_t13_fmc_veh.trg >> creprc.c
cc -P -C -D${1} creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${1} -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cree asql_t14_act_gen.trg
echo "asql_t14_act_gen.trg" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
cat asql_t14_act_gen.trg >> creprc.c
cc -P -C -D${1} creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${1} -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cree asql_t15_act_int.trg
echo "asql_t15_act_int.trg" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzahc.h\"" >> creprc.c
cat asql_t15_act_int.trg >> creprc.c
cc -P -C -D${1} creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${1} -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cree asql_t16_act_sis.trg
echo "asql_t16_act_sis.trg" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzahc.h\"" >> creprc.c
cat asql_t16_act_sis.trg >> creprc.c
cc -P -C -D${1} creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${1} -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cree asql_t17_act_dep.trg
echo "asql_t17_act_dep.trg" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzahc.h\"" >> creprc.c
cat asql_t17_act_dep.trg >> creprc.c
cc -P -C -D${1} creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${1} -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cree asql_t18_act_rmq.trg
echo "asql_t18_act_rmq.trg" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzahc.h\"" >> creprc.c
cat asql_t18_act_rmq.trg >> creprc.c
cc -P -C -D${1} creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${1} -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cree asql_t19_act_frg.trg
echo "asql_t19_act_frg.trg" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzahc.h\"" >> creprc.c
cat asql_t19_act_frg.trg >> creprc.c
cc -P -C -D${1} creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${1} -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cree asql_t20_act_gar.trg
echo "asql_t20_act_gar.trg" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzahc.h\"" >> creprc.c
cat asql_t20_act_gar.trg >> creprc.c
cc -P -C -D${1} creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${1} -i creprc.i >> creprc.err
rm creprc.c creprc.i
fi

if [[ $1 != "CI" && $type != "PC_SIMPL" ]]
then
# cree asql_t22_fmc_trs.trg
echo "asql_t22_fmc_trs.trg" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzahc.h\"" >> creprc.c
cat asql_t22_fmc_trs.trg >> creprc.c
cc -P -C -D${1} creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${1} -i creprc.i >> creprc.err
rm creprc.c creprc.i

fi

if [[ $1 = "CI" ]] || [[ $1 = "CA" ]]
then
# cree asql_t23_alt_exp.trg
echo "asql_t23_alt_exp.trg" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzahc.h\"" >> creprc.c
cat asql_t23_alt_exp.trg >> creprc.c
cc -P -C -D${1} creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${1} -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cree asql_t24_mes_niv.trg
echo "asql_t24_mes_niv.trg" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzahc.h\"" >> creprc.c
cat asql_t24_mes_niv.trg >> creprc.c
cc -P -C -D${1} creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${1} -i creprc.i >> creprc.err
rm creprc.c creprc.i

fi

