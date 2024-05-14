cd $HOME/exec
isql -SSQL_CI -t60 -w2000 -UMIGRAZUR -PMIGRAZUR -Jiso_1 -i update_respra_nce_sge.sql
isql -SSQL_DS -t60 -w2000 -UMIGRAZUR -PMIGRAZUR -Jiso_1 -i update_respra_nce_sge.sql

