ps -u$LOGNAME |
while read pid tty time cmd rest; do
    [[ "$cmd" = supervisionCS5  ]]  &&
	kill -9 $pid  &&  print "tache \"$cmd\" stoppee"
done

