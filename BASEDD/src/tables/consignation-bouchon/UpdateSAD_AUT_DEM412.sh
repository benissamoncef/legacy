echo "
use CFG
go
insert into CFG..SAD_AUT (autoroute, sens, libelle_autoroute, libelle_sens, PR_debut, PR_fin) values(9,1,'SUR L AUTOROUTE A7','EN DIRECTION DE MARSEILLE', 246245, 258510)
insert into CFG..SAD_AUT (autoroute, sens, libelle_autoroute, libelle_sens, PR_debut, PR_fin) values(9,2,'SUR L AUTOROUTE A7','EN DIRECTION DE LYON', 246245, 233980)
go" | isql -Usa -Pzigramur -SSQL_CI
