#!/bin/bash
ip=$1
protocole=$2
port=$3
encodeur=$4
ps -u$LOGNAME |
while read pid tty time cmd rest; do
    [[ "$cmd" = vlc  ]]  &&
        kill -9 $pid 
done

url=$protocole"://"$ip":"$port"/"$encodeur
echo $url
DISPLAY=:0.1 cvlc --scale 0.3 "$url" 2>&1 1>/tmp/video.log
#vlc "$url"
