#!/bin/ksh
# Fichier : $Id: xzam_make.ksh,v 1.8 2018/06/27 15:52:51 devgfi Exp $        $Revision: 1.8 $        $Date: 2018/06/27 15:52:51 $
##################################################################################
# GTIE # PROJET MIGRAZUR
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# MODULE XZAM # FICHIER xzam_make.ksh
##################################################################################
# DESCRIPTION DU MODULE :
#  Compile les procedures stockées de XZAM sur le serveur d'un site donné.
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# F. VOLCIC	03/11/94	: Creation
# C.TORREGROSSA	04/09/96	: ajout compilation sur DS et DC (1.2)
#                                 et copmilation conditionnelle
# GUILHOU	09/01/98	: gestion multi-sites (dem/1536) 1.3
# JPL		18/03/09	: Drop dans PRC; fichier d'erreurs par site et conserve 1.4
# JMG		15/09/09	: ajout XZAM12 FACTIS 1.5 DEM914
# JPL		03/01/13	: Compiler XZAM06 sur HIStorique aussi (DEM 1057)  1.6
# JMG		15/09/16	: ajout XZAM08 FACTIS 1.7
# JPL		27/06/18	: Migration Linux 64 bits (DEM 1274) : phase de precompilation  1.8
##################################################################################

# repertoire des includes
includes=$HOME/migrazur/XDMICG/inc

if [[ $# != 1 && $# != 2 ]]
then
        echo "Usage : xzam_make.ksh NomSite TypeSite"
        echo "          NomSite : CI ou HIS  ou DP ou VC  ou DM ou DS ou DC ..."
        echo "          Type : PC3 ou autre chose"
        echo "          ne fonctionne qu'au CI et sur PC niveau 1 et 2"
        exit 1
fi

typeset -u Nom_Site=$1

Fichier_Compil="xzam${Nom_Site}"


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


# Effacer le groupe de procedures du module
echo "
use PRC
go
drop procedure XZAM
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}


# initialiser le fichier de trace des erreurs
echo "trace de la creation des procedures de XZAM" > ${Fichier_Compil}.err
echo "" >> ${Fichier_Compil}.err


if [[ "$Nom_Site" != "HIS" ]]
then
	# crée XZAM;01
	echo "xzam01sp.prc" >> ${Fichier_Compil}.err
	echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
	cat xzam01sp.prc >> ${Fichier_Compil}.c
	cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
	rm ${Fichier_Compil}.c ${Fichier_Compil}.i

	# crée XZAM;02
	echo "xzam02sp.prc" >> ${Fichier_Compil}.err
	echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
	cat xzam02sp.prc >> ${Fichier_Compil}.c
	cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
	rm ${Fichier_Compil}.c ${Fichier_Compil}.i

	# crée XZAM;03
	echo "xzam03sp.prc" >> ${Fichier_Compil}.err
	echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
	cat xzam03sp.prc >> ${Fichier_Compil}.c
	cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
	rm ${Fichier_Compil}.c ${Fichier_Compil}.i

	# crée XZAM;04
	echo "xzam04sp.prc" >> ${Fichier_Compil}.err
	echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
	cat xzam04sp.prc >> ${Fichier_Compil}.c
	cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
	rm ${Fichier_Compil}.c ${Fichier_Compil}.i

	# crée XZAM;05
	echo "xzam05sp.prc" >> ${Fichier_Compil}.err
	echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
	cat xzam05sp.prc >> ${Fichier_Compil}.c
	cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
	rm ${Fichier_Compil}.c ${Fichier_Compil}.i
fi


# crée XZAM;06
echo "xzam06sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzam06sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i


if [[ "$Nom_Site" != "HIS" ]]
then
	# crée XZAM;07
	echo "xzam07sp.prc" >> ${Fichier_Compil}.err
	echo "#include \"$includes/xzamc.h\"" > ${Fichier_Compil}.c
	echo "#include \"$includes/xdc.h\"" >> ${Fichier_Compil}.c
	cat xzam07sp.prc >> ${Fichier_Compil}.c
	cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
	rm ${Fichier_Compil}.c ${Fichier_Compil}.i

	# crée XZAM;10
	echo "xzam10sp.prc" >> ${Fichier_Compil}.err
	echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
	cat xzam10sp.prc >> ${Fichier_Compil}.c
	cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
	rm ${Fichier_Compil}.c ${Fichier_Compil}.i

	# crée XZAM;11
	echo "xzam11sp.prc" >> ${Fichier_Compil}.err
	echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
	cat xzam11sp.prc >> ${Fichier_Compil}.c
	cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
	rm ${Fichier_Compil}.c ${Fichier_Compil}.i

	# crée XZAM;12
	echo "xzam12sp.prc" >> ${Fichier_Compil}.err
	echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
	cat xzam12sp.prc >> ${Fichier_Compil}.c
	cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
	rm ${Fichier_Compil}.c ${Fichier_Compil}.i
fi


# crée XZAM08
echo "xzam08sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzam08sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

