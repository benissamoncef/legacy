#!/bin/ksh

echo "
use CFG
go
alter table EQT_PAL add adresse_ip char(20) null,
			port int default 0 not null
go
alter table EQT_BAF add adresse_ip char(20) null,
			port int default 0 not null

go" | isql -Usa -Pzigramur -SSQL_HIS


