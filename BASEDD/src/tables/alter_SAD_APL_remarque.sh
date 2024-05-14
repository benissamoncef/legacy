#!/bin/ksh

echo "
use EXP
go
alter table SAD_APL modify remarque char(250) not null
go" | isql -Usa -Pzigramur -SSQL_DC1


# echo "
# use HIS
# go
# alter table SAD_APL modify remarque char(250) not null
# go" | isql -Usa -Pzigramur -SSQL_HIS


echo "
use EXP
go
alter table SAD_APL modify remarque char(250) not null
go" | isql -Usa -Pzigramur -SSQL_DM


echo "
use EXP
go
alter table SAD_APL modify remarque char(250) not null
go" | isql -Usa -Pzigramur -SSQL_DS


echo "
use EXP
go
alter table SAD_APL modify remarque char(250) not null
go" | isql -Usa -Pzigramur -SSQL_CI

