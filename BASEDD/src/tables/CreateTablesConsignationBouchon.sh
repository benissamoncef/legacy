if (($#!=1))
then
	echo "syntaxe : UpdateToConsignation.sh Site"
	echo "          Site : CI DM DP HIS"
	exit 0
fi

if [[ "$1" = "HIS" ]]
then
    exp="his"
else
    exp="exp"
fi

if [[ "$1" != "HIS" ]]
then
	table.sh $exp ref_pit_message $1
fi

table.sh $exp ref_alt_pit_acqt $1

table.sh $exp fmc_ref_ext $1

table.sh $exp pit_log $1

table.sh cfg res_ref_va $1
