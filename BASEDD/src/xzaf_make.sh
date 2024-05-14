#!/bin/ksh
# Fichier : $Id: xzaf_make.sh,v 1.3 1998/09/22 15:30:15 gaborit Exp $      Release : $Revision: 1.3 $        Date : $Date: 1998/09/22 15:30:15 $
##################################################################################
# STERIA # PROJET MIGRAZUR
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# MODULE XZAF # FICHIER xzaf_make.sh
##################################################################################
# DESCRIPTION DU MODULE :
#  compile les procedures stock휿 de XZAF sur SQL_${1}
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# C.TORREGROSSA	10/01/95	: Creation
# C.TORREGROSSA	23/01/95	: Ajout XZAF;16
##################################################################################

# repertoire des includes
includes=$HOME

# efface les procedures du module
echo " 
drop procedure XZAF 
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_$1

# initialiser le fichier de trace des erreurs
echo "trace de la creation des procedures de XZAF" > creprc.err
echo "" >> creprc.err

# cr휿 XZAF;10
echo "xzaf10sp.prc" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzahc.h\"" >> creprc.c
cat xzaf10sp.prc >> creprc.c
cc -P -C -DHIST creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_HIS -i creprc.i >> creprc.err
rm creprc.c creprc.i


# cr휿 XZAF;01
echo "xzaf01sp.prc" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
cat xzaf01sp.prc >> creprc.c
cc -P -C -DHIST creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_HIS -i creprc.i >> creprc.err
rm creprc.c creprc.i


# cr휿 XZAF;02
echo "xzaf02sp.prc" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzahc.h\"" >> creprc.c
cat xzaf02sp.prc >> creprc.c
cc -P -C -DHIST creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_HIS -i creprc.i >> creprc.err
rm creprc.c creprc.i


# cr휿 XZAF;03
echo "xzaf03sp.prc" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzahc.h\"" >> creprc.c
cat xzaf03sp.prc >> creprc.c
cc -P -C -DHIST creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_HIS -i creprc.i >> creprc.err
rm creprc.c creprc.i


# cr휿 XZAF;04
echo "xzaf04sp.prc" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzahc.h\"" >> creprc.c
cat xzaf04sp.prc >> creprc.c
cc -P -C -DHIST creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_HIS -i creprc.i >> creprc.err
rm creprc.c creprc.i


# cr휿 XZAF;05
echo "xzaf05sp.prc" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzahc.h\"" >> creprc.c
cat xzaf05sp.prc >> creprc.c
cc -P -C -DHIST creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_HIS -i creprc.i >> creprc.err
rm creprc.c creprc.i


# cr휿 XZAF;06
echo "xzaf06sp.prc" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzahc.h\"" >> creprc.c
cat xzaf06sp.prc >> creprc.c
cc -P -C -DHIST creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_HIS -i creprc.i >> creprc.err
rm creprc.c creprc.i


# cr휿 XZAF;07
echo "xzaf07sp.prc" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzahc.h\"" >> creprc.c
cat xzaf07sp.prc >> creprc.c
cc -P -C -DHIST creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_HIS -i creprc.i >> creprc.err
rm creprc.c creprc.i


# cr휿 XZAF;08
echo "xzaf08sp.prc" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzahc.h\"" >> creprc.c
cat xzaf08sp.prc >> creprc.c
cc -P -C -DHIST creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_HIS -i creprc.i >> creprc.err
rm creprc.c creprc.i


# cr휿 XZAF;09
echo "xzaf09sp.prc" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzahc.h\"" >> creprc.c
cat xzaf09sp.prc >> creprc.c
cc -P -C -DHIST creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_HIS -i creprc.i >> creprc.err
rm creprc.c creprc.i


# cr휿 XZAF;11
echo "xzaf11sp.prc" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzahc.h\"" >> creprc.c
cat xzaf11sp.prc >> creprc.c
cc -P -C -DHIST creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_HIS -i creprc.i >> creprc.err
rm creprc.c creprc.i


# cr휿 XZAF;12
echo "xzaf12sp.prc" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzahc.h\"" >> creprc.c
cat xzaf12sp.prc >> creprc.c
cc -P -C -DHIST creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_HIS -i creprc.i >> creprc.err
rm creprc.c creprc.i


# cr휿 XZAF;13
echo "xzaf13sp.prc" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzahc.h\"" >> creprc.c
cat xzaf13sp.prc >> creprc.c
cc -P -C -DHIST creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_HIS -i creprc.i >> creprc.err
rm creprc.c creprc.i


# cr휿 XZAF;14
echo "xzaf14sp.prc" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzahc.h\"" >> creprc.c
cat xzaf14sp.prc >> creprc.c
cc -P -C -DHIST creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_HIS -i creprc.i >> creprc.err
rm creprc.c creprc.i


if [[ $1 = "HIS" ]]
then
# cr휿 XZAF;16
echo "xzaf16sp.prc" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzahc.h\"" >> creprc.c
cat xzaf16sp.prc >> creprc.c
cc -P -C -DHIST creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_$1 -i creprc.i >> creprc.err
rm creprc.c creprc.i
else
# cr휿 XZAF;16
echo "xzaf16sp.prc" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzahc.h\"" >> creprc.c
cat xzaf16sp.prc >> creprc.c
cc -P -C -D$1 creprc.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_$1 -i creprc.i >> creprc.err
rm creprc.c creprc.i
fi
