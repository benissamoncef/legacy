#!/bin/ksh
# Fichier : $Id: tcsvm.ksh,v 1.2 1998/10/15 13:28:25 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 1998/10/15 13:28:25 $
################################################################################
#
#
# STERIA # PROJET MIGRAZUR
################################################################################
#
#
# SOUS-SYSTEME GECONF
################################################################################
#
#
# SCRIPT tcsvm.ksh
################################################################################
#
#
# DESCRIPTION DU SCRIPT :
#
# Dechencher des sauvegardes manuelles de bases de donnees
#
################################################################################
##
#
# HISTORIQUE DES MODIFICATIONS :
# C.T.  09/01/95        : Creation
#
################################################################################
#
#

#
################################################################################
#
#
# Service rendu
# Dechencher la sauvegarde manuelle d'une base
#
# Sequence d'appel
# tcsvm.ksh
#
# Arguments en entree
#
# Code retour
# 0             : fin
#
# Conditions d'utilisation
# Un nom du fichier sur la bande est attribue par base sauvegardee
#
################################################################################
#
#

# positionnement de la variable d'environnement 
###############################################

export exec_migrazur="/produits/migrazur/appliSD/exec"
vl_Repertoire="/produits/migrazur/appliSD/fichiers/dyn/"

# affichage du menu a l'utilisateur
###################################

echo "\n\n 1 - Sauvegarde des tables systemes. \n
 2 - Sauvegarde des tables de la base historique. \n
 3 - Sauvegarde des tables de la base configuration. \n
 4 - Sortie. \n
 \n
 Option choisie : \c"

# declenchement de la sauvegarde apres les saisies
##################################################

while read vl_Reponse
do
	case $vl_Reponse in
		1) 
			# sauvegarde des tables systemes
			echo "   Saisir le nom du volume             (6 caracteres) : \c"
			read vl_NomVolume

			if [[ "$vl_NomVolume" = "" || `expr "$vl_NomVolume" : '\(......\)'` != $vl_NomVolume ]]
			then
				echo "\n   Le nom de volume est incorrect."
				echo "   La sauvegarde est interrompue"
			else
				echo "   Saisir le nom generique des fichiers sur bande (16 caracteres) : \c"
				read vl_NomFichier
				if [[ "$vl_NomFichier" = "" || `expr "$vl_NomFichier" : '\(................\)'` != $vl_NomFichier ]]
				then
					echo "\n   Le nom de fichier est incorrect."
					echo "   La sauvegarde est interrompue"
				else
					# modification du nom du device
					echo "\n   Modifier le device (par defaut : /dev/rmt/0mn) : \c"
					read vl_Device
					if [[ "$vl_Device" = "" ]] then
						vl_Device="/dev/rmt/0mn"
					fi

					echo "\n\n   Recapitulatif : \n   
   La sauvegarde s'effectuera dans le volume \"$vl_NomVolume\" dans les fichiers suivants : \n
     - \"${vl_NomFichier}1\" : base master\n
     - \"${vl_NomFichier}2\" : base model\n
     - \"${vl_NomFichier}3\" : base tempdb\n
     - \"${vl_NomFichier}4\" : base sybsystemprocs\n
     - \"${vl_NomFichier}5\" : base RSSD\n"

					# declenchement de la sauvegarde
					xzag08.ksh CI $vl_Device $vl_NomVolume systeme $vl_Repertoire$vl_NomFichier 1433600 globale $vl_NomFichier
				fi
			fi;;
		2) 
			# sauvegarde des tables historiques
			echo "   Saisir le nom du volume             (6 caracteres) : \c"
			read vl_NomVolume

			if [[ "$vl_NomVolume" = "" || `expr "$vl_NomVolume" : '\(......\)'` != $vl_NomVolume ]]
			then
				echo "\n   Le nom de volume est incorrect."
				echo "   La sauvegarde est interrompue"
			else
				echo "   Saisir le nom du fichier sur bande (17 caracteres) : \c"
				read vl_NomFichier
				if [[ "$vl_NomFichier" = "" || `expr "$vl_NomFichier" : '\(.................\)'` != $vl_NomFichier ]]
				then
					echo "\n   Le nom de fichier est incorrect."
					echo "   La sauvegarde est interrompue"
				else
					# modification du nom du device
					echo "\n   Modifier le device (par defaut : /dev/rmt/0mn) : \c"
					read Device
					if [[ $vl_Device = "" ]] then
						vl_Device="/dev/rmt/0mn"
					fi

					echo "\n\n   Recapitulatif : \n   
   La sauvegarde des tables d'historique s'effectuera dans le volume \"$vl_NomVolume\" dans le fichier \"$vl_NomFichier\"\n"

					# declenchement de la sauvegarde
					xzag08.ksh CI $vl_Device $vl_NomVolume HIS $vl_Repertoire$vl_NomFichier 1433600 globale $vl_NomFichier
				fi
			fi;;
		3) 
			# sauvegarde des tables de configuration
			echo "   Saisir le nom du volume             (6 caracteres) : \c"
			read vl_NomVolume

			if [[ "$vl_NomVolume" = "" || `expr "$vl_NomVolume" : '\(......\)'` != $vl_NomVolume ]]
			then
				echo "\n   Le nom de volume est incorrect."
				echo "   La sauvegarde est interrompue"
			else
				echo "   Saisir le nom du fichier sur bande (17 caracteres) : \c"
				read vl_NomFichier
				if [[ "$vl_NomFichier" = "" || `expr "$vl_NomFichier" : '\(.................\)'` != $vl_NomFichier ]]
				then
					echo "\n   Le nom de fichier est incorrect."
					echo "   La sauvegarde est interrompue"
				else
					# modification du nom du device
					echo "\n   Modifier le device (par defaut : /dev/rmt/0mn) : \c"
					read Device
					if [[ $vl_Device = "" ]] then
						vl_Device="/dev/rmt/0mn"
					fi

					echo "\n\n   Recapitulatif : \n   
   La sauvegarde s'effectuera dans le volume \"$vl_NomVolume\" dans le fichier \"$vl_NomFichier\"\n"

					# declenchement de la sauvegarde
					xzag08.ksh CI $vl_Device $vl_NomVolume CFG $vl_Repertoire$vl_NomFichier 1433600 globale $vl_NomFichier
				fi
			fi;;
		4) break;;
	esac

	if [[ $vl_Reponse != 4 ]] then
		echo "\n\n 1 - Sauvegarde des tables systemes. \n
 2 - Sauvegarde des tables de la base historique. \n
 3 - Sauvegarde des tables de la base configuration. \n
 4 - Sortie. \n
 \n
 Option choisie : \c"
	fi
done

exit 0
