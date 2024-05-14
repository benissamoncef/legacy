# Produit le descriptif de chaque table de EXP de chaque site et de CFG du CI.

for dist in CI DM DS 
do
echo "DESCRIPTION DES TABLES EXP $dist	`date`" > description_EXP_$dist.lis

for t in `echo "select name from EXP..sysobjects where type='U'
go" | isql -SSQL_$dist -w2000 -UMIGRAZUR -PMIGRAZUR | grep "_" | grep -v rs_ | sort`
do
echo table $dist $t 
echo "use EXP
go
sp_help $t
go"  | isql -SSQL_$dist -w2000 -UMIGRAZUR -PMIGRAZUR > $t-$dist.lis
print  "^L" >>description_EXP_$dist.lis
echo table $t $dist >>description_EXP_$dist.lis
cat $t-$dist.lis >>description_EXP_$dist.lis
done

echo "DESCRIPTION DES TABLES CFG $dist	`date`" > description_CFG_$dist.lis
for t in `echo "select name from CFG..sysobjects where type='U'
go" | isql -SSQL_$dist -w2000 -UMIGRAZUR -PMIGRAZUR | grep "_" | grep -v rs_ | sort`
do
echo table $dist $t 
echo "use CFG
go
sp_help $t
go"  | isql -SSQL_$dist -w2000 -UMIGRAZUR -PMIGRAZUR > $t-$dist.lis
print  "^L" >>description_CFG_$dist.lis
echo table $t $dist >>description_CFG_$dist.lis
cat $t-$dist.lis >>description_CFG_$dist.lis

done
done


for dist in HIS 
do
echo "DESCRIPTION DES TABLES HIS $dist  `date`" > description_$dist.lis
for t in `echo "select name from HIS..sysobjects where type='U'
go" | isql -SSQL_$dist -w2000 -UMIGRAZUR -PMIGRAZUR | grep "_" | grep -v rs_ | sort`
do
echo table $dist $t 
echo "use HIS
go
sp_help $t
go"  | isql -SSQL_$dist -w2000 -UMIGRAZUR -PMIGRAZUR > $t-$dist.lis
print  "^L" >>description_$dist.lis
echo table $t $dist >>description_$dist.lis
cat $t-$dist.lis >>description_$dist.lis
done
done

dist="CI"
echo "DESCRIPTION DES TABLES CFG $dist	`date`" > description_CFG_$dist.lis
for t in `echo "select name from CFG..sysobjects where type='U'
go" | isql -SSQL_$dist -w2000 -UMIGRAZUR -PMIGRAZUR | grep "_" | grep -v rs_ | sort`
do
echo table $dist $t
echo "use CFG
go
sp_help $t
go"  | isql -SSQL_$dist -w2000 -UMIGRAZUR -PMIGRAZUR > $t-$dist.lis
print  "^L" >>description_CFG_$dist.lis
echo table $dist $t >>description_CFG_$dist.lis
cat $t-$dist.lis >>description_CFG_$dist.lis
done
