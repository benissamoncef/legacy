#!/bin/ksh

echo "
use CFG
go
alter table EQT_CAM_POS add pertinence tinyint default 10 not null
go" | isql -Usa -Pzigramur -SSQL_HIS

echo "
use CFG
go
alter table EQT_CAM_POS add pertinence tinyint default 10 not null
go" | isql -Usa -Pzigramur -SSQL_CI

echo "
use CFG
go
alter table EQT_CAM_POS add pertinence tinyint default 10 not null
go" | isql -Usa -Pzigramur -SSQL_CA

echo "
use CFG
go
alter table EQT_CAM_POS add pertinence tinyint default 10 not null
go" | isql -Usa -Pzigramur -SSQL_VE

echo "
use CFG
go
alter table EQT_CAM_POS add pertinence tinyint default 10 not null
go" | isql -Usa -Pzigramur -SSQL_PR

echo "
use CFG
go
alter table EQT_CAM_POS add pertinence tinyint default 10 not null
go" | isql -Usa -Pzigramur -SSQL_VD
