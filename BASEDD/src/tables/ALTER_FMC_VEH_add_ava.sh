echo "
use EXP
go
alter table FMC_VEH add id_ava char(50) null
go" | isql -Usa -Pzigramur -SSQL_CI

echo "
use EXP
go
alter table FMC_VEH add dangerosite int null
go" | isql -Usa -Pzigramur -SSQL_CI

echo "
use EXP
go
alter table FMC_VEH add id_ava char(50) null
go" | isql -Usa -Pzigramur -SSQL_DP

echo "
use EXP
go
alter table FMC_VEH add dangerosite int null
go" | isql -Usa -Pzigramur -SSQL_DP

echo "
use EXP
go
alter table FMC_VEH add id_ava char(50) null
go" | isql -Usa -Pzigramur -SSQL_DM

echo "
use EXP
go
alter table FMC_VEH add dangerosite int null
go" | isql -Usa -Pzigramur -SSQL_DM