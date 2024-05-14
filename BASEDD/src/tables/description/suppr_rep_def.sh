echo "drop replication definition DP_FMC_VEH
go"| isql -Usa -Pzigramur -SREP_DP
echo "drop replication definition DS_FMC_VEH
go"| isql -Usa -Pzigramur -SREP_DS
echo "drop replication definition DC_FMC_VEH
go"| isql -Usa -Pzigramur -SREP_DC
echo "drop replication definition DM_FMC_VEH
go"| isql -Usa -Pzigramur -SREP_DM
echo "drop replication definition CI_FMC_VEH
go"| isql -Usa -Pzigramur -SREP_CI

