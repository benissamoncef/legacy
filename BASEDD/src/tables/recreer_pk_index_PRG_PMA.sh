#!/bin/ksh

echo "
use EXP
go
drop index PRG_PMA.PRG_PMA_PK
go
sp_dropkey primary, PRG_PMA
go
execute sp_primarykey PRG_PMA, numero, debut, type_jour
go
create unique index PRG_PMA_PK on PRG_PMA (numero, debut, type_jour)
go" | isql -Usa -Pzigramur -SSQL_DC


echo "
use EXP
go
drop index PRG_PMA.PRG_PMA_PK
go
sp_dropkey primary, PRG_PMA
go
execute sp_primarykey PRG_PMA, numero, debut, type_jour
go
create unique index PRG_PMA_PK on PRG_PMA (numero, debut, type_jour)
go" | isql -Usa -Pzigramur -SSQL_CI


echo "
use EXP
go
drop index PRG_PMA.PRG_PMA_PK
go
sp_dropkey primary, PRG_PMA
go
execute sp_primarykey PRG_PMA, numero, debut, type_jour
go
create unique index PRG_PMA_PK on PRG_PMA (numero, debut, type_jour)
go" | isql -Usa -Pzigramur -SSQL_DS


echo "
use EXP
go
drop index PRG_PMA.PRG_PMA_PK
go
sp_dropkey primary, PRG_PMA
go
execute sp_primarykey PRG_PMA, numero, debut, type_jour
go
create unique index PRG_PMA_PK on PRG_PMA (numero, debut, type_jour)
go" | isql -Usa -Pzigramur -SSQL_DM

