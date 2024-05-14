echo "suppr server sql DA"

echo "sp_dropserver SQL_DA
go
sp_dropserver TASRV_DA
go" | isql -Usa -Pzigramur -SSQL_CI
