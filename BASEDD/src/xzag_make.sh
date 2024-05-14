#!/bin/ksh
# Fichier : $Id: xzag_make.sh,v 1.21 2018/06/27 13:10:35 devgfi Exp $        $Revision: 1.21 $        $Date: 2018/06/27 13:10:35 $
##################################################################################
# STERIA # PROJET MIGRAZUR
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# MODULE XZAG # FICHIER xzag_make.sh
##################################################################################
# DESCRIPTION DU MODULE :
#  Compile les procedures stockées de XZAG sur le serveur d'un site donné.
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# C.TORREGROSSA	27/10/94	: Creation
# C.TORREGROSSA	15/11/94	: Ajout de XZAG13, XZAG14
# C.TORREGROSSA	17/11/94	: Correction test sur site
# C.TORREGROSSA	20/06/95	: Ajout XZAG15 a XZAG22
# C.TORREGROSSA	23/06/95	: Ajout include pour XZAG15
# C.TORREGROSSA	10/08/95	: Ajout XZAG25 a XZAG30 (V 1.6)
# C.TORREGROSSA	22/09/95	: Ajout compilation conditionnelle pour XZAG26 a XZAG31 (V 1.7)
# C.TORREGROSSA	22/09/95	: Ajout XZAG35 (V 1.8)
# C.TORREGROSSA 09/09/96        : compilation conditionnelle PC simplifie  (RADT 1.11)
# P.niepceron   09/10/96        : Ajout de xzag50,51,52 (dem/1227)  (RADT 1.12)
# P.niepceron   09/10/96        : Ajout de xzag53 (dem/1396) ( 1.13 )
# Guilhou	15/05/97	: pasde xzag20 sur les PC simplifies 1.14
# Guilhou	09/01/98	: gestion multi-sites (dem/1536) 1.15
# C.TORREGROSSA 12/01/98        : ajout xzag17 pour pc niveau3 (1532) 1.16
# C.TORREGROSSA 04/02/98        : pas de compil de xzag54 pour pc niveau3 (1536) 1.17
# P.niepceron   30/06/98        : separation EXP/HIS 1.18
# P.niepceron   30/09/98	: compil de xzag20 sur PCS 1.19
# C.hilmarcher  27/02/03        : Ajout de XZAG05 pour PC niveau 3
#				 + suppression compilation xzag20 pour HIS  v1.12?
# JPL	27/06/18 : Migration Linux 64 bits (DEM 1274) : phase de precompilation  1.21
##################################################################################

# repertoire des includes
includes=$HOME

if [[ $# != 1 && $# != 2 ]]
then
	echo "syntaxe : xzag_make.sh NomSite TypeSite"
	echo "          NomSite : CI ou DN ou DM ou DC ou DS ou ..."
	echo "          Type : PC3 ou autre chose"
	exit 1
fi

typeset -u Nom_Site=$1

Fichier_Compil="xzag${Nom_Site}"


# Type de site
if [[ $# = 1 ]]
then
        type="xx"
else
        type=$2
fi


# Directives de compilation selon le site
Option_Site="-D${Nom_Site}"
if [[ "$Nom_Site" = "HIS" ]]
then
	Option_Site="-DHIST"
fi

if [[ "$type" = "PC3" ]]
then
	Option_Site="-DPC_SIMPL"
fi


# efface les procedures du module
echo "
use PRC
go
drop procedure XZAG
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}


# initialiser le fichier de trace des erreurs
echo "trace de la creation des procedures de XZAG" > xzag$1.err
echo "" >> xzag$1.err


if [[ $1 != "HIS" ]]
then
# crée XZAG;10
echo "xzag10sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
cat xzag10sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAG;15
echo "xzag15sp.prc" >> ${Fichier_Compil}.err
 echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzagc.h\"" >> ${Fichier_Compil}.c
cat xzag15sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xzagc.h\"" >> ${Fichier_Compil}.c
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAG;17
echo "xzag17sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzagc.h\"" >> ${Fichier_Compil}.c
cat xzag17sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xzagc.h\"" >> ${Fichier_Compil}.c
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAG;18
echo "xzag18sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzagc.h\"" >> ${Fichier_Compil}.c
cat xzag18sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xzagc.h\"" >> ${Fichier_Compil}.c
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAG;21
echo "xzag21sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzagc.h\"" >> ${Fichier_Compil}.c
cat xzag21sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xzagc.h\"" >> ${Fichier_Compil}.c
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAG;22
echo "xzag22sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzagc.h\"" >> ${Fichier_Compil}.c
cat xzag22sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xzagc.h\"" >> ${Fichier_Compil}.c
rm ${Fichier_Compil}.c ${Fichier_Compil}.i
fi

# crée XZAG;25
echo "xzag25sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzagc.h\"" >> ${Fichier_Compil}.c
cat xzag25sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xzagc.h\"" >> ${Fichier_Compil}.c
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

if [[ $1 != "HIS" ]]
then
# crée XZAG;05
echo "xzag05sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
cat xzag05sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i
fi

###############Debut compil des proc a ne pas faire sur PC 3 ##########
if [[ $type != "PC3" ]]
then
# crée XZAG;11
echo "xzag11sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
cat xzag11sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAG;01
echo "xzag01sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
cat xzag01sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAG;02
echo "xzag02sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
cat xzag02sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAG;03
echo "xzag03sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
cat xzag03sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAG;04
echo "xzag04sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
cat xzag04sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAG;06
echo "xzag06sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
cat xzag06sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAG;07
echo "xzag07sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
cat xzag07sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAG;08
echo "xzag08sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
cat xzag08sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAG;09
echo "xzag09sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
cat xzag09sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAG;12
echo "xzag12sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
cat xzag12sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
rm ${Fichier_Compil}.c ${Fichier_Compil}.i


if [[ $1 != "HIS" ]]
then
# crée XZAG;13
echo "xzag13sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzagc.h\"" >> ${Fichier_Compil}.c
cat xzag13sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xzagc.h\"" >> ${Fichier_Compil}.c
rm ${Fichier_Compil}.c ${Fichier_Compil}.i
fi

# crée XZAG;14
echo "xzag14sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzagc.h\"" >> ${Fichier_Compil}.c
cat xzag14sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xzagc.h\"" >> ${Fichier_Compil}.c
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

if [[ $1 != "HIS" ]]
then
# crée XZAG;16
echo "xzag16sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzagc.h\"" >> ${Fichier_Compil}.c
cat xzag16sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xzagc.h\"" >> ${Fichier_Compil}.c
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAG;19
echo "xzag19sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzagc.h\"" >> ${Fichier_Compil}.c
cat xzag19sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xzagc.h\"" >> ${Fichier_Compil}.c
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAG;54
echo "xzag54sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
cat xzag54sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
rm ${Fichier_Compil}.c ${Fichier_Compil}.i
fi

fi
#########################Fin proc a ne pas faire sur PC 3 ###############
if [[ $1 != "HIS" ]]
# crEe XZAG;20
then
echo "xzag20sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzagc.h\"" >> ${Fichier_Compil}.c
cat xzag20sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xzagc.h\"" >> ${Fichier_Compil}.c
rm ${Fichier_Compil}.c ${Fichier_Compil}.i
fi

#########################Debut on ne fait qu au CI #####################
if [[ "$Nom_Site" = "CI" || "$Nom_Site" = "HIS" ]]
then
# crée XZAG;26
echo "xzag26sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzagc.h\"" >> ${Fichier_Compil}.c
cat xzag26sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xzagc.h\"" >> ${Fichier_Compil}.c
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAG;27
echo "xzag27sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzagc.h\"" >> ${Fichier_Compil}.c
cat xzag27sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xzagc.h\"" >> ${Fichier_Compil}.c
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAG;28
echo "xzag28sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzagc.h\"" >> ${Fichier_Compil}.c
cat xzag28sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xzagc.h\"" >> ${Fichier_Compil}.c
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAG;29
echo "xzag29sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzagc.h\"" >> ${Fichier_Compil}.c
cat xzag29sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xzagc.h\"" >> ${Fichier_Compil}.c
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAG;30
echo "xzag30sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzagc.h\"" >> ${Fichier_Compil}.c
cat xzag30sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xzagc.h\"" >> ${Fichier_Compil}.c
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAG;31
echo "xzag31sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzagc.h\"" >> ${Fichier_Compil}.c
cat xzag31sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xzagc.h\"" >> ${Fichier_Compil}.c
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

if [[ $1 != "HIS" ]]
then
# crée XZAG;100
echo "xzag100sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
cat xzag100sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
rm ${Fichier_Compil}.c ${Fichier_Compil}.i
fi
fi
#####################FIN de on ne fait qu au CI ####################

if [[ $1 != "HIS" ]]
then
# crée XZAG;35
echo "xzag35sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzagc.h\"" >> ${Fichier_Compil}.c
cat xzag35sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xzagc.h\"" >> ${Fichier_Compil}.c
rm ${Fichier_Compil}.c ${Fichier_Compil}.i
fi

# crée XZAG;40
echo "xzag40sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
cat xzag40sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
rm ${Fichier_Compil}.c ${Fichier_Compil}.i


# crée XZAG;50
echo "xzag50sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
cat xzag50sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
rm ${Fichier_Compil}.c ${Fichier_Compil}.i


if [[ $1 != "HIS" ]]
then
# crée XZAG;51
echo "xzag51sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
cat xzag51sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
rm ${Fichier_Compil}.c ${Fichier_Compil}.i
fi

# crée XZAG;52
echo "xzag52sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
cat xzag52sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAG;53
echo "xzag53sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
cat xzag53sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAG;55
echo "xzag55sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
cat xzag55sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
rm ${Fichier_Compil}.c ${Fichier_Compil}.i
