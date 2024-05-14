#!/bin/ksh
# -------------------------------------------------------------
# -- Fichier permettant de lancer ticfg pour la configuration
# des plans de secours.
# 
#     usage: ticfgps.ksh 
#
# version : $Revision: 1.4 $	Date : $Date: 1996/06/17 11:53:27 $
#	
# Historique :
# P. Niepceron  05/09/95	Creation   
# P.Niepceron   17/06/96	Modifcation de /dev/console en /dev/null pour
#				eviter des msg dans l'écran en cas de perte de 
#				console
# --------------------------------------------------------------

cd ../modif/fichiers/ign


export DISPLAY=`hostname`:0
PATH=$PATH:/produits/migrazur/appliSD/exec


ticfg.x  >>/dev/null

exit 0  	
