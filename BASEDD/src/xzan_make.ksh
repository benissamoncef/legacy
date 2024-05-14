#!/bin/ksh
# Fichier : $Id: xzan_make.ksh,v 1.15 2018/06/27 09:16:52 devgfi Exp $        Release : $Revision: 1.15 $        Date : $Date: 2018/06/27 09:16:52 $
##################################################################################
# GTIE # PROJET MIGRAZUR
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# MODULE XZAN # FICHIER xzao_make.ksh
##################################################################################
# DESCRIPTION DU MODULE :
#  Compile les procedures stockée de XZAN sur le serveur d'un site donne.
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# F. VOLCIC	26/10/94	: Creation
# GABORIT	26/10/94	: ajout xzan37
# C.T.   	17/02/95	: correction erreur de compile conditionnelle V 1.7
# C.T.   	04/09/96	: ajout serveur DS et DC V 1.8
# JMG		14/11/96	: ajout xzan38 et suivante (refonte complete des astreintes) (DEM/1306) 1.9
# JMG		12/12/96	: ajout xzan44 (DEM/1335) 1.10
# JMG		09/01/98	: gestion multi-sites (dem/1536) 1.11
# PN		02/02/98	: correction 1.12
# JPL		18/03/09	: Drop dans PRC; fichier d'erreurs par site et conserve 1.13
# ???		12/04/13	: Ajout xzan45  1.14
# JPL		26/06/18	: Migration Linux 64 bits (DEM 1274) : phase de precompilation  1.15
##################################################################################

# repertoire des includes
includes=$HOME

if [[ $# != 1 && $# != 2 ]]
then
        echo "Usage : xzan_make.ksh NomSite TypeSite"
        echo "        NomSite : CI ou HIS  ou DP ou VC  ou DM ou DS ou DC ..."
        echo "        Type : PC3 ou autre chose"
        echo "          ne fonctionne qu'au CI et sur PC niveau 1 et 2"
        exit 1
fi

typeset -u Nom_Site=$1

Fichier_Compil="xzan${Nom_Site}"


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
drop procedure XZAN
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}


# initialiser le fichier de trace des erreurs
echo "trace de la creation des procedures de XZAN" > ${Fichier_Compil}.err
echo "" >> ${Fichier_Compil}.err


# crée XZAN;03
echo "xzan03sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzan03sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAN;04
echo "xzan04sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzan04sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

if [[ "$type" = "PC3" ]]
then
	exit 0
fi


# crée XZAN;01
echo "xzan01sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzan01sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAN;05
echo "xzan05sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzan05sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAN;06
echo "xzan06sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzan06sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAN;07
echo "xzan07sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzan07sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAN;08
echo "xzan08sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzanc.h\"" >> ${Fichier_Compil}.c
cat xzan08sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAN;09
echo "xzan09sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzan09sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAN;10
echo "xzan10sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzan10sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAN;11
echo "xzan11sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzan11sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAN;12
echo "xzan12sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzan12sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAN;13
echo "xzan13sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzan13sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAN;14
echo "xzan14sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzan14sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAN;15
echo "xzan15sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzan15sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAN;16
echo "xzan16sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzan16sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAN;17
echo "xzan17sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzan17sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAN;20
echo "xzan20sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzan20sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAN;21
echo "xzan21sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzan21sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAN;22
echo "xzan22sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzan22sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAN;30
echo "xzan30sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzan30sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAN;31
echo "xzan31sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzan31sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAN;32
echo "xzan32sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzan32sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAN;33
echo "xzan33sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzan33sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAN;34
echo "xzan34sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzan34sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAN;36
echo "xzan36sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzan36sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAN;25
echo "xzan25sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzanc.h\"" >> ${Fichier_Compil}.c
cat xzan25sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAN;35
echo "xzan35sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzan35sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAN;37
echo "xzan37sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzan37sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAN;38
echo "xzan38sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzan38sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAN;39
echo "xzan39sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzan39sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAN;40
echo "xzan40sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzan40sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAN;41
echo "xzan41sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzanc.h\"" >> ${Fichier_Compil}.c
cat xzan41sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAN;42
echo "xzan42sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzan42sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAN;43
echo "xzan43sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzan43sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAN;44
echo "xzan44sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzan44sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAN45
echo "xzan45sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzan45sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

