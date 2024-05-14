#!/bin/ksh
# Fichier : @(#)xzag08.ksh	1.9      Release : 1.9        Date : 10/05/98
#################################################################################
#
# STERIA # PROJET MIGRAZUR
#################################################################################
#
# SOUS-SYSTEME BASEDD
#################################################################################
#
# SCRIPT xzag08.ksh
#################################################################################
#
# DESCRIPTION DU SCRIPT :
#
# Dechencher la sauvegarde ou la restauration
#
#################################################################################
#
# HISTORIQUE DES MODIFICATIONS :
# C.T.  07/11/94        : Creation
# C.T.  28/11/94        : Modif plus de nom de repertoire pour lancer le sous script
#                         V 1.3 (non reteste)
# C.T.  05/12/94        : Modif de l'emplacement du fichier (V 1.1)
# C.T.  20/12/94        : Correction du code de retour (V 1.2)
# C.T.  09/01/95        : Modification des options de sauvegarde (V 1.3)
#                         possibilite de sauvegarder que les tables systeme 
#                         et possibilite de sauvegarder CFG et histo sur la meme
#                         cassette (CFG en globale et histo en incrementale)
# C.T.  12/09/95        : Ajout compte rendu dans le fichier de trace (V 1.4)
# C.T.  08/02/96        : Modif format date affichee (V 1.5)
# C.T.  16/04/96        : Modif type de sauvegarde hebdomadaire pour HIS (en globale) (V 1.6)
# P.N.	07/03/97	: Modif de la capacite pour demande chgt de bande (V1.7)
# P.N.  05/10/98	: Separation EXP/HIS dem/1696 V1.9
# P.F   31/03/05        : Pas le bon environnement je lance le .profile au debut
#                       : pour avoir le bon environnement V1.10
#################################################################################
#

#
#################################################################################
#
# Service rendu
# Dechencher la sauvegarde automatique d'une base
#
# Sequence d'appel
# xzag08.ksh Site Device Volume Mode Base FicTrace Capacite FicTape
#
# Arguments en entree
# Site          : CI ou DN ou DM
# Device        : nom du device a utiliser
# Volume        : nom de 6 caracteres maximum pour nommee la sauvegarde
# Mode          : sauvegarde de la base "globale" ou "incrementale"
# Base          : SVG ou HIS ou CFG ou systeme ou hebdo (CFG + HIS)
# FicTrace      : fichier de trace pour l'administrateur
# Capacite      : capacite de la bande
# FicTape       : fichier sur la bande ( optionnel : 15 caracteres min et 17 max)
#
# Code retour
# 0             : sauvegarde ok
# 1             : sauvegarde arretee suite a une erreur
#
# Conditions d'utilisation
# Si le nom du fichier sur la bande n'est pas renseigne, la sauvegarde
# se fait avec le nom attribue par sybase
# Si le nom du fichier sur la bande est renseigne, il doit etre de 15 caracteres
# sinon la restauration ne sera jamais realisee avec ce nom de fichier (erreur)
# Dans le cas ou la sauvegarde de toutes les bases systeme du site est demandee :
#  - master, model, tempdb, sybsystemprocs : bases systeme
#  - CI_RSSD ou DN_RSSD ou DM_RSSD         : base pour la replication
# le nom du fichier est ignore (a implementer)
#
#################################################################################
#

. /produits/migrazur/appliSD/.profile

# controle de la syntaxe d'appel
################################
if ((($#<7)) || [[ $1 != "CI" && $1 != "DN" && $1 != "DM" && $1 != "HIS" ]] || \
    [[ $4 != systeme && $4 != hebdo && $4 != HIS && $4 != CFG && $4 != SVG ]] ||
    [[ $7 != incrementale && $7 != globale ]] )
then
	echo "Usage : xzag08.ksh Site Device Volume Base Trace Capacite Mode FicTape"
	echo "        Site           : CI ou DN ou DM"
	echo "        Device         : nom du device"
	echo "        Volume         : nom du volume"
	echo "        Base           : systeme ou HIS ou CFG ou SVG ou hebdo"
	echo "        Trace          : nom du fichier contenant la trace "
	echo "        Capacite       : capacite de la cassette "
	echo "        Mode           : incrementale ou globale"
	echo "        FicTape        : fichier a lire sur la bande (optionnel)"
	exit 1
fi

echo "je suis dans xzag08 " > /tmp/HIST
echo $1 >> /tmp/HIST
echo $2 >> /tmp/HIST
echo $3 >> /tmp/HIST
echo $4 >> /tmp/HIST
echo $5 >> /tmp/HIST
echo $6 >> /tmp/HIST
echo $7 >> /tmp/HIST


# site de sauvegarde
####################
Site=$1

# nom du device
###############
Device=$2

# nom du volume de sauvegarde
#############################
Volume=$3

# base(s) a sauvegarde : HIS ou CFG ou SVG ou toutes les bases
##############################################################
Base=$4

# Fichier de trace de la sauvegarde
###################################
Trace=$5

# Capacite de la cassette
#########################
Capacite=$6

# type de la sauvegarde
#######################
Mode=$7

# fichier sur la bande
######################
if test $# = 8
then
	FicTape=$8
else
	FicTape=""
fi

# dechencher la sauvegarde demandee
###################################
if [[ $Base = systeme ]]
then
	# les bases systeme
	Date=`date '+%d/%m/%y %H:%M:%S'`
	echo "$Date  Sauvegarde de la base master" >> $Trace
	xzag07.ksh $Site $Device $Volume globale master $Trace ajoutdebut $Capacite ${FicTape}1
	if (($? == 2))
	then
		Date=`date '+%d/%m/%y %H:%M:%S'`
		echo "$Date  Arret de la sauvegarde globale" >> $Trace
		exit 1
	else
		Date=`date '+%d/%m/%y %H:%M:%S'`
		echo "$Date  Fin de la sauvegarde de la base master" >> $Trace
		echo "" >> $Trace
	fi
	Date=`date '+%d/%m/%y %H:%M:%S'`
	echo "$Date  Sauvegarde de la base model" >> $Trace
	xzag07.ksh $Site $Device $Volume globale model $Trace ajout $Capacite ${FicTape}2
	if (($? == 2))
	then
		Date=`date '+%d/%m/%y %H:%M:%S'`
		echo "$Date  Arret de la sauvegarde globale" >> $Trace
		exit 1
	else
		Date=`date '+%d/%m/%y %H:%M:%S'`
		echo "$Date  Fin de la sauvegarde de la base model" >> $Trace
		echo "" >> $Trace
	fi
	Date=`date '+%d/%m/%y %H:%M:%S'`
	echo "$Date  Sauvegarde de la base tempdb" >> $Trace
	xzag07.ksh $Site $Device $Volume globale tempdb $Trace ajout $Capacite ${FicTape}3
	if (($? == 2))
	then
		Date=`date '+%d/%m/%y %H:%M:%S'`
		echo "$Date  Arret de la sauvegarde globale" >> $Trace
		exit 1
	else
		Date=`date '+%d/%m/%y %H:%M:%S'`
		echo "$Date  Fin de la sauvegarde de la base tempdb" >> $Trace
		echo "" >> $Trace
	fi
	Date=`date`
	echo "$Date  Sauvegarde de la base sybsystemprocs" >> $Trace
	xzag07.ksh $Site $Device $Volume globale sybsystemprocs $Trace ajout $Capacite ${FicTape}4
	if (($? == 2))
	then
		Date=`date '+%d/%m/%y %H:%M:%S'`
		echo "$Date  Arret de la sauvegarde globale" >> $Trace
		exit 1
	else
		Date=`date '+%d/%m/%y %H:%M:%S'`
		echo "$Date  Fin de la sauvegarde de la base sybsystemprocs" >> $Trace
		echo "" >> $Trace
	fi
	echo "$Date  Fin de la sauvegarde de la base sybsystemprocs" >> $Trace
	echo "" >> $Trace

	# base de la replication
	Date=`date '+%d/%m/%y %H:%M:%S'`
	echo "$Date  Sauvegarde de la base RSSD" >> $Trace
	xzag07.ksh $Site $Device $Volume globale RSSD $Trace ajoutfin $Capacite ${FicTape}5
	if (($? == 2))
	then
		Date=`date '+%d/%m/%y %H:%M:%S'`
		echo "$Date  Arret de la sauvegarde globale" >> $Trace
		exit 1
	else
		Date=`date '+%d/%m/%y %H:%M:%S'`
		echo "$Date  Fin de la sauvegarde de la base RSSD" >> $Trace
		echo "" >> $Trace
		echo "Sauvegarde reussie" >> $Trace
	fi
elif [[ $Base = CFG ]]
then
	Date=`date '+%d/%m/%y %H:%M:%S'`
	echo "$Date  Sauvegarde de la base CFG" > $Trace
	xzag07.ksh $Site $Device $Volume globale CFG $Trace init $Capacite $FicTape
	if (($? != 0))
	then
		exit 1
	fi
	Date=`date '+%d/%m/%y %H:%M:%S'`
	echo "$Date  Fin de la sauvegarde de la base CFG\n" >> $Trace
	echo "" >> $Trace
	echo "Sauvegarde reussie" >> $Trace
elif [[ $Base = HIS ]]
then
	Date=`date '+%d/%m/%y %H:%M:%S'`
	echo "$Date  Sauvegarde de la base HIS" >> $Trace
	xzag07.ksh $Site $Device $Volume $Mode HIS $Trace init $Capacite $FicTape
	if (($? != 0)) 
	then
		exit 1
	fi
	Date=`date '+%d/%m/%y %H:%M:%S'`
	echo "$Date  Fin de la sauvegarde de la base HIS\n" >> $Trace
	echo "" >> $Trace
	echo "Sauvegarde reussie" >> $Trace
elif [[ $Base = hebdo ]]
then
	# base CFG
	Date=`date '+%d/%m/%y %H:%M:%S'`
	echo "$Date  Sauvegarde de la base CFG" > $Trace
	xzag07.ksh HIS $Device $Volume globale CFG $Trace ajoutdebut $Capacite ${FicTape}1
	if (($? == 2))
	then
		Date=`date '+%d/%m/%y %H:%M:%S'`
		echo "$Date  Arret de la sauvegarde hebdomadaire" >> $Trace
		read -ru Reponse
		exit 1
	else
		Date=`date '+%d/%m/%y %H:%M:%S'`
		echo "$Date  Fin de la sauvegarde de la base CFG" >> $Trace
	fi
	echo "" >> $Trace


	# base HIS pour le HIS
	if [[ $Site = HIS ]]
	then
		Date=`date '+%d/%m/%y %H:%M:%S'`
		echo "$Date  Sauvegarde de la base HIS" >> $Trace
	echo "C'est la con passe a mon avis " > /tmp/HIST

	xzag07.ksh HIS $Device $Volume globale HIS $Trace ajoutfin $Capacite ${FicTape}2
		if (($? == 2))
		then
			Date=`date '+%d/%m/%y %H:%M:%S'`
			echo "$Date  Arret de la sauvegarde hebdomadaire" >> $Trace
			read -ru Reponse
			exit 1
		else
			Date=`date '+%d/%m/%y %H:%M:%S'`
			echo "$Date  Fin de la sauvegarde de la base HIS" >> $Trace
			echo "" >> $Trace
			echo "Sauvegarde reussie" >> $Trace
			read -ru Reponse
		fi
	fi

else
	Date=`date '+%d/%m/%y %H:%M:%S'`
	echo "$Date  Sauvegarde de la base SVG" >> $Trace
	xzag07.ksh $Site $Device $Volume globale SVG $Trace init $Capacite $FicTape
	if (($? != 0)) 
	then
		exit 1
	fi
	Date=`date '+%d/%m/%y %H:%M:%S'`
	echo "$Date  Fin de la sauvegarde de la base SVG\n" >> $Trace
	echo "" >> $Trace
	echo "Sauvegarde reussie" >> $Trace
fi

exit 0
