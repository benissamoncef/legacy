#!/bin/ksh

echo "
use CFG
go
insert into dbo.MGA_PAR_REF (site, cle, code, ordre, valeur, valeur2, valeur3, valeur4, valeur5, description, supprime) values ('ALL', 'DASHBOARD.CATEGORIE_FMC', 'ACC/INC', 8, 'Accidents / Incidents', null, null, null, null, null, 0)
update dbo.MGA_PAR_REF set supprime = 1 where (code = 'DEPANNAGE' or code = 'COURTE_DUREE') and cle = 'DASHBOARD.CATEGORIE_FMC'
update dbo.MGA_PAR_REF set valeur2 = 'ACC/INC' where (valeur2 = 'DEPANNAGE' or valeur2 = 'COURTE_DUREE') and cle = 'DASHBOARD.FMC_TYPE'
insert into dbo.MGA_PAR_REF (site, cle, code, ordre, valeur, valeur2, valeur3, valeur4, valeur5, description, supprime) values ('ALL', 'DASHBOARD.FMC_FILTER', 'FMC_FILTER_ACC/INC', 13, 'ACC/INC', 'CATEGORY', null, null, null, null, 0)
update dbo.MGA_PAR_REF set supprime = 1 where (valeur = 'DEPANNAGE' or valeur = 'COURTE_DUREE') and cle = 'DASHBOARD.FMC_FILTER'
go" | isql -Usa -Pzigramur -SSQL_CI
