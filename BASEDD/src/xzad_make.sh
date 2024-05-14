#!/bin/ksh
# Fichier : $Id: xzad_make.sh,v 1.13 2018/06/27 14:55:47 devgfi Exp $        $Revision: 1.13 $        $Date: 2018/06/27 14:55:47 $
##################################################################################
# STERIA # PROJET MIGRAZUR
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# MODULE XZAD # FICHIER xzad_make.sh
##################################################################################
# DESCRIPTION DU MODULE :
#  Compile les procedures stockées de XZAD sur le serveur d'un site donné.
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# P.N	24/02/97	: Creation
# P.N   24/08/97	: Ajout xzad10 dem/1483 1.2
# C.P	07/01/98	: Modifications relatives a la proc XZAD03
# JMG	09/01/98	: gestion multi-sites (dem/1536) 1.6
# P.N	01/07/98	: Separation EXP/HIS 1.7
# JMG	28/09/05	: ajout XZAD17 1.8
# JPL	08/06/07	: Suppression 'drop procedure' pour certaines procedures 1.9
# JMG	14/10/08	: ajout XZAD11 1.10 DEM/837
# JPL	18/03/09	: Correction syntaxe du commentaire precedent 1.11
# JPL	03/05/18	: Ajout XZAD18 (DEM 1276) ; suppression 'drop's obsoletes  1.12
# JPL	27/06/18	: Migration Linux 64 bits (DEM 1274) : phase de precompilation  1.13
##################################################################################

# repertoire des includes
includes=$HOME

if [[ $# != 1 ]]
then
	echo "syntaxe : xzad_make.sh NomSite"
	exit 1
fi

typeset -u Nom_Site=$1

Fichier_Compil="xzad${Nom_Site}"


# Directives de compilation selon le site
Option_Site="-D${Nom_Site}"
if [[ "$Nom_Site" = "HIS" ]]
then
	Option_Site="-DHIST"
fi


# Initialiser le fichier de trace des erreurs
echo "trace de la creation des procedures de XZAD" > ${Fichier_Compil}.err
echo "" >> ${Fichier_Compil}.err


if [[ "$Nom_Site" = "HIS" ]]
then
	# crée XZAD03
	echo "xzad03sp.prc" >> ${Fichier_Compil}.err
	echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
	echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> ${Fichier_Compil}.c
	cat xzad03sp.prc >> ${Fichier_Compil}.c
	cpp -E -P -C -DHIST ${Fichier_Compil}.c  > ${Fichier_Compil}.i
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
	rm ${Fichier_Compil}.c ${Fichier_Compil}.i

	# crEe XZAD07
	echo "xzad07sp.prc" >> ${Fichier_Compil}.err
	echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
	echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> ${Fichier_Compil}.c
	cat xzad07sp.prc >> ${Fichier_Compil}.c
	cpp -E -P -C -DHIST ${Fichier_Compil}.c  > ${Fichier_Compil}.i
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
	rm ${Fichier_Compil}.c ${Fichier_Compil}.i

	# crée XZAD04
	echo "xzad04sp.prc" >> ${Fichier_Compil}.err
	echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
	echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> ${Fichier_Compil}.c
	cat xzad04sp.prc >> ${Fichier_Compil}.c
	cpp -E -P -C -DHIST ${Fichier_Compil}.c  > ${Fichier_Compil}.i
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
	rm ${Fichier_Compil}.c ${Fichier_Compil}.i

	# crée XZAD18
	echo "xzad18sp.prc" >> ${Fichier_Compil}.err
	echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
	echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> ${Fichier_Compil}.c
	cat xzad18sp.prc >> ${Fichier_Compil}.c
	cpp -E -P -C -DHIST ${Fichier_Compil}.c  > ${Fichier_Compil}.i
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
	rm ${Fichier_Compil}.c ${Fichier_Compil}.i
else

# crEe XZAD01
echo "xzad01sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
cat xzad01sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAD02
echo "xzad02sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> ${Fichier_Compil}.c
cat xzad02sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAD04
echo "xzad04sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> ${Fichier_Compil}.c
cat xzad04sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAD05
echo "xzad05sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> ${Fichier_Compil}.c
cat xzad05sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAD06
echo "xzad06sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> ${Fichier_Compil}.c
cat xzad06sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crEe XZAD07
echo "xzad07sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> ${Fichier_Compil}.c
cat xzad07sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err

# crEe XZAD08
echo "xzad08sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> ${Fichier_Compil}.c
cat xzad08sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crEe XZAD09
echo "xzad09sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
cat xzad09sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crEe XZAD10
echo "xzad10sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
cat xzad10sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crEe XZAD11
echo "xzad11sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
cat xzad11sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crEe XZAD17
echo "xzad17sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
cat xzad17sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

fi
