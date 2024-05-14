echo "drop route DA->HIS"

echo "drop route to REP_DA with nowait
go
sysadmin purge_route_at_replicate, REP_DA
go " | isql -Usa -Pzigramur -SREP_HIS

echo "drop route DA->DS"
echo "drop route to REP_DA with nowait
go
sysadmin purge_route_at_replicate, REP_DA
go " | isql -Usa -Pzigramur -SREP_DS

echo "drop route DA->CI"
echo "drop route to REP_DA with nowait
go
sysadmin purge_route_at_replicate, REP_DA
go " | isql -Usa -Pzigramur -SREP_CI

echo "drop route DA"
echo "drop route to REP_DSwith nowait
go
sysadmin purge_route_at_replicate, REP_DS
go
drop route to REP_CI with nowait
go
sysadmin purge_route_at_replicate, REP_CI
go
drop route to REP_HIS with nowait
go
sysadmin purge_route_at_replicate, REP_HIS
go " | isql -Usa -Pzigramur -SREP_DA
