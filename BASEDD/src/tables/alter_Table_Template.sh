#!/bin/ksh
# Verification sur quelle base on pointe
( ping -c 1 sdci1| grep SDCI ) > /dev/null 
if [ $? -eq 0 ]; then
# BDD Escota
echo "Escota"

echo "
use CFT/CFG/EXP
go
alter table MY_TABLE_TO_MODIFY 	add/drop...
go" | isql -Usa -Pzigramur -SSQL_CI
							-SSQL_DS
							-SSQL_DM
echo "
use HIS
go
alter table MY_TABLE_TO_MODIFY 	add/drop...
go" | isql -Usa -Pzigramur -SSQL_HIS

else
# BDD CNA
echo "CNA"

echo "
use CFT/CFG/EXP
go
alter table MY_TABLE_TO_MODIFY 	add/drop...
go" | isql -Usa -Pzigramur -SSQL_CA

echo "
use HIS
go
alter table MY_TABLE_TO_MODIFY 	add/drop...
go" | isql -Usa -Pzigramur -SSQL_HIS

fi