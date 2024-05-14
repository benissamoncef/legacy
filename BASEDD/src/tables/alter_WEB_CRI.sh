#!/bin/ksh

echo "
use HIS
go
alter table WEB_CRI add message2 char(255) null
go" | isql -Usa -Pzigramur -SSQL_HIS

echo "
use EXP
go
alter table WEB_CRI add message2 char(255) null
go" | isql -Usa -Pzigramur -SSQL_CI
