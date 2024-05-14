:
########################################################################################################
# Fichier : $Id: xzsx_cmd_sur.sh,v 1.1 2012/02/27 14:46:18 gesconf Exp $      Release : $Revision: 1.1 $        Date : $Date: 2012/02/27 14:46:18 $
########################################################################################################
#
# ESCOTA # PROJET MIGRAZUR
########################################################################################################
#
# SCRIPT xzsx_cmd_sur.sh
########################################################################################################
#
# DESCRIPTION DU SCRIPT	:
#
#  Execute une commande quelconque sur une machine distante.
#
# CODE DE RETOUR	:
#
#  0 ou 1 selon que la commande distante a emis du texte sur son canal d'erreur.
#
# REMARQUES		:
#
#  . Un code de retour, non transmis par les commandes de type "remote shell",
#    est fourni en fonction du contenu de la sortie d'erreur.
#
#  . Les productions de la commande sur ses canaux de sortie et d'erreur
#    sont preservees et sont redirigeables.
#
#  . Le script fonctionne aussi bien sous Linux que sous HP-UX.
#
########################################################################################################
#
# HISTORIQUE DES MODIFICATIONS :
#
# JPL	22/02/12 : Creation
#
########################################################################################################



########################################################################################################
##
##  FONCTION     : Machine_Accessible
##
##  ARGUMENTS    :
##    $1 : Nom de la machine a acceder.
##
##  COMPTE RENDU :
##    Vrai (0) ou faux (non nul) selon que la machine est ou non accessible.
##
########################################################################################################

Machine_Accessible()
{
	machine=$1

	# Envoyer UNE sonde a la machine cible
	if [ $(uname) = "HP-UX" ]; then
		ping "$machine" -n 1 -m 5  >&-  2>&-
	else
		ping "$machine" -c 1 -w 5  >&-  2>&-
	fi
}



########################################################################################################
##
##                                           Debut du script
##
########################################################################################################

Nom_Programme=`basename $0`

if [ $(uname) = "HP-UX" ]; then
	RCMD=remsh
else
	RCMD=rsh
fi


PID_CMD=$$

LOG_COMMANDE="/tmp/$Nom_Programme.log"
LOG_OUTPUT="/tmp/$Nom_Programme.out"
LOG_ERROR="/tmp/$Nom_Programme.err"


# Verifier la validite des parametres obligatoires
# ------------------------------------------------

Machine="$1"
Commande="$2"

if [ "$Machine" = "" ]; then
	echo "Usage : $Nom_Programme <Machine> <Commande>"  >&2
	echo "   ou : echo <Commande> | $Nom_Programme <Machine>"  >&2
	exit 2
fi



# Verifier l'accessibilite de la machine distante
# -----------------------------------------------

Machine_Accessible "$Machine" ||  {
	echo "$Nom_Programme : Machine '$Machine' inaccessible"  >&2
	exit 1
}



# Executer la commande sur la machine distante
# --------------------------------------------

if [ "$Commande" != "" ]; then
	#echo "$RCMD $Machine '$Commande'"  >$LOG_COMMANDE
	$RCMD "$Machine" "$Commande"  <&-  2>$LOG_ERROR
else
	$RCMD "$Machine" ksh  2>$LOG_ERROR
fi



# Definir le code de retour selon la production de texte d'erreur
# ---------------------------------------------------------------

Code_Retour=0
[ -s "$LOG_ERROR" ]  &&  Code_Retour=1


cat $LOG_ERROR >&2
rm $LOG_ERROR

exit $Code_Retour
