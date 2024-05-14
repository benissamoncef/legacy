#!/bin/ksh

###############################################################################
## Fichier : @(#)titransfertVH.ksh	1.2      Release : 1.2        Date : 10/30/12
###############################################################################
## ESCOTA * PROJET PASTRE
###############################################################################
## SOUS-SYSTEME  GESIHM
###############################################################################
## MODULE ITMA_TVH * FICHIER titransfertVH.ksh
###############################################################################
## DESCRIPTION DU MODULE :
##
##   Fonction de diffusion du document de Viabilite Hivernale
##   au destinataire DIT au moyen du protocole FTP.
##
###############################################################################
## HISTORIQUE :
##
## JPL	10/10/2012	: Creation  1.1
## JPL	30/10/2012	: Repertoire sur le serveur; configuration de test  1.2
## PNI  xx/xx/2014      : Linux  1.3
## PNI  17/02/2015      : Utilisation de /usr/bin/ftp  1.4
## PNI	07/11/2017	: modif du put 1.5 
## PNI	12/11/2021	: put et rename en 2 pahse pour récupérer la trace (le ls <fic> <trace> demande confirmation) de puis migration MOVIS 1.6
###############################################################################

# Ignorer les signaux hangup, interrupt et quit
# ---------------------------------------------
trap "" 1 2 3


NOM_PROG=${0##*/}


# Parametrage
# -----------
FICHIER_TRACE=/tmp/${NOM_PROG}.log
NOM_FICH_VH=vh-donnees-ESCOTA.xml
NOM_FICH_VH_TMP=vh-donnees-ESCOTA.ren
FICHIER_VH=/tmp/$NOM_FICH_VH
#FICHIER_VH=$NOM_FICH_VH

#SERVEUR_DEST=192.168.192.170
#COMPTE_DEST="ftp-escota rieboo2O"
#REPERTOIRE_DEST=.
SERVEUR_DEST=DATEX1TIPI
COMPTE_DEST="ftp-ESCOTA rXo7rd5z"
#SERVEUR_DEST=DATEX1TST
#COMPTE_DEST="ftp-ESCOTA AbxTMb8v"
#TEST : COMPTE_DEST="ftp-ESCOTA AbxTMb8v"
REPERTOIRE_DEST=.

# Pour test
#SERVEUR_DEST=194.5.181.237
#COMPTE_DEST="root xxxxx"
#REPERTOIRE_DEST=/tmp


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

Commandes_Transfert ()
{
#		delete $NOM_FICH_VH
	echo "
		open $SERVEUR_DEST
		user $COMPTE_DEST
		cd $REPERTOIRE_DEST
		binary
		put $FICHIER_VH $NOM_FICH_VH_TMP 
		ls $NOM_FICH_VH_TMP
		quit
	"
		#rename $NOM_FICH_VH_TMP $NOM_FICH_VH
}

Commandes_Transfert_fin ()
{
	echo "
	        open $SERVEUR_DEST
	        user $COMPTE_DEST
	        cd $REPERTOIRE_DEST
		binary
		rename $NOM_FICH_VH_TMP $NOM_FICH_VH
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
cd $(dirname $FICHIER_VH)


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
	#Commandes_Transfert  |  /usr/bin/ftp -in -u  >&-  2>&-
	Commandes_Transfert > /tmp/ftp.lis
	cat /tmp/ftp.lis |  /usr/bin/ftp >$FICHIER_TRACE
	Commandes_Transfert_fin > /tmp/ftp.lis
	cat /tmp/ftp.lis |  /usr/bin/ftp > /tmp/ftpvh.err

	#Commandes_Transfert  |  /usr/bin/ftp -in 

	touch $FICHIER_TRACE
	grep -q "${NOM_FICH_VH_TMP}"  $FICHIER_TRACE  ||
	{
		CR_CMD=$ERR_TRANSFERT
		TXT_CR_CMD="Erreur lors du transfert du document au destinataire"
	}
fi


# Terminer en signalant les erreurs a l'appelant
# ----------------------------------------------

#rm -f $FICHIER_TRACE
rm -f $FICHIER_VH


if [[ "$TXT_CR_CMD" != "" ]]; then
	print "$TXT_CR_CMD"  >&2
fi

exit $CR_CMD
