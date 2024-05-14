#!/bin/ksh
# Fichier : @(#)xzag07.ksh	1.13      Release : 1.13        Date : 10/05/98
#################################################################################
#
# STERIA # PROJET MIGRAZUR
#################################################################################
#
# SOUS-SYSTEME BASEDD
#################################################################################
#
# SCRIPT xzag07.ksh
#################################################################################
#
# DESCRIPTION DU SCRIPT :
#
# Dechencher la sauvegarde ou la restauration
#
#################################################################################
#
# HISTORIQUE DES MODIFICATIONS :
# C.T.  04/11/94        : Creation
# C.T.  28/11/94        : Modif du nom du fichier temporaire de travail (V 1.2)
#                         non reteste
# C.T.  05/12/94        : Modif de l'emplacement du fichier (V 1.1)
# C.T.  20/12/94        : Rajout cas d'erreur sql server interrompant sauvegarde
#                         (V 1.2)
# C.T.  09/01/95        : Positionner le nombre de jours d'interdiction de 
#                         surcharger les fichiers de sauvegarde sur bande (V 1.3)
# C.T.  24/04/95        : Ajout de la possibilite de sauvegarder PRC (V 1.4)
# C.T.  18/12/95        : Modif format date affichee (V 1.5)
# C.T.  15/03/96        : Ajout option iso_1 a isql pour les accents (1030) (V1.7)
# C.T.  19/04/96        : Correction test d'erreur declenchant l'abort de la session 
#                         de restauration (984,1053) (V1.8)
# C.T.  14/05/96        : Correction test d'erreur declenchant l'abort de la session 
#                         de restauration (984,1053) (V1.9)
# C.T.  26/09/96        : Modif mot de passe sa (V1.10)
# P.N.  29/01/98	: Ajout DA DY DP 1.11
# P.N.  05/10/98	: Separation EXP/HIS dem/1696 1.13
# P.F	31/03/05	: Pas le bon environnement je lance le .profile au debut
# 			: pour avoir le bon environnement 1.14
#################################################################################
#

#
#################################################################################
#
# Service rendu
# Dechencher la sauvegarde automatique d'une base
#
# Sequence d'appel
# xzag07.ksh Site Device Volume Mode Base FicTrace 
#                    AjoutCassette Capacite FicTape
#
# Arguments en entree
# Site          : CI ou DN ou DM
# Device        : nom du device a utiliser
# Volume        : nom de 6 caracteres maximum pour nommee la sauvegarde
# Mode          : sauvegarde de la base "globale" ou "incrementale"
# Base          : SVG ou HIS ou EXP ou CFG ou master ou model ou RSSD
#                 tempdb ou sybsystemprocs ou PRC
# FicTrace      : fichier de trace pour l'administrateur
# AjoutCassette : 
#           init       : ajout en debut de bande en overwrite, 
#                        avec ejection de la bande en fin de svg
#	    ajoutdebut : ajout en debut de bande en overwrite, 
#                        sans ejection de la bande en fin de svg
#           ajout      : ajout a la fin de la bande, sans ejection 
#                        de la bande en fin de svg
#	    ajoutfin   : ajout en fin de bande, 
#                        avec ejection de la bande en fin de svg
# Capacite     : capacite de la bande
# FicTape      : fichier sur la bande ( optionnel : 15 caracteres min et 17 max)
#
# Code retour
# 0             : sauvegarde ok
# 1             : sauvegarde arretee par l'administrateur
# 2             : sauvegarde arretee suite a une erreur grave 
#                 (aucune autre sauvegarde possible)
# 3             : sauvegarde arretee suite a une erreur
#
# Conditions d'utilisation
# Si le nom du fichier sur la bande n'est pas renseigne, la sauvegarde
# se fait avec le nom attribue par sybase
# Si le nom du fichier sur la bande est renseigne, il doit etre de 15 caracteres
# sinon la restauration ne sera jamais realisee avec ce nom de fichier (erreur)
# Pas de truncate possible sur dump database et sur un dump transaction, il faut par
# defaut
# Le nom du fichier de travail utilise est "sauvegarde" concatene au nom de la base sauvegardee
# avec comme extension "tmp" (car 8 arguments au maximum pour un cript ksh : limite du ksh)
#
#################################################################################
#
. /produits/migrazur/appliSD/.profile

#########################
# sauvegarde d'une base
#########################

# controle de la syntaxe d'appel
################################
if ((($#<8)) || [[ $1 != "CI" && $1 != "DN" && $1 != "DM" && $1 != "DC" && $1 != "DS" && $1 != "DA" && $1 != "DP" && $1 != "DY" && $1 != "HIS" ]] || \
    [[ $4 != incrementale && $4 != globale ]]  || \
    [[ $5 != SVG && $5 != HIS && $5 != CFG && $5 != EXP && $5 != PRC && $5 != master && \
       $5 != model && $5 != tempdb && $5 != sybsystemprocs && $5 != RSSD ]] || \
    [[ $7 != ajoutdebut && $7 != ajout && $7 != init && $7 != ajoutfin ]] )
then    
	echo "Usage : xzag07.ksh Site Device Volume Mode Base FicTrace AjoutCassette Capacite FicTape"
	echo "        Site       : CI ou DN ou DM ou DC ou DS ou DA ou DP ou DY"
	echo "        Device     : nom du device a utiliser"
	echo "        Volume     : nom de 6 caracteres maximum pour nommee la sauvegarde"
	echo "        Mode       : sauvegarde de la base globale ou incrementale"
	echo "        Base       : SVG ou HIS ou CFG ou master ou EXP ou model ou "
	echo "                     tempdb ou sybsystemprocs ou RSSD ou PRC"
	echo "        FicTrace   : fichier de trace pour l'administrateur"
	echo "        AjoutCassette : "
	echo "                  init       : ajout en debut de bande en overwrite, "
	echo "                               avec ejection de la bande en fin de svg"
	echo "                  ajoutdebut : ajout en debut de bande en overwrite, "
	echo "                               sans ejection de la bande en fin de svg"
	echo "                  ajout      : ajout a la fin de la bande, sans ejection "
	echo "                               de la bande en fin de svg"
	echo "                  ajoutfin   : ajout en fin de bande, "
	echo "                               avec ejection de la bande en fin de svg"
	echo "        Capacite   : capacite de la bande"
	echo "        FicTape    : nom du fichier sur la bande (optionnel : 15 caracteres)"
	exit 1
fi
echo "DANS SAUVEGARDE HIS " > /tmp/HIST
echo $1 >> /tmp/HIST 
echo $2 >> /tmp/HIST 
echo $3 >> /tmp/HIST 
echo $4 >> /tmp/HIST 
echo $5 >> /tmp/HIST 
echo $6 >> /tmp/HIST 
echo $7 >> /tmp/HIST 
echo $8 >> /tmp/HIST 

# site de sauvegarde
####################
Site=$1

# nom du device
###############
Device=$2

# nom du volume de sauvegarde
#############################
Volume=$3

# incrementale ou globale
#########################
Incremental=$4

# base a sauvegarder : HIS ou CFG ou SVG ou master ou RSSD
##########################################################
Base=$5
if [[ $Base = RSSD ]]
then
	# le nom de la base est CI_RSSD ou DN_RSSD ou DM_RSSD
	Base=$1_$5
fi

# Fichier de trace de la sauvegarde
###################################
Trace=$6

# Fichier de trace de la sauvegarde
###################################
TraceTravail="/tmp/sauvegarde${Base}.tmp"

# Ajout de sauvegarde a un volume
#################################
Ajout=$7
if test $Ajout = "ajout"
then
	TapeInit="noinit, "
	TapeLoad=""
elif test $Ajout = "ajoutfin"
then
	TapeInit="noinit, "
	TapeLoad="unload, "
elif test $Ajout = "ajoutdebut"
then
	TapeInit="init, "
	TapeLoad=""
else
	TapeInit="init, "
	TapeLoad="unload, "
fi

# Capacite de sauvegarde
########################
Capacity=$8

# Fichier sur la bande
######################
if test $# = 9
then
	FicTape=", file=\"${9}\""
else
	FicTape=""
fi

# Nombre de jours de sauvegarde de bande
########################################
if [[ $Base = CFG || $Incremental = incrementale ]] then
	Retaindays=365
else	Retaindays=20000
fi

# commande de sauvegarde
########################
if test $Incremental = "incrementale"
then
	Commande="dump tran $Base to \"$Device\" capacity=$Capacity, \
	dumpvolume=\"$Volume\" $FicTape with $TapeLoad retaindays=$Retaindays, $TapeInit notify=client"
else
	Commande="dump database $Base to \"$Device\" capacity=$Capacity, \
	dumpvolume=\"$Volume\" $FicTape with $TapeLoad retaindays=$Retaindays, $TapeInit notify=client"
fi

# modifier les droits du fichier de travail s'il existe
#######################################################
if test -f $TraceTravail
then
   chmod 666 $TraceTravail
fi
echo "Commande : "$Commande>> /tmp/HIST 

# declencher la sauvegarde 
##########################
if [[ $5 = SVG || $5 = HIS || $5 = CFG || $5 = EXP || $5 = PRC ]]
then
# les bases utilisateur (ne pas mettre de tabulation 
# devant la commande suivante)
echo "$Commande \n\
go\n\
print '...fin'\n\
go\n" \
| isql -SSQL_$Site -UMIGRAZUR -PMIGRAZUR -Jiso_1 > $TraceTravail &
else
# les bases systemes et de replication (ne pas mettre de tabulation 
# devant la commande suivante)
echo "$Commande \n\
go\n\
print '...fin'\n\
go\n" \
| isql -SSQL_$Site -Usa -Pzigramur -Jiso_1 > $TraceTravail &
fi

# attente de 30s 
sleep 30

exec 4>>$Trace

# si le fichier de trace de la sauvegarde n'existe pas alors probleme
#####################################################################
if test ! -w $TraceTravail
then
	echo "probleme pour declencher la sauvegarde"
	Date=`date  '+%d/%m/%y %H:%M:%S'`
	Message="$Date  Probleme pour declencher la sauvegarde de la base $Base"

	exit 2
fi

# lier le fichier a un descripteur
##################################
exec 3<$TraceTravail

# lecture des traces de isql
############################
Erreur=0
ligne=""
SauvegardeFinie="Non"
while read -u3 ligne || [[ $ligne != "...fin" ]]
do
	sleep 1

	if [[ "$ligne" != "...fin" && "$ligne" != "" ]]
	then
		# trace a l'ecran
		echo $ligne

		# numero d'erreur ou de message
		numero=`echo $ligne | sed -n "s/.*\([1-7]\.[1-9]*\.[1-4]\)\.[0-9].*/\1/p"`

		# severite d'erreur ou de message
		severite=`echo $numero | sed -n "s/.*\([1-4]\)$/\1/p"`

		# arret de la sauvegarde si une erreur de severite superieure a 1
		if test 0$severite -gt "1"
		then
			Message="$Date  Arret de la sauvegarde de la base $Base suite a l'erreur $numero"
			# ecrire dans le fichier de trace
			Date=`date '+%d/%m/%y %H:%M:%S'`
			print -u4 "$Date  $ligne"
			Erreur=3
		else

			case $numero in
				6.53.1) 
					# cette erreur ne doit pas arriver
					print "Le volume de sauvegarde est protegee en ecriture"
					Message="$Date  Arret de la sauvegarde de la base $Base suite a l'erreur $numero"
					Erreur=2;;
	
				6.47.1) 
					# l'operateur doit monter le volume suivant en mode Åcriture
	
					# ecrire dans le fichier de trace 
					Date=`date '+%d/%m/%y %H:%M:%S'`
					print -u4 "$Date  $ligne"
	
					# attente de la reponse de l'operateur
					print "Veillez a changer la bande du lecteur "
					print "Le changement de bande a-t-il ete realise (sinon la sauvegarde est interrompue) ? (OUI/NON) : "
					Reponse=""
					while [[ "$Reponse" != OUI && "$Reponse" != oui && \
						 "$Reponse" != NON && "$Reponse" != non ]]
					do
						read -ru Reponse
					done
	
					if [[ $Reponse = OUI || $Reponse = oui ]]
					then
						BandeChangee=1
					else
						BandeChangee=0
					fi

					Reponse="";;
	
				6.78.1)	
					# execution de la commande sp_volchanged
	
					# recherche du numero de session de sauvegarde
					NumSession=`grep "session_id" $TraceTravail | sed -n "1s/.* \([0-9][0-9]*\).*/\1/p"`
					if [[ X$NumSession = X ]]
					then
						Message="$Date  Changement automatique de bande impossible (numero de session non trouve)"
						Erreur=3
					fi
	
					# Prise en compte de l'action de l'operateur
					if (( $BandeChangee != 0 ))
					then
						# continuer la sauvegarde
	
						# tracer l'action de l'operateur
						print -u4 "$Date  Changement de la bande du lecteur effectue"
	
						Commande="sp_volchanged $NumSession, \"$Device\",\"PROCEED\""
echo "$Commande \n\
go\n" \
| isql -SSQL_$Site -UMIGRAZUR -PMIGRAZUR
					else
						# arreter la sauvegarde
	
						print -u4 "$Date  Arret de la sauvegarde demande"
	
						Commande="sp_volchanged $NumSession, \"$Device\", \"ABORT\""
echo "$Commande \n\
go\n" \
|isql -SSQL_$Site -UMIGRAZUR -PMIGRAZUR
	
						Erreur=1
					fi
	
					# ecrire dans le fichier de trace 
					Date=`date '+%d/%m/%y %H:%M:%S'`
					print -u4 "$Date  $ligne" ;;
	
				4.22.1)	
					# peripherique unload
	
					# ecrire dans le fichier de trace 
					Date=`date '+%d/%m/%y %H:%M:%S'`
					print -u4 "$Date  $ligne"

					Message="$Date  Arret de la sauvegarde de la base $Base suite a l'erreur $numero"
					Erreur=2;;
	
				3.42.1)	
					# la sauvegarde est terminee
	
					# ecrire dans le fichier de trace 
					Date=`date '+%d/%m/%y %H:%M:%S'`
					print -u4 "$Date  $ligne"
	
					SauvegardeFinie="Oui"
					break;;
	
				6.45.1)	
					# Veillez È retirer la bande/disquette
					Date=`date '+%d/%m/%y %H:%M:%S'`
					print -u4 "$Date  $ligne" ;;
	
				3.54.1|3.48.1)
					# Shutdown durant la sauvegarde
	
					Message="$Date  Arret de la sauvegarde de la base $Base suite a l'erreur $numero"
					# ecrire dans le fichier de trace 
					Date=`date '+%d/%m/%y %H:%M:%S'`
					print -u4 "$Date  $ligne"
	
					Erreur=2;;
	
				*)	# ecrire dans le fichier de trace
					Date=`date '+%d/%m/%y %H:%M:%S'`
					if test "$numero" != ""
					then
						print -u4 "$Date  $ligne"
					else
						print -u4 "                   $ligne"
					fi;;
			esac
		fi

		if [[ $Erreur != 0 ]]
		then break
		fi

	elif [[ "$ligne" = "...fin" ]]
	then
		break
	else
		print ".\c"
	fi
done

# copier la suite des messages
Date=`date '+%d/%m/%y %H:%M:%S'`
while read -u3 ligne && test "$ligne" != "...fin"
do
	numero=`echo $ligne | sed -n "s/.*\([1-7]\.[1-9]*\.[1-4]\)\.[0-9].*/\1/p"`

	# ecrire dans le fichier de trace
	if test "$numero" = ""
	then
		print -u4 "                   $ligne"
	else
		print -u4 "$Date  $ligne"
	fi

	# afficher a l'ecran
	echo $ligne
done

# ecriture de la cause de l'arret de la sauvegarde
if [[ "$Message" != "" ]]
then
	print $Message
	print -u4 $Message
fi

# arreter la sauvegarde sous isql si elle est en attente
if test $Erreur -gt 1
then
	# tester si le processus isql lance en background a terminer
	Fin=`grep -c "...fin" $TraceTravail`

	# aborter la sauvegarde
	if [[ $Fin = 0 ]]
	then
		# recherche du numero de session de sauvegarde
		NumSession=`grep "identificateur de session du serveur" $TraceTravail | sed -n "1s/.* \([0-9][0-9]*\).*/\1/p"`

		if [[ X$NumSession = X ]]
		then
			print -u4 "$Date  Sauvegarde en attente, elle doit etre supprimee manuellement"
			Erreur=2
		else
			# arreter la demande de sauvegarde
			Commande="sp_volchanged $NumSession, \"$Device\", \"ABORT\""
echo "$Commande \n\
go\n" \
|isql -SSQL_$Site -UMIGRAZUR -PMIGRAZUR
		fi
	fi
fi


# Cas ou l'erreur vient de sql et pas d'erreur du backup
# puiqu'il n'a pas ete declenche
########################################################
if [[ $SauvegardeFinie = "Non" ]]
then Erreur=2
fi

# fermer les descripteurs
exec 3<&-
exec 4>&-

# supprimer du fichier de travail
rm $TraceTravail

exit $Erreur
