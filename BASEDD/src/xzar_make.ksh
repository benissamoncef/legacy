#!/bin/ksh
# Fichier : $Id: xzar_make.ksh,v 1.4 2018/06/27 19:02:54 devgfi Exp $        $Revision: 1.4 $        $Date: 2018/06/27 19:02:54 $
##################################################################################
# GTIE # PROJET MIGRAZUR
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# MODULE XZAR # FICHIER xzar_make.ksh
##################################################################################
# DESCRIPTION DU MODULE :
#  compile les procedures stockÅe de XZAR sur SQL_CI
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# F. VOLCIC	02/11/94	: Creation
# GUILHOU	08/01/98	: gestion multi-sites (dem/1536) 1.2
# ???	26/03/07 : Ajout 'use PRC' avant 'drop proc'  1.3
# JPL	27/06/18 : Migration Linux 64 bits (DEM 1274) : phase de precompilation  1.4
##################################################################################

# repertoire des includes
includes=$HOME

if [[ $# != 1 && $# != 2 ]]
then
        echo "Usage : xzar_make.ksh NomSite TypeSite"
        echo "          NomSite : CI ou HIS  ou DP ou VC  ou DM ou DS ou DC ..."
        echo "         	ne fonctionne pas sur PC 3"
        exit 1
fi

typeset -u Nom_Site=$1

Fichier_Compil="xzar${Nom_Site}"


# Type de site
if [[ $# = 1 ]]
then
        type="xx"
else
        type=$2
fi

if [[ "$type" = "PC3" ]]
then
	echo "          ne fonctionne pas sur PC 3"
	exit 1
fi


# Directives de compilation selon le site


# Effacer les procedures du module
echo "
use PRC
go
drop procedure XZAR
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}


# Initialiser le fichier de trace des erreurs
echo "trace de la creation des procedures de XZAR" > ${Fichier_Compil}.err
echo "" >> ${Fichier_Compil}.err


# cree XZAR;03
echo "xzar03sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
cat xzar03sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  -D${Nom_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

