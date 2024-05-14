:

###############################################################################
## Fichier : titransfert_stat_radt.sh	                       29/10/2021
###############################################################################
## ESCOTA * PROJET MIGAZUR
###############################################################################
## SOUS-SYSTEME  GESIHM / OUTILS
###############################################################################
## DESCRIPTION DU MODULE :
##
##   Fonction de transfert de documents de statistiques RADT
##   depuis un poste jouant le role de passerelle
##   sur un serveur externe au SAE au moyen du protocole FTP.
##
###############################################################################
## HISTORIQUE :
##
## JPL	29/10/2021 : Creation  1.1
##
###############################################################################

# Ignorer les signaux hangup, interrupt et quit
# ---------------------------------------------
trap "" 1 2 3


NOM_PROG=${0##*/}

FICHIER_STAT="$1"


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
REPERTOIRE_DEST=EIT

# Surcharge a utiliser pour test
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
	echo "
		open $SERVEUR_DEST
		user $COMPTE_DEST
		cd ./SAE
		cd $REPERTOIRE_DEST
		delete $NOM_FICH_STAT
		ascii
		put $NOM_FICH_STAT
		ls $NOM_FICH_STAT
		quit
	"
}


Commandes_Transfert_Sftp ()
{
	echo "
		cd ./SAE
		cd $REPERTOIRE_DEST
		rm $NOM_FICH_STAT
		put $NOM_FICH_STAT
		ls $NOM_FICH_STAT
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


NOM_FICH_STAT=$(basename "$FICHIER_STAT")
cd $(dirname "$FICHIER_STAT")


# Verifier la disponibilite des donnees
# -------------------------------------
if [[ ! -r "$FICHIER_STAT" ]]; then
	CR_CMD=$ERR_FIC_ABSENT
	TXT_CR_CMD="Le fichier de statistiques est absent ou illisible."
fi


# Verifier la disponibilite du serveur
# ------------------------------------
if [[ $CR_CMD -eq 0 ]]; then
	Machine_Accessible "$SERVEUR_DEST"  ||  {
		CR_CMD=$ERR_COMM_SRVR
		TXT_CR_CMD="Le serveur destinataire est inaccessible."
	}
fi



# Si les conditions sont reunies, transferer le document
# ------------------------------------------------------
if [[ $CR_CMD -eq 0 ]]; then
	if [[ $PROG_FTP = "ftp" ]]; then
		Commandes_Transfert_Ftp  |  ftp -in  >$FICHIER_TRACE  2>&1
	else
		Commandes_Transfert_Sftp  |  sftp -b - -oPort=22  ${IDENTIFIANT_DEST}@${SERVEUR_DEST}  >$FICHIER_TRACE  2>&1
	fi

	grep -v "local:" $FICHIER_TRACE | grep -q "${NOM_FICHIER_STAT} *$"  ||  {
		CR_CMD=$ERR_TRANSFERT
		TXT_CR_CMD="Erreur lors du transfert du document au destinataire."
	}
fi



# Terminer en signalant les erreurs a l'appelant
# ----------------------------------------------

#rm -f $FICHIER_TRACE
rm -f $FICHIER_STAT


if [[ "$TXT_CR_CMD" != "" ]]; then
	#echo "$(date)	$TXT_CR_CMD"  >>$FICHIER_JOURNAL
	echo "$TXT_CR_CMD"  >&2
fi

exit $CR_CMD
