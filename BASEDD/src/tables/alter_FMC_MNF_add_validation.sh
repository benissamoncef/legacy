#!/bin/ksh

echo "
use EXP
go
alter table FMC_MNF add validation datetime  null
go" | isql -Usa -Pzigramur -SSQL_DC


echo "
use EXP
go
alter table FMC_MNF add validation datetime  null
go" | isql -Usa -Pzigramur -SSQL_CI


echo "
use EXP
go
alter table FMC_MNF add validation datetime  null
go" | isql -Usa -Pzigramur -SSQL_DS


echo "
use EXP
go
alter table FMC_MNF add validation datetime  null
go" | isql -Usa -Pzigramur -SSQL_DM

