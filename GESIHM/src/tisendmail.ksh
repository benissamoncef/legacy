#!/bin/ksh

## Fichier : @(#)tisendmail.ksh	1.2      Release : 1.2        Date : 12/05/08
###############################################################################
## STERIA *  PROJET MIGRAZUR
###############################################################################
## SOUS-SYSTEME  GESIHM
###############################################################################
## MODULE ITMA_TFX * FICHIER tisendmail.ksh
###############################################################################
## DESCRIPTION DU MODULE :
##
##   Module d'envoi de mail.
##
##   Cf. DCG 2.4.33
##
###############################################################################
## HISTORIQUE :
## JMG	12/12/07	creation
## JPL	05/12/08	Archivage version sur SPCI (LANG="") + Num action ecrit en fichier log
## PNI	01/10/14	Modif pour nouveau serveur SNMP
## PNI	09/12/14	Modif pour remplacer '-' par '_' (attente option par le send
## PNI	19/04/16	Modif pour remplacer '-' en début de ligne par ' -' 
## PNI	13/06/16	Destinataire en bcc systematiquement 
## PNI  01/09/16	Emetteur suivant le site d'envoi
## PNI  01/02/18	Ajout de l'emetteur en 'to' pour les SMS
## PNI 16/02/18		conversion en utf8 pour accent dans les SMS
## RGR  23/10/18      Correction synchro revision                       1.13 
## PNI 	02/09/19	Mise en echac de l'action pour mail sans destinataire ou adr invalide  1.14
##############################################################################

# Ignorer les signaux hangup, interrupt et quit
# ---------------------------------------------
trap "" 1 2 3

export LANG=""

NOMPROG=${0##*/}

PATHMIGRAZUR=/produits/migrazur/appliSD
SMTP_SERVER=10.40.210.110
MAIL_FROM="<CIT.Mandelieu@vinci-autoroutes.com>"
FIC_MAIL=/tmp/fic_mail.txt
FIC_MAIL_TMP=/tmp/fic_mail_tmp.txt

# Recuperer les parametres d'appel
# --------------------------------
FIC_TEXTE=$1
NUMSITE=$2
NOMSITE=$3
FIC_OBJET=$4
NB_DEST=$5


OS=`uname`

#rm -f /tmp/$NOMPROG.log

#Choix de l'emetteur

if [ "$NOMSITE" = "PR" ]
then
	MAIL_FROM="<pcsecurite.duranceprovence@vinci-autoroutes.com>"
else if [ "$NOMSITE" = "DP" ]
then
	MAIL_FROM="<pcsecurite.duranceprovence@vinci-autoroutes.com>"
else if [ "$NOMSITE" = "CA" ]
then
	MAIL_FROM="<pccotedazur.escota@vinci-autoroutes.com>"
else if [ "$NOMSITE" = "VE" ]
then
 	MAIL_FROM="<pcvaresterel.escota@vinci-autoroutes.com>"
else if [ "$NOMSITE" = "VC" ]
then
	MAIL_FROM="<pcsecurite.varcotedazur@vinci-autoroutes.com>"
fi
fi
fi
fi
fi



#PC VAR ESTEREL.Escota <pcvaresterel.escota@vinci-autoroutes.com>


# Lire les destinataires
# ----------------------
OLDIFS=$IFS
IFS="|"
horo=`date`
DATECR=`date '+%d/%m/%Y %H:%M:%S'`
print "$horo, $FIC_TEXTE" >>/tmp/$NOMPROG.log

#Substitution des -
cp $FIC_TEXTE $FIC_TEXTE.tmp
cp $FIC_OBJET $FIC_OBJET.tmp
sed -r 's/^\-/ \-/g' $FIC_TEXTE.tmp > $FIC_TEXTE.tmp2
sed -e 's/\"/ /g' $FIC_TEXTE.tmp2 > $FIC_TEXTE

sed -r 's/^\-/ \-/g' $FIC_OBJET.tmp > $FIC_OBJET.tmp2
sed -e 's/\"/ /g' $FIC_OBJET.tmp2 > $FIC_OBJET

i=0
while [ i -lt $NB_DEST ] 
do
	read NUMACTION EMAIL
	echo "action= : $NUMACTION-$EMAIL." >>/tmp/$NOMPROG.log

cp  $FIC_TEXTE /var/spymail/$NUMACTION.CORPS.log
cp  $FIC_OBJET /var/spymail/$NUMACTION.OBJET.log

	# et pour chacun envoyer le mail
	# -----------------------------

	rm -f $FIC_MAIL
	rm -f $FIC_MAIL_TMP
   	if [ "$OS" = "HP-UX" ]
	then
	################## Version HP-UX #################### 
	print "helo $SMTP_SERVER" >> $FIC_MAIL
	print "mail from: $MAIL_FROM" >> $FIC_MAIL
	print "rcpt to:" $EMAIL >> $FIC_MAIL
	print "data" >> $FIC_MAIL
	print "from: \"ESCOTA\" $MAIL_FROM"  >> $FIC_MAIL
	print "to: $EMAIL" >> $FIC_MAIL
	print "subject: " `cat $FIC_OBJET` >> $FIC_MAIL
	print "\n" >> $FIC_MAIL
	cat $FIC_TEXTE >> $FIC_MAIL

	print "ESCOTA le $DATECR." >> $FIC_MAIL
	print "\n" >> $FIC_MAIL
	print "." >> $FIC_MAIL
	print "quit" >> $FIC_MAIL
	cat $FIC_MAIL | while read ligne
	do
	echo $ligne
	if [[ "$ligne" = "." ]]; then
	sleep 1
	fi
	if [[ "$ligne" = "quit" ]]; then
	sleep 1
	fi
	done | telnet $SMTP_SERVER 25 > /tmp/$NOMPROG$NUMACTION.log

	else

	################## Version Linux  #################### 
        print "#!/usr/bin/expect" >> $FIC_MAIL
        print "spawn telnet $SMTP_SERVER 25" >> $FIC_MAIL
        print "send \"helo $SMTP_SERVER\\\r\"" >> $FIC_MAIL
        print "send \"mail from: $MAIL_FROM\\\r\"" >> $FIC_MAIL
        print "send \"rcpt to: $EMAIL\\\r\"" >> $FIC_MAIL
        print "send \"data\\\r\"" >> $FIC_MAIL
        print "send \"from: $MAIL_FROM\\\r\""  >> $FIC_MAIL
        if [[ "$EMAIL" = "123035@mailtosms.dmc.sfr-sh.fr" ]]; then
	        print "send \"to: $EMAIL\\\r\"" >> $FIC_MAIL
	fi	
	print "send \"bcc: $EMAIL\\\r\"" >> $FIC_MAIL
        print "send \"subject: " `cat $FIC_OBJET` "\\\r\"" >> $FIC_MAIL
        print "send \"\\\r\"" >> $FIC_MAIL
        if [[ "$EMAIL" = "123035@mailtosms.dmc.sfr-sh.fr" ]]; then
		cat $FIC_TEXTE | iconv -f ISO88592 -t UTF8 | awk '{ print "send \"" $0 "\\r\"" }' >> $FIC_MAIL
	else
        	cat $FIC_TEXTE | awk '{ print "send \"" $0 "\\r\"" }' >> $FIC_MAIL
        	print "send \"ESCOTA le $DATECR.\\\r\"" >> $FIC_MAIL
	fi
        print "send \"\\\r\"" >> $FIC_MAIL
        print "send \"\\\r.\\\r\"" >> $FIC_MAIL
        print "sleep 1" >> $FIC_MAIL
        print "send \"quit\\\r\"" >> $FIC_MAIL
cp  $FIC_MAIL /var/spymail/$NUMACTION.$NB_DEST.Text1.log
        print "sleep 1" >> $FIC_MAIL
        print "expect eof" >> $FIC_MAIL

        chmod +x $FIC_MAIL
        $FIC_MAIL > /tmp/$NOMPROG$NUMACTION.log

	fi
	((i=i+1))
#Queued mail for delivery

	cp  /tmp/$NOMPROG$NUMACTION.log /var/spymail/$NUMACTION.$NB_DEST.Final.log

	cr=`grep 'Queued mail for delivery' /tmp/$NOMPROG$NUMACTION.log | sed 's/[^-0-9]//g'`
	if [[ "$cr" = "" ]]; then
		#on met l action a ECHEC en base
		DATE_ECHEC="'$DATECR'"
		DATE_SUCCES=NULL
	else
		# on met l action a SUCCES en base
		DATE_SUCCES="'$DATECR'"
		DATE_ECHEC=NULL
		#rm -f /tmp/$NOMPROG$NUMACTION.log
	fi
	verifadr=0
	verifadr=`expr index "$EMAIL" "@"`

	if [ verifadr -eq 0 ]; then
		echo "Mail destinataire $NUMACTION incorrect" >>/tmp/$NOMPROG.log
	        # on met l action a ECHEC en base
		DATE_ECHEC="'$DATECR'"
		DATE_SUCCES=NULL
	fi
	
	{
	echo "exec PRC..XZAC18  $NUMACTION, $NUMSITE, $DATE_SUCCES, $DATE_ECHEC"
	echo "go"
	} | isql -UMIGRAZUR -PMIGRAZUR -SSQL_$NOMSITE >> /tmp/$NOMPROG.log &

	#rm -f /tmp/$NOMPROG$NUMACTION.log
done

find /tmp/ITMA_TFX_* -mtime +5 -exec rm {} \;

