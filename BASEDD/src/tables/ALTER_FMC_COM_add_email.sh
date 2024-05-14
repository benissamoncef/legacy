if [[ "$1" = "HIS" ]]
then
    exp="HIS"
else
    exp="EXP"
fi


echo "
use $exp
go
alter table FMC_COM add email varchar(255) null
go" | isql -Usa -Pzigramur -SSQL_$1