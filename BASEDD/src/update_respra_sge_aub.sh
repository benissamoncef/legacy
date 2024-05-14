cd $HOME/exec
isql -SSQL_DS -t60 -w2000 -UMIGRAZUR -PMIGRAZUR -Jiso_1 -i update_respra_sge_aub.sql
isql -SSQL_DM -t60 -w2000 -UMIGRAZUR -PMIGRAZUR -Jiso_1 -i update_respra_sge_aub.sql

