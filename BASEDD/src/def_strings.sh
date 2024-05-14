#!/bin/ksh

for table in $1 $2 $3
do
echo "
create function string CI_$table.rs_insert for HIS_class
create function string CI_$table.rs_select for HIS_class
create function string CI_$table.rs_select_with_lock for HIS_class
create function string CI_$table.rs_update for HIS_class
create function string CI_$table.rs_delete for HIS_class
output language ' '
go" | isql -Usa -Pzigramur -SREP_CI
echo "
create function string DM_$table.rs_insert for HIS_class
create function string DM_$table.rs_select for HIS_class
create function string DM_$table.rs_select_with_lock for HIS_class
create function string DM_$table.rs_update for HIS_class
create function string DM_$table.rs_delete for HIS_class
output language ' '
go" | isql -Usa -Pzigramur -SREP_DM
echo "
create function string DS_$table.rs_insert for HIS_class
create function string DS_$table.rs_select for HIS_class
create function string DS_$table.rs_select_with_lock for HIS_class
create function string DS_$table.rs_update for HIS_class
create function string DS_$table.rs_delete for HIS_class
output language ' '
go" | isql -Usa -Pzigramur -SREP_DS
done
