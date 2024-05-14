#!/bin/ksh
#Fichier :  $Id: tisvsc.ksh,v 1.1 1996/02/02 15:13:11 noel Exp $      Release : $Revision: 1.1 $        Date : $Date: 1996/02/02 15:13:11 $
#------------------------------------------------------
# STERIA *  PROJET MIGRAZUR 
#------------------------------------------------------
#* SOUS-SYSTEME  GESIHM
#------------------------------------------------------
#* MODULE MT * FICHIER  tisvsc.ksh
#------------------------------------------------------
#* DESCRIPTION DU MODULE :
#*
#*  Sauvegarde des fichiers de construction des scénarios de  
#*  formation sur bande DAT
#*   
#------------------------------------------------------
#* HISTORIQUE :
#*
#* Noel     30 Jan  1996     : Creation
########################################################

cd /produits/migrazur/appliSD/fichiers/dyn

echo "Mettre la cassette DAT dans le lecteur"
echo "Taper o et  <Entree> pour continuer"
echo "Taper n et  <Entree> pour arreter"
echo ""
read Reponse
if [[ $Reponse = "o" ]]
then
	tar cvf /dev/rmt/c201d3m /produits/migrazur/appliSD/fichiers/dyn/*.fmt
	if [ "$?" != "0" ]
	then
   		Reponse=""
   		echo "La Sauvegarde a echoue"
   		echo "Taper <Entree>"
   		read  Reponse
   		exit -1
	else
   		Reponse=""
   		echo "La Sauvegarde a reussi"
   		echo "Taper <Entree>"
   		read  Reponse
   		exit 0
	fi

else
	exit 0
fi
