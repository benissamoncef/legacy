#!/bin/ksh

###############################################################################
## Fichier : @(#)tienvoivh.ksh	1.1      Release : 1.1        Date : 10/29/12
###############################################################################
## ESCOTA * PROJET PASTRE
###############################################################################
## SOUS-SYSTEME  GESIHM
###############################################################################
## MODULE MTVH * FICHIER tienvoivh.ksh
###############################################################################
## DESCRIPTION DU MODULE :
##
##   Module d'envoi du document de Viabilite Hivernale a la DIT.
##
##   Effectue la copie du document sur le serveur de protocole
##   puis execute sur ce serveur une commande de transfert
##   du document au destinataire final.
##
##   Ecrit le compte-rendu de l'action complete dans un journal.
##
###############################################################################
## HISTORIQUE :
##
## JP	10/10/2012	: Creation  1.1
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
NOM_FICH_VH=vh-donnees-ESCOTA.xml

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
##  FONCTION     : Ecrire_Compte_Rendu
##
##  ARGUMENTS    :
##    $1 : Valeur du compte-rendu (0 pour OK) ;
##    $2 : Texte de compte-rendu.
##
##  COMPTE RENDU :
##    Aucun.
##
###############################################################################

Ecrire_Compte_Rendu ()
{
	cr="$1"
	texte_cr="$2"

	DATE_CR=$(date '+%d/%m/%Y %H:%M:%S')

	# Determiner les valeurs de statut de l'action
	if [[ "$cr" = "0" ]]; then
		STATUT="Succes"
		DATE_SUCCES=\"$DATE_CR\"
		DATE_ECHEC=NULL
	else
		STATUT="ECHEC"
		DATE_SUCCES=NULL
		DATE_ECHEC=\"$DATE_CR\"
	fi

	# Effectuer la requete en base pour ecrire le compte-rendu
	cr_ecriture_base=0
#	{
#		print "exec PRC..XZACnn  $NUM_ACTION, $NUM_SITE, $DATE_SUCCES, $DATE_ECHEC"
#		print "go"
#	}  | isql -UMIGRAZUR -PMIGRAZUR  -SSQL_${NOM_SITE}  |
#	     sed -n '/return.*status/s/[^0-9]*//gp'  |
#	     read cr_ecriture_base

	if [[ "$texte_cr" != "" ]]; then
		DETAILS=" ($texte_cr)"
	fi

	if [[ "$cr_ecriture_base" != "0" ]]; then
		DETAILS="$DETAILS ; ERREUR D'ECRITURE EN BASE"
	fi

#	print "$(date)  Action $NUM_ACTION : $STATUT $DETAILS"  >>/tmp/$NOM_PROG.log
	print "$(date)  Action Envoi VH : $STATUT $DETAILS"  >>/tmp/$NOM_PROG.log
}



###############################################################################
##
##                                Debut du script
##
###############################################################################


# Recuperer les identifiants de l'action et du site
# -------------------------------------------------
NUM_ACTION=$1
NUM_SITE=$2
NOM_SITE="CI"

CR_CMD=0
TXT_CR_CMD=""


# Initialisation Sybase
# ---------------------
[[ "$SYBASE" = "" ]]  &&  export SYBASE=/produits/sybase
if [[ -x "$SYBASE/SYBASE.sh" ]]; then
	.  $SYBASE/SYBASE.sh
else
	export PATH=$PATH:$SYBASE/bin
fi



# Transferer le document VH sur le serveur passerelle
# ---------------------------------------------------
fichierVH=${REP_FICH_VH}/${NOM_FICH_VH}

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
	Txt_Err_Transfert=$(xzsx_cmd_sur.sh "$SERVEUR_PROTO"  ". ./.profile >&- 2>&- ; titransfertVH.ksh >&-" 2>&1)
	CR_CMD=$?
	TXT_CR_CMD=$(print $Txt_Err_Transfert | head -1)
fi



# Ecrire le compte-rendu de l'action en base et le retourner a l'appelant
# -----------------------------------------------------------------------

Ecrire_Compte_Rendu  "$CR_CMD" "$TXT_CR_CMD"


if [[ "$TXT_CR_CMD" != "" ]]; then
	print "$TXT_CR_CMD"  >&2
fi

exit $CR_CMD
