#!/bin/ksh

echo "
use HIS
go
alter table FMC_ACC add cond_meteo	smallint null,
			cond_visibilite	smallint null,
			cond_etat_chaus	smallint null,
			cond_chantier	smallint null
go" | isql -Usa -Pzigramur -SSQL_HIS


echo "
use EXP
go
alter table FMC_ACC add cond_meteo	smallint null,
			cond_visibilite	smallint null,
			cond_etat_chaus	smallint null,
			cond_chantier	smallint null
go" | isql -Usa -Pzigramur -SSQL_CI


echo "
use EXP
go
alter table FMC_ACC add cond_meteo	smallint null,
			cond_visibilite	smallint null,
			cond_etat_chaus	smallint null,
			cond_chantier	smallint null
go" | isql -Usa -Pzigramur -SSQL_CA


echo "
use EXP
go
alter table FMC_ACC add cond_meteo	smallint null,
			cond_visibilite	smallint null,
			cond_etat_chaus	smallint null,
			cond_chantier	smallint null
go" | isql -Usa -Pzigramur -SSQL_VE


echo "
use EXP
go
alter table FMC_ACC add cond_meteo	smallint null,
			cond_visibilite	smallint null,
			cond_etat_chaus	smallint null,
			cond_chantier	smallint null
go" | isql -Usa -Pzigramur -SSQL_PR


echo "
use EXP
go
alter table FMC_ACC add cond_meteo	smallint null,
			cond_visibilite	smallint null,
			cond_etat_chaus	smallint null,
			cond_chantier	smallint null
go" | isql -Usa -Pzigramur -SSQL_VD
