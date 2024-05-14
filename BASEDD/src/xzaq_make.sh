#!/bin/ksh
# Fichier : $Id: xzaq_make.sh,v 1.6 2018/06/27 18:51:27 devgfi Exp $        $Revision: 1.6 $        $Date: 2018/06/27 18:51:27 $
##################################################################################
# STERIA # PROJET MIGRAZUR
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# MODULE XZAQ # FICHIER xzaq_make.sh
##################################################################################
# DESCRIPTION DU MODULE :
#  Compile les procedures stockées de XZAQ sur le serveur d'un site donné.
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# T.FOURNEL     29/12/94        : Creation 
# JPL		18/03/09	: Drop dans PRC; fichier d'erreurs par site et conserve 1.5
# JPL		27/06/18	: Migration Linux 64 bits (DEM 1274) : phase de precompilation  1.6
##################################################################################

# repertoire des includes
includes=$HOME

typeset -u Nom_Site=$1

Fichier_Compil="xzaq${Nom_Site}"

# Verifier le site
if [[ "$Nom_Site" != "HIS" ]]
then
	echo "Usage : xzaq_make.sh NomSite"
	echo "          NomSite : HIS"
	exit 1
fi


# Effacer les procedures du module
echo "
use PRC
go
drop procedure XZAQ
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site}


# Initialiser le fichier de trace des erreurs
echo "trace de la creation des procedures de XZAQ" > ${Fichier_Compil}.err
echo "" >> ${Fichier_Compil}.err

# cree XZAQ;01
echo "xzaq01sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> ${Fichier_Compil}.c
cat xzaq01sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C -DHIST ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i


# cree XZAQ;02
echo "xzaq02sp.prc" >> ${Fichier_Compil}.err
echo "#include \"$includes/migrazur/XDMICG/inc/xdc.h\"" > ${Fichier_Compil}.c
echo "#include \"$includes/migrazur/XDMICG/inc/xzasc.h\"" >> ${Fichier_Compil}.c
cat xzaq02sp.prc >> ${Fichier_Compil}.c
cpp -E -P -C -DHIST ${Fichier_Compil}.c  > ${Fichier_Compil}.i
isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err
rm ${Fichier_Compil}.c ${Fichier_Compil}.i
