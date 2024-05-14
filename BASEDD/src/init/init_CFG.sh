#!/bin/ksh
# Fichier : $Id: init_CFG.sh,v 1.2 1996/06/19 11:36:59 verdier Exp $        Release : $Revision: 1.2 $        Date : $Date: 1996/06/19 11:36:59 $
##################################################################################
# STERIA # PROJET MIGRAZUR
##################################################################################
# SOUS-SYSTEME BASEDD
##################################################################################
# SCRIPT init_CFG.sh
##################################################################################
# DESCRIPTION DU SCRIPT :
# renseigne les tables de CFG a partir des constantes
# dÅfinies dans XDC (È faire au CI)
##################################################################################
# HISTORIQUE DES MODIFICATIONS :
# B.G.	11/10/94	: Creation
# C.T.	15/03/95	: Ajout initialisation des tables de type
# P.V.  19/06/96        : Ajout initialisation de la table des types de libelles
##################################################################################
# repertoire des includes
includes=$HOME/migrazur/XDMICG/inc

# initialisation des noms des fonctions operateur
rm init.c
echo "#include \"$includes/xdc.h\"" > init.c
cat fonctions.bas >> init.c
cc -P -C init.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_CI <init.i
rm init.c init.i

# initialisation des types de FMC
rm init.c
echo "#include \"$includes/xzaec.h\"" > init.c
cat fmc.bas >> init.c
cc -P -C init.c
isql -UMIGRAZUR -PMIGRAZUR -SSQL_CI <init.i
rm init.c init.i

# initialisation des types de FMC
isql -UMIGRAZUR -PMIGRAZUR -SSQL_CI <alarmes.bas

# initialisation des types d'alerte
isql -UMIGRAZUR -PMIGRAZUR -SSQL_CI <alertes.bas

# initialisation des types de carburant
isql -UMIGRAZUR -PMIGRAZUR -SSQL_CI <carburants.bas

# initialisation des categories d'astreintes
isql -UMIGRAZUR -PMIGRAZUR -SSQL_CI <categories.bas

# initialisation des fonctions operateur par machine
isql -UMIGRAZUR -PMIGRAZUR -SSQL_CI <familles.bas

# initialisation des differents motifs d'appel
isql -UMIGRAZUR -PMIGRAZUR -SSQL_CI <motifs.bas

# initialisation des differents types de libelles
isql -UMIGRAZUR -PMIGRAZUR -SSQL_CI <libelles.bas

exit 0
