#!/bin/ksh


echo "
use CFG
go
alter table RES_DIT add version_radt	char(10)	not null	default "1.0"
go" | isql -Usa -Pzigramur -SSQL_CI

# echo "
# use CFG
# go
# update RES_DIT set version_radt="1.0"
# go" | isql -Usa -Pzigramur -SSQL_CI

#echo "
#use CFG
#go
#alter table RES_DIT add version_radt	char(10) null
#go" | isql -Usa -Pzigramur -SSQL_CI


