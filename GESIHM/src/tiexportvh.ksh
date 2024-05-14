#!/bin/ksh

###############################################################################
## Fichier : $Id: tiexportvh.ksh,v 1.2 2013/01/16 19:03:48 gesconf Exp $      Release : $Revision: 1.2 $        Date : $Date: 2013/01/16 19:03:48 $
###############################################################################
## ESCOTA * PROJET PASTRE
###############################################################################
## SOUS-SYSTEME  GESIHM
###############################################################################
## MODULE ITMA_TVH * FICHIER tiexportvh.ksh
###############################################################################
## DESCRIPTION DU MODULE :
##
##   Fonction de d'exportation du document de Viabilite Hivernale
##   au format bureautique sur le serveur prevu au moyen du protocole FTP.
##
###############################################################################
## HISTORIQUE :
##
## JPL	13/11/2012 : Creation  1.1
## JPL	21/12/2012 : Nouveau compte d'acces serveur, repertoire SAE/EIT  1.2
##
###############################################################################

# Ignorer les signaux hangup, interrupt et quit
# ---------------------------------------------
trap "" 1 2 3


NOM_PROG=${0##*/}

FICHIER_VH="$1"


# Parametrage
# -----------
FICHIER_TRACE=/tmp/${NOM_PROG}.tra

SERVEUR_DEST=10.202.10.250
IDENTIFIANT_DEST="svc_ftp"
MOT_DE_PASSE_DEST='$erv1ce'
REPERTOIRE_DEST=EIT

# Surcharge pour test
#SERVEUR_DEST=194.5.181.237
#IDENTIFIANT_DEST="root"
#MOT_DE_PASSE_DEST="xxxxx"
#REPERTOIRE_DEST=/tmp

COMPTE_DEST="$IDENTIFIANT_DEST $MOT_DE_PASSE_DEST"


ERR_FIC_ABSENT=2
ERR_COMM_SRVR=3
ERR_TRANSFERT=4



###############################################################################
##
##  FONCTION     : Machine_Accessible
##
##  ARGUMENTS    :
##    $1 : Nom de la machine a acceder.
##
##  COMPTE RENDU :
##    Vrai (0) ou faux (non nul) selon que la machine est ou non accessible.
##
###############################################################################

Machine_Accessible ()
{
	machine=$1

	# Envoyer UNE sonde a la machine cible
	if [[ $(uname) = "HP-UX" ]]; then
		ping "$machine" -n 1 -m 5  >&-  2>&-
	else
		ping "$machine" -c 1 -w 5  >&-  2>&-
	fi
}



###############################################################################
##
##  FONCTION     : Commandes_Transfert
##
##  ARGUMENTS    : Aucun
##
##  COMPTE RENDU : Aucun
##
###############################################################################

Commandes_Transfert_Ftp ()
{
	print "
		open $SERVEUR_DEST
		user $COMPTE_DEST
		cd ./SAE
		cd $REPERTOIRE_DEST
		delete $NOM_FICH_VH
		binary
		put $NOM_FICH_VH
		ls $NOM_FICH_VH  $FICHIER_TRACE
		quit
	"
}



###############################################################################
##
##                                Debut du script
##
###############################################################################


CR_CMD=0
TXT_CR_CMD=""


rm -f $FICHIER_TRACE


NOM_FICH_VH=$(basename "$FICHIER_VH")
cd $(dirname "$FICHIER_VH")


# Verifier la disponibilite du serveur et des donnees
# ---------------------------------------------------

if Machine_Accessible "$SERVEUR_DEST"
then
	if [[ ! -r "$FICHIER_VH" ]]; then
		CR_CMD=$ERR_FIC_ABSENT
		TXT_CR_CMD="Aucun document de Viabilite Hivernale"
	fi
else
	CR_CMD=$ERR_COMM_SRVR
	TXT_CR_CMD="Serveur destinataire inaccessible"
fi


# Si les conditions sont reunies, transferer le document VH
# puis verifier la presence de la copie sur le destinataire
# ---------------------------------------------------------
if [[ $CR_CMD -eq 0 ]]; then
	Commandes_Transfert_Ftp  |  ftp -in  >/dev/null  2>&1

	touch $FICHIER_TRACE
	grep -q "${NOM_FICH_VH} *$"  $FICHIER_TRACE  ||
	{
		CR_CMD=$ERR_TRANSFERT
		TXT_CR_CMD="Erreur lors du transfert du document au destinataire"
	}
fi


# Terminer en signalant les erreurs a l'appelant
# ----------------------------------------------

#rm -f $FICHIER_TRACE
#rm -f $FICHIER_VH


if [[ "$TXT_CR_CMD" != "" ]]; then
	print "$TXT_CR_CMD"  >&2
fi

exit $CR_CMD
