#!/bin/ksh
# Fichier : $Id: xzag16.ksh,v 1.6 1998/08/25 12:10:58 gesconf Exp $      Release : $Revision: 1.6 $        Date : $Date: 1998/08/25 12:10:58 $
#################################################################################
#
# STERIA # PROJET MIGRAZUR
#################################################################################
#
# SOUS-SYSTEME BASEDD
#################################################################################
#
# SCRIPT xzag16.ksh
#################################################################################
#
# DESCRIPTION DU SCRIPT :
#
# Dechencher le demarrage des log transfer manager d'un site
#
#################################################################################
#
# HISTORIQUE DES MODIFICATIONS :
# C.T.  25/11/94        : Creation  non teste (V 1.1)
# C.T.  25/11/94        : Correction erreur  (V 1.2)
# C.T.  20/03/95        : Modif nom du serveur de replication  (V 1.3)
# C.T.  02/09/96        : Ajout cas des serveurs DC et DS (V 1.4)
# P.N.  29/01/98	: Ajout DA DY DP 1.5
# P.N.	25/08/98	: Ajout HIS 1.6
#################################################################################
#

#
#################################################################################
#
# Service rendu
# Dechencher le demarrage des log transfer manager d'un site (CI ou DN ou DM ou DY ou DP ou DA ou HIS)
#
# Sequence d'appel
# xzag16.ksh Site 
#
# Arguments en entree
# Site          : CI ou DN ou DM ou DY ou DP ou DA ou HIS
#
# Code retour
# 0             : sauvegarde ok
# 1             : syntaxe d'appel incorrecte
#
# Conditions d'utilisation
# L'execution du script doit etre effectue dans le repertoire
# "/produits/migrazur/appliSD/traces"
# pour que les traces soient dans le repertoire 
# "/produits/migrazur/appliSD/traces" prevu a cet effet
#
#################################################################################
#

# tester si le nom du site est correct
if  [[ $# < 1 || ($1 != "CI" && $1 != "DN" && $1 != "DM" && $1 != "DC" && $1 != "DS" && $1 != "DA" && $1 != "DP" && $1 != "DY"  && $1 != "HIS" ) ]]
then
	echo "syntaxe : xzag15.ksh site nomBase"
	echo "          site : CI ou DN ou DM ou DC ou DS ou DA ou DP ou DY ou HIS"
	echo "          nomBase : EXP ou RSSD ou CFG (optionnel)"
	exit 1
fi

# tester si le nom du ltm specifie est correct
if [[ ($# = 2 && $2 != "EXP" && $2 != "RSSD" && $2 != "CFG") || \
   ($# = 2 && $1 != "CI" && $2 = "CFG") ]]
then
	echo "syntaxe : xzag15.ksh site nomBase"
	echo "          site : CI ou DN ou DM ou DC ou DS ou DA ou DP ou DY ou HIS"
	echo "          nomBase : EXP ou RSSD ou CFG (optionnel)"
	exit 1
fi

unset LANG

# cas ou un seul ltm doit etre lance
if [ $# = 2 ]
then
rm /produits/migrazur/appliSD/traces/${1}_${2}_LTM.log.old
mv /produits/migrazur/appliSD/traces/${1}_${2}_LTM.log /produits/migrazur/appliSD/traces/${1}_${2}_LTM.log.old
$SYBASE/install/startserver -f $SYBASE/RUN_${1}_${2}_LTM
exit 0
fi

# lancement du ltm de la base RSSD (base de la replication)
rm /produits/migrazur/appliSD/traces/${1}_RSSD_LTM.log.old
mv /produits/migrazur/appliSD/traces/${1}_RSSD_LTM.log /produits/migrazur/appliSD/traces/${1}_RSSD_LTM.log.old
$SYBASE/install/startserver -f $SYBASE/RUN_${1}_RSSD_LTM

# attente que le lancement du ltm soit effectue
sleep 10

if [ $1 != "HIS" ]
then
# lancement du ltm de la base EXP
rm /produits/migrazur/appliSD/traces/${1}_EXP_LTM.log.old
mv /produits/migrazur/appliSD/traces/${1}_EXP_LTM.log /produits/migrazur/appliSD/traces/${1}_EXP_LTM.log.old
$SYBASE/install/startserver -f $SYBASE/RUN_${1}_EXP_LTM
fi 

# attente que le lancement du ltm soit effectue
sleep 10

# lancement du ltm de la base CFG (existant qu'au CI)
if [ $1 = "CI" ]
then
rm /produits/migrazur/appliSD/traces/${1}_CFG_LTM.log.old
mv /produits/migrazur/appliSD/traces/${1}_CFG_LTM.log /produits/migrazur/appliSD/traces/${1}_CFG_LTM.log.old
$SYBASE/install/startserver -f $SYBASE/RUN_${1}_CFG_LTM
fi

# attente que le lancement du ltm soit effectue
sleep 10

export LANG=french

exit 0
