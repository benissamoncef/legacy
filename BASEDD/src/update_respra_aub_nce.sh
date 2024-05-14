cd $HOME/exec
isql -SSQL_CI -t60 -w2000 -UMIGRAZUR -PMIGRAZUR -Jiso_1 -i update_respra_aub_nce.sql
isql -SSQL_DM -t60 -w2000 -UMIGRAZUR -PMIGRAZUR -Jiso_1 -i update_respra_aub_nce.sql

