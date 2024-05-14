#!/bin/ksh

###############################################################################
## Fichier : $Id: tienvoivh_ASF.ksh,v 1.1 2015/02/17 17:43:12 devgfi Exp $      Release : $Revision: 1.1 $        Date : $Date: 2015/02/17 17:43:12 $
###############################################################################
## ESCOTA * PROJET PASTRE
###############################################################################
## SOUS-SYSTEME  GESIHM
###############################################################################
## MODULE MTVH * FICHIER tienvoivh_ASF.ksh
###############################################################################
## DESCRIPTION DU MODULE :
##
##   Module d'envoi du document de Viabilite Hivernale aux ASF.
##
##   Effectue la copie du document sur le serveur de protocole
##   puis execute sur ce serveur une commande de transfert
##   du document au destinataire final.
##
###############################################################################
## HISTORIQUE :
##
## JPL	17/02/2015	: Creation  1.1
##
###############################################################################

# Ignorer les signaux hangup, interrupt et quit
# ---------------------------------------------
trap "" 1 2 3


NOM_PROG=${0##*/}


# Parametrage
# -----------
REP_MIGRAZUR=/produits/migrazur/appliSD
REP_FICH_VH=$REP_MIGRAZUR/fichiers/dyn

SERVEUR_PROTO=SPCI


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
##                                Debut du script
##
###############################################################################


# Recuperer le nom du fichier a envoyer
# -------------------------------------
fichierVH=$1
NOM_FICH_VH=$(basename "$fichierVH")

CR_CMD=0
TXT_CR_CMD=""


# Transferer le document VH sur le serveur passerelle
# ---------------------------------------------------
if Machine_Accessible "$SERVEUR_PROTO"
then
	{
		chmod a+w  "$fichierVH"
		rcp "$fichierVH"  $SERVEUR_PROTO:/tmp
	} 2>&-

	if [[ $? -ne 0 ]]; then
		CR_CMD=$ERR_TRANSFERT
		TXT_CR_CMD="Erreur lors du transfert du document sur le site local"
	fi
else
	CR_CMD=$ERR_COMM_SRVR
	TXT_CR_CMD="Serveur local inaccessible"
fi



# Si le transfert sur le serveur local s'est bien deroule alors
# commander au serveur le transfert du document au destinataire
# -------------------------------------------------------------
if [[ $CR_CMD -eq 0 ]]; then
	Txt_Err_Transfert=$(xzsx_cmd_sur.sh "$SERVEUR_PROTO"  ". ./.profile >&- 2>&- ; titransfertVH_ASF.ksh $NOM_FICH_VH >&-" 2>&1)
	CR_CMD=$?
	TXT_CR_CMD=$(print $Txt_Err_Transfert | head -1)
fi



# Retourner le compte-rendu de l'envoi a l'appelant
# -------------------------------------------------

if [[ "$TXT_CR_CMD" != "" ]]; then
	print "$TXT_CR_CMD"  >&2
fi

exit $CR_CMD
