#!/bin/ksh
# Fichier : $Id: xzag23.ksh,v 1.4 1998/02/04 16:29:09 gaborit Exp $      Release : $Revision: 1.4 $        Date : $Date: 1998/02/04 16:29:09 $
#################################################################################
#
# STERIA # PROJET MIGRAZUR
#################################################################################
#
# SOUS-SYSTEME BASEDD
#################################################################################
#
# SCRIPT xzag23.ksh
#################################################################################
#
# DESCRIPTION DU SCRIPT :
#
# Demander a la replication de se reconnecter sur EXP et HIS
#
#################################################################################
#
# HISTORIQUE DES MODIFICATIONS :
# C.T.  10/08/95	: creation (1.1)
# C.T.  02/09/96	: ajout serveur DC et DS (1.2)
# C.T.  26/09/96	: Modif mot de passe sa (1.3)
# P.N.  29/01/98	: Ajout DA DY DP 1.4
#################################################################################
#

#
#################################################################################
#
# Service rendu
# Reprendre la connexion sur EXP et HIS (si au CI)
#
# Sequence d'appel
# xzag23.ksh Site 
#
# Arguments en entree
# Site          : CI ou DN ou DM ou DC ou DS ou DY ou DP ou DA
#
# Code retour
# 0             : sauvegarde ok
# 1             : syntaxe d'appel incorrecte
#
# Conditions d'utilisation
#
#################################################################################
#

# tester si le nom du site est correct
if [[ $1 != "CI" && $1 != "DN" && $1 != "DM" && $1 != "DC" && $1 != "DS" && $1 != "DA" && $1 != "DP" && $1 != "DY" ]]
then
	echo "syntaxe : xzag23.ksh site"
	echo "          site : CI ou DN ou DM ou DC ou DS ou DA ou DP ou DY"
	exit 1
fi


# reprendre la connexion avec la base EXP
#########################################
# creer la commande
Commande="resume connection to SQL_${1}.EXP"

# envoi de la commande au serveur de replication du site
echo "$Commande \n\
go\n" \
| isql -SREP_${1} -Usa -Pzigramur >> /dev/null

# reprendre la connexion avec la base HIS si au CI
##################################################
if [[ $1 = "CI" ]]
then
# creer la commande
Commande="resume connection to SQL_CI.HIS"

# envoi de la commande au serveur de replication du site
echo "$Commande \n\
go\n" \
| isql -SREP_CI -Usa -Pzigramur >> /dev/null
fi

exit 0
