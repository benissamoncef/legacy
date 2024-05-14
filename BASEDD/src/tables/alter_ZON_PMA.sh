#!/bin/ksh

echo "
use CFG
go
alter table ZON_PMA add distance int default 0 not null
go" | isql -Usa -Pzigramur -SSQL_HIS

echo "
use CFG
go
alter table ZON_PMA add distance int default 0 not null
go" | isql -Usa -Pzigramur -SSQL_CI

echo "
use CFG
go
alter table ZON_PMA add distance int default 0 not null
go" | isql -Usa -Pzigramur -SSQL_CA

echo "
use CFG
go
alter table ZON_PMA add distance int default 0 not null
go" | isql -Usa -Pzigramur -SSQL_VE

echo "
use CFG
go
alter table ZON_PMA add distance int default 0 not null
go" | isql -Usa -Pzigramur -SSQL_PR

echo "
use CFG
go
alter table ZON_PMA add distance int default 0 not null
go" | isql -Usa -Pzigramur -SSQL_VD
