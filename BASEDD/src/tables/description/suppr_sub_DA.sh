cat description_REP_DA.lis | while read sub sql base
do
if [[ $base != "" ]] 
then
	suppr_sub.sh $base $sub $sql
fi
done > suppr.lis
