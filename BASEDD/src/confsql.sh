#!/bin/ksh
# Fichier : $Id: confsql.sh,v 1.4 1996/12/05 18:21:00 gaborit Exp $        Release : $Revision: 1.4 $        Date : $Date: 1996/12/05 18:21:00 $
##################################################################################
# STERIA # PROJET MIGRAZUR
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# SCRIPT confsql.sh
##################################################################################
# DESCRIPTION DU SCRIPT :
# configure un SQL Server donne en argument :
#  cree le user applicatif MIGRAZUR
#  cree les devices
#  cree les bases
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# B.GABORIT	09/08/94	: Creation
# C.TORREGROSSA	21/03/95	: Modif script a appeler
# C.TORREGROSSA	25/09/96	: Modif mot de passe sa
##################################################################################

# Creation des bases

if (($# != 1 )) || [[ $1 != "CI" && $1 != "DN" && $1 != "DM" ]]
then	echo "Syntaxe : confsql.sh CI|DN|DM"

elif [[ $1 = "CI" ]]
then
	# execute le script de configuration du serveur SQL_CI
	isql -Usa -SSQL_$1 -Pzigramur < confsqlci.bas

elif [[ $1 = "DN" ]]
then
	# execute le script de configuration du serveur SQL_DN
	isql -Usa -SSQL_$1 -Pzigramur < confsqldn.bas

elif [[ $1 = "DM" ]]
then
	# execute le script de configuration du serveur SQL_DM
	isql -Usa -SSQL_$1 -Pzigramur < confsqldm.bas

fi
