#!/bin/ksh
# Fichier : $Id: xzag20.ksh,v 1.4 1998/02/04 16:32:45 gaborit Exp $      Release : $Revision: 1.4 $        Date : $Date: 1998/02/04 16:32:45 $
#################################################################################
#
# STERIA # PROJET MIGRAZUR
#################################################################################
#
# SOUS-SYSTEME BASEDD
#################################################################################
#
# SCRIPT xzag20.ksh
#################################################################################
#
# DESCRIPTION DU SCRIPT :
#
# Dechencher le demarrage d'un log transfer manager d'un site
#
#################################################################################
#
# HISTORIQUE DES MODIFICATIONS :
# C.T.  28/11/94        : Creation (V 1.1)
# C.T.  05/12/94        : Modif de l'emplacement du fichier (V 1.2)
# C.T.  21/03/95        : Modif du chemin d'acces du fichier de lancement (V 1.3)
# C.T.  02/09/96        : Ajout cas des serveurs DC et DS (V 1.4)
## P.N.  29/01/98	: Ajout DA DY DP 1.5

#################################################################################
#

#
#################################################################################
#
# Service rendu
# Dechencher le redemarrage d'un log transfer manager mort durant la session 
# du replication server manager
#
# Sequence d'appel
# xzag20.ksh Ltm 
#
# Arguments en entree
# Ltm          : CI_CFG_LTM ou CI_EXP_LTM ou CI_RSSD_LTM ou 
#                DN__EXP_LTM ou DN_RSSD_LTM ou
#                DM__EXP_LTM ou DM_RSSD_LTM ou
#                DC__EXP_LTM ou DC_RSSD_LTM 
#                DS__EXP_LTM ou DS_RSSD_LTM 
#                DA__EXP_LTM ou DA_RSSD_LTM 
#                DY__EXP_LTM ou DY_RSSD_LTM 
#                DP__EXP_LTM ou DP_RSSD_LTM 
#
# Code retour
# 0             : sauvegarde ok
# 1             : le dechenchement ne provient pas de la mort d'un ltm : aucune action
#
# Conditions d'utilisation
# Ce script est dechenche automatiquement par le rsm lorsqu'un ltm meurt.
# Le nom du ltm peut etre :
# pour le CI : CI_CFG_LTM ou CI_EXP_LTM ou CI_RSSD_LTM
# pour le DN : DN_EXP_LTM ou DN_RSSD_LTM
# pour le DM : DM_EXP_LTM ou DM_RSSD_LTM
# pour le DC : DC_EXP_LTM ou DC_RSSD_LTM
# pour le DS : DS_EXP_LTM ou DS_RSSD_LTM
# pour le DY : DY_EXP_LTM ou DY_RSSD_LTM
# pour le DP : DP_EXP_LTM ou DP_RSSD_LTM
# pour le DA : DA_EXP_LTM ou DA_RSSD_LTM
#
#################################################################################
#

# tester si le nom du serveur est celui d'un ltm
if  [[ $# < 1 || ($1 != "CI_CFG_LTM" && $1 != "CI_EXP_LTM" && \
       $1 != "CI_RSSD_LTM" && \
       $1 != "DN_EXP_LTM" && $1 != "DN_RSSD_LTM" && \
       $1 != "DC_EXP_LTM" && $1 != "DC_RSSD_LTM" && \
       $1 != "DS_EXP_LTM" && $1 != "DS_RSSD_LTM" &&\
       $1 != "DY_EXP_LTM" && $1 != "DY_RSSD_LTM" && \
       $1 != "DP_EXP_LTM" && $1 != "DP_RSSD_LTM" && \
       $1 != "DA_EXP_LTM" && $1 != "DA_RSSD_LTM" && \
       $1 != "DM_EXP_LTM" && $1 != "DM_RSSD_LTM") ]]
then
	exit 1
fi

unset LANG

# lancement du ltm de la base 
if [[ $1 = "CI_CFG_LTM" || $1 = "CI_EXP_LTM" || $1 = "CI_RSSD_LTM" ]]
then
rm /produits/migrazur/appliSD/traces/${1}.log.old
mv /produits/migrazur/appliSD/traces/${1}.log /produits/migrazur/appliSD/traces/${1}.log.old
cd /produits/migrazur/appliSD/traces;$SYBASE/install/startserver -f $SYBASE/RUN_${1}
elif [[ $1 = "DC_RSSD_LTM" || $1 = "DC_EXP_LTM" ]]
then
remsh podc1 -l migrazur "export SYBASE=/produits/sybase;rm /produits/migrazur/appliSD/traces/${1}.log.old;\
mv /produits/migrazur/appliSD/traces/${1}.log /produits/migrazur/appliSD/traces/${1}.log.old;\
cd /produits/migrazur/appliSD/traces;$SYBASE/install/startserver -f $SYBASE/RUN_${1}"
elif [[ $1 = "DS_RSSD_LTM" || $1 = "DS_EXP_LTM" ]]
then
remsh pods1 -l migrazur "export SYBASE=/produits/sybase;rm /produits/migrazur/appliSD/traces/${1}.log.old;\
mv /produits/migrazur/appliSD/traces/${1}.log /produits/migrazur/appliSD/traces/${1}.log.old;\
cd /produits/migrazur/appliSD/traces;$SYBASE/install/startserver -f $SYBASE/RUN_${1}"
elif [[ $1 = "DY_RSSD_LTM" || $1 = "DY_EXP_LTM" ]]
then
remsh pods1 -l migrazur "export SYBASE=/produits/sybase;rm /produits/migrazur/appliSD/traces/${1}.log.old;\
mv /produits/migrazur/appliSD/traces/${1}.log /produits/migrazur/appliSD/traces/${1}.log.old;\
cd /produits/migrazur/appliSD/traces;$SYBASE/install/startserver -f $SYBASE/RUN_${1}"
elif [[ $1 = "DP_RSSD_LTM" || $1 = "DP_EXP_LTM" ]]
then
remsh pods1 -l migrazur "export SYBASE=/produits/sybase;rm /produits/migrazur/appliSD/traces/${1}.log.old;\
mv /produits/migrazur/appliSD/traces/${1}.log /produits/migrazur/appliSD/traces/${1}.log.old;\
cd /produits/migrazur/appliSD/traces;$SYBASE/install/startserver -f $SYBASE/RUN_${1}"
elif [[ $1 = "DA_RSSD_LTM" || $1 = "DA_EXP_LTM" ]]
then
remsh pods1 -l migrazur "export SYBASE=/produits/sybase;rm /produits/migrazur/appliSD/traces/${1}.log.old;\
mv /produits/migrazur/appliSD/traces/${1}.log /produits/migrazur/appliSD/traces/${1}.log.old;\
cd /produits/migrazur/appliSD/traces;$SYBASE/install/startserver -f $SYBASE/RUN_${1}"
elif [[ $1 = "DN_RSSD_LTM" || $1 = "DN_EXP_LTM" ]]
then
remsh sddn1 -l migrazur "export SYBASE=/produits/sybase;rm /produits/migrazur/appliSD/traces/${1}.log.old;\
mv /produits/migrazur/appliSD/traces/${1}.log /produits/migrazur/appliSD/traces/${1}.log.old;\
cd /produits/migrazur/appliSD/traces;$SYBASE/install/startserver -f $SYBASE/RUN_${1}"
else
remsh sddm1 -l migrazur "export SYBASE=/produits/sybase;rm /produits/migrazur/appliSD/traces/${1}.log.old;\
mv /produits/migrazur/appliSD/traces/${1}.log /produits/migrazur/appliSD/traces/${1}.log.old;\
cd /produits/migrazur/appliSD/traces;$SYBASE/install/startserver -f $SYBASE/RUN_${1}"
fi

exit 0
