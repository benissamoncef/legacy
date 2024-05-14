#!/bin/ksh

## Fichier : %W%      Release : %I%        Date : %G%
###############################################################################
## STERIA *  PROJET MIGRAZUR
###############################################################################
## SOUS-SYSTEME  GESIHM
###############################################################################
## MODULE ITMA_TFX * FICHIER tisendfax.ksh
###############################################################################
## DESCRIPTION DU MODULE :
##
##   Module d'envoi de fax.
##
##   Cf. DCG 2.4.33
##
###############################################################################
## HISTORIQUE :
##
## Lecrivain  14 Avr 1995 : Creation d'apres JMG d'apres ...              V1.1
## Lecrivain  20 Avr 1995 : Ajout du nom du site                          V1.2
## Niepceron  04 Jan 1996 : Ajout de trace                                v1.3
## Niepceron  04 Avr 1998 : Ajout version vfx pour PC2                    v1.4
## Niepceron  19 Oct 2014 : Mise en echec de l'action , fonction fax désactivée DEM1102 v1.5
## RGR        23/10/18    : Correction Synchro revision                     1.9
###############################################################################

# Ignorer les signaux hangup, interrupt et quit
# ---------------------------------------------
trap "" 1 2 3

export LANG=""

NOMPROG=${0##*/}

PATHMIGRAZUR=/produits/migrazur/appliSD


# Recuperer les parametres d'appel
# --------------------------------
FICHIER=$1
NUMSITE=$2
NOMSITE=$3
PGGARDE=$4                                                      # 0 ou 1
NOMEMETTEUR=$5
OBJET=$6
NOMDEST=$7


# Lire les destinataires
# ----------------------
OLDIFS=$IFS
IFS="|"

print "fichier : $FICHIER" >>/tmp/$NOMPROG.log
while read NUMACTION NUMFAX; do
    print "action : $NUMACTION-$NUMFAX" >>/tmp/$NOMPROG.log
echo "action : $NUMACTION-$NUMFAX $NUMACTION, $NUMSITE $NOMSITE"

       #on met l action a ECHEC en base
	DATECR=`date '+%d/%m/%Y %H:%M:%S'`
	DATE_ECHEC=$DATECR
	DATE_SUCCES=NULL
echo "exec PRC..XZAC18  $NUMACTION, $NUMSITE, $DATE_SUCCES, \"$DATE_ECHEC\" 
go" >> /tmp/$NOMPROG.log

echo "exec PRC..XZAC18  $NUMACTION, $NUMSITE, $DATE_SUCCES, \"$DATE_ECHEC\" 
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_$NOMSITE >> /tmp/$NOMPROG.log 


done
