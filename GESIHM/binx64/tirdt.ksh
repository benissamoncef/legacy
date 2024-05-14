# -------------------------------------------------------------
# -- Fichier permettant de recuperer les derniers fichiers livres
#    d'un type donne pour les placer dans l'environnement de travail
#    de l'administrateur systeme.
#     usage: tirdt.ksh type_fichier
#	type_fichier = adm, aid, bdc, deq, dft, dra, eqa, ign, inf, mbo, nav, 
#			pic, pmv, snt, syn.
# version : $Revision: 1.3 $	Date : $Date: 1996/03/28 14:39:06 $
#	
# Historique :
# B. Noel	16/06/95	Creation		V1.1
# P. Niepceron  01/09/95        Ajout de l'option -r    v1.2
# JM. Guilhou   28/03/96        Modification chemins fichiers pour prendre la reference (DEM 969) V1.3
# --------------------------------------------------------------

 

cp -r /users/gesconf/migrazur/reference/fichiers/$1 \
      /produits/migrazur/appliSD/modif/fichiers

exit 0  	
