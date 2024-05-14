#!/bin/sh

make_xzaa_grp.sh $1 $2 
make_xzac_grp.sh $1 $2
make_xzae_grp.sh $1 $2  
make_xzag_grp.sh $1 $2 
make_xzah_grp.sh $1 $2  
make_xzai_grp.sh $1 $2  
make_xzal_grp.sh $1 $2  
make_xzam_grp.sh $1 $2
make_xzan_grp.sh $1 $2  
make_xzao_grp.sh $1 $2
make_xzap_grp.sh $1 $2  
make_xzaq_grp.sh $1 $2
make_xzar_grp.sh $1 $2  
make_xzas_grp.sh $1 $2  
make_xzat_grp.sh $1 $2  
make_xzau_grp.sh $1 $2
make_xzaz_grp.sh $1 $2  


if [[ "$1" = "HIS" ]] 
then
	make_xzaq_grp.sh $1 $2  
fi

# compilation des trigger
#make_asql_grp.sh $1 $2 
