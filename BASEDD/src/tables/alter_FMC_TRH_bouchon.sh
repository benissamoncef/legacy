echo "
use EXP
go
alter table FMC_TRH add bouchon tinyint null
go" | isql -Usa -Pzigramur -SSQL_CI

