echo " stop rep agent "

echo "sp_stop_rep_agent EXP
go
sp_stop_rep_agent CFG*
go " | isql -Usa -Pzigramur -SSQL_DA

echo " drop connections"
echo "drop connection SQL_DS.EXP
go
drop connection SQL_HIS.HIS
go
drop connection SQL_CI.EXP
go
drop connection SQL_CI.CFG
go" | isql -Usa -Pzigramur -SREP_DA
