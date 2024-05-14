#!/bin/ksh

echo "
use EXP
go
alter table FMC_HIS_DER add ecoulement tinyint null
go" | isql -Usa -Pzigramur -SSQL_DC


echo "
use EXP
go
alter table FMC_HIS_DER add ecoulement tinyint null
go" | isql -Usa -Pzigramur -SSQL_DS

echo "
use EXP
go
alter table FMC_HIS_DER add ecoulement tinyint null
go" | isql -Usa -Pzigramur -SSQL_DM

echo "
use EXP
go
alter table FMC_HIS_DER add ecoulement tinyint null
go" | isql -Usa -Pzigramur -SSQL_CI
