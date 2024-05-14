#!/bin/ksh

echo "
use EXP
go
alter table FMC_VEH add feu_maitrise tinyint null
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_CI
echo "
use EXP
go
alter table FMC_VEH add feu_maitrise tinyint null
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_DM
echo "
use EXP
go
alter table FMC_VEH add feu_maitrise tinyint null
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_DC
echo "
use EXP
go
alter table FMC_VEH add feu_maitrise tinyint null
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_DS
echo "
use HIS
go
alter table FMC_VEH add feu_maitrise tinyint null
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_HIS
echo "
use EXP
go
alter table FMC_VEH add feu_maitrise tinyint null
go" | isql -UMIGRAZUR -PMIGRAZUR -SSQL_DP
