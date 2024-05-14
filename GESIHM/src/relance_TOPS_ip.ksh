sleep 5
ip=$1
port1=$2
port2=$3
cat /produits/migrazur/appliSD/fichiers/adm/TOPS_modele.txt | sed "s/adresse_ip/$ip/g" | sed "s/port1/$port1/g" | sed "s/port2/$port2/g" > /produits/migrazur/appliSD/fichiers/adm/TOPS_`hostname`.txt

/produits/migrazur/appliSD/kill_tache.sh titbi.x
/produits/migrazur/appliSD/kill_tache.sh tisbi.x
