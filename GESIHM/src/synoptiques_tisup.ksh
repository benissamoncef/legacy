#/produits/migrazur/appliSD/exec/tiflashclean.ksh
/produits/migrazur/appliSD/exec/tisupervclean.ksh

HOSTNAME=`hostname`
fichier_config=$HOME/fichiers/adm/nsup_$HOSTNAME
#if grep -q 'titbi.x' $fichier_config
#then export DISPLAY=:0.1
#else export DISPLAY=:0.0
#fi
#export DISPLAY=:0.0

#export DISPLAY=172.24.9.184:0
#export DISPLAY=10.202.253.177:0.0
#export DISPLAY=172.24.241.242:0.0

#export DISPLAY=172.24.7.188:1.0
if [[ "$DISPLAY" = "" ]] ; then
export DISPLAY=:0.0
echo "export DISPLAY=:0.0"
fi

#export DISPLAY=172.24.6.182:1.0
#export DISPLAY=172.24.6.186:0
export DISPLAY=172.24.9.185.0
HOST=`python -c "import socket;print socket.gethostbyname(socket.gethostname())" `
#export HOST=172.24.6.186
/produits/migrazur/appliSD/fichiers/syn/supervisionCS5/bin/supervisionCS5  $HOST SUPERVISION &
#/produits/migrazur/appliSD/fichiers/syn/synoptiqueCS5/bin/synoptiqueCS5  10.16.10.10 &
