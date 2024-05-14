echo "suppression rep server DA"

echo "sysadmin droprs, REP_DA
go " | isql -Usa -Pzigramur -SREP_CI

echo "sysadmin dropdb, SQL_DA, EXP
go " | isql -Usa -Pzigramur -SREP_CI

echo "sysadmin dropdb, SQL_DA, CFG
go " | isql -Usa -Pzigramur -SREP_CI

echo "sysadmin dropdb, SQL_DA, DA_RSSD
go " | isql -Usa -Pzigramur -SREP_CI

