# Produit le descriptif des

for dist in CI DM DS HIS
do
echo "DESCRIPTION REPLICATION DE $dist	`date`" > description_REP_$dist.lis
echo "Routes :">>description_REP_$dist.lis
echo "use ${dist}_RSSD
go
select 'source:',I1.name1,' dest:',I2.name1
from rs_routes R, rs_idnames I1, rs_idnames I2
where R.dest_rsid=I2.id and R.source_rsid=I1.id
go" | isql -SSQL_$dist -Usa -Pzigramur>>description_REP_$dist.lis

echo "Subscriptions :">>description_REP_$dist.lis
echo "use ${dist}_RSSD
go
select subname+' '+dsname+' '+dbname from rs_subscriptions r,rs_databases d  where subname not like 'rs_%' and r.dbid=d.dbid
go" | isql -SSQL_$dist -Usa -Pzigramur >>description_REP_$dist.lis

done
exit

