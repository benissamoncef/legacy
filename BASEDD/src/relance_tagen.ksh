
echo " Suppresssion de la tache tagen.x "
ps -u$LOGNAME |
while read pid tty time cmd rest; do
    [[ "$cmd" = tagen.x ]]  &&
        kill $pid
done

echo " Nettoyage de la memoire partagee "
/produits/migrazur/appliSD/exec/clean_ipcs.ksh


echo " Suppresssion de la tache tatdp.x "
# kill des tatdp.x
ps -u$LOGNAME |
while read pid tty time cmd rest; do
    [[ "$cmd" = tatdp.x ]]  &&
        kill $pid
done

echo " Suppresssion de la tache tacop.x"
# kill des tacop.x
ps -u$LOGNAME |
while read pid tty time cmd rest; do
    [[ "$cmd" = tacop.x ]]  &&
        kill $pid
done

echo " Suppresssion de la tache taweb.x"
#kill des taweb.x
ps -u$LOGNAME |
while read pid tty time cmd rest; do
    [[ "$cmd" = taweb.x ]]  &&								
            kill $pid
done

echo " "
echo " Traitement spécifique pour tada2.x"
echo " Deplacement de tada2.x en tada2.x_tmp pour pas de relance automatique"
if  [ -f /produits/migrazur/appliSD/exec/tada2.x ] ; then 
	echo " ERREUR : le fichier tada2.x n'existe pas"
else				
	mv /produits/migrazur/appliSD/exec/tada2.x /produits/migrazur/appliSD/exec/tada2.x_tmp
fi	
				
#kill des tada2.x
ps -u$LOGNAME |
while read pid tty time cmd rest; do
    [[ "$cmd" = tada2.x ]]  &&
    kill $pid
done

echo " Attente 1 minute et 10 secondes pour que la memoire partagee (tagen.x) soit initialisee corectement"
echo " MERCI d'attendre sinon tada2.x_tmp  ne sera pas renome en tada2.x "
sleep 70 
echo " deplacement de tada2.x_tmp en tada2.x "
mv /produits/migrazur/appliSD/exec/tada2.x_tmp /produits/migrazur/appliSD/exec/tada2.x
echo "Lancement de tada2.x via nohup"
echo "nohup /produits/migrazur/appliSD/exec/tada2.x migrazur&"
nohup /produits/migrazur/appliSD/exec/tada2.x migrazur&
								
							
