ps -u$LOGNAME |
while read pid tty time cmd rest; do
    [[ "$cmd" = synoptiqueCS5  ]]  &&
	kill -9 $pid  &&  print "tache \"$cmd\" stoppee"
done

