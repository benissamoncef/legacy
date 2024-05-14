echo "
use CFG
go
alter table EQT_ZDP modify code smallint not null
go" | isql -Usa -Pzigramur -SSQL_CI

echo "
use CFG
go
alter table EQT_ZDP modify code smallint not null
go" | isql -Usa -Pzigramur -SSQL_HIS

echo "
use CFG
go
alter table EQT_ZDP modify code smallint not null
go" | isql -Usa -Pzigramur -SSQL_DS

echo "
use CFG
go
alter table EQT_ZDP modify code smallint not null
go" | isql -Usa -Pzigramur -SSQL_DM

