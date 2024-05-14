#!/bin/ksh

###############################################################################
## Fichier : $Id: titransfert_MTBR.ksh,v 1.4 2013/01/09 17:13:45 devgfi Exp $      Release : $Revision: 1.4 $        Date : $Date: 2013/01/09 17:13:45 $
###############################################################################
## ESCOTA * PROJET MIGAZUR / PASTRE
###############################################################################
## SOUS-SYSTEME  GESIHM
###############################################################################
## MODULE ITMA_TVH * FICHIER titransfert_MTBR.ksh
###############################################################################
## DESCRIPTION DU MODULE :
##
##   Fonction d'exportation de documents Bureautique depuis le poste SAE
##   sur le serveur du site local au moyen du protocole FTP.
##
###############################################################################
## HISTORIQUE :
##
## JPL	19/12/2012 : Creation  1.1
## JPL	20/12/2012 : Ajout du support de sftp pour Linux  1.2
## JPL	04/01/2013 : Modification port serveur, compte, MdP, repertoire ...  1.3
## JPL	09/01/2013 : Usage de ftp sur toutes types de postes  1.4
###############################################################################

# Ignorer les signaux hangup, interrupt et quit
# ---------------------------------------------
trap "" 1 2 3


NOM_PROG=${0##*/}

LISTE_FICH_SRC="$1"
REPERTOIRE_SRC="$2"

REPERTOIRE_DEST="$3"
if [[ "$REPERTOIRE_DEST" = "" ]]; then
	REPERTOIRE_DEST="."
fi


# Parametrage
# -----------
FICHIER_JOURNAL=/tmp/${NOM_PROG}.log
FICHIER_TRACE=/tmp/${NOM_PROG}.tra


# Commande utilisee pour le transfert
PROG_FTP=ftp
#if [[ $(uname -s) = "Linux" ]]; then
#	PROG_FTP=sftp
#fi


SERVEUR_DEST=10.202.10.250
IDENTIFIANT_DEST="svc_ftp"
MOT_DE_PASSE_DEST='$erv1ce'

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
	if [[ $(uname -s) = "HP-UX" ]]; then
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
	"

	cat "$LISTE_FICH_SRC" |
	while read f; do
		print "delete $f"
		f_majusc=$(echo "$f" | tr '[a-z]' '[A-Z]')
		case "$f_majusc" in
		   *.TXT) print "ascii" ;;
		   *)     print "binary" ;;
		esac
		print "put $f"
	done

	print "ls -1"
	print quit
}


Commandes_Transfert_Sftp ()
{
	print "cd $REPERTOIRE_DEST"

	cat "$LISTE_FICH_SRC" |
	while read f; do
		print "rm $f"
		print "put $f"
	done

	print "ls -1"
	print quit
}



###############################################################################
##
##                                Debut du script
##
###############################################################################


CR_CMD=0
TXT_CR_CMD=""


rm -f $FICHIER_TRACE


if [[ -d "$REPERTOIRE_SRC" ]]; then
	cd "$REPERTOIRE_SRC"
fi


# Verifier la disponibilite des donnees
# -------------------------------------
if [[ -r "$LISTE_FICH_SRC" ]]; then
	cat "$LISTE_FICH_SRC" |
	while read f; do
		if [[ ! -r "$f" ]]; then
			CR_CMD=$ERR_FIC_ABSENT
			TXT_CR_CMD="L'un des fichiers à transférer ($f) est introuvable."
		fi
	done
else
	CR_CMD=$ERR_FIC_ABSENT
	TXT_CR_CMD="La liste des fichiers à transférer est introuvable."
fi



# Verifier la disponibilite du serveur
# ------------------------------------
if [[ $CR_CMD -eq 0 ]]; then
	Machine_Accessible "$SERVEUR_DEST"  ||  {
		CR_CMD=$ERR_COMM_SRVR
		TXT_CR_CMD="Le serveur destinataire est inaccessible."
	}
fi



# Si les conditions sont reunies, transferer les documents
# --------------------------------------------------------
if [[ $CR_CMD -eq 0 ]]; then
	if [[ $PROG_FTP = "ftp" ]]; then
		Commandes_Transfert_Ftp  |  ftp -in  >$FICHIER_TRACE  2>&1
	else
		Commandes_Transfert_Sftp  |  sftp -b - -oPort=22  ${IDENTIFIANT_DEST}@${SERVEUR_DEST}  >$FICHIER_TRACE  2>&1
	fi

	cat $LISTE_FICH_SRC |
	while read f; do
		grep -q "^${f}$"  $FICHIER_TRACE  ||  {
			CR_CMD=$ERR_TRANSFERT
			TXT_CR_CMD="Le fichier \"$f\" n'a pas été correctement transféré."
		}
	done
fi



# Terminer en signalant les erreurs a l'appelant
# ----------------------------------------------

#rm -f $FICHIER_TRACE
#rm -f $LISTE_FICH_SRC


if [[ "$TXT_CR_CMD" != "" ]]; then
	print "$(date)	$TXT_CR_CMD"  >>$FICHIER_JOURNAL
	print "$TXT_CR_CMD"  >&2
fi

exit $CR_CMD
