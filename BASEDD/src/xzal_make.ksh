#!/bin/ksh
# Fichier : $Id: xzal_make.ksh,v 1.7 2018/06/27 16:21:53 devgfi Exp $        $Revision: 1.7 $        $Date: 2018/06/27 16:21:53 $
##################################################################################
# GTIE # PROJET MIGRAZUR
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# MODULE XZAL # FICHIER xzal_make.ksh
##################################################################################
# DESCRIPTION DU MODULE :
#  Compile les procedures stockées de XZAL sur le serveur d'un site donné.
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# F. VOLCIC		03/11/94	: Creation
# C. TORREGROSSA	04/05/95	: modif nom du serveur en parametre d'entree
# GUILHOU		09/01/98	: gestion multi-sites (dem/1536) 1.4
# PN			01/07/98	: separation EXP/HIS 1.5
# JPL			18/03/09	: Drop dans PRC; XZAL02 et XZAL04 sur HIS seulement;
#					  fichier d'erreurs par site et conserve 1.6
# JPL	27/06/18 : Migration Linux 64 bits (DEM 1274) : phase de precompilation  1.7
##################################################################################

# repertoire des includes
includes=$HOME/migrazur/XDMICG/inc

# controle la syntaxe d'appel
if [[ $# != 1 && $# != 2 ]]
then
        echo "Usage : xzal_make.ksh NomSite TypeSite"
        echo "          NomSite : CI ou HIS  ou DP ou VC  ou DM ou DS ou DC ..."
        echo "          Type : PC3 ou autre chose"
        echo "          ne fonctionne qu'au CI et sur PC niveau 1 et 2"
        exit 1
fi

typeset -u Nom_Site=$1

Fichier_Compil="xzal${Nom_Site}"


# Type de site
if [[ $# = 1 ]]
then
        type="xx"
else
        type=$2
fi

if [[ $type = "PC3" ]]
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


# Efface les procedures du module
echo "
use PRC
go
drop procedure XZAL
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}


# initialiser le fichier de trace des erreurs
echo "trace de la creation des procedures de XZAL" > ${Fichier_Compil}.err
echo "" >> ${Fichier_Compil}.err


if [[ "$Nom_Site" = "HIS" ]]
then
# crée XZAL;04
echo "xzal04sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzal04sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAL;02
echo "xzal02sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzal02sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

fi


if [[ "$Nom_Site" != "HIS" ]]
then
# crée XZAL;01
echo "xzal01sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzal01sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

# crée XZAL;03
echo "xzal03sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c
cat xzal03sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C  ${Option_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i
fi

