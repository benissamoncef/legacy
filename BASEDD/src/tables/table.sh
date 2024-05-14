if (($#!=3))
then
	echo "syntaxe : table.sh NomBase NomTable Site"
	echo "          NomBase en minuscule"
	echo "          NomTable en minuscule"
	echo "          Site : CI VC DP CA"
	exit 0
fi

cat ~/migrazur/BASEDD/src/$1.use  ~/migrazur/BASEDD/src/tables/$2.tab | isql -SSQL_$3 -UMIGRAZUR -PMIGRAZUR
cat ~/migrazur/BASEDD/src/$1.use  ~/migrazur/BASEDD/src/tables/$2.cle | isql -SSQL_$3 -UMIGRAZUR -PMIGRAZUR

exit 1
