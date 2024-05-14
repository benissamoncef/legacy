#!/bin/ksh

## Fichier : @(#)tiscanfax.ksh	1.5      Release : 1.5        Date : 04/25/95
###############################################################################
## STERIA *  PROJET MIGRAZUR
###############################################################################
## SOUS-SYSTEME  GESIHM
###############################################################################
## MODULE ITMA_TFX * FICHIER tiscanfax.ksh
###############################################################################
## DESCRIPTION DU MODULE :
##
##   Module d'envoi de fax.
##   Script de scrutation des compte-rendus et d'archivage en base.
##
##   Cf. DCG 2.4.33
##
###############################################################################
## HISTORIQUE :
##
## Lecrivain  20 Avr 1995 : Creation                                      V1.1
## Lecrivain  21 Avr 1995 : Optimisation                                  V1.3
## Lecrivain  21 Avr 1995 : Arret de la tache s'il n'y a rien a traiter   V1.4
## Lecrivain  25 Avr 1995 : Trace plus pertinente                         V1.5
## Cluchague  06 Oct 2003 : export SYBASE pour sybase 12.5                V1.6
## Niepceron  12 Jan 2010 : ajout du cas $CR=FAILED	DEM927		  V1.7
###############################################################################

# Ignorer les signaux hangup, interrupt et quit
# ---------------------------------------------
trap "" 1 2 3


NOMPROG=${0##*/}

PATHMIGRAZUR=/produits/migrazur/appliSD
PATHFICHFAX=$PATHMIGRAZUR/fichiers/dyn

DELAI_SCRUT=30


# Initialisation Sybase
# ---------------------
export SYBASE=/produits/sybase
PATH=$PATH:$SYBASE/bin


###############################################################################
##                                                                           ##
##  FONCTION  : update_action                                                ##
##  ARGUMENTS :                                                              ##
##    $1 : Nom du fichier resultat de l'action.                              ##
##                                                                           ##
###############################################################################

function update_action
{
read NUMACTION NUMSITE NOMSITE CR DATECR < $1

# Selon le compte-rendu valuer la date de succes ou d'echec
# ---------------------------------------------------------
if [[ "$CR" = NORMAL ]]; then
    DATE_SUCCES=\"$DATECR\"; DATE_ECHEC=NULL
elif [[ "$CR" = MAXTRY ]]; then
    DATE_SUCCES=NULL; DATE_ECHEC=\"$DATECR\"
elif [[ "$CR" = FAILED ]]; then
    DATE_SUCCES=NULL; DATE_ECHEC=\"$DATECR\"
else
    return 0
fi


# puis mettre a jour la base de donnees
# -------------------------------------
cr=`{ print "exec PRC..XZAC;18  $NUMACTION, $NUMSITE, $DATE_SUCCES, $DATE_ECHEC"
      print go; } |
    isql -UMIGRAZUR -PMIGRAZUR -SSQL_$NOMSITE |
    grep 'return *status' |
    sed 's/status =/#/g' | awk -F# '{ print $2 }' | sed 's/[^-0-9]//g'`

if [[ "$cr" = "" ]]; then
    print $DATECR,Action $NUMACTION, fichier ${1##*/} : Erreur isql >>/tmp/$NOMPROG.log
    return 1
else
    #[[ $cr = 0 ]] ||
#	print $DATECR,Action $NUMACTION, fichier ${1##*/} : Compte-rendu isql=$cr >>/tmp/$NOMPROG.log
        horo=`date`
	print $horo,Action $NUMACTION, fichier ${1##*/} : Compte-rendu isql=$cr >>/tmp/$NOMPROG.log
    return $cr
fi
}


###############################################################################
##                                                                           ##
##                                  MAIN                                     ##
##                                                                           ##
###############################################################################

# A chaque periode de scrutation
# ------------------------------
while true; do

    # traiter chaque fichier de compte-rendu d'action fax present
    # -----------------------------------------------------------
    FICHFAX=`ls $PATHFICHFAX/*.cr 2>&-`
    [[ "$FICHFAX" = "" ]]  &&  return 0
    for f in $FICHFAX; do
#	update_action $f  &&  rm -f $f
	update_action $f  &&  mv -f $f /produits/migrazur/appliSD/fichiers/dyn/action.cr.old
    done

    sleep $DELAI_SCRUT
done
