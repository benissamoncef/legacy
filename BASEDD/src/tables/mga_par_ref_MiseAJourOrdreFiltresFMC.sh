#!/bin/ksh

echo "
use CFG
go
update dbo.MGA_PAR_REF set ordre = 1 where valeur = 'REVEILS' and cle = 'DASHBOARD.FMC_FILTER'
update dbo.MGA_PAR_REF set ordre = 2 where valeur = 'ACC/INC' and cle = 'DASHBOARD.FMC_FILTER'
update dbo.MGA_PAR_REF set ordre = 3 where valeur = 'TRAFIC' and cle = 'DASHBOARD.FMC_FILTER'
update dbo.MGA_PAR_REF set ordre = 4 where valeur = 'TRAVAUX' and cle = 'DASHBOARD.FMC_FILTER'
update dbo.MGA_PAR_REF set ordre = 5 where valeur = 'CME_MEX' and cle = 'DASHBOARD.FMC_FILTER'
update dbo.MGA_PAR_REF set ordre = 6 where valeur = 'VIABILITE' and cle = 'DASHBOARD.FMC_FILTER'
update dbo.MGA_PAR_REF set ordre = 7 where valeur = 'AUTRES' and cle = 'DASHBOARD.FMC_FILTER'
update dbo.MGA_PAR_REF set ordre = 8 where valeur = 'DR_VOISINES' and cle = 'DASHBOARD.FMC_FILTER'
update dbo.MGA_PAR_REF set ordre = 9 where valeur = 'TERMINEES' and cle = 'DASHBOARD.FMC_FILTER'
update dbo.MGA_PAR_REF set ordre = 10 where valeur = 'OPERATEUR' and cle = 'DASHBOARD.FMC_FILTER'
update dbo.MGA_PAR_REF set ordre = 11 where valeur = 'TOUS' and cle = 'DASHBOARD.FMC_FILTER'
go" | isql -Usa -Pzigramur -SSQL_CI
