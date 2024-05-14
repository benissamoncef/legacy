#!/bin/ksh
# Fichier : $Id: xzah_make.sh,v 1.22 2018/06/27 11:32:43 devgfi Exp $        $Revision: 1.22 $        $Date: 2018/06/27 11:32:43 $
##################################################################################
# STERIA # PROJET MIGRAZUR
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# MODULE XZAH # FICHIER xzah_make.sh
##################################################################################
# DESCRIPTION DU MODULE :
#  Compile les procedures stockées de XZAH sur le serveur d'un site donné.
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# B.GABORIT	13/08/94	: Creation
# B.GABORIT	11/11/94	: multisite (1.3)
# B.GABORIT	21/11/94	: droits execute à public (1.4)
# C.TORREGROSSA 02/12/94	: ajout de XZAH;20 a XZAH;23 (1.5)
# C.TORREGROSSA 07/12/94	: ajout de XZAH;19 a XZAH;24 (1.6)
# C.TORREGROSSA 08/12/94	: ajout de XZAH;25   (1.7)
# B.GABORIT	15/01/95	: ajout de XZAH;27   (1.7)
# C.TORREGROSSA 02/04/95	: ajout de XZAH;29   (1.9)
# P.NIEPCERON	08/01/97	: ajout xzah32	     (1.10)
# GUILHOU	09/01/98	: gestion multi-sites (dem/1536) 1.14
# P.NIEPCERON   19/10/98	: suppression satir : xzah27 dem/1700 1.15
# ??????????????? 1.16
# C.HILMARCHER  18/03/05	: ajout XZAH02 et XZAH34 v1.17
# JPL		06/07/07	: Suppression 'drop procedure' superflus v1.20
# VR		17/04/12	: Ajout xzah15 xzah02 (DEM/1016)
# JPL		26/06/18	: Migration Linux 64 bits (DEM 1274) : phase de precompilation  1.22
##################################################################################

# repertoire des includes
includes=$HOME

if [[ $# != 1 && $# != 2 ]]
then
	echo "Usage : xzah_make.sh NomSite TypeSite"
	echo "          NomSite : CI ou HIS  ou DP ou VC  ou DM ou DS ou DC ..."
	echo "          Type : PC3 ou autre chose"
	exit 1
fi

typeset -u Nom_Site=$1

Fichier_Compil="xzah${Nom_Site}"


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

if [[ "$type" = "PC3" ]]
then
	echo "          Pas sur PC niveau 3"
	exit 1
fi


# Effacer les procedures du module
echo "
use PRC
go
drop procedure XZAH
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}


# initialiser le fichier de trace des erreurs
echo "trace de la creation des procedures de XZAH" > xzah$1.err
echo "" >> xzah$1.err


# crée XZAH;01
echo "xzah01.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzah01sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# cree XZAH02
echo "xzah02.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzahc.h\"" >> ${Fichier_Compil}.c
cat xzah02sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAH;03
echo "xzah03.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzah03sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i


# crée XZAH;04
echo "xzah04.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzah04sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i


# crée XZAH;05
echo "xzah05.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzah05sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i


# crée XZAH;07
echo "xzah07.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzah07sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i


# crée XZAH;08
echo "xzah08.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzah08sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i


# crée XZAH;12
echo "xzah12.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzahc.h\"" >> ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
cat xzah12sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i



# crée XZAH;13
echo "xzah13.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzahc.h\"" >> ${Fichier_Compil}.c
cat xzah13sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i



# crée XZAH;14
echo "xzah14.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzahc.h\"" >> ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
cat xzah14sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i


# crée XZAH;15
echo "xzah15.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzahc.h\"" >> ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
cat xzah15sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i


# crée XZAH;16
echo "xzah16.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzahc.h\"" >> ${Fichier_Compil}.c
cat xzah16sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i


# crée XZAH;17
echo "xzah17.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzahc.h\"" >> ${Fichier_Compil}.c
cat xzah17sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAH;18
echo "xzah18.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzahc.h\"" >> ${Fichier_Compil}.c
cat xzah18sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAH;19
echo "xzah19.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzahc.h\"" >> ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
cat xzah19sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAH;20
echo "xzah20.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzahc.h\"" >> ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
cat xzah20sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAH;21
echo "xzah21.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzahc.h\"" >> ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
cat xzah21sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAH;22
echo "xzah22.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzahc.h\"" >> ${Fichier_Compil}.c
cat xzah22sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAH;23
echo "xzah23.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzahc.h\"" >> ${Fichier_Compil}.c
cat xzah23sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAH;24
echo "xzah24.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzahc.h\"" >> ${Fichier_Compil}.c
cat xzah24sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAH;25
echo "xzah25.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzahc.h\"" >> ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
cat xzah25sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAH;29
echo "xzah29.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzahc.h\"" >> ${Fichier_Compil}.c
cat xzah29sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAH34
echo "xzah34sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzahc.h\"" >> ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
cat xzah34sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAH;30
echo "xzah30.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzahc.h\"" >> ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
cat xzah30sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAH;31
echo "xzah31.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzahc.h\"" >> ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
cat xzah31sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAH;32
echo "xzah32.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzahc.h\"" >> ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
cat xzah32sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAH;33
echo "xzah33.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzahc.h\"" >> ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
cat xzah33sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i


# ajoute les droits pour public
echo "ajout des droits à public" >> ${Fichier_Compil}.err
echo "grant execute on XZAH to public" > ${Fichier_Compil}.c
echo "go" >> ${Fichier_Compil}.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.c >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c
