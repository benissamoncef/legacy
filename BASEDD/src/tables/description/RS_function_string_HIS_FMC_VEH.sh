/* FMC_VEH */

echo "create function string CI_FMC_VEH.rs_insert for HIS_class
go

create function string CI_FMC_VEH.rs_select for HIS_class
go

create function string CI_FMC_VEH.rs_select_with_lock for HIS_class
go

create function string CI_FMC_VEH.rs_update for HIS_class
go

create function string CI_FMC_VEH.rs_delete for HIS_class
output language ' '
go" | isql -Usa -Pzigramur -SREP_CI

echo "create function string DP_FMC_VEH.rs_insert for HIS_class
go

create function string DP_FMC_VEH.rs_select for HIS_class
go

create function string DP_FMC_VEH.rs_select_with_lock for HIS_class
go

create function string DP_FMC_VEH.rs_update for HIS_class
go

create function string DP_FMC_VEH.rs_delete for HIS_class
output language ' '
go" | isql -Usa -Pzigramur -SREP_DP

echo "create function string DS_FMC_VEH.rs_insert for HIS_class
go

create function string DS_FMC_VEH.rs_select for HIS_class
go

create function string DS_FMC_VEH.rs_select_with_lock for HIS_class
go

create function string DS_FMC_VEH.rs_update for HIS_class
go

create function string DS_FMC_VEH.rs_delete for HIS_class
output language ' '
go" | isql -Usa -Pzigramur -SREP_DS

echo "create function string DC_FMC_VEH.rs_insert for HIS_class
go

create function string DC_FMC_VEH.rs_select for HIS_class
go

create function string DC_FMC_VEH.rs_select_with_lock for HIS_class
go

create function string DC_FMC_VEH.rs_update for HIS_class
go

create function string DC_FMC_VEH.rs_delete for HIS_class
output language ' '
go" | isql -Usa -Pzigramur -SREP_DC

echo "create function string DM_FMC_VEH.rs_insert for HIS_class
go

create function string DM_FMC_VEH.rs_select for HIS_class
go

create function string DM_FMC_VEH.rs_select_with_lock for HIS_class
go

create function string DM_FMC_VEH.rs_update for HIS_class
go

create function string DM_FMC_VEH.rs_delete for HIS_class
output language ' '
go" | isql -Usa -Pzigramur -SREP_DM

