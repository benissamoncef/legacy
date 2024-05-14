#!/bin/ksh
##################################################################################
# syntaxe : make_prc.sh xzae 14 CI
##################################################################################

# repertoire des includes
includes=$HOME

echo " 
drop procedure $1
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_CI

# initialiser le fichier de trace des erreurs
echo "trace de la creation des procedures de VUE" > creprc.err
echo "" >> creprc.err

echo "$2.prc" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c


cat $2.prc >> creprc.c
cc -P -C -DCI creprc.c

isql -UMIGRAZUR -PMIGRAZUR -SSQL_CI -i creprc.i >> creprc.err
rm creprc.c creprc.i


