#!/bin/ksh

echo "
use CFG
go
update dbo.MGA_PAR_REF set ordre = 1 where code = 'TRAFIC' and cle = 'DASHBOARD.CATEGORIE_FMC'
update dbo.MGA_PAR_REF set ordre = 2 where code = 'TRAVAUX' and cle = 'DASHBOARD.CATEGORIE_FMC'
update dbo.MGA_PAR_REF set ordre = 3 where code = 'ACC/INC' and cle = 'DASHBOARD.CATEGORIE_FMC'
update dbo.MGA_PAR_REF set ordre = 4 where code = 'VIABILITE' and cle = 'DASHBOARD.CATEGORIE_FMC'
update dbo.MGA_PAR_REF set ordre = 5 where code = 'CME_MEX' and cle = 'DASHBOARD.CATEGORIE_FMC'
update dbo.MGA_PAR_REF set ordre = 6 where code = 'AUTRES' and cle = 'DASHBOARD.CATEGORIE_FMC'

update dbo.MGA_PAR_REF set valeur2 = 'AUTRES' where valeur = 'Hold-up' and cle = 'DASHBOARD.FMC_TYPE'
update dbo.MGA_PAR_REF set ordre = 5 where valeur = 'Hold-up' and cle = 'DASHBOARD.FMC_TYPE'

update dbo.MGA_PAR_REF set ordre = 1 where valeur = 'Accident' and cle = 'DASHBOARD.FMC_TYPE'
update dbo.MGA_PAR_REF set ordre = 2 where valeur = 'Contre-sens' and cle = 'DASHBOARD.FMC_TYPE'
update dbo.MGA_PAR_REF set ordre = 3 where valeur like '%hicule en feu' and cle = 'DASHBOARD.FMC_TYPE'
update dbo.MGA_PAR_REF set ordre = 4 where valeur like '%hicule en panne' and cle = 'DASHBOARD.FMC_TYPE'
update dbo.MGA_PAR_REF set ordre = 5 where valeur like '%hicule arr%' and cle = 'DASHBOARD.FMC_TYPE'
update dbo.MGA_PAR_REF set ordre = 6 where valeur like 'Obstacle sur chauss%' and cle = 'DASHBOARD.FMC_TYPE'
update dbo.MGA_PAR_REF set ordre = 7 where valeur = 'Animal errant' and cle = 'DASHBOARD.FMC_TYPE'
update dbo.MGA_PAR_REF set ordre = 8 where valeur = '%ton sur chauss%' and cle = 'DASHBOARD.FMC_TYPE'
update dbo.MGA_PAR_REF set ordre = 9 where valeur = 'Agression' and cle = 'DASHBOARD.FMC_TYPE'
update dbo.MGA_PAR_REF set ordre = 10 where valeur = 'Altercation usager' and cle = 'DASHBOARD.FMC_TYPE'
update dbo.MGA_PAR_REF set ordre = 11 where valeur like '%hicule lent' and cle = 'DASHBOARD.FMC_TYPE'
update dbo.MGA_PAR_REF set ordre = 12 where valeur = 'Manifestation' and cle = 'DASHBOARD.FMC_TYPE'
update dbo.MGA_PAR_REF set ordre = 13 where valeur = 'Cirque' and cle = 'DASHBOARD.FMC_TYPE'
update dbo.MGA_PAR_REF set ordre = 14 where valeur like '%hicule en marche arri%' and cle = 'DASHBOARD.FMC_TYPE'
update dbo.MGA_PAR_REF set ordre = 15 where valeur = 'Convoi exceptionnel' and cle = 'DASHBOARD.FMC_TYPE'
update dbo.MGA_PAR_REF set ordre = 16 where valeur = 'Convoi militaire' and cle = 'DASHBOARD.FMC_TYPE'
update dbo.MGA_PAR_REF set ordre = 17 where valeur = 'Malaise usager' and cle = 'DASHBOARD.FMC_TYPE'
update dbo.MGA_PAR_REF set ordre = 18 where valeur like '%ration escargot' and cle = 'DASHBOARD.FMC_TYPE'
go" | isql -Usa -Pzigramur -SSQL_CI
