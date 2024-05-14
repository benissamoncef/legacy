#################################################################################
#
# STERIA # PROJET MIGRAZUR
#################################################################################
#
# SOUS-SYSTEME BASEDD
#################################################################################
#
# SCRIPT ITMA_TFX.ksh
#################################################################################
#
# DESCRIPTION DU SCRIPT :
#
# Suppression des accents et dans caracteres provoquant une erreur pour 
# l'envoi d'un fax
#
#################################################################################
#
# HISTORIQUE DES MODIFICATIONS :
# C.T.  04/11/94        : Creation
# C.T.  20/12/95        : Modif interface du script (V1.2)
# P.N.	23/12/96	: Ajout d'autres caracteres... (1.3)
# JPL	11/10/11	: Completer a tous les caracteres productibles au clavier  1.6
# JPL	19/04/16	: Restriction a quelques caracteres (crochets, etoile ...)  1.7
#################################################################################

#
#################################################################################
#
# Service rendu
# Filtrage d'un fichier de caracteres
#
# Sequence d'appel
# ITMA_TFX.ksh FichierAFiltrer FichierResultat
#
# Arguments en entree
# FichierAFiltrer       : fichier contenant le texte a filtrer
# FichierResultat       : fichier contenant le texte filtre
#
# Code retour
# neant          
#
#################################################################################

sed  -e 'y/[]*/()x/'    \
-e  "s/|/\//g" -e "s/\`/\'/g" $1 > $2
