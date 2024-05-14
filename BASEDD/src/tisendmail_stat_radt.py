## Importation des modules
import os
import sys
import smtplib
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
from email.mime.base import MIMEBase
from email import encoders

# Nom du programme
# ----------------
Reps_Prog = sys.argv[0].split ('/')
Reps_Prog = Reps_Prog[len(Reps_Prog) - 1].split ("\\")
Nom_Prog = Reps_Prog[len(Reps_Prog) - 1]


# Verifier la presence du fichier de piece jointe en argument
# -----------------------------------------------------------
if len(sys.argv) < 2 :
	sys.stderr.write (Nom_Prog + ": Un nom de fichier de piece jointe doit etre fourni\n")
	sys.exit (1)


# Ouvrir le fichier de piece jointe
# ---------------------------------
Fichier_PJ = sys.argv[1]
Fichier_PJ = Fichier_PJ.replace ('\\', '/')
idxSepar = Fichier_PJ.rfind('/')
if idxSepar < 0 :
	Rep_PJ = "."
	Nom_Fichier_PJ = Fichier_PJ
else :
	Rep_PJ = Fichier_PJ[:idxSepar + 1]
	Nom_Fichier_PJ = Fichier_PJ[idxSepar + 1:]

if Nom_Fichier_PJ == "" :
	sys.stderr.write (Nom_Prog + ": Le nom du fichier de piece jointe est errone (vide)\n")
	sys.exit (2)

try :
	os.chdir(Rep_PJ)
	piece = open(Nom_Fichier_PJ, "rb")
except Exception :
	sys.stderr.write (Nom_Prog + ": Fichier de piece jointe absent ou illisible : \"" +
	                     Nom_Fichier_PJ + "\"\n")
	sys.exit (3)


# Definir l'expediteur et les destinataires
# -----------------------------------------
Expediteur = "<CIT.Mandelieu@vinci-autoroutes.com>"
Destinataires = [
    'Olivier.Candelier@vinci-autoroutes.com',
    'David.Roig@vinci-autoroutes.com',
    'Belina.Neubert@vinci-autoroutes.com',
    'Michel.Ribiere@vinci-autoroutes.com',
    'Jaouad.Elfatmi@vinci-autoroutes.com',
    'Jean-Claude.Dacostamoreira@vinci-autoroutes.com',
    'damien.bedreddine@vinci-autoroutes.com'
]


# Constituer le message avec son texte
# ------------------------------------
message = MIMEMultipart()
message['From'] = Expediteur
message['To'] = ';'.join(Destinataires)
message['Subject'] = "Rapport hebdomadaire d'etat de fonctionnement des stations RADT"
msgText = "Veuillez trouver ci-joint le rapport d'etat de fonctionnement des stations RADT pour la semaine derniere.\n\nCordialement."
message.attach(MIMEText(msgText.encode('utf-8'), 'plain', 'utf-8'))


# Encoder la piece jointe en Base64 et l'attacher au message
# ----------------------------------------------------------
part = MIMEBase('application', 'octet-stream')	## Encodage piece jointe en Base64
part.set_payload((piece).read())
encoders.encode_base64(part)
part.add_header('Content-Disposition', "piece; filename= %s" % Nom_Fichier_PJ)
message.attach(part)


# Se connecter au serveur sortant sur le port SMTP et envoyer l'e-mail
# --------------------------------------------------------------------
#serveur = smtplib.SMTP('smtp.gmail.com', 587)
serveur = smtplib.SMTP('10.40.210.110')
#serveur.starttls()
#serveur.login(Expediteur, "xxx")
texte = message.as_string().encode('utf-8')
serveur.sendmail(Expediteur, Destinataires, texte)
serveur.quit()
