for t in `echo "select name from CFG..sysobjects where type='U'
go" | isql -SSQL_DM  -w2000 -UMIGRAZUR -PMIGRAZUR | grep "_" | grep -v rs_ | sort`
do
echo $t
echo "use CFG
go
alter table $t lock datarows
go 
update index statistics $t with hashing
go" | isql -SSQL_DM  -w2000 -UMIGRAZUR -PMIGRAZUR
done

echo "EXP"
for t in `echo "select name from EXP..sysobjects where type='U' 
go" | isql -SSQL_DM  -w2000 -UMIGRAZUR -PMIGRAZUR | grep "_" | grep -v rs_ | sort`
do
echo $t
echo "use EXP
go
alter table $t lock datarows
go 
update index statistics $t with hashing
go" | isql -SSQL_DM  -w2000 -UMIGRAZUR -PMIGRAZUR
done




