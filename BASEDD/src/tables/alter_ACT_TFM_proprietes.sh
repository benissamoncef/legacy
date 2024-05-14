
#echo "
#use EXP
#go
#alter table ACT_TFM add proprietes tinyint default 0 not null
#go" | isql -Usa -Pzigramur -SSQL_DC

echo "
use HIS
go
alter table ACT_TFM add proprietes tinyint default 0 not null
go" | isql -Usa -Pzigramur -SSQL_HIS

echo "
use EXP
go
alter table ACT_TFM add proprietes tinyint default 0 not null
go" | isql -Usa -Pzigramur -SSQL_CI

echo "
use EXP
go
alter table ACT_TFM add proprietes tinyint default 0 not null
go" | isql -Usa -Pzigramur -SSQL_DS

echo "
use EXP
go
alter table ACT_TFM add proprietes tinyint default 0 not null
go" | isql -Usa -Pzigramur -SSQL_DM

