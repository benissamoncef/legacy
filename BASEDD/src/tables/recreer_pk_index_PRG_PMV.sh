#!/bin/ksh

echo "
use EXP
go
drop index PRG_PMV.PRG_PMV_PK
go
sp_dropkey primary, PRG_PMV
go
execute sp_primarykey PRG_PMV, numero, debut, type_jour
go
create unique index PRG_PMV_PK on PRG_PMV (numero, debut, type_jour)
go" | isql -Usa -Pzigramur -SSQL_DC


echo "
use EXP
go
drop index PRG_PMV.PRG_PMV_PK
go
sp_dropkey primary, PRG_PMV
go
execute sp_primarykey PRG_PMV, numero, debut, type_jour
go
create unique index PRG_PMV_PK on PRG_PMV (numero, debut, type_jour)
go" | isql -Usa -Pzigramur -SSQL_CI


echo "
use EXP
go
drop index PRG_PMV.PRG_PMV_PK
go
sp_dropkey primary, PRG_PMV
go
execute sp_primarykey PRG_PMV, numero, debut, type_jour
go
create unique index PRG_PMV_PK on PRG_PMV (numero, debut, type_jour)
go" | isql -Usa -Pzigramur -SSQL_DS


echo "
use EXP
go
drop index PRG_PMV.PRG_PMV_PK
go
sp_dropkey primary, PRG_PMV
go
execute sp_primarykey PRG_PMV, numero, debut, type_jour
go
create unique index PRG_PMV_PK on PRG_PMV (numero, debut, type_jour)
go" | isql -Usa -Pzigramur -SSQL_DM

