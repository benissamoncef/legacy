#!/bin/ksh

echo "
use CFG
go
alter table ANN_GEO add sens tinyint default 0 not null
go" | isql -Usa -Pzigramur -SSQL_HIS

echo "
use CFG
go
alter table ANN_GEO add sens tinyint default 0 not null
go" | isql -Usa -Pzigramur -SSQL_CA

echo "
use CFG
go
alter table ANN_GEO add sens tinyint default 0 not null
go" | isql -Usa -Pzigramur -SSQL_VE

echo "
use CFG
go
alter table ANN_GEO add sens tinyint default 0 not null
go" | isql -Usa -Pzigramur -SSQL_PR


echo "
use CFG
go
alter table ANN_GEO add sens tinyint default 0 not null
go" | isql -Usa -Pzigramur -SSQL_VD


echo "
use CFG
go
alter table ANN_GEO add sens tinyint default 0 not null
go" | isql -Usa -Pzigramur -SSQL_CI

