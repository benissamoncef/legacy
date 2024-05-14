#!/bin/ksh
# Fichier : $Id: xzaa_make.ksh,v 1.17 2019/05/14 10:19:59 pc2dpdy Exp $        $Revision: 1.17 $        $Date: 2019/05/14 10:19:59 $
##################################################################################
# GTIE # PROJET MIGRAZUR
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# MODULE XZAA # FICHIER xzao_make.ksh
##################################################################################
# DESCRIPTION DU MODULE :
#  Compile les procedures stockees de XZAA sur le serveur d'un site donne.
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# F. VOLCIC	27/10/94	: Creation
# Guilhou	29/12/94	: compil par site			V1.2
# Guilhou	01/12/97	: ajout xzaa03 (strada)			V1.3
# Penot		07/01/98	: Modification xzaa03 qu'au CI		V1.6
# Guilhou	09/01/98	: gestion multi-sites			1.7
# C.T.		06/03/98	: ajout xzaa07 (1606)			1.8-1.9
# P.N.		07/07/98	: separation EXP/HIS (1696) 1.10
# JMG		24/03/11	: ajout XZZA21				1.11
# JMG		10/04/12 	: ajout xzaac.h pour xzaa11 			1.12 1.13
# VR		16/04/12 	: ajout xzaa30 1.12 (DEM/1016)
# JPL		12/07/18 	: Migration Linux 64 bits (DEM 1274) : phase de precompilation  1.16
# JMG		15/05/19 	: ajout include xzaac pour xzaa01 GAPLA	1.17
# GGY   	15/09/2023  : Adaptation pour CNA (DEM-483)
##################################################################################

# repertoire des includes
includes=$HOME

if [[ $# != 1 && $# != 2 ]]
then
	echo "Usage : xzaa_make.ksh NomSite TypeSite"
	echo "          NomSite : CI ou HIS  ou DP ou VC  ou DM ou DS ou DC ..."
	echo "          Type : PC3 ou autre chose"
	exit 1
fi

typeset -u Nom_Site=$1

Fichier_Compil="xzaa${Nom_Site}"


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

#if [[ "$type" = "PC3" ]]
#then
#	echo	"          ne fonctionne qu'au CI et sur PC niveau 1 et 2"
#	exit 0
#fi


# Effacer les procedures du module
echo "
use PRC
go
drop procedure XZAA
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}


# Initialiser le fichier de trace des erreurs
echo "trace de la creation des procedures de XZAA" > ${Fichier_Compil}.err
echo "" >> ${Fichier_Compil}.err


# cree XZAA;06
echo "xzaa06sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzaa06sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

if [[ "$Nom_Site" != "HIS" ]]
then

# cree XZAA;01
echo "xzaa01sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaac.h\"" >> ${Fichier_Compil}.c
cat xzaa01sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAA;02
echo "xzaa02sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaac.h\"" >> ${Fichier_Compil}.c
cat xzaa02sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

if [[ "$Nom_Site" = "CI" ]] || [[ "$Site_Compil" = "CA" ]]
then
# cree XZAA;03
echo "xzaa03sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaac.h\"" >> ${Fichier_Compil}.c
cat xzaa03sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i
fi


# cree XZAA;05
echo "xzaa05sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzaa05sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAA;07
if [[ "$Nom_Site" != "CI" ]] && [[ "$Site_Compil" != "CA" ]]
then
echo "xzaa07sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
cat xzaa07sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i
fi

# cree XZAA;10
echo "xzaa10sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzaa10sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

fi

# cree XZAA21
echo "xzaa21sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaac.h\"" >> ${Fichier_Compil}.c
cat xzaa21sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

if [[ "$Nom_Site" = "CI" ]] || [[ "$Site_Compil" = "CA" ]]
then
# cree XZAA30
echo "xzaa30sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaac.h\"" >> ${Fichier_Compil}.c
cat xzaa30sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i
fi

# cree XZAA11
echo "xzaa11sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaac.h\"" >> ${Fichier_Compil}.c
cat xzaa11sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

