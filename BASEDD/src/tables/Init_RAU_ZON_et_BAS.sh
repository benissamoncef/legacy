#!/bin/sh
# Verification sur quelle base on pointe
( ping -c 1 sdci1| grep SDCI ) > /dev/null 
if [ $? -eq 0 ]; then
# BDD Escota
echo "Escota"

else
# BDD CNA
echo "CNA"

echo "
use CFG
go
insert into ZON_RAU values ('Z_CNA_A11', 101, 264950, 265950)
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_CA

echo "
use EXP
go
insert into BAS_RAU values ('Z_CNA_A11', 'A11', 'LOC', 15)
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_CA

fi
