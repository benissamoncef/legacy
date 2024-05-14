# -------------------------------------------------------------
# -- Fichier permettant de sauvegarder sur DAT les fichiers
#    d'un type donne depuis l'environnement de reference
#     usage: tisrd.ksh 
#
# version : $Revision: 1.2 $	Date : $Date: 1995/09/01 11:57:44 $
#	
# Historique :
# B. Noel	16/06/95	Creation		V1.1
# P. Niepceron  01/09/95	appel de tisvgref.ksh   v1.2
# --------------------------------------------------------------

 

#tar cvf /dev/rmt/c201d3m /users/gesconf/migrazur/reference/fichiers

xterm -title "Sauvegarde de la Reference" -e /produits/migrazur/appliSD/exec/tisvgref.ksh

exit 0  	
