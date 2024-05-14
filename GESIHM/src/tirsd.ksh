# -------------------------------------------------------------
# -- Fichier permettant de recuperer depuis le DAT les fichiers
#    de parametrage a mettre en reference.
#     usage: tirsd.ksh 
#
# version : $Revision: 1.2 $	Date : $Date: 1995/09/01 11:57:47 $
#	
# Historique :
# B. Noel	16/06/95	Creation		V1.1
# P. Niepceron  01/09/95	Ajout appel tirecref.ksh v1.2
# --------------------------------------------------------------

 
cd /users/gesconf/migrazur/reference

xterm -title "Recuperation de la Reference" -e /produits/migrazur/appliSD/exec/tirecref.ksh

exit 0  	
