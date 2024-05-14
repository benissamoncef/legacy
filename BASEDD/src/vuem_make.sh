#!/bin/ksh
# Fichier : @(#)xzas_make.sh	1.6      Release : 1.6        Date : 02/01/95
##################################################################################
# STERIA # PROJET MIGRAZUR
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# MODULE XZAS # FICHIER xzas_make.sh
##################################################################################
# DESCRIPTION DU MODULE :
#  compile les procedures stock휿 des vues metiers sur SQL_$1 
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# C.TORREGROSSA	31/07/95	: Creation
# C.TORREGROSSA	28/09/95	: Ajout de xzae90sp.prc
# C.TORREGROSSA	01/07/96	: Ajout de vuem51,vuem52
# C.TORREGROSSA	19/09/96	: Suppression de xzae90sp.prc, vuem50, vuem51,vuem52
# L.VATHELOT	16/04/97	: Ajout des procs stockees du lot n�2
# C.TORREGROSSA	24/02/98	: Ajout de vuem50
# P.NIEPCERON	09/07/98	: separation EXP/HIS (dem/1696) 
# JPL		16/08/17	: Suppression inclusion de vuemc.h par vuem06
##################################################################################

# repertoire des includes
includes=$HOME


# efface les procedures du module
echo " 
drop procedure VUEM01 
drop procedure VUEM02 
drop procedure VUEM03 
drop procedure VUEM04 
drop procedure VUEM05 
drop procedure VUEM06 
drop procedure VUEM07 
drop procedure VUEM08 
drop procedure VUEM11
drop procedure VUEM14 
drop procedure VUEM15 
drop procedure VUEMb15 
drop procedure VUEM18 
drop procedure VUEM19 
drop procedure VUEM09
drop procedure VUEM12
drop procedure VUEM16
drop procedure VUEM21
drop procedure VUEM22
drop procedure VUEM24
drop procedure VUEM25
drop procedure VUEM26
drop procedure VUEM28
drop procedure VUEM28b
drop procedure VUEM50
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_$1

if [[ $1 = "HIS" ]]
then 
	Compilation="HIST"
else
	Compilation=$1
fi


# initialiser le fichier de trace des erreurs
echo "trace de la creation des procedures de XZAS" > creprc.err
echo "" >> creprc.err

if [[ $1 = "HIS" ]]
then 
# cr휿 VUEM01
echo "vuem01sp.prc" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
cat vuem01sp.prc >> creprc.c
cc -P -C -D${Compilation} creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_$1 -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cr휿 VUEM02
echo "vuem02sp.prc" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
cat vuem02sp.prc >> creprc.c
cc -P -C -D${Compilation} creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_$1 -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cr휿 VUEM03
echo "vuem03sp.prc" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
cat vuem03sp.prc >> creprc.c
cc -P -C -D${Compilation} creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_$1 -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cr휿 VUEM04
echo "vuem04sp.prc" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
cat vuem04sp.prc >> creprc.c
cc -P -C -D${Compilation} creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_$1 -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cr휿 VUEM05
echo "vuem05sp.prc" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
cat vuem05sp.prc >> creprc.c
cc -P -C -D${Compilation} creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_$1 -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cr휿 VUEM06
echo "vuem06sp.prc" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
cat vuem06sp.prc >> creprc.c
cc -P -C -D${Compilation} creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_$1 -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cr휿 VUEM07
echo "vuem07sp.prc" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
cat vuem07sp.prc >> creprc.c
cc -P -C -D${Compilation} creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_$1 -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cr휿 VUEM08
echo "vuem08sp.prc" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
cat vuem08sp.prc >> creprc.c
cc -P -C -D${Compilation} creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_$1 -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cr휿 VUEM11
echo "vuem11sp.prc" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
cat vuem11sp.prc >> creprc.c
cc -P -C -D${Compilation} creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_$1 -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cr휿 VUEM14
echo "vuem14sp.prc" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
cat vuem14sp.prc >> creprc.c
cc -P -C -D${Compilation} creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_$1 -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cr휿 VUEM15
echo "vuem15sp.prc" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
cat vuem15sp.prc >> creprc.c
cc -P -C -D${Compilation} creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_$1 -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cr휿 VUEMb15
echo "vuemb15sp.prc" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
cat vuemb15sp.prc >> creprc.c
cc -P -C -D${Compilation} creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_$1 -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cr휿 VUEM18
echo "vuem18sp.prc" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
cat vuem18sp.prc >> creprc.c
cc -P -C -D${Compilation} creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_$1 -i creprc.i >> creprc.err
rm creprc.c creprc.i
fi

# cr휿 VUEM19
echo "vuem19sp.prc" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
cat vuem19sp.prc >> creprc.c
cc -P -C -D${Compilation} creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_$1 -i creprc.i >> creprc.err
rm creprc.c creprc.i

if [[ $1 = "HIS" ]]
then 
# cr휿 VUEM09
echo "vuem09sp.prc" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
cat vuem09sp.prc >> creprc.c
cc -P -C -D${Compilation} creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_$1 -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cr휿 VUEM12
echo "vuem12sp.prc" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
cat vuem12sp.prc >> creprc.c
cc -P -C -D${Compilation} creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_$1 -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cr휿 VUEM16
echo "vuem16sp.prc" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
cat vuem16sp.prc >> creprc.c
cc -P -C -D${Compilation} creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_$1 -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cr휿 VUEM21
echo "vuem21sp.prc" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
cat vuem21sp.prc >> creprc.c
cc -P -C -D${Compilation} creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_$1 -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cr휿 VUEM22
echo "vuem22sp.prc" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
cat vuem22sp.prc >> creprc.c
cc -P -C -D${Compilation} creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_$1 -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cr휿 VUEM24
echo "vuem24sp.prc" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
cat vuem24sp.prc >> creprc.c
cc -P -C -D${Compilation} creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_$1 -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cr휿 VUEM25
echo "vuem25sp.prc" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
cat vuem25sp.prc >> creprc.c
cc -P -C -D${Compilation} creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_$1 -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cr휿 VUEM26
echo "vuem26sp.prc" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
cat vuem26sp.prc >> creprc.c
cc -P -C -D${Compilation} creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_$1 -i creprc.i >> creprc.err
rm creprc.c creprc.i

# crEe VUEM28b
echo "vuem28sp.prc" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
cat vuem28sp.prc >> creprc.c
cc -P -C -D${Compilation} creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_$1 -i creprc.i >> creprc.err
rm creprc.c creprc.i

# crEe VUEM28b
echo "vuem28bsp.prc" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
cat vuem28bsp.prc >> creprc.c
cc -P -C -D${Compilation} creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_$1 -i creprc.i >> creprc.err
rm creprc.c creprc.i

# cr휿 VUEM50
echo "vuem50sp.prc" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
cat vuem50sp.prc >> creprc.c
cc -P -C -D${Compilation} creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_$1 -i creprc.i >> creprc.err
rm creprc.c creprc.i
fi

