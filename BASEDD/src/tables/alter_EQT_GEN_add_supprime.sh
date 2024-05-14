#!/bin/ksh

echo "
use CFT
go
alter table EQT_GEN add eqt_supprime bit default 0 not null
go" | isql -Usa -Pzigramur -SSQL_CI



echo "
use CFG
go
alter table EQT_GEN add eqt_supprime bit default 0 not null
go" | isql -Usa -Pzigramur -SSQL_DC

echo "
use CFG
go
alter table EQT_GEN add eqt_supprime bit default 0 not null
go" | isql -Usa -Pzigramur -SSQL_HIS

echo "
use CFG
go
alter table EQT_GEN add eqt_supprime bit default 0 not null
go" | isql -Usa -Pzigramur -SSQL_CI

echo "
use CFG
go
alter table EQT_GEN add eqt_supprime bit default 0 not null
go" | isql -Usa -Pzigramur -SSQL_DS


echo "
use CFG
go
alter table EQT_GEN add eqt_supprime bit default 0 not null
go" | isql -Usa -Pzigramur -SSQL_DM
