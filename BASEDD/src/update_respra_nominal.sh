cd $HOME/exec
isql -SSQL_CI -t60 -w2000 -UMIGRAZUR -PMIGRAZUR -Jiso_1 -i update_respra_nominal.sql
isql -SSQL_DS -t60 -w2000 -UMIGRAZUR -PMIGRAZUR -Jiso_1 -i update_respra_nominal.sql
isql -SSQL_DM -t60 -w2000 -UMIGRAZUR -PMIGRAZUR -Jiso_1 -i update_respra_nominal.sql

