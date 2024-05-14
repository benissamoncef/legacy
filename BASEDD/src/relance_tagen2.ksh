cd /produits/migrazur/appliSD/exec

mv tagen.x tagen.x.stop
mv tatdp.x tatdp.x.stop
mv tacop.x tacop.x.stop
mv taweb.x taweb.x.stop
mv tada2.x tada2.x.stop
echo " Suppresssion de la tache tagen.x "
ps -u$LOGNAME |
while read pid tty time cmd rest; do
    [[ "$cmd" = tagen.x ]]  &&
        kill -9 $pid
done

echo " Nettoyage de la memoire partagee "
/produits/migrazur/appliSD/exec/clean_ipcs.ksh


echo " Suppresssion de la tache tatdp.x "
# kill des tatdp.x
ps -u$LOGNAME |
while read pid tty time cmd rest; do
    [[ "$cmd" = tatdp.x ]]  &&
        kill -9 $pid
done

echo " Suppresssion de la tache tacop.x"
# kill des tacop.x
ps -u$LOGNAME |
while read pid tty time cmd rest; do
    [[ "$cmd" = tacop.x ]]  &&
        kill -9 $pid
done

echo " Suppresssion de la tache taweb.x"
#kill des taweb.x
ps -u$LOGNAME |
while read pid tty time cmd rest; do
    [[ "$cmd" = taweb.x ]]  &&
            kill -9 $pid
done

#kill des tada2.x
ps -u$LOGNAME |
while read pid tty time cmd rest; do
    [[ "$cmd" = tada2.x ]]  &&
    kill -9 $pid
done

/produits/migrazur/appliSD/exec/clean_ipcs.ksh

echo "redemarrage"
mv tagen.x.stop tagen.x
mv tatdp.x.stop tatdp.x
mv tacop.x.stop tacop.x
mv taweb.x.stop taweb.x

nohup tagen.x migrazur&
sleep 5
nohup tatdp.x migrazur&
nohup tacop.x migrazur&
nohup taweb.x migrazur&


echo " Attente 1 minute et 10 secondes pour que la memoire partagee (tagen.x) soit initialisee corectement"
echo " MERCI d'attendre sinon tada2.x_tmp  ne sera pas renome en tada2.x "
sleep 70
mv tada2.x.stop tada2.x
echo "Lancement de tada2.x via nohup"
echo "nohup /produits/migrazur/appliSD/exec/tada2.x migrazur&"
nohup tada2.x migrazur&
