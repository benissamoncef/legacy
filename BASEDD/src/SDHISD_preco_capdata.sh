for t in `echo "select name from HIS..sysobjects where type='U' 
go" | isql -SSQL_HIS  -w2000 -UMIGRAZUR -PMIGRAZUR | grep "_" | grep -v rs_ | sort`
do
echo $t
echo "use HIS
go
alter table $t lock datarows
go 
update index statistics $t with hashing
go" | isql -SSQL_HIS  -w2000 -UMIGRAZUR -PMIGRAZUR
done
