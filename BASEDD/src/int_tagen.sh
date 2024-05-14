ps -u$LOGNAME |
while read pid tty time cmd rest; do
    [[ "$cmd" = tagen.x ]]  &&
        kill $pid
done
/produits/migrazur/appliSD/exec/clean_ipcs.ksh

#kill des tesur
ps -u$LOGNAME |
while read pid tty time cmd rest; do
    [[ "$cmd" = tatdp.x ]]  &&
        kill $pid
done
#kill des tesur
ps -u$LOGNAME |
while read pid tty time cmd rest; do
    [[ "$cmd" = tacop.x ]]  &&
        kill $pid
done
#kill des tesur
ps -u$LOGNAME |
while read pid tty time cmd rest; do
    [[ "$cmd" = taweb.x ]]  &&
            kill $pid
done

#kill des tesur
ps -u$LOGNAME |
while read pid tty time cmd rest; do
    [[ "$cmd" = tada2.x ]]  &&
          kill $pid
done

