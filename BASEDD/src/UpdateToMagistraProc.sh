if (($#<2))
then
	echo "syntaxe : mga_par_ref.sh Site | cible"
	echo "     Site : CI DM DP"
	echo "     Cible : DEV | num jira"
	exit 0
fi

#Magistra
sae_liv $2 -p xzaa08sp.prc $1

sae_liv $2 -p xzac82sp.prc $1

sae_liv $2 -p xzae02sp.prc $1

sae_liv $2 -p xzae81sp.prc $1

sae_liv $2 -p xzae82sp.prc $1

#modif ESCOTA xzap04
sae_liv $2 -p make_xzap_grp.sh $1

#Magistra syno xzac100, xzac101, xzac631, xzac632, xzao1240
#présence d'un defined xzac100, xzac101, xzac631, xzac632
sae_liv $2 -p make_xzac_grp.sh $1

sae_liv $2 -p xzao1240sp.prc $1

#syn a rajouter sur les districts
sae_liv $2 -p xzao1270sp.prc $1

sae_liv $2 -p xzao1490sp.prc $1

sae_liv $2 -p xzao1481sp.prc $1

sae_liv $2 -p xzao1250sp.prc $1

sae_liv $2 -p xzao1280sp.prc $1

#Ajout du drop if exist
sae_liv $2 -p xzag1300sp.prc $1

sae_liv $2 -p xzag1301sp.prc $1

#syn
sae_liv $2 -p xzao1270sp.prc $1

sae_liv $2 -p xzao1490sp.prc $1

sae_liv $2 -p xzao1481sp.prc $1

sae_liv $2 -p xzao1250sp.prc $1

sae_liv $2 -p xzao1280sp.prc $1

#Fix compil pour xzae10 FMC en prévision
sae_liv $2 -p make_xzae_grp.sh $1