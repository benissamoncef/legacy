#!/bin/ksh

echo "
use CFG
go
alter table EQT_CAM add IP char(20) null,
			protocole char(10) null,
			port smallint default 0 not null,
			encoder char(20) null
go" | isql -Usa -Pzigramur -SSQL_HIS

echo "
use CFG
go
alter table EQT_CAM add IP char(20) null,
			protocole char(10) null,
			port smallint default 0 not null,
			encoder char(20) null
go" | isql -Usa -Pzigramur -SSQL_CI

echo "
use CFG
go
alter table EQT_CAM add IP char(20) null,
			protocole char(10) null,
			port smallint default 0 not null,
			encoder char(20) null
go" | isql -Usa -Pzigramur -SSQL_CA

echo "
use CFG
go
alter table EQT_CAM add IP char(20) null,
			protocole char(10) null,
			port smallint default 0 not null,
			encoder char(20) null
go" | isql -Usa -Pzigramur -SSQL_VE

echo "
use CFG
go
alter table EQT_CAM add IP char(20) null,
			protocole char(10) null,
			port smallint default 0 not null,
			encoder char(20) null
go" | isql -Usa -Pzigramur -SSQL_PR

echo "
use CFG
go
alter table EQT_CAM add IP char(20) null,
			protocole char(10) null,
			port smallint default 0 not null,
			encoder char(20) null
go" | isql -Usa -Pzigramur -SSQL_VD
