:

###############################################################################
## Fichier : tienvoi_stat_radt.sh	                 Date : 22/10/21
###############################################################################
## ESCOTA * PROJET MIGRAZUR
###############################################################################
## SOUS-SYSTEME  GESIHM / OUTILS
###############################################################################
## MODULE MTOU * FICHIER tienvoi_stat_radt.sh
###############################################################################
## DESCRIPTION DU MODULE :
##
##   Outil de production et transfert quotidien sur un serveur OII
##   d'un document de statistiques sur le fonctionnement des stations RADT.
##
##   Interroge la base de donnees Historique pour elaborer les resultats
##   constitues d'un fichier au format CSV.
##
##   Effectue ensuite la copie du document sur un serveur jouant le role
##   de passerelle du SAE vers l'exterieur, puis execute sur ce serveur
##   une commande de transfert du document au destinataire final.
##
###############################################################################
## HISTORIQUE :
##
## JPL	22/10/2021	: Creation  1.1
## JPL	08/11/2021	: Parametre de la commande sur serveur passerelle  1.2
##
###############################################################################

# Ignorer les signaux hangup, interrupt et quit
# ---------------------------------------------
trap "" 1 2 3


NOM_PROG=${0##*/}

DATE_DU_JOUR=$(date '+%Y%m%d')


# Parametrage
# -----------
REP_MIGRAZUR=/produits/migrazur/appliSD
REP_FICH_STAT=${REP_MIGRAZUR}/fichiers/dyn

RACINE_FICH_STAT=Statistiques-RADT
NOM_FICH_STAT=${RACINE_FICH_STAT}-${DATE_DU_JOUR}.csv
FICHIER_STAT=${REP_FICH_STAT}/${NOM_FICH_STAT}

NOM_FICH_TMP=${RACINE_FICH_STAT}-${DATE_DU_JOUR}.$$.csv


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
##  FONCTION     : Produire_Statistiques_RADT
##
##  ARGUMENTS    :
##    Aucun
##
##  COMPTE RENDU :
##    Aucun.
##
###############################################################################

Produire_Statistiques_RADT ()
{
	rm -f ${FICHIER_STAT}

	AUJOURD_HUI=$(date '+%d/%m/%Y')
	#Fichier_Tempo=${REP_FICH_STAT}/${NOM_FICH_TMP}

	{
		echo ";;;;;Rapport quotidien d'état de fonctionnement des stations RADT en date du ${AUJOURD_HUI}"
		echo ""
	} > ${FICHIER_STAT}

	# Effectuer la requete en base pour extraire les resultats
	{
		echo exec PRC..FM_Statistiques_RADT '"'${AUJOURD_HUI}'"', NULL
		echo "go"
	}  | isql -SSQL_HIS  -UMIGRAZUR -PMIGRAZUR  -w2000  -s ';'  |
	     sed -e '/^(/d'  -e '/^.---/d'  -e 's/J_/J-/g'   >> ${FICHIER_STAT}
}



###############################################################################
##
##                                Debut du script
##
###############################################################################


CR_CMD=0
TXT_CR_CMD=""


. $HOME/.profile  >&-  2>&-

# Initialisation Sybase
# ---------------------
[[ "$SYBASE" = "" ]]  &&  export SYBASE=/produits/sybase
if [[ -x "$SYBASE/SYBASE.sh" ]]; then
	.  $SYBASE/SYBASE.sh
else
	export PATH=$PATH:$SYBASE/bin
fi



# Produire le document
# --------------------
Produire_Statistiques_RADT
chmod a+w  "${FICHIER_STAT}"



# Transferer le document de statistiques sur le serveur passerelle
# ----------------------------------------------------------------
if Machine_Accessible "${SERVEUR_PROTO}"
then
	{
		rcp "${FICHIER_STAT}"  ${SERVEUR_PROTO}:/tmp
	} 2>&-

	if [[ $? -ne 0 ]]; then
		CR_CMD=$ERR_TRANSFERT
		TXT_CR_CMD="Erreur lors du transfert du document sur le site passerelle"
	fi
else
	CR_CMD=$ERR_COMM_SRVR
	TXT_CR_CMD="Serveur passerelle inaccessible"
fi



# Si le transfert sur le serveur passerelle s'est bien deroule alors
#  commander sur ce serveur le transfert du document au destinataire
# ------------------------------------------------------------------
if [[ $CR_CMD -eq 0 ]]; then
	Txt_Err_Transfert=$(xzsx_cmd_sur.sh "${SERVEUR_PROTO}"  ". ./.profile >&- 2>&- ; titransfert_stat_radt.sh /tmp/${NOM_FICH_STAT} >/dev/null" 2>&1)
	CR_CMD=$?
	TXT_CR_CMD=$(echo $Txt_Err_Transfert | head -1)
fi



# Retourner le compte-rendu a l'appelant
# --------------------------------------
if [[ "$TXT_CR_CMD" != "" ]]; then
	echo "$TXT_CR_CMD"  >&2
fi

exit $CR_CMD
