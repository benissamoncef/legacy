#!/bin/ksh

echo "
use HIS
go
drop index FMC_MNF.FMC_MNF_PK
go
sp_dropkey primary, FMC_MNF
go
execute sp_primarykey FMC_MNF, numero, cle, validation
go
create unique index FMC_MNF_PK on FMC_MNF (numero, cle, validation)
go" | isql -Usa -Pzigramur -SSQL_HIS


echo "
use EXP
go
drop index FMC_MNF.FMC_MNF_PK
go
sp_dropkey primary, FMC_MNF
go
execute sp_primarykey FMC_MNF, numero, cle, validation
go
create unique index FMC_MNF_PK on FMC_MNF (numero, cle, validation)
go" | isql -Usa -Pzigramur -SSQL_DC


echo "
use EXP
go
drop index FMC_MNF.FMC_MNF_PK
go
sp_dropkey primary, FMC_MNF
go
execute sp_primarykey FMC_MNF, numero, cle, validation
go
create unique index FMC_MNF_PK on FMC_MNF (numero, cle, validation)
go" | isql -Usa -Pzigramur -SSQL_CI


echo "
use EXP
go
drop index FMC_MNF.FMC_MNF_PK
go
sp_dropkey primary, FMC_MNF
go
execute sp_primarykey FMC_MNF, numero, cle, validation
go
create unique index FMC_MNF_PK on FMC_MNF (numero, cle, validation)
go" | isql -Usa -Pzigramur -SSQL_DS


echo "
use EXP
go
drop index FMC_MNF.FMC_MNF_PK
go
sp_dropkey primary, FMC_MNF
go
execute sp_primarykey FMC_MNF, numero, cle, validation
go
create unique index FMC_MNF_PK on FMC_MNF (numero, cle, validation)
go" | isql -Usa -Pzigramur -SSQL_DM

