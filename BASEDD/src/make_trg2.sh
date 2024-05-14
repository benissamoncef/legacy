#!/bin/ksh
##################################################################################
# syntaxe : make_trg.sh fichier
##################################################################################

# repertoire des includes
includes=$HOME

if [[ $# = 0 ]]
then
	echo "syntaxe : make_prc.sh NomFichier NomServeur"
	echo "          NomFichier : avec extension"
	echo "          NomServeur : CI ou DM ou DN"
	exit 1
fi

# initialiser le fichier de trace des erreurs
echo "trace de la creation des procedures de $1" > creprc.err
echo "" >> creprc.err

echo "$1" >> creprc.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> creprc.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzahc.h\"" >> creprc.c
cat $1 >> creprc.c

cc -P -C creprc.c

isql -UMIGRAZUR -PMIGRAZUR -SSQL_$2 -i creprc.i >> creprc.err
rm creprc.c creprc.i
