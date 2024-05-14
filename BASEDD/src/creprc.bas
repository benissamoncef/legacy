#!/bin/ksh
# Fichier : $Id: creprc.bas,v 1.1 1994/09/15 17:55:18 torregrossa Exp $        Release : $Revision: 1.1 $        Date : $Date: 1994/09/15 17:55:18 $
##################################################################################
# STERIA # PROJET MIGRAZUR
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# SCRIPT creprc.bas
##################################################################################
# DESCRIPTION DU SCRIPT :
# creation des types de la base PRC
#
#
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# C.T.	30/08/94	: Creation
##################################################################################


# controle des parametres d'appels
if ((($#<2)) || (test $2 != "PRC"))
then echo "syntaxe : creprc.bas NomServeur PRC NomFicErreur"
     echo "          NomBase : PRC"
     echo "          NomFicErreur optionnel"
     exit 0
fi

# serveur a utiliser
NomServeur=$1

# nom de la base
NomBase=$2
Use="prc.use"

# fichier de compte rendu d'erreur
if (($#==3))
then FicErr=${3}.err
else FicErr="creprc.err"
fi


################
# Creation des types 
################

cat $Use cretyp.bas | isql -U MIGRAZUR -P MIGRAZUR -S $NomServeur >> $FicErr

# Test si un probleme est survenu lors de la creation de type
grep "Msg" $FicErr | wc -l | read NbLignes ResteInfo
if ((NbLignes!=0))
then exit 0
fi
rm $FicErr

exit 1
