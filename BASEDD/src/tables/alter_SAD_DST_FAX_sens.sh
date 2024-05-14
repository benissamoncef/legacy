#!/bin/ksh

echo "
use EXP
go
alter table SAD_DST_FAX add sens tinyint default 0 not null
go" | isql -Usa -Pzigramur -SSQL_CA

echo "
use EXP
go
alter table SAD_DST_FAX add sens tinyint default 0 not null
go" | isql -Usa -Pzigramur -SSQL_VE

echo "
use EXP
go
alter table SAD_DST_FAX add sens tinyint default 0 not null
go" | isql -Usa -Pzigramur -SSQL_PR


echo "
use EXP
go
alter table SAD_DST_FAX add sens tinyint default 0 not null
go" | isql -Usa -Pzigramur -SSQL_VD


echo "
use EXP
go
alter table SAD_DST_FAX add sens tinyint default 0 not null
go" | isql -Usa -Pzigramur -SSQL_CI

