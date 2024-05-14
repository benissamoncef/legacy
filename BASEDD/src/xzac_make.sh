#!/bin/ksh
# Fichier : $Id: xzac_make.sh,v 1.41 2020/02/05 10:51:23 devgtie Exp $        $Revision: 1.41 $        $Date: 2020/02/05 10:51:23 $
##################################################################################
# STERIA # PROJET MIGRAZUR
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# MODULE XZAC # FICHIER xzac_make.sh
##################################################################################
# DESCRIPTION DU MODULE :
#  compile les procedures stockee de XZAC sur SQL_CI
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# B.GABORIT		05/09/94	: Creation
# C.TORREGROSSA	03/09/95	: ajout cas du PC simplifie
# GUILHOU       08/01/98    : gestion type du PC en argument du makefile (dem/1536) 1.10
# C.TORREGROSSA	20/01/98	: ajout xzac67, xzac68 (1538) 1.11
# C.P.			09/06/98	: separation EXP et HIS
# P.N.			03/12/98	: ajout xzac70 71 72 73 74 75 76
# Hilmarcher    22/03/04    : ajout xzac190 200 v1.17
# Niepceron    	01/03/05    : ajout xzac77 v1.19
# F.L.          09/03/04    : ajout xzac80 81 (SAGA)
# JMG			27/06/05	: correction SAGA v1.50 DEM473 v1.20
# JMG			28/09/05	: ajout xzac82 83 84 85 86 90 91 92 93 94 95 98 99
# PNI			12/12/05	: ajout xzac611 DEM464
# AAZ       	28/08/2008  : Ajout xzac100...104 et xzac110..114
# JPL			01/09/10	: suppression d'ordres "drop" incluss dans les sources
# VR			16/04/12	: Ajout xzac802 xzac805 xzac303 xzac1000 xzac550 (DEM/1016)
# JPL			08/11/12	: Ajout xzac45, xzac78, xzac79 (DEM 1032)  1.25
# JPL			05/03/13	: Ajout xzac610, xzac840 (DEM 1061)  1.26
# PNI			22/04/15	: Ajout xzac?.h pour xzac78  1.29
# JPL			08/03/17	: Ajout xzac841 (DEM 1196)  1.30
# JPL			24/03/17	: Suppression de certains "drop" ; ajout xzac682  1.31
# JPL			27/07/17	: Ajout xzac640 (DEM 1221)  1.33
# JPL			27/07/17	: Support option de compilation pour plateforme de developpement (DEM 1231)  1.34
# JPL			20/06/18	: Inclure xzacc.h pour compiler XZAC640  1.35
# JPL			27/06/18	: Migration Linux 64 bits (DEM 1274) : reglages precompilation  1.36
# JMG			27/07/18	: ajout xzac126 LOT25 DEM1301                                   1.37
# RGR			23/10/18	: Correction synchro revision                                   1.39
# RGR			05/02/20	: Ajout @IP Plateforme de DEV					1.40
# JPC       	15/03/23    : Conversion de XZAC11 en degroupee
# GGY   		15/09/2023  : Adaptation pour CNA (DEM-483)
##################################################################################

# repertoire des includes
includes=$HOME/migrazur/XDMICG/inc

if [[ $# != 1 && $# != 2 ]]
then
        echo "Usage : xzac_make.sh NomSite TypeSite"
        echo "          NomSite : CI ou HIS  ou DP ou VC  ou DM ou DS ou DC ..."
        echo "          Type : PC3 ou autre chose"
        exit 1
fi

typeset -u Nom_Site=$1

Fichier_Compil="xzac${Nom_Site}"


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


# Option de compilation selon la plateforme
Option_Plateforme=""
ADRESSE_IP_POSTE=$( python -c "import socket; print socket.gethostbyname(socket.gethostname())" )
case "${ADRESSE_IP_POSTE}" in
	172.* | 10.* )                              #  Plateforme Enfrasys | Plateforme de DEV
		Option_Plateforme="-DDEV" ;;
esac


# efface les procedures du module
if [[ "$Nom_Site" = "CI" ]] || [[ "$Nom_Site" = "CA" ]]
then
echo " 
use PRC
go
drop procedure XZAC 
drop procedure XZAC72 
drop procedure XZAC73 
drop procedure XZAC75 
drop procedure XZAC80 /* Ajout SAGA */
drop procedure XZAC77
drop procedure XZAC190
drop procedure XZAC200
drop procedure XZAC86
drop procedure XZAC90
drop procedure XZAC91
drop procedure XZAC93
drop procedure XZAC98
drop procedure XZAC99
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
else
echo "
use PRC
go
drop procedure XZAC
drop procedure XZAC73
drop procedure XZAC75
drop procedure XZAC86
drop procedure XZAC90
drop procedure XZAC91
drop procedure XZAC93
drop procedure XZAC98
drop procedure XZAC99
drop procedure XZAC77
drop procedure XZAC200
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
fi

# initialiser le fichier de trace des erreurs
echo "trace de la creation des procedures de XZAC" > ${Fichier_Compil}.err
echo "" >> ${Fichier_Compil}.err

# cree XZAC;60
echo "xzac60sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac60sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crEe XZAC74
echo "xzac74sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac74sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i


if [[ "$Nom_Site" != "CI" ]]  &&  [[ "$Nom_Site" != "HIS" ]] && [[ "$Nom_Site" != "CA" ]]
then
# cree XZAC841  appelee par actions PMV / PMVA ( XZAC01, XZAC61x, XZAC82, XZAC84, XZAC682, XZAC840)
echo "xzac841sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac841sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i
fi


if [[ "$Nom_Site" != "HIS" ]]
then
# cree XZAC;01
echo "xzac01sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac01sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i


# cree XZAC;02
echo "xzac02sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac02sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i


# cree XZAC;03
echo "xzac03sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac03sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i
fi

# cree XZAC;21
echo "xzac21sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac21sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC;25
echo "xzac25sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac25sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i


# cree XZAC;27
echo "xzac27sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac27sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC;40
echo "xzac40sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac40sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i


# cree XZAC;41
echo "xzac41sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac41sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

if [[ "$Nom_Site" != "HIS" ]]
then
# cree XZAC;50
echo "xzac50sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac50sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i


# cree XZAC;51
echo "xzac51sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac51sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i


# cree XZAC;61
echo "xzac61sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac61sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC610
echo "xzac610sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/xzacc.h\"" >> ${Fichier_Compil}.c
cat xzac610sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC611
echo "xzac611sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac611sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i


# cree XZAC;62
echo "xzac62sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac62sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i


# cree XZAC;65
echo "xzac65sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac65sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i


# cree XZAC;66
echo "xzac66sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac66sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i


# cree XZAC;67
echo "xzac67sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac67sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i
fi

# cree XZAC;68
echo "xzac68sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac68sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC82
echo "xzac82sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac82sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC682
echo "xzac682sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac682sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC83
echo "xzac83sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac83sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC84
echo "xzac84sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac84sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC840
echo "xzac840sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/xzacc.h\"" >> ${Fichier_Compil}.c
cat xzac840sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC85
echo "xzac85sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac85sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC86
echo "xzac86sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac86sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC1000
echo "xzac1000sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac1000sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC90
echo "xzac90sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac90sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC91
echo "xzac91sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac91sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC92
echo "xzac92sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac92sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC93
echo "xzac93sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac93sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC94
echo "xzac94sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac94sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC95
echo "xzac95sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac95sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC98
echo "xzac98sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac98sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC303
echo "xzac303sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac303sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC550
echo "xzac550sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac550sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC99
echo "xzac99sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac99sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC126
echo "xzac126sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac99sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

#############Liste des proc a ne pas creer sur les PC simplifies ####
if [[ "$type" != "PC3" ]]
then
if [[ "$Nom_Site" != "HIS" ]]
then
# AAZ 28/08/2007 

# cree XZAC;100
echo "xzac100sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac100sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC;101
echo "xzac101sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac101sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC;102
echo "xzac102sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac102sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC;103
echo "xzac103sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac103sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC;104
echo "xzac104sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac104sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC;105
echo "xzac105sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac105sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC;110
echo "xzac110sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac110sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC;111
echo "xzac111sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac111sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC;112
echo "xzac112sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac112sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC;113
echo "xzac113sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac113sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC;114
echo "xzac114sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac114sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC;115
echo "xzac115sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac115sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# AAZ

# cree XZAC;05
echo "xzac05sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac05sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i


# cree XZAC;06
echo "xzac06sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac06sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i


# cree XZAC;07
echo "xzac07sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac07sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i


# cree XZAC;08
echo "xzac08sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac08sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i


# cree XZAC;11
echo "xzac11sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac11sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC;10
echo "xzac10sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac10sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i


# cree XZAC;09
echo "xzac09sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac09sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i


# cree XZAC;12
echo "xzac12sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac12sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i


# cree XZAC;13
echo "xzac13sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac13sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i


# cree XZAC;14
echo "xzac14sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac14sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i


# cree XZAC;15
echo "xzac15sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac15sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i


# cree XZAC;16
echo "xzac16sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac16sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i


# cree XZAC;17
echo "xzac17sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac17sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i


# cree XZAC;18
echo "xzac18sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/xzaac.h\"" >> ${Fichier_Compil}.c
echo "#include \"$includes/xzacc.h\"" >> ${Fichier_Compil}.c
cat xzac18sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

if [[ "$Nom_Site" = "CI" ]] || [[ "$Nom_Site" = "CA" ]]
then
# cree XZAC;19
echo "xzac19sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac19sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i
fi

# cree XZAC;20
echo "xzac20sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac20sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i
fi

# cree XZAC;22
echo "xzac22sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac22sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i


# cree XZAC;23
echo "xzac23sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac23sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC;26
echo "xzac26sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac26sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC;31
echo "xzac31sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac31sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i


# cree XZAC;30
echo "xzac30sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac30sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i


# cree XZAC;32
echo "xzac32sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac32sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i


# cree XZAC;33
echo "xzac33sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac33sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i


# cree XZAC;34
echo "xzac34sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac34sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i


# cree XZAC;35
echo "xzac35sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac35sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC;36
echo "xzac36sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac36sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i


# cree XZAC;37
echo "xzac37sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac37sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC;42
echo "xzac42sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac42sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC120
echo "xzac120sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac120sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC121
echo "xzac121sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac121sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC122
echo "xzac122sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac122sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC123
echo "xzac123sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac123sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC124
echo "xzac124sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac124sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC125
echo "xzac125sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac125sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC;43
echo "xzac43sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac43sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i


# cree XZAC;44
echo "xzac44sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac44sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

if [[ "$Nom_Site" != "HIS" ]]
then
# cree XZAC;52
echo "xzac52sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac52sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i


# cree XZAC;53
echo "xzac53sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac53sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i


# cree XZAC;54
echo "xzac54sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac54sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC;63
echo "xzac63sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac63sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC;631
echo "xzac631sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac631sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC;632
echo "xzac632sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac632sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC;633
echo "xzac633sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac633sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i


# cree XZAC;634
echo "xzac634sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac634sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC;635
echo "xzac635sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac635sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC;636
echo "xzac636sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac636sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i



# cree XZAC;64
echo "xzac64sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac64sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

if [[ "$Nom_Site" = "CI" ]] || [[ "$Nom_Site" = "CA" ]]
then
# crEe XZAC190
echo "xzac190sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac190sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crEe XZAC70
echo "xzac70sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac70sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crEe XZAC71
echo "xzac71sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac71sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crEe XZAC72
echo "xzac72sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac72sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i
fi

# crEe XZAC73
echo "xzac73sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac73sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crEe XZAC75
echo "xzac75sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac75sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crEe XZAC76
echo "xzac76sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac76sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crEe XZAC77
echo "xzac77sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac77sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crEe XZAC182
echo "xzac182sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac182sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crEe XZAC683
echo "xzac683sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac683sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crEe XZAC200
echo "xzac200sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac200sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crEe XZAC802
echo "xzac802sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac802sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crEe XZAC8021
echo "xzac8021sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac8021sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crEe XZAC805
echo "xzac805sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac805sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crEe XZAC11 en degroupee
echo "xzac11bsp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac11bsp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

if [[ "$Nom_Site" = "CI" ]] || [[ "$Nom_Site" = "CA" ]]
then
# crEe XZAC80 (ajout SAGA)
echo "xzac80sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac80sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crEe XZAC81 (ajout SAGA)
echo "xzac81sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac81sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i
fi
fi


if [[ "$Nom_Site" != "CI" ]]  &&  [[ "$Nom_Site" != "HIS" ]] && [[ "$Nom_Site" != "CA" ]]
then
# cree XZAC640
echo "xzac640sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/xzacc.h\"" >> ${Fichier_Compil}.c
cat xzac640sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i
fi


if [[ "$Nom_Site" = "CI" ]]  ||  [[ "$Nom_Site" = "HIS" ]] || [[ "$Nom_Site" = "CA" ]]
then
# cree XZAC45
echo "xzac45sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzac45sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i
fi


if [[ "$Nom_Site" = "CI" ]] || [[ "$Nom_Site" = "CA" ]]
then
# cree XZAC78
echo "xzac78sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/xzaec.h\"" >> ${Fichier_Compil}.c
cat xzac78sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAC79
echo "xzac79sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/xzaec.h\"" >> ${Fichier_Compil}.c
cat xzac79sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C ${Option_Plateforme} ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i
fi



fi

#####################FIN liste des proc a ne pas creer sur PC niveau 3 ##############
