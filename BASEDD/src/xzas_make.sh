#!/bin/ksh
# Fichier : $Id: xzas_make.sh,v 1.24 2018/06/19 17:38:10 devgfi Exp $      Release : $Revision: 1.24 $        Date : $Date: 2018/06/19 17:38:10 $
##################################################################################
# STERIA # PROJET MIGRAZUR
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# MODULE XZAS # FICHIER xzas_make.sh
##################################################################################
# DESCRIPTION DU MODULE :
#  compile les procedures stockee de XZAS sur SQL_CI ou SQL_DN ou SQL_DM
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# C.TORREGROSSA	26/10/94	: Creation
# C.TORREGROSSA	15/11/94	: Ajout la compilation conditionnelle
# T.FOURNEL	23/12/94    	: Ajout de la procedure xzas22sp.prc (XZAS;22)
# T.FOURNEL 26/12/94    	: Ajout de la procedure xzas20sp.prc (XZAS;20)
#                       	: Ajout de la procedure xzas21sp.prc (XZAS;21)
#                       	: Ajout de la procedure xzas21sp.prc (XZAS;23)
#                       	: Ajout de la procedure xzas21sp.prc (XZAS;24)
#                       	: Ajout de la procedure xzas21sp.prc (XZAS;25)
#                       	: Ajout de la procedure xzas21sp.prc (XZAS;26)
#                       	: Ajout de la procedure xzas21sp.prc (XZAS;30)
#                       	: Ajout de la procedure xzas21sp.prc (XZAS;31)
#                       	: Ajout de la procedure xzas21sp.prc (XZAS;32)
#                       	: Ajout de la procedure xzas21sp.prc (XZAS;34)
# C.TORREGROSSA	27/01/95	: Suppression des XZAS;10 a 12
# C.TORREGROSSA	25/03/96	: Modif include pour XZAS;34 
# C.TORREGROSSA	18/09/96	: Ajout compilation sur DC et DS 
# GUILHOU	09/05/97		: ajout XZAS55 a XZAS57 (strada)
# PENOT		15/05/97		: correction pour compil sur PC simplifie
# GUILHOU	09/01/98		: gestion multi-sites (dem/1536) 1.12
# NIEPCERON	30/06/98		: separation EXP/HIS 1.13
# NIEPCERON 06/01/98		: ajout xzas70 dem/1724 1.14
# NIEPCERON 29/03/99		: xzas 58 -> 60 1.16
# NIEPCERON 09/01/07		: xzas91   DEM605 v1.18
# JMG		15/09/08		: ajout xzas85 xzas81 xzas86 xzas100 DEM 836
# NIEPCERON 18/03/09		: ajout xzas101 102 103
# JPL		08/04/09		: ajout xzas88 manquante  1.22
# PNI		19/12/12		: ajout use PRC 1.23
# JPL		19/06/18		: Migration Linux 64 bits (DEM 1274) : phase de precompilation  1.24
# GGY   	15/09/2023  	: Adaptation pour CNA (DEM-483)
###################################################################################

# repertoire des includes
includes=$HOME

if [[ $# != 1 && $# != 2 ]]
then
        echo "Usage : xzas_make.sh NomSite TypeSite"
        echo "        NomSite : CI ou HIS  ou DP ou VC  ou DM ou DS ou DC ..."
        echo "        TypeSite : PC3 ou autre chose"
        exit 1
fi

typeset -u Nom_Site=$1

Fichier_Compil="xzas${Nom_Site}"


# Directives de compilation selon le site
Option_Site="-D${Nom_Site}"
if [[ "$Nom_Site" = "HIS" ]]
then
	Option_Site="-DHIST"
fi


# Type de site
if [[ $# = 1 ]]
then
        type="xx"
else
        type=$2
fi


# Effacer les procedures du module
echo " 
use PRC
go
drop procedure XZAS 
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
echo " 
use PRC
go
drop procedure XZAS53
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
echo " 
use PRC
go
drop procedure XZAS54
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}

if [[ ${Nom_Site} = "CI" ]] || [[ ${Nom_Site} = "CA" ]]
then
echo "
use PRC
go
drop procedure XZAS55
drop procedure XZAS56
drop procedure XZAS57
drop procedure XZAS58
drop procedure XZAS59
drop procedure XZAS60
drop procedure XZAS62
drop procedure XZAS70
drop procedure XZAS80
drop procedure XZAS81
drop procedure XZAS82
drop procedure XZAS83
drop procedure XZAS84
drop procedure XZAS85
drop procedure XZAS86
drop procedure XZAS87
drop procedure XZAS100
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
fi

# initialiser le fichier de trace des erreurs
echo "Trace de la creation des procedures de XZAS" > xzas.err
echo "" >> xzas.err

# cree XZAS;01
echo "xzas01sp.prc" >> xzas.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzas.c
cat xzas01sp.prc >> xzas.c
cpp -E -P -C  ${Option_Site} xzas.c  > xzas.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzas.i >> xzas.err
rm xzas.c xzas.i


# cree XZAS;02
echo "xzas02sp.prc" >> xzas.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzas.c
cat xzas02sp.prc >> xzas.c
cpp -E -P -C  ${Option_Site} xzas.c  > xzas.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzas.i >> xzas.err
rm xzas.c xzas.i


# cree XZAS;03
if [[ $type != "PC3" ]]
then
echo "xzas03sp.prc" >> xzas.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> xzas.c
cat xzas03sp.prc >> xzas.c
cpp -E -P -C  ${Option_Site} xzas.c  > xzas.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzas.i >> xzas.err
rm xzas.c xzas.i

# cree XZAS;31
echo "xzas31sp.prc" >> xzas.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzas.c
cat xzas31sp.prc >> xzas.c
cpp -E -P -C  ${Option_Site} xzas.c  > xzas.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzas.i >> xzas.err
rm xzas.c xzas.i


fi


# cree XZAS;04
echo "xzas04sp.prc" >> xzas.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzas.c
cat xzas04sp.prc >> xzas.c
cpp -E -P -C  ${Option_Site} xzas.c  > xzas.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzas.i >> xzas.err
rm xzas.c xzas.i


# cree XZAS;06
echo "xzas06sp.prc" >> xzas.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzas.c
cat xzas06sp.prc >> xzas.c
cpp -E -P -C  ${Option_Site} xzas.c  > xzas.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzas.i >> xzas.err
rm xzas.c xzas.i


# cree XZAS;20
echo "xzas20sp.prc" >> xzas.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzas.c
cat xzas20sp.prc >> xzas.c
cpp -E -P -C  ${Option_Site} xzas.c  > xzas.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzas.i >> xzas.err
rm xzas.c xzas.i

# cree XZAS;21
echo "xzas21sp.prc" >> xzas.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzas.c
cat xzas21sp.prc >> xzas.c
cpp -E -P -C  ${Option_Site} xzas.c  > xzas.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzas.i >> xzas.err
rm xzas.c xzas.i

if [[ $1 != "HIS" ]]
then
# cree XZAS;22
echo "xzas22sp.prc" >> xzas.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzas.c
cat xzas22sp.prc >> xzas.c
cpp -E -P -C  ${Option_Site} xzas.c  > xzas.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzas.i >> xzas.err
rm xzas.c xzas.i
fi

# cree XZAS;23
echo "xzas23sp.prc" >> xzas.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzas.c
cat xzas23sp.prc >> xzas.c
cpp -E -P -C  ${Option_Site} xzas.c  > xzas.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzas.i >> xzas.err
rm xzas.c xzas.i

# cree XZAS;24
echo "xzas24sp.prc" >> xzas.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> xzas.c
cat xzas24sp.prc >> xzas.c
cpp -E -P -C  ${Option_Site} xzas.c  > xzas.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzas.i >> xzas.err
rm xzas.c xzas.i

# cree XZAS;25
echo "xzas25sp.prc" >> xzas.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzas.c
cat xzas25sp.prc >> xzas.c
cpp -E -P -C  ${Option_Site} xzas.c  > xzas.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzas.i >> xzas.err
rm xzas.c xzas.i

# cree XZAS;26
echo "xzas26sp.prc" >> xzas.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzas.c
cat xzas26sp.prc >> xzas.c
cpp -E -P -C  ${Option_Site} xzas.c  > xzas.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzas.i >> xzas.err
rm xzas.c xzas.i

# cree XZAS;30
echo "xzas30sp.prc" >> xzas.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzas.c
cat xzas30sp.prc >> xzas.c
cpp -E -P -C  ${Option_Site} xzas.c  > xzas.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzas.i >> xzas.err
rm xzas.c xzas.i

# cree XZAS;32
echo "xzas32sp.prc" >> xzas.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzas.c
cat xzas32sp.prc >> xzas.c
cpp -E -P -C  ${Option_Site} xzas.c  > xzas.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzas.i >> xzas.err
rm xzas.c xzas.i

# cree XZAS;34
echo "xzas34sp.prc" >> xzas.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> xzas.c
cat xzas34sp.prc >> xzas.c
cpp -E -P -C  ${Option_Site} xzas.c  > xzas.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzas.i >> xzas.err
rm xzas.c xzas.i

# crEe XZAS;53
echo "xzas53sp.prc" >> xzas.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> xzas.c
cat xzas53sp.prc >> xzas.c
cpp -E -P -C  ${Option_Site} xzas.c  > xzas.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzas.i >> xzas.err
rm xzas.c xzas.i

if [[ $1 != "HIS" ]]
then
# crEe XZAS;54
echo "xzas54sp.prc" >> xzas.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> xzas.c
cat xzas54sp.prc >> xzas.c
cpp -E -P -C  ${Option_Site} xzas.c  > xzas.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzas.i >> xzas.err
rm xzas.c xzas.i

if [[ $1 = "CI" ]] || [[ $1 = "CA" ]]
then
# crEe XZAS;62
echo "xzas62sp.prc" >> xzas.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> xzas.c
cat xzas62sp.prc >> xzas.c
cpp -E -P -C  ${Option_Site} xzas.c  > xzas.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzas.i >> xzas.err
rm xzas.c xzas.i
# crEe XZAS;60
echo "xzas60sp.prc" >> xzas.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> xzas.c
cat xzas60sp.prc >> xzas.c
cpp -E -P -C  ${Option_Site} xzas.c  > xzas.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzas.i >> xzas.err
rm xzas.c xzas.i
# crEe XZAS;70
echo "xzas70sp.prc" >> xzas.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> xzas.c
cat xzas70sp.prc >> xzas.c
cpp -E -P -C  ${Option_Site} xzas.c  > xzas.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzas.i >> xzas.err
rm xzas.c xzas.i

# crEe XZAS;55
echo "xzas55sp.prc" >> xzas.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> xzas.c
cat xzas55sp.prc >> xzas.c
cpp -E -P -C  ${Option_Site} xzas.c  > xzas.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzas.i >> xzas.err
rm xzas.c xzas.i

# cree XZAS;56
echo "xzas56sp.prc" >> xzas.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> xzas.c
cat xzas56sp.prc >> xzas.c
cpp -E -P -C  ${Option_Site} xzas.c  > xzas.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzas.i >> xzas.err
rm xzas.c xzas.i


echo "xzas57sp.prc" >> xzas.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> xzas.c
cat xzas57sp.prc >> xzas.c
cpp -E -P -C  ${Option_Site} xzas.c  > xzas.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzas.i >> xzas.err
rm xzas.c xzas.i
echo "xzas58p.prc" >> xzas.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> xzas.c
cat xzas58sp.prc >> xzas.c
cpp -E -P -C  ${Option_Site} xzas.c  > xzas.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzas.i >> xzas.err
rm xzas.c xzas.i
echo "xzas59sp.prc" >> xzas.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> xzas.c
cat xzas59sp.prc >> xzas.c
cpp -E -P -C  ${Option_Site} xzas.c  > xzas.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzas.i >> xzas.err
rm xzas.c xzas.i

echo "xzas80sp.prc" >> xzas.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> xzas.c
cat xzas80sp.prc >> xzas.c
cpp -E -P -C  ${Option_Site} xzas.c  > xzas.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzas.i >> xzas.err
rm xzas.c xzas.i

echo "xzas81sp.prc" >> xzas.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> xzas.c
cat xzas81sp.prc >> xzas.c
cpp -E -P -C  ${Option_Site} xzas.c  > xzas.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzas.i >> xzas.err
rm xzas.c xzas.i

echo "xzas82sp.prc" >> xzas.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> xzas.c
cat xzas82sp.prc >> xzas.c
cpp -E -P -C  ${Option_Site} xzas.c  > xzas.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzas.i >> xzas.err
rm xzas.c xzas.i

echo "xzas83sp.prc" >> xzas.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> xzas.c
cat xzas83sp.prc >> xzas.c
cpp -E -P -C  ${Option_Site} xzas.c  > xzas.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzas.i >> xzas.err
rm xzas.c xzas.i

echo "xzas84sp.prc" >> xzas.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> xzas.c
cat xzas84sp.prc >> xzas.c
cpp -E -P -C  ${Option_Site} xzas.c  > xzas.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzas.i >> xzas.err
rm xzas.c xzas.i

echo "xzas85sp.prc" >> xzas.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> xzas.c
cat xzas85sp.prc >> xzas.c
cpp -E -P -C  ${Option_Site} xzas.c  > xzas.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzas.i >> xzas.err
rm xzas.c xzas.i

echo "xzas86sp.prc" >> xzas.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> xzas.c
cat xzas86sp.prc >> xzas.c
cpp -E -P -C  ${Option_Site} xzas.c  > xzas.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzas.i >> xzas.err
rm xzas.c xzas.i

echo "xzas87sp.prc" >> xzas.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> xzas.c
cat xzas87sp.prc >> xzas.c
cpp -E -P -C  ${Option_Site} xzas.c  > xzas.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzas.i >> xzas.err
rm xzas.c xzas.i

echo "xzas88sp.prc" >> xzas.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> xzas.c
cat xzas88sp.prc >> xzas.c
cpp -E -P -C  ${Option_Site} xzas.c  > xzas.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzas.i >> xzas.err
rm xzas.c xzas.i

echo "xzas91sp.prc" >> xzas.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> xzas.c
cat xzas91sp.prc >> xzas.c
cpp -E -P -C  ${Option_Site} xzas.c  > xzas.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzas.i >> xzas.err
rm xzas.c xzas.i


echo "xzas100sp.prc" >> xzas.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> xzas.c
cat xzas100sp.prc >> xzas.c
cpp -E -P -C  ${Option_Site} xzas.c  > xzas.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzas.i >> xzas.err
rm xzas.c xzas.i

echo "xzas101sp.prc" >> xzas.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> xzas.c
cat xzas101sp.prc >> xzas.c
cpp -E -P -C  ${Option_Site} xzas.c  > xzas.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzas.i >> xzas.err
rm xzas.c xzas.i
echo "xzas102sp.prc" >> xzas.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> xzas.c
cat xzas102sp.prc >> xzas.c
cpp -E -P -C  ${Option_Site} xzas.c  > xzas.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzas.i >> xzas.err
rm xzas.c xzas.i
echo "xzas103sp.prc" >> xzas.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> xzas.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> xzas.c
cat xzas103sp.prc >> xzas.c
cpp -E -P -C  ${Option_Site} xzas.c  > xzas.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i xzas.i >> xzas.err
rm xzas.c xzas.i

fi
fi
