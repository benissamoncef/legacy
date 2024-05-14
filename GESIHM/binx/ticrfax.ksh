#!/bin/ksh

## Fichier : @(#)ticrfax.ksh	1.4      Release : 1.4        Date : 07/07/09
###############################################################################
## STERIA *  PROJET MIGRAZUR
###############################################################################
## SOUS-SYSTEME  GESIHM
###############################################################################
## MODULE ITMA_TFX * FICHIER ticrfax.ksh
###############################################################################
## DESCRIPTION DU MODULE :
##
##   Module d'envoi de fax.
##   Script appele par vsifax pour rendre compte du succÉs ou de l'Åchec
##   d'une requÁte d'Åmission de fax (cf option -P de la commande fx).
##
##   Cf. DCG 2.4.33
##
###############################################################################
## HISTORIQUE :
##
## Lecrivain  18 Avr 1995 : Creation d'apres JMG d'apres LD d'apres ...   V1.1
# Niepceron 6 avril 1998 : Modif pour nouvelle version vsifax 	v1.2
###############################################################################

# Ignorer les signaux hangup, interrupt et quit
# ---------------------------------------------
trap "" 1 2 3


NOMPROG=${0##*/}

PATHMIGRAZUR=/produits/migrazur/appliSD
PATHFICHFAX=$PATHMIGRAZUR/fichiers/dyn

TACHESCAN=tiscanfax.ksh


# Si un identifiant de requete VSIFAX a ete fourni alors
# ------------------------------------------------------
ID_REQ_VSI=$1

horo=`date`

[[ "$ID_REQ_VSI" = "" ]]  ||  {

    # laisser au spooler le temps de mettre È jour le fichier OLOG
    # puis rÅcupÅrer les caractÅristiques de la requÁte VSIFAX
    # ------------------------------------------------------------
    sleep 3		
	eval `/produits/vsifax/bin/vfxolog -F eval "$1"`
    # et si un numero d'action est donne creer le fichier de compte-rendu
    # -------------------------------------------------------------------
    NUMACTION=$tg2
    print "$horo, action  : $NUMACTION      requete  : $ID_REQ_VSI" >>/tmp/$NOMPROG.log
    [[ "$NUMACTION" = "" ]]  ||  {
	DATECR=`date '+%d/%m/%Y %H:%M:%S'`
        print "DATE  : $DATECR" >>/tmp/$NOMPROG.log
	print $NUMACTION $tg1 $rrs $DATECR >$PATHFICHFAX/$ID_REQ_VSI.cr
    }
}


# Si la tache de scrutation des CR de fax n'est pas active alors la lancer
# ------------------------------------------------------------------------

ps -umigrazur | grep $TACHESCAN || ps -uvsifax | grep $TACHESCAN  ||  {
    $PATHMIGRAZUR/exec/$TACHESCAN &
}

exit 0
