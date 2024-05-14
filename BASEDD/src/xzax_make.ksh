#!/bin/ksh
# Fichier : $Id: xzax_make.ksh,v 1.1 2019/01/23 12:28:11 devgfi Exp $        $Revision: 1.1 $        $Date: 2019/01/23 12:28:11 $
##################################################################################
# ESCOTA # PROJET MIGRAZUR
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# MODULE XZAX # FICHIER xzax_make.sh
##################################################################################
# DESCRIPTION DU MODULE :
#  Compile les procedures stockées du groupe XZAX sur le serveur d'un site donne.
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
#
# JPL	28/11/18 : Création  1.1
##################################################################################

# Repertoire des includes
includes=$HOME/migrazur/XDMICG/inc

if [[ $# != 1 && $# != 2 ]]
then
        echo "Usage : xzax_make.ksh NomSite "
        echo "        NomSite : CI ou HIS  ou DP ou VC  ou DM ou DS ou DC ..."
        exit 1
fi

typeset -u Nom_Site=$1

Fichier_Compil="xzax${Nom_Site}"


# Directives de compilation selon le site
typeset -u Option_Site=$1
if [[ "$Option_Site" = "HIS" ]]
then
	Option_Site=HIST
fi


########################################################################################################
##
##  FONCTION     : Compiler_Procedure_XZAE
##
##  ARGUMENTS    :
##    $1 : Le nom de la procédure stockée à compiler (ex. XZAX03).
##
##  COMPTE RENDU :
##    Vrai (0) ou faux (1) selon que l'opération s'est correctement déroulée ou non.
##
########################################################################################################

Compiler_Procedure_XZAX ()
{
	typeset -u Nom_Proc="$1"

	typeset -l Racine_Proc="$1"
	Fichier_Proc=${Racine_Proc}sp.prc

	[[ -r "${Fichier_Proc}" ]]  ||  {
		echo "   Fichier $Fichier_Proc introuvable."  >&2
		false
		return
	}

	echo "$Fichier_Proc" >> ${Fichier_Compil}.err

	echo "#include \"$includes/xdc.h\"" > ${Fichier_Compil}.c

	cat ${Fichier_Proc} >> ${Fichier_Compil}.c

	cpp -E -P -C -D${Option_Site} ${Fichier_Compil}.c  > ${Fichier_Compil}.i  
	isql -UMIGRAZUR -PMIGRAZUR -SSQL_${Nom_Site} -i ${Fichier_Compil}.i >> ${Fichier_Compil}.err

	rm ${Fichier_Compil}.c  ${Fichier_Compil}.i
}


########################################################################################################

# Initialiser le fichier de trace des erreurs
echo "Compilation des procedures de XZAX sur site ${Nom_Site}" > ${Fichier_Compil}.err
echo "" >> ${Fichier_Compil}.err


Compiler_Procedure_XZAX  XZAX01
Compiler_Procedure_XZAX  XZAX02
Compiler_Procedure_XZAX  XZAX03
