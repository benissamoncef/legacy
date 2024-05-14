#!/bin/ksh

echo "
use EXP
go
alter table PRG_PMV add type_jour   tinyint default 0 not null,
                        heure_debut tinyint default 0     null,
                        heure_fin   tinyint default 0     null,
                        discontinu  bit     default 0 not null
go" | isql -Usa -Pzigramur -SSQL_DC


echo "
use EXP
go
alter table PRG_PMV add type_jour   tinyint default 0 not null,
                        heure_debut tinyint default 0     null,
                        heure_fin   tinyint default 0     null,
                        discontinu  bit     default 0 not null
go" | isql -Usa -Pzigramur -SSQL_CI


echo "
use EXP
go
alter table PRG_PMV add type_jour   tinyint default 0 not null,
                        heure_debut tinyint default 0     null,
                        heure_fin   tinyint default 0     null,
                        discontinu  bit     default 0 not null
go" | isql -Usa -Pzigramur -SSQL_DS


echo "
use EXP
go
alter table PRG_PMV add type_jour   tinyint default 0 not null,
                        heure_debut tinyint default 0     null,
                        heure_fin   tinyint default 0     null,
                        discontinu  bit     default 0 not null
go" | isql -Usa -Pzigramur -SSQL_DM

