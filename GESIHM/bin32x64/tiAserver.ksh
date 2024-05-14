#!/bin/ksh

ps -uroot |
while read pid tty time cmd rest; do
    [[ "$cmd" = Aserver ]]  &&
	kill -9 $pid  
done

/opt/audio/bin/Aserver -f
