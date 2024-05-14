#!/bin/ksh
# Verification sur quelle base on pointe
( ping -c 1 sdci1| grep SDCI ) > /dev/null 
if [ $? -eq 0 ]; then
# BDD Escota
echo "Escota"

echo "
use EXP
go
alter table SAD_PMV modify bandeau char(12)
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_CI

echo "
use EXP
go
alter table SAD_PMV modify bandeau char(12)
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_DS

echo "
use EXP
go
alter table SAD_PMV modify bandeau char(12)
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_DM

###############################

echo "
use EXP
go
alter table SAD_PMV modify picto char(10)
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_CI

echo "
use EXP
go
alter table SAD_PMV modify picto char(10)
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_DS

echo "
use EXP
go
alter table SAD_PMV modify picto char(10)
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_DM

###############################

echo "
use EXP
go
alter table SAD_PMV add flash bit default 0
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_CI

echo "
use EXP
go
alter table SAD_PMV add flash bit default 0
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_DS

echo "
use EXP
go
alter table SAD_PMV add flash bit default 0
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_DM

else
# BDD CNA
echo "CNA"

echo "
use EXP
go
alter table SAD_PMV modify bandeau char(12)
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_CA

echo "
use EXP
go
alter table SAD_PMV modify picto char(10)
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_CA

echo "
use EXP
go
alter table SAD_PMV add flash bit default 0
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_CA

fi

