if (($#!=1))
then
	echo "syntaxe : mga_par_ref.sh Site"
	echo "          Site : CI DM DP"
	exit 0
fi

if [[ "$1" = "HIS" ]]
then
    exp="his"
else
    exp="exp"
fi

table.sh $exp alt_cau_his $1

table.sh cfg eqt_loc $1

table.sh $exp mga_cfg_po $1

table.sh $exp mga_msg_po $1

table.sh $exp mga_msg_prg $1

table.sh cfg mga_par_ref $1

table.sh cfg opr_ide $1

table.sh cfg res_bas $1

table.sh cfg res_sec $1

table.sh cfg res_sec_por $1

table.sh cfg syn_lib_flt $1

table.sh cfg syn_pse $1

table.sh cfg syn_ptc_loc $1