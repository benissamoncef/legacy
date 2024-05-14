#!/bin/bash
#export HOME=/produits/migrazur/appliSD/ecrans
export HOME=/produits/migrazur/appliSD
#cd /opt/mozilla
auto=$1
pr=$2
#sens=$3

if [ "$auto" = "1" ]; then
autoroute="A"
elif [ "$auto" = "6" ]; then
autoroute="B"
elif [ "$auto" = "3" ]; then
autoroute="C"
elif [ "$auto" = "4" ]; then
autoroute="D"
elif [ "$auto" = "7" ]; then
autoroute="E"
elif [ "$auto" = "5" ]; then
autoroute="F"
elif [ "$auto" = "8" ]; then
autoroute="G"
elif [ "$auto" = "2" ]; then
autoroute="H"
else 
autoroute="A"
fi

echo "auto=$autoroute; Pr=$pr"
DISPLAY=:0.1 firefox -P default "http://serveuririsvision/Default.aspx?classe=geocodee&loc=H$autoroute$pr"
#http://serveuririsvision/Default.aspx?classe=geocodee&loc=PF109000_0
#http://serveuririsvision/Default.aspx?classe=geocodee&loc=HF109200
