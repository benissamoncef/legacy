#!/bin/ksh
# Fichier : $Id: xzai_make.ksh,v 1.10 2018/06/27 14:14:30 devgfi Exp $        $Revision: 1.10 $        $Date: 2018/06/27 14:14:30 $
##################################################################################
# GTIE # PROJET MIGRAZUR
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# MODULE XZAI # FICHIER xzao_make.ksh
##################################################################################
# DESCRIPTION DU MODULE :
#  compile les procedures stockée de XZAI sur SQL_CI
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# F. VOLCIC	26/10/94	: Creation
# GUILHOU	09/01/98	: gestion multi-sites (dem/1536)  1.3
# JMG		13/06/05	: ajout XZAI500, 501, 502 SAGA 1.4
# JPL		03/07/07	: XZAI04 necessite xzaec.h; nom fichier d'erreurs 1.5
# JPL		12/01/12	: ajout 'use PRC' car base master par defaut 1.6
# JMG		30/10/14	: ajout XZAI400 1.7
# JPL		04/07/17	: ajout XZAI600 et support compilation sur HIS  1.8
# ???		28/02/18	: Compilation des procedures XZAI en région  1.9
# JPL		04/07/17	: Migration Linux 64 bits (DEM 1274) : phase de precompilation  1.10
##################################################################################

# repertoire des includes
includes=$HOME/migrazur/XDMICG/inc

if [[ $# != 1 && $# != 2 ]]
then
	echo "Usage : xzae_make.sh NomSite TypeSite"
	echo "          NomSite : CI ou HIS  ou DP ou VC  ou DM ou DS ou DC ..."
	echo "          Type : PC3 ou autre chose"
	echo "          ne fonctionne qu'au CI et sur PC niveau 1 et 2"
	exit 1
fi

typeset -u Nom_Site=$1

Fichier_Compil="xzai${Nom_Site}"


# Type de site
if [[ $# = 1 ]]
then
	type="xx"
else
	type=$2
fi

if [[ "$type" = "PC3" ]]
then
	echo    "          ne fonctionne qu'au CI et sur PC niveau 1 et 2"
	exit 0
fi


# Directives de compilation selon le site
Option_Site="-D${Nom_Site}"
if [[ "$Nom_Site" = "HIS" ]]
then
	Option_Site="-DHIST"
fi


# Effacer le groupe de procedures du module
echo "
use PRC
go
drop procedure XZAI
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}


# initialiser le fichier de trace des erreurs
echo "trace de la creation des procedures de XZAI" > ${Fichier_Compil}.err
echo "" >> ${Fichier_Compil}.err



	# crée XZAI;01
	#echo "xzai01sp.prc" >> ${Fichier_Compil}.err
	#echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
	#cat xzai01sp.prc >> ${Fichier_Compil}.c
	#cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
	#isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
	#rm ${Fichier_Compil}.c ${Fichier_Compil}.i

	# crée XZAI;02
	echo "xzai02sp.prc" >> ${Fichier_Compil}.err
	echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
	cat xzai02sp.prc >> ${Fichier_Compil}.c
	cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
	rm ${Fichier_Compil}.c ${Fichier_Compil}.i

	# crée XZAI;03
	echo "xzai03sp.prc" >> ${Fichier_Compil}.err
	echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
	cat xzai03sp.prc >> ${Fichier_Compil}.c
	cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
	rm ${Fichier_Compil}.c ${Fichier_Compil}.i

	# crée XZAI;04
	echo "xzai04sp.prc" >> ${Fichier_Compil}.err
	echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
	echo "#include \"$includes/xzaec.h\"" >> ${Fichier_Compil}.c
	cat xzai04sp.prc >> ${Fichier_Compil}.c
	cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
	rm ${Fichier_Compil}.c ${Fichier_Compil}.i

	# crée XZAI;05
	echo "xzai05sp.prc" >> ${Fichier_Compil}.err
	echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
	cat xzai05sp.prc >> ${Fichier_Compil}.c
	cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
	rm ${Fichier_Compil}.c ${Fichier_Compil}.i

	# crée XZAI;06
	echo "xzai06sp.prc" >> ${Fichier_Compil}.err
	echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
	cat xzai06sp.prc >> ${Fichier_Compil}.c
	cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
	rm ${Fichier_Compil}.c ${Fichier_Compil}.i

	# crée XZAI;07
	echo "xzai07sp.prc" >> ${Fichier_Compil}.err
	echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
	cat xzai07sp.prc >> ${Fichier_Compil}.c
	cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
	rm ${Fichier_Compil}.c ${Fichier_Compil}.i

	# crée XZAI;08
	echo "xzai08sp.prc" >> ${Fichier_Compil}.err
	echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
	cat xzai08sp.prc >> ${Fichier_Compil}.c
	cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
	rm ${Fichier_Compil}.c ${Fichier_Compil}.i

	# cree XZAI500
	{ echo "use PRC"; echo go; echo "drop procedure XZAI500"; echo go; }  | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
	echo "xzai500sp.prc" >> ${Fichier_Compil}.err
	echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
	cat xzai500sp.prc >> ${Fichier_Compil}.c
	cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
	rm ${Fichier_Compil}.c ${Fichier_Compil}.i

	# cree XZAI400
	echo "xzai400sp.prc" >> ${Fichier_Compil}.err
	echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
	echo "#include \"$includes/xzaec.h\"" >> ${Fichier_Compil}.c
	cat xzai400sp.prc >> ${Fichier_Compil}.c
	cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
	rm ${Fichier_Compil}.c ${Fichier_Compil}.i

	# cree XZAI501
	{ echo "use PRC"; echo go; echo "drop procedure XZAI501"; echo go; }  | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
	echo "xzai501sp.prc" >> ${Fichier_Compil}.err
	echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
	cat xzai501sp.prc >> ${Fichier_Compil}.c
	cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
	rm ${Fichier_Compil}.c ${Fichier_Compil}.i

	# cree XZAI502
	{ echo "use PRC"; echo go; echo "drop procedure XZAI502"; echo go; }  | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}
	echo "xzai502sp.prc" >> ${Fichier_Compil}.err
	echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
	cat xzai502sp.prc >> ${Fichier_Compil}.c
	cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
	rm ${Fichier_Compil}.c ${Fichier_Compil}.i


if [[ "$Nom_Site" = "HIS" ]]
then
	# cree XZAI600
	echo "xzai600sp.prc" >> ${Fichier_Compil}.err
	echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
	cat xzai600sp.prc >> ${Fichier_Compil}.c
	cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
	rm ${Fichier_Compil}.c ${Fichier_Compil}.i
fi

