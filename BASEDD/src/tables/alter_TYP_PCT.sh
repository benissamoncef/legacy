#!/bin/ksh
# Verification sur quelle base on pointe
( ping -c 1 sdci1| grep SDCI ) > /dev/null 
if [ $? -eq 0 ]; then
# BDD Escota
echo "Escota"

echo "
use CFT
go
alter table TYP_PCT
add type_pnc int default 1 not null, fichier_pnc char(25) null
go" | isql -Usa -Pzigramur -SSQL_CI

echo "
use CFG
go
alter table TYP_PCT
add type_pnc int default 1 not null, fichier_pnc char(25) null
go" | isql -Usa -Pzigramur -SSQL_CI

echo "
use CFG
go
alter table TYP_PCT
add type_pnc int default 1 not null, fichier_pnc char(25) null
go" | isql -Usa -Pzigramur -SSQL_DP

echo "
use CFG
go
alter table TYP_PCT
add type_pnc int default 1 not null, fichier_pnc char(25) null
go" | isql -Usa -Pzigramur -SSQL_VC

echo "
use CFG
go
alter table TYP_PCT
add type_pnc int default 1 not null, fichier_pnc char(25) null
go" | isql -Usa -Pzigramur -SSQL_HIS

else
# BDD CNA
echo "CNA"

echo "
use CFT
go
alter table TYP_PCT
add type_pnc int default 1 not null, fichier_pnc char(25) null
go" | isql -Usa -Pzigramur -SSQL_CA

echo "
use CFG
go
alter table TYP_PCT
add type_pnc int default 1 not null, fichier_pnc char(25) null
go" | isql -Usa -Pzigramur -SSQL_CA

echo "
use CFG
go
alter table TYP_PCT
add type_pnc int default 1 not null, fichier_pnc char(25) null
go" | isql -Usa -Pzigramur -SSQL_HIS

fi
