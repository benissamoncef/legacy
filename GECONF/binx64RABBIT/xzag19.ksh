#!/bin/ksh
# Fichier : $Id: xzag19.ksh,v 1.6 1996/12/06 09:52:16 gaborit Exp $      Release : $Revision: 1.6 $        Date : $Date: 1996/12/06 09:52:16 $
#################################################################################
#
# STERIA # PROJET MIGRAZUR
#################################################################################
#
# SOUS-SYSTEME BASEDD
#################################################################################
#
# SCRIPT xzag19.ksh
#################################################################################
#
# DESCRIPTION DU SCRIPT :
#
# Dechencher le demarrage du replication server manager
#
#################################################################################
#
# DESCRIPTION DU SCRIPT :
#
# Dechencher le demarrage du replication server manager
#
#################################################################################
#
# HISTORIQUE DES MODIFICATIONS :
# C.T.  25/11/94        : Creation
# C.T.  05/12/94        : Ajout des export de SYBASE pour rsm_agent (V 1.2)
# C.T.  21/03/95        : Lancement du rsm sur PODFA (V 1.3)
# C.T.  06/09/96        : Ajout des rsm_agent (V1.4)
# C.T.  15/10/96        : Modif test de presence rsm_agent (V1.6)
#
#################################################################################
#

#
#################################################################################
#
# Service rendu
# Dechencher le demarrage du replication server manager sur le CI
#
# Sequence d'appel
# xzag19.ksh NomMachine
#
# Arguments en entree
# 
#
# Code retour
# 0       : sauvegarde ok
# 1       : syntaxe d'appel incorrecte
#
# Conditions d'utilisation
#
#################################################################################
#

if [ $# != 1 ]
then
	echo "syntaxe : xzag19.ksh NomMachine"
	exit 1
fi

# pour la connection au serveur X
xhost +&

# lancement du rsm_agent pour lire le fichier log du CI (si pas en cours)
remsh sdci1 -l migrazur "ps -ef | grep sm_agent | grep rsm_agent" | wc -l | read testLancement ResteInfo
if [[ $testLancement = 0 ]]
then
remsh sdci1 -l migrazur "unset LANG;export SYBASE=/produits/sybase;cd /produits/migrazur/appliSD/traces;/produits/sybase/bin/rsm_agent RSM_CI  > /dev/null 2> /dev/null &"
fi

# lancement du rsm_agent pour lire le fichier log de Nice (si pas en cours)
remsh sddn1 -l migrazur "ps -ef | grep sm_agent | grep rsm_agent" | wc -l | read testLancement ResteInfo
if [[ $testLancement = 0 ]]
then
remsh sddn1 -l migrazur "unset LANG;export SYBASE=/produits/sybase;cd /produits/migrazur/appliSD/traces;/produits/sybase/bin/rsm_agent RSM_DN  > /dev/null 2> /dev/null &"
fi

# lancement du rsm_agent pour lire le fichier log de Mandelieu (si pas en cours)
remsh sddm1 -l migrazur "ps -ef | grep sm_agent | grep rsm_agent" | wc -l | read testLancement ResteInfo
if [[ $testLancement = 0 ]]
then
remsh sddm1 -l migrazur "unset LANG;export SYBASE=/produits/sybase;cd /produits/migrazur/appliSD/traces;/produits/sybase/bin/rsm_agent RSM_DM  > /dev/null 2> /dev/null &"
fi

# lancement du rsm_agent pour lire le fichier log de Cannet (si pas en cours)
remsh podc1 -l migrazur "ps -ef | grep sm_agent | grep rsm_agent" | wc -l | read testLancement ResteInfo
if [[ $testLancement = 0 ]]
then
remsh podc1 -l migrazur "unset LANG;export SYBASE=/produits/sybase;cd /produits/migrazur/appliSD/traces;/produits/sybase/bin/rsm_agent RSM_DC  > /dev/null 2> /dev/null &"
fi

# lancement du rsm_agent pour lire le fichier log de Saint Maximin (si pas en cours)
remsh pods1 -l migrazur "ps -ef | grep sm_agent | grep rsm_agent" | wc -l | read testLancement ResteInfo
if [[ $testLancement = 0 ]]
then
remsh pods1 -l migrazur "unset LANG;export SYBASE=/produits/sybase;cd /produits/migrazur/appliSD/traces;/produits/sybase/bin/rsm_agent RSM_DC  > /dev/null 2> /dev/null &"
fi

# lancement du replication server manager
export DISPLAY="${1}:0.0"
if [[ `hostname` = "PODFA" ]]
then
testLancement=`ps -ef | grep rsm$ | wc -l`
else
remsh podfa -l migrazur "ps -ef | grep rsm$" | wc -l | read testLancement ResteInfo
fi
if [[ $testLancement = 0 ]]
then
remsh podfa -l migrazur "export SYBASE=/produits/sybase;export PATH=/usr/bin/X11:$PATH;export PATH=$SYBASE/bin:$PATH;export DISPLAY=\"${1}:0.0\";unset LANG;cd /produits/migrazur/appliSD/traces;/produits/sybase/bin/rsm > /dev/null 2> /dev/null &"
fi

exit 0

