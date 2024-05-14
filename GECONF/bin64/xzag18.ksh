#!/bin/ksh
# Fichier : $Id: xzag18.ksh,v 1.4 1998/02/04 16:25:55 gaborit Exp $      Release : $Revision: 1.4 $        Date : $Date: 1998/02/04 16:25:55 $
#################################################################################
#
# STERIA # PROJET MIGRAZUR
#################################################################################
#
# SOUS-SYSTEME BASEDD
#################################################################################
#
# SCRIPT xzag18.ksh
#################################################################################
#
# DESCRIPTION DU SCRIPT :
#
# Dechencher le demarrage des serveurs
#
#################################################################################
#
# HISTORIQUE DES MODIFICATIONS :
# C.T.  25/11/94        : Creation non teste (V 1.1)
# C.T.  28/11/94        : Correction test sur resultat script non teste (V 1.2)
# C.T.  02/09/96        : Ajout cas des serveurs DC et DS (V 1.3)
## P.N.  29/01/98	: Ajout DA DY DP 1.4

#################################################################################
#

#
#################################################################################
#
# Service rendu
# Dechencher le demarrage des serveurs sur un site (CI ou DN ou DM ou DC ou DS ou DA ou DP ou DY)
#
# Sequence d'appel
# xzag18.ksh Site 
#
# Arguments en entree
# Site          : CI ou DN ou DM ou DC ou DS
#
# Code retour
# 0             : sauvegarde ok
# 1             : syntaxe d'appel incorrecte
#
# Conditions d'utilisation
# L'execution du script genere des traces dans le repertoire
# "/produits/migrazur/appliSD/traces"
# Tous les scripts doivent etre dans le repertoire "/produits/migrazur/appliSD/exec"
#
#################################################################################
#

# tester si le nom du site est correct
if [[ $1 != "CI" && $1 != "DN" && $1 != "DM" && $1 != "DC" && $1 != "DS"  && $1 != "DA" && $1 != "DP" && $1 != "DY" ]]
then
	echo "syntaxe : xzag18.ksh site"
	echo "          site : CI ou DN ou DM ou DC ou DS ou DA ou DP ou DY"
	exit 1
fi

# lancement du sql serveur
xzag21.ksh $1

# test si le lancement a ete effectue
if [ $? != 0 ]
then
	exit 1
fi

sleep 10

# lancement du serveur de replication
xzag15.ksh $1

# test si le lancement a ete effectue
if [ $? != 0 ]
then
	exit 1
fi

sleep 10

# lancement des ltm
xzag16.ksh $1

# test si le lancement a ete effectue
if [ $? != 0 ]
then
	exit 1
fi

# lancement du backup serveur
xzag17.ksh $1

# test si le lancement a ete effectue
if [ $? != 0 ]
then
	exit 1
fi

exit 0
