:
########################################################################################################
# Fichier : $Id: xzsa01_ListeMachine.sh,v 1.1 2012/02/24 17:42:06 gesconf Exp $      Release : $Revision: 1.1 $        Date : $Date: 2012/02/24 17:42:06 $
########################################################################################################
#
# ESCOTA # PROJET MIGRAZUR
########################################################################################################
#
# SCRIPT xzsa01_ListeMachine.sh
########################################################################################################
#
# DESCRIPTION DU SCRIPT	:
#
#  Fournit la liste des machines du systeme.
#
# CODE DE RETOUR	:
#
#     0	si la commande est executee correctement ;
#  <> 0	sur erreur.
#
# REMARQUE		:
#
#  La source de donnees de la commande est le fichier de configuration
#     "XDC_NSUP_PATHOPER/XDF_NSUP_ADMINIS"
#
########################################################################################################
#
# HISTORIQUE DES MODIFICATIONS :
#
# JPL	24/02/12 : Creation
#
########################################################################################################

Nom_Programme=`basename $0`


# Definition des parametres de configuration
# ------------------------------------------

XDC_PATH_MIGRAZURHOME="/produits/migrazur/appliSD"
XDC_NSUP_PATHOPER="$XDC_PATH_MIGRAZURHOME/fichiers/adm"

XDF_NSUP_ADMINIS="nsup_adminis"


# Verifier l'existence et la validite des fichiers de configuration
# -----------------------------------------------------------------

if [ ! -r "$XDC_NSUP_PATHOPER/$XDF_NSUP_ADMINIS" ]; then
	echo "*** $Nom_Programme : Fichier $XDF_NSUP_ADMINIS introuvable"  >&2
	exit 1
fi


# Extraire les machines du fichier de configuration
# -------------------------------------------------

grep '^[ 	]*Machine[ 	]'  "$XDC_NSUP_PATHOPER/$XDF_NSUP_ADMINIS"  |
awk '{ print $2 " " $3 " " $5 }'  |
sort

exit 0
