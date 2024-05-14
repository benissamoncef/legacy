cat description_REP_DA.lis| grep _DA_ | while read sub sql base
do
if [[ $base != "" ]] 
then
	suppr_rep.sh $base $sub $sql
fi
done > suppr_rep.lis
