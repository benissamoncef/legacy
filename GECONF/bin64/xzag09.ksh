#!/bin/ksh
# Fichier : @(#)xzag09.ksh	1.13      Release : 1.13        Date : 02/19/98
#################################################################################
#
# STERIA # PROJET MIGRAZUR
#################################################################################
#
# SOUS-SYSTEME BASEDD
#################################################################################
#
# SCRIPT xzag09.ksh
#################################################################################
#
# DESCRIPTION DU MODULE :
#
# Dechencher la restauration ou la sauvegarde
#
#################################################################################
#
# HISTORIQUE DES MODIFICATIONS :
# C.T.  09/11/94        : Creation (la restauration est refusee par sybase avec une 
#                         sauvegarde sur plusieurs bandes !) V 1.1
# C.T.  14/11/94        : Modif commentaire d'erreur de syntaxe V 1.2
# C.T.  05/12/94        : Modif de l'emplacement du fichier (V 1.1)
# C.T.  20/12/94        : Rajout cas d'erreur sql server interrompant 
#                         restauration (V 1.2)
# C.T.  16/01/95        : Rajout cas d'erreur pour fichier de trace non accessible
#			  (V 1.3)
# C.T.  23/02/95        : Ajout de la suppression des triggers dans CFT et
#                         du tronquage sur ltm positionner a non (V 1.4)
# C.T.  12/09/95        : Ajout compte rendu chargement (V 1.5)
# C.T.  08/02/96        : Modif format date affichee (V 1.6)
# C.T.  15/03/96        : Ajout option iso_1 a isql pour les accents (1030) (V1.8)
# C.T.  15/04/96        : Ajout option iso_1 a isql pour les accents (1030) (V1.9)
#                         et correction test d'erreur declenchant l'abort de la session 
#                         de restauration  (984,1053)
# C.T.  14/05/96        : correction test d'erreur declenchant l'abort de la session 
#                         de restauration  (984,1053) (V1.10)
# C.T.  11/09/96        : Ajout restaurer sur pc DC et DS (V1.11)
# P.N.  29/01/98	: Ajout DA DY DP 1.12
# C.T.  18/02/98        : Ajout CFT on line suite a SQL 11.0.3.2 
# P.F   31/03/05        : Pas le bon environnement je lance le .profile au debut
#                       : pour avoir le bon environnement V1.14
#################################################################################
#

#
#################################################################################
#
# Service rendu
# Dechencher la restauration automatique globale de la base SVG ou CFT
#
# Sequence d'appel
# xzag09.ksh Site Device Volume Base FicTrace FicTravail FicTape
#
# Arguments en entree
# Site          : CI ou DN ou DM ou DC ou DS
# Device        : nom du device a utiliser
# Volume        : nom de 6 caracteres maximum pour nommee la sauvegarde
# FicTrace      : fichier de trace pour l'administrateur
# FicTravail    : fichier de travail temporaire
# FicTape       : fichier sur la bande ( optionnel : 15 a 17 caracteres )
#
# Code retour
# 0             : restauration ok
# 1             : restauration arretee par l'administrateur
# 2             : restauration arretee suite a une erreur
#
# Conditions d'utilisation
# Si le nom du fichier sur la bande n'est pas renseigne, la restauration
# se fait avec l'option listonly
# Si le nom du fichier sur la bande est renseigne, il doit etre de 15 caracteres
# minimum a 17 maximum sinon la restauration n'est pas realisee (erreur)
#
#
#################################################################################
#

. /produits/migrazur/appliSD/.profile

#########################
# restauration d'une base
#########################

# controle de la syntaxe d'appel
################################
if ((($#<6)) || [[ $1 != "CI" && $1 != "DN" && $1 != "DM" && $1 != "DC" && $1 != "DS" && $1 != "DA" && $1 != "DP" && $1 != "DY" ]] || \
    [[ $4 != SVG && $4 != CFT ]])
then    
	echo "Usage : xzag09.ksh Site Device Volume Base FicTrace FicTravail FicTape"
	echo "        Site       : CI ou DN ou DM ou DC ou DS ou DA ou DP ou DY"
	echo "        Device     : nom du device a utiliser"
	echo "        Volume     : nom de 6 caracteres maximum pour nommee la sauvegarde"
	echo "        FicTrace   : fichier de trace pour l'administrateur"
	echo "        FicTravail : fichier de travail temporaire"
	echo "        FicTape    : nom du fichier sur la bande (optionnel : 15 a 17 caracteres)"
	exit 1
fi

# site de restauration
######################
Site=$1

# nom du device
###############
Device=$2

# nom du volume de restauration
###############################
Volume=$3

# base a restaurer : SVG ou CFT
###############################
Base=$4

# Fichier de trace de la restauration
#####################################
Trace=$5

# Fichier de travail de la restauration
#######################################
TraceTravail=$6

# Fichier sur la bande
######################
if test $# = 7
then
	FicTape=", file=\"$7\""
	ListOnly=""
else
	FicTape=""
	ListOnly="listonly, "
fi

# commande de restauration
##########################
Commande="load database $Base from \"$Device\" \
dumpvolume=\"$Volume\" $FicTape with unload, $ListOnly notify=client"

# dans le cas de la restauration de CFT, indiquer qu'il n'a pas de ltm
# sur CFT pour le tronquage du log
if [[ $Base = CFT ]]
then
	CommandeSuite="online database CFT \n\
go\n\
use CFT \n\
go\n\
drop trigger res_air_u_trg \n\
go\n\
drop trigger res_aut_u_trg \n\
go\n\
drop trigger res_cha_u_trg \n\
go\n\
drop trigger res_dis_u_trg \n\
go\n\
drop trigger res_ech_u_trg \n\
go\n\
drop trigger res_inf_u_trg \n\
go\n\
drop trigger res_itp_u_trg \n\
go\n\
drop trigger res_neu_u_trg \n\
go\n\
drop trigger res_pea_u_trg \n\
go\n\
drop trigger res_pnt_u_trg \n\
go\n\
drop trigger res_pol_u_trg \n\
go\n\
drop trigger res_por_u_trg \n\
go\n\
drop trigger res_ptl_u_trg \n\
go\n\
drop trigger res_seg_u_trg \n\
go\n\
drop trigger res_sup_u_trg \n\
go\n\
drop trigger res_trc_u_trg \n\
go\n\
drop trigger res_trn_u_trg \n\
go\n\
drop trigger res_vil_u_trg \n\
go\n\
dbcc settrunc ('ltm','ignore')\n\
go\n\"
else
	CommandeSuite="\n\"
fi

# modifier les droits du fichier de travail s'il existe
#######################################################
if test -f $TraceTravail
then
   chmod 666 $TraceTravail
fi

# declencher la restauration 
############################
echo "$Commande \n\
go\n\
print '...fin'\n\
go\n\
$CommandeSuite" \
| isql -SSQL_$Site -UMIGRAZUR -PMIGRAZUR -Jiso_1 > $TraceTravail &

# attente de 30s 
sleep 30

# Ecrire dans le fichier de trace une entete
############################################
Date=`date '+%d/%m/%y %H:%M:%S'`
print "$Date  Restauration de la base $Base\n\n">> $Trace

# si le fichier de trace de la restauration n'existe pas alors probleme
#######################################################################
if test ! -w $Trace
then
	Message="$Date  Fichier de compte-rendu non accessible en ecriture pour declencher la restauration de la base $Base"
	echo $Message

	exit 2
fi

# si le fichier de trace de la restauration n'existe pas alors probleme
#######################################################################
if test ! -w $TraceTravail
then
	Message="$Date  Probleme pour declencher la restauration de la base $Base"
	echo $Message
	print $Message >> $Trace

	exit 2
fi

# lier le fichier a un descripteur
##################################
exec 3<$TraceTravail
exec 4>>$Trace

# lecture des traces de isql
#############################
Erreur=0
BandeChangee=0
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

		# arret de la restauration si une erreur de severite superieure a 1
		if test 0$severite -gt "1"
		then
			Message="$Date  Arret de la restauration de la base $Base suite a l'erreur $numero"
			# ecrire dans le fichier de trace
			Date=`date '+%d/%m/%y %H:%M:%S'`
			print -u4 "$Date  $ligne"
			Erreur=3
		else

			case $numero in
				6.46.1|6.50.1) 
					# l'operateur doit monter le volume suivant en mode Åcriture
	
					# ecrire dans le fichier de trace 
					Date=`date '+%d/%m/%y %H:%M:%S'`
					print -u4 "$Date  $ligne"
	
					# attente de la reponse de l'operateur
					print "Veillez a changer la bande du lecteur "
					print "Le changement de bande a-t-il ete realise (sinon la restauration est interrompue) ? (OUI/NON) : "

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
	
					# recherche du numero de session de restauration
					NumSession=`grep "session_id" $TraceTravail | sed -n "1s/.* \([0-9][0-9]*\).*/\1/p"`
					if [[ X$NumSession = X ]]
					then
						Message="$Date  Changement automatique de bande impossible (numero de session non trouve)"
						Erreur=3
					fi
	
					# Prise en compte de l'action de l'operateur
					if (( $BandeChangee != 0 ))
					then
						# continuer la restauration
	
						# tracer l'action de l'operateur
						print -u4 "$Date  Changement de la bande du lecteur effectue"
	
						Commande="sp_volchanged $NumSession, \"$Device\",\"PROCEED\""
echo "$Commande \n\
go\n" \
| isql -SSQL_$Site -UMIGRAZUR -PMIGRAZUR  -Jiso_1
					else
						# arreter la restauration
	
						print -u4 "$Date  Arret de la restauration demande"
	
						Commande="sp_volchanged $NumSession, \"$Device\", \"ABORT\""
echo "$Commande \n\
go\n" \
|isql -SSQL_$Site -UMIGRAZUR -PMIGRAZUR  -Jiso_1
	
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

					Message="$Date  Arret de la restauration de la base $Base suite a l'erreur $numero"
					Erreur=2;;
	
				3.42.1)	
					# la restauration est terminee
	
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
					# Shutdown durant la restauration
	
					Message="$Date  Arret de la restauration de la base $Base suite a l'erreur $numero"
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
##############################
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

# ecriture de la cause de l'arret de la restauration
####################################################
if [[ "$Message" != "" ]]
then
	print $Message
	print -u4 $Message
fi

# arreter la restauration sous isql si elle est en attente
###########################################################
if test $Erreur -gt 1
then
	# tester si le processus isql lance en background a termine
	Fin=`grep -c "\.\.\.fin" $TraceTravail`

	# aborter la restauration
	if [[ $Fin = 0 ]]
	then
		# recherche du numero de session de restauration
		NumSession=`grep "identificateur de session du serveur" $TraceTravail | sed -n "1s/.* \([0-9][0-9]*\).*/\1/p"`

		if [[ X$NumSession = X ]]
		then
			print -u4 "$Date  Sauvegarde en attente, elle doit etre supprimee manuellement"
			Erreur=2
		else
			# arreter la demande de restauration
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

# ajout un compte rendu 
#######################
if [[ $Erreur = 0 ]]
then print -u4 "\nRestauration reussie"
fi

# fermer les descripteurs
##########################
exec 3<&-
exec 4>&-

# supprimer le fichier de travail
#################################
rm $TraceTravail


exit $Erreur
