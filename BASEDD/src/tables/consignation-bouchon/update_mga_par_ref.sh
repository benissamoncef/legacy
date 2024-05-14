echo "
use CFG
go
insert into dbo.MGA_PAR_REF (site, cle, code, ordre, valeur, valeur2, valeur3, valeur4, valeur5, description, supprime) values ('ALL', 'VABOU.DOUBLON.RAYON_LIMITE', 'RAYON LIMITE', null, '500', null, null, null, null, null, 0)
go" | isql -Usa -Pzigramur -SSQL_CI
