#!/bin/ksh
# Fichier : $Id: xzaz_make.sh,v 1.3 2018/06/27 19:18:09 devgfi Exp $        $Revision: 1.3 $        $Date: 2018/06/27 19:18:09 $
##################################################################################
# STERIA # PROJET MIGRAZUR
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# MODULE XZAZ # FICHIER xzaz_make.sh
##################################################################################
# DESCRIPTION DU MODULE :
#  Compile les procedures stockees de XZAZ sur le serveur d'un site donne.
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# C.TORREGROSSA	18/01/95	: Creation
# JPL			18/03/09	: Drop dans PRC; ne rien compiler si on n'est pas au CI 1.2
# JPL			27/06/18	: Migration Linux 64 bits (DEM 1274) : phase de precompilation  1.3
# GGY   		15/09/2023  : Adaptation pour CNA (DEM-483)
##################################################################################

# repertoire des includes
includes=$HOME

typeset -u Nom_Site=$1

Fichier_Compil="xzaz${Nom_Site}"


# Verifier le site
if [[ "$Nom_Site" != "CI" ]] && [[ "$Nom_Site" != "CA" ]]
then
	echo "Usage : xzaz_make.sh NomSite"
	echo "          NomSite : CI"
	exit 1
fi


# Effacer les procedures du module
echo "
use PRC
go
drop procedure XZAZ
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_CI


# Initialiser le fichier de trace des erreurs
echo "trace de la creation des procedures de XZAZ" > ${Fichier_Compil}.err
echo "" >> ${Fichier_Compil}.err


# cree XZAZ;01
echo "xzaz01sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzaec.h\"" >> ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzazc.h\"" >> ${Fichier_Compil}.c
cat xzaz01sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C -D${Nom_Site}  ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_CI -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i

