#!/bin/ksh

echo "
use EXP
go
alter table SAD_PMV add clignotement int default 0 null
go" | isql -Usa -Pzigramur -SSQL_CI

echo "
use EXP
go
alter table SAD_PMV add clignotement int default 0 null
go" | isql -Usa -Pzigramur -SSQL_DP

echo "
use EXP
go
alter table SAD_PMV add clignotement int default 0 null
go" | isql -Usa -Pzigramur -SSQL_VC
