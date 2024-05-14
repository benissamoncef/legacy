#!/bin/ksh
# Niepceron 09/11/1995 :ajout du -9 sinon sans effet sur acnet v4.1  v1.3
# Niepceron 20/11/1995 :ajout de la suppression des fichiers /tmp/syblog.* v1.5
#Niepceron 20/02/1996	: remplacement de rm par echo ... | args rm 

ps -u$LOGNAME |
while read pid tty time cmd rest; do
    [[ "$cmd" = axmain  ||  "$cmd" = elfsybas  ||  "$cmd" = applix ||  "$cmd" = axnet ||  "$cmd" = softmsgsrv ]]  &&
	kill -9 $pid  &&  print "tache \"$cmd\" stoppee"
done

# Suppression des fichiers /tmp/syblog.*
echo /tmp/syblog.* | xargs rm

