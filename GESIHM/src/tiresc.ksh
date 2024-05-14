#!/bin/ksh
#Fichier :  $Id: tiresc.ksh,v 1.1 1996/02/02 15:12:42 noel Exp $      Release : $Revision: 1.1 $        Date : $Date: 1996/02/02 15:12:42 $
#------------------------------------------------------
# STERIA *  PROJET MIGRAZUR 
#------------------------------------------------------
#* SOUS-SYSTEME  GESIHM
#------------------------------------------------------
#* MODULE MT * FICHIER tiresc.ksh
#------------------------------------------------------
#* DESCRIPTION DU MODULE :
#*
#*  Recuperation des fichiers de construction des scénarios de
#*  formation a partir d'une bande DAT
#*   
#------------------------------------------------------
#* HISTORIQUE :
#*
#* Noel     30 Jan  1996     : Creation
########################################################

cd /produits/migrazur/appliSD/fichiers/dyn

echo "Mettre la cassette DAT dans le lecteur"
echo "Attention la récupération va écraser les scénarios actuellement en place"
echo "Arréter la récupération et lancer une sauvegarde si nécessaire"
echo "Taper o et  <Entree> pour continuer"
echo "Taper n et  <Entree> pour arreter"
echo ""
read Reponse
if [[ $Reponse = "o" ]]
then
	rm *.fmt

	tar tvf /dev/rmt/c201d3m
	if [ "$?" != "0" ]
	then
		echo "La lecture des fichiers scenario a échoué"
		print "Taper <Entree>"
		read Reponse
		exit -1
	else
		echo "Les fichiers ci-dessus seront recupérés"
		echo "Taper o et <Entree> pour continuer"
		echo "Taper n et <Entree> pour arreter"
		read Reponse
		if [[ $Reponse = "o" ]]
		then
			tar xvf /dev/rmt/c201d3m 
			if [ "$?" != "0" ]
			then
   				echo "La Récupération a échoué"
   				print "Taper <Entree>"
   				read Reponse
   				exit -1
			else
   				echo "La Récuperation a réussi"
   				print "Taper <Entree>"
   				read Reponse
   				exit 0 
			fi
		else
			exit 0
		fi

	fi
else
	exit 0
fi


