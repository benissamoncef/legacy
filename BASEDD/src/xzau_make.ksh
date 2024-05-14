#!/bin/ksh
# Fichier : $Id: xzau_make.ksh,v 1.9 2018/06/27 18:28:30 devgfi Exp $        $Revision: 1.9 $        $Date: 2018/06/27 18:28:30 $
##################################################################################
# GTIE # PROJET MIGRAZUR
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# MODULE XZAU # FICHIER xzau_make.ksh
##################################################################################
# DESCRIPTION DU MODULE :
#  Compile les procedures stockées de XZAU sur le serveur d'un site donné.
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# F. VOLCIC	02/11/94	: Creation
# B.GABORIT	29/11/94	: modif
# F. VOLCIC	19/05/94	: Ajout XZAU16
# GUILHOU	02/09/96	: Ajout compil sur DC ou DS
# Guilhou	08/01/98	: gestion multi-sites (dem/1536)  1.5
# JPL		18/03/09	: Drop dans PRC; fichier d'erreurs par site et conserve 1.6
# PNI		26/05/11	: ajout xzau111
# JPL		27/06/18	: Suppression directives 'drop proc' obsoletes  1.8
# JPL		27/06/18	: Migration Linux 64 bits (DEM 1274) : phase de precompilation  1.9
##################################################################################

# repertoire des includes
includes=$HOME

if [[ $# != 1 && $# != 2 ]]
then
	echo "Usage : xzau_make.ksh NomSite TypeSite "
	echo "          NomSite : CI ou HIS  ou DP ou VC  ou DM ou DS ou DC ..."
	exit 1
fi

typeset -u Nom_Site=$1

Fichier_Compil="xzau${Nom_Site}"


# Effacer les procedures du module
echo "
use PRC
go
drop procedure XZAU
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}


# Initialiser le fichier de trace des erreurs
echo "trace de la creation des procedures de XZAU" > ${Fichier_Compil}.err
echo "" >> ${Fichier_Compil}.err


# crée XZAU;01
echo "xzau01sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzau01sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C -D${Nom_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAU;02
echo "xzau02sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzau02sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C -D${Nom_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAU;03
echo "xzau03sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzau03sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C -D${Nom_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAU;04
echo "xzau04sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzau04sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C -D${Nom_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAU;05
echo "xzau05sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzau05sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C -D${Nom_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAU;06
echo "xzau06sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzau06sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C -D${Nom_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAU;07
echo "xzau07sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzau07sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C -D${Nom_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAU;08
echo "xzau08sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzau08sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C -D${Nom_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAU;09
echo "xzau09sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzau09sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C -D${Nom_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAU;10
echo "xzau10sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzau10sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C -D${Nom_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAU;11
echo "xzau11sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzau11sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C -D${Nom_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAU;12
echo "xzau12sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzau12sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C -D${Nom_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAU;13
echo "xzau13sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzau13sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C -D${Nom_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAU;14
echo "xzau14sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzau14sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C -D${Nom_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAU;15
echo "xzau15sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzau15sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C -D${Nom_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAU;16
echo "xzau16sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzau16sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C -D${Nom_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAU111
echo "xzau111sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzau111sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C -D${Nom_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i
