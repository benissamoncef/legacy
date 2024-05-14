# -------------------------------------------------------------
# -- Fichier permettant de mettre dans l'espace operationnel les fichiers de configuration
#    d'un type donne depuis l'environnement de travail de l'administrateur
#    systeme pour faire des tests.
#     usage: timop.ksh type_fichier
#	type_fichier = adm, aid, bdc, deq, dft, dra, eqa, ign, inf, mbo, nav, 
#			pic, pmv, snt, syn.
# version : $Revision: 1.2 $	Date : $Date: 1995/09/01 14:01:01 $
#	
# Historique :
# B. Noel	16/06/95	Creation		V1.1
# P. Niepceron  01/09/95	Ajout de l'option -r    v1.2
# --------------------------------------------------------------

 

cp -r /produits/migrazur/appliSD/modif/fichiers/$1 \
      /produits/migrazur/appliSD/fichiers

exit 0  	
