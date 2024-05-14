#!/bin/ksh
# Fichier : $Id: xzat_make.sh,v 1.22 2018/05/29 16:08:50 devgfi Exp $        $Revision: 1.22 $        $Date: 2018/05/29 16:08:50 $
##################################################################################
# STERIA # PROJET MIGRAZUR
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# MODULE XZAT # FICHIER xzat_make.sh
##################################################################################
# DESCRIPTION DU MODULE :
#  compile les procedures stockees du groupe XZAT sur un serveur donne.
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# C.T.			22/09/94	: Creation
# C.T.			22/09/94	: Ajout le test sur le site (V 1.3)
# C.T.			14/11/94	: Ajout la compiltation conditionnelle (V 1.4)
# C.T.			17/11/94	: Correction erreur multi site (V 1.5)
# C.T.			29/11/94	: xzag18 en compil. condition. (V 1.6)
# C.T.			17/02/95	: ajout de xzat26 (V 1.7)
# C.T.			04/09/96	: ajout cas des serveurs DS et DC (V 1.8)
#                             compilation conditionnelle PC simplifie
# JMG			09/01/98	: gestion multi-sites (dem/1536) 1.9
# C.T.			16/01/98	: ajout de xzat27 xzat28 (1538) (V 1.10)
# P.N.			03/07/98	: separation EXP/HIS 1.11
# Hilmarcher    05/05/01    : ajout XZAT280 v1.7?
# JMG			28/09/05	: ajout xzat35 xzat36 xzat050
# VR			16/04/12	: ajout xzat51 xzat060 xzat520 xzat100 (DEM/1016)
# JPL			18/03/13	: ajout xzat60 a xzat63 et xzat65 a xzat68
# JPL			25/05/18	: ajout xzat70 a xzat73 et xzat75 a xzat78
# JPL			29/05/18	: Suppression d'instructions "drop proc" inutiles
# JPL			29/05/18	: Migration Linux: phase de precompilation
# GGY   		15/09/2023  : Adaptation pour CNA (DEM-483)
##################################################################################

# repertoire des includes
includes=$HOME

# controle la syntaxe d'appel
if [[ $# != 1 && $# != 2 ]]
then
        echo "syntaxe : xzat_make.ksh NomSite TypeSite"
        echo "          NomSite : CI ou HIS  ou DP ou VC  ou DM ou DS ou DC ..."
        echo "          Type : PC3 ou autre chose"
        exit 1
fi

typeset -u Nom_Site=$1


if [[ $# = 1 ]]
then
        type="xx"
else
        type=$2
fi


# Directives de compilation selon le site
if [[ $type != "PC3" ]]
then
	typeset -u Option_Site=$1
	if [[ "$Option_Site" = "HIS" ]]
	then
		Option_Site="HIST"
	fi
else
	Option_Site="PC_SIMPL"
fi


# Directives de compilation selon le systeme
if [[ "$SYSTEME" = "HP-UX" ]]
then
	PATH=$PATH:/usr/ccs/lbin
fi



# efface les procedures du module
echo " 
use PRC
go
drop procedure XZAT 
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}

# initialiser le fichier de trace des erreurs
echo "trace de la creation des procedures de XZAT" > xzat.err
echo "" >> xzat.err


# cree XZAT;01
echo "xzat01.prc" >> xzat.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
cat xzat01sp.prc >> xzat.c
cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
rm xzat.c xzat.i


if [[ $1 != "HIS" ]]
then
# cree XZAT;03
echo "xzat03.prc" >> xzat.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
cat xzat03sp.prc >> xzat.c
cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
rm xzat.c xzat.i


# cree XZAT;04
echo "xzat04.prc" >> xzat.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
cat xzat04sp.prc >> xzat.c
cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
rm xzat.c xzat.i
fi

# cree XZAT;05
echo "xzat05.prc" >> xzat.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
cat xzat05sp.prc >> xzat.c
cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
rm xzat.c xzat.i

# cree XZAT050
echo "xzat050.prc" >> xzat.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
cat xzat050sp.prc >> xzat.c
cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
rm xzat.c xzat.i

# cree XZAT060
echo "xzat060.prc" >> xzat.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
cat xzat060sp.prc >> xzat.c
cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
rm xzat.c xzat.i

# cree XZAT;06
echo "xzat06.prc" >> xzat.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
cat xzat06sp.prc >> xzat.c
cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
rm xzat.c xzat.i

# cree XZAT35
echo "xzat35.prc" >> xzat.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
cat xzat35sp.prc >> xzat.c
cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
rm xzat.c xzat.i

# cree XZAT51
echo "xzat51.prc" >> xzat.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
cat xzat51sp.prc >> xzat.c
cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
rm xzat.c xzat.i

# cree XZAT511
echo "xzat511.prc" >> xzat.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
cat xzat511sp.prc >> xzat.c
cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
rm xzat.c xzat.i

# cree XZAT36
echo "xzat36.prc" >> xzat.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
cat xzat36sp.prc >> xzat.c
cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
rm xzat.c xzat.i



if [[ $1 != "HIS" ]]
then
# cree XZAT;10
echo "xzat10.prc" >> xzat.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
cat xzat10sp.prc >> xzat.c
cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
rm xzat.c xzat.i


# cree XZAT;11
echo "xzat11.prc" >> xzat.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
cat xzat11sp.prc >> xzat.c
cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
rm xzat.c xzat.i


# cree XZAT;18
echo "xzat18.prc" >> xzat.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
cat xzat18sp.prc >> xzat.c
cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
rm xzat.c xzat.i

# cree XZAT;19
echo "xzat19.prc" >> xzat.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
cat xzat19sp.prc >> xzat.c
cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
rm xzat.c xzat.i


# cree XZAT;20
echo "xzat20.prc" >> xzat.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
cat xzat20sp.prc >> xzat.c
cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
rm xzat.c xzat.i

# cree XZAT;21
echo "xzat21.prc" >> xzat.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
cat xzat21sp.prc >> xzat.c
cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
rm xzat.c xzat.i

# cree XZAT;27
echo "xzat27.prc" >> xzat.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
cat xzat27sp.prc >> xzat.c
cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
rm xzat.c xzat.i
fi

# cree XZAT;28
#echo "xzat28.prc" >> xzat.err
#echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
#cat xzat28sp.prc >> xzat.c
#cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
#isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
#rm xzat.c xzat.i

# cree XZAT280
echo "xzat280.prc" >> xzat.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
cat xzat280sp.prc >> xzat.c
cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
rm xzat.c xzat.i


if [[ $type != "PC3" ]]
then
# cree XZAT;07
echo "xzat07.prc" >> xzat.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
cat xzat07sp.prc >> xzat.c
cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
rm xzat.c xzat.i


# cree XZAT;08
echo "xzat08.prc" >> xzat.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
cat xzat08sp.prc >> xzat.c
cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
rm xzat.c xzat.i


# cree XZAT;09
echo "xzat09.prc" >> xzat.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
cat xzat09sp.prc >> xzat.c
cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
rm xzat.c xzat.i

if [[ $1 != "HIS" ]]
then
# cree XZAT;22
echo "xzat22.prc" >> xzat.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
cat xzat22sp.prc >> xzat.c
cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
rm xzat.c xzat.i

# cree XZAT;23
echo "xzat23.prc" >> xzat.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
cat xzat23sp.prc >> xzat.c
cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
rm xzat.c xzat.i

# AAZ 28/0/2007 ajout xzat40..xzat44 
# cree XZAT;40
echo "xzat40.prc" >> xzat.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
cat xzat40sp.prc >> xzat.c
cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
rm xzat.c xzat.i


# cree XZAT;41
echo "xzat41.prc" >> xzat.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
cat xzat41sp.prc >> xzat.c
cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
rm xzat.c xzat.i

# cree XZAT;42
echo "xzat42.prc" >> xzat.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
cat xzat42sp.prc >> xzat.c
cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
rm xzat.c xzat.i

# cree XZAT;43
echo "xzat43.prc" >> xzat.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
cat xzat43sp.prc >> xzat.c
cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
rm xzat.c xzat.i

# cree XZAT;44
echo "xzat44.prc" >> xzat.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
cat xzat44sp.prc >> xzat.c
cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
rm xzat.c xzat.i

# cree XZAT;45
echo "xzat45.prc" >> xzat.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
cat xzat45sp.prc >> xzat.c
cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
rm xzat.c xzat.i

# AAZ 28/0/2007 ajout xzat40..xzat44 
# cree XZAT;45
echo "xzat45.prc" >> xzat.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
cat xzat45sp.prc >> xzat.c
cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
rm xzat.c xzat.i

fi
fi


if [[ $1 != "HIS" ]]
then
# cree XZAT;24
echo "xzat24.prc" >> xzat.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
cat xzat24sp.prc >> xzat.c
cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
rm xzat.c xzat.i


# cree XZAT;25
echo "xzat25.prc" >> xzat.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
cat xzat25sp.prc >> xzat.c
cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
rm xzat.c xzat.i

# cree XZAT100
echo "
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
echo "xzat100.prc" >> xzat.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
cat xzat100sp.prc >> xzat.c
cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
rm xzat.c xzat.i

# cree XZAT250
echo "xzat250.prc" >> xzat.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
cat xzat250sp.prc >> xzat.c
cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
rm xzat.c xzat.i

# cree XZAT520
echo "xzat520.prc" >> xzat.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
cat xzat520sp.prc >> xzat.c
cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
rm xzat.c xzat.i

# cree XZAT;26
echo "xzat26.prc" >> xzat.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
cat xzat26sp.prc >> xzat.c
cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
rm xzat.c xzat.i
fi

if [[ $1 = "CI" ]] || [[ $1 = "CA" ]]
then
# cree XZAT010 pour pilotage cam pc niv 2
# cree XZAT010
echo "xzat010sp.prc" >> xzat.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
cat xzat010sp.prc >> xzat.c
cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
rm xzat.c xzat.i
fi


if [[ $1 != "HIS" ]]
then
	# cree XZAT60
	echo "xzat60.prc" >> xzat.err
	echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
	cat xzat60sp.prc >> xzat.c
	cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
	rm xzat.c xzat.i

	# cree XZAT61
	echo "xzat61.prc" >> xzat.err
	echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
	echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> xzat.c
	cat xzat61sp.prc >> xzat.c
	cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
	rm xzat.c xzat.i

	# cree XZAT62
	echo "xzat62.prc" >> xzat.err
	echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
	echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> xzat.c
	cat xzat62sp.prc >> xzat.c
	cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
	rm xzat.c xzat.i

	# cree XZAT63
	echo "xzat63.prc" >> xzat.err
	echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
	cat xzat63sp.prc >> xzat.c
	cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
	rm xzat.c xzat.i

	# cree XZAT65
	echo "xzat65.prc" >> xzat.err
	echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
	cat xzat65sp.prc >> xzat.c
	cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
	rm xzat.c xzat.i

	# cree XZAT66
	echo "xzat66.prc" >> xzat.err
	echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
	echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> xzat.c
	cat xzat66sp.prc >> xzat.c
	cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
	rm xzat.c xzat.i

	# cree XZAT67
	echo "xzat67.prc" >> xzat.err
	echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
	echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> xzat.c
	cat xzat67sp.prc >> xzat.c
	cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
	rm xzat.c xzat.i

	# cree XZAT68
	echo "xzat68.prc" >> xzat.err
	echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
	cat xzat68sp.prc >> xzat.c
	cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
	rm xzat.c xzat.i

	# cree XZAT70
	echo "xzat70.prc" >> xzat.err
	echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
	cat xzat70sp.prc >> xzat.c
	cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
	rm xzat.c xzat.i

	# cree XZAT71
	echo "xzat71.prc" >> xzat.err
	echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
	echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> xzat.c
	cat xzat71sp.prc >> xzat.c
	cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
	rm xzat.c xzat.i

	# cree XZAT72
	echo "xzat72.prc" >> xzat.err
	echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
	echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> xzat.c
	cat xzat72sp.prc >> xzat.c
	cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
	rm xzat.c xzat.i

	# cree XZAT73
	echo "xzat73.prc" >> xzat.err
	echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
	cat xzat73sp.prc >> xzat.c
	cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
	rm xzat.c xzat.i

	# cree XZAT75
	echo "xzat75.prc" >> xzat.err
	echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
	cat xzat75sp.prc >> xzat.c
	cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
	rm xzat.c xzat.i

	# cree XZAT76
	echo "xzat76.prc" >> xzat.err
	echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
	echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> xzat.c
	cat xzat76sp.prc >> xzat.c
	cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
	rm xzat.c xzat.i

	# cree XZAT77
	echo "xzat77.prc" >> xzat.err
	echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
	echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> xzat.c
	cat xzat77sp.prc >> xzat.c
	cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
	rm xzat.c xzat.i

	# cree XZAT78
	echo "xzat78.prc" >> xzat.err
	echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzat.c
	cat xzat78sp.prc >> xzat.c
	cpp -E -P -C -D${Option_Site} xzat.c  > xzat.i  2>&-
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzat.i >> xzat.err
	rm xzat.c xzat.i

fi
