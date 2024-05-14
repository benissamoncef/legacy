#!/bin/ksh

echo "
use EXP
go
alter table FMC_INC add code_nature tinyint null
go" | isql -Usa -Pzigramur -SSQL_DC

echo "
use HIS
go
alter table FMC_INC add code_nature tinyint null
go" | isql -Usa -Pzigramur -SSQL_HIS

echo "
use EXP
go
alter table FMC_INC add code_nature tinyint null
go" | isql -Usa -Pzigramur -SSQL_CI

echo "
use EXP
go
alter table FMC_INC add code_nature tinyint null
go" | isql -Usa -Pzigramur -SSQL_DS

echo "
use EXP
go
alter table FMC_INC add code_nature tinyint null
go" | isql -Usa -Pzigramur -SSQL_DM
