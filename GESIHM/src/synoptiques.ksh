/produits/migrazur/appliSD/exec/tiflashclean.ksh

HOSTNAME=`hostname`
fichier_config=$HOME/fichiers/adm/nsup_$HOSTNAME
if [ $# -eq 3 ]
	then export DISPLAY=$1:$2.$3
elif grep -q 'titbi.x' $fichier_config
	then export DISPLAY=:0.1
else export DISPLAY=:0.0
fi

HOST=`python -c "import socket;print socket.gethostbyname(socket.gethostname())" `

/produits/migrazur/appliSD/fichiers/syn/synoptiqueCS5/bin/synoptiqueCS5  $HOST &
#/produits/migrazur/appliSD/fichiers/syn/synoptiqueCS5/bin/synoptiqueCS5  10.16.10.10 &
