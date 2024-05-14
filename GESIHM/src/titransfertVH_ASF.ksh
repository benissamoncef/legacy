#!/bin/ksh

###############################################################################
##
###############################################################################
## ESCOTA * PROJET PASTRE
###############################################################################
## SOUS-SYSTEME  GESIHM
###############################################################################
## MODULE ITMA_TVH * FICHIER titransfertVH_ASF.ksh
###############################################################################
## DESCRIPTION DU MODULE :
##
##   Fonction de diffusion du document de Viabilite Hivernale
##   au destinataire DIT au moyen du protocole FTP.
##
###############################################################################
## HISTORIQUE :
##
## JPL	17/02/2015	: Creation  1.1
## PNI	18/02/2015	: ajout envoi doublé 1.2
## PNI	16/03/2015	: changement de serveurs ASF 1.3
## PNI	12/10/2020	: changement de serveurs ASF 1.4
#  PNI	17/11/2021	: suppression des \\ dans les mot de passe suite à la migration MOVIS 1.5
###############################################################################

# Ignorer les signaux hangup, interrupt et quit
# ---------------------------------------------
trap "" 1 2 3


NOM_PROG=${0##*/}



# Parametrage
# -----------
FICHIER_TRACE=/tmp/${NOM_PROG}.logasf
FICHIER_TRACE2=/tmp/${NOM_PROG}.logasf2

SERVEUR_DEST=FTP-TRA-CCVH.vinci-autoroutes.com
COMPTE_DEST='ftp-tra-ccvh.vinci-autoroutes.com|va\\svc_ftp_ccvh_esc_prd gjxjCg7bZPaJgKIN'
REPERTOIRE_DEST=CCVH_ESCOTA
SERVEUR_DEST2=FTP-TRA-CCVH-PP.vinci-autoroutes.com
COMPTE_DEST2='ftp-tra-ccvh-pp.vinci-autoroutes.com|va\\svc_ftp_ccvh_esc_pp 5FUgFvHzHh4n3HLKZ6SB'
REPERTOIRE_DEST2=CCVH_ESCOTA

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
		put $NOM_FICH_VH
		ls $NOM_FICH_VH  
		quit
	"
}

Commandes_Transfert2 ()
{
#               delete $NOM_FICH_VH
        echo "
        	open $SERVEUR_DEST2
        	user $COMPTE_DEST2
                cd $REPERTOIRE_DEST2
                binary
                put $NOM_FICH_VH
                ls $NOM_FICH_VH  
                quit
        "
}


###############################################################################
##
##                                Debut du script
##
###############################################################################


NOM_FICH_VH=$1
FICHIER_VH=/tmp/$NOM_FICH_VH


CR_CMD=0
TXT_CR_CMD=""


rm -f $FICHIER_TRACE
cd $(dirname $FICHIER_VH)


# Verifier la disponibilite du serveur et des donnees
# ---------------------------------------------------

#if Machine_Accessible "$SERVEUR_DEST"
#then
	if [[ ! -r "$FICHIER_VH" ]]; then
		CR_CMD=$ERR_FIC_ABSENT
		TXT_CR_CMD="Aucun document de Viabilite Hivernale"
	fi
#else
#	CR_CMD=$ERR_COMM_SRVR
#	TXT_CR_CMD="Serveur destinataire inaccessible"
#fi


# Si les conditions sont reunies, transferer le document VH
# puis verifier la presence de la copie sur le destinataire
# ---------------------------------------------------------
if [[ $CR_CMD -eq 0 ]]; then
	#Commandes_Transfert  |  /usr/bin/ftp -in -u  >&-  2>&-
	Commandes_Transfert >/tmp/cmd.txt
	Commandes_Transfert  |  /usr/bin/ftp -in >  $FICHIER_TRACE
	Commandes_Transfert2  |  /usr/bin/ftp -in >  $FICHIER_TRACE2

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
rm -f $FICHIER_VH


if [[ "$TXT_CR_CMD" != "" ]]; then
	echo "$TXT_CR_CMD"  >&2
fi

exit $CR_CMD
