#!/bin/ksh
# Fichier : $Id: xzma_transfert.ksh,v 1.2 1999/12/20 16:21:00 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 1999/12/20 16:21:00 $
#################################################################################
#
# STERIA # PROJET MIGRAZUR
#################################################################################
#
# SOUS-SYSTEME MAINTE
#################################################################################
#
# SCRIPT xzma_transfert.ksh
#################################################################################
#
# DESCRIPTION DU SCRIPT :
#
# Dechencher la recopie des fichiers de compteur de marche sur pocma
#
#################################################################################
#
# HISTORIQUE DES MODIFICATIONS :
# B.G.  28/12/95        : Creation
# D.M.  20/12/99        : Suppression de la recopie des fichier de compteur vers POCMA
#
#################################################################################
#
# Service rendu
#
# Dechencher la recopie des fichiers de compteur de marche sur pocma
#
# Sequence d'appel
# xzma_transfert.ksh  duree_en_minutes
#
# Arguments en entree
# duree_en_minute : temporisation avant de faire le rcp
#
# Conditions d'utilisation
#
# appelé par la tache tmcpt sur pocma
#################################################################################
#

# test des arguments d'entrée
if ((($# != 1)) || ( [[ $1 != [0-9] ]] && [[ $1 != [0-9][0-9] ]] && [[ $1 != [0-9][0-9][0-9] ]] ) || [[ $1 -lt 1 ]] || [[ $1 -gt 120 ]] )
then    
	echo "Usage : xzma_transfert.ksh  duree_en_minutes (comprise entre 1 et 120 minutes)"
	exit 1
fi

# temporisation de $1 minutes
sleep `expr $1 \* 60`

# sur la liste des fichiers transfert...
for i in `ls /produits/migrazur/appliSD/fichiers/compteur/transfert*`
do
   # recopie des fichiers sur pocma
#   rcp $i pocma:$i

   # si le rcp s'est bien déroulé, rm du fichier
#   if [[ $? -eq 0 ]]
#   then
      rm $i
#   fi
done

